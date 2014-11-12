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
#include "usr_buzz.h"

/*
 * MACRO DEFINITIONS
 ****************************************************************************************
 */

#define BUZZ_PWM_PERIOD 200
#define BUZZ_ALERT_PERIOD_S   6
#define BUZZ_ALERT_PERIOD_L   84

enum buzz_vol
{
    BUZZ_VOL_HIGH = 150,
    BUZZ_VOL_LOW = 20
};

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
extern void buzzer_on(void);//enum buzz_vol volume);
extern void buzzer_off(void);

#endif
