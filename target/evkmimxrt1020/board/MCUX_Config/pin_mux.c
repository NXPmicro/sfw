/*
 * Copyright 2019-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/***********************************************************************************************************************
 * This file was generated by the MCUXpresso Config Tools. Any manual edits made to this file
 * will be overwritten if the respective MCUXpresso Config Tools is used to update this file.
 **********************************************************************************************************************/

/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Pins v7.0
processor: MIMXRT1021xxxxx
package_id: MIMXRT1021DAG5A
mcu_data: ksdk2_0
processor_version: 0.7.10
board: MIMXRT1020-EVK
power_domains: {NVCC_GPIO: '3.3'}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "pin_mux.h"
#include <sfw.h>

/* FUNCTION ************************************************************************************************************
 * 
 * Function Name : BOARD_InitBootPins
 * Description   : Calls initialization functions.
 * 
 * END ****************************************************************************************************************/
void BOARD_InitBootPins(void) {
    BOARD_InitPins();
}

/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitPins:
- options: {callFromInitBoot: 'true', prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '101', peripheral: LPUART1, signal: RX, pin_signal: GPIO_AD_B0_07, software_input_on: Disable, open_drain: Disable}
  - {pin_num: '105', peripheral: LPUART1, signal: TX, pin_signal: GPIO_AD_B0_06, software_input_on: Disable, open_drain: Disable}
  - {pin_num: '97', peripheral: ARM, signal: arm_trace_swo, pin_signal: GPIO_AD_B0_11, slew_rate: Fast}
  - {pin_num: '19', peripheral: FLEXSPI, signal: FLEXSPI_A_SS0_B, pin_signal: GPIO_SD_B1_11, slew_rate: Fast, software_input_on: Enable, open_drain: Disable, speed: MHZ_200}
  - {pin_num: '24', peripheral: FLEXSPI, signal: FLEXSPI_A_SCLK, pin_signal: GPIO_SD_B1_07, slew_rate: Fast, software_input_on: Enable, speed: MHZ_200}
  - {pin_num: '23', peripheral: FLEXSPI, signal: FLEXSPI_A_DATA0, pin_signal: GPIO_SD_B1_08, slew_rate: Fast, software_input_on: Enable, speed: MHZ_200}
  - {pin_num: '21', peripheral: FLEXSPI, signal: FLEXSPI_A_DATA1, pin_signal: GPIO_SD_B1_10, slew_rate: Fast, software_input_on: Enable, speed: MHZ_200}
  - {pin_num: '22', peripheral: FLEXSPI, signal: FLEXSPI_A_DATA2, pin_signal: GPIO_SD_B1_09, slew_rate: Fast, software_input_on: Enable, speed: MHZ_200}
  - {pin_num: '25', peripheral: FLEXSPI, signal: FLEXSPI_A_DATA3, pin_signal: GPIO_SD_B1_06, slew_rate: Fast, software_input_on: Enable, speed: MHZ_200}
  - {pin_num: '26', peripheral: FLEXSPI, signal: FLEXSPI_A_DQS, pin_signal: GPIO_SD_B1_05, slew_rate: Fast, software_input_on: Enable, speed: MHZ_200}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitPins(void) {
  CLOCK_EnableClock(kCLOCK_Iomuxc);           /* iomuxc clock (iomuxc_clk_enable): 0x03U */

  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_B0_06_LPUART1_TX,        /* GPIO_AD_B0_06 is configured as LPUART1_TX */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_B0_07_LPUART1_RX,        /* GPIO_AD_B0_07 is configured as LPUART1_RX */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_B0_11_ARM_CM7_TRACE_SWO,  /* GPIO_AD_B0_11 is configured as ARM_CM7_TRACE_SWO */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_SD_B1_05_FLEXSPI_A_DQS,     /* GPIO_SD_B1_05 is configured as FLEXSPI_A_DQS */
      1U);                                    /* Software Input On Field: Force input path of pad GPIO_SD_B1_05 */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_SD_B1_06_FLEXSPI_A_DATA03,  /* GPIO_SD_B1_06 is configured as FLEXSPI_A_DATA03 */
      1U);                                    /* Software Input On Field: Force input path of pad GPIO_SD_B1_06 */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_SD_B1_07_FLEXSPI_A_SCLK,    /* GPIO_SD_B1_07 is configured as FLEXSPI_A_SCLK */
      1U);                                    /* Software Input On Field: Force input path of pad GPIO_SD_B1_07 */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_SD_B1_08_FLEXSPI_A_DATA00,  /* GPIO_SD_B1_08 is configured as FLEXSPI_A_DATA00 */
      1U);                                    /* Software Input On Field: Force input path of pad GPIO_SD_B1_08 */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_SD_B1_09_FLEXSPI_A_DATA02,  /* GPIO_SD_B1_09 is configured as FLEXSPI_A_DATA02 */
      1U);                                    /* Software Input On Field: Force input path of pad GPIO_SD_B1_09 */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_SD_B1_10_FLEXSPI_A_DATA01,  /* GPIO_SD_B1_10 is configured as FLEXSPI_A_DATA01 */
      1U);                                    /* Software Input On Field: Force input path of pad GPIO_SD_B1_10 */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_SD_B1_11_FLEXSPI_A_SS0_B,   /* GPIO_SD_B1_11 is configured as FLEXSPI_A_SS0_B */
      1U);                                    /* Software Input On Field: Force input path of pad GPIO_SD_B1_11 */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_AD_B0_06_LPUART1_TX,        /* GPIO_AD_B0_06 PAD functional properties : */
      0x10B0U);                               /* Slew Rate Field: Slow Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: medium(100MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Keeper
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_AD_B0_07_LPUART1_RX,        /* GPIO_AD_B0_07 PAD functional properties : */
      0x10B0U);                               /* Slew Rate Field: Slow Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: medium(100MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Keeper
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_AD_B0_11_ARM_CM7_TRACE_SWO,  /* GPIO_AD_B0_11 PAD functional properties : */
      0x10B1U);                               /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: medium(100MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Keeper
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_SD_B1_05_FLEXSPI_A_DQS,     /* GPIO_SD_B1_05 PAD functional properties : */
      0x10F1u);                               /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: max(200MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Keeper
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_SD_B1_06_FLEXSPI_A_DATA03,  /* GPIO_SD_B1_06 PAD functional properties : */
      0x10F1u);                               /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: max(200MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Keeper
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_SD_B1_07_FLEXSPI_A_SCLK,    /* GPIO_SD_B1_07 PAD functional properties : */
      0x10F1u);                               /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: max(200MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Keeper
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_SD_B1_08_FLEXSPI_A_DATA00,  /* GPIO_SD_B1_08 PAD functional properties : */
      0x10F1u);                               /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: max(200MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Keeper
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_SD_B1_09_FLEXSPI_A_DATA02,  /* GPIO_SD_B1_09 PAD functional properties : */
      0x10F1u);                               /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: max(200MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Keeper
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_SD_B1_10_FLEXSPI_A_DATA01,  /* GPIO_SD_B1_10 PAD functional properties : */
      0x10F1u);                               /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: max(200MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Keeper
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_SD_B1_11_FLEXSPI_A_SS0_B,   /* GPIO_SD_B1_11 PAD functional properties : */
      0x10F1u);                               /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: max(200MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Keeper
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                 Hyst. Enable Field: Hysteresis Disabled */

#ifdef COMPONENT_PHY
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_B0_04_GPIO1_IO04,        /* GPIO_AD_B0_04 is configured as GPIO1_IO04 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_B0_08_ENET_REF_CLK1,     /* GPIO_AD_B0_08 is configured as ENET_REF_CLK1 */
      1U);                                    /* Software Input On Field: Force input path of pad GPIO_AD_B0_08 */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_B0_09_ENET_RDATA01,      /* GPIO_AD_B0_09 is configured as ENET_RDATA01 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_B0_10_ENET_RDATA00,      /* GPIO_AD_B0_10 is configured as ENET_RDATA00 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_B0_11_ENET_RX_EN,        /* GPIO_AD_B0_11 is configured as ENET_RX_EN */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_B0_12_ENET_RX_ER,        /* GPIO_AD_B0_12 is configured as ENET_RX_ER */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_B0_13_ENET_TX_EN,        /* GPIO_AD_B0_13 is configured as ENET_TX_EN */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_B0_14_ENET_TDATA00,      /* GPIO_AD_B0_14 is configured as ENET_TDATA00 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_B0_15_ENET_TDATA01,      /* GPIO_AD_B0_15 is configured as ENET_TDATA01 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_B1_06_GPIO1_IO22,        /* GPIO_AD_B1_06 is configured as GPIO1_IO22 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_EMC_40_ENET_MDIO,           /* GPIO_EMC_40 is configured as ENET_MDIO */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_EMC_41_ENET_MDC,            /* GPIO_EMC_41 is configured as ENET_MDC */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_AD_B0_04_GPIO1_IO04,        /* GPIO_AD_B0_04 PAD functional properties : */
      0xB0A9U);                               /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/5
                                                 Speed Field: medium(100MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_AD_B0_08_ENET_REF_CLK1,     /* GPIO_AD_B0_08 PAD functional properties : */
      0xB0E9U);                               /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/5
                                                 Speed Field: max(200MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_AD_B0_09_ENET_RDATA01,      /* GPIO_AD_B0_09 PAD functional properties : */
      0xB0E9U);                               /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/5
                                                 Speed Field: max(200MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_AD_B0_10_ENET_RDATA00,      /* GPIO_AD_B0_10 PAD functional properties : */
      0xB0E9U);                               /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/5
                                                 Speed Field: max(200MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_AD_B0_11_ENET_RX_EN,        /* GPIO_AD_B0_11 PAD functional properties : */
      0xB0E9U);                               /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/5
                                                 Speed Field: max(200MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_AD_B0_12_ENET_RX_ER,        /* GPIO_AD_B0_12 PAD functional properties : */
      0xB0E9U);                               /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/5
                                                 Speed Field: max(200MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_AD_B0_13_ENET_TX_EN,        /* GPIO_AD_B0_13 PAD functional properties : */
      0xB0E9U);                               /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/5
                                                 Speed Field: max(200MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_AD_B0_14_ENET_TDATA00,      /* GPIO_AD_B0_14 PAD functional properties : */
      0xB0E9U);                               /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/5
                                                 Speed Field: max(200MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_AD_B0_15_ENET_TDATA01,      /* GPIO_AD_B0_15 PAD functional properties : */
      0xB0E9U);                               /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/5
                                                 Speed Field: max(200MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_AD_B1_06_GPIO1_IO22,        /* GPIO_AD_B1_06 PAD functional properties : */
      0xB0A9U);                               /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/5
                                                 Speed Field: medium(100MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_EMC_40_ENET_MDIO,           /* GPIO_EMC_40 PAD functional properties : */
      0xB0E9U);                               /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/5
                                                 Speed Field: max(200MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_EMC_41_ENET_MDC,            /* GPIO_EMC_41 PAD functional properties : */
      0xB0E9U);                               /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/5
                                                 Speed Field: max(200MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Disabled */
#endif
}

/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
