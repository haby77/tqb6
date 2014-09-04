
/*
 * INCLUDE FILES
 ****************************************************************************************
 */
 
#include "app_env.h"
#if BLE_BEACON_SERVER
#include "app_beacon.h"

void app_beacon_create_db()
{
//     uint8_t i;
    struct beacon_create_db_req * msg = KE_MSG_ALLOC(BEACON_CREATE_DB_REQ, TASK_BEACON, TASK_APP, beacon_create_db_req);

//     msg->bas_nb = bas_nb;
//     // Features of each BAS instance
//     for (i = 0; i < bas_nb; i++)
//         msg->features[i] = features[i];
    ke_msg_send(msg);
}

void app_beacon_enable_req(uint16_t conhdl, uint8_t sec_lvl, uint8_t con_type, 
                         uint8_t measured_power_val)
{
    struct beacon_enable_req * msg = KE_MSG_ALLOC(BEACON_ENABLE_REQ, TASK_BEACON, TASK_APP, beacon_enable_req);

    msg->conhdl = conhdl;
    msg->sec_lvl = sec_lvl;
    msg->con_type = con_type;
    
    msg->measured_power_val = measured_power_val;
    
    ke_msg_send(msg);
}


#endif

