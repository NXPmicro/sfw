/*
 * Copyright (c) 2015 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 - 2017, 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "usb_device_config.h"
#include "usb.h"
#include "usb_device.h"

#include "usb_device_audio.h"

#include "usb_device_ch9.h"
#include "usb_device_descriptor.h"

#include "audio_speaker.h"
#include "fsl_device_registers.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_debug_console.h"
#include <stdio.h>
#include <stdlib.h>
#if (defined(FSL_FEATURE_SOC_SYSMPU_COUNT) && (FSL_FEATURE_SOC_SYSMPU_COUNT > 0U))
#include "fsl_sysmpu.h"
#endif /* FSL_FEATURE_SOC_SYSMPU_COUNT */

#include "app.h"

#if ((defined FSL_FEATURE_SOC_USBPHY_COUNT) && (FSL_FEATURE_SOC_USBPHY_COUNT > 0U))
#include "usb_phy.h"
#endif

#include "fsl_sctimer.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define USB_AUDIO_ENTER_CRITICAL() \
                                   \
    OSA_SR_ALLOC();                \
                                   \
    OSA_ENTER_CRITICAL()

#define USB_AUDIO_EXIT_CRITICAL() OSA_EXIT_CRITICAL()
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void APPInit(void);
usb_status_t USB_DeviceAudioProcessTerminalRequest(uint32_t audioCommand,
                                                   uint32_t *length,
                                                   uint8_t **buffer,
                                                   uint8_t entityOrEndpoint);
void BOARD_InitHardware(void);
void USB_DeviceClockInit(void);
void USB_DeviceIsrEnable(void);
#if USB_DEVICE_CONFIG_USE_TASK
void USB_DeviceTaskFn(void *deviceHandle);
#endif

extern void Init_Board_Audio(void);
extern void audio_trim_up(void);
extern void audio_trim_down(void);
/*******************************************************************************
 * Variables
 ******************************************************************************/
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
uint8_t audioPlayDataBuff[AUDIO_SPEAKER_DATA_WHOLE_BUFFER_LENGTH * AUDIO_PLAY_TRANSFER_SIZE];
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
uint8_t audioPlayPacket[(FS_ISO_OUT_ENDP_PACKET_SIZE + AUDIO_FORMAT_CHANNELS * AUDIO_FORMAT_SIZE)];

#if (defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U))
static uint32_t eventCounterU = 0;
static uint32_t captureRegisterNumber;
static sctimer_config_t sctimerInfo;
#endif
extern uint8_t
    g_UsbDeviceInterface[USB_AUDIO_SPEAKER_INTERFACE_COUNT]; /* Default value of audio speaker device struct */

USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE) static uint8_t s_SetupOutBuffer[8];
USB_DMA_INIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
usb_audio_speaker_struct_t g_UsbDeviceAudioSpeaker = {
    .deviceHandle                           = NULL,
    .currentStreamOutMaxPacketSize          = FS_ISO_OUT_ENDP_PACKET_SIZE,
    .attach                                 = 0U,
    .currentStreamInterfaceAlternateSetting = 0U,
    .copyProtect                            = 0x01U,
    .curMute                                = 0x00U,
    .curVolume                              = {0x00U, 0x1fU},
    .minVolume                              = {0x00U, 0x00U},
    .maxVolume                              = {0x00U, 0x43U},
    .resVolume                              = {0x01U, 0x00U},
    .curBass                                = 0x00U,
    .minBass                                = 0x80U,
    .maxBass                                = 0x7FU,
    .resBass                                = 0x01U,
    .curMid                                 = 0x00U,
    .minMid                                 = 0x80U,
    .maxMid                                 = 0x7FU,
    .resMid                                 = 0x01U,
    .curTreble                              = 0x01U,
    .minTreble                              = 0x80U,
    .maxTreble                              = 0x7FU,
    .resTreble                              = 0x01U,
    .curAutomaticGain                       = 0x01U,
    .curDelay                               = {0x00U, 0x40U},
    .minDelay                               = {0x00U, 0x00U},
    .maxDelay                               = {0xFFU, 0xFFU},
    .resDelay                               = {0x00U, 0x01U},
    .curLoudness                            = 0x01U,
    .curSamplingFrequency                   = {0x00U, 0x00U, 0x01U},
    .minSamplingFrequency                   = {0x00U, 0x00U, 0x01U},
    .maxSamplingFrequency                   = {0x00U, 0x00U, 0x01U},
    .resSamplingFrequency                   = {0x00U, 0x00U, 0x01U},
#if (USB_DEVICE_CONFIG_AUDIO_CLASS_2_0)
    .curMute20          = 0U,
    .curClockValid      = 1U,
    .curVolume20        = {0x00U, 0x1FU},
    .curSampleFrequency = 48000U, /* This should be changed to 48000 if sampling rate is 48k */
    .freqControlRange   = {1U, 48000U, 48000U, 0U},
    .volumeControlRange = {1U, 0x8001U, 0x7FFFU, 1U},
#endif
    .speed                  = USB_SPEED_FULL,
    .tdReadNumberPlay       = 0,
    .tdWriteNumberPlay      = 0,
    .audioSendCount         = 0,
    .lastAudioSendCount     = 0,
    .usbRecvCount           = 0,
    .audioSendTimes         = 0,
    .usbRecvTimes           = 0,
    .startPlay              = 0,
    .startPlayHalfFull      = 0,
    .speakerIntervalCount   = 0,
    .speakerReservedSpace   = 0,
    .timesFeedbackCalculate = 0,
    .speakerDetachOrNoInput = 0,
    .curAudioPllFrac        = AUDIO_PLL_FRACTIONAL_DIVIDER,
    .audioPllTicksPrev      = 0,
    .audioPllTicksDiff      = 0,
    .audioPllTicksEma       = AUDIO_PLL_USB1_SOF_INTERVAL_COUNT,
    .audioPllTickEmaFrac    = 0,
    .audioPllStep           = AUDIO_PLL_FRACTIONAL_CHANGE_STEP,
};
/*******************************************************************************
 * Code
 ******************************************************************************/

usb_status_t USB_DeviceAudioGetControlTerminal(usb_device_handle handle,
                                               usb_setup_struct_t *setup,
                                               uint32_t *length,
                                               uint8_t **buffer)
{
    usb_status_t error    = kStatus_USB_InvalidRequest;
    uint32_t audioCommand = 0U;
    uint8_t entityId      = (uint8_t)(setup->wIndex >> 0x08);

    switch (setup->bRequest)
    {
        /* Copy Protect Control only supports the CUR attribute!*/
        case USB_DEVICE_AUDIO_GET_CUR_VOLUME_REQUEST:
            break;
        default:
            break;
    }
    error = USB_DeviceAudioProcessTerminalRequest(audioCommand, length, buffer, entityId);
    return error;
}

usb_status_t USB_DeviceAudioGetCurAudioFeatureUnit(usb_device_handle handle,
                                                   usb_setup_struct_t *setup,
                                                   uint32_t *length,
                                                   uint8_t **buffer)
{
    usb_status_t error      = kStatus_USB_InvalidRequest;
    uint8_t controlSelector = (setup->wValue >> 0x08) & 0xFFU;
    uint32_t audioCommand   = 0U;
    uint8_t entityId        = (uint8_t)(setup->wIndex >> 0x08);

    /* Select SET request Control Feature Unit Module */
    switch (controlSelector)
    {
        case USB_DEVICE_AUDIO_MUTE_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_GET_CUR_MUTE_CONTROL;
            break;
        case USB_DEVICE_AUDIO_VOLUME_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_GET_CUR_VOLUME_CONTROL;
            break;
        case USB_DEVICE_AUDIO_BASS_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_GET_CUR_BASS_CONTROL;
            break;
        case USB_DEVICE_AUDIO_MID_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_GET_CUR_MID_CONTROL;
            break;
        case USB_DEVICE_AUDIO_TREBLE_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_GET_CUR_TREBLE_CONTROL;
            break;
        case USB_DEVICE_AUDIO_GRAPHIC_EQUALIZER_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_GET_CUR_GRAPHIC_EQUALIZER_CONTROL;
            break;
        case USB_DEVICE_AUDIO_AUTOMATIC_GAIN_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_GET_CUR_AUTOMATIC_GAIN_CONTROL;
            break;
        case USB_DEVICE_AUDIO_DELAY_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_GET_CUR_DELAY_CONTROL;
            break;
        case USB_DEVICE_AUDIO_BASS_BOOST_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_GET_CUR_BASS_BOOST_CONTROL;
            break;
        case USB_DEVICE_AUDIO_LOUDNESS_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_GET_CUR_LOUDNESS_CONTROL;
            break;
        default:
            break;
    }
    error = USB_DeviceAudioProcessTerminalRequest(audioCommand, length, buffer, entityId);

    return error;
}

usb_status_t USB_DeviceAudioGetMinAudioFeatureUnit(usb_device_handle handle,
                                                   usb_setup_struct_t *setup,
                                                   uint32_t *length,
                                                   uint8_t **buffer)
{
    usb_status_t error      = kStatus_USB_InvalidRequest;
    uint8_t controlSelector = (setup->wValue >> 0x08) & 0xFFU;
    uint32_t audioCommand   = 0U;
    uint8_t entityId        = (uint8_t)(setup->wIndex >> 0x08);

    /* Select SET request Control Feature Unit Module */
    switch (controlSelector)
    {
        case USB_DEVICE_AUDIO_VOLUME_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_GET_MIN_VOLUME_CONTROL;
            break;
        case USB_DEVICE_AUDIO_BASS_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_GET_MIN_BASS_CONTROL;
            break;
        case USB_DEVICE_AUDIO_MID_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_GET_MIN_MID_CONTROL;
            break;
        case USB_DEVICE_AUDIO_TREBLE_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_GET_MIN_TREBLE_CONTROL;
            break;
        case USB_DEVICE_AUDIO_GRAPHIC_EQUALIZER_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_GET_MIN_GRAPHIC_EQUALIZER_CONTROL;
            break;
        case USB_DEVICE_AUDIO_DELAY_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_GET_MIN_DELAY_CONTROL;
            break;
        default:
            break;
    }
    error = USB_DeviceAudioProcessTerminalRequest(audioCommand, length, buffer, entityId);
    return error;
}

usb_status_t USB_DeviceAudioGetMaxAudioFeatureUnit(usb_device_handle handle,
                                                   usb_setup_struct_t *setup,
                                                   uint32_t *length,
                                                   uint8_t **buffer)
{
    usb_status_t error      = kStatus_USB_InvalidRequest;
    uint8_t controlSelector = (setup->wValue >> 0x08) & 0xFFU;
    uint32_t audioCommand   = 0U;
    uint8_t entityId        = (uint8_t)(setup->wIndex >> 0x08);

    /* Select SET request Control Feature Unit Module */
    switch (controlSelector)
    {
        case USB_DEVICE_AUDIO_VOLUME_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_GET_MAX_VOLUME_CONTROL;
            break;
        case USB_DEVICE_AUDIO_BASS_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_GET_MAX_BASS_CONTROL;
            break;
        case USB_DEVICE_AUDIO_MID_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_GET_MAX_MID_CONTROL;
            break;
        case USB_DEVICE_AUDIO_TREBLE_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_GET_MAX_TREBLE_CONTROL;
            break;
        case USB_DEVICE_AUDIO_GRAPHIC_EQUALIZER_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_GET_MAX_GRAPHIC_EQUALIZER_CONTROL;
            break;
        case USB_DEVICE_AUDIO_DELAY_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_GET_MAX_DELAY_CONTROL;
            break;
        default:
            break;
    }
    error = USB_DeviceAudioProcessTerminalRequest(audioCommand, length, buffer, entityId);
    return error;
}

usb_status_t USB_DeviceAudioGetResAudioFeatureUnit(usb_device_handle handle,
                                                   usb_setup_struct_t *setup,
                                                   uint32_t *length,
                                                   uint8_t **buffer)
{
    usb_status_t error      = kStatus_USB_InvalidRequest;
    uint8_t controlSelector = (setup->wValue >> 0x08) & 0xFFU;
    uint32_t audioCommand   = 0U;
    uint8_t entityId        = (uint8_t)(setup->wIndex >> 0x08);

    /* Select SET request Control Feature Unit Module */
    switch (controlSelector)
    {
        case USB_DEVICE_AUDIO_VOLUME_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_GET_RES_VOLUME_CONTROL;
            break;
        case USB_DEVICE_AUDIO_BASS_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_GET_RES_BASS_CONTROL;
            break;
        case USB_DEVICE_AUDIO_MID_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_GET_RES_MID_CONTROL;
            break;
        case USB_DEVICE_AUDIO_TREBLE_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_GET_RES_TREBLE_CONTROL;
            break;
        case USB_DEVICE_AUDIO_GRAPHIC_EQUALIZER_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_GET_RES_GRAPHIC_EQUALIZER_CONTROL;
            break;
        case USB_DEVICE_AUDIO_DELAY_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_GET_RES_DELAY_CONTROL;
            break;
        default:
            break;
    }
    error = USB_DeviceAudioProcessTerminalRequest(audioCommand, length, buffer, entityId);

    return error;
}

usb_status_t USB_DeviceAudioSetCurAudioFeatureUnit(usb_device_handle handle,
                                                   usb_setup_struct_t *setup,
                                                   uint32_t *length,
                                                   uint8_t **buffer)
{
    usb_status_t error      = kStatus_USB_InvalidRequest;
    uint8_t controlSelector = (setup->wValue >> 0x08) & 0xFFU;
    uint32_t audioCommand   = 0U;
    uint8_t entityId        = (uint8_t)(setup->wIndex >> 0x08);

    switch (controlSelector)
    {
        case USB_DEVICE_AUDIO_MUTE_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_SET_CUR_MUTE_CONTROL;
            break;
        case USB_DEVICE_AUDIO_VOLUME_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_SET_CUR_VOLUME_CONTROL;
            break;
        case USB_DEVICE_AUDIO_BASS_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_SET_CUR_BASS_CONTROL;
            break;
        case USB_DEVICE_AUDIO_MID_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_SET_CUR_MID_CONTROL;
            break;
        case USB_DEVICE_AUDIO_TREBLE_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_SET_CUR_TREBLE_CONTROL;
            break;
        case USB_DEVICE_AUDIO_GRAPHIC_EQUALIZER_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_SET_CUR_GRAPHIC_EQUALIZER_CONTROL;
            break;
        case USB_DEVICE_AUDIO_AUTOMATIC_GAIN_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_SET_CUR_AUTOMATIC_GAIN_CONTROL;
            break;
        case USB_DEVICE_AUDIO_DELAY_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_SET_CUR_DELAY_CONTROL;
            break;
        case USB_DEVICE_AUDIO_BASS_BOOST_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_SET_CUR_BASS_BOOST_CONTROL;
            break;
        case USB_DEVICE_AUDIO_LOUDNESS_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_SET_CUR_LOUDNESS_CONTROL;
            break;
        default:
            break;
    }
    error = USB_DeviceAudioProcessTerminalRequest(audioCommand, length, buffer, entityId);
    return error;
}

usb_status_t USB_DeviceAudioSetMinAudioFeatureUnit(usb_device_handle handle,
                                                   usb_setup_struct_t *setup,
                                                   uint32_t *length,
                                                   uint8_t **buffer)
{
    usb_status_t error      = kStatus_USB_InvalidRequest;
    uint8_t controlSelector = (setup->wValue >> 0x08) & 0xFFU;
    uint32_t audioCommand   = 0U;
    uint8_t entityId        = (uint8_t)(setup->wIndex >> 0x08);

    switch (controlSelector)
    {
        case USB_DEVICE_AUDIO_VOLUME_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_SET_MIN_VOLUME_CONTROL;
            break;
        case USB_DEVICE_AUDIO_BASS_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_SET_MIN_BASS_CONTROL;
            break;
        case USB_DEVICE_AUDIO_MID_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_SET_MIN_MID_CONTROL;
            break;
        case USB_DEVICE_AUDIO_TREBLE_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_SET_MIN_TREBLE_CONTROL;
            break;
        case USB_DEVICE_AUDIO_GRAPHIC_EQUALIZER_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_SET_MIN_GRAPHIC_EQUALIZER_CONTROL;
            break;
        case USB_DEVICE_AUDIO_DELAY_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_SET_MIN_DELAY_CONTROL;
            break;
        default:
            break;
    }
    error = USB_DeviceAudioProcessTerminalRequest(audioCommand, length, buffer, entityId);

    return error;
}

usb_status_t USB_DeviceAudioSetMaxAudioFeatureUnit(usb_device_handle handle,
                                                   usb_setup_struct_t *setup,
                                                   uint32_t *length,
                                                   uint8_t **buffer)
{
    usb_status_t error      = kStatus_USB_InvalidRequest;
    uint8_t controlSelector = (setup->wValue >> 0x08) & 0xFFU;
    uint32_t audioCommand   = 0U;
    uint8_t entityId        = (uint8_t)(setup->wIndex >> 0x08);

    switch (controlSelector)
    {
        case USB_DEVICE_AUDIO_VOLUME_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_SET_MAX_VOLUME_CONTROL;
            break;
        case USB_DEVICE_AUDIO_BASS_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_SET_MAX_BASS_CONTROL;
            break;
        case USB_DEVICE_AUDIO_MID_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_SET_MAX_MID_CONTROL;
            break;
        case USB_DEVICE_AUDIO_TREBLE_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_SET_MAX_TREBLE_CONTROL;
            break;
        case USB_DEVICE_AUDIO_GRAPHIC_EQUALIZER_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_SET_MAX_GRAPHIC_EQUALIZER_CONTROL;
            break;
        case USB_DEVICE_AUDIO_DELAY_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_SET_MAX_DELAY_CONTROL;
            break;
        default:
            break;
    }
    error = USB_DeviceAudioProcessTerminalRequest(audioCommand, length, buffer, entityId);

    return error;
}

usb_status_t USB_DeviceAudioSetResAudioFeatureUnit(usb_device_handle handle,
                                                   usb_setup_struct_t *setup,
                                                   uint32_t *length,
                                                   uint8_t **buffer)
{
    usb_status_t error      = kStatus_USB_InvalidRequest;
    uint8_t controlSelector = (setup->wValue >> 0x08) & 0xFFU;
    uint32_t audioCommand   = 0U;
    uint8_t entityId        = (uint8_t)(setup->wIndex >> 0x08);

    switch (controlSelector)
    {
        case USB_DEVICE_AUDIO_VOLUME_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_SET_RES_VOLUME_CONTROL;
            break;
        case USB_DEVICE_AUDIO_BASS_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_SET_RES_BASS_CONTROL;
            break;
        case USB_DEVICE_AUDIO_MID_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_SET_RES_MID_CONTROL;
            break;
        case USB_DEVICE_AUDIO_TREBLE_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_SET_RES_TREBLE_CONTROL;
            break;
        case USB_DEVICE_AUDIO_GRAPHIC_EQUALIZER_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_SET_RES_GRAPHIC_EQUALIZER_CONTROL;
            break;
        case USB_DEVICE_AUDIO_DELAY_CONTROL_SELECTOR:
            audioCommand = USB_DEVICE_AUDIO_SET_RES_DELAY_CONTROL;
            break;
        default:
            break;
    }
    error = USB_DeviceAudioProcessTerminalRequest(audioCommand, length, buffer, entityId);
    return error;
}

usb_status_t USB_DeviceAudioGetFeatureUnit(usb_device_handle handle,
                                           usb_setup_struct_t *setup,
                                           uint32_t *length,
                                           uint8_t **buffer)
{
    usb_status_t error = kStatus_USB_InvalidRequest;

    /* Select SET request Control Feature Unit Module */
    switch (setup->bRequest)
    {
        case USB_DEVICE_AUDIO_GET_CUR_VOLUME_REQUEST:
            error = USB_DeviceAudioGetCurAudioFeatureUnit(handle, setup, length, buffer);
            break;
        case USB_DEVICE_AUDIO_GET_MIN_VOLUME_REQUEST:
            error = USB_DeviceAudioGetMinAudioFeatureUnit(handle, setup, length, buffer);
            break;
        case USB_DEVICE_AUDIO_GET_MAX_VOLUME_REQUEST:
            error = USB_DeviceAudioGetMaxAudioFeatureUnit(handle, setup, length, buffer);
            break;
        case USB_DEVICE_AUDIO_GET_RES_VOLUME_REQUEST:
            error = USB_DeviceAudioGetResAudioFeatureUnit(handle, setup, length, buffer);
            break;
        default:
            break;
    }
    return error;
}

usb_status_t USB_DeviceAudioSetFeatureUnit(usb_device_handle handle,
                                           usb_setup_struct_t *setup,
                                           uint32_t *length,
                                           uint8_t **buffer)
{
    usb_status_t error = kStatus_USB_InvalidRequest;
    /* Select SET request Control Feature Unit Module */
    switch (setup->bRequest)
    {
        case USB_DEVICE_AUDIO_SET_CUR_VOLUME_REQUEST:
            error = USB_DeviceAudioSetCurAudioFeatureUnit(handle, setup, length, buffer);
            break;
        case USB_DEVICE_AUDIO_SET_MIN_VOLUME_REQUEST:
            error = USB_DeviceAudioSetMinAudioFeatureUnit(handle, setup, length, buffer);
            break;
        case USB_DEVICE_AUDIO_SET_MAX_VOLUME_REQUEST:
            error = USB_DeviceAudioSetMaxAudioFeatureUnit(handle, setup, length, buffer);
            break;
        case USB_DEVICE_AUDIO_SET_RES_VOLUME_REQUEST:
            error = USB_DeviceAudioSetResAudioFeatureUnit(handle, setup, length, buffer);
            break;
        default:
            break;
    }
    return error;
}

usb_status_t USB_DeviceAudioSetControlTerminal(usb_device_handle handle,
                                               usb_setup_struct_t *setup,
                                               uint32_t *length,
                                               uint8_t **buffer)
{
    usb_status_t error    = kStatus_USB_InvalidRequest;
    uint32_t audioCommand = 0U;
    uint8_t entityId      = (uint8_t)(setup->wIndex >> 0x08);

    switch (setup->bRequest)
    {
        /* Copy Protect Control only supports the CUR attribute!*/
        case USB_DEVICE_AUDIO_SET_CUR_VOLUME_REQUEST:

            break;
        default:
            break;
    }
    error = USB_DeviceAudioProcessTerminalRequest(audioCommand, length, buffer, entityId);
    return error;
}

usb_status_t USB_DeviceAudioSetRequestInterface(usb_device_handle handle,
                                                usb_setup_struct_t *setup,
                                                uint32_t *length,
                                                uint8_t **buffer)
{
    usb_status_t error = kStatus_USB_InvalidRequest;
    uint8_t entity_id  = (uint8_t)(setup->wIndex >> 0x08);

    if (USB_AUDIO_SPEAKER_CONTROL_INPUT_TERMINAL_ID == entity_id)
    {
        error = USB_DeviceAudioSetControlTerminal(handle, setup, length, buffer);
    }
    else if (USB_AUDIO_SPEAKER_CONTROL_FEATURE_UNIT_ID == entity_id)
    {
        error = USB_DeviceAudioSetFeatureUnit(handle, setup, length, buffer);
    }
    else
    {
    }

    return error;
}

usb_status_t USB_DeviceAudioGetRequestInterface(usb_device_handle handle,
                                                usb_setup_struct_t *setup,
                                                uint32_t *length,
                                                uint8_t **buffer)
{
    usb_status_t error = kStatus_USB_InvalidRequest;
    uint8_t entity_id  = (uint8_t)(setup->wIndex >> 0x08);

    if (USB_AUDIO_SPEAKER_CONTROL_INPUT_TERMINAL_ID == entity_id)
    {
        error = USB_DeviceAudioGetControlTerminal(handle, setup, length, buffer);
    }
    else if (USB_AUDIO_SPEAKER_CONTROL_FEATURE_UNIT_ID == entity_id)
    {
        error = USB_DeviceAudioGetFeatureUnit(handle, setup, length, buffer);
    }
    else
    {
    }
    return error;
}

usb_status_t USB_DeviceAudioSetRequestEndpoint(usb_device_handle handle,
                                               usb_setup_struct_t *setup,
                                               uint32_t *length,
                                               uint8_t **buffer)
{
    usb_status_t error      = kStatus_USB_InvalidRequest;
    uint8_t controlSelector = (setup->wValue >> 0x08) & 0xFFU;
    uint32_t audioCommand   = 0U;
    uint8_t endpoint        = (uint8_t)(setup->wIndex >> 0x08);

    /* Select SET request Control Feature Unit Module */
    switch (setup->bRequest)
    {
        case USB_DEVICE_AUDIO_SET_CUR_VOLUME_REQUEST:
            switch (controlSelector)
            {
                case USB_DEVICE_AUDIO_SAMPLING_FREQ_CONTROL_SELECTOR:
                    audioCommand = USB_DEVICE_AUDIO_SET_CUR_SAMPLING_FREQ_CONTROL;
                    break;
                case USB_DEVICE_AUDIO_PITCH_CONTROL_SELECTOR:
                    audioCommand = USB_DEVICE_AUDIO_SET_CUR_PITCH_CONTROL;
                    break;
                default:
                    break;
            }
            break;
        case USB_DEVICE_AUDIO_SET_MIN_VOLUME_REQUEST:
            switch (controlSelector)
            {
                case USB_DEVICE_AUDIO_SAMPLING_FREQ_CONTROL_SELECTOR:
                    audioCommand = USB_DEVICE_AUDIO_SET_MIN_SAMPLING_FREQ_CONTROL;
                    break;
                default:
                    break;
            }
            break;
        case USB_DEVICE_AUDIO_SET_MAX_VOLUME_REQUEST:
            switch (controlSelector)
            {
                case USB_DEVICE_AUDIO_SAMPLING_FREQ_CONTROL_SELECTOR:
                    audioCommand = USB_DEVICE_AUDIO_SET_MAX_SAMPLING_FREQ_CONTROL;
                    break;
                default:
                    break;
            }
            break;
        case USB_DEVICE_AUDIO_SET_RES_VOLUME_REQUEST:
            switch (controlSelector)
            {
                case USB_DEVICE_AUDIO_SAMPLING_FREQ_CONTROL_SELECTOR:
                    audioCommand = USB_DEVICE_AUDIO_SET_RES_SAMPLING_FREQ_CONTROL;
                    break;
                default:
                    break;
            }
            break;

        default:
            break;
    }
    error = USB_DeviceAudioProcessTerminalRequest(audioCommand, length, buffer, endpoint); /* endpoint is not used */
    return error;
}

usb_status_t USB_DeviceAudioGetRequestEndpoint(usb_device_handle handle,
                                               usb_setup_struct_t *setup,
                                               uint32_t *length,
                                               uint8_t **buffer)
{
    usb_status_t error      = kStatus_USB_InvalidRequest;
    uint8_t controlSelector = (setup->wValue >> 0x08) & 0xFFU;
    uint32_t audioCommand   = 0U;
    uint8_t endpoint        = (uint8_t)(setup->wIndex >> 0x08);

    /* Select SET request Control Feature Unit Module */
    switch (setup->bRequest)
    {
        case USB_DEVICE_AUDIO_GET_CUR_VOLUME_REQUEST:
            switch (controlSelector)
            {
                case USB_DEVICE_AUDIO_SAMPLING_FREQ_CONTROL_SELECTOR:

                    audioCommand = USB_DEVICE_AUDIO_GET_CUR_SAMPLING_FREQ_CONTROL;
                    break;
                default:
                    break;
            }
            break;
        case USB_DEVICE_AUDIO_GET_MIN_VOLUME_REQUEST:
            switch (controlSelector)
            {
                case USB_DEVICE_AUDIO_SAMPLING_FREQ_CONTROL_SELECTOR:

                    audioCommand = USB_DEVICE_AUDIO_GET_MIN_SAMPLING_FREQ_CONTROL;
                    break;
                default:
                    break;
            }
            break;
        case USB_DEVICE_AUDIO_GET_MAX_VOLUME_REQUEST:
            switch (controlSelector)
            {
                case USB_DEVICE_AUDIO_SAMPLING_FREQ_CONTROL_SELECTOR:

                    audioCommand = USB_DEVICE_AUDIO_GET_MAX_SAMPLING_FREQ_CONTROL;
                    break;
                default:
                    break;
            }
            break;
        case USB_DEVICE_AUDIO_GET_RES_VOLUME_REQUEST:
            switch (controlSelector)
            {
                case USB_DEVICE_AUDIO_SAMPLING_FREQ_CONTROL_SELECTOR:
                    audioCommand = USB_DEVICE_AUDIO_GET_RES_SAMPLING_FREQ_CONTROL;

                    break;
                default:
                    break;
            }
            break;

        default:
            break;
    }
    error = USB_DeviceAudioProcessTerminalRequest(audioCommand, length, buffer, endpoint); /* endpoint is not used */
    return error;
}

usb_status_t USB_DeviceProcessClassRequest(usb_device_handle handle,
                                           usb_setup_struct_t *setup,
                                           uint32_t *length,
                                           uint8_t **buffer)
{
    usb_status_t error = kStatus_USB_InvalidRequest;
#if (USB_DEVICE_CONFIG_AUDIO_CLASS_2_0)
    /* Handle the audio class specific request. */
    uint8_t entityId      = (uint8_t)(setup->wIndex >> 0x08);
    uint32_t audioCommand = 0;

    switch (entityId)
    {
        case USB_AUDIO_SPEAKER_CONTROL_OUTPUT_TERMINAL_ID:
            break;
        case USB_AUDIO_SPEAKER_CONTROL_INPUT_TERMINAL_ID:
            break;
        case USB_AUDIO_SPEAKER_CONTROL_CLOCK_SOURCE_UNIT_ID:
            if (((setup->bmRequestType & USB_REQUEST_TYPE_DIR_MASK) == USB_REQUEST_TYPE_DIR_IN))
            {
                switch (setup->wValue >> 8)
                {
                    case USB_DEVICE_AUDIO_CS_SAM_FREQ_CONTROL:
                        if (setup->bRequest == USB_DEVICE_AUDIO_REQUEST_CUR)
                        {
                            audioCommand = USB_DEVICE_AUDIO_GET_CUR_SAM_FREQ_CONTROL;
                        }
                        else if (setup->bRequest == USB_DEVICE_AUDIO_REQUEST_RANGE)
                        {
                            audioCommand = USB_DEVICE_AUDIO_GET_RANGE_SAM_FREQ_CONTROL;
                        }
                        else
                        {
                        }
                        break;
                    case USB_DEVICE_AUDIO_CS_CLOCK_VALID_CONTROL:
                        audioCommand = USB_DEVICE_AUDIO_GET_CUR_CLOCK_VALID_CONTROL;
                        break;
                    default:
                        break;
                }
            }
            else if (((setup->bmRequestType & USB_REQUEST_TYPE_DIR_MASK) == USB_REQUEST_TYPE_DIR_OUT))
            {
                switch (setup->wValue >> 8)
                {
                    case USB_DEVICE_AUDIO_CS_SAM_FREQ_CONTROL:
                        audioCommand = USB_DEVICE_AUDIO_SET_CUR_SAM_FREQ_CONTROL;
                        break;
                    case USB_DEVICE_AUDIO_CS_CLOCK_VALID_CONTROL:
                        audioCommand = USB_DEVICE_AUDIO_SET_CUR_CLOCK_VALID_CONTROL;
                        break;
                    default:
                        break;
                }
            }
            else
            {
            }
            break;
        case USB_AUDIO_SPEAKER_CONTROL_FEATURE_UNIT_ID:
            if (((setup->bmRequestType & USB_REQUEST_TYPE_DIR_MASK) == USB_REQUEST_TYPE_DIR_IN))
            {
                switch (setup->wValue >> 8)
                {
                    case USB_DEVICE_AUDIO_FU_MUTE_CONTROL:
                        audioCommand = USB_DEVICE_AUDIO_GET_CUR_MUTE_CONTROL_AUDIO20;
                        break;
                    case USB_DEVICE_AUDIO_FU_VOLUME_CONTROL:
                        if (setup->bRequest == USB_DEVICE_AUDIO_REQUEST_CUR)
                        {
                            audioCommand = USB_DEVICE_AUDIO_GET_CUR_VOLUME_CONTROL_AUDIO20;
                        }
                        else if (setup->bRequest == USB_DEVICE_AUDIO_REQUEST_RANGE)
                        {
                            audioCommand = USB_DEVICE_AUDIO_GET_RANGE_VOLUME_CONTROL_AUDIO20;
                        }
                        else
                        {
                        }
                        break;
                    default:
                        break;
                }
            }
            else if (((setup->bmRequestType & USB_REQUEST_TYPE_DIR_MASK) == USB_REQUEST_TYPE_DIR_OUT))
            {
                switch (setup->wValue >> 8)
                {
                    case USB_DEVICE_AUDIO_FU_MUTE_CONTROL:
                        audioCommand = USB_DEVICE_AUDIO_SET_CUR_MUTE_CONTROL_AUDIO20;
                        break;
                    case USB_DEVICE_AUDIO_FU_VOLUME_CONTROL:
                        if (setup->bRequest == USB_DEVICE_AUDIO_REQUEST_CUR)
                        {
                            audioCommand = USB_DEVICE_AUDIO_SET_CUR_VOLUME_CONTROL_AUDIO20;
                        }
                        break;
                    default:
                        break;
                }
            }
            break;
        default:
            break;
    }

    error = USB_DeviceAudioProcessTerminalRequest(audioCommand, length, buffer, entityId);

#else

    switch (setup->bmRequestType)
    {
        case USB_DEVICE_AUDIO_SET_REQUEST_INTERFACE:
            error = USB_DeviceAudioSetRequestInterface(handle, setup, length, buffer);
            break;
        case USB_DEVICE_AUDIO_GET_REQUEST_INTERFACE:
            error = USB_DeviceAudioGetRequestInterface(handle, setup, length, buffer);
            break;
        case USB_DEVICE_AUDIO_SET_REQUEST_ENDPOINT:
            error = USB_DeviceAudioSetRequestEndpoint(handle, setup, length, buffer);
            break;
        case USB_DEVICE_AUDIO_GET_REQUEST_ENDPOINT:
            error = USB_DeviceAudioGetRequestEndpoint(handle, setup, length, buffer);
            break;
        default:
            break;
    }
#endif

    return error;
}

usb_status_t USB_DeviceAudioProcessTerminalRequest(uint32_t audioCommand,
                                                   uint32_t *length,
                                                   uint8_t **buffer,
                                                   uint8_t entityOrEndpoint)
{
    usb_status_t error     = kStatus_USB_Success;
    static uint16_t volume = 0;
    uint8_t *volBuffAddr;

    switch (audioCommand)
    {
        case USB_DEVICE_AUDIO_GET_CUR_MUTE_CONTROL:
            *buffer = &g_UsbDeviceAudioSpeaker.curMute;
            *length = sizeof(g_UsbDeviceAudioSpeaker.curMute);
            break;
        case USB_DEVICE_AUDIO_GET_CUR_VOLUME_CONTROL:
            *buffer = g_UsbDeviceAudioSpeaker.curVolume;
            *length = sizeof(g_UsbDeviceAudioSpeaker.curVolume);
            break;
        case USB_DEVICE_AUDIO_GET_CUR_BASS_CONTROL:
            *buffer = &g_UsbDeviceAudioSpeaker.curBass;
            *length = sizeof(g_UsbDeviceAudioSpeaker.curBass);
            break;
        case USB_DEVICE_AUDIO_GET_CUR_MID_CONTROL:
            *buffer = &g_UsbDeviceAudioSpeaker.curMid;
            *length = sizeof(g_UsbDeviceAudioSpeaker.curMid);
            break;
        case USB_DEVICE_AUDIO_GET_CUR_TREBLE_CONTROL:
            *buffer = &g_UsbDeviceAudioSpeaker.curTreble;
            *length = sizeof(g_UsbDeviceAudioSpeaker.curTreble);
            break;
        case USB_DEVICE_AUDIO_GET_CUR_AUTOMATIC_GAIN_CONTROL:
            *buffer = &g_UsbDeviceAudioSpeaker.curAutomaticGain;
            *length = sizeof(g_UsbDeviceAudioSpeaker.curAutomaticGain);
            break;
        case USB_DEVICE_AUDIO_GET_CUR_DELAY_CONTROL:
            *buffer = g_UsbDeviceAudioSpeaker.curDelay;
            *length = sizeof(g_UsbDeviceAudioSpeaker.curDelay);
            break;
        case USB_DEVICE_AUDIO_GET_CUR_SAMPLING_FREQ_CONTROL:
            *buffer = g_UsbDeviceAudioSpeaker.curSamplingFrequency;
            *length = sizeof(g_UsbDeviceAudioSpeaker.curSamplingFrequency);
            break;
        case USB_DEVICE_AUDIO_GET_MIN_VOLUME_CONTROL:
            *buffer = g_UsbDeviceAudioSpeaker.minVolume;
            *length = sizeof(g_UsbDeviceAudioSpeaker.minVolume);
            break;
        case USB_DEVICE_AUDIO_GET_MIN_BASS_CONTROL:
            *buffer = &g_UsbDeviceAudioSpeaker.minBass;
            *length = sizeof(g_UsbDeviceAudioSpeaker.minBass);
            break;
        case USB_DEVICE_AUDIO_GET_MIN_MID_CONTROL:
            *buffer = &g_UsbDeviceAudioSpeaker.minMid;
            *length = sizeof(g_UsbDeviceAudioSpeaker.minMid);
            break;
        case USB_DEVICE_AUDIO_GET_MIN_TREBLE_CONTROL:
            *buffer = &g_UsbDeviceAudioSpeaker.minTreble;
            *length = sizeof(g_UsbDeviceAudioSpeaker.minTreble);
            break;
        case USB_DEVICE_AUDIO_GET_MIN_DELAY_CONTROL:
            *buffer = g_UsbDeviceAudioSpeaker.minDelay;
            *length = sizeof(g_UsbDeviceAudioSpeaker.minDelay);
            break;
        case USB_DEVICE_AUDIO_GET_MIN_SAMPLING_FREQ_CONTROL:
            *buffer = g_UsbDeviceAudioSpeaker.minSamplingFrequency;
            *length = sizeof(g_UsbDeviceAudioSpeaker.minSamplingFrequency);
            break;
        case USB_DEVICE_AUDIO_GET_MAX_VOLUME_CONTROL:
            *buffer = g_UsbDeviceAudioSpeaker.maxVolume;
            *length = sizeof(g_UsbDeviceAudioSpeaker.maxVolume);
            break;
        case USB_DEVICE_AUDIO_GET_MAX_BASS_CONTROL:
            *buffer = &g_UsbDeviceAudioSpeaker.maxBass;
            *length = sizeof(g_UsbDeviceAudioSpeaker.maxBass);
            break;
        case USB_DEVICE_AUDIO_GET_MAX_MID_CONTROL:
            *buffer = &g_UsbDeviceAudioSpeaker.maxMid;
            *length = sizeof(g_UsbDeviceAudioSpeaker.maxMid);
            break;
        case USB_DEVICE_AUDIO_GET_MAX_TREBLE_CONTROL:
            *buffer = &g_UsbDeviceAudioSpeaker.maxTreble;
            *length = sizeof(g_UsbDeviceAudioSpeaker.maxTreble);
            break;
        case USB_DEVICE_AUDIO_GET_MAX_DELAY_CONTROL:
            *buffer = g_UsbDeviceAudioSpeaker.maxDelay;
            *length = sizeof(g_UsbDeviceAudioSpeaker.maxDelay);
            break;
        case USB_DEVICE_AUDIO_GET_MAX_SAMPLING_FREQ_CONTROL:
            *buffer = g_UsbDeviceAudioSpeaker.maxSamplingFrequency;
            *length = sizeof(g_UsbDeviceAudioSpeaker.maxSamplingFrequency);
            break;
        case USB_DEVICE_AUDIO_GET_RES_VOLUME_CONTROL:
            *buffer = g_UsbDeviceAudioSpeaker.resVolume;
            *length = sizeof(g_UsbDeviceAudioSpeaker.resVolume);
            break;
        case USB_DEVICE_AUDIO_GET_RES_BASS_CONTROL:
            *buffer = &g_UsbDeviceAudioSpeaker.resBass;
            *length = sizeof(g_UsbDeviceAudioSpeaker.resBass);
            break;
        case USB_DEVICE_AUDIO_GET_RES_MID_CONTROL:
            *buffer = &g_UsbDeviceAudioSpeaker.resMid;
            *length = sizeof(g_UsbDeviceAudioSpeaker.resMid);
            break;
        case USB_DEVICE_AUDIO_GET_RES_TREBLE_CONTROL:
            *buffer = &g_UsbDeviceAudioSpeaker.resTreble;
            *length = sizeof(g_UsbDeviceAudioSpeaker.resTreble);
            break;
        case USB_DEVICE_AUDIO_GET_RES_DELAY_CONTROL:
            *buffer = g_UsbDeviceAudioSpeaker.resDelay;
            *length = sizeof(g_UsbDeviceAudioSpeaker.resDelay);
            break;
        case USB_DEVICE_AUDIO_GET_RES_SAMPLING_FREQ_CONTROL:
            *buffer = g_UsbDeviceAudioSpeaker.resSamplingFrequency;
            *length = sizeof(g_UsbDeviceAudioSpeaker.resSamplingFrequency);
            break;
#if (USB_DEVICE_CONFIG_AUDIO_CLASS_2_0)
        case USB_DEVICE_AUDIO_GET_CUR_SAM_FREQ_CONTROL:
            *buffer = (uint8_t *)&g_UsbDeviceAudioSpeaker.curSampleFrequency;
            *length = sizeof(g_UsbDeviceAudioSpeaker.curSampleFrequency);
            break;
        case USB_DEVICE_AUDIO_GET_RANGE_SAM_FREQ_CONTROL:
            *buffer = (uint8_t *)&g_UsbDeviceAudioSpeaker.freqControlRange;
            *length = sizeof(g_UsbDeviceAudioSpeaker.freqControlRange);
            break;
        case USB_DEVICE_AUDIO_GET_CUR_CLOCK_VALID_CONTROL:
            *buffer = &g_UsbDeviceAudioSpeaker.curClockValid;
            *length = sizeof(g_UsbDeviceAudioSpeaker.curClockValid);
            break;
        case USB_DEVICE_AUDIO_GET_CUR_MUTE_CONTROL_AUDIO20:
            *buffer = (uint8_t *)&g_UsbDeviceAudioSpeaker.curMute20;
            *length = sizeof(g_UsbDeviceAudioSpeaker.curMute20);
            break;
        case USB_DEVICE_AUDIO_GET_CUR_VOLUME_CONTROL_AUDIO20:
            *buffer = (uint8_t *)&g_UsbDeviceAudioSpeaker.curVolume20;
            *length = sizeof(g_UsbDeviceAudioSpeaker.curVolume20);
            break;
        case USB_DEVICE_AUDIO_GET_RANGE_VOLUME_CONTROL_AUDIO20:
            *buffer = (uint8_t *)&g_UsbDeviceAudioSpeaker.volumeControlRange;
            *length = sizeof(g_UsbDeviceAudioSpeaker.volumeControlRange);
            break;
#endif

        case USB_DEVICE_AUDIO_SET_CUR_VOLUME_CONTROL:
            volBuffAddr                          = *buffer;
            g_UsbDeviceAudioSpeaker.curVolume[0] = *volBuffAddr;
            g_UsbDeviceAudioSpeaker.curVolume[1] = *(volBuffAddr + 1);
            volume                               = (uint16_t)((uint16_t)g_UsbDeviceAudioSpeaker.curVolume[1] << 8U);
            volume |= (uint8_t)(g_UsbDeviceAudioSpeaker.curVolume[0]);
            g_UsbDeviceAudioSpeaker.codecTask |= VOLUME_CHANGE_TASK;
            /* If needs print information while adjusting the volume, please enable the following sentence. */
            /* usb_echo("Set Cur Volume : %x\r\n", volume); */
            break;
        case USB_DEVICE_AUDIO_SET_CUR_MUTE_CONTROL:
            g_UsbDeviceAudioSpeaker.curMute = **(buffer);
            if (g_UsbDeviceAudioSpeaker.curMute)
            {
                g_UsbDeviceAudioSpeaker.codecTask |= MUTE_CODEC_TASK;
            }
            else
            {
                g_UsbDeviceAudioSpeaker.codecTask |= UNMUTE_CODEC_TASK;
            }
            /* If needs print information while adjusting the volume, please enable the following sentence. */
            /* usb_echo("Set Cur Mute : %x\r\n", g_UsbDeviceAudioSpeaker.curMute); */
            break;
        case USB_DEVICE_AUDIO_SET_CUR_BASS_CONTROL:
            g_UsbDeviceAudioSpeaker.curBass = **(buffer);
            break;
        case USB_DEVICE_AUDIO_SET_CUR_MID_CONTROL:
            g_UsbDeviceAudioSpeaker.curMid = **(buffer);
            break;
        case USB_DEVICE_AUDIO_SET_CUR_TREBLE_CONTROL:
            g_UsbDeviceAudioSpeaker.curTreble = **(buffer);
            break;
        case USB_DEVICE_AUDIO_SET_CUR_AUTOMATIC_GAIN_CONTROL:
            g_UsbDeviceAudioSpeaker.curAutomaticGain = **(buffer);
            break;
        case USB_DEVICE_AUDIO_SET_CUR_DELAY_CONTROL:
            g_UsbDeviceAudioSpeaker.curDelay[0] = **(buffer);
            g_UsbDeviceAudioSpeaker.curDelay[1] = *((*buffer) + 1);
            break;
        case USB_DEVICE_AUDIO_SET_CUR_SAMPLING_FREQ_CONTROL:
            g_UsbDeviceAudioSpeaker.curSamplingFrequency[0] = **(buffer);
            g_UsbDeviceAudioSpeaker.curSamplingFrequency[1] = *((*buffer) + 1);
            break;
        case USB_DEVICE_AUDIO_SET_MIN_VOLUME_CONTROL:
            g_UsbDeviceAudioSpeaker.minVolume[0] = **(buffer);
            g_UsbDeviceAudioSpeaker.minVolume[1] = *((*buffer) + 1);
            break;
        case USB_DEVICE_AUDIO_SET_MIN_BASS_CONTROL:
            g_UsbDeviceAudioSpeaker.minBass = **(buffer);
            break;
        case USB_DEVICE_AUDIO_SET_MIN_MID_CONTROL:
            g_UsbDeviceAudioSpeaker.minMid = **(buffer);
            break;
        case USB_DEVICE_AUDIO_SET_MIN_TREBLE_CONTROL:
            g_UsbDeviceAudioSpeaker.minTreble = **(buffer);
            break;
        case USB_DEVICE_AUDIO_SET_MIN_DELAY_CONTROL:
            g_UsbDeviceAudioSpeaker.minDelay[0] = **(buffer);
            g_UsbDeviceAudioSpeaker.minDelay[1] = *((*buffer) + 1);
            break;
        case USB_DEVICE_AUDIO_SET_MIN_SAMPLING_FREQ_CONTROL:
            g_UsbDeviceAudioSpeaker.minSamplingFrequency[0] = **(buffer);
            g_UsbDeviceAudioSpeaker.minSamplingFrequency[1] = *((*buffer) + 1);
            break;
        case USB_DEVICE_AUDIO_SET_MAX_VOLUME_CONTROL:
            g_UsbDeviceAudioSpeaker.maxVolume[0] = **(buffer);
            g_UsbDeviceAudioSpeaker.maxVolume[1] = *((*buffer) + 1);
            break;
        case USB_DEVICE_AUDIO_SET_MAX_BASS_CONTROL:
            g_UsbDeviceAudioSpeaker.maxBass = **(buffer);
            break;
        case USB_DEVICE_AUDIO_SET_MAX_MID_CONTROL:
            g_UsbDeviceAudioSpeaker.maxMid = **(buffer);
            break;
        case USB_DEVICE_AUDIO_SET_MAX_TREBLE_CONTROL:
            g_UsbDeviceAudioSpeaker.maxTreble = **(buffer);
            break;
        case USB_DEVICE_AUDIO_SET_MAX_DELAY_CONTROL:
            g_UsbDeviceAudioSpeaker.maxDelay[0] = **(buffer);
            g_UsbDeviceAudioSpeaker.maxDelay[1] = *((*buffer) + 1);
            break;
        case USB_DEVICE_AUDIO_SET_MAX_SAMPLING_FREQ_CONTROL:
            g_UsbDeviceAudioSpeaker.maxSamplingFrequency[0] = **(buffer);
            g_UsbDeviceAudioSpeaker.maxSamplingFrequency[1] = *((*buffer) + 1);
            break;
        case USB_DEVICE_AUDIO_SET_RES_VOLUME_CONTROL:
            g_UsbDeviceAudioSpeaker.resVolume[0] = **(buffer);
            g_UsbDeviceAudioSpeaker.resVolume[1] = *((*buffer) + 1);
            break;
        case USB_DEVICE_AUDIO_SET_RES_BASS_CONTROL:
            g_UsbDeviceAudioSpeaker.resBass = **(buffer);
            break;
        case USB_DEVICE_AUDIO_SET_RES_MID_CONTROL:
            g_UsbDeviceAudioSpeaker.resMid = **(buffer);
            break;
        case USB_DEVICE_AUDIO_SET_RES_TREBLE_CONTROL:
            g_UsbDeviceAudioSpeaker.resTreble = **(buffer);
            break;
        case USB_DEVICE_AUDIO_SET_RES_DELAY_CONTROL:
            g_UsbDeviceAudioSpeaker.resDelay[0] = **(buffer);
            g_UsbDeviceAudioSpeaker.resDelay[1] = *((*buffer) + 1);
            break;
        case USB_DEVICE_AUDIO_SET_RES_SAMPLING_FREQ_CONTROL:
            g_UsbDeviceAudioSpeaker.resSamplingFrequency[0] = **(buffer);
            g_UsbDeviceAudioSpeaker.resSamplingFrequency[1] = *((*buffer) + 1);
            break;
#if (USB_DEVICE_CONFIG_AUDIO_CLASS_2_0)
        case USB_DEVICE_AUDIO_SET_CUR_SAM_FREQ_CONTROL:
            g_UsbDeviceAudioSpeaker.curSampleFrequency = **(buffer);
            break;
        case USB_DEVICE_AUDIO_SET_CUR_CLOCK_VALID_CONTROL:
            g_UsbDeviceAudioSpeaker.curClockValid = **(buffer);
            break;
        case USB_DEVICE_AUDIO_SET_CUR_MUTE_CONTROL_AUDIO20:
            g_UsbDeviceAudioSpeaker.curMute20 = **(buffer);
            if (g_UsbDeviceAudioSpeaker.curMute20)
            {
                g_UsbDeviceAudioSpeaker.codecTask |= MUTE_CODEC_TASK;
            }
            else
            {
                g_UsbDeviceAudioSpeaker.codecTask |= UNMUTE_CODEC_TASK;
            }
            break;
        case USB_DEVICE_AUDIO_SET_CUR_VOLUME_CONTROL_AUDIO20:
            volBuffAddr                            = *buffer;
            g_UsbDeviceAudioSpeaker.curVolume20[0] = *(volBuffAddr);
            g_UsbDeviceAudioSpeaker.curVolume20[1] = *(volBuffAddr + 1);
            g_UsbDeviceAudioSpeaker.codecTask |= VOLUME_CHANGE_TASK;
            break;
#endif
        default:
            error = kStatus_USB_InvalidRequest;
            break;
    }
    return error;
}

/* The USB_AudioSpeakerBufferSpaceUsed() function gets the used speaker ringbuffer size */
uint32_t USB_AudioSpeakerBufferSpaceUsed(void)
{
    if (g_UsbDeviceAudioSpeaker.tdReadNumberPlay > g_UsbDeviceAudioSpeaker.tdWriteNumberPlay)
    {
        g_UsbDeviceAudioSpeaker.speakerReservedSpace =
            g_UsbDeviceAudioSpeaker.tdReadNumberPlay - g_UsbDeviceAudioSpeaker.tdWriteNumberPlay;
    }
    else
    {
        g_UsbDeviceAudioSpeaker.speakerReservedSpace =
            g_UsbDeviceAudioSpeaker.tdReadNumberPlay +
            AUDIO_SPEAKER_DATA_WHOLE_BUFFER_LENGTH * AUDIO_PLAY_TRANSFER_SIZE -
            g_UsbDeviceAudioSpeaker.tdWriteNumberPlay;
    }
    return g_UsbDeviceAudioSpeaker.speakerReservedSpace;
}

#if (defined(USB_DEVICE_CONFIG_LPCIP3511FS) && (USB_DEVICE_CONFIG_LPCIP3511FS > 0U))
static int32_t audioSpeakerUsedDiff = 0x0, audioSpeakerDiffThres = 0x0;
static uint32_t audioSpeakerUsedSpace = 0x0, audioSpeakerLastUsedSpace = 0x0;
void USB_AudioFSSync()
{
    if (g_UsbDeviceAudioSpeaker.speakerIntervalCount != AUDIO_CALCULATE_Ff_INTERVAL)
    {
        g_UsbDeviceAudioSpeaker.speakerIntervalCount++;
        return;
    }
    g_UsbDeviceAudioSpeaker.speakerIntervalCount = 1;
    g_UsbDeviceAudioSpeaker.timesFeedbackCalculate++;
    if (g_UsbDeviceAudioSpeaker.timesFeedbackCalculate == 2)
    {
        audioSpeakerUsedSpace     = USB_AudioSpeakerBufferSpaceUsed();
        audioSpeakerLastUsedSpace = audioSpeakerUsedSpace;
    }

    if (g_UsbDeviceAudioSpeaker.timesFeedbackCalculate > 2)
    {
        audioSpeakerUsedSpace = USB_AudioSpeakerBufferSpaceUsed();
        audioSpeakerUsedDiff += (audioSpeakerUsedSpace - audioSpeakerLastUsedSpace);
        audioSpeakerLastUsedSpace = audioSpeakerUsedSpace;

        if ((audioSpeakerUsedDiff > -AUDIO_SAMPLING_RATE_KHZ) && (audioSpeakerUsedDiff < AUDIO_SAMPLING_RATE_KHZ))
        {
            audioSpeakerDiffThres = 4 * AUDIO_SAMPLING_RATE_KHZ;
        }
        if (audioSpeakerUsedDiff <= -audioSpeakerDiffThres)
        {
            audioSpeakerDiffThres += 4 * AUDIO_SAMPLING_RATE_KHZ;
            audio_trim_down();
        }
        if (audioSpeakerUsedDiff >= audioSpeakerDiffThres)
        {
            audioSpeakerDiffThres += 4 * AUDIO_SAMPLING_RATE_KHZ;
            audio_trim_up();
        }
    }
}
#endif

/* The USB_AudioSpeakerPutBuffer() function fills the audioRecDataBuff with audioPlayPacket in every callback*/
void USB_AudioSpeakerPutBuffer(uint8_t *buffer, uint32_t size)
{
#if defined(USB_AUDIO_UAC5_1) && (USB_AUDIO_UAC5_1 > 0U)
    uint8_t *buffer_temp;
    for (uint32_t i = 0; i < size; i += AUDIO_FORMAT_SIZE * AUDIO_FORMAT_CHANNELS)
    {
        buffer_temp = buffer + i;
        for (uint32_t j = 0; j < AUDIO_FORMAT_SIZE * 2; j++)
        {
#if defined(USB_AUDIO_UAC5_1_CHANNEL_PAIR_SEL) && (0x01 == USB_AUDIO_UAC5_1_CHANNEL_PAIR_SEL)
            audioPlayDataBuff[g_UsbDeviceAudioSpeaker.tdReadNumberPlay] = *(buffer_temp + j);
#endif
#if defined(USB_AUDIO_UAC5_1_CHANNEL_PAIR_SEL) && (0x02 == USB_AUDIO_UAC5_1_CHANNEL_PAIR_SEL)
            audioPlayDataBuff[g_UsbDeviceAudioSpeaker.tdReadNumberPlay] = *(buffer_temp + j + AUDIO_FORMAT_SIZE * 2);
#endif
#if defined(USB_AUDIO_UAC5_1_CHANNEL_PAIR_SEL) && (0x03 == USB_AUDIO_UAC5_1_CHANNEL_PAIR_SEL)
            audioPlayDataBuff[g_UsbDeviceAudioSpeaker.tdReadNumberPlay] = *(buffer_temp + j + AUDIO_FORMAT_SIZE * 4);
#endif
            g_UsbDeviceAudioSpeaker.tdReadNumberPlay++;
            if (g_UsbDeviceAudioSpeaker.tdReadNumberPlay >=
                AUDIO_SPEAKER_DATA_WHOLE_BUFFER_LENGTH * AUDIO_PLAY_TRANSFER_SIZE)
            {
                g_UsbDeviceAudioSpeaker.tdReadNumberPlay = 0;
            }
        }
    }
#else
    while (size)
    {
        audioPlayDataBuff[g_UsbDeviceAudioSpeaker.tdReadNumberPlay] = *buffer;
        g_UsbDeviceAudioSpeaker.tdReadNumberPlay++;
        buffer++;
        size--;

        if (g_UsbDeviceAudioSpeaker.tdReadNumberPlay >=
            AUDIO_SPEAKER_DATA_WHOLE_BUFFER_LENGTH * AUDIO_PLAY_TRANSFER_SIZE)
        {
            g_UsbDeviceAudioSpeaker.tdReadNumberPlay = 0;
        }
    }
#endif
}

/* USB device audio ISO IN endpoint callback */
usb_status_t USB_DeviceAudioIsoOut(usb_device_handle deviceHandle,
                                   usb_device_endpoint_callback_message_struct_t *event,
                                   void *arg)
{
    usb_status_t error = kStatus_USB_InvalidRequest;
    usb_device_endpoint_callback_message_struct_t *ep_cb_param;
    ep_cb_param = (usb_device_endpoint_callback_message_struct_t *)event;

    if ((g_UsbDeviceAudioSpeaker.attach) && (ep_cb_param->length != (USB_UNINITIALIZED_VAL_32)))
    {
        if (g_UsbDeviceAudioSpeaker.startPlay == 0)
        {
            g_UsbDeviceAudioSpeaker.startPlay = 1;
        }
        if ((g_UsbDeviceAudioSpeaker.tdReadNumberPlay >=
             AUDIO_SPEAKER_DATA_WHOLE_BUFFER_LENGTH * AUDIO_PLAY_TRANSFER_SIZE / 2) &&
            (g_UsbDeviceAudioSpeaker.startPlayHalfFull == 0))
        {
            g_UsbDeviceAudioSpeaker.startPlayHalfFull = 1;
        }
        USB_AudioSpeakerPutBuffer(audioPlayPacket, ep_cb_param->length);
        g_UsbDeviceAudioSpeaker.usbRecvCount += ep_cb_param->length;
        g_UsbDeviceAudioSpeaker.usbRecvTimes++;
#if (defined(USB_DEVICE_CONFIG_LPCIP3511FS) && (USB_DEVICE_CONFIG_LPCIP3511FS > 0U))
        USB_AUDIO_ENTER_CRITICAL();
        USB_AudioFSSync();
        USB_AUDIO_EXIT_CRITICAL();
#endif
        error = USB_DeviceRecvRequest(deviceHandle, USB_AUDIO_SPEAKER_STREAM_ENDPOINT, &audioPlayPacket[0],
                                      g_UsbDeviceAudioSpeaker.currentStreamOutMaxPacketSize);
    }
    return error;
}

/*!
 * @brief Get the setup packet buffer.
 *
 * This function provides the buffer for setup packet.
 *
 * @param handle The USB device handle.
 * @param setupBuffer The pointer to the address of setup packet buffer.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceGetSetupBuffer(usb_device_handle handle, usb_setup_struct_t **setupBuffer)
{
    static uint32_t audioSpeakerSetup[2];
    if (NULL == setupBuffer)
    {
        return kStatus_USB_InvalidParameter;
    }
    *setupBuffer = (usb_setup_struct_t *)&audioSpeakerSetup;
    return kStatus_USB_Success;
}

/*!
 * @brief Get the setup packet data buffer.
 *
 * This function gets the data buffer for setup packet.
 *
 * @param handle The USB device handle.
 * @param setup The pointer to the setup packet.
 * @param length The pointer to the length of the data buffer.
 * @param buffer The pointer to the address of setup packet data buffer.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceGetClassReceiveBuffer(usb_device_handle handle,
                                             usb_setup_struct_t *setup,
                                             uint32_t *length,
                                             uint8_t **buffer)
{
    if ((NULL == buffer) || ((*length) > sizeof(s_SetupOutBuffer)))
    {
        return kStatus_USB_InvalidRequest;
    }
    *buffer = s_SetupOutBuffer;
    return kStatus_USB_Success;
}

/*!
 * @brief Configure remote wakeup feature.
 *
 * This function configures the remote wakeup feature.
 *
 * @param handle The USB device handle.
 * @param enable 1: enable, 0: disable.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceConfigureRemoteWakeup(usb_device_handle handle, uint8_t enable)
{
    return kStatus_USB_InvalidRequest;
}

/*!
 * @brief USB configure endpoint function.
 *
 * This function configure endpoint status.
 *
 * @param handle The USB device handle.
 * @param ep Endpoint address.
 * @param status A flag to indicate whether to stall the endpoint. 1: stall, 0: unstall.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceConfigureEndpointStatus(usb_device_handle handle, uint8_t ep, uint8_t status)
{
    if (status)
    {
        if ((USB_AUDIO_SPEAKER_STREAM_ENDPOINT == (ep & USB_ENDPOINT_NUMBER_MASK)) && (ep & 0x80))
        {
            return USB_DeviceStallEndpoint(handle, ep);
        }
        else if ((USB_AUDIO_CONTROL_ENDPOINT == (ep & USB_ENDPOINT_NUMBER_MASK)) && (ep & 0x80))
        {
            return USB_DeviceStallEndpoint(handle, ep);
        }
        else
        {
        }
    }
    else
    {
        if ((USB_AUDIO_SPEAKER_STREAM_ENDPOINT == (ep & USB_ENDPOINT_NUMBER_MASK)) && (ep & 0x80))
        {
            return USB_DeviceUnstallEndpoint(handle, ep);
        }
        else if ((USB_AUDIO_CONTROL_ENDPOINT == (ep & USB_ENDPOINT_NUMBER_MASK)) && (ep & 0x80))
        {
            return USB_DeviceUnstallEndpoint(handle, ep);
        }
        else
        {
        }
    }
    return kStatus_USB_InvalidRequest;
}

/* The USB_DeviceAudioSpeakerStatusReset() function resets the audio speaker status to the initialized status */
void USB_DeviceAudioSpeakerStatusReset(void)
{
    g_UsbDeviceAudioSpeaker.startPlay              = 0;
    g_UsbDeviceAudioSpeaker.startPlayHalfFull      = 0;
    g_UsbDeviceAudioSpeaker.tdReadNumberPlay       = 0;
    g_UsbDeviceAudioSpeaker.tdWriteNumberPlay      = 0;
    g_UsbDeviceAudioSpeaker.audioSendCount         = 0;
    g_UsbDeviceAudioSpeaker.usbRecvCount           = 0;
    g_UsbDeviceAudioSpeaker.lastAudioSendCount     = 0;
    g_UsbDeviceAudioSpeaker.audioSendTimes         = 0;
    g_UsbDeviceAudioSpeaker.usbRecvTimes           = 0;
    g_UsbDeviceAudioSpeaker.speakerIntervalCount   = 0;
    g_UsbDeviceAudioSpeaker.speakerReservedSpace   = 0;
    g_UsbDeviceAudioSpeaker.timesFeedbackCalculate = 0;
    g_UsbDeviceAudioSpeaker.speakerDetachOrNoInput = 0;
    g_UsbDeviceAudioSpeaker.curAudioPllFrac        = AUDIO_PLL_FRACTIONAL_DIVIDER;
    g_UsbDeviceAudioSpeaker.audioPllTicksPrev      = 0;
    g_UsbDeviceAudioSpeaker.audioPllTicksDiff      = 0;
    g_UsbDeviceAudioSpeaker.audioPllTicksEma       = AUDIO_PLL_USB1_SOF_INTERVAL_COUNT;
    g_UsbDeviceAudioSpeaker.audioPllTickEmaFrac    = 0;
    g_UsbDeviceAudioSpeaker.audioPllStep           = AUDIO_PLL_FRACTIONAL_CHANGE_STEP;
}

/*!
 * @brief USB device callback function.
 *
 * This function handles the usb device specific requests.
 *
 * @param handle		  The USB device handle.
 * @param event 		  The USB device event type.
 * @param param 		  The parameter of the device specific request.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceCallback(usb_device_handle handle, uint32_t event, void *param)
{
    usb_status_t error = kStatus_USB_Error;
    uint8_t *temp8     = (uint8_t *)param;
    uint8_t count      = 0U;

    switch (event)
    {
        case kUSB_DeviceEventBusReset:
        {
            for (count = 0U; count < USB_AUDIO_SPEAKER_INTERFACE_COUNT; count++)
            {
                g_UsbDeviceAudioSpeaker.currentInterfaceAlternateSetting[count] = 0U;
            }
            g_UsbDeviceAudioSpeaker.attach                                 = 0U;
            g_UsbDeviceAudioSpeaker.currentConfiguration                   = 0U;
            g_UsbDeviceAudioSpeaker.currentStreamInterfaceAlternateSetting = 0U;
            error                                                          = kStatus_USB_Success;
            USB_DeviceControlPipeInit(g_UsbDeviceAudioSpeaker.deviceHandle);
#if (defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)) || \
    (defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U))
            /* Get USB speed to configure the device, including max packet size and interval of the endpoints. */
            if (kStatus_USB_Success == USB_DeviceGetStatus(g_UsbDeviceAudioSpeaker.deviceHandle, kUSB_DeviceStatusSpeed,
                                                           &g_UsbDeviceAudioSpeaker.speed))
            {
                USB_DeviceSetSpeed(g_UsbDeviceAudioSpeaker.speed);
            }

            if (USB_SPEED_HIGH == g_UsbDeviceAudioSpeaker.speed)
            {
                g_UsbDeviceAudioSpeaker.currentStreamOutMaxPacketSize =
                    (HS_ISO_OUT_ENDP_PACKET_SIZE + AUDIO_FORMAT_CHANNELS * AUDIO_FORMAT_SIZE);
            }
#endif
        }
        break;
        case kUSB_DeviceEventSetConfiguration:
            if (0U == (*temp8))
            {
                g_UsbDeviceAudioSpeaker.attach                                 = 0U;
                g_UsbDeviceAudioSpeaker.currentConfiguration                   = 0U;
                g_UsbDeviceAudioSpeaker.currentStreamInterfaceAlternateSetting = 0U;
            }
            else if (USB_AUDIO_SPEAKER_CONFIGURE_INDEX == (*temp8))
            {
                g_UsbDeviceAudioSpeaker.attach                                 = 1U;
                g_UsbDeviceAudioSpeaker.currentConfiguration                   = *temp8;
                g_UsbDeviceAudioSpeaker.currentStreamInterfaceAlternateSetting = 0U;
            }
            else
            {
                error = kStatus_USB_InvalidRequest;
            }
            break;
        case kUSB_DeviceEventSetInterface:
            if (g_UsbDeviceAudioSpeaker.attach)
            {
                uint8_t interface        = (*temp8) & 0xFF;
                uint8_t alternateSetting = g_UsbDeviceInterface[interface];

                if (g_UsbDeviceAudioSpeaker.currentStreamInterfaceAlternateSetting != alternateSetting)
                {
                    if (g_UsbDeviceAudioSpeaker.currentStreamInterfaceAlternateSetting)
                    {
                        USB_DeviceDeinitEndpoint(g_UsbDeviceAudioSpeaker.deviceHandle,
                                                 USB_AUDIO_SPEAKER_STREAM_ENDPOINT | (USB_OUT << 7U));
                    }

                    else
                    {
                        usb_device_endpoint_init_struct_t epInitStruct;
                        usb_device_endpoint_callback_struct_t epCallback;

                        epCallback.callbackFn    = USB_DeviceAudioIsoOut;
                        epCallback.callbackParam = handle;

                        epInitStruct.zlt             = 0U;
                        epInitStruct.transferType    = USB_ENDPOINT_ISOCHRONOUS;
                        epInitStruct.endpointAddress = USB_AUDIO_SPEAKER_STREAM_ENDPOINT |
                                                       (USB_OUT << USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT);
                        if (USB_SPEED_HIGH == g_UsbDeviceAudioSpeaker.speed)
                        {
                            epInitStruct.maxPacketSize =
                                (HS_ISO_OUT_ENDP_PACKET_SIZE + AUDIO_FORMAT_CHANNELS * AUDIO_FORMAT_SIZE);
                            epInitStruct.interval = HS_ISO_OUT_ENDP_INTERVAL;
                        }
                        else
                        {
                            epInitStruct.maxPacketSize =
                                (FS_ISO_OUT_ENDP_PACKET_SIZE + AUDIO_FORMAT_CHANNELS * AUDIO_FORMAT_SIZE);
                            epInitStruct.interval = FS_ISO_OUT_ENDP_INTERVAL;
                        }

                        USB_DeviceInitEndpoint(g_UsbDeviceAudioSpeaker.deviceHandle, &epInitStruct, &epCallback);

                        error = USB_DeviceRecvRequest(g_UsbDeviceAudioSpeaker.deviceHandle,
                                                      USB_AUDIO_SPEAKER_STREAM_ENDPOINT, &audioPlayPacket[0],
                                                      g_UsbDeviceAudioSpeaker.currentStreamOutMaxPacketSize);
                    }
                    g_UsbDeviceAudioSpeaker.currentStreamInterfaceAlternateSetting = alternateSetting;
                }
            }
            break;
        default:
            break;
    }

    return error;
}

#if (defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U))
void SCTIMER_SOF_TOGGLE_HANDLER()
{
    uint32_t currentSctCap = 0, pllCountPeriod = 0, pll_change = 0;
    uint32_t usedSpace      = 0;
    static int32_t pllCount = 0, pllDiff = 0;
    static int32_t err, abs_err;
    if (SCTIMER_GetStatusFlags(SCT0) & (1 << eventCounterU))
    {
        /* Clear interrupt flag.*/
        SCTIMER_ClearStatusFlags(SCT0, (1 << eventCounterU));
    }

    if (g_UsbDeviceAudioSpeaker.speakerIntervalCount != 100)
    {
        g_UsbDeviceAudioSpeaker.speakerIntervalCount++;
        return;
    }
    g_UsbDeviceAudioSpeaker.speakerIntervalCount = 1;
    currentSctCap                                = SCT0->CAP[0];
    pllCountPeriod                               = currentSctCap - g_UsbDeviceAudioSpeaker.audioPllTicksPrev;
    g_UsbDeviceAudioSpeaker.audioPllTicksPrev    = currentSctCap;
    pllCount                                     = pllCountPeriod;
    if (g_UsbDeviceAudioSpeaker.attach)
    {
        if (abs(pllCount - AUDIO_PLL_USB1_SOF_INTERVAL_COUNT) < (AUDIO_PLL_USB1_SOF_INTERVAL_COUNT >> 7))
        {
            pllDiff = pllCount - g_UsbDeviceAudioSpeaker.audioPllTicksEma;
            g_UsbDeviceAudioSpeaker.audioPllTickEmaFrac += (pllDiff % 8);
            g_UsbDeviceAudioSpeaker.audioPllTicksEma += (pllDiff / 8) + g_UsbDeviceAudioSpeaker.audioPllTickEmaFrac / 8;
            g_UsbDeviceAudioSpeaker.audioPllTickEmaFrac = (g_UsbDeviceAudioSpeaker.audioPllTickEmaFrac % 8);

            err     = g_UsbDeviceAudioSpeaker.audioPllTicksEma - AUDIO_PLL_USB1_SOF_INTERVAL_COUNT;
            abs_err = abs(err);
            if (abs_err > g_UsbDeviceAudioSpeaker.audioPllStep)
            {
                if (err > 0)
                {
                    g_UsbDeviceAudioSpeaker.curAudioPllFrac -= abs_err / g_UsbDeviceAudioSpeaker.audioPllStep;
                }
                else
                {
                    g_UsbDeviceAudioSpeaker.curAudioPllFrac += abs_err / g_UsbDeviceAudioSpeaker.audioPllStep;
                }
                pll_change = 1;
            }
            if (g_UsbDeviceAudioSpeaker.startPlayHalfFull)
            {
                usedSpace = USB_AudioSpeakerBufferSpaceUsed();
                if (usedSpace > ((AUDIO_SPEAKER_DATA_WHOLE_BUFFER_LENGTH / 2 + 1) * AUDIO_PLAY_TRANSFER_SIZE))
                {
                    g_UsbDeviceAudioSpeaker.curAudioPllFrac++;
                    pll_change = 1;
                }
                else if (usedSpace < ((AUDIO_SPEAKER_DATA_WHOLE_BUFFER_LENGTH / 2 - 1) * AUDIO_PLAY_TRANSFER_SIZE))
                {
                    g_UsbDeviceAudioSpeaker.curAudioPllFrac--;
                    pll_change = 1;
                }
            }
            if (pll_change)
            {
                SYSCON->AUDPLLFRAC = g_UsbDeviceAudioSpeaker.curAudioPllFrac;
                SYSCON->AUDPLLFRAC = g_UsbDeviceAudioSpeaker.curAudioPllFrac | (1U << SYSCON_AUDPLLFRAC_REQ_SHIFT);
            }
        }
    }
}

void SCTIMER_CaptureInit(void)
{
    INPUTMUX->SCT0_INMUX[eventCounterU] = 0x10U; /* 0x10U for USB1 and 0x0FU for USB0. */
    SCTIMER_GetDefaultConfig(&sctimerInfo);

    /* Switch to 16-bit mode */
    sctimerInfo.clockMode   = kSCTIMER_Input_ClockMode;
    sctimerInfo.clockSelect = kSCTIMER_Clock_On_Rise_Input_7;

    /* Initialize SCTimer module */
    SCTIMER_Init(SCT0, &sctimerInfo);

    if (SCTIMER_SetupCaptureAction(SCT0, kSCTIMER_Counter_U, &captureRegisterNumber, eventCounterU) == kStatus_Fail)
    {
        usb_echo("SCT Setup Capture failed!\r\n");
    }
    SCT0->EV[0].STATE = 0x1;
    SCT0->EV[0].CTRL  = (0x01 << 10) | (0x2 << 12);

    /* Enable interrupt flag for event associated with out 4, we use the interrupt to update dutycycle */
    SCTIMER_EnableInterrupts(SCT0, (1 << eventCounterU));

    /* Receive notification when event is triggered */
    SCTIMER_SetCallback(SCT0, SCTIMER_SOF_TOGGLE_HANDLER, eventCounterU);

    /* Enable at the NVIC */
    EnableIRQ(SCT0_IRQn);

    /* Start the L counter */
    SCTIMER_StartTimer(SCT0, kSCTIMER_Counter_U);
}
#endif

/*!
 * @brief Application initialization function.
 *
 * This function initializes the application.
 *
 * @return None.
 */

void APPInit(void)
{
    USB_DeviceClockInit();
#if (defined(FSL_FEATURE_SOC_SYSMPU_COUNT) && (FSL_FEATURE_SOC_SYSMPU_COUNT > 0U))
    SYSMPU_Enable(SYSMPU, 0);
#endif /* FSL_FEATURE_SOC_SYSMPU_COUNT */
    Init_Board_Audio();

    if (kStatus_USB_Success != USB_DeviceInit(CONTROLLER_ID, USB_DeviceCallback, &g_UsbDeviceAudioSpeaker.deviceHandle))
    {
        usb_echo("USB device failed\r\n");
        return;
    }
    else
    {
        usb_echo("USB device audio speaker demo\r\n");
    }

    /* Install isr, set priority, and enable IRQ. */
    USB_DeviceIsrEnable();

    /*Add one delay here to make the DP pull down long enough to allow host to detect the previous disconnection.*/
    SDK_DelayAtLeastUs(5000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    USB_DeviceRun(g_UsbDeviceAudioSpeaker.deviceHandle);

#if (defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U))
    SCTIMER_CaptureInit();
#endif
}

void USB_AudioCodecTask(void)
{
    if (g_UsbDeviceAudioSpeaker.codecTask & MUTE_CODEC_TASK)
    {
#if (USB_DEVICE_CONFIG_AUDIO_CLASS_2_0)
        usb_echo("Set Cur Mute : %x\r\n", g_UsbDeviceAudioSpeaker.curMute20);
#else
        usb_echo("Set Cur Mute : %x\r\n", g_UsbDeviceAudioSpeaker.curMute);
#endif
        g_UsbDeviceAudioSpeaker.codecTask &= ~MUTE_CODEC_TASK;
    }
    if (g_UsbDeviceAudioSpeaker.codecTask & UNMUTE_CODEC_TASK)
    {
#if (USB_DEVICE_CONFIG_AUDIO_CLASS_2_0)
        usb_echo("Set Cur Mute : %x\r\n", g_UsbDeviceAudioSpeaker.curMute20);
#else
        usb_echo("Set Cur Mute : %x\r\n", g_UsbDeviceAudioSpeaker.curMute);
#endif
        g_UsbDeviceAudioSpeaker.codecTask &= ~UNMUTE_CODEC_TASK;
    }
    if (g_UsbDeviceAudioSpeaker.codecTask & VOLUME_CHANGE_TASK)
    {
#if (USB_DEVICE_CONFIG_AUDIO_CLASS_2_0)
        usb_echo("Set Cur Volume : %x\r\n",
                 (uint16_t)(g_UsbDeviceAudioSpeaker.curVolume20[1] << 8U) | g_UsbDeviceAudioSpeaker.curVolume20[0]);
#else
        usb_echo("Set Cur Volume : %x\r\n",
                 (uint16_t)(g_UsbDeviceAudioSpeaker.curVolume[1] << 8U) | g_UsbDeviceAudioSpeaker.curVolume[0]);
#endif
        g_UsbDeviceAudioSpeaker.codecTask &= ~VOLUME_CHANGE_TASK;
    }
}

void USB_AudioSpeakerResetTask(void)
{
    if (g_UsbDeviceAudioSpeaker.speakerDetachOrNoInput)
    {
        USB_DeviceAudioSpeakerStatusReset();
    }
}
/*!
 * @brief Application task function.
 *
 * This function runs the task for application.
 *
 * @return None.
 */
#if defined(__CC_ARM) || (defined(__ARMCC_VERSION)) || defined(__GNUC__)
int main(void)
#else
void main(void)
#endif
{
    BOARD_InitHardware();

    APPInit();

    while (1)
    {
        USB_AudioCodecTask();

        USB_AudioSpeakerResetTask();

#if USB_DEVICE_CONFIG_USE_TASK
        USB_DeviceTaskFn(g_UsbDeviceAudioSpeaker.deviceHandle);
#endif
    }
}
