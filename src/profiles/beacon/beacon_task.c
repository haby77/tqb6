/**
 ****************************************************************************************
 *
 * @file beacon_task.c
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

#include "gap.h"
#include "gatt_task.h"
#include "atts_util.h"

#include "beacon.h"
#include "beacon_task.h"

#include "prf_utils.h"
#include "app_printf.h"

#include "nvds.h"


/**
 ****************************************************************************************
 * @brief Handles reception of the @ref BEACON_CREATE_DB_REQ message.
 * The handler adds BAS into the database using value of the features param.
 * @param[in] msgid Id of the message received (probably unused).
 * @param[in] param Pointer to the parameters of the message.
 * @param[in] dest_id ID of the receiving task instance (probably unused).
 * @param[in] src_id ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int beacon_create_db_req_handler(ke_msg_id_t const msgid,
                                      struct beacon_create_db_req const *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id)
{
    // Service content flag
    uint8_t cfg_flag = BEACON_CFG_FLAG_MANDATORY_MASK;
    // Status
    uint8_t status = PRF_ERR_OK;
//     // Counter
//     uint8_t i;
//     // Battery Level characteristic value permissions
//     uint16_t perm;
//     // Battery Level characteristic value properties
//     uint8_t prop;

    // Save profile id
    beacon_env.con_info.prf_id = TASK_BEACON;
    //beacon_env.features        = param->features;
    status = atts_svc_create_db(&beacon_env.shdl, (uint8_t *)&cfg_flag, BEACON_IDX_NB, NULL,dest_id, &beacon_att_db[0]);

    //Disable service
    status = attsdb_svc_set_permission(beacon_env.shdl, PERM(SVC, DISABLE));

    
    //Go to Idle State
    if (status == ATT_ERR_NO_ERROR)
    {
        //If we are here, database has been fulfilled with success, go to idle test
        ke_state_set(TASK_BEACON, BEACON_IDLE);
    }
    
    //Send response to application
    struct beacon_create_db_cfm * cfm = KE_MSG_ALLOC(BEACON_CREATE_DB_CFM, beacon_env.con_info.appid,
                                                   TASK_BEACON, beacon_create_db_cfm);
    cfm->status = status;
    ke_msg_send(cfm);
    
//     // Check number of BAS instances
//     if (param->bas_nb <= BASS_NB_BAS_INSTANCES_MAX)
//     {
//         // Save number of BAS
//         bass_env.bas_nb = param->bas_nb;

//         for (i = 0; ((i < param->bas_nb) && (status == PRF_ERR_OK)); i++)
//         {
//             // Save database configuration
//             bass_env.features[i] = param->features[i];

//             // Check if notifications are supported
//             if (bass_env.features[i] == BAS_BATT_LVL_NTF_SUP)
//             {
//                 cfg_flag |= BAS_CFG_FLAG_NTF_SUP_MASK;
//             }

//             // Check if multiple instances
//             if (bass_env.bas_nb > 1)
//             {
//                 cfg_flag |= BAS_CFG_FLAG_MTP_BAS_MASK;
//             }

//             //Create BAS in the DB
//             status = atts_svc_create_db(&bass_env.shdl[i], (uint8_t *)&cfg_flag, BAS_IDX_NB, NULL,
//                                         dest_id, &bas_att_db[0]);

//             //Disable the service and set optional features
//             if (status == PRF_ERR_OK)
//             {
//                 //Disable service
//                 status = attsdb_svc_set_permission(bass_env.shdl[i], PERM(SVC, DISABLE));

//                 //Set optional properties and permissions
//                 if (bass_env.features[i] == BAS_BATT_LVL_NTF_SUP)
//                 {
//                     prop = ATT_CHAR_PROP_RD | ATT_CHAR_PROP_NTF;
//                     perm = PERM(RD, ENABLE) | PERM(NTF, ENABLE);

//                     attsdb_att_partial_value_update(bass_env.shdl[i] + BAS_IDX_BATT_LVL_CHAR, 0, 1, &prop);
//                     attsdb_att_set_permission(bass_env.shdl[i] + BAS_IDX_BATT_LVL_VAL, perm);
//                 }
//             }

//             // Reset configuration flag
//             cfg_flag = BAS_CFG_FLAG_MANDATORY_MASK;
//         }

//         if (status == PRF_ERR_OK)
//         {
//             //If we are here, database has been fulfilled with success, go to idle state
//             ke_state_set(TASK_BASS, BASS_IDLE);
//         }
//     }
//     else
//     {
//         status = PRF_ERR_INVALID_PARAM;
//     }

//     // Send confirmation to application
//     struct bass_create_db_cfm * cfm = KE_MSG_ALLOC(BASS_CREATE_DB_CFM, src_id, TASK_BASS,
//                                                    bass_create_db_cfm);
//     cfm->status = status;
//     ke_msg_send(cfm);

     return (KE_MSG_CONSUMED);
}

//为编辑
/**
 ****************************************************************************************
 * @brief Handles reception of the @ref BAPS_ENABLE_REQ message.
 * The handler enables the Battery 'Profile' Server Role.
 * @param[in] msgid Id of the message received (probably unused).
 * @param[in] param Pointer to the parameters of the message.
 * @param[in] dest_id ID of the receiving task instance (probably unused).
 * @param[in] src_id ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int beacon_enable_req_handler(ke_msg_id_t const msgid,
                                   struct beacon_enable_req const *param,
                                   ke_task_id_t const dest_id,
                                   ke_task_id_t const src_id)
{
    // Save the application task id
    beacon_env.con_info.appid = src_id;
    // Save the connection handle associated to the profile
    beacon_env.conhdl = param->conhdl;
    
    // Check if the provided connection exist
    if (gap_get_rec_idx(param->conhdl) == GAP_INVALID_CONIDX)
    {
        // ??????????????????
        // The connection doesn't exist, request disallowed
        prf_server_error_ind_send((prf_env_struct *)&beacon_env, PRF_ERR_REQ_DISALLOWED,
                                  BEACON_ERROR_IND, BEACON_ENABLE_REQ);
    }
    else
    {
        attsdb_att_set_value(beacon_env.shdl + BEACON_IDX_LVL_VAL, sizeof(uint8_t),
                                  (uint8_t *)&param->measured_power_val);
        attsdb_svc_set_permission(beacon_env.shdl, param->sec_lvl);
    
        // Go to connected state
        ke_state_set(TASK_BEACON, BEACON_CONNECTED);
    }
    
    
    
//     // Attribute offset - Used to retrieve Char. Pres. Format Descriptor handle
//     uint8_t offset;
//     // Packed Char. Presentation Format value
//     uint8_t packed_char_pres[PRF_CHAR_PRES_FMT_SIZE];
//     // Counter
//     uint8_t i;
//     // Notification Configuration
//     uint16_t ntf_cfg = 0;

//     // Save the application task id
//     bass_env.con_info.appid = src_id;
//     // Save the connection handle associated to the profile
//     bass_env.con_info.conhdl = param->conhdl;

//     // Check if the provided connection exist
//     if (gap_get_rec_idx(param->conhdl) == GAP_INVALID_CONIDX)
//     {
//         // The connection doesn't exist, request disallowed
//         prf_server_error_ind_send((prf_env_struct *)&bass_env, PRF_ERR_REQ_DISALLOWED,
//                                   BASS_ERROR_IND, BASS_ENABLE_REQ);
//     }
//     else
//     {
//         // For each BAS instance
//         for (i = 0; i < bass_env.bas_nb; i++)
//         {
//             // Reset Offset value
//             offset = BAS_IDX_BATT_LVL_NTF_CFG;

//             // Update Battery Level value in DB
//             attsdb_att_set_value(bass_env.shdl[i] + BAS_IDX_BATT_LVL_VAL, sizeof(uint8_t),
//                                  (uint8_t *)&param->current_batt_lvl[i]);

//             // Check if BAS supports notifications
//             if (bass_env.features[i] == BAS_BATT_LVL_NTF_SUP)
//             {
//                 // Increment offset (Client Char. Cfg is placed before Char. Pres Fmt. in DB)
//                 offset++;

//                 // If client is a bonded client
//                 if (param->con_type == PRF_CON_NORMAL)
//                 {
//                     ntf_cfg = param->batt_level_ntf_cfg[i];

//                     /*
//                      * The server shall send a notification if value of the Battery Level characteristic
//                      * has changed while the service has been disconnected from a bonded client.
//                      */
//                     if (param->batt_level_ntf_cfg[i] == PRF_CLI_START_NTF)
//                     {
//                         // Conserve information in environment
//                         bass_env.features[i] |= BASS_FLAG_NTF_CFG_BIT;

//                         // Check if old and previous battery level values are different
//                         if (param->old_batt_lvl[i] != param->current_batt_lvl[i])
//                         {
//                             //Notify current battery level value
//                             struct gatt_notify_req *ntf = KE_MSG_ALLOC(GATT_NOTIFY_REQ, TASK_GATT,
//                                                                        TASK_BASS, gatt_notify_req);

//                             ntf->conhdl  = bass_env.con_info.conhdl;
//                             ntf->charhdl = bass_env.shdl[i] + BAS_IDX_BATT_LVL_VAL;

//                             ke_msg_send(ntf);
//                         }
//                     }
//                 }

//                 // Set NTF Cfg value in the DB
//                 attsdb_att_set_value(bass_env.shdl[i] + BAS_IDX_BATT_LVL_NTF_CFG, sizeof(uint16_t),
//                                      (uint8_t *)&ntf_cfg);
//             }

//             // Check if Characteristic Presentation Format descriptor has been added
//             if (bass_env.bas_nb > 1)
//             {
//                 prf_pack_char_pres_fmt(&packed_char_pres[0], &param->batt_level_pres_format[i]);

//                 /*
//                  * Set Characteristic Presentation Format descriptor value
//                  * Not supposed to change during connection
//                  */
//                 attsdb_att_set_value(bass_env.shdl[i] + offset,
//                                      PRF_CHAR_PRES_FMT_SIZE,
//                                      &packed_char_pres[0]);
//             }

//             // Enable Service + Set Security Level
//             attsdb_svc_set_permission(bass_env.shdl[i], param->sec_lvl);
//         }

//         // Go to connected state
//         ke_state_set(TASK_BASS, BASS_CONNECTED);
//     }

    return (KE_MSG_CONSUMED);
}

// /**
//  ****************************************************************************************
//  * @brief Handles reception of the @ref BAPS_BATT_LEVEL_SEND_REQ message.
//  * @param[in] msgid Id of the message received (probably unused).
//  * @param[in] param Pointer to the parameters of the message.
//  * @param[in] dest_id ID of the receiving task instance (probably unused).
//  * @param[in] src_id ID of the sending task instance.
//  * @return If the message was consumed or not.
//  ****************************************************************************************
//  */
// static int beacon_val_upd_req_handler(ke_msg_id_t const msgid,
//                                             struct beacon_val_upd_req const *param,
//                                             ke_task_id_t const dest_id,
//                                             ke_task_id_t const src_id)
// {
//     uint8_t status = PRF_ERR_OK;
//     QPRINTF("beacon_val_upd_req_handler : %d \r\n",param->measured_power_val);
//     // Check provided values
//     if(param->conhdl == beacon_env.con_info.conhdl)
//     {
//         // Update BEACON Level value in DB
//         QPRINTF("2beacon_val_upd_req_handler : %d \r\n",param->measured_power_val);
//         attsdb_att_set_value(beacon_env.shdl + BEACON_IDX_LVL_VAL,
//                              sizeof(uint8_t), (uint8_t *)&param->measured_power_val);
//     }
//     else
//     {
//         status = PRF_ERR_INVALID_PARAM;
//     }

// //     if (status != PRF_ERR_OK)
// //     {
// //         bass_batt_level_upd_cfm_send(status);
// //     }

//     return (KE_MSG_CONSUMED);
// }

static int gatt_write_cmd_ind_handler(ke_msg_id_t const msgid,
                                      struct gatt_write_cmd_ind const *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id)
{
    //uint8_t char_code = PROXR_ERR_CHAR;
    uint8_t status = PRF_APP_ERROR;
    
    if (param->conhdl == beacon_env.con_info.conhdl)
    {
        if (param->handle == beacon_env.shdl + BEACON_IDX_LVL_VAL)
        {

            //Save value in DB
            attsdb_att_set_value(param->handle, sizeof(uint8_t), (uint8_t *)&param->value[0]);
                
#if QN_NVDS_WRITE
            if (NVDS_OK == nvds_put(NVDS_TAG_MEASURED_POWER, sizeof(param->value[0]), (uint8_t *)&param->value[0]))
            {
                status = PRF_ERR_OK;
            }
#else
            status = PRF_ERR_OK;
#endif
            //Response
            atts_write_rsp_send(beacon_env.con_info.conhdl, param->handle, status);
        }
    }

    return (KE_MSG_CONSUMED);
}
 

/**
 ****************************************************************************************
 * @brief Disconnection indication to BAPS.
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance
 * @param[in] src_id    ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int gap_discon_cmp_evt_handler(ke_msg_id_t const msgid,
                                        struct gap_discon_cmp_evt const *param,
                                        ke_task_id_t const dest_id,
                                        ke_task_id_t const src_id)
{
    //Check Connection Handle
    if (param->conhdl == beacon_env.con_info.conhdl)
    {
        //bass_disable();
    }

    return (KE_MSG_CONSUMED);
}
 
/// Disabled State handler definition.
const struct ke_msg_handler beacon_disabled[] =
{
    {BEACON_CREATE_DB_REQ,            (ke_msg_func_t) beacon_create_db_req_handler}
};

/// Idle State handler definition.
const struct ke_msg_handler beacon_idle[] =
{
    {BEACON_ENABLE_REQ,               (ke_msg_func_t) beacon_enable_req_handler}
};

/// Connected State handler definition.
const struct ke_msg_handler beacon_connected[] =
{
//     {BEACON_VAL_UPD_REQ,            (ke_msg_func_t) beacon_val_upd_req_handler},
    {GATT_WRITE_CMD_IND,            (ke_msg_func_t) gatt_write_cmd_ind_handler},
//     {GATT_NOTIFY_CMP_EVT,           (ke_msg_func_t) gatt_notify_cmp_evt_handler},
};
 
/// Specifies the message handler structure for every input state.
const struct ke_state_handler beacon_state_handler[BEACON_STATE_MAX] =
{
    [BEACON_DISABLED]       = KE_STATE_HANDLER(beacon_disabled),
    [BEACON_IDLE]           = KE_STATE_HANDLER(beacon_idle),
    [BEACON_CONNECTED]      = KE_STATE_HANDLER(beacon_connected),
};
 
/// Default State handlers definition
const struct ke_msg_handler beacon_default_state[] =
{
    {GAP_DISCON_CMP_EVT,         (ke_msg_func_t)gap_discon_cmp_evt_handler},
};

/// Specifies the message handlers that are common to all states.
const struct ke_state_handler beacon_default_handler = KE_STATE_HANDLER(beacon_default_state);
 
/// Defines the place holder for the states of all the task instances.
ke_state_t beacon_state[BEACON_IDX_MAX];


// Registet BEACON task into kernel
void task_beacon_desc_register(void)
{
    struct ke_task_desc task_beacon_desc;
    
    task_beacon_desc.state_handler = beacon_state_handler;
    task_beacon_desc.default_handler = &beacon_default_handler;
    task_beacon_desc.state = beacon_state;
    task_beacon_desc.state_max = BEACON_STATE_MAX;
    task_beacon_desc.idx_max = BEACON_IDX_MAX;
    
    task_desc_register(TASK_BEACON, task_beacon_desc);
}



#endif


