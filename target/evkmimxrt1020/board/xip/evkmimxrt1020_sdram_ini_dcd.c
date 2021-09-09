/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/***********************************************************************************************************************
 * This file was generated by the MCUXpresso Config Tools. Any manual edits made to this file
 * will be overwritten if the respective MCUXpresso Config Tools is used to update this file.
 **********************************************************************************************************************/

#include "evkmimxrt1020_sdram_ini_dcd.h"

/* Component ID definition, used by tools. */
#ifndef FSL_COMPONENT_ID
#define FSL_COMPONENT_ID "platform.drivers.xip_board"
#endif

#if defined(XIP_BOOT_HEADER_ENABLE) && (XIP_BOOT_HEADER_ENABLE == 1)
#if defined(XIP_BOOT_HEADER_DCD_ENABLE) && (XIP_BOOT_HEADER_DCD_ENABLE == 1)
#if defined(__CC_ARM) || defined(__ARMCC_VERSION) || defined(__GNUC__)
__attribute__((section(".boot_hdr.dcd_data")))
#elif defined(__ICCARM__)
#pragma location = ".boot_hdr.dcd_data"
#endif

/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: DCDx V2.0
processor: MIMXRT1021xxxxx
package_id: MIMXRT1021DAG5A
mcu_data: ksdk2_0
processor_version: 0.0.0
output_format: c_array
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/
/* COMMENTS BELOW ARE USED AS SETTINGS FOR DCD DATA */
const uint8_t dcd_data[] = {
	/* HEADER */
	/* Tag */
	0xD2,
	/* Image Length */
	0x03, 0xE8,
	/* Version */
	0x41,

	/* COMMANDS */

	/* group: 'Imported Commands' */
	/* #1.1-8, command header bytes for merged 'Write - value' command */
	0xCC, 0x00, 0x44, 0x04,
	/* #1.1, command: write_value, address: CCM_CCGR0, value: 0xFFFFFFFF, size: 4 */
	0x40, 0x0F, 0xC0, 0x68, 0xFF, 0xFF, 0xFF, 0xFF,
	/* #1.2, command: write_value, address: CCM_CCGR1, value: 0xFFFFFFFF, size: 4 */
	0x40, 0x0F, 0xC0, 0x6C, 0xFF, 0xFF, 0xFF, 0xFF,
	/* #1.3, command: write_value, address: CCM_CCGR2, value: 0xFFFFFFFF, size: 4 */
	0x40, 0x0F, 0xC0, 0x70, 0xFF, 0xFF, 0xFF, 0xFF,
	/* #1.4, command: write_value, address: CCM_CCGR3, value: 0xFFFFFFFF, size: 4 */
	0x40, 0x0F, 0xC0, 0x74, 0xFF, 0xFF, 0xFF, 0xFF,
	/* #1.5, command: write_value, address: CCM_CCGR4, value: 0xFFFFFFFF, size: 4 */
	0x40, 0x0F, 0xC0, 0x78, 0xFF, 0xFF, 0xFF, 0xFF,
	/* #1.6, command: write_value, address: CCM_CCGR5, value: 0xFFFFFFFF, size: 4 */
	0x40, 0x0F, 0xC0, 0x7C, 0xFF, 0xFF, 0xFF, 0xFF,
	/* #1.7, command: write_value, address: CCM_CCGR6, value: 0xFFFFFFFF, size: 4 */
	0x40, 0x0F, 0xC0, 0x80, 0xFF, 0xFF, 0xFF, 0xFF,
	/* #1.8, command: write_value, address: CCM_ANALOG_PLL_SYS, value: 0x2001, size: 4 */
	0x40, 0x0D, 0x80, 0x30, 0x00, 0x00, 0x20, 0x01,
	/* #2, command: write_clear_bits, address: CCM_ANALOG_PFD_528, value: 0x800000, size: 4 */
	0xCC, 0x00, 0x0C, 0x0C, 0x40, 0x0D, 0x81, 0x00, 0x00, 0x80, 0x00, 0x00,
	/* #3.1-98, command header bytes for merged 'Write - value' command */
	0xCC, 0x03, 0x14, 0x04,
	/* #3.1, command: write_value, address: CCM_CBCDR, value: 0xA8340, size: 4 */
	0x40, 0x0F, 0xC0, 0x14, 0x00, 0x0A, 0x83, 0x40,
	/* #3.2, command: write_value, address: IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_00, value: 0x00, size: 4 */
	0x40, 0x1F, 0x80, 0x14, 0x00, 0x00, 0x00, 0x00,
	/* #3.3, command: write_value, address: IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_01, value: 0x00, size: 4 */
	0x40, 0x1F, 0x80, 0x18, 0x00, 0x00, 0x00, 0x00,
	/* #3.4, command: write_value, address: IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_02, value: 0x00, size: 4 */
	0x40, 0x1F, 0x80, 0x1C, 0x00, 0x00, 0x00, 0x00,
	/* #3.5, command: write_value, address: IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_03, value: 0x00, size: 4 */
	0x40, 0x1F, 0x80, 0x20, 0x00, 0x00, 0x00, 0x00,
	/* #3.6, command: write_value, address: IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_04, value: 0x00, size: 4 */
	0x40, 0x1F, 0x80, 0x24, 0x00, 0x00, 0x00, 0x00,
	/* #3.7, command: write_value, address: IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_05, value: 0x00, size: 4 */
	0x40, 0x1F, 0x80, 0x28, 0x00, 0x00, 0x00, 0x00,
	/* #3.8, command: write_value, address: IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_06, value: 0x00, size: 4 */
	0x40, 0x1F, 0x80, 0x2C, 0x00, 0x00, 0x00, 0x00,
	/* #3.9, command: write_value, address: IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_07, value: 0x00, size: 4 */
	0x40, 0x1F, 0x80, 0x30, 0x00, 0x00, 0x00, 0x00,
	/* #3.10, command: write_value, address: IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_08, value: 0x00, size: 4 */
	0x40, 0x1F, 0x80, 0x34, 0x00, 0x00, 0x00, 0x00,
	/* #3.11, command: write_value, address: IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_09, value: 0x00, size: 4 */
	0x40, 0x1F, 0x80, 0x38, 0x00, 0x00, 0x00, 0x00,
	/* #3.12, command: write_value, address: IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_10, value: 0x00, size: 4 */
	0x40, 0x1F, 0x80, 0x3C, 0x00, 0x00, 0x00, 0x00,
	/* #3.13, command: write_value, address: IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_11, value: 0x00, size: 4 */
	0x40, 0x1F, 0x80, 0x40, 0x00, 0x00, 0x00, 0x00,
	/* #3.14, command: write_value, address: IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_12, value: 0x00, size: 4 */
	0x40, 0x1F, 0x80, 0x44, 0x00, 0x00, 0x00, 0x00,
	/* #3.15, command: write_value, address: IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_13, value: 0x00, size: 4 */
	0x40, 0x1F, 0x80, 0x48, 0x00, 0x00, 0x00, 0x00,
	/* #3.16, command: write_value, address: IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_14, value: 0x00, size: 4 */
	0x40, 0x1F, 0x80, 0x4C, 0x00, 0x00, 0x00, 0x00,
	/* #3.17, command: write_value, address: IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_15, value: 0x00, size: 4 */
	0x40, 0x1F, 0x80, 0x50, 0x00, 0x00, 0x00, 0x00,
	/* #3.18, command: write_value, address: IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_16, value: 0x00, size: 4 */
	0x40, 0x1F, 0x80, 0x54, 0x00, 0x00, 0x00, 0x00,
	/* #3.19, command: write_value, address: IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_17, value: 0x00, size: 4 */
	0x40, 0x1F, 0x80, 0x58, 0x00, 0x00, 0x00, 0x00,
	/* #3.20, command: write_value, address: IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_18, value: 0x00, size: 4 */
	0x40, 0x1F, 0x80, 0x5C, 0x00, 0x00, 0x00, 0x00,
	/* #3.21, command: write_value, address: IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_19, value: 0x00, size: 4 */
	0x40, 0x1F, 0x80, 0x60, 0x00, 0x00, 0x00, 0x00,
	/* #3.22, command: write_value, address: IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_20, value: 0x00, size: 4 */
	0x40, 0x1F, 0x80, 0x64, 0x00, 0x00, 0x00, 0x00,
	/* #3.23, command: write_value, address: IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_21, value: 0x00, size: 4 */
	0x40, 0x1F, 0x80, 0x68, 0x00, 0x00, 0x00, 0x00,
	/* #3.24, command: write_value, address: IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_22, value: 0x00, size: 4 */
	0x40, 0x1F, 0x80, 0x6C, 0x00, 0x00, 0x00, 0x00,
	/* #3.25, command: write_value, address: IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_23, value: 0x00, size: 4 */
	0x40, 0x1F, 0x80, 0x70, 0x00, 0x00, 0x00, 0x00,
	/* #3.26, command: write_value, address: IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_24, value: 0x00, size: 4 */
	0x40, 0x1F, 0x80, 0x74, 0x00, 0x00, 0x00, 0x00,
	/* #3.27, command: write_value, address: IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_25, value: 0x00, size: 4 */
	0x40, 0x1F, 0x80, 0x78, 0x00, 0x00, 0x00, 0x00,
	/* #3.28, command: write_value, address: IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_26, value: 0x00, size: 4 */
	0x40, 0x1F, 0x80, 0x7C, 0x00, 0x00, 0x00, 0x00,
	/* #3.29, command: write_value, address: IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_27, value: 0x00, size: 4 */
	0x40, 0x1F, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00,
	/* #3.30, command: write_value, address: IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_28, value: 0x10, size: 4 */
	0x40, 0x1F, 0x80, 0x84, 0x00, 0x00, 0x00, 0x10,
	/* #3.31, command: write_value, address: IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_29, value: 0x00, size: 4 */
	0x40, 0x1F, 0x80, 0x88, 0x00, 0x00, 0x00, 0x00,
	/* #3.32, command: write_value, address: IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_30, value: 0x00, size: 4 */
	0x40, 0x1F, 0x80, 0x8C, 0x00, 0x00, 0x00, 0x00,
	/* #3.33, command: write_value, address: IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_31, value: 0x00, size: 4 */
	0x40, 0x1F, 0x80, 0x90, 0x00, 0x00, 0x00, 0x00,
	/* #3.34, command: write_value, address: IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_32, value: 0x00, size: 4 */
	0x40, 0x1F, 0x80, 0x94, 0x00, 0x00, 0x00, 0x00,
	/* #3.35, command: write_value, address: IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_33, value: 0x00, size: 4 */
	0x40, 0x1F, 0x80, 0x98, 0x00, 0x00, 0x00, 0x00,
	/* #3.36, command: write_value, address: IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_34, value: 0x00, size: 4 */
	0x40, 0x1F, 0x80, 0x9C, 0x00, 0x00, 0x00, 0x00,
	/* #3.37, command: write_value, address: IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_35, value: 0x00, size: 4 */
	0x40, 0x1F, 0x80, 0xA0, 0x00, 0x00, 0x00, 0x00,
	/* #3.38, command: write_value, address: IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_36, value: 0x00, size: 4 */
	0x40, 0x1F, 0x80, 0xA4, 0x00, 0x00, 0x00, 0x00,
	/* #3.39, command: write_value, address: IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_37, value: 0x00, size: 4 */
	0x40, 0x1F, 0x80, 0xA8, 0x00, 0x00, 0x00, 0x00,
	/* #3.40, command: write_value, address: IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_38, value: 0x00, size: 4 */
	0x40, 0x1F, 0x80, 0xAC, 0x00, 0x00, 0x00, 0x00,
	/* #3.41, command: write_value, address: IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_39, value: 0x00, size: 4 */
	0x40, 0x1F, 0x80, 0xB0, 0x00, 0x00, 0x00, 0x00,
	/* #3.42, command: write_value, address: IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_31, value: 0xE1, size: 4 */
	0x40, 0x1F, 0x82, 0x04, 0x00, 0x00, 0x00, 0xE1,
	/* #3.43, command: write_value, address: IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_32, value: 0xE1, size: 4 */
	0x40, 0x1F, 0x82, 0x08, 0x00, 0x00, 0x00, 0xE1,
	/* #3.44, command: write_value, address: IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_33, value: 0xE1, size: 4 */
	0x40, 0x1F, 0x82, 0x0C, 0x00, 0x00, 0x00, 0xE1,
	/* #3.45, command: write_value, address: IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_34, value: 0xE1, size: 4 */
	0x40, 0x1F, 0x82, 0x10, 0x00, 0x00, 0x00, 0xE1,
	/* #3.46, command: write_value, address: IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_35, value: 0xE1, size: 4 */
	0x40, 0x1F, 0x82, 0x14, 0x00, 0x00, 0x00, 0xE1,
	/* #3.47, command: write_value, address: IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_36, value: 0xE1, size: 4 */
	0x40, 0x1F, 0x82, 0x18, 0x00, 0x00, 0x00, 0xE1,
	/* #3.48, command: write_value, address: IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_37, value: 0xE1, size: 4 */
	0x40, 0x1F, 0x82, 0x1C, 0x00, 0x00, 0x00, 0xE1,
	/* #3.49, command: write_value, address: IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_38, value: 0xE1, size: 4 */
	0x40, 0x1F, 0x82, 0x20, 0x00, 0x00, 0x00, 0xE1,
	/* #3.50, command: write_value, address: IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_39, value: 0xE1, size: 4 */
	0x40, 0x1F, 0x82, 0x24, 0x00, 0x00, 0x00, 0xE1,
	/* #3.51, command: write_value, address: IOMUXC_SW_PAD_CTL_PAD_GPIO_AD_B0_00, value: 0xE1, size: 4 */
	0x40, 0x1F, 0x82, 0x30, 0x00, 0x00, 0x00, 0xE1,
	/* #3.52, command: write_value, address: IOMUXC_SW_PAD_CTL_PAD_GPIO_AD_B0_01, value: 0xE1, size: 4 */
	0x40, 0x1F, 0x82, 0x34, 0x00, 0x00, 0x00, 0xE1,
	/* #3.53, command: write_value, address: IOMUXC_SW_PAD_CTL_PAD_GPIO_AD_B0_02, value: 0xE1, size: 4 */
	0x40, 0x1F, 0x82, 0x38, 0x00, 0x00, 0x00, 0xE1,
	/* #3.54, command: write_value, address: IOMUXC_SW_PAD_CTL_PAD_GPIO_AD_B0_03, value: 0xE1, size: 4 */
	0x40, 0x1F, 0x82, 0x3C, 0x00, 0x00, 0x00, 0xE1,
	/* #3.55, command: write_value, address: IOMUXC_SW_PAD_CTL_PAD_GPIO_AD_B0_04, value: 0xE1, size: 4 */
	0x40, 0x1F, 0x82, 0x40, 0x00, 0x00, 0x00, 0xE1,
	/* #3.56, command: write_value, address: IOMUXC_SW_PAD_CTL_PAD_GPIO_AD_B0_05, value: 0xE1, size: 4 */
	0x40, 0x1F, 0x82, 0x44, 0x00, 0x00, 0x00, 0xE1,
	/* #3.57, command: write_value, address: IOMUXC_SW_PAD_CTL_PAD_GPIO_AD_B0_06, value: 0xE1, size: 4 */
	0x40, 0x1F, 0x82, 0x48, 0x00, 0x00, 0x00, 0xE1,
	/* #3.58, command: write_value, address: IOMUXC_SW_PAD_CTL_PAD_GPIO_AD_B0_07, value: 0xE1, size: 4 */
	0x40, 0x1F, 0x82, 0x4C, 0x00, 0x00, 0x00, 0xE1,
	/* #3.59, command: write_value, address: IOMUXC_SW_PAD_CTL_PAD_GPIO_AD_B0_08, value: 0xE1, size: 4 */
	0x40, 0x1F, 0x82, 0x50, 0x00, 0x00, 0x00, 0xE1,
	/* #3.60, command: write_value, address: IOMUXC_SW_PAD_CTL_PAD_GPIO_AD_B0_09, value: 0xE1, size: 4 */
	0x40, 0x1F, 0x82, 0x54, 0x00, 0x00, 0x00, 0xE1,
	/* #3.61, command: write_value, address: IOMUXC_SW_PAD_CTL_PAD_GPIO_AD_B0_10, value: 0xE1, size: 4 */
	0x40, 0x1F, 0x82, 0x58, 0x00, 0x00, 0x00, 0xE1,
	/* #3.62, command: write_value, address: IOMUXC_SW_PAD_CTL_PAD_GPIO_AD_B0_11, value: 0xE1, size: 4 */
	0x40, 0x1F, 0x82, 0x5C, 0x00, 0x00, 0x00, 0xE1,
	/* #3.63, command: write_value, address: IOMUXC_SW_PAD_CTL_PAD_GPIO_AD_B0_12, value: 0xE1, size: 4 */
	0x40, 0x1F, 0x82, 0x60, 0x00, 0x00, 0x00, 0xE1,
	/* #3.64, command: write_value, address: IOMUXC_SW_PAD_CTL_PAD_GPIO_AD_B0_13, value: 0xE1, size: 4 */
	0x40, 0x1F, 0x82, 0x64, 0x00, 0x00, 0x00, 0xE1,
	/* #3.65, command: write_value, address: IOMUXC_SW_PAD_CTL_PAD_GPIO_AD_B0_14, value: 0xE1, size: 4 */
	0x40, 0x1F, 0x82, 0x68, 0x00, 0x00, 0x00, 0xE1,
	/* #3.66, command: write_value, address: IOMUXC_SW_PAD_CTL_PAD_GPIO_AD_B0_15, value: 0xE1, size: 4 */
	0x40, 0x1F, 0x82, 0x6C, 0x00, 0x00, 0x00, 0xE1,
	/* #3.67, command: write_value, address: IOMUXC_SW_PAD_CTL_PAD_GPIO_AD_B1_00, value: 0xE1, size: 4 */
	0x40, 0x1F, 0x82, 0x70, 0x00, 0x00, 0x00, 0xE1,
	/* #3.68, command: write_value, address: IOMUXC_SW_PAD_CTL_PAD_GPIO_AD_B1_01, value: 0xE1, size: 4 */
	0x40, 0x1F, 0x82, 0x74, 0x00, 0x00, 0x00, 0xE1,
	/* #3.69, command: write_value, address: IOMUXC_SW_PAD_CTL_PAD_GPIO_AD_B1_02, value: 0xE1, size: 4 */
	0x40, 0x1F, 0x82, 0x78, 0x00, 0x00, 0x00, 0xE1,
	/* #3.70, command: write_value, address: IOMUXC_SW_PAD_CTL_PAD_GPIO_AD_B1_03, value: 0xE1, size: 4 */
	0x40, 0x1F, 0x82, 0x7C, 0x00, 0x00, 0x00, 0xE1,
	/* #3.71, command: write_value, address: IOMUXC_SW_PAD_CTL_PAD_GPIO_AD_B1_04, value: 0xE1, size: 4 */
	0x40, 0x1F, 0x82, 0x80, 0x00, 0x00, 0x00, 0xE1,
	/* #3.72, command: write_value, address: IOMUXC_SW_PAD_CTL_PAD_GPIO_AD_B1_05, value: 0xE1, size: 4 */
	0x40, 0x1F, 0x82, 0x84, 0x00, 0x00, 0x00, 0xE1,
	/* #3.73, command: write_value, address: IOMUXC_SW_PAD_CTL_PAD_GPIO_AD_B1_06, value: 0xE1, size: 4 */
	0x40, 0x1F, 0x82, 0x88, 0x00, 0x00, 0x00, 0xE1,
	/* #3.74, command: write_value, address: IOMUXC_SW_PAD_CTL_PAD_GPIO_AD_B1_07, value: 0xE1, size: 4 */
	0x40, 0x1F, 0x82, 0x8C, 0x00, 0x00, 0x00, 0xE1,
	/* #3.75, command: write_value, address: IOMUXC_SW_PAD_CTL_PAD_GPIO_AD_B1_08, value: 0xE1, size: 4 */
	0x40, 0x1F, 0x82, 0x90, 0x00, 0x00, 0x00, 0xE1,
	/* #3.76, command: write_value, address: IOMUXC_SW_PAD_CTL_PAD_GPIO_AD_B1_09, value: 0xE1, size: 4 */
	0x40, 0x1F, 0x82, 0x94, 0x00, 0x00, 0x00, 0xE1,
	/* #3.77, command: write_value, address: IOMUXC_SW_PAD_CTL_PAD_GPIO_AD_B1_10, value: 0xE1, size: 4 */
	0x40, 0x1F, 0x82, 0x98, 0x00, 0x00, 0x00, 0xE1,
	/* #3.78, command: write_value, address: IOMUXC_SW_PAD_CTL_PAD_GPIO_AD_B1_11, value: 0xE1, size: 4 */
	0x40, 0x1F, 0x82, 0x9C, 0x00, 0x00, 0x00, 0xE1,
	/* #3.79, command: write_value, address: IOMUXC_SW_PAD_CTL_PAD_GPIO_AD_B1_12, value: 0xE1, size: 4 */
	0x40, 0x1F, 0x82, 0xA0, 0x00, 0x00, 0x00, 0xE1,
	/* #3.80, command: write_value, address: IOMUXC_SW_PAD_CTL_PAD_GPIO_AD_B1_13, value: 0xE1, size: 4 */
	0x40, 0x1F, 0x82, 0xA4, 0x00, 0x00, 0x00, 0xE1,
	/* #3.81, command: write_value, address: IOMUXC_SW_PAD_CTL_PAD_GPIO_AD_B1_14, value: 0xE1, size: 4 */
	0x40, 0x1F, 0x82, 0xA8, 0x00, 0x00, 0x00, 0xE1,
	/* #3.82, command: write_value, address: SEMC_MCR, value: 0x10000004, size: 4 */
	0x40, 0x2F, 0x00, 0x00, 0x10, 0x00, 0x00, 0x04,
	/* #3.83, command: write_value, address: SEMC_BMCR0, value: 0x30524, size: 4 */
	0x40, 0x2F, 0x00, 0x08, 0x00, 0x03, 0x05, 0x24,
	/* #3.84, command: write_value, address: SEMC_BMCR1, value: 0x6030524, size: 4 */
	0x40, 0x2F, 0x00, 0x0C, 0x06, 0x03, 0x05, 0x24,
	/* #3.85, command: write_value, address: SEMC_BR0, value: 0x8000001B, size: 4 */
	0x40, 0x2F, 0x00, 0x10, 0x80, 0x00, 0x00, 0x1B,
	/* #3.86, command: write_value, address: SEMC_BR1, value: 0x8200001B, size: 4 */
	0x40, 0x2F, 0x00, 0x14, 0x82, 0x00, 0x00, 0x1B,
	/* #3.87, command: write_value, address: SEMC_BR2, value: 0x8400001B, size: 4 */
	0x40, 0x2F, 0x00, 0x18, 0x84, 0x00, 0x00, 0x1B,
	/* #3.88, command: write_value, address: SEMC_IOCR, value: 0x7988, size: 4 */
	0x40, 0x2F, 0x00, 0x04, 0x00, 0x00, 0x79, 0x88,
	/* #3.89, command: write_value, address: SEMC_SDRAMCR0, value: 0xF37, size: 4 */
	0x40, 0x2F, 0x00, 0x40, 0x00, 0x00, 0x0F, 0x37,
	/* #3.90, command: write_value, address: SEMC_SDRAMCR1, value: 0x652922, size: 4 */
	0x40, 0x2F, 0x00, 0x44, 0x00, 0x65, 0x29, 0x22,
	/* #3.91, command: write_value, address: SEMC_SDRAMCR2, value: 0x10920, size: 4 */
	0x40, 0x2F, 0x00, 0x48, 0x00, 0x01, 0x09, 0x20,
	/* #3.92, command: write_value, address: SEMC_SDRAMCR3, value: 0x50210A08, size: 4 */
	0x40, 0x2F, 0x00, 0x4C, 0x50, 0x21, 0x0A, 0x08,
	/* #3.93, command: write_value, address: SEMC_DBICR0, value: 0x21, size: 4 */
	0x40, 0x2F, 0x00, 0x80, 0x00, 0x00, 0x00, 0x21,
	/* #3.94, command: write_value, address: SEMC_DBICR1, value: 0x888888, size: 4 */
	0x40, 0x2F, 0x00, 0x84, 0x00, 0x88, 0x88, 0x88,
	/* #3.95, command: write_value, address: SEMC_IPCR1, value: 0x02, size: 4 */
	0x40, 0x2F, 0x00, 0x94, 0x00, 0x00, 0x00, 0x02,
	/* #3.96, command: write_value, address: SEMC_IPCR2, value: 0x00, size: 4 */
	0x40, 0x2F, 0x00, 0x98, 0x00, 0x00, 0x00, 0x00,
	/* #3.97, command: write_value, address: SEMC_IPCR0, value: 0x80000000, size: 4 */
	0x40, 0x2F, 0x00, 0x90, 0x80, 0x00, 0x00, 0x00,
	/* #3.98, command: write_value, address: SEMC_IPCMD, value: 0xA55A000F, size: 4 */
	0x40, 0x2F, 0x00, 0x9C, 0xA5, 0x5A, 0x00, 0x0F,
	/* #4, command: check_any_bit_set, address: SEMC_INTR, value: 0x01, size: 4 */
	0xCF, 0x00, 0x0C, 0x1C, 0x40, 0x2F, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x01,
	/* #5.1-2, command header bytes for merged 'Write - value' command */
	0xCC, 0x00, 0x14, 0x04,
	/* #5.1, command: write_value, address: SEMC_IPCR0, value: 0x80000000, size: 4 */
	0x40, 0x2F, 0x00, 0x90, 0x80, 0x00, 0x00, 0x00,
	/* #5.2, command: write_value, address: SEMC_IPCMD, value: 0xA55A000C, size: 4 */
	0x40, 0x2F, 0x00, 0x9C, 0xA5, 0x5A, 0x00, 0x0C,
	/* #6, command: check_any_bit_set, address: SEMC_INTR, value: 0x01, size: 4 */
	0xCF, 0x00, 0x0C, 0x1C, 0x40, 0x2F, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x01,
	/* #7.1-2, command header bytes for merged 'Write - value' command */
	0xCC, 0x00, 0x14, 0x04,
	/* #7.1, command: write_value, address: SEMC_IPCR0, value: 0x80000000, size: 4 */
	0x40, 0x2F, 0x00, 0x90, 0x80, 0x00, 0x00, 0x00,
	/* #7.2, command: write_value, address: SEMC_IPCMD, value: 0xA55A000C, size: 4 */
	0x40, 0x2F, 0x00, 0x9C, 0xA5, 0x5A, 0x00, 0x0C,
	/* #8, command: check_any_bit_set, address: SEMC_INTR, value: 0x01, size: 4 */
	0xCF, 0x00, 0x0C, 0x1C, 0x40, 0x2F, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x01,
	/* #9.1-3, command header bytes for merged 'Write - value' command */
	0xCC, 0x00, 0x1C, 0x04,
	/* #9.1, command: write_value, address: SEMC_IPTXDAT, value: 0x33, size: 4 */
	0x40, 0x2F, 0x00, 0xA0, 0x00, 0x00, 0x00, 0x33,
	/* #9.2, command: write_value, address: SEMC_IPCR0, value: 0x80000000, size: 4 */
	0x40, 0x2F, 0x00, 0x90, 0x80, 0x00, 0x00, 0x00,
	/* #9.3, command: write_value, address: SEMC_IPCMD, value: 0xA55A000A, size: 4 */
	0x40, 0x2F, 0x00, 0x9C, 0xA5, 0x5A, 0x00, 0x0A,
	/* #10, command: check_any_bit_set, address: SEMC_INTR, value: 0x01, size: 4 */
	0xCF, 0x00, 0x0C, 0x1C, 0x40, 0x2F, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x01,
	/* #11, command: write_value, address: SEMC_SDRAMCR3, value: 0x50210A09, size: 4 */
	0xCC, 0x00, 0x0C, 0x04, 0x40, 0x2F, 0x00, 0x4C, 0x50, 0x21, 0x0A, 0x09
	};
/* BE CAREFUL MODIFYING THIS SETTINGS - IT IS YAML SETTINGS FOR TOOLS */

#else
const uint8_t dcd_data[] = {0x00};
#endif /* XIP_BOOT_HEADER_DCD_ENABLE */
#endif /* XIP_BOOT_HEADER_ENABLE */
