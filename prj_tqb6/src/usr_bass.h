#ifndef __USR_BASS_H__
#define __USR_BASS_H__


#include "app_env.h"
/*
 * DEFINITIONS
 ****************************************************************************************
 */


// The interval of updating, unit: 10ms
#define APP_BASS_BATT_LEVEL_TO            1100
// the event id of adc sample complete
#define EVENT_ADC_SAMPLE_CMP_ID           3
// The fully charged voltage and fully discharged voltage value, unit: mv
#define BASS_FULLY_CHARGED_VOLTAGE        3000
#define BASS_FULLY_DISCHARGED_VOLTAGE     2300


extern int app_bass_batt_level_timer_handler(ke_msg_id_t const msgid,
                                      void *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id);												 

extern void app_event_adc_sample_cmp_handler(void);

#endif
