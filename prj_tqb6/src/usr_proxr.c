
#include "usr_proxr.h"
#include "buzz.h"

/**
 ****************************************************************************************
 * @brief   Show proxr alert. User can add their own code here to show alert.
 ****************************************************************************************
 */
void usr_proxr_alert(struct proxr_alert_ind *param)
{
    // If it is PROXR_LLS_CHAR writting indication, don't alert,
    // otherwise (PROXR_IAS_CHAR, disconnect) to alert
    if (param->char_code == PROXR_IAS_CHAR || app_proxr_env->enabled == false)
    {
        if (buzz_env.st == BUZZER_FORCE_ON)
            return;
        if(param->alert_lvl == 2)
        {
           buzz_env.vol = BUZZ_VOL_HIGH;
           app_buzz_config(BUZZ_ON_S,BUZZER_NORMAL_ON);

        }
        else if(param->alert_lvl == 1)
        {
            buzz_env.vol = BUZZ_VOL_LOW;
            app_buzz_config(BUZZ_ON_S,BUZZER_NORMAL_ON);
        }
        else
        { 
            buzzer_off();
            buzz_env.st = BUZZER_IDLE;
        }
        ke_timer_set(APP_SYS_BUZZ_TIMER, TASK_APP,buzz_env.priod); 
    }

}

/**
 ****************************************************************************************
 * @brief Handles alert stop timer.
 *
 * @param[in] msgid     APP_PROXR_ALERT_STOP_TO
 * @param[in] param     Null
 * @param[in] dest_id   TASK_APP
 * @param[in] src_id    TASK_NONE
 *
 * @return If the message was consumed or not.
 * @description
 * This handler is used to request the Application to start the alert on the device
 * considering the indicated alert level. The handler may be triggered on two conditions:
 * The IAS alert level characteristic has been written to a valid value, in which case
 * alert_lvl will be set to the IAS alert level value.
 * A disconnection with a reason other than the normal local/remote link terminations has
 * been received, in which case alert_lvl will be set to the LLS alert level value.
 * The Application actions following reception of this indication are strictly implementation
 * specific (it may try to reconnect to the peer and stop alert upon that, or timeout the
 * alert after acertain time, please see the specification)
 ****************************************************************************************
 */
int app_proxr_alert_to_handler(ke_msg_id_t const msgid,
                               void *param,
                               ke_task_id_t const dest_id,
                               ke_task_id_t const src_id)
{
    // Stop proxr alert
    if (buzz_env.st != BUZZER_FORCE_ON )
    {
        buzz_env.st = BUZZER_IDLE;
        buzzer_off();
    }
    QPRINTF("alert_stop_timer_handler.\r\n");
    return (KE_MSG_CONSUMED);
}

