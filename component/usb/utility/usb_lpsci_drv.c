/*
 * Copyright (c) 2015 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 - 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_lpsci.h"
#include "fsl_common.h"
#include "board.h"
#include "usb_serial_port.h"

typedef struct _lpsci_status_struct
{
    lpsci_handle_t lpsciHandle;
    uint8_t isUsed;
} lpsci_status_struct_t;

lpsci_status_struct_t lpsciStatus[USB_SERIAL_PORT_INSTANCE_COUNT];

/*!
 * @brief lpsci callback function.
 *
 *This callback will be called if the lpsci has get specific num(USB_HOST_CDC_LPSCI_RX_MAX_LEN) char.
 *
 * @param instance           instancehandle.
 * @param lpsciState           callback event code, please reference to enumeration host_event_t.
 *
 */
void LPSCI_SeralPortTransferCallback(UART0_Type *base, lpsci_handle_t *handle, status_t status, void *userData)
{
    usb_serial_port_handle_t *serialPortHandle = (usb_serial_port_handle_t *)userData;
    if (NULL != serialPortHandle->callback.callbackFunction)
    {
        serialPortHandle->callback.callbackFunction(handle, (((uint32_t)status) % 100) + 1,
                                                    serialPortHandle->callback.callbackParam);
    }
}

usb_serial_port_status_t USB_SerialPortInit(uint8_t instance,
                                            const usb_serial_port_config_t *config,
                                            usb_serial_port_callback_struct_t *callback,
                                            uint32_t sourceClockHz,
                                            usb_serial_port_handle_t *handle)
{
    UART0_Type *lpsci[] = UART0_BASE_PTRS;
    int i;
    lpsci_config_t lpsciConfiguration;

    if (instance >= (sizeof(lpsci) / sizeof(UART0_Type *)))
    {
        return kStatus_USB_SERIAL_PORT_InvalidParameter;
    }

    if ((NULL == handle) || (NULL == callback))
    {
        return kStatus_USB_SERIAL_PORT_InvalidParameter;
    }

    handle->serialPortHandle = NULL;

    for (i = 0; i < USB_SERIAL_PORT_INSTANCE_COUNT; i++)
    {
        if (0 == lpsciStatus[i].isUsed)
        {
            handle->serialPortHandle = &lpsciStatus[i];
            lpsciStatus[i].isUsed    = 1U;
            break;
        }
    }
    if (NULL == handle->serialPortHandle)
    {
        return kStatus_USB_SERIAL_PORT_Busy;
    }

    handle->callback.callbackFunction = callback->callbackFunction;
    handle->callback.callbackParam    = callback->callbackParam;

    LPSCI_GetDefaultConfig(&lpsciConfiguration);

    lpsciConfiguration.baudRate_Bps = config->baudRate_Bps;
    lpsciConfiguration.enableTx     = config->enableTx;
    lpsciConfiguration.enableRx     = config->enableRx;
    LPSCI_Init(lpsci[instance], &lpsciConfiguration, sourceClockHz);

    handle->baseReg  = lpsci[instance];
    handle->instance = instance;

    LPSCI_TransferCreateHandle(lpsci[instance],
                               (lpsci_handle_t *)&((lpsci_status_struct_t *)handle->serialPortHandle)->lpsciHandle,
                               LPSCI_SeralPortTransferCallback, handle);

    return kStatus_USB_SERIAL_PORT_Success;
}

usb_serial_port_status_t USB_SerialPortRecv(usb_serial_port_handle_t *handle,
                                            usb_serial_port_xfer_t *xfer,
                                            size_t *receivedBytes)
{
    return (usb_serial_port_status_t)(
        LPSCI_TransferReceiveNonBlocking(
            handle->baseReg, (lpsci_handle_t *)&((lpsci_status_struct_t *)handle->serialPortHandle)->lpsciHandle,
            (lpsci_transfer_t *)xfer, receivedBytes) %
            100 +
        1);
}

usb_serial_port_status_t USB_SerialPortSend(usb_serial_port_handle_t *handle, usb_serial_port_xfer_t *xfer)
{
    return (usb_serial_port_status_t)(
        LPSCI_TransferSendNonBlocking(
            handle->baseReg, (lpsci_handle_t *)&((lpsci_status_struct_t *)handle->serialPortHandle)->lpsciHandle,
            (lpsci_transfer_t *)xfer) %
            100 +
        1);
}

usb_serial_port_status_t USB_SerialPortDeinit(usb_serial_port_handle_t *handle)
{
    LPSCI_Deinit(handle->baseReg);
    return kStatus_USB_SERIAL_PORT_Success;
}

void USB_SerialPortIRQHandler(usb_serial_port_handle_t *handle)
{
    LPSCI_TransferHandleIRQ(handle->baseReg,
                            (lpsci_handle_t *)&((lpsci_status_struct_t *)handle->serialPortHandle)->lpsciHandle);
/* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
  exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}
