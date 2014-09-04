

#include "app_env.h"

#if BLE_BEACON_SERVER
#include "app_beacon.h"

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */
struct app_beacon_env_tag *app_beacon_env = &app_env.beacon_ev;

/// @endcond

/*
 ****************************************************************************************
 * @brief Handles the create database confirmation from the BASS.   *//**
 *
 * @param[in] msgid     BASS_CREATE_DB_CFM
 * @param[in] param     Pointer to the struct bass_create_db_cfm
 * @param[in] dest_id   TASK_APP
 * @param[in] src_id    TASK_BEACON
 *
 * @return If the message was consumed or not.
 * @description
 * This handler will be called after a database creation. The status of parameter may have
 * the following values:
 * - PRF_ERR_OK
 * - PRF_ERR_INVALID_PARAM
 * - ATT_INSUFF_RESOURCE
 ****************************************************************************************
 */
int app_beacon_create_db_cfm_handler(ke_msg_id_t const msgid,
                                   struct beacon_create_db_cfm *param,
                                   ke_task_id_t const dest_id,
                                   ke_task_id_t const src_id)
{
    if (param->status == ATT_ERR_NO_ERROR)
    {
        //app_clear_local_service_flag(BLE_BATT_SERVER_BIT);
    }
    return (KE_MSG_CONSUMED);
}

#endif //BLE_BEACON_SERVER
