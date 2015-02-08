

#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/clk.h>

#include <plat/control.h>
#include <plat/clkdev_omap.h>

#include "clock.h"
#include "clock44xx.h"
#include "cm.h"
#include "cm-regbits-44xx.h"
#include "prm.h"
#include "prm-regbits-44xx.h"

/* Root clocks */

static struct clk extalt_clkin_ck = {
	.name		= "extalt_clkin_ck",
	.rate		= 59000000,
	.ops		= &clkops_null,
};

static struct clk pad_clks_ck = {
	.name		= "pad_clks_ck",
	.rate		= 12000000,
	.ops		= &clkops_null,
};

static struct clk pad_slimbus_core_clks_ck = {
	.name		= "pad_slimbus_core_clks_ck",
	.rate		= 12000000,
	.ops		= &clkops_null,
};

static struct clk secure_32k_clk_src_ck = {
	.name		= "secure_32k_clk_src_ck",
	.rate		= 32768,
	.ops		= &clkops_null,
};

static struct clk slimbus_clk = {
	.name		= "slimbus_clk",
	.rate		= 12000000,
	.ops		= &clkops_null,
};

static struct clk sys_32k_ck = {
	.name		= "sys_32k_ck",
	.rate		= 32768,
	.ops		= &clkops_null,
};

static struct clk virt_12000000_ck = {
	.name		= "virt_12000000_ck",
	.ops		= &clkops_null,
	.rate		= 12000000,
};

static struct clk virt_13000000_ck = {
	.name		= "virt_13000000_ck",
	.ops		= &clkops_null,
	.rate		= 13000000,
};

static struct clk virt_16800000_ck = {
	.name		= "virt_16800000_ck",
	.ops		= &clkops_null,
	.rate		= 16800000,
};

static struct clk virt_19200000_ck = {
	.name		= "virt_19200000_ck",
	.ops		= &clkops_null,
	.rate		= 19200000,
};

static struct clk virt_26000000_ck = {
	.name		= "virt_26000000_ck",
	.ops		= &clkops_null,
	.rate		= 26000000,
};

static struct clk virt_27000000_ck = {
	.name		= "virt_27000000_ck",
	.ops		= &clkops_null,
	.rate		= 27000000,
};

static struct clk virt_38400000_ck = {
	.name		= "virt_38400000_ck",
	.ops		= &clkops_null,
	.rate		= 38400000,
};

static const struct clksel_rate div_1_0_rates[] = {
	{ .div = 1, .val = 0, .flags = RATE_IN_4430 },
	{ .div = 0 },
};

static const struct clksel_rate div_1_1_rates[] = {
	{ .div = 1, .val = 1, .flags = RATE_IN_4430 },
	{ .div = 0 },
};

static const struct clksel_rate div_1_2_rates[] = {
	{ .div = 1, .val = 2, .flags = RATE_IN_4430 },
	{ .div = 0 },
};

static const struct clksel_rate div_1_3_rates[] = {
	{ .div = 1, .val = 3, .flags = RATE_IN_4430 },
	{ .div = 0 },
};

static const struct clksel_rate div_1_4_rates[] = {
	{ .div = 1, .val = 4, .flags = RATE_IN_4430 },
	{ .div = 0 },
};

static const struct clksel_rate div_1_5_rates[] = {
	{ .div = 1, .val = 5, .flags = RATE_IN_4430 },
	{ .div = 0 },
};

static const struct clksel_rate div_1_6_rates[] = {
	{ .div = 1, .val = 6, .flags = RATE_IN_4430 },
	{ .div = 0 },
};

static const struct clksel_rate div_1_7_rates[] = {
	{ .div = 1, .val = 7, .flags = RATE_IN_4430 },
	{ .div = 0 },
};

static const struct clksel sys_clkin_sel[] = {
	{ .parent = &virt_12000000_ck, .rates = div_1_1_rates },
	{ .parent = &virt_13000000_ck, .rates = div_1_2_rates },
	{ .parent = &virt_16800000_ck, .rates = div_1_3_rates },
	{ .parent = &virt_19200000_ck, .rates = div_1_4_rates },
	{ .parent = &virt_26000000_ck, .rates = div_1_5_rates },
	{ .parent = &virt_27000000_ck, .rates = div_1_6_rates },
	{ .parent = &virt_38400000_ck, .rates = div_1_7_rates },
	{ .parent = NULL },
};

static struct clk sys_clkin_ck = {
	.name		= "sys_clkin_ck",
	.rate		= 38400000,
	.clksel		= sys_clkin_sel,
	.init		= &omap2_init_clksel_parent,
	.clksel_reg	= OMAP4430_CM_SYS_CLKSEL,
	.clksel_mask	= OMAP4430_SYS_CLKSEL_MASK,
	.ops		= &clkops_null,
	.recalc		= &omap2_clksel_recalc,
};

static struct clk utmi_phy_clkout_ck = {
	.name		= "utmi_phy_clkout_ck",
	.rate		= 12000000,
	.ops		= &clkops_null,
};

static struct clk xclk60mhsp1_ck = {
	.name		= "xclk60mhsp1_ck",
	.rate		= 12000000,
	.ops		= &clkops_null,
};

static struct clk xclk60mhsp2_ck = {
	.name		= "xclk60mhsp2_ck",
	.rate		= 12000000,
	.ops		= &clkops_null,
};

static struct clk xclk60motg_ck = {
	.name		= "xclk60motg_ck",
	.rate		= 60000000,
	.ops		= &clkops_null,
};

/* Module clocks and DPLL outputs */

static const struct clksel_rate div2_1to2_rates[] = {
	{ .div = 1, .val = 0, .flags = RATE_IN_4430 },
	{ .div = 2, .val = 1, .flags = RATE_IN_4430 },
	{ .div = 0 },
};

static const struct clksel dpll_sys_ref_clk_div[] = {
	{ .parent = &sys_clkin_ck, .rates = div2_1to2_rates },
	{ .parent = NULL },
};

static struct clk dpll_sys_ref_clk = {
	.name		= "dpll_sys_ref_clk",
	.parent		= &sys_clkin_ck,
	.clksel		= dpll_sys_ref_clk_div,
	.clksel_reg	= OMAP4430_CM_DPLL_SYS_REF_CLKSEL,
	.clksel_mask	= OMAP4430_CLKSEL_0_0_MASK,
	.ops		= &clkops_null,
	.recalc		= &omap2_clksel_recalc,
	.round_rate	= &omap2_clksel_round_rate,
	.set_rate	= &omap2_clksel_set_rate,
};

static const struct clksel abe_dpll_refclk_mux_sel[] = {
	{ .parent = &dpll_sys_ref_clk, .rates = div_1_0_rates },
	{ .parent = &sys_32k_ck, .rates = div_1_1_rates },
	{ .parent = NULL },
};

static struct clk abe_dpll_refclk_mux_ck = {
	.name		= "abe_dpll_refclk_mux_ck",
	.parent		= &dpll_sys_ref_clk,
	.clksel		= abe_dpll_refclk_mux_sel,
	.init		= &omap2_init_clksel_parent,
	.clksel_reg	= OMAP4430_CM_ABE_PLL_REF_CLKSEL,
	.clksel_mask	= OMAP4430_CLKSEL_0_0_MASK,
	.ops		= &clkops_null,
	.recalc		= &omap2_clksel_recalc,
};

/* DPLL_ABE */
static struct dpll_data dpll_abe_dd = {
	.mult_div1_reg	= OMAP4430_CM_CLKSEL_DPLL_ABE,
	.clk_bypass	= &sys_clkin_ck,
	.clk_ref	= &abe_dpll_refclk_mux_ck,
	.control_reg	= OMAP4430_CM_CLKMODE_DPLL_ABE,
	.modes		= (1 << DPLL_LOW_POWER_BYPASS) | (1 << DPLL_LOCKED),
	.autoidle_reg	= OMAP4430_CM_AUTOIDLE_DPLL_ABE,
	.idlest_reg	= OMAP4430_CM_IDLEST_DPLL_ABE,
	.mult_mask	= OMAP4430_DPLL_MULT_MASK,
	.div1_mask	= OMAP4430_DPLL_DIV_MASK,
	.enable_mask	= OMAP4430_DPLL_EN_MASK,
	.autoidle_mask	= OMAP4430_AUTO_DPLL_MODE_MASK,
	.idlest_mask	= OMAP4430_ST_DPLL_CLK_MASK,
	.max_multiplier	= OMAP4430_MAX_DPLL_MULT,
	.max_divider	= OMAP4430_MAX_DPLL_DIV,
	.min_divider	= 1,
};


static struct clk dpll_abe_ck = {
	.name		= "dpll_abe_ck",
	.parent		= &abe_dpll_refclk_mux_ck,
	.dpll_data	= &dpll_abe_dd,
	.init		= &omap2_init_dpll_parent,
	.ops		= &clkops_omap3_noncore_dpll_ops,
	.recalc		= &omap3_dpll_recalc,
	.round_rate	= &omap2_dpll_round_rate,
	.set_rate	= &omap3_noncore_dpll_set_rate,
};

static struct clk dpll_abe_m2x2_ck = {
	.name		= "dpll_abe_m2x2_ck",
	.parent		= &dpll_abe_ck,
	.ops		= &clkops_null,
	.recalc		= &followparent_recalc,
};

static struct clk abe_24m_fclk = {
	.name		= "abe_24m_fclk",
	.parent		= &dpll_abe_m2x2_ck,
	.ops		= &clkops_null,
	.recalc		= &followparent_recalc,
};

static const struct clksel_rate div3_1to4_rates[] = {
	{ .div = 1, .val = 0, .flags = RATE_IN_4430 },
	{ .div = 2, .val = 1, .flags = RATE_IN_4430 },
	{ .div = 4, .val = 2, .flags = RATE_IN_4430 },
	{ .div = 0 },
};

static const struct clksel abe_clk_div[] = {
	{ .parent = &dpll_abe_m2x2_ck, .rates = div3_1to4_rates },
	{ .parent = NULL },
};

static struct clk abe_clk = {
	.name		= "abe_clk",
	.parent		= &dpll_abe_m2x2_ck,
	.clksel		= abe_clk_div,
	.clksel_reg	= OMAP4430_CM_CLKSEL_ABE,
	.clksel_mask	= OMAP4430_CLKSEL_OPP_MASK,
	.ops		= &clkops_null,
	.recalc		= &omap2_clksel_recalc,
	.round_rate	= &omap2_clksel_round_rate,
	.set_rate	= &omap2_clksel_set_rate,
};

static const struct clksel aess_fclk_div[] = {
	{ .parent = &abe_clk, .rates = div2_1to2_rates },
	{ .parent = NULL },
};

static struct clk aess_fclk = {
	.name		= "aess_fclk",
	.parent		= &abe_clk,
	.clksel		= aess_fclk_div,
	.clksel_reg	= OMAP4430_CM1_ABE_AESS_CLKCTRL,
	.clksel_mask	= OMAP4430_CLKSEL_AESS_FCLK_MASK,
	.ops		= &clkops_null,
	.recalc		= &omap2_clksel_recalc,
	.round_rate	= &omap2_clksel_round_rate,
	.set_rate	= &omap2_clksel_set_rate,
};

static const struct clksel_rate div31_1to31_rates[] = {
	{ .div = 1, .val = 1, .flags = RATE_IN_4430 },
	{ .div = 2, .val = 2, .flags = RATE_IN_4430 },
	{ .div = 3, .val = 3, .flags = RATE_IN_4430 },
	{ .div = 4, .val = 4, .flags = RATE_IN_4430 },
	{ .div = 5, .val = 5, .flags = RATE_IN_4430 },
	{ .div = 6, .val = 6, .flags = RATE_IN_4430 },
	{ .div = 7, .val = 7, .flags = RATE_IN_4430 },
	{ .div = 8, .val = 8, .flags = RATE_IN_4430 },
	{ .div = 9, .val = 9, .flags = RATE_IN_4430 },
	{ .div = 10, .val = 10, .flags = RATE_IN_4430 },
	{ .div = 11, .val = 11, .flags = RATE_IN_4430 },
	{ .div = 12, .val = 12, .flags = RATE_IN_4430 },
	{ .div = 13, .val = 13, .flags = RATE_IN_4430 },
	{ .div = 14, .val = 14, .flags = RATE_IN_4430 },
	{ .div = 15, .val = 15, .flags = RATE_IN_4430 },
	{ .div = 16, .val = 16, .flags = RATE_IN_4430 },
	{ .div = 17, .val = 17, .flags = RATE_IN_4430 },
	{ .div = 18, .val = 18, .flags = RATE_IN_4430 },
	{ .div = 19, .val = 19, .flags = RATE_IN_4430 },
	{ .div = 20, .val = 20, .flags = RATE_IN_4430 },
	{ .div = 21, .val = 21, .flags = RATE_IN_4430 },
	{ .div = 22, .val = 22, .flags = RATE_IN_4430 },
	{ .div = 23, .val = 23, .flags = RATE_IN_4430 },
	{ .div = 24, .val = 24, .flags = RATE_IN_4430 },
	{ .div = 25, .val = 25, .flags = RATE_IN_4430 },
	{ .div = 26, .val = 26, .flags = RATE_IN_4430 },
	{ .div = 27, .val = 27, .flags = RATE_IN_4430 },
	{ .div = 28, .val = 28, .flags = RATE_IN_4430 },
	{ .div = 29, .val = 29, .flags = RATE_IN_4430 },
	{ .div = 30, .val = 30, .flags = RATE_IN_4430 },
	{ .div = 31, .val = 31, .flags = RATE_IN_4430 },
	{ .div = 0 },
};

static const struct clksel dpll_abe_m3_div[] = {
	{ .parent = &dpll_abe_ck, .rates = div31_1to31_rates },
	{ .parent = NULL },
};

static struct clk dpll_abe_m3_ck = {
	.name		= "dpll_abe_m3_ck",
	.parent		= &dpll_abe_ck,
	.clksel		= dpll_abe_m3_div,
	.clksel_reg	= OMAP4430_CM_DIV_M3_DPLL_ABE,
	.clksel_mask	= OMAP4430_DPLL_CLKOUTHIF_DIV_MASK,
	.ops		= &clkops_null,
	.recalc		= &omap2_clksel_recalc,
	.round_rate	= &omap2_clksel_round_rate,
	.set_rate	= &omap2_clksel_set_rate,
};

static const struct clksel core_hsd_byp_clk_mux_sel[] = {
	{ .parent = &dpll_sys_ref_clk, .rates = div_1_0_rates },
	{ .parent = &dpll_abe_m3_ck, .rates = div_1_1_rates },
	{ .parent = NULL },
};

static struct clk core_hsd_byp_clk_mux_ck = {
	.name		= "core_hsd_byp_clk_mux_ck",
	.parent		= &dpll_sys_ref_clk,
	.clksel		= core_hsd_byp_clk_mux_sel,
	.init		= &omap2_init_clksel_parent,
	.clksel_reg	= OMAP4430_CM_CLKSEL_DPLL_CORE,
	.clksel_mask	= OMAP4430_DPLL_BYP_CLKSEL_MASK,
	.ops		= &clkops_null,
	.recalc		= &omap2_clksel_recalc,
};

/* DPLL_CORE */
static struct dpll_data dpll_core_dd = {
	.mult_div1_reg	= OMAP4430_CM_CLKSEL_DPLL_CORE,
	.clk_bypass	= &core_hsd_byp_clk_mux_ck,
	.clk_ref	= &dpll_sys_ref_clk,
	.control_reg	= OMAP4430_CM_CLKMODE_DPLL_CORE,
	.modes		= (1 << DPLL_LOW_POWER_BYPASS) | (1 << DPLL_LOCKED),
	.autoidle_reg	= OMAP4430_CM_AUTOIDLE_DPLL_CORE,
	.idlest_reg	= OMAP4430_CM_IDLEST_DPLL_CORE,
	.mult_mask	= OMAP4430_DPLL_MULT_MASK,
	.div1_mask	= OMAP4430_DPLL_DIV_MASK,
	.enable_mask	= OMAP4430_DPLL_EN_MASK,
	.autoidle_mask	= OMAP4430_AUTO_DPLL_MODE_MASK,
	.idlest_mask	= OMAP4430_ST_DPLL_CLK_MASK,
	.max_multiplier	= OMAP4430_MAX_DPLL_MULT,
	.max_divider	= OMAP4430_MAX_DPLL_DIV,
	.min_divider	= 1,
};


static struct clk dpll_core_ck = {
	.name		= "dpll_core_ck",
	.parent		= &dpll_sys_ref_clk,
	.dpll_data	= &dpll_core_dd,
	.init		= &omap2_init_dpll_parent,
	.ops		= &clkops_null,
	.recalc		= &omap3_dpll_recalc,
};

static const struct clksel dpll_core_m6_div[] = {
	{ .parent = &dpll_core_ck, .rates = div31_1to31_rates },
	{ .parent = NULL },
};

static struct clk dpll_core_m6_ck = {
	.name		= "dpll_core_m6_ck",
	.parent		= &dpll_core_ck,
	.clksel		= dpll_core_m6_div,
	.clksel_reg	= OMAP4430_CM_DIV_M6_DPLL_CORE,
	.clksel_mask	= OMAP4430_HSDIVIDER_CLKOUT3_DIV_MASK,
	.ops		= &clkops_null,
	.recalc		= &omap2_clksel_recalc,
	.round_rate	= &omap2_clksel_round_rate,
	.set_rate	= &omap2_clksel_set_rate,
};

static const struct clksel dbgclk_mux_sel[] = {
	{ .parent = &sys_clkin_ck, .rates = div_1_0_rates },
	{ .parent = &dpll_core_m6_ck, .rates = div_1_1_rates },
	{ .parent = NULL },
};

static struct clk dbgclk_mux_ck = {
	.name		= "dbgclk_mux_ck",
	.parent		= &sys_clkin_ck,
	.ops		= &clkops_null,
	.recalc		= &followparent_recalc,
};

static struct clk dpll_core_m2_ck = {
	.name		= "dpll_core_m2_ck",
	.parent		= &dpll_core_ck,
	.clksel		= dpll_core_m6_div,
	.clksel_reg	= OMAP4430_CM_DIV_M2_DPLL_CORE,
	.clksel_mask	= OMAP4430_DPLL_CLKOUT_DIV_MASK,
	.ops		= &clkops_null,
	.recalc		= &omap2_clksel_recalc,
	.round_rate	= &omap2_clksel_round_rate,
	.set_rate	= &omap2_clksel_set_rate,
};

static struct clk ddrphy_ck = {
	.name		= "ddrphy_ck",
	.parent		= &dpll_core_m2_ck,
	.ops		= &clkops_null,
	.recalc		= &followparent_recalc,
};

static struct clk dpll_core_m5_ck = {
	.name		= "dpll_core_m5_ck",
	.parent		= &dpll_core_ck,
	.clksel		= dpll_core_m6_div,
	.clksel_reg	= OMAP4430_CM_DIV_M5_DPLL_CORE,
	.clksel_mask	= OMAP4430_HSDIVIDER_CLKOUT2_DIV_MASK,
	.ops		= &clkops_null,
	.recalc		= &omap2_clksel_recalc,
	.round_rate	= &omap2_clksel_round_rate,
	.set_rate	= &omap2_clksel_set_rate,
};

static const struct clksel div_core_div[] = {
	{ .parent = &dpll_core_m5_ck, .rates = div2_1to2_rates },
	{ .parent = NULL },
};

static struct clk div_core_ck = {
	.name		= "div_core_ck",
	.parent		= &dpll_core_m5_ck,
	.clksel		= div_core_div,
	.clksel_reg	= OMAP4430_CM_CLKSEL_CORE,
	.clksel_mask	= OMAP4430_CLKSEL_CORE_MASK,
	.ops		= &clkops_null,
	.recalc		= &omap2_clksel_recalc,
	.round_rate	= &omap2_clksel_round_rate,
	.set_rate	= &omap2_clksel_set_rate,
};

static const struct clksel_rate div4_1to8_rates[] = {
	{ .div = 1, .val = 0, .flags = RATE_IN_4430 },
	{ .div = 2, .val = 1, .flags = RATE_IN_4430 },
	{ .div = 4, .val = 2, .flags = RATE_IN_4430 },
	{ .div = 8, .val = 3, .flags = RATE_IN_4430 },
	{ .div = 0 },
};

static const struct clksel div_iva_hs_clk_div[] = {
	{ .parent = &dpll_core_m5_ck, .rates = div4_1to8_rates },
	{ .parent = NULL },
};

static struct clk div_iva_hs_clk = {
	.name		= "div_iva_hs_clk",
	.parent		= &dpll_core_m5_ck,
	.clksel		= div_iva_hs_clk_div,
	.clksel_reg	= OMAP4430_CM_BYPCLK_DPLL_IVA,
	.clksel_mask	= OMAP4430_CLKSEL_0_1_MASK,
	.ops		= &clkops_null,
	.recalc		= &omap2_clksel_recalc,
	.round_rate	= &omap2_clksel_round_rate,
	.set_rate	= &omap2_clksel_set_rate,
};

static struct clk div_mpu_hs_clk = {
	.name		= "div_mpu_hs_clk",
	.parent		= &dpll_core_m5_ck,
	.clksel		= div_iva_hs_clk_div,
	.clksel_reg	= OMAP4430_CM_BYPCLK_DPLL_MPU,
	.clksel_mask	= OMAP4430_CLKSEL_0_1_MASK,
	.ops		= &clkops_null,
	.recalc		= &omap2_clksel_recalc,
	.round_rate	= &omap2_clksel_round_rate,
	.set_rate	= &omap2_clksel_set_rate,
};

static struct clk dpll_core_m4_ck = {
	.name		= "dpll_core_m4_ck",
	.parent		= &dpll_core_ck,
	.clksel		= dpll_core_m6_div,
	.clksel_reg	= OMAP4430_CM_DIV_M4_DPLL_CORE,
	.clksel_mask	= OMAP4430_HSDIVIDER_CLKOUT1_DIV_MASK,
	.ops		= &clkops_null,
	.recalc		= &omap2_clksel_recalc,
	.round_rate	= &omap2_clksel_round_rate,
	.set_rate	= &omap2_clksel_set_rate,
};

static struct clk dll_clk_div_ck = {
	.name		= "dll_clk_div_ck",
	.parent		= &dpll_core_m4_ck,
	.ops		= &clkops_null,
	.recalc		= &followparent_recalc,
};

static struct clk dpll_abe_m2_ck = {
	.name		= "dpll_abe_m2_ck",
	.parent		= &dpll_abe_ck,
	.clksel		= dpll_abe_m3_div,
	.clksel_reg	= OMAP4430_CM_DIV_M2_DPLL_ABE,
	.clksel_mask	= OMAP4430_DPLL_CLKOUT_DIV_MASK,
	.ops		= &clkops_null,
	.recalc		= &omap2_clksel_recalc,
	.round_rate	= &omap2_clksel_round_rate,
	.set_rate	= &omap2_clksel_set_rate,
};

static struct clk dpll_core_m3_ck = {
	.name		= "dpll_core_m3_ck",
	.parent		= &dpll_core_ck,
	.clksel		= dpll_core_m6_div,
	.clksel_reg	= OMAP4430_CM_DIV_M3_DPLL_CORE,
	.clksel_mask	= OMAP4430_DPLL_CLKOUTHIF_DIV_MASK,
	.ops		= &clkops_null,
	.recalc		= &omap2_clksel_recalc,
	.round_rate	= &omap2_clksel_round_rate,
	.set_rate	= &omap2_clksel_set_rate,
};

static struct clk dpll_core_m7_ck = {
	.name		= "dpll_core_m7_ck",
	.parent		= &dpll_core_ck,
	.clksel		= dpll_core_m6_div,
	.clksel_reg	= OMAP4430_CM_DIV_M7_DPLL_CORE,
	.clksel_mask	= OMAP4430_HSDIVIDER_CLKOUT4_DIV_MASK,
	.ops		= &clkops_null,
	.recalc		= &omap2_clksel_recalc,
	.round_rate	= &omap2_clksel_round_rate,
	.set_rate	= &omap2_clksel_set_rate,
};

static const struct clksel iva_hsd_byp_clk_mux_sel[] = {
	{ .parent = &dpll_sys_ref_clk, .rates = div_1_0_rates },
	{ .parent = &div_iva_hs_clk, .rates = div_1_1_rates },
	{ .parent = NULL },
};

static struct clk iva_hsd_byp_clk_mux_ck = {
	.name		= "iva_hsd_byp_clk_mux_ck",
	.parent		= &dpll_sys_ref_clk,
	.ops		= &clkops_null,
	.recalc		= &followparent_recalc,
};

/* DPLL_IVA */
static struct dpll_data dpll_iva_dd = {
	.mult_div1_reg	= OMAP4430_CM_CLKSEL_DPLL_IVA,
	.clk_bypass	= &iva_hsd_byp_clk_mux_ck,
	.clk_ref	= &dpll_sys_ref_clk,
	.control_reg	= OMAP4430_CM_CLKMODE_DPLL_IVA,
	.modes		= (1 << DPLL_LOW_POWER_BYPASS) | (1 << DPLL_LOCKED),
	.autoidle_reg	= OMAP4430_CM_AUTOIDLE_DPLL_IVA,
	.idlest_reg	= OMAP4430_CM_IDLEST_DPLL_IVA,
	.mult_mask	= OMAP4430_DPLL_MULT_MASK,
	.div1_mask	= OMAP4430_DPLL_DIV_MASK,
	.enable_mask	= OMAP4430_DPLL_EN_MASK,
	.autoidle_mask	= OMAP4430_AUTO_DPLL_MODE_MASK,
	.idlest_mask	= OMAP4430_ST_DPLL_CLK_MASK,
	.max_multiplier	= OMAP4430_MAX_DPLL_MULT,
	.max_divider	= OMAP4430_MAX_DPLL_DIV,
	.min_divider	= 1,
};


static struct clk dpll_iva_ck = {
	.name		= "dpll_iva_ck",
	.parent		= &dpll_sys_ref_clk,
	.dpll_data	= &dpll_iva_dd,
	.init		= &omap2_init_dpll_parent,
	.ops		= &clkops_omap3_noncore_dpll_ops,
	.recalc		= &omap3_dpll_recalc,
	.round_rate	= &omap2_dpll_round_rate,
	.set_rate	= &omap3_noncore_dpll_set_rate,
};

static const struct clksel dpll_iva_m4_div[] = {
	{ .parent = &dpll_iva_ck, .rates = div31_1to31_rates },
	{ .parent = NULL },
};

static struct clk dpll_iva_m4_ck = {
	.name		= "dpll_iva_m4_ck",
	.parent		= &dpll_iva_ck,
	.clksel		= dpll_iva_m4_div,
	.clksel_reg	= OMAP4430_CM_DIV_M4_DPLL_IVA,
	.clksel_mask	= OMAP4430_HSDIVIDER_CLKOUT1_DIV_MASK,
	.ops		= &clkops_null,
	.recalc		= &omap2_clksel_recalc,
	.round_rate	= &omap2_clksel_round_rate,
	.set_rate	= &omap2_clksel_set_rate,
};

static struct clk dpll_iva_m5_ck = {
	.name		= "dpll_iva_m5_ck",
	.parent		= &dpll_iva_ck,
	.clksel		= dpll_iva_m4_div,
	.clksel_reg	= OMAP4430_CM_DIV_M5_DPLL_IVA,
	.clksel_mask	= OMAP4430_HSDIVIDER_CLKOUT2_DIV_MASK,
	.ops		= &clkops_null,
	.recalc		= &omap2_clksel_recalc,
	.round_rate	= &omap2_clksel_round_rate,
	.set_rate	= &omap2_clksel_set_rate,
};

/* DPLL_MPU */
static struct dpll_data dpll_mpu_dd = {
	.mult_div1_reg	= OMAP4430_CM_CLKSEL_DPLL_MPU,
	.clk_bypass	= &div_mpu_hs_clk,
	.clk_ref	= &dpll_sys_ref_clk,
	.control_reg	= OMAP4430_CM_CLKMODE_DPLL_MPU,
	.modes		= (1 << DPLL_LOW_POWER_BYPASS) | (1 << DPLL_LOCKED),
	.autoidle_reg	= OMAP4430_CM_AUTOIDLE_DPLL_MPU,
	.idlest_reg	= OMAP4430_CM_IDLEST_DPLL_MPU,
	.mult_mask	= OMAP4430_DPLL_MULT_MASK,
	.div1_mask	= OMAP4430_DPLL_DIV_MASK,
	.enable_mask	= OMAP4430_DPLL_EN_MASK,
	.autoidle_mask	= OMAP4430_AUTO_DPLL_MODE_MASK,
	.idlest_mask	= OMAP4430_ST_DPLL_CLK_MASK,
	.max_multiplier	= OMAP4430_MAX_DPLL_MULT,
	.max_divider	= OMAP4430_MAX_DPLL_DIV,
	.min_divider	= 1,
};


static struct clk dpll_mpu_ck = {
	.name		= "dpll_mpu_ck",
	.parent		= &dpll_sys_ref_clk,
	.dpll_data	= &dpll_mpu_dd,
	.init		= &omap2_init_dpll_parent,
	.ops		= &clkops_omap3_noncore_dpll_ops,
	.recalc		= &omap3_dpll_recalc,
	.round_rate	= &omap2_dpll_round_rate,
	.set_rate	= &omap3_noncore_dpll_set_rate,
};

static const struct clksel dpll_mpu_m2_div[] = {
	{ .parent = &dpll_mpu_ck, .rates = div31_1to31_rates },
	{ .parent = NULL },
};

static struct clk dpll_mpu_m2_ck = {
	.name		= "dpll_mpu_m2_ck",
	.parent		= &dpll_mpu_ck,
	.clksel		= dpll_mpu_m2_div,
	.clksel_reg	= OMAP4430_CM_DIV_M2_DPLL_MPU,
	.clksel_mask	= OMAP4430_DPLL_CLKOUT_DIV_MASK,
	.ops		= &clkops_null,
	.recalc		= &omap2_clksel_recalc,
	.round_rate	= &omap2_clksel_round_rate,
	.set_rate	= &omap2_clksel_set_rate,
};

static struct clk per_hs_clk_div_ck = {
	.name		= "per_hs_clk_div_ck",
	.parent		= &dpll_abe_m3_ck,
	.ops		= &clkops_null,
	.recalc		= &followparent_recalc,
};

static const struct clksel per_hsd_byp_clk_mux_sel[] = {
	{ .parent = &dpll_sys_ref_clk, .rates = div_1_0_rates },
	{ .parent = &per_hs_clk_div_ck, .rates = div_1_1_rates },
	{ .parent = NULL },
};

static struct clk per_hsd_byp_clk_mux_ck = {
	.name		= "per_hsd_byp_clk_mux_ck",
	.parent		= &dpll_sys_ref_clk,
	.clksel		= per_hsd_byp_clk_mux_sel,
	.init		= &omap2_init_clksel_parent,
	.clksel_reg	= OMAP4430_CM_CLKSEL_DPLL_PER,
	.clksel_mask	= OMAP4430_DPLL_BYP_CLKSEL_MASK,
	.ops		= &clkops_null,
	.recalc		= &omap2_clksel_recalc,
};

/* DPLL_PER */
static struct dpll_data dpll_per_dd = {
	.mult_div1_reg	= OMAP4430_CM_CLKSEL_DPLL_PER,
	.clk_bypass	= &per_hsd_byp_clk_mux_ck,
	.clk_ref	= &dpll_sys_ref_clk,
	.control_reg	= OMAP4430_CM_CLKMODE_DPLL_PER,
	.modes		= (1 << DPLL_LOW_POWER_BYPASS) | (1 << DPLL_LOCKED),
	.autoidle_reg	= OMAP4430_CM_AUTOIDLE_DPLL_PER,
	.idlest_reg	= OMAP4430_CM_IDLEST_DPLL_PER,
	.mult_mask	= OMAP4430_DPLL_MULT_MASK,
	.div1_mask	= OMAP4430_DPLL_DIV_MASK,
	.enable_mask	= OMAP4430_DPLL_EN_MASK,
	.autoidle_mask	= OMAP4430_AUTO_DPLL_MODE_MASK,
	.idlest_mask	= OMAP4430_ST_DPLL_CLK_MASK,
	.max_multiplier	= OMAP4430_MAX_DPLL_MULT,
	.max_divider	= OMAP4430_MAX_DPLL_DIV,
	.min_divider	= 1,
};


static struct clk dpll_per_ck = {
	.name		= "dpll_per_ck",
	.parent		= &dpll_sys_ref_clk,
	.dpll_data	= &dpll_per_dd,
	.init		= &omap2_init_dpll_parent,
	.ops		= &clkops_omap3_noncore_dpll_ops,
	.recalc		= &omap3_dpll_recalc,
	.round_rate	= &omap2_dpll_round_rate,
	.set_rate	= &omap3_noncore_dpll_set_rate,
};

static const struct clksel dpll_per_m2_div[] = {
	{ .parent = &dpll_per_ck, .rates = div31_1to31_rates },
	{ .parent = NULL },
};

static struct clk dpll_per_m2_ck = {
	.name		= "dpll_per_m2_ck",
	.parent		= &dpll_per_ck,
	.clksel		= dpll_per_m2_div,
	.clksel_reg	= OMAP4430_CM_DIV_M2_DPLL_PER,
	.clksel_mask	= OMAP4430_DPLL_CLKOUT_DIV_MASK,
	.ops		= &clkops_null,
	.recalc		= &omap2_clksel_recalc,
	.round_rate	= &omap2_clksel_round_rate,
	.set_rate	= &omap2_clksel_set_rate,
};

static struct clk dpll_per_m2x2_ck = {
	.name		= "dpll_per_m2x2_ck",
	.parent		= &dpll_per_ck,
	.ops		= &clkops_null,
	.recalc		= &followparent_recalc,
};

static struct clk dpll_per_m3_ck = {
	.name		= "dpll_per_m3_ck",
	.parent		= &dpll_per_ck,
	.clksel		= dpll_per_m2_div,
	.clksel_reg	= OMAP4430_CM_DIV_M3_DPLL_PER,
	.clksel_mask	= OMAP4430_DPLL_CLKOUTHIF_DIV_MASK,
	.ops		= &clkops_null,
	.recalc		= &omap2_clksel_recalc,
	.round_rate	= &omap2_clksel_round_rate,
	.set_rate	= &omap2_clksel_set_rate,
};

static struct clk dpll_per_m4_ck = {
	.name		= "dpll_per_m4_ck",
	.parent		= &dpll_per_ck,
	.clksel		= dpll_per_m2_div,
	.clksel_reg	= OMAP4430_CM_DIV_M4_DPLL_PER,
	.clksel_mask	= OMAP4430_HSDIVIDER_CLKOUT1_DIV_MASK,
	.ops		= &clkops_null,
	.recalc		= &omap2_clksel_recalc,
	.round_rate	= &omap2_clksel_round_rate,
	.set_rate	= &omap2_clksel_set_rate,
};

static struct clk dpll_per_m5_ck = {
	.name		= "dpll_per_m5_ck",
	.parent		= &dpll_per_ck,
	.clksel		= dpll_per_m2_div,
	.clksel_reg	= OMAP4430_CM_DIV_M5_DPLL_PER,
	.clksel_mask	= OMAP4430_HSDIVIDER_CLKOUT2_DIV_MASK,
	.ops		= &clkops_null,
	.recalc		= &omap2_clksel_recalc,
	.round_rate	= &omap2_clksel_round_rate,
	.set_rate	= &omap2_clksel_set_rate,
};

static struct clk dpll_per_m6_ck = {
	.name		= "dpll_per_m6_ck",
	.parent		= &dpll_per_ck,
	.clksel		= dpll_per_m2_div,
	.clksel_reg	= OMAP4430_CM_DIV_M6_DPLL_PER,
	.clksel_mask	= OMAP4430_HSDIVIDER_CLKOUT3_DIV_MASK,
	.ops		= &clkops_null,
	.recalc		= &omap2_clksel_recalc,
	.round_rate	= &omap2_clksel_round_rate,
	.set_rate	= &omap2_clksel_set_rate,
};

static struct clk dpll_per_m7_ck = {
	.name		= "dpll_per_m7_ck",
	.parent		= &dpll_per_ck,
	.clksel		= dpll_per_m2_div,
	.clksel_reg	= OMAP4430_CM_DIV_M7_DPLL_PER,
	.clksel_mask	= OMAP4430_HSDIVIDER_CLKOUT4_DIV_MASK,
	.ops		= &clkops_null,
	.recalc		= &omap2_clksel_recalc,
	.round_rate	= &omap2_clksel_round_rate,
	.set_rate	= &omap2_clksel_set_rate,
};

/* DPLL_UNIPRO */
static struct dpll_data dpll_unipro_dd = {
	.mult_div1_reg	= OMAP4430_CM_CLKSEL_DPLL_UNIPRO,
	.clk_bypass	= &dpll_sys_ref_clk,
	.clk_ref	= &dpll_sys_ref_clk,
	.control_reg	= OMAP4430_CM_CLKMODE_DPLL_UNIPRO,
	.modes		= (1 << DPLL_LOW_POWER_BYPASS) | (1 << DPLL_LOCKED),
	.autoidle_reg	= OMAP4430_CM_AUTOIDLE_DPLL_UNIPRO,
	.idlest_reg	= OMAP4430_CM_IDLEST_DPLL_UNIPRO,
	.mult_mask	= OMAP4430_DPLL_MULT_MASK,
	.div1_mask	= OMAP4430_DPLL_DIV_MASK,
	.enable_mask	= OMAP4430_DPLL_EN_MASK,
	.autoidle_mask	= OMAP4430_AUTO_DPLL_MODE_MASK,
	.idlest_mask	= OMAP4430_ST_DPLL_CLK_MASK,
	.max_multiplier	= OMAP4430_MAX_DPLL_MULT,
	.max_divider	= OMAP4430_MAX_DPLL_DIV,
	.min_divider	= 1,
};


static struct clk dpll_unipro_ck = {
	.name		= "dpll_unipro_ck",
	.parent		= &dpll_sys_ref_clk,
	.dpll_data	= &dpll_unipro_dd,
	.init		= &omap2_init_dpll_parent,
	.ops		= &clkops_omap3_noncore_dpll_ops,
	.recalc		= &omap3_dpll_recalc,
	.round_rate	= &omap2_dpll_round_rate,
	.set_rate	= &omap3_noncore_dpll_set_rate,
};

static const struct clksel dpll_unipro_m2x2_div[] = {
	{ .parent = &dpll_unipro_ck, .rates = div31_1to31_rates },
	{ .parent = NULL },
};

static struct clk dpll_unipro_m2x2_ck = {
	.name		= "dpll_unipro_m2x2_ck",
	.parent		= &dpll_unipro_ck,
	.clksel		= dpll_unipro_m2x2_div,
	.clksel_reg	= OMAP4430_CM_DIV_M2_DPLL_UNIPRO,
	.clksel_mask	= OMAP4430_DPLL_CLKOUT_DIV_MASK,
	.ops		= &clkops_null,
	.recalc		= &omap2_clksel_recalc,
	.round_rate	= &omap2_clksel_round_rate,
	.set_rate	= &omap2_clksel_set_rate,
};

static struct clk usb_hs_clk_div_ck = {
	.name		= "usb_hs_clk_div_ck",
	.parent		= &dpll_abe_m3_ck,
	.ops		= &clkops_null,
	.recalc		= &followparent_recalc,
};

/* DPLL_USB */
static struct dpll_data dpll_usb_dd = {
	.mult_div1_reg	= OMAP4430_CM_CLKSEL_DPLL_USB,
	.clk_bypass	= &usb_hs_clk_div_ck,
	.clk_ref	= &dpll_sys_ref_clk,
	.control_reg	= OMAP4430_CM_CLKMODE_DPLL_USB,
	.modes		= (1 << DPLL_LOW_POWER_BYPASS) | (1 << DPLL_LOCKED),
	.autoidle_reg	= OMAP4430_CM_AUTOIDLE_DPLL_USB,
	.idlest_reg	= OMAP4430_CM_IDLEST_DPLL_USB,
	.mult_mask	= OMAP4430_DPLL_MULT_MASK,
	.div1_mask	= OMAP4430_DPLL_DIV_MASK,
	.enable_mask	= OMAP4430_DPLL_EN_MASK,
	.autoidle_mask	= OMAP4430_AUTO_DPLL_MODE_MASK,
	.idlest_mask	= OMAP4430_ST_DPLL_CLK_MASK,
	.max_multiplier	= OMAP4430_MAX_DPLL_MULT,
	.max_divider	= OMAP4430_MAX_DPLL_DIV,
	.min_divider	= 1,
	.flags		= DPLL_J_TYPE | DPLL_NO_DCO_SEL
};


static struct clk dpll_usb_ck = {
	.name		= "dpll_usb_ck",
	.parent		= &dpll_sys_ref_clk,
	.dpll_data	= &dpll_usb_dd,
	.init		= &omap2_init_dpll_parent,
	.ops		= &clkops_omap3_noncore_dpll_ops,
	.recalc		= &omap3_dpll_recalc,
	.round_rate	= &omap2_dpll_round_rate,
	.set_rate	= &omap3_noncore_dpll_set_rate,
};

static struct clk dpll_usb_clkdcoldo_ck = {
	.name		= "dpll_usb_clkdcoldo_ck",
	.parent		= &dpll_usb_ck,
	.ops		= &clkops_null,
	.recalc		= &followparent_recalc,
};

static const struct clksel dpll_usb_m2_div[] = {
	{ .parent = &dpll_usb_ck, .rates = div31_1to31_rates },
	{ .parent = NULL },
};

static struct clk dpll_usb_m2_ck = {
	.name		= "dpll_usb_m2_ck",
	.parent		= &dpll_usb_ck,
	.clksel		= dpll_usb_m2_div,
	.clksel_reg	= OMAP4430_CM_DIV_M2_DPLL_USB,
	.clksel_mask	= OMAP4430_DPLL_CLKOUT_DIV_0_6_MASK,
	.ops		= &clkops_null,
	.recalc		= &omap2_clksel_recalc,
	.round_rate	= &omap2_clksel_round_rate,
	.set_rate	= &omap2_clksel_set_rate,
};

static const struct clksel ducati_clk_mux_sel[] = {
	{ .parent = &div_core_ck, .rates = div_1_0_rates },
	{ .parent = &dpll_per_m6_ck, .rates = div_1_1_rates },
	{ .parent = NULL },
};

static struct clk ducati_clk_mux_ck = {
	.name		= "ducati_clk_mux_ck",
	.parent		= &div_core_ck,
	.clksel		= ducati_clk_mux_sel,
	.init		= &omap2_init_clksel_parent,
	.clksel_reg	= OMAP4430_CM_CLKSEL_DUCATI_ISS_ROOT,
	.clksel_mask	= OMAP4430_CLKSEL_0_0_MASK,
	.ops		= &clkops_null,
	.recalc		= &omap2_clksel_recalc,
};

static struct clk func_12m_fclk = {
	.name		= "func_12m_fclk",
	.parent		= &dpll_per_m2x2_ck,
	.ops		= &clkops_null,
	.recalc		= &followparent_recalc,
};

static struct clk func_24m_clk = {
	.name		= "func_24m_clk",
	.parent		= &dpll_per_m2_ck,
	.ops		= &clkops_null,
	.recalc		= &followparent_recalc,
};

static struct clk func_24mc_fclk = {
	.name		= "func_24mc_fclk",
	.parent		= &dpll_per_m2x2_ck,
	.ops		= &clkops_null,
	.recalc		= &followparent_recalc,
};

static const struct clksel_rate div2_4to8_rates[] = {
	{ .div = 4, .val = 0, .flags = RATE_IN_4430 },
	{ .div = 8, .val = 1, .flags = RATE_IN_4430 },
	{ .div = 0 },
};

static const struct clksel func_48m_fclk_div[] = {
	{ .parent = &dpll_per_m2x2_ck, .rates = div2_4to8_rates },
	{ .parent = NULL },
};

static struct clk func_48m_fclk = {
	.name		= "func_48m_fclk",
	.parent		= &dpll_per_m2x2_ck,
	.clksel		= func_48m_fclk_div,
	.clksel_reg	= OMAP4430_CM_SCALE_FCLK,
	.clksel_mask	= OMAP4430_SCALE_FCLK_MASK,
	.ops		= &clkops_null,
	.recalc		= &omap2_clksel_recalc,
	.round_rate	= &omap2_clksel_round_rate,
	.set_rate	= &omap2_clksel_set_rate,
};

static struct clk func_48mc_fclk = {
	.name		= "func_48mc_fclk",
	.parent		= &dpll_per_m2x2_ck,
	.ops		= &clkops_null,
	.recalc		= &followparent_recalc,
};

static const struct clksel_rate div2_2to4_rates[] = {
	{ .div = 2, .val = 0, .flags = RATE_IN_4430 },
	{ .div = 4, .val = 1, .flags = RATE_IN_4430 },
	{ .div = 0 },
};

static const struct clksel func_64m_fclk_div[] = {
	{ .parent = &dpll_per_m4_ck, .rates = div2_2to4_rates },
	{ .parent = NULL },
};

static struct clk func_64m_fclk = {
	.name		= "func_64m_fclk",
	.parent		= &dpll_per_m4_ck,
	.clksel		= func_64m_fclk_div,
	.clksel_reg	= OMAP4430_CM_SCALE_FCLK,
	.clksel_mask	= OMAP4430_SCALE_FCLK_MASK,
	.ops		= &clkops_null,
	.recalc		= &omap2_clksel_recalc,
	.round_rate	= &omap2_clksel_round_rate,
	.set_rate	= &omap2_clksel_set_rate,
};

static const struct clksel func_96m_fclk_div[] = {
	{ .parent = &dpll_per_m2x2_ck, .rates = div2_2to4_rates },
	{ .parent = NULL },
};

static struct clk func_96m_fclk = {
	.name		= "func_96m_fclk",
	.parent		= &dpll_per_m2x2_ck,
	.clksel		= func_96m_fclk_div,
	.clksel_reg	= OMAP4430_CM_SCALE_FCLK,
	.clksel_mask	= OMAP4430_SCALE_FCLK_MASK,
	.ops		= &clkops_null,
	.recalc		= &omap2_clksel_recalc,
	.round_rate	= &omap2_clksel_round_rate,
	.set_rate	= &omap2_clksel_set_rate,
};

static const struct clksel hsmmc6_fclk_sel[] = {
	{ .parent = &func_64m_fclk, .rates = div_1_0_rates },
	{ .parent = &func_96m_fclk, .rates = div_1_1_rates },
	{ .parent = NULL },
};

static struct clk hsmmc6_fclk = {
	.name		= "hsmmc6_fclk",
	.parent		= &func_64m_fclk,
	.ops		= &clkops_null,
	.recalc		= &followparent_recalc,
};

static const struct clksel_rate div2_1to8_rates[] = {
	{ .div = 1, .val = 0, .flags = RATE_IN_4430 },
	{ .div = 8, .val = 1, .flags = RATE_IN_4430 },
	{ .div = 0 },
};

static const struct clksel init_60m_fclk_div[] = {
	{ .parent = &dpll_usb_m2_ck, .rates = div2_1to8_rates },
	{ .parent = NULL },
};

static struct clk init_60m_fclk = {
	.name		= "init_60m_fclk",
	.parent		= &dpll_usb_m2_ck,
	.clksel		= init_60m_fclk_div,
	.clksel_reg	= OMAP4430_CM_CLKSEL_USB_60MHZ,
	.clksel_mask	= OMAP4430_CLKSEL_0_0_MASK,
	.ops		= &clkops_null,
	.recalc		= &omap2_clksel_recalc,
	.round_rate	= &omap2_clksel_round_rate,
	.set_rate	= &omap2_clksel_set_rate,
};

static const struct clksel l3_div_div[] = {
	{ .parent = &div_core_ck, .rates = div2_1to2_rates },
	{ .parent = NULL },
};

static struct clk l3_div_ck = {
	.name		= "l3_div_ck",
	.parent		= &div_core_ck,
	.clksel		= l3_div_div,
	.clksel_reg	= OMAP4430_CM_CLKSEL_CORE,
	.clksel_mask	= OMAP4430_CLKSEL_L3_MASK,
	.ops		= &clkops_null,
	.recalc		= &omap2_clksel_recalc,
	.round_rate	= &omap2_clksel_round_rate,
	.set_rate	= &omap2_clksel_set_rate,
};

static const struct clksel l4_div_div[] = {
	{ .parent = &l3_div_ck, .rates = div2_1to2_rates },
	{ .parent = NULL },
};

static struct clk l4_div_ck = {
	.name		= "l4_div_ck",
	.parent		= &l3_div_ck,
	.clksel		= l4_div_div,
	.clksel_reg	= OMAP4430_CM_CLKSEL_CORE,
	.clksel_mask	= OMAP4430_CLKSEL_L4_MASK,
	.ops		= &clkops_null,
	.recalc		= &omap2_clksel_recalc,
	.round_rate	= &omap2_clksel_round_rate,
	.set_rate	= &omap2_clksel_set_rate,
};

static struct clk lp_clk_div_ck = {
	.name		= "lp_clk_div_ck",
	.parent		= &dpll_abe_m2x2_ck,
	.ops		= &clkops_null,
	.recalc		= &followparent_recalc,
};

static const struct clksel l4_wkup_clk_mux_sel[] = {
	{ .parent = &sys_clkin_ck, .rates = div_1_0_rates },
	{ .parent = &lp_clk_div_ck, .rates = div_1_1_rates },
	{ .parent = NULL },
};

static struct clk l4_wkup_clk_mux_ck = {
	.name		= "l4_wkup_clk_mux_ck",
	.parent		= &sys_clkin_ck,
	.clksel		= l4_wkup_clk_mux_sel,
	.init		= &omap2_init_clksel_parent,
	.clksel_reg	= OMAP4430_CM_L4_WKUP_CLKSEL,
	.clksel_mask	= OMAP4430_CLKSEL_0_0_MASK,
	.ops		= &clkops_null,
	.recalc		= &omap2_clksel_recalc,
};

static const struct clksel per_abe_nc_fclk_div[] = {
	{ .parent = &dpll_abe_m2_ck, .rates = div2_1to2_rates },
	{ .parent = NULL },
};

static struct clk per_abe_nc_fclk = {
	.name		= "per_abe_nc_fclk",
	.parent		= &dpll_abe_m2_ck,
	.clksel		= per_abe_nc_fclk_div,
	.clksel_reg	= OMAP4430_CM_SCALE_FCLK,
	.clksel_mask	= OMAP4430_SCALE_FCLK_MASK,
	.ops		= &clkops_null,
	.recalc		= &omap2_clksel_recalc,
	.round_rate	= &omap2_clksel_round_rate,
	.set_rate	= &omap2_clksel_set_rate,
};

static const struct clksel mcasp2_fclk_sel[] = {
	{ .parent = &func_96m_fclk, .rates = div_1_0_rates },
	{ .parent = &per_abe_nc_fclk, .rates = div_1_1_rates },
	{ .parent = NULL },
};

static struct clk mcasp2_fclk = {
	.name		= "mcasp2_fclk",
	.parent		= &func_96m_fclk,
	.ops		= &clkops_null,
	.recalc		= &followparent_recalc,
};

static struct clk mcasp3_fclk = {
	.name		= "mcasp3_fclk",
	.parent		= &func_96m_fclk,
	.ops		= &clkops_null,
	.recalc		= &followparent_recalc,
};

static struct clk ocp_abe_iclk = {
	.name		= "ocp_abe_iclk",
	.parent		= &aess_fclk,
	.ops		= &clkops_null,
	.recalc		= &followparent_recalc,
};

static struct clk per_abe_24m_fclk = {
	.name		= "per_abe_24m_fclk",
	.parent		= &dpll_abe_m2_ck,
	.ops		= &clkops_null,
	.recalc		= &followparent_recalc,
};

static const struct clksel pmd_stm_clock_mux_sel[] = {
	{ .parent = &sys_clkin_ck, .rates = div_1_0_rates },
	{ .parent = &dpll_core_m6_ck, .rates = div_1_1_rates },
	{ .parent = &dpll_per_m7_ck, .rates = div_1_2_rates },
	{ .parent = NULL },
};

static struct clk pmd_stm_clock_mux_ck = {
	.name		= "pmd_stm_clock_mux_ck",
	.parent		= &sys_clkin_ck,
	.ops		= &clkops_null,
	.recalc		= &followparent_recalc,
};

static struct clk pmd_trace_clk_mux_ck = {
	.name		= "pmd_trace_clk_mux_ck",
	.parent		= &sys_clkin_ck,
	.ops		= &clkops_null,
	.recalc		= &followparent_recalc,
};

static struct clk syc_clk_div_ck = {
	.name		= "syc_clk_div_ck",
	.parent		= &sys_clkin_ck,
	.clksel		= dpll_sys_ref_clk_div,
	.clksel_reg	= OMAP4430_CM_ABE_DSS_SYS_CLKSEL,
	.clksel_mask	= OMAP4430_CLKSEL_0_0_MASK,
	.ops		= &clkops_null,
	.recalc		= &omap2_clksel_recalc,
	.round_rate	= &omap2_clksel_round_rate,
	.set_rate	= &omap2_clksel_set_rate,
};

/* Leaf clocks controlled by modules */

static struct clk aes1_fck = {
	.name		= "aes1_fck",
	.ops		= &clkops_omap2_dflt,
	.enable_reg	= OMAP4430_CM_L4SEC_AES1_CLKCTRL,
	.enable_bit	= OMAP4430_MODULEMODE_SWCTRL,
	.clkdm_name	= "l4_secure_clkdm",
	.parent		= &l3_div_ck,
	.recalc		= &followparent_recalc,
};

static struct clk aes2_fck = {
	.name		= "aes2_fck",
	.ops		= &clkops_omap2_dflt,
	.enable_reg	= OMAP4430_CM_L4SEC_AES2_CLKCTRL,
	.enable_bit	= OMAP4430_MODULEMODE_SWCTRL,
	.clkdm_name	= "l4_secure_clkdm",
	.parent		= &l3_div_ck,
	.recalc		= &followparent_recalc,
};

static struct clk aess_fck = {
	.name		= "aess_fck",
	.ops		= &clkops_omap2_dflt,
	.enable_reg	= OMAP4430_CM1_ABE_AESS_CLKCTRL,
	.enable_bit	= OMAP4430_MODULEMODE_SWCTRL,
	.clkdm_name	= "abe_clkdm",
	.parent		= &aess_fclk,
	.recalc		= &followparent_recalc,
};

static struct clk cust_efuse_fck = {
	.name		= "cust_efuse_fck",
	.ops		= &clkops_omap2_dflt,
	.enable_reg	= OMAP4430_CM_CEFUSE_CEFUSE_CLKCTRL,
	.enable_bit	= OMAP4430_MODULEMODE_SWCTRL,
	.clkdm_name	= "l4_cefuse_clkdm",
	.parent		= &sys_clkin_ck,
	.recalc		= &followparent_recalc,
};

static struct clk des3des_fck = {
	.name		= "des3des_fck",
	.ops		= &clkops_omap2_dflt,
	.enable_reg	= OMAP4430_CM_L4SEC_DES3DES_CLKCTRL,
	.enable_bit	= OMAP4430_MODULEMODE_SWCTRL,
	.clkdm_name	= "l4_secure_clkdm",
	.parent		= &l4_div_ck,
	.recalc		= &followparent_recalc,
};

static const struct clksel dmic_sync_mux_sel[] = {
	{ .parent = &abe_24m_fclk, .rates = div_1_0_rates },
	{ .parent = &syc_clk_div_ck, .rates = div_1_1_rates },
	{ .parent = &func_24m_clk, .rates = div_1_2_rates },
	{ .parent = NULL },
};

static struct clk dmic_sync_mux_ck = {
	.name		= "dmic_sync_mux_ck",
	.parent		= &abe_24m_fclk,
	.clksel		= dmic_sync_mux_sel,
	.init		= &omap2_init_clksel_parent,
	.clksel_reg	= OMAP4430_CM1_ABE_DMIC_CLKCTRL,
	.clksel_mask	= OMAP4430_CLKSEL_INTERNAL_SOURCE_MASK,
	.ops		= &clkops_null,
	.recalc		= &omap2_clksel_recalc,
};

static const struct clksel func_dmic_abe_gfclk_sel[] = {
	{ .parent = &dmic_sync_mux_ck, .rates = div_1_0_rates },
	{ .parent = &pad_clks_ck, .rates = div_1_1_rates },
	{ .parent = &slimbus_clk, .rates = div_1_2_rates },
	{ .parent = NULL },
};

/* Merged func_dmic_abe_gfclk into dmic */
static struct clk dmic_fck = {
	.name		= "dmic_fck",
	.parent		= &dmic_sync_mux_ck,
	.clksel		= func_dmic_abe_gfclk_sel,
	.init		= &omap2_init_clksel_parent,
	.clksel_reg	= OMAP4430_CM1_ABE_DMIC_CLKCTRL,
	.clksel_mask	= OMAP4430_CLKSEL_SOURCE_MASK,
	.ops		= &clkops_omap2_dflt,
	.recalc		= &omap2_clksel_recalc,
	.enable_reg	= OMAP4430_CM1_ABE_DMIC_CLKCTRL,
	.enable_bit	= OMAP4430_MODULEMODE_SWCTRL,
	.clkdm_name	= "abe_clkdm",
};

static struct clk dss_fck = {
	.name		= "dss_fck",
	.ops		= &clkops_omap2_dflt,
	.enable_reg	= OMAP4430_CM_DSS_DSS_CLKCTRL,
	.enable_bit	= OMAP4430_MODULEMODE_SWCTRL,
	.clkdm_name	= "l3_dss_clkdm",
	.parent		= &l3_div_ck,
	.recalc		= &followparent_recalc,
};

static struct clk ducati_ick = {
	.name		= "ducati_ick",
	.ops		= &clkops_omap2_dflt,
	.enable_reg	= OMAP4430_CM_DUCATI_DUCATI_CLKCTRL,
	.enable_bit	= OMAP4430_MODULEMODE_HWCTRL,
	.clkdm_name	= "ducati_clkdm",
	.parent		= &ducati_clk_mux_ck,
	.recalc		= &followparent_recalc,
};

static struct clk emif1_ick = {
	.name		= "emif1_ick",
	.ops		= &clkops_omap2_dflt,
	.enable_reg	= OMAP4430_CM_MEMIF_EMIF_1_CLKCTRL,
	.enable_bit	= OMAP4430_MODULEMODE_HWCTRL,
	.flags		= ENABLE_ON_INIT,
	.clkdm_name	= "l3_emif_clkdm",
	.parent		= &ddrphy_ck,
	.recalc		= &followparent_recalc,
};

static struct clk emif2_ick = {
	.name		= "emif2_ick",
	.ops		= &clkops_omap2_dflt,
	.enable_reg	= OMAP4430_CM_MEMIF_EMIF_2_CLKCTRL,
	.enable_bit	= OMAP4430_MODULEMODE_HWCTRL,
	.flags		= ENABLE_ON_INIT,
	.clkdm_name	= "l3_emif_clkdm",
	.parent		= &ddrphy_ck,
	.recalc		= &followparent_recalc,
};

static const struct clksel fdif_fclk_div[] = {
	{ .parent = &dpll_per_m4_ck, .rates = div3_1to4_rates },
	{ .parent = NULL },
};

/* Merged fdif_fclk into fdif */
static struct clk fdif_fck = {
	.name		= "fdif_fck",
	.parent		= &dpll_per_m4_ck,
	.clksel		= fdif_fclk_div,
	.clksel_reg	= OMAP4430_CM_CAM_FDIF_CLKCTRL,
	.clksel_mask	= OMAP4430_CLKSEL_FCLK_MASK,
	.ops		= &clkops_omap2_dflt,
	.recalc		= &omap2_clksel_recalc,
	.round_rate	= &omap2_clksel_round_rate,
	.set_rate	= &omap2_clksel_set_rate,
	.enable_reg	= OMAP4430_CM_CAM_FDIF_CLKCTRL,
	.enable_bit	= OMAP4430_MODULEMODE_SWCTRL,
	.clkdm_name	= "iss_clkdm",
};

static const struct clksel per_sgx_fclk_div[] = {
	{ .parent = &dpll_per_m2x2_ck, .rates = div3_1to4_rates },
	{ .parent = NULL },
};

static struct clk per_sgx_fclk = {
	.name		= "per_sgx_fclk",
	.parent		= &dpll_per_m2x2_ck,
	.clksel		= per_sgx_fclk_div,
	.clksel_reg	= OMAP4430_CM_GFX_GFX_CLKCTRL,
	.clksel_mask	= OMAP4430_CLKSEL_PER_192M_MASK,
	.ops		= &clkops_null,
	.recalc		= &omap2_clksel_recalc,
	.round_rate	= &omap2_clksel_round_rate,
	.set_rate	= &omap2_clksel_set_rate,
};

static const struct clksel sgx_clk_mux_sel[] = {
	{ .parent = &dpll_core_m7_ck, .rates = div_1_0_rates },
	{ .parent = &per_sgx_fclk, .rates = div_1_1_rates },
	{ .parent = NULL },
};

/* Merged sgx_clk_mux into gfx */
static struct clk gfx_fck = {
	.name		= "gfx_fck",
	.parent		= &dpll_core_m7_ck,
	.clksel		= sgx_clk_mux_sel,
	.init		= &omap2_init_clksel_parent,
	.clksel_reg	= OMAP4430_CM_GFX_GFX_CLKCTRL,
	.clksel_mask	= OMAP4430_CLKSEL_SGX_FCLK_MASK,
	.ops		= &clkops_omap2_dflt,
	.recalc		= &omap2_clksel_recalc,
	.enable_reg	= OMAP4430_CM_GFX_GFX_CLKCTRL,
	.enable_bit	= OMAP4430_MODULEMODE_SWCTRL,
	.clkdm_name	= "l3_gfx_clkdm",
};

static struct clk gpio1_ick = {
	.name		= "gpio1_ick",
	.ops		= &clkops_omap2_dflt,
	.enable_reg	= OMAP4430_CM_WKUP_GPIO1_CLKCTRL,
	.enable_bit	= OMAP4430_MODULEMODE_HWCTRL,
	.clkdm_name	= "l4_wkup_clkdm",
	.parent		= &l4_wkup_clk_mux_ck,
	.recalc		= &followparent_recalc,
};

static struct clk gpio2_ick = {
	.name		= "gpio2_ick",
	.ops		= &clkops_omap2_dflt,
	.enable_reg	= OMAP4430_CM_L4PER_GPIO2_CLKCTRL,
	.enable_bit	= OMAP4430_MODULEMODE_HWCTRL,
	.clkdm_name	= "l4_per_clkdm",
	.parent		= &l4_div_ck,
	.recalc		= &followparent_recalc,
};

static struct clk gpio3_ick = {
	.name		= "gpio3_ick",
	.ops		= &clkops_omap2_dflt,
	.enable_reg	= OMAP4430_CM_L4PER_GPIO3_CLKCTRL,
	.enable_bit	= OMAP4430_MODULEMODE_HWCTRL,
	.clkdm_name	= "l4_per_clkdm",
	.parent		= &l4_div_ck,
	.recalc		= &followparent_recalc,
};

static struct clk gpio4_ick = {
	.name		= "gpio4_ick",
	.ops		= &clkops_omap2_dflt,
	.enable_reg	= OMAP4430_CM_L4PER_GPIO4_CLKCTRL,
	.enable_bit	= OMAP4430_MODULEMODE_HWCTRL,
	.clkdm_name	= "l4_per_clkdm",
	.parent		= &l4_div_ck,
	.recalc		= &followparent_recalc,
};

static struct clk gpio5_ick = {
	.name		= "gpio5_ick",
	.ops		= &clkops_omap2_dflt,
	.enable_reg	= OMAP4430_CM_L4PER_GPIO5_CLKCTRL,
	.enable_bit	= OMAP4430_MODULEMODE_HWCTRL,
	.clkdm_name	= "l4_per_clkdm",
	.parent		= &l4_div_ck,
	.recalc		= &followparent_recalc,
};

static struct clk gpio6_ick = {
	.name		= "gpio6_ick",
	.ops		= &clkops_omap2_dflt,
	.enable_reg	= OMAP4430_CM_L4PER_GPIO6_CLKCTRL,
	.enable_bit	= OMAP4430_MODULEMODE_HWCTRL,
	.clkdm_name	= "l4_per_clkdm",
	.parent		= &l4_div_ck,
	.recalc		= &followparent_recalc,
};

static struct clk gpmc_ick = {
	.name		= "gpmc_ick",
	.ops		= &clkops_omap2_dflt,
	.enable_reg	= OMAP4430_CM_L3_2_GPMC_CLKCTRL,
	.enable_bit	= OMAP4430_MODULEMODE_HWCTRL,
	.clkdm_name	= "l3_2_clkdm",
	.parent		= &l3_div_ck,
	.recalc		= &followparent_recalc,
};

static const struct clksel dmt1_clk_mux_sel[] = {
	{ .parent = &sys_clkin_ck, .rates = div_1_0_rates },
	{ .parent = &sys_32k_ck, .rates = div_1_1_rates },
	{ .parent = NULL },
};

static struct clk gpt1_fck = {
	.name		= "gpt1_fck",
	.parent		= &sys_clkin_ck,
	.clksel		= dmt1_clk_mux_sel,
	.init		= &omap2_init_clksel_parent,
	.clksel_reg	= OMAP4430_CM_WKUP_TIMER1_CLKCTRL,
	.clksel_mask	= OMAP4430_CLKSEL_MASK,
	.ops		= &clkops_omap2_dflt,
	.recalc		= &omap2_clksel_recalc,
	.enable_reg	= OMAP4430_CM_WKUP_TIMER1_CLKCTRL,
	.enable_bit	= OMAP4430_MODULEMODE_SWCTRL,
	.clkdm_name	= "l4_wkup_clkdm",
};

static struct clk gpt10_fck = {
	.name		= "gpt10_fck",
	.parent		= &sys_clkin_ck,
	.clksel		= dmt1_clk_mux_sel,
	.init		= &omap2_init_clksel_parent,
	.clksel_reg	= OMAP4430_CM_L4PER_DMTIMER10_CLKCTRL,
	.clksel_mask	= OMAP4430_CLKSEL_MASK,
	.ops		= &clkops_omap2_dflt,
	.recalc		= &omap2_clksel_recalc,
	.enable_reg	= OMAP4430_CM_L4PER_DMTIMER10_CLKCTRL,
	.enable_bit	= OMAP4430_MODULEMODE_SWCTRL,
	.clkdm_name	= "l4_per_clkdm",
};

static struct clk gpt11_fck = {
	.name		= "gpt11_fck",
	.parent		= &sys_clkin_ck,
	.clksel		= dmt1_clk_mux_sel,
	.init		= &omap2_init_clksel_parent,
	.clksel_reg	= OMAP4430_CM_L4PER_DMTIMER11_CLKCTRL,
	.clksel_mask	= OMAP4430_CLKSEL_MASK,
	.ops		= &clkops_omap2_dflt,
	.recalc		= &omap2_clksel_recalc,
	.enable_reg	= OMAP4430_CM_L4PER_DMTIMER11_CLKCTRL,
	.enable_bit	= OMAP4430_MODULEMODE_SWCTRL,
	.clkdm_name	= "l4_per_clkdm",
};

static struct clk gpt2_fck = {
	.name		= "gpt2_fck",
	.parent		= &sys_clkin_ck,
	.clksel		= dmt1_clk_mux_sel,
	.init		= &omap2_init_clksel_parent,
	.clksel_reg	= OMAP4430_CM_L4PER_DMTIMER2_CLKCTRL,
	.clksel_mask	= OMAP4430_CLKSEL_MASK,
	.ops		= &clkops_omap2_dflt,
	.recalc		= &omap2_clksel_recalc,
	.enable_reg	= OMAP4430_CM_L4PER_DMTIMER2_CLKCTRL,
	.enable_bit	= OMAP4430_MODULEMODE_SWCTRL,
	.clkdm_name	= "l4_per_clkdm",
};

static struct clk gpt3_fck = {
	.name		= "gpt3_fck",
	.parent		= &sys_clkin_ck,
	.clksel		= dmt1_clk_mux_sel,
	.init		= &omap2_init_clksel_parent,
	.clksel_reg	= OMAP4430_CM_L4PER_DMTIMER3_CLKCTRL,
	.clksel_mask	= OMAP4430_CLKSEL_MASK,
	.ops		= &clkops_omap2_dflt,
	.recalc		= &omap2_clksel_recalc,
	.enable_reg	= OMAP4430_CM_L4PER_DMTIMER3_CLKCTRL,
	.enable_bit	= OMAP4430_MODULEMODE_SWCTRL,
	.clkdm_name	= "l4_per_clkdm",
};

static struct clk gpt4_fck = {
	.name		= "gpt4_fck",
	.parent		= &sys_clkin_ck,
	.clksel		= dmt1_clk_mux_sel,
	.init		= &omap2_init_clksel_parent,
	.clksel_reg	= OMAP4430_CM_L4PER_DMTIMER4_CLKCTRL,
	.clksel_mask	= OMAP4430_CLKSEL_MASK,
	.ops		= &clkops_omap2_dflt,
	.recalc		= &omap2_clksel_recalc,
	.enable_reg	= OMAP4430_CM_L4PER_DMTIMER4_CLKCTRL,
	.enable_bit	= OMAP4430_MODULEMODE_SWCTRL,
	.clkdm_name	= "l4_per_clkdm",
};

static const struct clksel timer5_sync_mux_sel[] = {
	{ .parent = &syc_clk_div_ck, .rates = div_1_0_rates },
	{ .parent = &sys_32k_ck, .rates = div_1_1_rates },
	{ .parent = NULL },
};

static struct clk gpt5_fck = {
	.name		= "gpt5_fck",
	.parent		= &syc_clk_div_ck,
	.clksel		= timer5_sync_mux_sel,
	.init		= &omap2_init_clksel_parent,
	.clksel_reg	= OMAP4430_CM1_ABE_TIMER5_CLKCTRL,
	.clksel_mask	= OMAP4430_CLKSEL_MASK,
	.ops		= &clkops_omap2_dflt,
	.recalc		= &omap2_clksel_recalc,
	.enable_reg	= OMAP4430_CM1_ABE_TIMER5_CLKCTRL,
	.enable_bit	= OMAP4430_MODULEMODE_SWCTRL,
	.clkdm_name	= "abe_clkdm",
};

static struct clk gpt6_fck = {
	.name		= "gpt6_fck",
	.parent		= &syc_clk_div_ck,
	.clksel		= timer5_sync_mux_sel,
	.init		= &omap2_init_clksel_parent,
	.clksel_reg	= OMAP4430_CM1_ABE_TIMER6_CLKCTRL,
	.clksel_mask	= OMAP4430_CLKSEL_MASK,
	.ops		= &clkops_omap2_dflt,
	.recalc		= &omap2_clksel_recalc,
	.enable_reg	= OMAP4430_CM1_ABE_TIMER6_CLKCTRL,
	.enable_bit	= OMAP4430_MODULEMODE_SWCTRL,
	.clkdm_name	= "abe_clkdm",
};

static struct clk gpt7_fck = {
	.name		= "gpt7_fck",
	.parent		= &syc_clk_div_ck,
	.clksel		= timer5_sync_mux_sel,
	.init		= &omap2_init_clksel_parent,
	.clksel_reg	= OMAP4430_CM1_ABE_TIMER7_CLKCTRL,
	.clksel_mask	= OMAP4430_CLKSEL_MASK,
	.ops		= &clkops_omap2_dflt,
	.recalc		= &omap2_clksel_recalc,
	.enable_reg	= OMAP4430_CM1_ABE_TIMER7_CLKCTRL,
	.enable_bit	= OMAP4430_MODULEMODE_SWCTRL,
	.clkdm_name	= "abe_clkdm",
};

static struct clk gpt8_fck = {
	.name		= "gpt8_fck",
	.parent		= &syc_clk_div_ck,
	.clksel		= timer5_sync_mux_sel,
	.init		= &omap2_init_clksel_parent,
	.clksel_reg	= OMAP4430_CM1_ABE_TIMER8_CLKCTRL,
	.clksel_mask	= OMAP4430_CLKSEL_MASK,
	.ops		= &clkops_omap2_dflt,
	.recalc		= &omap2_clksel_recalc,
	.enable_reg	= OMAP4430_CM1_ABE_TIMER8_CLKCTRL,
	.enable_bit	= OMAP4430_MODULEMODE_SWCTRL,
	.clkdm_name	= "abe_clkdm",
};

static struct clk gpt9_fck = {
	.name		= "gpt9_fck",
	.parent		= &sys_clkin_ck,
	.clksel		= dmt1_clk_mux_sel,
	.init		= &omap2_init_clksel_parent,
	.clksel_reg	= OMAP4430_CM_L4PER_DMTIMER9_CLKCTRL,
	.clksel_mask	= OMAP4430_CLKSEL_MASK,
	.ops		= &clkops_omap2_dflt,
	.recalc		= &omap2_clksel_recalc,
	.enable_reg	= OMAP4430_CM_L4PER_DMTIMER9_CLKCTRL,
	.enable_bit	= OMAP4430_MODULEMODE_SWCTRL,
	.clkdm_name	= "l4_per_clkdm",
};

static struct clk hdq1w_fck = {
	.name		= "hdq1w_fck",
	.ops		= &clkops_omap2_dflt,
	.enable_reg	= OMAP4430_CM_L4PER_HDQ1W_CLKCTRL,
	.enable_bit	= OMAP4430_MODULEMODE_SWCTRL,
	.clkdm_name	= "l4_per_clkdm",
	.parent		= &func_12m_fclk,
	.recalc		= &followparent_recalc,
};

/* Merged hsi_fclk into hsi */
static struct clk hsi_ick = {
	.name		= "hsi_ick",
	.parent		= &dpll_per_m2x2_ck,
	.clksel		= per_sgx_fclk_div,
	.clksel_reg	= OMAP4430_CM_L3INIT_HSI_CLKCTRL,
	.clksel_mask	= OMAP4430_CLKSEL_24_25_MASK,
	.ops		= &clkops_omap2_dflt,
	.recalc		= &omap2_clksel_recalc,
	.round_rate	= &omap2_clksel_round_rate,
	.set_rate	= &omap2_clksel_set_rate,
	.enable_reg	= OMAP4430_CM_L3INIT_HSI_CLKCTRL,
	.enable_bit	= OMAP4430_MODULEMODE_HWCTRL,
	.clkdm_name	= "l3_init_clkdm",
};

static struct clk i2c1_fck = {
	.name		= "i2c1_fck",
	.ops		= &clkops_omap2_dflt,
	.enable_reg	= OMAP4430_CM_L4PER_I2C1_CLKCTRL,
	.enable_bit	= OMAP4430_MODULEMODE_SWCTRL,
	.clkdm_name	= "l4_per_clkdm",
	.parent		= &func_96m_fclk,
	.recalc		= &followparent_recalc,
};

static struct clk i2c2_fck = {
	.name		= "i2c2_fck",
	.ops		= &clkops_omap2_dflt,
	.enable_reg	= OMAP4430_CM_L4PER_I2C2_CLKCTRL,
	.enable_bit	= OMAP4430_MODULEMODE_SWCTRL,
	.clkdm_name	= "l4_per_clkdm",
	.parent		= &func_96m_fclk,
	.recalc		= &followparent_recalc,
};

static struct clk i2c3_fck = {
	.name		= "i2c3_fck",
	.ops		= &clkops_omap2_dflt,
	.enable_reg	= OMAP4430_CM_L4PER_I2C3_CLKCTRL,
	.enable_bit	= OMAP4430_MODULEMODE_SWCTRL,
	.clkdm_name	= "l4_per_clkdm",
	.parent		= &func_96m_fclk,
	.recalc		= &followparent_recalc,
};

static struct clk i2c4_fck = {
	.name		= "i2c4_fck",
	.ops		= &clkops_omap2_dflt,
	.enable_reg	= OMAP4430_CM_L4PER_I2C4_CLKCTRL,
	.enable_bit	= OMAP4430_MODULEMODE_SWCTRL,
	.clkdm_name	= "l4_per_clkdm",
	.parent		= &func_96m_fclk,
	.recalc		= &followparent_recalc,
};

static struct clk iss_fck = {
	.name		= "iss_fck",
	.ops		= &clkops_omap2_dflt,
	.enable_reg	= OMAP4430_CM_CAM_ISS_CLKCTRL,
	.enable_bit	= OMAP4430_MODULEMODE_SWCTRL,
	.clkdm_name	= "iss_clkdm",
	.parent		= &ducati_clk_mux_ck,
	.recalc		= &followparent_recalc,
};

static struct clk ivahd_ick = {
	.name		= "ivahd_ick",
	.ops		= &clkops_omap2_dflt,
	.enable_reg	= OMAP4430_CM_IVAHD_IVAHD_CLKCTRL,
	.enable_bit	= OMAP4430_MODULEMODE_HWCTRL,
	.clkdm_name	= "ivahd_clkdm",
	.parent		= &dpll_iva_m5_ck,
	.recalc		= &followparent_recalc,
};

static struct clk keyboard_fck = {
	.name		= "keyboard_fck",
	.ops		= &clkops_omap2_dflt,
	.enable_reg	= OMAP4430_CM_WKUP_KEYBOARD_CLKCTRL,
	.enable_bit	= OMAP4430_MODULEMODE_SWCTRL,
	.clkdm_name	= "l4_wkup_clkdm",
	.parent		= &sys_32k_ck,
	.recalc		= &followparent_recalc,
};

static struct clk l3_instr_interconnect_ick = {
	.name		= "l3_instr_interconnect_ick",
	.ops		= &clkops_omap2_dflt,
	.enable_reg	= OMAP4430_CM_L3INSTR_L3_INSTR_CLKCTRL,
	.enable_bit	= OMAP4430_MODULEMODE_HWCTRL,
	.clkdm_name	= "l3_instr_clkdm",
	.parent		= &l3_div_ck,
	.recalc		= &followparent_recalc,
};

static struct clk l3_interconnect_3_ick = {
	.name		= "l3_interconnect_3_ick",
	.ops		= &clkops_omap2_dflt,
	.enable_reg	= OMAP4430_CM_L3INSTR_L3_3_CLKCTRL,
	.enable_bit	= OMAP4430_MODULEMODE_HWCTRL,
	.clkdm_name	= "l3_instr_clkdm",
	.parent		= &l3_div_ck,
	.recalc		= &followparent_recalc,
};

static struct clk mcasp_sync_mux_ck = {
	.name		= "mcasp_sync_mux_ck",
	.parent		= &abe_24m_fclk,
	.clksel		= dmic_sync_mux_sel,
	.init		= &omap2_init_clksel_parent,
	.clksel_reg	= OMAP4430_CM1_ABE_MCASP_CLKCTRL,
	.clksel_mask	= OMAP4430_CLKSEL_INTERNAL_SOURCE_MASK,
	.ops		= &clkops_null,
	.recalc		= &omap2_clksel_recalc,
};

static const struct clksel func_mcasp_abe_gfclk_sel[] = {
	{ .parent = &mcasp_sync_mux_ck, .rates = div_1_0_rates },
	{ .parent = &pad_clks_ck, .rates = div_1_1_rates },
	{ .parent = &slimbus_clk, .rates = div_1_2_rates },
	{ .parent = NULL },
};

/* Merged func_mcasp_abe_gfclk into mcasp */
static struct clk mcasp_fck = {
	.name		= "mcasp_fck",
	.parent		= &mcasp_sync_mux_ck,
	.clksel		= func_mcasp_abe_gfclk_sel,
	.init		= &omap2_init_clksel_parent,
	.clksel_reg	= OMAP4430_CM1_ABE_MCASP_CLKCTRL,
	.clksel_mask	= OMAP4430_CLKSEL_SOURCE_MASK,
	.ops		= &clkops_omap2_dflt,
	.recalc		= &omap2_clksel_recalc,
	.enable_reg	= OMAP4430_CM1_ABE_MCASP_CLKCTRL,
	.enable_bit	= OMAP4430_MODULEMODE_SWCTRL,
	.clkdm_name	= "abe_clkdm",
};

static struct clk mcbsp1_sync_mux_ck = {
	.name		= "mcbsp1_sync_mux_ck",
	.parent		= &abe_24m_fclk,
	.clksel		= dmic_sync_mux_sel,
	.init		= &omap2_init_clksel_parent,
	.clksel_reg	= OMAP4430_CM1_ABE_MCBSP1_CLKCTRL,
	.clksel_mask	= OMAP4430_CLKSEL_INTERNAL_SOURCE_MASK,
	.ops		= &clkops_null,
	.recalc		= &omap2_clksel_recalc,
};

static const struct clksel func_mcbsp1_gfclk_sel[] = {
	{ .parent = &mcbsp1_sync_mux_ck, .rates = div_1_0_rates },
	{ .parent = &pad_clks_ck, .rates = div_1_1_rates },
	{ .parent = &slimbus_clk, .rates = div_1_2_rates },
	{ .parent = NULL },
};

/* Merged func_mcbsp1_gfclk into mcbsp1 */
static struct clk mcbsp1_fck = {
	.name		= "mcbsp1_fck",
	.parent		= &mcbsp1_sync_mux_ck,
	.clksel		= func_mcbsp1_gfclk_sel,
	.init		= &omap2_init_clksel_parent,
	.clksel_reg	= OMAP4430_CM1_ABE_MCBSP1_CLKCTRL,
	.clksel_mask	= OMAP4430_CLKSEL_SOURCE_MASK,
	.ops		= &clkops_omap2_dflt,
	.recalc		= &omap2_clksel_recalc,
	.enable_reg	= OMAP4430_CM1_ABE_MCBSP1_CLKCTRL,
	.enable_bit	= OMAP4430_MODULEMODE_SWCTRL,
	.clkdm_name	= "abe_clkdm",
};

static struct clk mcbsp2_sync_mux_ck = {
	.name		= "mcbsp2_sync_mux_ck",
	.parent		= &abe_24m_fclk,
	.clksel		= dmic_sync_mux_sel,
	.init		= &omap2_init_clksel_parent,
	.clksel_reg	= OMAP4430_CM1_ABE_MCBSP2_CLKCTRL,
	.clksel_mask	= OMAP4430_CLKSEL_INTERNAL_SOURCE_MASK,
	.ops		= &clkops_null,
	.recalc		= &omap2_clksel_recalc,
};

static const struct clksel func_mcbsp2_gfclk_sel[] = {
	{ .parent = &mcbsp2_sync_mux_ck, .rates = div_1_0_rates },
	{ .parent = &pad_clks_ck, .rates = div_1_1_rates },
	{ .parent = &slimbus_clk, .rates = div_1_2_rates },
	{ .parent = NULL },
};

/* Merged func_mcbsp2_gfclk into mcbsp2 */
static struct clk mcbsp2_fck = {
	.name		= "mcbsp2_fck",
	.parent		= &mcbsp2_sync_mux_ck,
	.clksel		= func_mcbsp2_gfclk_sel,
	.init		= &omap2_init_clksel_parent,
	.clksel_reg	= OMAP4430_CM1_ABE_MCBSP2_CLKCTRL,
	.clksel_mask	= OMAP4430_CLKSEL_SOURCE_MASK,
	.ops		= &clkops_omap2_dflt,
	.recalc		= &omap2_clksel_recalc,
	.enable_reg	= OMAP4430_CM1_ABE_MCBSP2_CLKCTRL,
	.enable_bit	= OMAP4430_MODULEMODE_SWCTRL,
	.clkdm_name	= "abe_clkdm",
};

static struct clk mcbsp3_sync_mux_ck = {
	.name		= "mcbsp3_sync_mux_ck",
	.parent		= &abe_24m_fclk,
	.clksel		= dmic_sync_mux_sel,
	.init		= &omap2_init_clksel_parent,
	.clksel_reg	= OMAP4430_CM1_ABE_MCBSP3_CLKCTRL,
	.clksel_mask	= OMAP4430_CLKSEL_INTERNAL_SOURCE_MASK,
	.ops		= &clkops_null,
	.recalc		= &omap2_clksel_recalc,
};

static const struct clksel func_mcbsp3_gfclk_sel[] = {
	{ .parent = &mcbsp3_sync_mux_ck, .rates = div_1_0_rates },
	{ .parent = &pad_clks_ck, .rates = div_1_1_rates },
	{ .parent = &slimbus_clk, .rates = div_1_2_rates },
	{ .parent = NULL },
};

/* Merged func_mcbsp3_gfclk into mcbsp3 */
static struct clk mcbsp3_fck = {
	.name		= "mcbsp3_fck",
	.parent		= &mcbsp3_sync_mux_ck,
	.clksel		= func_mcbsp3_gfclk_sel,
	.init		= &omap2_init_clksel_parent,
	.clksel_reg	= OMAP4430_CM1_ABE_MCBSP3_CLKCTRL,
	.clksel_mask	= OMAP4430_CLKSEL_SOURCE_MASK,
	.ops		= &clkops_omap2_dflt,
	.recalc		= &omap2_clksel_recalc,
	.enable_reg	= OMAP4430_CM1_ABE_MCBSP3_CLKCTRL,
	.enable_bit	= OMAP4430_MODULEMODE_SWCTRL,
	.clkdm_name	= "abe_clkdm",
};

static struct clk mcbsp4_sync_mux_ck = {
	.name		= "mcbsp4_sync_mux_ck",
	.parent		= &func_96m_fclk,
	.clksel		= mcasp2_fclk_sel,
	.init		= &omap2_init_clksel_parent,
	.clksel_reg	= OMAP4430_CM_L4PER_MCBSP4_CLKCTRL,
	.clksel_mask	= OMAP4430_CLKSEL_INTERNAL_SOURCE_MASK,
	.ops		= &clkops_null,
	.recalc		= &omap2_clksel_recalc,
};

static const struct clksel per_mcbsp4_gfclk_sel[] = {
	{ .parent = &mcbsp4_sync_mux_ck, .rates = div_1_0_rates },
	{ .parent = &pad_clks_ck, .rates = div_1_1_rates },
	{ .parent = NULL },
};

/* Merged per_mcbsp4_gfclk into mcbsp4 */
static struct clk mcbsp4_fck = {
	.name		= "mcbsp4_fck",
	.parent		= &mcbsp4_sync_mux_ck,
	.clksel		= per_mcbsp4_gfclk_sel,
	.init		= &omap2_init_clksel_parent,
	.clksel_reg	= OMAP4430_CM_L4PER_MCBSP4_CLKCTRL,
	.clksel_mask	= OMAP4430_CLKSEL_SOURCE_24_24_MASK,
	.ops		= &clkops_omap2_dflt,
	.recalc		= &omap2_clksel_recalc,
	.enable_reg	= OMAP4430_CM_L4PER_MCBSP4_CLKCTRL,
	.enable_bit	= OMAP4430_MODULEMODE_SWCTRL,
	.clkdm_name	= "l4_per_clkdm",
};

static struct clk mcspi1_fck = {
	.name		= "mcspi1_fck",
	.ops		= &clkops_omap2_dflt,
	.enable_reg	= OMAP4430_CM_L4PER_MCSPI1_CLKCTRL,
	.enable_bit	= OMAP4430_MODULEMODE_SWCTRL,
	.clkdm_name	= "l4_per_clkdm",
	.parent		= &func_48m_fclk,
	.recalc		= &followparent_recalc,
};

static struct clk mcspi2_fck = {
	.name		= "mcspi2_fck",
	.ops		= &clkops_omap2_dflt,
	.enable_reg	= OMAP4430_CM_L4PER_MCSPI2_CLKCTRL,
	.enable_bit	= OMAP4430_MODULEMODE_SWCTRL,
	.clkdm_name	= "l4_per_clkdm",
	.parent		= &func_48m_fclk,
	.recalc		= &followparent_recalc,
};

static struct clk mcspi3_fck = {
	.name		= "mcspi3_fck",
	.ops		= &clkops_omap2_dflt,
	.enable_reg	= OMAP4430_CM_L4PER_MCSPI3_CLKCTRL,
	.enable_bit	= OMAP4430_MODULEMODE_SWCTRL,
	.clkdm_name	= "l4_per_clkdm",
	.parent		= &func_48m_fclk,
	.recalc		= &followparent_recalc,
};

static struct clk mcspi4_fck = {
	.name		= "mcspi4_fck",
	.ops		= &clkops_omap2_dflt,
	.enable_reg	= OMAP4430_CM_L4PER_MCSPI4_CLKCTRL,
	.enable_bit	= OMAP4430_MODULEMODE_SWCTRL,
	.clkdm_name	= "l4_per_clkdm",
	.parent		= &func_48m_fclk,
	.recalc		= &followparent_recalc,
};

/* Merged hsmmc1_fclk into mmc1 */
static struct clk mmc1_fck = {
	.name		= "mmc1_fck",
	.parent		= &func_64m_fclk,
	.clksel		= hsmmc6_fclk_sel,
	.init		= &omap2_init_clksel_parent,
	.clksel_reg	= OMAP4430_CM_L3INIT_MMC1_CLKCTRL,
	.clksel_mask	= OMAP4430_CLKSEL_MASK,
	.ops		= &clkops_omap2_dflt,
	.recalc		= &omap2_clksel_recalc,
	.enable_reg	= OMAP4430_CM_L3INIT_MMC1_CLKCTRL,
	.enable_bit	= OMAP4430_MODULEMODE_SWCTRL,
	.clkdm_name	= "l3_init_clkdm",
};

/* Merged hsmmc2_fclk into mmc2 */
static struct clk mmc2_fck = {
	.name		= "mmc2_fck",
	.parent		= &func_64m_fclk,
	.clksel		= hsmmc6_fclk_sel,
	.init		= &omap2_init_clksel_parent,
	.clksel_reg	= OMAP4430_CM_L3INIT_MMC2_CLKCTRL,
	.clksel_mask	= OMAP4430_CLKSEL_MASK,
	.ops		= &clkops_omap2_dflt,
	.recalc		= &omap2_clksel_recalc,
	.enable_reg	= OMAP4430_CM_L3INIT_MMC2_CLKCTRL,
	.enable_bit	= OMAP4430_MODULEMODE_SWCTRL,
	.clkdm_name	= "l3_init_clkdm",
};

static struct clk mmc3_fck = {
	.name		= "mmc3_fck",
	.ops		= &clkops_omap2_dflt,
	.enable_reg	= OMAP4430_CM_L4PER_MMCSD3_CLKCTRL,
	.enable_bit	= OMAP4430_MODULEMODE_SWCTRL,
	.clkdm_name	= "l4_per_clkdm",
	.parent		= &func_48m_fclk,
	.recalc		= &followparent_recalc,
};

static struct clk mmc4_fck = {
	.name		= "mmc4_fck",
	.ops		= &clkops_omap2_dflt,
	.enable_reg	= OMAP4430_CM_L4PER_MMCSD4_CLKCTRL,
	.enable_bit	= OMAP4430_MODULEMODE_SWCTRL,
	.clkdm_name	= "l4_per_clkdm",
	.parent		= &func_48m_fclk,
	.recalc		= &followparent_recalc,
};

static struct clk mmc5_fck = {
	.name		= "mmc5_fck",
	.ops		= &clkops_omap2_dflt,
	.enable_reg	= OMAP4430_CM_L4PER_MMCSD5_CLKCTRL,
	.enable_bit	= OMAP4430_MODULEMODE_SWCTRL,
	.clkdm_name	= "l4_per_clkdm",
	.parent		= &func_48m_fclk,
	.recalc		= &followparent_recalc,
};

static struct clk ocp_wp1_ick = {
	.name		= "ocp_wp1_ick",
	.ops		= &clkops_omap2_dflt,
	.enable_reg	= OMAP4430_CM_L3INSTR_OCP_WP1_CLKCTRL,
	.enable_bit	= OMAP4430_MODULEMODE_HWCTRL,
	.clkdm_name	= "l3_instr_clkdm",
	.parent		= &l3_div_ck,
	.recalc		= &followparent_recalc,
};

static struct clk pdm_fck = {
	.name		= "pdm_fck",
	.ops		= &clkops_omap2_dflt,
	.enable_reg	= OMAP4430_CM1_ABE_PDM_CLKCTRL,
	.enable_bit	= OMAP4430_MODULEMODE_SWCTRL,
	.clkdm_name	= "abe_clkdm",
	.parent		= &pad_clks_ck,
	.recalc		= &followparent_recalc,
};

static struct clk pkaeip29_fck = {
	.name		= "pkaeip29_fck",
	.ops		= &clkops_omap2_dflt,
	.enable_reg	= OMAP4430_CM_L4SEC_PKAEIP29_CLKCTRL,
	.enable_bit	= OMAP4430_MODULEMODE_SWCTRL,
	.clkdm_name	= "l4_secure_clkdm",
	.parent		= &l4_div_ck,
	.recalc		= &followparent_recalc,
};

static struct clk rng_ick = {
	.name		= "rng_ick",
	.ops		= &clkops_omap2_dflt,
	.enable_reg	= OMAP4430_CM_L4SEC_RNG_CLKCTRL,
	.enable_bit	= OMAP4430_MODULEMODE_HWCTRL,
	.clkdm_name	= "l4_secure_clkdm",
	.parent		= &l4_div_ck,
	.recalc		= &followparent_recalc,
};

static struct clk sha2md51_fck = {
	.name		= "sha2md51_fck",
	.ops		= &clkops_omap2_dflt,
	.enable_reg	= OMAP4430_CM_L4SEC_SHA2MD51_CLKCTRL,
	.enable_bit	= OMAP4430_MODULEMODE_SWCTRL,
	.clkdm_name	= "l4_secure_clkdm",
	.parent		= &l3_div_ck,
	.recalc		= &followparent_recalc,
};

static struct clk sl2_ick = {
	.name		= "sl2_ick",
	.ops		= &clkops_omap2_dflt,
	.enable_reg	= OMAP4430_CM_IVAHD_SL2_CLKCTRL,
	.enable_bit	= OMAP4430_MODULEMODE_HWCTRL,
	.clkdm_name	= "ivahd_clkdm",
	.parent		= &dpll_iva_m5_ck,
	.recalc		= &followparent_recalc,
};

static struct clk slimbus1_fck = {
	.name		= "slimbus1_fck",
	.ops		= &clkops_omap2_dflt,
	.enable_reg	= OMAP4430_CM1_ABE_SLIMBUS_CLKCTRL,
	.enable_bit	= OMAP4430_MODULEMODE_SWCTRL,
	.clkdm_name	= "abe_clkdm",
	.parent		= &ocp_abe_iclk,
	.recalc		= &followparent_recalc,
};

static struct clk slimbus2_fck = {
	.name		= "slimbus2_fck",
	.ops		= &clkops_omap2_dflt,
	.enable_reg	= OMAP4430_CM_L4PER_SLIMBUS2_CLKCTRL,
	.enable_bit	= OMAP4430_MODULEMODE_SWCTRL,
	.clkdm_name	= "l4_per_clkdm",
	.parent		= &l4_div_ck,
	.recalc		= &followparent_recalc,
};

static struct clk sr_core_fck = {
	.name		= "sr_core_fck",
	.ops		= &clkops_omap2_dflt,
	.enable_reg	= OMAP4430_CM_ALWON_SR_CORE_CLKCTRL,
	.enable_bit	= OMAP4430_MODULEMODE_SWCTRL,
	.clkdm_name	= "l4_ao_clkdm",
	.parent		= &l4_wkup_clk_mux_ck,
	.recalc		= &followparent_recalc,
};

static struct clk sr_iva_fck = {
	.name		= "sr_iva_fck",
	.ops		= &clkops_omap2_dflt,
	.enable_reg	= OMAP4430_CM_ALWON_SR_IVA_CLKCTRL,
	.enable_bit	= OMAP4430_MODULEMODE_SWCTRL,
	.clkdm_name	= "l4_ao_clkdm",
	.parent		= &l4_wkup_clk_mux_ck,
	.recalc		= &followparent_recalc,
};

static struct clk sr_mpu_fck = {
	.name		= "sr_mpu_fck",
	.ops		= &clkops_omap2_dflt,
	.enable_reg	= OMAP4430_CM_ALWON_SR_MPU_CLKCTRL,
	.enable_bit	= OMAP4430_MODULEMODE_SWCTRL,
	.clkdm_name	= "l4_ao_clkdm",
	.parent		= &l4_wkup_clk_mux_ck,
	.recalc		= &followparent_recalc,
};

static struct clk tesla_ick = {
	.name		= "tesla_ick",
	.ops		= &clkops_omap2_dflt,
	.enable_reg	= OMAP4430_CM_TESLA_TESLA_CLKCTRL,
	.enable_bit	= OMAP4430_MODULEMODE_HWCTRL,
	.clkdm_name	= "tesla_clkdm",
	.parent		= &dpll_iva_m4_ck,
	.recalc		= &followparent_recalc,
};

static struct clk uart1_fck = {
	.name		= "uart1_fck",
	.ops		= &clkops_omap2_dflt,
	.enable_reg	= OMAP4430_CM_L4PER_UART1_CLKCTRL,
	.enable_bit	= OMAP4430_MODULEMODE_SWCTRL,
	.clkdm_name	= "l4_per_clkdm",
	.parent		= &func_48m_fclk,
	.recalc		= &followparent_recalc,
};

static struct clk uart2_fck = {
	.name		= "uart2_fck",
	.ops		= &clkops_omap2_dflt,
	.enable_reg	= OMAP4430_CM_L4PER_UART2_CLKCTRL,
	.enable_bit	= OMAP4430_MODULEMODE_SWCTRL,
	.clkdm_name	= "l4_per_clkdm",
	.parent		= &func_48m_fclk,
	.recalc		= &followparent_recalc,
};

static struct clk uart3_fck = {
	.name		= "uart3_fck",
	.ops		= &clkops_omap2_dflt,
	.enable_reg	= OMAP4430_CM_L4PER_UART3_CLKCTRL,
	.enable_bit	= OMAP4430_MODULEMODE_SWCTRL,
	.clkdm_name	= "l4_per_clkdm",
	.parent		= &func_48m_fclk,
	.recalc		= &followparent_recalc,
};

static struct clk uart4_fck = {
	.name		= "uart4_fck",
	.ops		= &clkops_omap2_dflt,
	.enable_reg	= OMAP4430_CM_L4PER_UART4_CLKCTRL,
	.enable_bit	= OMAP4430_MODULEMODE_SWCTRL,
	.clkdm_name	= "l4_per_clkdm",
	.parent		= &func_48m_fclk,
	.recalc		= &followparent_recalc,
};

static struct clk unipro1_fck = {
	.name		= "unipro1_fck",
	.ops		= &clkops_omap2_dflt,
	.enable_reg	= OMAP4430_CM_L3INIT_UNIPRO1_CLKCTRL,
	.enable_bit	= OMAP4430_MODULEMODE_SWCTRL,
	.clkdm_name	= "l3_init_clkdm",
	.parent		= &func_96m_fclk,
	.recalc		= &followparent_recalc,
};

static struct clk usb_host_fck = {
	.name		= "usb_host_fck",
	.ops		= &clkops_omap2_dflt,
	.enable_reg	= OMAP4430_CM_L3INIT_USB_HOST_CLKCTRL,
	.enable_bit	= OMAP4430_MODULEMODE_SWCTRL,
	.clkdm_name	= "l3_init_clkdm",
	.parent		= &init_60m_fclk,
	.recalc		= &followparent_recalc,
};

static struct clk usb_host_fs_fck = {
	.name		= "usb_host_fs_fck",
	.ops		= &clkops_omap2_dflt,
	.enable_reg	= OMAP4430_CM_L3INIT_USB_HOST_FS_CLKCTRL,
	.enable_bit	= OMAP4430_MODULEMODE_SWCTRL,
	.clkdm_name	= "l3_init_clkdm",
	.parent		= &func_48mc_fclk,
	.recalc		= &followparent_recalc,
};

static struct clk usb_otg_ick = {
	.name		= "usb_otg_ick",
	.ops		= &clkops_omap2_dflt,
	.enable_reg	= OMAP4430_CM_L3INIT_USB_OTG_CLKCTRL,
	.enable_bit	= OMAP4430_MODULEMODE_HWCTRL,
	.clkdm_name	= "l3_init_clkdm",
	.parent		= &l3_div_ck,
	.recalc		= &followparent_recalc,
};

static struct clk usb_tll_ick = {
	.name		= "usb_tll_ick",
	.ops		= &clkops_omap2_dflt,
	.enable_reg	= OMAP4430_CM_L3INIT_USB_TLL_CLKCTRL,
	.enable_bit	= OMAP4430_MODULEMODE_HWCTRL,
	.clkdm_name	= "l3_init_clkdm",
	.parent		= &l4_div_ck,
	.recalc		= &followparent_recalc,
};

static struct clk usbphyocp2scp_ick = {
	.name		= "usbphyocp2scp_ick",
	.ops		= &clkops_omap2_dflt,
	.enable_reg	= OMAP4430_CM_L3INIT_USBPHYOCP2SCP_CLKCTRL,
	.enable_bit	= OMAP4430_MODULEMODE_HWCTRL,
	.clkdm_name	= "l3_init_clkdm",
	.parent		= &l4_div_ck,
	.recalc		= &followparent_recalc,
};

static struct clk usim_fck = {
	.name		= "usim_fck",
	.ops		= &clkops_omap2_dflt,
	.enable_reg	= OMAP4430_CM_WKUP_USIM_CLKCTRL,
	.enable_bit	= OMAP4430_MODULEMODE_SWCTRL,
	.clkdm_name	= "l4_wkup_clkdm",
	.parent		= &sys_32k_ck,
	.recalc		= &followparent_recalc,
};

static struct clk wdt2_fck = {
	.name		= "wdt2_fck",
	.ops		= &clkops_omap2_dflt,
	.enable_reg	= OMAP4430_CM_WKUP_WDT2_CLKCTRL,
	.enable_bit	= OMAP4430_MODULEMODE_SWCTRL,
	.clkdm_name	= "l4_wkup_clkdm",
	.parent		= &sys_32k_ck,
	.recalc		= &followparent_recalc,
};

static struct clk wdt3_fck = {
	.name		= "wdt3_fck",
	.ops		= &clkops_omap2_dflt,
	.enable_reg	= OMAP4430_CM1_ABE_WDT3_CLKCTRL,
	.enable_bit	= OMAP4430_MODULEMODE_SWCTRL,
	.clkdm_name	= "abe_clkdm",
	.parent		= &sys_32k_ck,
	.recalc		= &followparent_recalc,
};

/* Remaining optional clocks */
static const struct clksel otg_60m_gfclk_sel[] = {
	{ .parent = &utmi_phy_clkout_ck, .rates = div_1_0_rates },
	{ .parent = &xclk60motg_ck, .rates = div_1_1_rates },
	{ .parent = NULL },
};

static struct clk otg_60m_gfclk_ck = {
	.name		= "otg_60m_gfclk_ck",
	.parent		= &utmi_phy_clkout_ck,
	.clksel		= otg_60m_gfclk_sel,
	.init		= &omap2_init_clksel_parent,
	.clksel_reg	= OMAP4430_CM_L3INIT_USB_OTG_CLKCTRL,
	.clksel_mask	= OMAP4430_CLKSEL_60M_MASK,
	.ops		= &clkops_null,
	.recalc		= &omap2_clksel_recalc,
};

static const struct clksel stm_clk_div_div[] = {
	{ .parent = &pmd_stm_clock_mux_ck, .rates = div3_1to4_rates },
	{ .parent = NULL },
};

static struct clk stm_clk_div_ck = {
	.name		= "stm_clk_div_ck",
	.parent		= &pmd_stm_clock_mux_ck,
	.clksel		= stm_clk_div_div,
	.clksel_reg	= OMAP4430_CM_EMU_DEBUGSS_CLKCTRL,
	.clksel_mask	= OMAP4430_CLKSEL_PMD_STM_CLK_MASK,
	.ops		= &clkops_null,
	.recalc		= &omap2_clksel_recalc,
	.round_rate	= &omap2_clksel_round_rate,
	.set_rate	= &omap2_clksel_set_rate,
};

static const struct clksel trace_clk_div_div[] = {
	{ .parent = &pmd_trace_clk_mux_ck, .rates = div3_1to4_rates },
	{ .parent = NULL },
};

static struct clk trace_clk_div_ck = {
	.name		= "trace_clk_div_ck",
	.parent		= &pmd_trace_clk_mux_ck,
	.clksel		= trace_clk_div_div,
	.clksel_reg	= OMAP4430_CM_EMU_DEBUGSS_CLKCTRL,
	.clksel_mask	= OMAP4430_CLKSEL_PMD_TRACE_CLK_MASK,
	.ops		= &clkops_null,
	.recalc		= &omap2_clksel_recalc,
	.round_rate	= &omap2_clksel_round_rate,
	.set_rate	= &omap2_clksel_set_rate,
};

static const struct clksel_rate div2_14to18_rates[] = {
	{ .div = 14, .val = 0, .flags = RATE_IN_4430 },
	{ .div = 18, .val = 1, .flags = RATE_IN_4430 },
	{ .div = 0 },
};

static const struct clksel usim_fclk_div[] = {
	{ .parent = &dpll_per_m4_ck, .rates = div2_14to18_rates },
	{ .parent = NULL },
};

static struct clk usim_fclk = {
	.name		= "usim_fclk",
	.parent		= &dpll_per_m4_ck,
	.clksel		= usim_fclk_div,
	.clksel_reg	= OMAP4430_CM_WKUP_USIM_CLKCTRL,
	.clksel_mask	= OMAP4430_CLKSEL_DIV_MASK,
	.ops		= &clkops_null,
	.recalc		= &omap2_clksel_recalc,
	.round_rate	= &omap2_clksel_round_rate,
	.set_rate	= &omap2_clksel_set_rate,
};

static const struct clksel utmi_p1_gfclk_sel[] = {
	{ .parent = &init_60m_fclk, .rates = div_1_0_rates },
	{ .parent = &xclk60mhsp1_ck, .rates = div_1_1_rates },
	{ .parent = NULL },
};

static struct clk utmi_p1_gfclk_ck = {
	.name		= "utmi_p1_gfclk_ck",
	.parent		= &init_60m_fclk,
	.clksel		= utmi_p1_gfclk_sel,
	.init		= &omap2_init_clksel_parent,
	.clksel_reg	= OMAP4430_CM_L3INIT_USB_HOST_CLKCTRL,
	.clksel_mask	= OMAP4430_CLKSEL_UTMI_P1_MASK,
	.ops		= &clkops_null,
	.recalc		= &omap2_clksel_recalc,
};

static const struct clksel utmi_p2_gfclk_sel[] = {
	{ .parent = &init_60m_fclk, .rates = div_1_0_rates },
	{ .parent = &xclk60mhsp2_ck, .rates = div_1_1_rates },
	{ .parent = NULL },
};

static struct clk utmi_p2_gfclk_ck = {
	.name		= "utmi_p2_gfclk_ck",
	.parent		= &init_60m_fclk,
	.clksel		= utmi_p2_gfclk_sel,
	.init		= &omap2_init_clksel_parent,
	.clksel_reg	= OMAP4430_CM_L3INIT_USB_HOST_CLKCTRL,
	.clksel_mask	= OMAP4430_CLKSEL_UTMI_P2_MASK,
	.ops		= &clkops_null,
	.recalc		= &omap2_clksel_recalc,
};


static struct omap_clk omap44xx_clks[] = {
	CLK(NULL,	"extalt_clkin_ck",		&extalt_clkin_ck,	CK_443X),
	CLK(NULL,	"pad_clks_ck",			&pad_clks_ck,	CK_443X),
	CLK(NULL,	"pad_slimbus_core_clks_ck",	&pad_slimbus_core_clks_ck,	CK_443X),
	CLK(NULL,	"secure_32k_clk_src_ck",	&secure_32k_clk_src_ck,	CK_443X),
	CLK(NULL,	"slimbus_clk",			&slimbus_clk,	CK_443X),
	CLK(NULL,	"sys_32k_ck",			&sys_32k_ck,	CK_443X),
	CLK(NULL,	"virt_12000000_ck",		&virt_12000000_ck,	CK_443X),
	CLK(NULL,	"virt_13000000_ck",		&virt_13000000_ck,	CK_443X),
	CLK(NULL,	"virt_16800000_ck",		&virt_16800000_ck,	CK_443X),
	CLK(NULL,	"virt_19200000_ck",		&virt_19200000_ck,	CK_443X),
	CLK(NULL,	"virt_26000000_ck",		&virt_26000000_ck,	CK_443X),
	CLK(NULL,	"virt_27000000_ck",		&virt_27000000_ck,	CK_443X),
	CLK(NULL,	"virt_38400000_ck",		&virt_38400000_ck,	CK_443X),
	CLK(NULL,	"sys_clkin_ck",			&sys_clkin_ck,	CK_443X),
	CLK(NULL,	"utmi_phy_clkout_ck",		&utmi_phy_clkout_ck,	CK_443X),
	CLK(NULL,	"xclk60mhsp1_ck",		&xclk60mhsp1_ck,	CK_443X),
	CLK(NULL,	"xclk60mhsp2_ck",		&xclk60mhsp2_ck,	CK_443X),
	CLK(NULL,	"xclk60motg_ck",		&xclk60motg_ck,	CK_443X),
	CLK(NULL,	"dpll_sys_ref_clk",		&dpll_sys_ref_clk,	CK_443X),
	CLK(NULL,	"abe_dpll_refclk_mux_ck",	&abe_dpll_refclk_mux_ck,	CK_443X),
	CLK(NULL,	"dpll_abe_ck",			&dpll_abe_ck,	CK_443X),
	CLK(NULL,	"dpll_abe_m2x2_ck",		&dpll_abe_m2x2_ck,	CK_443X),
	CLK(NULL,	"abe_24m_fclk",			&abe_24m_fclk,	CK_443X),
	CLK(NULL,	"abe_clk",			&abe_clk,	CK_443X),
	CLK(NULL,	"aess_fclk",			&aess_fclk,	CK_443X),
	CLK(NULL,	"dpll_abe_m3_ck",		&dpll_abe_m3_ck,	CK_443X),
	CLK(NULL,	"core_hsd_byp_clk_mux_ck",	&core_hsd_byp_clk_mux_ck,	CK_443X),
	CLK(NULL,	"dpll_core_ck",			&dpll_core_ck,	CK_443X),
	CLK(NULL,	"dpll_core_m6_ck",		&dpll_core_m6_ck,	CK_443X),
	CLK(NULL,	"dbgclk_mux_ck",		&dbgclk_mux_ck,	CK_443X),
	CLK(NULL,	"dpll_core_m2_ck",		&dpll_core_m2_ck,	CK_443X),
	CLK(NULL,	"ddrphy_ck",			&ddrphy_ck,	CK_443X),
	CLK(NULL,	"dpll_core_m5_ck",		&dpll_core_m5_ck,	CK_443X),
	CLK(NULL,	"div_core_ck",			&div_core_ck,	CK_443X),
	CLK(NULL,	"div_iva_hs_clk",		&div_iva_hs_clk,	CK_443X),
	CLK(NULL,	"div_mpu_hs_clk",		&div_mpu_hs_clk,	CK_443X),
	CLK(NULL,	"dpll_core_m4_ck",		&dpll_core_m4_ck,	CK_443X),
	CLK(NULL,	"dll_clk_div_ck",		&dll_clk_div_ck,	CK_443X),
	CLK(NULL,	"dpll_abe_m2_ck",		&dpll_abe_m2_ck,	CK_443X),
	CLK(NULL,	"dpll_core_m3_ck",		&dpll_core_m3_ck,	CK_443X),
	CLK(NULL,	"dpll_core_m7_ck",		&dpll_core_m7_ck,	CK_443X),
	CLK(NULL,	"iva_hsd_byp_clk_mux_ck",	&iva_hsd_byp_clk_mux_ck,	CK_443X),
	CLK(NULL,	"dpll_iva_ck",			&dpll_iva_ck,	CK_443X),
	CLK(NULL,	"dpll_iva_m4_ck",		&dpll_iva_m4_ck,	CK_443X),
	CLK(NULL,	"dpll_iva_m5_ck",		&dpll_iva_m5_ck,	CK_443X),
	CLK(NULL,	"dpll_mpu_ck",			&dpll_mpu_ck,	CK_443X),
	CLK(NULL,	"dpll_mpu_m2_ck",		&dpll_mpu_m2_ck,	CK_443X),
	CLK(NULL,	"per_hs_clk_div_ck",		&per_hs_clk_div_ck,	CK_443X),
	CLK(NULL,	"per_hsd_byp_clk_mux_ck",	&per_hsd_byp_clk_mux_ck,	CK_443X),
	CLK(NULL,	"dpll_per_ck",			&dpll_per_ck,	CK_443X),
	CLK(NULL,	"dpll_per_m2_ck",		&dpll_per_m2_ck,	CK_443X),
	CLK(NULL,	"dpll_per_m2x2_ck",		&dpll_per_m2x2_ck,	CK_443X),
	CLK(NULL,	"dpll_per_m3_ck",		&dpll_per_m3_ck,	CK_443X),
	CLK(NULL,	"dpll_per_m4_ck",		&dpll_per_m4_ck,	CK_443X),
	CLK(NULL,	"dpll_per_m5_ck",		&dpll_per_m5_ck,	CK_443X),
	CLK(NULL,	"dpll_per_m6_ck",		&dpll_per_m6_ck,	CK_443X),
	CLK(NULL,	"dpll_per_m7_ck",		&dpll_per_m7_ck,	CK_443X),
	CLK(NULL,	"dpll_unipro_ck",		&dpll_unipro_ck,	CK_443X),
	CLK(NULL,	"dpll_unipro_m2x2_ck",		&dpll_unipro_m2x2_ck,	CK_443X),
	CLK(NULL,	"usb_hs_clk_div_ck",		&usb_hs_clk_div_ck,	CK_443X),
	CLK(NULL,	"dpll_usb_ck",			&dpll_usb_ck,	CK_443X),
	CLK(NULL,	"dpll_usb_clkdcoldo_ck",	&dpll_usb_clkdcoldo_ck,	CK_443X),
	CLK(NULL,	"dpll_usb_m2_ck",		&dpll_usb_m2_ck,	CK_443X),
	CLK(NULL,	"ducati_clk_mux_ck",		&ducati_clk_mux_ck,	CK_443X),
	CLK(NULL,	"func_12m_fclk",		&func_12m_fclk,	CK_443X),
	CLK(NULL,	"func_24m_clk",			&func_24m_clk,	CK_443X),
	CLK(NULL,	"func_24mc_fclk",		&func_24mc_fclk,	CK_443X),
	CLK(NULL,	"func_48m_fclk",		&func_48m_fclk,	CK_443X),
	CLK(NULL,	"func_48mc_fclk",		&func_48mc_fclk,	CK_443X),
	CLK(NULL,	"func_64m_fclk",		&func_64m_fclk,	CK_443X),
	CLK(NULL,	"func_96m_fclk",		&func_96m_fclk,	CK_443X),
	CLK(NULL,	"hsmmc6_fclk",			&hsmmc6_fclk,	CK_443X),
	CLK(NULL,	"init_60m_fclk",		&init_60m_fclk,	CK_443X),
	CLK(NULL,	"l3_div_ck",			&l3_div_ck,	CK_443X),
	CLK(NULL,	"l4_div_ck",			&l4_div_ck,	CK_443X),
	CLK(NULL,	"lp_clk_div_ck",		&lp_clk_div_ck,	CK_443X),
	CLK(NULL,	"l4_wkup_clk_mux_ck",		&l4_wkup_clk_mux_ck,	CK_443X),
	CLK(NULL,	"per_abe_nc_fclk",		&per_abe_nc_fclk,	CK_443X),
	CLK(NULL,	"mcasp2_fclk",			&mcasp2_fclk,	CK_443X),
	CLK(NULL,	"mcasp3_fclk",			&mcasp3_fclk,	CK_443X),
	CLK(NULL,	"ocp_abe_iclk",			&ocp_abe_iclk,	CK_443X),
	CLK(NULL,	"per_abe_24m_fclk",		&per_abe_24m_fclk,	CK_443X),
	CLK(NULL,	"pmd_stm_clock_mux_ck",		&pmd_stm_clock_mux_ck,	CK_443X),
	CLK(NULL,	"pmd_trace_clk_mux_ck",		&pmd_trace_clk_mux_ck,	CK_443X),
	CLK(NULL,	"syc_clk_div_ck",		&syc_clk_div_ck,	CK_443X),
	CLK(NULL,	"aes1_fck",			&aes1_fck,	CK_443X),
	CLK(NULL,	"aes2_fck",			&aes2_fck,	CK_443X),
	CLK(NULL,	"aess_fck",			&aess_fck,	CK_443X),
	CLK(NULL,	"cust_efuse_fck",		&cust_efuse_fck,	CK_443X),
	CLK(NULL,	"des3des_fck",			&des3des_fck,	CK_443X),
	CLK(NULL,	"dmic_sync_mux_ck",		&dmic_sync_mux_ck,	CK_443X),
	CLK(NULL,	"dmic_fck",			&dmic_fck,	CK_443X),
	CLK(NULL,	"dss_fck",			&dss_fck,	CK_443X),
	CLK(NULL,	"ducati_ick",			&ducati_ick,	CK_443X),
	CLK(NULL,	"emif1_ick",			&emif1_ick,	CK_443X),
	CLK(NULL,	"emif2_ick",			&emif2_ick,	CK_443X),
	CLK(NULL,	"fdif_fck",			&fdif_fck,	CK_443X),
	CLK(NULL,	"per_sgx_fclk",			&per_sgx_fclk,	CK_443X),
	CLK(NULL,	"gfx_fck",			&gfx_fck,	CK_443X),
	CLK(NULL,	"gpio1_ick",			&gpio1_ick,	CK_443X),
	CLK(NULL,	"gpio2_ick",			&gpio2_ick,	CK_443X),
	CLK(NULL,	"gpio3_ick",			&gpio3_ick,	CK_443X),
	CLK(NULL,	"gpio4_ick",			&gpio4_ick,	CK_443X),
	CLK(NULL,	"gpio5_ick",			&gpio5_ick,	CK_443X),
	CLK(NULL,	"gpio6_ick",			&gpio6_ick,	CK_443X),
	CLK(NULL,	"gpmc_ick",			&gpmc_ick,	CK_443X),
	CLK(NULL,	"gpt1_fck",			&gpt1_fck,	CK_443X),
	CLK(NULL,	"gpt10_fck",			&gpt10_fck,	CK_443X),
	CLK(NULL,	"gpt11_fck",			&gpt11_fck,	CK_443X),
	CLK(NULL,	"gpt2_fck",			&gpt2_fck,	CK_443X),
	CLK(NULL,	"gpt3_fck",			&gpt3_fck,	CK_443X),
	CLK(NULL,	"gpt4_fck",			&gpt4_fck,	CK_443X),
	CLK(NULL,	"gpt5_fck",			&gpt5_fck,	CK_443X),
	CLK(NULL,	"gpt6_fck",			&gpt6_fck,	CK_443X),
	CLK(NULL,	"gpt7_fck",			&gpt7_fck,	CK_443X),
	CLK(NULL,	"gpt8_fck",			&gpt8_fck,	CK_443X),
	CLK(NULL,	"gpt9_fck",			&gpt9_fck,	CK_443X),
	CLK("omap2_hdq.0",	"fck",				&hdq1w_fck,	CK_443X),
	CLK(NULL,	"hsi_ick",			&hsi_ick,	CK_443X),
	CLK("i2c_omap.1",	"fck",				&i2c1_fck,	CK_443X),
	CLK("i2c_omap.2",	"fck",				&i2c2_fck,	CK_443X),
	CLK("i2c_omap.3",	"fck",				&i2c3_fck,	CK_443X),
	CLK("i2c_omap.4",	"fck",				&i2c4_fck,	CK_443X),
	CLK(NULL,	"iss_fck",			&iss_fck,	CK_443X),
	CLK(NULL,	"ivahd_ick",			&ivahd_ick,	CK_443X),
	CLK(NULL,	"keyboard_fck",			&keyboard_fck,	CK_443X),
	CLK(NULL,	"l3_instr_interconnect_ick",	&l3_instr_interconnect_ick,	CK_443X),
	CLK(NULL,	"l3_interconnect_3_ick",	&l3_interconnect_3_ick,	CK_443X),
	CLK(NULL,	"mcasp_sync_mux_ck",		&mcasp_sync_mux_ck,	CK_443X),
	CLK(NULL,	"mcasp_fck",			&mcasp_fck,	CK_443X),
	CLK(NULL,	"mcbsp1_sync_mux_ck",		&mcbsp1_sync_mux_ck,	CK_443X),
	CLK("omap-mcbsp.1",	"fck",				&mcbsp1_fck,	CK_443X),
	CLK(NULL,	"mcbsp2_sync_mux_ck",		&mcbsp2_sync_mux_ck,	CK_443X),
	CLK("omap-mcbsp.2",	"fck",				&mcbsp2_fck,	CK_443X),
	CLK(NULL,	"mcbsp3_sync_mux_ck",		&mcbsp3_sync_mux_ck,	CK_443X),
	CLK("omap-mcbsp.3",	"fck",				&mcbsp3_fck,	CK_443X),
	CLK(NULL,	"mcbsp4_sync_mux_ck",		&mcbsp4_sync_mux_ck,	CK_443X),
	CLK("omap-mcbsp.4",	"fck",				&mcbsp4_fck,	CK_443X),
	CLK("omap2_mcspi.1",	"fck",				&mcspi1_fck,	CK_443X),
	CLK("omap2_mcspi.2",	"fck",				&mcspi2_fck,	CK_443X),
	CLK("omap2_mcspi.3",	"fck",				&mcspi3_fck,	CK_443X),
	CLK("omap2_mcspi.4",	"fck",				&mcspi4_fck,	CK_443X),
	CLK("mmci-omap-hs.0",	"fck",				&mmc1_fck,	CK_443X),
	CLK("mmci-omap-hs.1",	"fck",				&mmc2_fck,	CK_443X),
	CLK("mmci-omap-hs.2",	"fck",				&mmc3_fck,	CK_443X),
	CLK("mmci-omap-hs.3",	"fck",				&mmc4_fck,	CK_443X),
	CLK("mmci-omap-hs.4",	"fck",				&mmc5_fck,	CK_443X),
	CLK(NULL,	"ocp_wp1_ick",			&ocp_wp1_ick,	CK_443X),
	CLK(NULL,	"pdm_fck",			&pdm_fck,	CK_443X),
	CLK(NULL,	"pkaeip29_fck",			&pkaeip29_fck,	CK_443X),
	CLK("omap_rng",	"ick",				&rng_ick,	CK_443X),
	CLK(NULL,	"sha2md51_fck",			&sha2md51_fck,	CK_443X),
	CLK(NULL,	"sl2_ick",			&sl2_ick,	CK_443X),
	CLK(NULL,	"slimbus1_fck",			&slimbus1_fck,	CK_443X),
	CLK(NULL,	"slimbus2_fck",			&slimbus2_fck,	CK_443X),
	CLK(NULL,	"sr_core_fck",			&sr_core_fck,	CK_443X),
	CLK(NULL,	"sr_iva_fck",			&sr_iva_fck,	CK_443X),
	CLK(NULL,	"sr_mpu_fck",			&sr_mpu_fck,	CK_443X),
	CLK(NULL,	"tesla_ick",			&tesla_ick,	CK_443X),
	CLK(NULL,	"uart1_fck",			&uart1_fck,	CK_443X),
	CLK(NULL,	"uart2_fck",			&uart2_fck,	CK_443X),
	CLK(NULL,	"uart3_fck",			&uart3_fck,	CK_443X),
	CLK(NULL,	"uart4_fck",			&uart4_fck,	CK_443X),
	CLK(NULL,	"unipro1_fck",			&unipro1_fck,	CK_443X),
	CLK(NULL,	"usb_host_fck",			&usb_host_fck,	CK_443X),
	CLK(NULL,	"usb_host_fs_fck",		&usb_host_fs_fck,	CK_443X),
	CLK("musb_hdrc",	"ick",				&usb_otg_ick,	CK_443X),
	CLK(NULL,	"usb_tll_ick",			&usb_tll_ick,	CK_443X),
	CLK(NULL,	"usbphyocp2scp_ick",		&usbphyocp2scp_ick,	CK_443X),
	CLK(NULL,	"usim_fck",			&usim_fck,	CK_443X),
	CLK("omap_wdt",	"fck",				&wdt2_fck,	CK_443X),
	CLK(NULL,	"wdt3_fck",			&wdt3_fck,	CK_443X),
	CLK(NULL,	"otg_60m_gfclk_ck",		&otg_60m_gfclk_ck,	CK_443X),
	CLK(NULL,	"stm_clk_div_ck",		&stm_clk_div_ck,	CK_443X),
	CLK(NULL,	"trace_clk_div_ck",		&trace_clk_div_ck,	CK_443X),
	CLK(NULL,	"usim_fclk",			&usim_fclk,	CK_443X),
	CLK(NULL,	"utmi_p1_gfclk_ck",		&utmi_p1_gfclk_ck,	CK_443X),
	CLK(NULL,	"utmi_p2_gfclk_ck",		&utmi_p2_gfclk_ck,	CK_443X),
	CLK(NULL,	"gpio1_dbck",			&dummy_ck,	CK_443X),
	CLK(NULL,	"gpio2_dbck",			&dummy_ck,	CK_443X),
	CLK(NULL,	"gpio3_dbck",			&dummy_ck,	CK_443X),
	CLK(NULL,	"gpio4_dbck",			&dummy_ck,	CK_443X),
	CLK(NULL,	"gpio5_dbck",			&dummy_ck,	CK_443X),
	CLK(NULL,	"gpio6_dbck",			&dummy_ck,	CK_443X),
	CLK(NULL,	"gpmc_ck",			&dummy_ck,	CK_443X),
	CLK(NULL,	"gpt1_ick",			&dummy_ck,	CK_443X),
	CLK(NULL,	"gpt2_ick",			&dummy_ck,	CK_443X),
	CLK(NULL,	"gpt3_ick",			&dummy_ck,	CK_443X),
	CLK(NULL,	"gpt4_ick",			&dummy_ck,	CK_443X),
	CLK(NULL,	"gpt5_ick",			&dummy_ck,	CK_443X),
	CLK(NULL,	"gpt6_ick",			&dummy_ck,	CK_443X),
	CLK(NULL,	"gpt7_ick",			&dummy_ck,	CK_443X),
	CLK(NULL,	"gpt8_ick",			&dummy_ck,	CK_443X),
	CLK(NULL,	"gpt9_ick",			&dummy_ck,	CK_443X),
	CLK(NULL,	"gpt10_ick",			&dummy_ck,	CK_443X),
	CLK(NULL,	"gpt11_ick",			&dummy_ck,	CK_443X),
	CLK("i2c_omap.1",	"ick",				&dummy_ck,	CK_443X),
	CLK("i2c_omap.2",	"ick",				&dummy_ck,	CK_443X),
	CLK("i2c_omap.3",	"ick",				&dummy_ck,	CK_443X),
	CLK("i2c_omap.4",	"ick",				&dummy_ck,	CK_443X),
	CLK("omap-mcbsp.1",	"ick",				&dummy_ck,	CK_443X),
	CLK("omap-mcbsp.2",	"ick",				&dummy_ck,	CK_443X),
	CLK("omap-mcbsp.3",	"ick",				&dummy_ck,	CK_443X),
	CLK("omap-mcbsp.4",	"ick",				&dummy_ck,	CK_443X),
	CLK("omap2_mcspi.1",	"ick",			&dummy_ck,	CK_443X),
	CLK("omap2_mcspi.2",	"ick",			&dummy_ck,	CK_443X),
	CLK("omap2_mcspi.3",	"ick",			&dummy_ck,	CK_443X),
	CLK("omap2_mcspi.4",	"ick",			&dummy_ck,	CK_443X),
	CLK("mmci-omap-hs.0",	"ick",		&dummy_ck,	CK_443X),
	CLK("mmci-omap-hs.1",	"ick",		&dummy_ck,	CK_443X),
	CLK("mmci-omap-hs.2",	"ick",		&dummy_ck,	CK_443X),
	CLK("mmci-omap-hs.3",	"ick",		&dummy_ck,	CK_443X),
	CLK("mmci-omap-hs.4",	"ick",		&dummy_ck,	CK_443X),
	CLK(NULL,	"uart1_ick",			&dummy_ck,	CK_443X),
	CLK(NULL,	"uart2_ick",			&dummy_ck,	CK_443X),
	CLK(NULL,	"uart3_ick",			&dummy_ck,	CK_443X),
	CLK(NULL,	"uart4_ick",			&dummy_ck,	CK_443X),
	CLK("omap_wdt",	"ick",				&dummy_ck,	CK_443X),
};

int __init omap4xxx_clk_init(void)
{
	struct omap_clk *c;
	u32 cpu_clkflg;

	if (cpu_is_omap44xx()) {
		cpu_mask = RATE_IN_4430;
		cpu_clkflg = CK_443X;
	}

	clk_init(&omap2_clk_functions);

	for (c = omap44xx_clks; c < omap44xx_clks + ARRAY_SIZE(omap44xx_clks);
									  c++)
		clk_preinit(c->lk.clk);

	for (c = omap44xx_clks; c < omap44xx_clks + ARRAY_SIZE(omap44xx_clks);
									  c++)
		if (c->cpu & cpu_clkflg) {
			clkdev_add(&c->lk);
			clk_register(c->lk.clk);
			omap2_init_clk_clkdm(c->lk.clk);
		}

	recalculate_root_clocks();

	/*
	 * Only enable those clocks we will need, let the drivers
	 * enable other clocks as necessary
	 */
	clk_enable_init_clocks();

	return 0;
}