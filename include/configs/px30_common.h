/*
 * (C) Copyright 2017 Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#ifndef __CONFIG_PX30_COMMON_H
#define __CONFIG_PX30_COMMON_H

#include "rockchip-common.h"

#define CONFIG_SYS_MALLOC_LEN		(64 << 20) /* 64M */
#define CONFIG_SYS_CBSIZE		1024
#define CONFIG_SKIP_LOWLEVEL_INIT

#define CONFIG_SPL_FRAMEWORK

#define CONFIG_SYS_NS16550_MEM32

#define CONFIG_SYS_TEXT_BASE		0x00200000
#define CONFIG_SYS_INIT_SP_ADDR		0x00300000
#define CONFIG_SYS_LOAD_ADDR		0x00800800
#define CONFIG_SPL_STACK		0x00400000
#define CONFIG_SPL_TEXT_BASE		0x00000000
#define CONFIG_SPL_MAX_SIZE		0x10000
#define CONFIG_SPL_BSS_START_ADDR	0x2000000
#define CONFIG_SPL_BSS_MAX_SIZE		0x2000
#define CONFIG_SYS_BOOTM_LEN		(64 << 20)	/* 64M */

#define COUNTER_FREQUENCY		24000000

#define GICD_BASE			0xff131000
#define GICC_BASE			0xff132000

#define CONFIG_SYS_BOOTM_LEN	(64 << 20)	/* 64M */

/* MMC/SD IP block */
#define CONFIG_BOUNCE_BUFFER

#define CONFIG_SYS_SDRAM_BASE		0
#define SDRAM_MAX_SIZE			0xff000000
#define SDRAM_BANK_SIZE			(2UL << 30)
#define CONFIG_PREBOOT

#define ROCKCHIP_SUPPORT_EINK
#define HTFYUN_EINK
#ifdef HTFYUN_EINK
#define PRIVATE_BASE        0x8b000	/* the start-addr of privdata area at parameter */
/* base block addr of eink waveform file on storage */
/* #define EINK_WAVEFORM_BASE	(PRIVATE_BASE+0x800)*/ /* 1M->1320 for waveform */
/* eink waveform file size (Bytes) */
/*#define EINK_WAVEFORM_SIZE	(328*1024)*/
/* #define EINK_LOGO_SIZE	2048000 */
#define PRIVATE_BASE_X      0x83000
#else
/* base block addr of eink waveform file on storage */
#define EINK_WAVEFORM_BASE	0x8000
/* eink waveform file size (Bytes) */
#define EINK_WAVEFORM_SIZE	512000
#endif

#ifndef CONFIG_SPL_BUILD

/* usb mass storage */
#define CONFIG_USB_FUNCTION_MASS_STORAGE
#define CONFIG_ROCKUSB_G_DNL_PID        0x330d

#define ENV_MEM_LAYOUT_SETTINGS \
        "scriptaddr=0x00500000\0" \
        "pxefile_addr_r=0x00600000\0" \
        "fdt_addr_r=0x01f00000\0" \
        "kernel_addr_r=0x02080000\0" \
        "waveform_add_r=0x08300000\0" \
        "logo_add_r=0x10000000\0" \
        "ramdisk_addr_r=0x0a200000\0"

#include <config_distro_bootcmd.h>
#define CONFIG_EXTRA_ENV_SETTINGS \
	ENV_MEM_LAYOUT_SETTINGS \
	"partitions=" PARTS_DEFAULT \
	ROCKCHIP_DEVICE_SETTINGS \
	RKIMG_DET_BOOTDEV \
	BOOTENV

#endif

/* rockchip ohci host driver */
#define CONFIG_USB_OHCI_NEW
#define CONFIG_SYS_USB_OHCI_MAX_ROOT_PORTS	1

#endif
