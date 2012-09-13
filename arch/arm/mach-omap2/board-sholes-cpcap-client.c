/*
 * arch/arm/mach-omap2/board-sholes-cpcap-client.c
 *
 * Copyright (C) 2009-2010 Motorola, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/irq.h>
#include <linux/platform_device.h>
#include <linux/spi/cpcap.h>
#include <linux/cpcap_audio_platform_data.h>
#include <linux/pm_dbg.h>
#include <mach/gpio.h>
#include <plat/mux.h>
#include <plat/resource.h>
#include <plat/omap34xx.h>
#include <linux/leds-cpcap-abmode.h>
#include <linux/leds-cpcap-adb.h>
#include <linux/leds-cpcap-gpio.h>
#include <linux/leds-cpcap-kpb.h>
#include <linux/leds-cpcap-mdb.h>
#include <linux/leds-cpcap-rgb.h>

/*
 * CPCAP devcies are common for different HW Rev.
 *
 */
static struct platform_device cpcap_3mm5_device = {
	.name   = "cpcap_3mm5",
	.id     = -1,
	.dev    = {
		.platform_data  = NULL,
	},
};

#ifdef CONFIG_CPCAP_USB
static struct platform_device cpcap_usb_device = {
	.name           = "cpcap_usb",
	.id             = -1,
	.dev.platform_data = NULL,
};

static struct platform_device cpcap_usb_det_device = {
	.name   = "cpcap_usb_det",
	.id     = -1,
	.dev    = {
		.platform_data  = NULL,
	},
};
#endif /* CONFIG_CPCAP_USB */

#ifdef CONFIG_TTA_CHARGER
static struct platform_device cpcap_tta_det_device = {
	.name   = "cpcap_tta_charger",
	.id     = -1,
	.dev    = {
		.platform_data  = NULL,
	},
};
#endif

#ifdef CONFIG_SOUND_CPCAP_OMAP
static struct platform_device cpcap_audio_device = {
	.name           = "cpcap_audio",
	.id             = -1,
	.dev.platform_data  = NULL,
};
#endif

#ifdef CONFIG_LEDS_AF_LED
static struct platform_device cpcap_af_led = {
	.name           = LD_AF_LED_DEV,
	.id             = -1,
	.dev            = {
		.platform_data  = NULL,
       },
};
#endif

static struct platform_device cpcap_bd7885 = {
	.name           = "bd7885",
	.id             = -1,
	.dev            = {
		.platform_data  = NULL,
       },
};

static struct platform_device cpcap_vio_active_device = {
	.name		= "cpcap_vio_active",
	.id		= -1,
	.dev		= {
		.platform_data = NULL,
	},
};

#ifdef CONFIG_PM_DBG_DRV
static struct platform_device cpcap_pm_dbg_device = {
	.name		= "cpcap_pm_dbg",
	.id		= -1,
	.dev		= {
		.platform_data = NULL,
	},
};

static struct pm_dbg_drvdata cpcap_pm_dbg_drvdata = {
	.pm_cd_factor = 1000,
};
#endif

static struct platform_device *cpcap_devices[] = {
#ifdef CONFIG_CPCAP_USB
	&cpcap_usb_device,
	&cpcap_usb_det_device,
#endif
#ifdef CONFIG_SOUND_CPCAP_OMAP
	&cpcap_audio_device,
#endif
	&cpcap_3mm5_device,
#ifdef CONFIG_TTA_CHARGER
	&cpcap_tta_det_device,
#endif
#ifdef CONFIG_LEDS_AF_LED
	&cpcap_af_led,
#endif
	&cpcap_bd7885
};


/*
 * CPCAP devcies whose availability depends on HW
 *
 */
static struct platform_device cpcap_lm3554 = {
	.name           = "flash-torch",
	.id             = -1,
	.dev            = {
	.platform_data  = NULL,
	},
};

static struct platform_device cpcap_lm3559 = {
	.name           = "flash-torch-3559",
	.id             = -1,
	.dev            = {
		.platform_data  = NULL,
	},
};

static struct cpcap_abmode_config_data abmode_config_data = {
	.abmode_init = CPCAP_ABMODE_INIT,
};

static struct cpcap_adb_led_config_data adb_led_data = {
	.init = CPCAP_ADB_INIT,
	.on = CPCAP_ADB_ON,
	.abmode_config = &abmode_config_data,
	.class_name = CPCAP_ADB_LED_CLASS_NAME,
};

static struct platform_device cpcap_adb_led = {
	.name = CPCAP_ADB_LED_DRV_NAME,
	.id             = -1,
	.dev            = {
	.platform_data  = &adb_led_data,
	},
};

static struct cpcap_gpio_led_config_data gpio_led_data = {
	.reg = CPCAP_REG_GPIO6,
	.init_mask = CPCAP_GPIO_INIT_MASK,
	.init = CPCAP_GPIO_INIT,
	.class_name = "keyboard-backlight",
};

static struct platform_device cpcap_gpio_led = {
	.name = CPCAP_GPIO_LED_DRV_NAME,
	.id             = -1,
	.dev            = {
	.platform_data  = &gpio_led_data,
	},
};

static struct cpcap_kpb_led_config_data kpb_led_data = {
	.init = CPCAP_KPB_INIT,
	.on = CPCAP_KPB_ON,
	.class_name = "button-backlight",
};

static struct platform_device cpcap_kpb_led = {
	.name = CPCAP_KPB_LED_DRV_NAME,
	.id             = -1,
	.dev            = {
		.platform_data  = &kpb_led_data,
	},
};

static struct cpcap_mdb_led_config_data mdb_led_data = {
	.init = CPCAP_MDB_INIT,
	.abmode_config = &abmode_config_data,
	.class_name = CPCAP_MDB_LED_CLASS_NAME,
};

static struct platform_device cpcap_mdb_led = {
	.name = CPCAP_MDB_LED_DRV_NAME,
	.id             = -1,
	.dev            = {
		.platform_data  = &mdb_led_data,
	},
};

static struct cpcap_rgb_led_config_data rgb_led_data = {
	.red_enable = true,
	.green_enable = true,
	.blue_enable = true,
	.blink_enable = true,
	.class_name_red = "red",
	.class_name_green = "green",
	.class_name_blue = "blue",
};

static struct platform_device cpcap_rgb_led = {
	.name = CPCAP_RGB_LED_DRV_NAME,
	.id             = -1,
	.dev            = {
		.platform_data  = &rgb_led_data,
	},
};


static void cpcap_leds_init(void)
{
	struct device_node *node;
	const void *prop;
	int len = 0;

	pr_info("%s Configuring CPCAP LED drivers\n", __func__);

				//pr_info("CPCAP,ADB found.\n");
				//cpcap_device_register(&cpcap_adb_led);
				
				pr_info("CPCAP,GPIO found.\n");
				cpcap_device_register(&cpcap_gpio_led);
				
				pr_info("CPCAP,KPB found.\n");
				cpcap_device_register(&cpcap_kpb_led);
				
				//pr_info("CPCAP,MDB found.\n");
				//cpcap_device_register(&cpcap_mdb_led);
				
				pr_info("CPCAP,RGB found.\n");
				cpcap_device_register(&cpcap_rgb_led);
				
}

static int __init led_cpcap_lm3554_init(void)
{
	u8 device_available;

	device_available = 1;
	return device_available;
}

static int __init led_cpcap_lm3559_init(void)
{
	u8 device_available;
	device_available = 0;
	return device_available;
}

static int is_cpcap_vio_supply_converter(void)
{
	/* The converter is existing by default */
	return 1;
}

void __init sholes_cpcap_client_init(void)
{
	int i;
 
	//for (i = 0; i < ARRAY_SIZE(cpcap_devices); i++) /*these are still in cpcap-core, so don't add them here*/
	//	cpcap_device_register(cpcap_devices[i]);

		cpcap_leds_init();

	if (led_cpcap_lm3554_init() > 0)
		cpcap_device_register(&cpcap_lm3554);

	if (led_cpcap_lm3559_init() > 0)
		cpcap_device_register(&cpcap_lm3559);

	if (!is_cpcap_vio_supply_converter())
		cpcap_device_register(&cpcap_vio_active_device);

#ifdef CONFIG_PM_DBG_DRV
	//get_pm_dbg_drvdata();	//pm_dbg is broken, and already in board-sholes.c so don't add it here
	//cpcap_device_register(&cpcap_pm_dbg_device);
	//platform_set_drvdata(&cpcap_pm_dbg_device, &cpcap_pm_dbg_drvdata);
#endif
}
