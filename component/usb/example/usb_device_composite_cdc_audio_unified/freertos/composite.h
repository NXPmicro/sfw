/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __USB_DEVICE_COMPOSITE_H__
#define __USB_DEVICE_COMPOSITE_H__ 1

#include "audio_unified.h"
#include "virtual_com.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "event_groups.h"
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

typedef struct _usb_device_composite_struct
{
    usb_device_handle deviceHandle;
    usb_audio_composite_struct_t audioUnified;
    usb_cdc_vcom_struct_t cdcVcom; /* CDC virtual com device structure. */
    TaskHandle_t applicationTaskHandle;
    TaskHandle_t deviceTaskHandle;
    uint8_t speed;
    uint8_t attach;
    uint8_t currentConfiguration;
    uint8_t currentInterfaceAlternateSetting[USB_DEVICE_INTERFACE_COUNT];
} usb_device_composite_struct_t;

extern void audio_generator_task(void);

/*******************************************************************************
 * API
 ******************************************************************************/
/*!
 * @brief Audio class specific callback function.
 *
 * This function handles the Audio class specific requests.
 *
 * @param handle		  The Audio class handle.
 * @param event 		  The Audio class event type.
 * @param param 		  The parameter of the class specific request.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
extern usb_status_t USB_DeviceAudioCompositeCallback(class_handle_t handle, uint32_t event, void *param);
/*!
 * @brief Audio device set configuration function.
 *
 * This function sets configuration for Audio class.
 *
 * @param handle The Audio class handle.
 * @param configure The Audio class configure index.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
extern usb_status_t USB_DeviceAudioCompositeSetConfigure(class_handle_t handle, uint8_t configure);
/*!
 * @brief Audio device initialization function.
 *
 * This function initializes the device with the composite device class information.
 *
 * @param device_composite The pointer to the composite device structure.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
extern usb_status_t USB_DeviceAudioCompositeInit(usb_device_composite_struct_t *device_composite);
extern usb_status_t USB_DeviceAudioRecorderSetInterface(class_handle_t handle,
                                                        uint8_t interface,
                                                        uint8_t alternateSetting);
extern usb_status_t USB_DeviceAudioSpeakerSetInterface(class_handle_t handle,
                                                       uint8_t interface,
                                                       uint8_t alternateSetting);
/*!
 * @brief CDC class specific callback function.
 *
 * This function handles the CDC class specific requests.
 *
 * @param handle          The CDC ACM class handle.
 * @param event           The CDC ACM class event type.
 * @param param           The parameter of the class specific request.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
extern usb_status_t USB_DeviceCdcVcomCallback(class_handle_t handle, uint32_t event, void *param);
/*!
 * @brief Virtual COM device set configuration function.
 *
 * This function sets configuration for CDC class.
 *
 * @param handle The CDC ACM class handle.
 * @param configure The CDC ACM class configure index.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
extern usb_status_t USB_DeviceCdcVcomSetConfigure(class_handle_t handle, uint8_t configure);
/*!
 * @brief Virtual COM device initialization function.
 *
 * This function initializes the device with the composite device class information.
 *
 * @param deviceComposite The pointer to the composite device structure.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
extern usb_status_t USB_DeviceCdcVcomInit(usb_device_composite_struct_t *deviceComposite);
/*!
 * @brief Application task function.
 *
 * This function runs the task for application.
 *
 * @return None.
 */
extern void USB_DeviceCdcVcomTask(void);

#endif
