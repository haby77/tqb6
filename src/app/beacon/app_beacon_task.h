#ifndef __APP_BEACON_TASK_H__
#define __APP_BEACON_TASK_H__

#if BLE_BEACON_SERVER

#include "app_beacon.h"


// Battery Service Server environment variable
struct app_beacon_env_tag
{
    // Profile role state: enabled/disabled
    uint8_t enabled;
    uint8_t features;
    // Connection handle
    uint16_t conhdl;
    //MeasuredPower 
    uint8_t measured_power_val;
    
//     // Battery Level Notification Configurations
//     uint16_t batt_level_ntf_cfg[BASS_NB_BAS_INSTANCES_MAX];
//     // Last Battery Level
//     uint8_t old_batt_lvl[BASS_NB_BAS_INSTANCES_MAX];
//     // Current Battery Level
//     uint8_t current_batt_lvl[BASS_NB_BAS_INSTANCES_MAX];
//     // Battery Level Characteristic Presentation Format - Should not change during connection
//     struct prf_char_pres_fmt batt_level_pres_format[BASS_NB_BAS_INSTANCES_MAX];
//     // Current Time Notification flow control
//     bool ntf_sending;
};

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */
extern struct app_beacon_env_tag *app_beacon_env;

extern int app_beacon_create_db_cfm_handler(ke_msg_id_t const msgid,
                                   struct beacon_create_db_cfm *param,
                                   ke_task_id_t const dest_id,
                                   ke_task_id_t const src_id);


#endif //BLE_BEACON_SERVER
#endif

