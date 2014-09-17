
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
        if(param->alert_lvl == 2)
        {
            buzzer_on(BUZZ_VOL_HIGH);
            ke_timer_set(APP_SYS_BUZZ_TIMER, TASK_APP, BUZZ_ALERT_PERIOD);    // 5 seconds
        }
        else if(param->alert_lvl == 1)
        {
            buzzer_on(BUZZ_VOL_HIGH);
            //ke_timer_set(APP_PROXR_ALERT_STOP_TIMER,TASK_APP,500);
            ke_timer_set(APP_SYS_BUZZ_TIMER, TASK_APP, BUZZ_ALERT_PERIOD);    // 5 seconds
        }
        else
        {
            if (usr_env.buzz_st_on == false)  
            {  
                buzzer_off();
                ke_timer_clear(APP_SYS_BUZZ_TIMER, TASK_APP);
                ke_timer_clear(APP_PROXR_ALERT_STOP_TIMER, TASK_APP);
            }
        }
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
    
    //usr_env.buzz_st_on = false;
    usr_buzz_process(BUZZER_INTER_OFF);
    //app_proxr_env->alert_lvl = 0;
    QPRINTF("alert_stop_timer_handler.\r\n");
    return (KE_MSG_CONSUMED);
}

