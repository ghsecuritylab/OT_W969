

#include "includes.h"

#include "common.h"
#include "aes_wrap.h"
#include "crypto.h"

#ifndef CONFIG_NO_AES_WRAP

int aes_wrap(const u8 *kek, int n, const u8 *plain, u8 *cipher)
{
	u8 *a, *r, b[16];
	int i, j;
	void *ctx;

	a = cipher;
	r = cipher + 8;

	/* 1) Initialize variables. */
	os_memset(a, 0xa6, 8);
	os_memcpy(r, plain, 8 * n);

	ctx = aes_encrypt_init(kek, 16);
	if (ctx == NULL)
		return -1;

	/* 2) Calculate intermediate values.
	 * For j = 0 to 5
	 *     For i=1 to n
	 *         B = AES(K, A | R[i])
	 *         A = MSB(64, B) ^ t where t = (n*j)+i
	 *         R[i] = LSB(64, B)
	 */
	for (j = 0; j <= 5; j++) {
		r = cipher + 8;
		for (i = 1; i <= n; i++) {
			os_memcpy(b, a, 8);
			os_memcpy(b + 8, r, 8);
			aes_encrypt(ctx, b, b);
			os_memcpy(a, b, 8);
			a[7] ^= n * j + i;
			os_memcpy(r, b + 8, 8);
			r += 8;
		}
	}
	aes_encrypt_deinit(ctx);

	/* 3) Output the results.
	 *
	 * These are already in @cipher due to the location of temporary
	 * variables.
	 */

	return 0;
}

#endif /* CONFIG_NO_AES_WRAP */


#ifndef CONFIG_NO_AES_UNWRAP

int aes_unwrap(const u8 *kek, int n, const u8 *cipher, u8 *plain)
{
	u8 a[8], *r, b[16];
	int i, j;
	void *ctx;

	/* 1) Initialize variables. */
	os_memcpy(a, cipher, 8);
	r = plain;
	os_memcpy(r, cipher + 8, 8 * n);

	ctx = aes_decrypt_init(kek, 16);
	if (ctx == NULL)
		return -1;

	/* 2) Compute intermediate values.
	 * For j = 5 to 0
	 *     For i = n to 1
	 *         B = AES-1(K, (A ^ t) | R[i]) where t = n*j+i
	 *         A = MSB(64, B)
	 *         R[i] = LSB(64, B)
	 */
	for (j = 5; j >= 0; j--) {
		r = plain + (n - 1) * 8;
		for (i = n; i >= 1; i--) {
			os_memcpy(b, a, 8);
			b[7] ^= n * j + i;

			os_memcpy(b + 8, r, 8);
			aes_decrypt(ctx, b, b);
			os_memcpy(a, b, 8);
			os_memcpy(r, b + 8, 8);
			r -= 8;
		}
	}
	aes_decrypt_deinit(ctx);

	/* 3) Output results.
	 *
	 * These are already in @plain due to the location of temporary
	 * variables. Just verify that the IV matches with the expected value.
	 */
	for (i = 0; i < 8; i++) {
		if (a[i] != 0xa6)
			return -1;
	}

	return 0;
}

#endif /* CONFIG_NO_AES_UNWRAP */


#define BLOCK_SIZE 16

#ifndef CONFIG_NO_AES_OMAC1

static void gf_mulx(u8 *pad)
{
	int i, carry;

	carry = pad[0] & 0x80;
	for (i = 0; i < BLOCK_SIZE - 1; i++)
		pad[i] = (pad[i] << 1) | (pad[i + 1] >> 7);
	pad[BLOCK_SIZE - 1] <<= 1;
	if (carry)
		pad[BLOCK_SIZE - 1] ^= 0x87;
}


int omac1_aes_128_vector(const u8 *key, size_t num_elem,
			 const u8 *addr[], const size_t *len, u8 *mac)
{
	void *ctx;
	u8 cbc[BLOCK_SIZE], pad[BLOCK_SIZE];
	const u8 *pos, *end;
	size_t i, e, left, total_len;

	ctx = aes_encrypt_init(key, 16);
	if (ctx == NULL)
		return -1;
	os_memset(cbc, 0, BLOCK_SIZE);

	total_len = 0;
	for (e = 0; e < num_elem; e++)
		total_len += len[e];
	left = total_len;

	e = 0;
	pos = addr[0];
	end = pos + len[0];

	while (left >= BLOCK_SIZE) {
		for (i = 0; i < BLOCK_SIZE; i++) {
			cbc[i] ^= *pos++;
			if (pos >= end) {
				e++;
				pos = addr[e];
				end = pos + len[e];
			}
		}
		if (left > BLOCK_SIZE)
			aes_encrypt(ctx, cbc, cbc);
		left -= BLOCK_SIZE;
	}

	os_memset(pad, 0, BLOCK_SIZE);
	aes_encrypt(ctx, pad, pad);
	gf_mulx(pad);

	if (left || total_len == 0) {
		for (i = 0; i < left; i++) {
			cbc[i] ^= *pos++;
			if (pos >= end) {
				e++;
				pos = addr[e];
				end = pos + len[e];
			}
		}
		cbc[left] ^= 0x80;
		gf_mulx(pad);
	}

	for (i = 0; i < BLOCK_SIZE; i++)
		pad[i] ^= cbc[i];
	aes_encrypt(ctx, pad, mac);
	aes_encrypt_deinit(ctx);
	return 0;
}


int omac1_aes_128(const u8 *key, const u8 *data, size_t data_len, u8 *mac)
{
	return omac1_aes_128_vector(key, 1, &data, &data_len, mac);
}

#endif /* CONFIG_NO_AES_OMAC1 */


#ifndef CONFIG_NO_AES_ENCRYPT_BLOCK
int aes_128_encrypt_block(const u8 *key, const u8 *in, u8 *out)
{
	void *ctx;
	ctx = aes_encrypt_init(key, 16);
	if (ctx == NULL)
		return -1;
	aes_encrypt(ctx, in, out);
	aes_encrypt_deinit(ctx);
	return 0;
}
#endif /* CONFIG_NO_AES_ENCRYPT_BLOCK */


#ifndef CONFIG_NO_AES_CTR

int aes_128_ctr_encrypt(const u8 *key, const u8 *nonce,
			u8 *data, size_t data_len)
{
	void *ctx;
	size_t j, len, left = data_len;
	int i;
	u8 *pos = data;
	u8 counter[BLOCK_SIZE], buf[BLOCK_SIZE];

	ctx = aes_encrypt_init(key, 16);
	if (ctx == NULL)
		return -1;
	os_memcpy(counter, nonce, BLOCK_SIZE);

	while (left > 0) {
		aes_encrypt(ctx, counter, buf);

		len = (left < BLOCK_SIZE) ? left : BLOCK_SIZE;
		for (j = 0; j < len; j++)
			pos[j] ^= buf[j];
		pos += len;
		left -= len;

		for (i = BLOCK_SIZE - 1; i >= 0; i--) {
			counter[i]++;
			if (counter[i])
				break;
		}
	}
	aes_encrypt_deinit(ctx);
	return 0;
}

#endif /* CONFIG_NO_AES_CTR */


#ifndef CONFIG_NO_AES_EAX

int aes_128_eax_encrypt(const u8 *key, const u8 *nonce, size_t nonce_len,
			const u8 *hdr, size_t hdr_len,
			u8 *data, size_t data_len, u8 *tag)
{
	u8 *buf;
	size_t buf_len;
	u8 nonce_mac[BLOCK_SIZE], hdr_mac[BLOCK_SIZE], data_mac[BLOCK_SIZE];
	int i, ret = -1;

	if (nonce_len > data_len)
		buf_len = nonce_len;
	else
		buf_len = data_len;
	if (hdr_len > buf_len)
		buf_len = hdr_len;
	buf_len += 16;

	buf = os_malloc(buf_len);
	if (buf == NULL)
		return -1;

	os_memset(buf, 0, 15);

	buf[15] = 0;
	os_memcpy(buf + 16, nonce, nonce_len);
	if (omac1_aes_128(key, buf, 16 + nonce_len, nonce_mac))
		goto fail;

	buf[15] = 1;
	os_memcpy(buf + 16, hdr, hdr_len);
	if (omac1_aes_128(key, buf, 16 + hdr_len, hdr_mac))
		goto fail;

	if (aes_128_ctr_encrypt(key, nonce_mac, data, data_len))
		goto fail;
	buf[15] = 2;
	os_memcpy(buf + 16, data, data_len);
	if (omac1_aes_128(key, buf, 16 + data_len, data_mac))
		goto fail;

	for (i = 0; i < BLOCK_SIZE; i++)
		tag[i] = nonce_mac[i] ^ data_mac[i] ^ hdr_mac[i];

	ret = 0;
fail:
	os_free(buf);

	return ret;
}


int aes_128_eax_decrypt(const u8 *key, const u8 *nonce, size_t nonce_len,
			const u8 *hdr, size_t hdr_len,
			u8 *data, size_t data_len, const u8 *tag)
{
	u8 *buf;
	size_t buf_len;
	u8 nonce_mac[BLOCK_SIZE], hdr_mac[BLOCK_SIZE], data_mac[BLOCK_SIZE];
	int i;

	if (nonce_len > data_len)
		buf_len = nonce_len;
	else
		buf_len = data_len;
	if (hdr_len > buf_len)
		buf_len = hdr_len;
	buf_len += 16;

	buf = os_malloc(buf_len);
	if (buf == NULL)
		return -1;

	os_memset(buf, 0, 15);

	buf[15] = 0;
	os_memcpy(buf + 16, nonce, nonce_len);
	if (omac1_aes_128(key, buf, 16 + nonce_len, nonce_mac)) {
		os_free(buf);
		return -1;
	}

	buf[15] = 1;
	os_memcpy(buf + 16, hdr, hdr_len);
	if (omac1_aes_128(key, buf, 16 + hdr_len, hdr_mac)) {
		os_free(buf);
		return -1;
	}

	buf[15] = 2;
	os_memcpy(buf + 16, data, data_len);
	if (omac1_aes_128(key, buf, 16 + data_len, data_mac)) {
		os_free(buf);
		return -1;
	}

	os_free(buf);

	for (i = 0; i < BLOCK_SIZE; i++) {
		if (tag[i] != (nonce_mac[i] ^ data_mac[i] ^ hdr_mac[i]))
			return -2;
	}

	return aes_128_ctr_encrypt(key, nonce_mac, data, data_len);
}

#endif /* CONFIG_NO_AES_EAX */


#ifndef CONFIG_NO_AES_CBC

int aes_128_cbc_encrypt(const u8 *key, const u8 *iv, u8 *data, size_t data_len)
{
	void *ctx;
	u8 cbc[BLOCK_SIZE];
	u8 *pos = data;
	int i, j, blocks;

	ctx = aes_encrypt_init(key, 16);
	if (ctx == NULL)
		return -1;
	os_memcpy(cbc, iv, BLOCK_SIZE);

	blocks = data_len / BLOCK_SIZE;
	for (i = 0; i < blocks; i++) {
		for (j = 0; j < BLOCK_SIZE; j++)
			cbc[j] ^= pos[j];
		aes_encrypt(ctx, cbc, cbc);
		os_memcpy(pos, cbc, BLOCK_SIZE);
		pos += BLOCK_SIZE;
	}
	aes_encrypt_deinit(ctx);
	return 0;
}


int aes_128_cbc_decrypt(const u8 *key, const u8 *iv, u8 *data, size_t data_len)
{
	void *ctx;
	u8 cbc[BLOCK_SIZE], tmp[BLOCK_SIZE];
	u8 *pos = data;
	int i, j, blocks;

	ctx = aes_decrypt_init(key, 16);
	if (ctx == NULL)
		return -1;
	os_memcpy(cbc, iv, BLOCK_SIZE);

	blocks = data_len / BLOCK_SIZE;
	for (i = 0; i < blocks; i++) {
		os_memcpy(tmp, pos, BLOCK_SIZE);
		aes_decrypt(ctx, pos, pos);
		for (j = 0; j < BLOCK_SIZE; j++)
			pos[j] ^= cbc[j];
		os_memcpy(cbc, tmp, BLOCK_SIZE);
		pos += BLOCK_SIZE;
	}
	aes_decrypt_deinit(ctx);
	return 0;
}

#endif /* CONFIG_NO_AES_CBC */