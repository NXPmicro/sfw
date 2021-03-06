/*
 * Copyright 2016 - 2017 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "usb_pd_config.h"
#include "usb_pd.h"
#include "usb_pd_i2c.h"
#include "pd_app.h"
#include "fsl_gpio.h"
#include "pd_power_interface.h"
#include "pd_board_config.h"
#include "pd_power_nx20p3483.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define USBPD_SINK_OVP_VALUE_SHILED2 23000

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

typedef struct _pd_power_control_instance
{
    uint32_t sourceVbusVoltage;
    void *pdHandle;
} pd_power_control_instance_t;

static pd_power_control_instance_t s_PowerControlInstances[PD_CONFIG_MAX_PORT];

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

void PD_PowerBoardControlInit(uint8_t port, pd_handle pdHandle)
{
    pd_power_control_instance_t *powerControl = &s_PowerControlInstances[port - 1];
    powerControl->pdHandle                    = pdHandle;

    if ((port == 0) || (port > PD_CONFIG_MAX_PORT))
    {
        return;
    }

    {
        uint32_t getInfo = 0;
        PD_Control(pdHandle, PD_CONTROL_GET_TYPEC_CURRENT_VALUE, &getInfo);
        switch (getInfo)
        {
            case kCurrent_StdUSB:
                getInfo = 1000;
                break;
            case kCurrent_1A5:
                getInfo = 1800;
                break;
            case kCurrent_3A:
                getInfo = 3400;
                break;
            default:
                getInfo = 1000;
                break;
        }
        PD_NX20PInit(port, getInfo, USBPD_SINK_OVP_VALUE_SHILED2);
    }
}

void PD_PowerBoardControlDeinit(uint8_t port)
{
    pd_power_control_instance_t *powerControl = &s_PowerControlInstances[port - 1];

    if ((port == 0) || (port > PD_CONFIG_MAX_PORT))
    {
        return;
    }
    powerControl->pdHandle = NULL;
}

pd_status_t PD_PowerBoardReset(uint8_t port)
{
    pd_ptn5110_ctrl_pin_t phyPowerPinCtrl;
    pd_power_control_instance_t *powerControl = &s_PowerControlInstances[port - 1];

    if ((port == 0) || (port > PD_CONFIG_MAX_PORT))
    {
        return kStatus_PD_Error;
    }

    phyPowerPinCtrl.enSRC  = 0;
    phyPowerPinCtrl.enSNK1 = 0;
    PD_Control(powerControl->pdHandle, PD_CONTROL_PHY_POWER_PIN, &phyPowerPinCtrl);

    return kStatus_PD_Success;
}

/***************source need implement follow vbus power related functions***************/

/***************sink need implement follow vbus power related functions***************/

pd_status_t PD_PowerBoardSinkEnableVbusPower(uint8_t port, pd_vbus_power_t vbusPower)
{
    pd_ptn5110_ctrl_pin_t phyPowerPinCtrl;
    uint32_t voltage;
    pd_power_control_instance_t *powerControl = &s_PowerControlInstances[port - 1];

    if ((port == 0) || (port > PD_CONFIG_MAX_PORT))
    {
        return kStatus_PD_Error;
    }

    phyPowerPinCtrl.enSRC  = 0;
    phyPowerPinCtrl.enSNK1 = 1;
    PD_Control(powerControl->pdHandle, PD_CONTROL_PHY_POWER_PIN, &phyPowerPinCtrl);
    voltage = (vbusPower.minVoltage * 50) | ((vbusPower.maxVoltage * 50) << 16);
    PD_Control(powerControl->pdHandle, PD_CONTROL_INFORM_VBUS_VOLTAGE_RANGE, &voltage);
    return kStatus_PD_Success;
}

/***************if support vconn, need implement the follow related functions***************/
pd_status_t PD_PowerBoardControlVconn(uint8_t port, uint8_t on)
{
    uint8_t controlVal;
    pd_power_control_instance_t *powerControl = &s_PowerControlInstances[port - 1];

    if ((port == 0) || (port > PD_CONFIG_MAX_PORT))
    {
        return kStatus_PD_Error;
    }
    controlVal = (on ? 1 : 0);
    PD_Control(powerControl->pdHandle, PD_CONTROL_VCONN, &controlVal);
    return kStatus_PD_Success;
}
