#ifndef __APP_BEACON_H__
#define __APP_BEACON_H__

#if BLE_BEACON_SERVER
#include "beacon.h"
#include "beacon_task.h"
//#include "app_bass_task.h"

extern void app_beacon_create_db(void);

extern void app_beacon_enable_req(uint16_t conhdl, uint8_t sec_lvl, uint8_t con_type, 
                         uint8_t measured_power_val);

#endif //BLE_BEACON_SERVER

#endif


