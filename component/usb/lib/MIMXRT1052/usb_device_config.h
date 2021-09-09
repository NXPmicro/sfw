/*
 * Copyright (c) 2015 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 - 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _USB_DEVICE_CONFIG_H_
#define _USB_DEVICE_CONFIG_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*!
 * @addtogroup usb_device_configuration
 * @{
 */

/*!
 * @name Hardware instance define
 * @{
 */

/*! @brief KHCI instance count */
#define USB_DEVICE_CONFIG_KHCI (0U)

/*! @brief EHCI instance count */
#define USB_DEVICE_CONFIG_EHCI (2U)

/*! @brief LPC USB IP3511 FS instance count */
#define USB_DEVICE_CONFIG_LPCIP3511FS (0U)

/*! @brief LPC USB IP3511 HS instance count */
#define USB_DEVICE_CONFIG_LPCIP3511HS (0U)

/*! @brief Device instance count, the sum of KHCI and EHCI instance counts*/
#define USB_DEVICE_CONFIG_NUM \
    (USB_DEVICE_CONFIG_KHCI + USB_DEVICE_CONFIG_EHCI + USB_DEVICE_CONFIG_LPCIP3511FS + USB_DEVICE_CONFIG_LPCIP3511HS)

/* @} */

/*!
 * @name class instance define
 * @{
 */

/*! @brief HID instance count */
#define USB_DEVICE_CONFIG_HID (1U)

/*! @brief CDC ACM instance count */
#define USB_DEVICE_CONFIG_CDC_ACM (1U)

/*! @brief CDC ACM instance count */
#define USB_DEVICE_CONFIG_CDC_RNDIS (1U)
/*! @brief MSC instance count */
#define USB_DEVICE_CONFIG_MSC (1U)

/*! @brief Audio instance count */
#define USB_DEVICE_CONFIG_AUDIO (1U)

/*! @brief PHDC instance count */
#define USB_DEVICE_CONFIG_PHDC (1U)

/*! @brief Video instance count */
#define USB_DEVICE_CONFIG_VIDEO (1U)

/*! @brief CCID instance count */
#define USB_DEVICE_CONFIG_CCID (1U)

/*! @brief Printer instance count */
#define USB_DEVICE_CONFIG_PRINTER (1U)

/*! @brief DFU instance count */
#define USB_DEVICE_CONFIG_DFU (1U)

/* @} */

#define USB_DEVICE_CONFIG_AUDIO_CLASS_2_0 (1U)

/*! @brief Whether device is self power. 1U supported, 0U not supported */
#define USB_DEVICE_CONFIG_SELF_POWER (1U)

/*! @brief How many endpoints are supported in the stack. */
#define USB_DEVICE_CONFIG_ENDPOINTS (4U)

/*! @brief Whether the device task is enabled. */
#define USB_DEVICE_CONFIG_USE_TASK (0U)

/*! @brief How many the notification message are supported when the device task is enabled. */
#define USB_DEVICE_CONFIG_MAX_MESSAGES (8U)

/*! @brief Whether test mode enabled. */
#define USB_DEVICE_CONFIG_USB20_TEST_MODE (0U)

/*! @brief Whether device CV test is enabled. */
#define USB_DEVICE_CONFIG_CV_TEST (0U)

/*! @brief Whether device compliance test is enabled. If the macro is enabled,
    the test mode and CV test macroes will be set.*/
#define USB_DEVICE_CONFIG_COMPLIANCE_TEST (0U)

#if ((defined(USB_DEVICE_CONFIG_COMPLIANCE_TEST)) && (USB_DEVICE_CONFIG_COMPLIANCE_TEST > 0U))

/*! @brief Undefine the macro USB_DEVICE_CONFIG_USB20_TEST_MODE. */
#undef USB_DEVICE_CONFIG_USB20_TEST_MODE
/*! @brief Undefine the macro USB_DEVICE_CONFIG_CV_TEST. */
#undef USB_DEVICE_CONFIG_CV_TEST

/*! @brief enable the test mode. */
#define USB_DEVICE_CONFIG_USB20_TEST_MODE (1U)

/*! @brief enable the CV test */
#define USB_DEVICE_CONFIG_CV_TEST (1U)

#endif

#if ((defined(USB_DEVICE_CONFIG_KHCI)) && (USB_DEVICE_CONFIG_KHCI > 0U))

/*! @brief The MAX buffer length for the KHCI DMA workaround.*/
#define USB_DEVICE_CONFIG_KHCI_DMA_ALIGN_BUFFER_LENGTH (64U)
#endif

#if ((defined(USB_DEVICE_CONFIG_EHCI)) && (USB_DEVICE_CONFIG_EHCI > 0U))
/*! @brief How many the DTD are supported. */
#define USB_DEVICE_CONFIG_EHCI_MAX_DTD (16U)
/*! @brief Whether the EHCI ID pin detect feature enabled. */
#define USB_DEVICE_CONFIG_EHCI_ID_PIN_DETECT (1U)
#endif

/*! @brief Whether the keep alive feature enabled. */
#define USB_DEVICE_CONFIG_KEEP_ALIVE_MODE (1U)

/*! @brief Whether the transfer buffer is cache-enabled or not. */
#define USB_DEVICE_CONFIG_BUFFER_PROPERTY_CACHEABLE (0U)

/*! @brief Whether the low power mode is enabled or not. */
#define USB_DEVICE_CONFIG_LOW_POWER_MODE (1U)

#if ((defined(USB_DEVICE_CONFIG_LOW_POWER_MODE)) && (USB_DEVICE_CONFIG_LOW_POWER_MODE > 0U))
/*! @brief Whether device remote wakeup supported. 1U supported, 0U not supported */
#define USB_DEVICE_CONFIG_REMOTE_WAKEUP (1U)

/*! @brief Whether LPM is supported. 1U supported, 0U not supported */
#define USB_DEVICE_CONFIG_LPM_L1 (1U)
#else
/*! @brief The device remote wakeup is unsupported. */
#define USB_DEVICE_CONFIG_REMOTE_WAKEUP (1U)
#endif

/*! @brief Whether the device detached feature is enabled or not. */
#define USB_DEVICE_CONFIG_DETACH_ENABLE (1U)

/*! @brief Whether handle the USB bus error. */
#define USB_DEVICE_CONFIG_ERROR_HANDLING (0U)

/*! @brief Whether the device charger detect feature is enabled or not. */
#define USB_DEVICE_CONFIG_CHARGER_DETECT (1U)

/* @} */

#endif /* _USB_DEVICE_CONFIG_H_ */
