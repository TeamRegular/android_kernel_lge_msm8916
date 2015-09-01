/* Copyright (c) 2013-2014, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/kernel.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_platform.h>
#include <linux/of_fdt.h>
#include <linux/of_irq.h>
#include <asm/mach/arch.h>
#include <soc/qcom/socinfo.h>
#include <mach/board.h>
#include <mach/msm_memtypes.h>
#include <soc/qcom/rpm-smd.h>
#include <soc/qcom/smd.h>
#include <soc/qcom/smem.h>
#include <soc/qcom/spm.h>
#include <soc/qcom/pm.h>
#include "../board-dt.h"
#include "../platsmp.h"

#include <mach/board_lge.h>

#ifdef CONFIG_LGE_LCD_TUNING
#include "../../../../drivers/video/msm/mdss/mdss_dsi.h"
int tun_lcd[128];

int lcd_set_values(int *tun_lcd_t)
{
	memset(tun_lcd,0,128*sizeof(int));
	memcpy(tun_lcd,tun_lcd_t,128*sizeof(int));
	printk("lcd_set_values ::: tun_lcd[0]=[%x], tun_lcd[1]=[%x], tun_lcd[2]=[%x] ......\n"
			,tun_lcd[0],tun_lcd[1],tun_lcd[2]);
	return 0;
}
static int lcd_get_values(int *tun_lcd_t)
{
	memset(tun_lcd_t,0,128*sizeof(int));
	memcpy(tun_lcd_t,tun_lcd,128*sizeof(int));
	printk("lcd_get_values\n");
	return 0;
}

static struct lcd_platform_data lcd_pdata ={
	.set_values = lcd_set_values,
	.get_values = lcd_get_values,
};
static struct platform_device lcd_ctrl_device = {
	.name = "lcd_ctrl",
	.dev = {
	.platform_data = &lcd_pdata,
	}
};

void __init lge_add_lcd_ctrl_devices(void)
{
	platform_device_register(&lcd_ctrl_device);
}
#endif

static void __init msm8916_dt_reserve(void)
{
	of_scan_flat_dt(dt_scan_for_memory_reserve, NULL);
#ifdef CONFIG_MACH_LGE
	of_scan_flat_dt(lge_init_dt_scan_chosen, NULL);
#endif
}

static void __init msm8916_map_io(void)
{
	msm_map_msm8916_io();
}

static struct of_dev_auxdata msm8916_auxdata_lookup[] __initdata = {
	{}
};

/*
 * Used to satisfy dependencies for devices that need to be
 * run early or in a particular order. Most likely your device doesn't fall
 * into this category, and thus the driver should not be added here. The
 * EPROBE_DEFER can satisfy most dependency problems.
 */
void __init msm8916_add_drivers(void)
{
	msm_smd_init();
	msm_rpm_driver_init();
	msm_spm_device_init();
	msm_pm_sleep_status_init();
#ifdef CONFIG_LGE_LCD_TUNING
	 lge_add_lcd_ctrl_devices();
#endif
#ifdef CONFIG_USB_G_LGE_ANDROID
	 lge_add_android_usb_devices();
#endif
#ifdef CONFIG_LGE_DIAG_USB_ACCESS_LOCK
	 lge_add_diag_devices();
#endif
}

static void __init msm8916_init(void)
{
	struct of_dev_auxdata *adata = msm8916_auxdata_lookup;

	/*
	 * populate devices from DT first so smem probe will get called as part
	 * of msm_smem_init.  socinfo_init needs smem support so call
	 * msm_smem_init before it.
	 */
	of_platform_populate(NULL, of_default_bus_match_table, adata, NULL);
	msm_smem_init();

	if (socinfo_init() < 0)
		pr_err("%s: socinfo_init() failed\n", __func__);

	msm8916_add_drivers();
}

static const char *msm8916_dt_match[] __initconst = {
	"qcom,msm8916",
	NULL
};

DT_MACHINE_START(MSM8916_DT, "Qualcomm MSM 8916 (Flattened Device Tree)")
	.map_io = msm8916_map_io,
	.init_machine = msm8916_init,
	.dt_compat = msm8916_dt_match,
	.reserve = msm8916_dt_reserve,
	.smp = &msm8916_smp_ops,
MACHINE_END
