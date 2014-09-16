/**
 ****************************************************************************************
 *
 * @file buzz.c
 *
 * @brief buzzer driver for qn evb.
 *
 * Copyright (C) Quintic 2012-2013
 *
 * $Rev: 1.0 $
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup  BUZZ
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "pwm.h"
#include "buzz.h"

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief   Switch buzzer on
 * @param[in]    volume     buzzer's volume
 ****************************************************************************************
 */
void buzzer_on(enum buzz_vol volume)
{
    pwm_init(PWM_CH0);
    pwm_config(PWM_CH0, PWM_PSCAL_DIV, PWM_COUNT_US(BUZZ_PWM_PERIOD, PWM_PSCAL_DIV), PWM_COUNT_US(volume, PWM_PSCAL_DIV));
    pwm_enable(PWM_CH0, MASK_ENABLE);
}

/**
 ****************************************************************************************
 * @brief   Switch buzzer off
 ****************************************************************************************
 */
void buzzer_off(void)
{
    pwm_enable(PWM_CH0, MASK_DISABLE);
}

void usr_buzz_process(void)
{
    if(pwm_pwm_GetCR(QN_PWM) & PWM_CH0)
    {
        buzzer_off();
        ke_timer_set(APP_SYS_BUZZ_TIMER, TASK_APP,BUZZ_ALERT_PERIOD);
    }
    else
    {
        
        buzzer_on(BUZZ_VOL_HIGH);
        ke_timer_set(APP_SYS_BUZZ_TIMER, TASK_APP,BUZZ_ALERT_PERIOD);
    }
}

/// @} BUZZ
