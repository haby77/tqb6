/**
 ****************************************************************************************
 *
 * @file beacon_task.h
 *
 * @brief BEACON Server Implementation.
 *
 * Copyright (C) RivieraWaves 2014
 *
 * Author : wbj@t-chip.com.cn
 *
 * Date : 2014-09-02
 *
 ****************************************************************************************
 */


#ifndef __BEACON_TASK_H__
#define __BEACON_TASK_H__

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#if (BLE_BEACON_SERVER)

#include "ke_task.h"

///Maximum number of BEACON profile task instances
#define BEACON_IDX_MAX     0x01

/// Possible states of the BEACON task
enum
{
    /// Disabled state
    BEACON_DISABLED,
    /// Idle state
	BEACON_IDLE,
    /// Connected state
	BEACON_CONNECTED,

    /// Number of defined states.
	BEACON_STATE_MAX
};


/// Messages for BEACON
enum
{
    ///Start the Beacon Profile Server - at connection
    BEACON_ENABLE_REQ = KE_FIRST_MSG(TASK_BEACON),
    ///Disable confirmation with configuration to save after profile disabled
    BEACON_DISABLE_IND,
    

    ///Error indication to Host
    BEACON_ERROR_IND,
//     ///Send value from APP
//     QPPS_DATA_SEND_REQ,
//     ///Send data confirm to APP if correctly sent.
//     QPPS_DATA_SEND_CFM,
//     ///Inform APP of new configuration value
//     QPPS_CFG_INDNTF_IND,
//     ///Client value send to APP
//     QPPS_DAVA_VAL_IND,
    
//     //VALUE UPDATE
//     BEACON_VAL_UPD_REQ,

    ///Add BEACON into the database
    BEACON_CREATE_DB_REQ,
    ///Inform APP about DB creation status
    BEACON_CREATE_DB_CFM,
};


/*
 * APIs Structures
 ****************************************************************************************
 */

///Parameters of the @ref BEACON_CREATE_DB_CFM message
struct beacon_create_db_cfm
{
    ///Status
    uint8_t status;
};


/// Parameters of the @ref BEACON_CREATE_DB_REQ message
struct beacon_create_db_req
{
//     /// Number of BAS to add
//     uint8_t bas_nb;
    /// Features of each BAS instance
    uint8_t features[1];
};

///Parameters of the @ref BEACON_DISABLE_IND message
struct beacon_disable_ind
{
    uint16_t conhdl;
    /// Notification configuration
    //uint16_t ntf_en;
};

/// Parameters of the @ref BEACON_ENABLE_REQ message
struct beacon_enable_req
{
    ///Connection handle
    uint16_t conhdl;
    /// Security Level
    uint8_t sec_lvl;
    /// Type of connection
    uint8_t con_type;
    
    //MeasuredPower 
    uint8_t measured_power_val;

//     /// Battery Level Notification Configurations
//     uint16_t batt_level_ntf_cfg[BASS_NB_BAS_INSTANCES_MAX];
//     /// Last Battery Level
//     uint8_t old_batt_lvl[BASS_NB_BAS_INSTANCES_MAX];
//     /// Current Battery Level
//     uint8_t current_batt_lvl[BASS_NB_BAS_INSTANCES_MAX];

//     /// Battery Level Characteristic Presentation Format - Should not change during connection
//     struct prf_char_pres_fmt batt_level_pres_format[BASS_NB_BAS_INSTANCES_MAX];
};

// ///Parameters of the @ref BEACON_VAL_UPD_REQ message
// struct beacon_val_upd_req
// {
//     /// Connection handle
//     uint16_t conhdl;
//     /// BEACON Mesured Power
//     uint8_t measured_power_val;
// };


/*
 * TASK DESCRIPTOR DECLARATIONS
 ****************************************************************************************
 */

extern void task_beacon_desc_register(void);

#endif //BLE_BEACON_SERVER

#endif
