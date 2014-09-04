/**
 ****************************************************************************************
 *
 * @file beacon.c
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


/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "app_config.h"

#if (BLE_BEACON_SERVER)

#include "gatt_task.h"
#include "atts_util.h"
#include "beacon.h"
#include "beacon_task.h"

/*
 * BEACON ATTRIBUTES DEFINITION
 ****************************************************************************************
 */

/// Full BEACON Database Description - Used to add attributes into the database
const struct atts_desc beacon_att_db[BEACON_IDX_NB] =
{
    // iBeacon Service Declaration
    [BEACON_IDX_SVC]                  =   {ATT_DECL_PRIMARY_SERVICE, PERM(RD, ENABLE), sizeof(beacon_svc),
                                        sizeof(beacon_svc), (uint8_t *)&beacon_svc},

    // iBeacon Level Characteristic Declaration
    [BEACON_IDX_LVL_CHAR]             =   {ATT_DECL_CHARACTERISTIC, PERM(RD, ENABLE), sizeof(beacon_level_char),
                                        sizeof(beacon_level_char), (uint8_t *)&beacon_level_char},
    // iBeacon Level Characteristic Value
    [BEACON_IDX_LVL_VAL]              =   {BEACON_DATA_UUID, PERM(RD, ENABLE)| PERM(WR, ENABLE), sizeof(uint8_t),
                                        0, NULL},
    // iBeacon User Declaration
    [BEACON_VERSION_DATA_USER_DESP]   =   {ATT_DESC_CHAR_USER_DESCRIPTION, PERM(RD, ENABLE), sizeof("MeasuredPower"),
                                         sizeof("MeasuredPower"), "MeasuredPower"},
};

/// iBeacon Service
const atts_svc_desc_t beacon_svc = BEACON_SERVICE_UUID;

/// iBeacon Level characteristic
const struct atts_char_desc beacon_level_char = ATTS_CHAR(ATT_CHAR_PROP_RD|ATT_CHAR_PROP_WR,
                                                            0,
                                                            BEACON_DATA_UUID);


/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

struct beacon_env_tag beacon_env;


/*
 * GLOBAL FUNCTIONS DEFINITIONS
 ****************************************************************************************
 */

void beacon_init(void)
{
    // Reset the find me target environment
    memset(&beacon_env, 0, sizeof(beacon_env));
	
	// Register beacon_env task into kernel
    task_beacon_desc_register();

    // Go to IDLE state
    ke_state_set(TASK_BEACON, BEACON_DISABLED);
}


void beacon_disable(void)
{

    //Send current configuration to APP
    struct beacon_disable_ind * cfg = KE_MSG_ALLOC(BEACON_DISABLE_IND,
                                                 beacon_env.con_info.appid, TASK_BEACON,
                                                 beacon_disable_ind);
    memcpy(&cfg->conhdl, &beacon_env.con_info.conhdl, sizeof(uint16_t));

//     //Notifications Configuration
// 		cfg->ntf_en = beacon_env.features;

    ke_msg_send(cfg);

    //Go to idle state
    ke_state_set(TASK_BEACON, BEACON_IDLE);
}

#endif


