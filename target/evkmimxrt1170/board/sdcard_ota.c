/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <string.h>
#include "fsl_sd.h"
#include "fsl_debug_console.h"
#include "ff.h"
#include "diskio.h"
#include "fsl_sd_disk.h"
#include "board.h"
#include "sdmmc_config.h"

#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_common.h"
#include "fsl_iomuxc.h"

/* MCUboot */
#include "flexspi_flash.h"
#include "sysflash.h"
#include "image.h"
#include "bootutil_priv.h"

#include "flexspi_flash.h"

#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "timers.h"
#include "sbl_ota_flag.h"
#if defined(CONFIG_BOOT_ENCRYPTED_XIP)
#include "update_key_context.h"
#endif
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BUFFER_SIZE (4096U) 
#define VERSION32(hdr) (((hdr)->ih_ver.iv_major << 24) | ((hdr)->ih_ver.iv_minor << 16) | ((hdr)->ih_ver.iv_revision))

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void SDCARD_DetectCallBack(bool isInserted, void *userData);

const char imageName_sd[] = "newapp";
/*******************************************************************************
 * Variables
 ******************************************************************************/
static volatile bool s_cardInserted     = false;
static volatile bool s_cardInsertStatus = false;

static SemaphoreHandle_t s_CardDetectSemaphore = NULL;

static FATFS g_fileSystem;
static FIL g_fileObject; 

SDK_ALIGN(uint8_t g_bufferRead[SDK_SIZEALIGN(BUFFER_SIZE, SDMMC_DATA_BUFFER_ALIGN_CACHE)],
          MAX(SDMMC_DATA_BUFFER_ALIGN_CACHE, 4));

struct image_header *pri_hdr = (struct image_header *)(FLASH_DEVICE_BASE_ADDR + FLASH_AREA_IMAGE_1_OFFSET);

/*******************************************************************************
 * Code
 ******************************************************************************/
void BOARD_SD_InitPins(void) {
  CLOCK_EnableClock(kCLOCK_Iomuxc);           /* LPCG on: LPCG is ON. */

  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_32_GPIO_MUX3_IO31,       /* GPIO_AD_32 is configured as GPIO_MUX3_IO31 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_34_USDHC1_VSELECT,       /* GPIO_AD_34 is configured as USDHC1_VSELECT */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_35_GPIO10_IO02,          /* GPIO_AD_35 is configured as GPIO10_IO02 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_SD_B1_00_USDHC1_CMD,        /* GPIO_SD_B1_00 is configured as USDHC1_CMD */
      1U);                                    /* Software Input On Field: Force input path of pad GPIO_SD_B1_00 */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_SD_B1_01_USDHC1_CLK,        /* GPIO_SD_B1_01 is configured as USDHC1_CLK */
      1U);                                    /* Software Input On Field: Force input path of pad GPIO_SD_B1_01 */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_SD_B1_02_USDHC1_DATA0,      /* GPIO_SD_B1_02 is configured as USDHC1_DATA0 */
      1U);                                    /* Software Input On Field: Force input path of pad GPIO_SD_B1_02 */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_SD_B1_03_USDHC1_DATA1,      /* GPIO_SD_B1_03 is configured as USDHC1_DATA1 */
      1U);                                    /* Software Input On Field: Force input path of pad GPIO_SD_B1_03 */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_SD_B1_04_USDHC1_DATA2,      /* GPIO_SD_B1_04 is configured as USDHC1_DATA2 */
      1U);                                    /* Software Input On Field: Force input path of pad GPIO_SD_B1_04 */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_SD_B1_05_USDHC1_DATA3,      /* GPIO_SD_B1_05 is configured as USDHC1_DATA3 */
      1U);                                    /* Software Input On Field: Force input path of pad GPIO_SD_B1_05 */
  IOMUXC_GPR->GPR43 = ((IOMUXC_GPR->GPR43 &
    (~(IOMUXC_GPR_GPR43_GPIO_MUX3_GPIO_SEL_HIGH_MASK))) /* Mask bits to zero which are setting */
      | IOMUXC_GPR_GPR43_GPIO_MUX3_GPIO_SEL_HIGH(0x8000U) /* GPIO3 and GPIO_M7_3 share same IO MUX function, GPIO_MUX3 selects one GPIO function: 0x8000U */
    );
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_SD_B1_00_USDHC1_CMD,        /* GPIO_SD_B1_00 PAD functional properties : */
      0x04U);                                 /* PDRV Field: high driver
                                                 Pull Down Pull Up Field: PU
                                                 Open Drain Field: Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_SD_B1_01_USDHC1_CLK,        /* GPIO_SD_B1_01 PAD functional properties : */
      0x0CU);                                 /* PDRV Field: high driver
                                                 Pull Down Pull Up Field: No Pull
                                                 Open Drain Field: Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_SD_B1_02_USDHC1_DATA0,      /* GPIO_SD_B1_02 PAD functional properties : */
      0x04U);                                 /* PDRV Field: high driver
                                                 Pull Down Pull Up Field: PU
                                                 Open Drain Field: Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_SD_B1_03_USDHC1_DATA1,      /* GPIO_SD_B1_03 PAD functional properties : */
      0x04U);                                 /* PDRV Field: high driver
                                                 Pull Down Pull Up Field: PU
                                                 Open Drain Field: Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_SD_B1_04_USDHC1_DATA2,      /* GPIO_SD_B1_04 PAD functional properties : */
      0x04U);                                 /* PDRV Field: high driver
                                                 Pull Down Pull Up Field: PU
                                                 Open Drain Field: Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_SD_B1_05_USDHC1_DATA3,      /* GPIO_SD_B1_05 PAD functional properties : */
      0x04U);                                 /* PDRV Field: high driver
                                                 Pull Down Pull Up Field: PU
                                                 Open Drain Field: Disabled */
}

void local_version(void)
{
    if (pri_hdr->ih_magic == IMAGE_MAGIC)
        PRINTF("Local image version: %d.%d.%d\r\n", pri_hdr->ih_ver.iv_major,
	                                                pri_hdr->ih_ver.iv_minor,
                                                    pri_hdr->ih_ver.iv_revision);
    else
        PRINTF("Failed to get version number\r\n");
}

void SDCARD_DetectCallBack(bool isInserted, void *userData)
{
    s_cardInsertStatus = isInserted;
    xSemaphoreGiveFromISR(s_CardDetectSemaphore, NULL);
}

status_t DEMO_SDCardOTAUpgrade(void)
{
    char *image_title = NULL;
    char *dir;
    int dir_len;
    FRESULT error;
    DIR directory = {0};
    FILINFO fileInformation;
    UINT bytesRead, buffer_size;
    
    struct image_version *cur_ver;
    struct image_header *new_hdr;
    uint8_t version_buff[8];
    int8_t cmp_result;
    uint8_t image_position;
    uint32_t dstAddr = FLASH_AREA_IMAGE_2_OFFSET;
    
    const TCHAR driverNumberBuffer[3U] = {SDDISK + '0', ':', '/'};

    sfw_flash_read(REMAP_FLAG_ADDRESS, &image_position, 1);
    if(image_position == 0x01)
    {
        dstAddr = FLASH_AREA_IMAGE_2_OFFSET;
        sfw_flash_read_ipc(FLASH_AREA_IMAGE_1_OFFSET + IMAGE_VERSION_OFFSET, version_buff, 8);
        cur_ver = (struct image_version *)version_buff;
    }
    else if(image_position == 0x02)
    {
        dstAddr = FLASH_AREA_IMAGE_1_OFFSET;
        sfw_flash_read_ipc(FLASH_AREA_IMAGE_2_OFFSET + IMAGE_VERSION_OFFSET, version_buff, 8);
        cur_ver = (struct image_version *)version_buff;
    }
    else
    {
        return kStatus_Fail;
    }
    
    status_t status;
    volatile uint32_t primask;
    
    dir = ".";
    dir_len = strlen(dir);
    if ((dir[dir_len - 1] == '/') || (dir[dir_len - 1] == '\\'))
        dir[dir_len - 1] = '\0';

    if (f_mount(&g_fileSystem, driverNumberBuffer, 0U))
    {
        return kStatus_Fail;
    }

#if (FF_FS_RPATH >= 2U)
    error = f_chdrive((char const *)&driverNumberBuffer[0U]);
    if (error)
    {
        return kStatus_Fail;
    }
#endif
    
    error = f_opendir(&directory, dir);
    if (error)
    {
        return kStatus_Fail;
    }
    
    //PRINTF("dir list:\r\n");
    for (;;)
    {
        error = f_readdir(&directory, &fileInformation);
        if ((error != FR_OK) || (fileInformation.fname[0U] == 0U)) 
        {
            break;
        }
        if (fileInformation.fname[0] == '.') 
        {
            continue;
        }
#if 0        
        if (fileInformation.fattrib & AM_DIR) {
            PRINTF("\033[40;33m%s\033[0m \r\n", fileInformation.fname);
        } else {
            PRINTF("%s \r\n", fileInformation.fname);
        }
#endif   
            if (strncmp(fileInformation.fname, imageName_sd, (sizeof(imageName_sd)-1)) == 0)
            {
                image_title = fileInformation.fname;
                break;
            }
    }
    
    error = f_open(&g_fileObject, _T(image_title), FA_READ);
    if (error) 
    {
        goto exit;
    }
    
    PRINTF("reading...\r\n");
    
    buffer_size = sizeof(g_bufferRead);
    error = f_read(&g_fileObject, g_bufferRead, buffer_size, &bytesRead);
    if (error || (bytesRead != buffer_size) ) 
    {
        PRINTF("failed to read image data\r\n");
        goto exit;
    }
    
    new_hdr = (struct image_header *)g_bufferRead;
    if (new_hdr->ih_magic == IMAGE_MAGIC) 
    {
        if(cur_ver->iv_major == 0xff && cur_ver->iv_minor == 0xff)
        {
            PRINTF("new img verison: %d.%d.%d\r\n", new_hdr->ih_ver.iv_major,
                                new_hdr->ih_ver.iv_minor,
                                new_hdr->ih_ver.iv_revision);
        }
        else
        {
            cmp_result = compare_image_version(&new_hdr->ih_ver, cur_ver);
            if(cmp_result > 0)
            {
                PRINTF("new img verison: %d.%d.%d\r\n", new_hdr->ih_ver.iv_major,
                                            new_hdr->ih_ver.iv_minor,
                                            new_hdr->ih_ver.iv_revision);
            }
            else
            {
                PRINTF("The version number of the new image is not greater than the current image version number, please retry!\r\n");
                goto exit;
            }
        }
    } 
    else 
    {
        PRINTF("the new image is invalid.\r\n");
        goto exit;
    }
    
    primask = DisableGlobalIRQ();
    status = sfw_flash_erase(dstAddr, FLASH_AREA_IMAGE_2_SIZE);
    if (status) 
    {
        goto exit;
    }
    EnableGlobalIRQ(primask);

    PRINTF("updating...\r\n");
    
    primask = DisableGlobalIRQ();
    //already read the first 4KB for image validation checking, write to flash directly
    status = sfw_flash_write(dstAddr, (uint32_t *)g_bufferRead, bytesRead);  
    if (status) 
    {
        goto exit;
    }
    EnableGlobalIRQ(primask);
    
    while((bytesRead == buffer_size)) 
    {  
        dstAddr += bytesRead;
        error = f_read(&g_fileObject, (void *)g_bufferRead, buffer_size, &bytesRead);
        if (error == FR_OK) 
        {
            primask = DisableGlobalIRQ();
            status = sfw_flash_write(dstAddr, (uint32_t *)g_bufferRead, bytesRead);
            if (status) 
            {
                goto exit;
            }
            EnableGlobalIRQ(primask);
        }
        else 
        {
            goto exit;
        }
    }
    
    PRINTF("finished\r\n");
    
    write_update_type(UPDATE_TYPE_SDCARD);
    enable_image();
#if defined(CONFIG_BOOT_ENCRYPTED_XIP)
    update_key_context();
#endif
    //taskEXIT_CRITICAL();
      
    PRINTF("Please remove the SD Card!\r\n");
    PRINTF("sys rst...\r\n\r\n");
    vTaskDelay(5000U);

#if 0
    primask = DisableGlobalIRQ();
    status = flexspi_nor_flash_reset_opi(EXAMPLE_FLEXSPI);
    if (status) 
    {
        goto exit;
    }
    EnableGlobalIRQ(primask);
#endif

    NVIC_SystemReset();
    
exit:
    //FLEXSPI_SoftwareReset(EXAMPLE_FLEXSPI); //bus fault
    if (f_close(&g_fileObject)) 
    {
        return kStatus_Fail;   
    }
    return kStatus_Fail;    
}

void sdcard_ota_app(void *pvParameters)
{
    PRINTF("SD Card updating task enable.\r\n");
    
    s_CardDetectSemaphore = xSemaphoreCreateBinary();
    
    BOARD_SD_Config(&g_sd, SDCARD_DetectCallBack, 5, NULL);
    
    if(SD_HostInit(&g_sd) != kStatus_Success)
    {
        PRINTF("SD host init fail\r\n");
        vTaskDelete(NULL);
    }
    
    SD_SetCardPower(&g_sd, false);

    PRINTF("Please insert a sd card into board.\r\n");
    
    uint8_t last_update_type;
    
    sfw_flash_read(UPDATE_TYPE_FLAG_ADDRESS, &last_update_type, 1);
    
    if(last_update_type == UPDATE_TYPE_SDCARD)
    {
        PRINTF("Update done, the last update type: %s\r\n",
               last_update_type == UPDATE_TYPE_SDCARD ? "SD Card" :
               last_update_type == UPDATE_TYPE_UDISK ? "U-Disk" :
               last_update_type == UPDATE_TYPE_AWS_CLOUD ? 
                       "AWS platform" : "BUG; can't happend");
        write_image_ok();        
    }
    
    while(true)
    {
        if (xSemaphoreTake(s_CardDetectSemaphore, portMAX_DELAY) == pdTRUE) // 0
        {
            if (s_cardInserted != s_cardInsertStatus)
            {
                s_cardInserted = s_cardInsertStatus;
     
                if (s_cardInserted)
                {
                    PRINTF("Card inserted.\r\n");
                    /* power off card */
                    SD_SetCardPower(&g_sd, false);
                    /* power on the card */
                    SD_SetCardPower(&g_sd, true);
                    vTaskDelay(500U);
                    
                    //start your sdcard application here
                    if(DEMO_SDCardOTAUpgrade() != kStatus_Success)
                    {
                        PRINTF("sd card ota task running failed.\r\n");
                    }   
                }
            }
        }
        vTaskDelay(1000U);
    }
}

/*!
 * @brief Main function
 */
int sdcard_ota_main(void)
{
    BOARD_SD_InitPins();

    if (pdPASS != xTaskCreate(sdcard_ota_app, "sdcard_ota_app", 1024U, NULL,
                              configMAX_PRIORITIES - 4, NULL))
    {
        return -1;
    }
    
    return 0;
}
