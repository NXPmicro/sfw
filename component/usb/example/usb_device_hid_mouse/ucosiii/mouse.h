/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __USB_HID_MOUSE_H__
#define __USB_HID_MOUSE_H__

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
#define CONTROLLER_ID kUSB_ControllerEhci0
#endif
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U)
#define CONTROLLER_ID kUSB_ControllerKhci0
#endif
#if defined(USB_DEVICE_CONFIG_LPCIP3511FS) && (USB_DEVICE_CONFIG_LPCIP3511FS > 0U)
#define CONTROLLER_ID kUSB_ControllerLpcIp3511Fs0
#endif
#if defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U)
#define CONTROLLER_ID kUSB_ControllerLpcIp3511Hs0
#endif

#if defined(__GIC_PRIO_BITS)
#define USB_DEVICE_INTERRUPT_PRIORITY (25U)
#elif defined(__NVIC_PRIO_BITS) && (__NVIC_PRIO_BITS >= 3)
#define USB_DEVICE_INTERRUPT_PRIORITY (6U)
#else
#define USB_DEVICE_INTERRUPT_PRIORITY (3U)
#endif

#define USB_HID_MOUSE_REPORT_LENGTH (0x04U)

#define UCOSIII_APPLICATION_TASK_PRIORITY (0x0AU)
#define UCOSIII_DEVICE_TASK_PRIORITY (0x09U)

#define UCOSIII_APPLICATION_TASK_STACK (4000U)
#define UCOSIII_DEVICE_TASK_STACK (4000U)

typedef struct _ucosiii_task_struct
{
    CPU_STK *stackMemory; /* Pointer to task stack. */
    OS_TCB taskBlock;     /* Block of task. */
} ucosiii_task_struct_t;

typedef struct _usb_hid_mouse_struct
{
    usb_device_handle deviceHandle;
    class_handle_t hidHandle;
    ucosiii_task_struct_t applicationTask;
    ucosiii_task_struct_t deviceTask;
    uint8_t applicationTaskStack[UCOSIII_APPLICATION_TASK_STACK];
    uint8_t deviceTaskStack[UCOSIII_DEVICE_TASK_STACK];
    uint8_t *buffer;
    uint8_t currentConfiguration;
    uint8_t currentInterfaceAlternateSetting[USB_HID_MOUSE_INTERFACE_COUNT];
    uint8_t speed;
    uint8_t attach;
} usb_hid_mouse_struct_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#endif /* __USB_HID_MOUSE_H__ */