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
#include "usr_task.h"


struct buzz_env_tag buzz_env = {BUZZ_VOL_HIGH,BUZZER_IDLE,0,0,BUZZ_ALERT_PERIOD_S};

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

int usr_buzz_process_handle(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    QPRINTF("usr_buzz_process_handle.\r\n");
    if(pwm_pwm_GetCR(QN_PWM) & PWM_CH0)
    {
        buzzer_off();
    }
    else
    {
        buzzer_on(buzz_env.vol);
        ke_timer_set(USR_BUZZER_TIMER,TASK_USR,buzz_env.priod);
    }
    return(KE_MSG_CONSUMED);
}

void app_buzz_config( uint8_t alert_mode,uint8_t buzz_state)
{
    if (alert_mode == BUZZ_ON_L)
    {
         buzz_env.mode = 1;
         buzz_env.priod = BUZZ_ALERT_PERIOD_L;
    }
    else
    {
        buzz_env.mode = 0;
        buzz_env.priod = BUZZ_ALERT_PERIOD_S;
    }
        switch(buzz_state)
        {
            case    BUZZER_FORCE_ON:
                buzz_env.st = BUZZER_FORCE_ON;
                ke_timer_set(APP_SYS_BUZZ_TIMER, TASK_APP,buzz_env.priod);
                break;
            case    BUZZER_COUNT_ON:
                buzz_env.st = BUZZER_COUNT_ON;
                ke_timer_set(APP_SYS_BUZZ_TIMER, TASK_APP,buzz_env.priod);
//                if ((--buzz_env.count) == 0)
//                {
//                    ke_timer_clear(APP_SYS_BUZZ_TIMER, TASK_APP);
//                }
                break;
            case    BUZZER_NORMAL_ON:
                buzz_env.st = BUZZER_NORMAL_ON;
                ke_timer_set(APP_SYS_BUZZ_TIMER, TASK_APP,buzz_env.priod);
                break;
            case    BUZZER_IDLE:
                buzz_env.st = BUZZER_IDLE;
                ke_timer_set(APP_SYS_BUZZ_TIMER, TASK_APP,buzz_env.priod);
                break;
            case    BUZZER_OFF:
                buzz_env.st = BUZZER_OFF;
                ke_timer_set(APP_SYS_BUZZ_TIMER, TASK_APP,buzz_env.priod);
                break;
            default:
                break;
        }
}


/// @} BUZZ
