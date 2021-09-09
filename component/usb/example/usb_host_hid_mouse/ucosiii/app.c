/*
 * Copyright (c) 2015 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 - 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "usb_host_config.h"
#include "usb_host.h"
#include "fsl_device_registers.h"
#include "usb_host_hid.h"
#include "board.h"
#include "host_mouse.h"
#include "fsl_common.h"
#if (defined(FSL_FEATURE_SOC_SYSMPU_COUNT) && (FSL_FEATURE_SOC_SYSMPU_COUNT > 0U))
#include "fsl_sysmpu.h"
#endif /* FSL_FEATURE_SOC_SYSMPU_COUNT */
#include "app.h"
#include "board.h"

#if ((!USB_HOST_CONFIG_KHCI) && (!USB_HOST_CONFIG_EHCI) && (!USB_HOST_CONFIG_OHCI) && (!USB_HOST_CONFIG_IP3516HS))
#error Please enable USB_HOST_CONFIG_KHCI, USB_HOST_CONFIG_EHCI, USB_HOST_CONFIG_OHCI, or USB_HOST_CONFIG_IP3516HS in file usb_host_config.
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*!
 * @brief host callback function.
 *
 * device attach/detach callback function.
 *
 * @param deviceHandle          device handle.
 * @param configurationHandle   attached device's configuration descriptor information.
 * @param eventCode             callback event code, please reference to enumeration host_event_t.
 *
 * @retval kStatus_USB_Success              The host is initialized successfully.
 * @retval kStatus_USB_NotSupported         The application don't support the configuration.
 */
static usb_status_t USB_HostEvent(usb_device_handle deviceHandle,
                                  usb_host_configuration_handle configurationHandle,
                                  uint32_t eventCode);

/*!
 * @brief application initialization.
 */
static void USB_HostApplicationInit(void);

/*!
 * @brief host ucosiii task function.
 *
 * @param g_HostHandle   host handle
 */
static void USB_HostTask(void *param);

/*!
 * @brief host mouse ucosiii task function.
 *
 * @param param   the host mouse instance pointer.
 */
static void USB_HostApplicationTask(void *param);

extern void USB_HostClockInit(void);
extern void USB_HostIsrEnable(void);
extern void USB_HostTaskFn(void *param);
void BOARD_InitHardware(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*! @brief USB host mouse instance global variable */
extern usb_host_mouse_instance_t g_HostHidMouse;
usb_host_handle g_HostHandle;

static uint8_t s_UsbHostApplicationTaskStack[USB_HOST_UCOSIII_APP_TASK_STACK_SIZE];
static uint8_t s_UsbHostTaskStack[USB_HOST_UCOSIII_HOST_TASK_STACK_SIZE];
static OS_TCB s_UsbHostApplicationTaskBlock;
static OS_TCB s_UsbHostTaskBlock;

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief USB isr function.
 */

static usb_status_t USB_HostEvent(usb_device_handle deviceHandle,
                                  usb_host_configuration_handle configurationHandle,
                                  uint32_t eventCode)
{
    usb_status_t status = kStatus_USB_Success;

    switch (eventCode & 0x0000FFFFU)
    {
        case kUSB_HostEventAttach:
            status = USB_HostHidMouseEvent(deviceHandle, configurationHandle, eventCode);
            break;

        case kUSB_HostEventNotSupported:
            usb_echo("device not supported.\r\n");
            break;

        case kUSB_HostEventEnumerationDone:
            status = USB_HostHidMouseEvent(deviceHandle, configurationHandle, eventCode);
            break;

        case kUSB_HostEventDetach:
            status = USB_HostHidMouseEvent(deviceHandle, configurationHandle, eventCode);
            break;

        case kUSB_HostEventEnumerationFail:
            usb_echo("enumeration failed\r\n");
            break;

        default:
            break;
    }
    return status;
}

static void USB_HostApplicationInit(void)
{
    usb_status_t status = kStatus_USB_Success;

    USB_HostClockInit();

#if ((defined FSL_FEATURE_SOC_SYSMPU_COUNT) && (FSL_FEATURE_SOC_SYSMPU_COUNT))
    SYSMPU_Enable(SYSMPU, 0);
#endif /* FSL_FEATURE_SOC_SYSMPU_COUNT */

    status = USB_HostInit(CONTROLLER_ID, &g_HostHandle, USB_HostEvent);
    if (status != kStatus_USB_Success)
    {
        usb_echo("host init error\r\n");
        return;
    }
    USB_HostIsrEnable();

    usb_echo("host init done\r\n");
}

static void USB_HostTask(void *param)
{
    while (1)
    {
        USB_HostTaskFn(param);
    }
}

static void USB_HostApplicationTask(void *param)
{
    OS_ERR error;

    OS_CPU_SysTickInit(SystemCoreClock); /* MUST be called after OSStart() */

    USB_HostApplicationInit();

    OSTaskCreate(&s_UsbHostTaskBlock,                                     /* Task control block handle */
                 "usb host task",                                         /* Task Name */
                 USB_HostTask,                                            /* pointer to the task */
                 g_HostHandle,                                            /* Task input parameter */
                 3,                                                       /* Task priority */
                 (CPU_STK *)&s_UsbHostTaskStack[0],                       /* Task stack base address */
                 0,                                                       /* Task stack limit */
                 USB_HOST_UCOSIII_HOST_TASK_STACK_SIZE / sizeof(CPU_STK), /* Task stack size */
                 0, /* Mumber of messages can be sent to the task */
                 0, /* Default time_quanta */
                 0, /* Task extension */
                 0, /* Task save FP */
                 &error);
    if (error != OS_ERR_NONE)
    {
        usb_echo("create host task error\r\n");
    }

    while (1)
    {
        USB_HostHidMouseTask(param);
    }
}

int main(void)
{
    OS_ERR error;

    BOARD_InitHardware();

    OSInit(&error);
#if OS_CFG_SCHED_ROUND_ROBIN_EN > 0u
    /* Enable task round robin. */
    OSSchedRoundRobinCfg((CPU_BOOLEAN)1, 0, &error);
#endif

    OSTaskCreate(&s_UsbHostApplicationTaskBlock,                         /* Task control block handle */
                 "app task",                                             /* Task Name */
                 USB_HostApplicationTask,                                /* pointer to the task */
                 &g_HostHidMouse,                                        /* Task input parameter */
                 4,                                                      /* Task priority */
                 (CPU_STK *)&s_UsbHostApplicationTaskStack[0],           /* Task stack base address */
                 0,                                                      /* Task stack limit */
                 USB_HOST_UCOSIII_APP_TASK_STACK_SIZE / sizeof(CPU_STK), /* Task stack size */
                 0, /* Mumber of messages can be sent to the task */
                 0, /* Default time_quanta */
                 0, /* Task extension */
                 0, /* Task save FP */
                 &error);
    if (error != OS_ERR_NONE)
    {
        usb_echo("create host task error\r\n");
    }

    OSStart(&error);

    while (1)
    {
        ;
    }
}
