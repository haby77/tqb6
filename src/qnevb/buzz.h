/**
 ****************************************************************************************
 *
 * @file buzz.h
 *
 * @brief Header file of buzz driver for qn evb.
 *
 * Copyright (C) Quintic 2012-2013
 *
 * $Rev: 1.0 $
 *
 ****************************************************************************************
 */
#ifndef _BUZZ_H_
#define _BUZZ_H_

#include "app_env.h"

/*
 * MACRO DEFINITIONS
 ****************************************************************************************
 */

#define BUZZ_PWM_PERIOD 200
#define BUZZ_ALERT_PERIOD_S   34
#define BUZZ_ALERT_PERIOD_L   84

#define BUZZ_ON_L     0
#define BUZZ_ON_S     1
#define BUZZ_OFF    2
/*
 * ENUMERATION DEFINITIONS
 ****************************************************************************************
 */

enum buzz_vol
{
    BUZZ_VOL_HIGH = 150,
    BUZZ_VOL_LOW = 20
};

enum buzz_st
{
    BUZZER_FORCE_ON = 1,
    BUZZER_COUNT_ON,
    BUZZER_NORMAL_ON,
    BUZZER_OFF,
    BUZZER_IDLE
};

struct buzz_env_tag
{   
    enum buzz_vol vol;         
    enum buzz_st  st;       //buzzer work state
    uint8_t mode;           //
    uint8_t count;          //times count
    uint8_t priod;          //alert_mode   long alert or short alert
};

extern struct buzz_env_tag buzz_env;

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

extern void buzzer_on(enum buzz_vol volume);
extern void buzzer_off(void);
extern void app_buzz_config(uint8_t alert_mode,uint8_t buzz_state);
extern int usr_buzz_process_handle(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id, ke_task_id_t const src_id);


#endif
