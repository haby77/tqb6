/**
 ****************************************************************************************
 *
 * @file usr_design.c
 *
 * @brief Product related design.
 *
 * Copyright (C) Quintic 2012-2013
 *
 * $Rev: 1.0 $
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup  USR
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include <stdint.h>
#include "app_env.h"
#include "led.h"
#include "uart.h"
#include "buzz.h"
#include "lib.h"
#include "usr_design.h"
#include "gpio.h"
#include "button.h"
#if (defined(QN_ADV_WDT))
#include "wdt.h"
#endif
#include "sleep.h"
//tchip add
#include "usr_led.h"
#include "usr_button.h"
#include "usr_task.h"


/*
 * MACRO DEFINITIONS
 ****************************************************************************************
 */

#define LED_ON_DUR_ADV_FAST        75//
#define LED_OFF_DUR_ADV_FAST       75//(uint16_t)((GAP_ADV_FAST_INTV2*0.625)/10)
#define LED_ON_DUR_ADV_SLOW        25//2                                                //unused
#define LED_OFF_DUR_ADV_SLOW       250//(uint16_t)((GAP_ADV_SLOW_INTV*0.625))            //unused
#define LED_ON_DUR_CON             2
#define LED_OFF_DUR_CON            1000
#define LED_ON_DUR_IDLE                   0
#define LED_OFF_DUR_IDLE                  0xffff


///IOS Connection Parameter
#define IOS_CONN_INTV_MAX                              0x0010
#define IOS_CONN_INTV_MIN                              0x0008
#define IOS_SLAVE_LATENCY                              0x0000
#define IOS_STO_MULT                                   0x012c

//tchip add
#define KEY_SHORT_GATE                    5                 
#define KEY_LONG_GATE                     6
#define KEY_ALERT_GATE                    45

#define KEY_NO_LONG_PRESS           (true)
#define KEY_LONG_PRESS              (false)

#define KEY_NO_ALERT_PRESS          (true)
#define KEY_ALERT_PRESS             (false)

/*
 * LOCAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

static  bool    KEY_LONG_PRESS_FLAG;     
static bool     KEY_ALERT_FLAG = false;
static  uint8_t Button_key_count = 0;
//end

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

#if (defined(QN_ADV_WDT))
static void adv_wdt_to_handler(void)
{
    ke_state_set(TASK_APP, APP_IDLE);

    // start adv
    app_gap_adv_start_req(GAP_GEN_DISCOVERABLE|GAP_UND_CONNECTABLE,
                          app_env.adv_data, app_set_adv_data(GAP_GEN_DISCOVERABLE),
                          app_env.scanrsp_data, app_set_scan_rsp_data(app_get_local_service_flag()),
                          GAP_ADV_FAST_INTV1, GAP_ADV_FAST_INTV2);
}
#endif

struct usr_env_tag usr_env = {LED_ON_DUR_IDLE, LED_OFF_DUR_IDLE};


void usr_env_init(void)
{
	#ifdef CFG_ADV_WDT
		usr_env.adv_wdt_enable = false;
		usr_env.adv_wdt_to = adv_wdt_to_handler;
	#endif
	
	#ifdef	USR_LED_BREATH
		usr_env.led_breath_enable = false;
	#endif

	#ifdef CFG_PRF_BASS
		//usr_env.bas_reg_buf = 
	#endif

	#ifdef CFG_PRF_OTAS
		usr_env.ota_update = false;
	#endif
}

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */


/**
 ****************************************************************************************
 * @brief   Application task message handler
 ****************************************************************************************
 */
void app_task_msg_hdl(ke_msg_id_t const msgid, void const *param)
{
    switch(msgid)
    {
        case GAP_SET_MODE_REQ_CMP_EVT:
            if(APP_IDLE == ke_state_get(TASK_APP))
            {
                usr_led1_set(LED_ON_DUR_ADV_FAST, LED_OFF_DUR_ADV_FAST);
                ke_timer_set(APP_ADV_INTV_UPDATE_TIMER, TASK_APP, 20 * 100);
#if (defined(QN_ADV_WDT))
                usr_env.adv_wdt_enable = true;
#endif
            }
            break;

        case GAP_ADV_REQ_CMP_EVT:
            usr_led1_set(LED_ON_DUR_IDLE,LED_OFF_DUR_IDLE);
            ke_timer_clear(APP_ADV_INTV_UPDATE_TIMER, TASK_APP);
            break;

        case GAP_DISCON_CMP_EVT:
            //tchip add
            if (((struct gap_discon_cmp_evt *)param)->reason == 0x08)   //discon with close the ios app or close bluetooth on iphone
            {
                app_buzz_config(BUZZ_ON_S,BUZZER_FORCE_ON); 
							  app_gap_adv_start_req(GAP_GEN_DISCOVERABLE|GAP_UND_CONNECTABLE,
                    app_env.adv_data, app_set_adv_data(GAP_GEN_DISCOVERABLE),
                    app_env.scanrsp_data, app_set_scan_rsp_data(app_get_local_service_flag()),
                    GAP_ADV_FAST_INTV1, GAP_ADV_FAST_INTV2);
								ke_timer_set(APP_ADV_INTV_UPDATE_TIMER, TASK_APP, 30 * 100);
								if (((struct proxr_alert_ind*)param)->alert_lvl != 0)
                      usr_led1_set(LED_ON_DUR_ADV_FAST, LED_OFF_DUR_ADV_FAST);
                else
                      usr_led1_set(LED_ON_DUR_IDLE, LED_OFF_DUR_IDLE);							
            }
            else    
                if (((struct gap_discon_cmp_evt *)param)->reason == 0x13)   //discon with ios app normal cancel
                {
                    buzz_env.count = 3;
                    app_buzz_config(BUZZ_ON_S,BUZZER_COUNT_ON); 
                }
                else
                {
                    buzz_env.count = 2;
                    app_buzz_config(BUZZ_ON_S,BUZZER_COUNT_ON);
                }
                ke_timer_set(APP_SYS_BUZZ_TIMER,TASK_APP,1);
            //end
            //usr_led1_set(LED_ON_DUR_IDLE, LED_OFF_DUR_IDLE);
 
//tchip add 
#ifdef CFG_PRF_BASS 
            ke_timer_clear(APP_BASS_BATT_LEVEL_TIMER, TASK_APP);
#endif
//end

            // start adv
//            app_gap_adv_start_req(GAP_GEN_DISCOVERABLE|GAP_UND_CONNECTABLE,
//                    app_env.adv_data, app_set_adv_data(GAP_GEN_DISCOVERABLE),
//                    app_env.scanrsp_data, app_set_scan_rsp_data(app_get_local_service_flag()),
//                    GAP_ADV_FAST_INTV1, GAP_ADV_FAST_INTV2);
            break;

        case GAP_LE_CREATE_CONN_REQ_CMP_EVT:
            if(((struct gap_le_create_conn_req_cmp_evt *)param)->conn_info.status == CO_ERROR_NO_ERROR)
            {
                if(GAP_PERIPHERAL_SLV == app_get_role())
                {
                    ke_timer_clear(APP_ADV_INTV_UPDATE_TIMER, TASK_APP);
                    usr_led1_set(LED_ON_DUR_CON, LED_OFF_DUR_CON);
#if (defined(QN_ADV_WDT))
                    usr_env.adv_wdt_enable = false;
#endif

                    // Update cnx parameters
                    //if (((struct gap_le_create_conn_req_cmp_evt *)param)->conn_info.con_interval >  IOS_CONN_INTV_MAX)
                    {
                        // Update connection parameters here
                        struct gap_conn_param_update conn_par;
                        /// Connection interval minimum
                        conn_par.intv_min = IOS_CONN_INTV_MIN;
                        /// Connection interval maximum
                        conn_par.intv_max = IOS_CONN_INTV_MAX;
                        /// Latency
                        conn_par.latency = IOS_SLAVE_LATENCY;
                        /// Supervision timeout, Time = N * 10 msec
                        conn_par.time_out = IOS_STO_MULT;
                        app_gap_param_update_req(((struct gap_le_create_conn_req_cmp_evt *)param)->conn_info.conhdl, &conn_par);
                    }
                    //t-chip add
                    buzz_env.st = BUZZER_IDLE;
                    buzz_env.count = 1;
                    app_buzz_config(BUZZ_ON_L,BUZZER_COUNT_ON);
                    ke_timer_set(APP_SYS_BUZZ_TIMER,TASK_APP,1);
                    //end
                }
           }   
//tchip add
#ifdef CFG_PRF_BASS  
		if (0)
			{
				//Force immediately update the battery voltage
            	app_bass_batt_level_timer_handler(APP_BASS_BATT_LEVEL_TIMER, NULL, TASK_APP, TASK_APP);
			}	
            
#endif
            break;

        case QPPS_DISABLE_IND:
            break;

        case QPPS_CFG_INDNTF_IND:
            //tchip
            break;
		        
        case OTAS_TRANSIMIT_STATUS_IND:
					
#ifdef CFG_PRF_BASS
						bass_disable();
#endif
            QPRINTF(" APP get OTA transmit status = %d , describe = %d \r\n" , ((struct otas_transimit_status_ind*)param)->status,
                                                                              ((struct otas_transimit_status_ind*)param)->status_des);
            
            //only need response once when ota status is in ota status start request
            if(((struct otas_transimit_status_ind*)param)->status == OTA_STATUS_START_REQ)  
            {
                app_ota_ctrl_resp(START_OTA);
            }
            break;
//end
	
#ifdef CFG_PRF_PXPR
				case PROXR_ALERT_IND:
						usr_proxr_alert((struct proxr_alert_ind*)param);
                        if (((struct proxr_alert_ind*)param)->alert_lvl != 0)
                            usr_led1_set(LED_ON_DUR_ADV_FAST, LED_OFF_DUR_ADV_FAST);
                        else
                            usr_led1_set(LED_ON_DUR_IDLE, LED_OFF_DUR_IDLE);
						break;
#endif

//tchip add
#ifdef CFG_PRF_BASS     
                
        case BASS_DISABLE_IND:
            ke_timer_clear(APP_BASS_BATT_LEVEL_TIMER, TASK_APP);
            break;

        case BASS_BATT_LEVEL_NTF_CFG_IND:
			if (usr_env.ota_update == true)
			{
				bass_disable();
				break;
			}	
				
            if (((struct bass_batt_level_ntf_cfg_ind *)param)->ntf_cfg == PRF_CLI_START_NTF)
            {
                app_bass_env->ntf_sending = false;
                // Start update timer
                ke_timer_set(APP_BASS_BATT_LEVEL_TIMER, TASK_APP, APP_BASS_BATT_LEVEL_TO);
            }
            else
            {
                // Stop update timer
                ke_timer_clear(APP_BASS_BATT_LEVEL_TIMER, TASK_APP);
            }
            break;
#endif
//end
        default:
            break;
    }
}

/*******************************************************************************************






********************************************************************************************/
int app_gap_adv_sotp_timer_handler(ke_msg_id_t const msgid, void const *param,
                               ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    app_gap_adv_stop_req();
    return(KE_MSG_CONSUMED);
}



/**
 ****************************************************************************************
 * @brief Handles LED status timer.
 *
 * @param[in] msgid      APP_SYS_UART_DATA_IND
 * @param[in] param      Pointer to struct app_uart_data_ind
 * @param[in] dest_id    TASK_APP
 * @param[in] src_id     TASK_APP
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
int app_led_timer_handler(ke_msg_id_t const msgid, void const *param,
                               ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    if(msgid == APP_SYS_LED_1_TIMER)
    {
//tchip add
#if (defined(LED_BREATH))
    if (usr_env.led_breath_enable == LED_BREATH_ON)
    {
        usr_env.led_breath_enable = LED_BREATH_OFF;
        led_breath_off();   
        led_set(1,LED_OFF);
    }

#endif
//end
        usr_led1_process();
    }

    return (KE_MSG_CONSUMED);
}

//tchip add
/****************************************************************************************
 * @brief Handles BUZZ status timer.
 *
 * @param[in] msgid      APP_SYS_UART_DATA_IND
 * @param[in] param      Pointer to struct app_uart_data_ind
 * @param[in] dest_id    TASK_APP
 * @param[in] src_id     TASK_APP
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
int app_buzz_timer_handler(ke_msg_id_t const msgid, void const *param,
                               ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
        switch((uint8_t)buzz_env.st)
        {
            case   BUZZER_FORCE_ON:
            case   BUZZER_NORMAL_ON:       
                ke_timer_set(USR_BUZZER_TIMER,TASK_USR,buzz_env.priod);
                ke_timer_set(APP_SYS_BUZZ_TIMER,TASK_APP,buzz_env.priod+1);
                break;
            case   BUZZER_COUNT_ON:
                if (buzz_env.count-- != 0)
                {
                    ke_timer_set(USR_BUZZER_TIMER,TASK_USR,buzz_env.priod);
                    ke_timer_set(APP_SYS_BUZZ_TIMER,TASK_APP,buzz_env.priod+1);
                }
                break;
            case    BUZZER_IDLE:
                ke_timer_clear(USR_BUZZER_TIMER,TASK_USR);
                buzzer_off();
                break;
            case    BUZZER_OFF:
                buzzer_off();
                ke_timer_clear(USR_BUZZER_TIMER,TASK_USR);
                ke_timer_clear(APP_SYS_BUZZ_TIMER,TASK_APP);
                break;
                
        }
        return (KE_MSG_CONSUMED);
}
//end

/**
 ****************************************************************************************
 * @brief Handles advertising mode timer event.
 *
 * @param[in] msgid     APP_ADV_INTV_UPDATE_TIMER
 * @param[in] param     None
 * @param[in] dest_id   TASK_APP
 * @param[in] src_id    TASK_APP
 *
 * @return If the message was consumed or not.
 * @description
 *
 * This handler is used to inform the application that first phase of adversting mode is timeout.
 ****************************************************************************************
 */
int app_gap_adv_intv_update_timer_handler(ke_msg_id_t const msgid, void const *param,
                                          ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    if(APP_ADV == ke_state_get(TASK_APP))
    {
        usr_led1_set(LED_ON_DUR_ADV_SLOW, LED_OFF_DUR_ADV_SLOW);

        // Update Advertising Parameters
        app_gap_adv_start_req(GAP_GEN_DISCOVERABLE|GAP_UND_CONNECTABLE, 
                                app_env.adv_data, app_set_adv_data(GAP_GEN_DISCOVERABLE), 
                                app_env.scanrsp_data, app_set_scan_rsp_data(app_get_local_service_flag()),
                                GAP_ADV_SLOW_INTV, GAP_ADV_SLOW_INTV);
    }

    return (KE_MSG_CONSUMED);
}

/**
 ****************************************************************************************
 * @brief   Restore peripheral setting after wakeup
 ****************************************************************************************
 */
void usr_sleep_restore(void)
{
#if QN_DBG_PRINT
    uart_init(QN_DEBUG_UART, USARTx_CLK(0), UART_9600);
    uart_tx_enable(QN_DEBUG_UART, MASK_ENABLE);
    uart_rx_enable(QN_DEBUG_UART, MASK_ENABLE);
#endif

#if (defined(QN_ADV_WDT))
    if(usr_env.adv_wdt_enable)
    {
        wdt_init(1007616, WDT_INT_MOD); // 30.75s
    }
#endif
}

/**
 ****************************************************************************************
 * @brief Handles button press after cancel the jitter.
 *
 * @param[in] msgid     Id of the message received
 * @param[in] param     None
 * @param[in] dest_id   TASK_APP
 * @param[in] src_id    TASK_APP
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
int app_button_timer_handler(ke_msg_id_t const msgid, void const *param,
                               ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
            // make sure the button is pressed
            if (KEY_LONG_PRESS_FLAG == KEY_LONG_PRESS)
            {
                
                if(APP_IDLE == ke_state_get(TASK_APP))
                {
                    if(!app_qpps_env->enabled)
                    {
                        // start adv
                        app_gap_adv_start_req(GAP_GEN_DISCOVERABLE|GAP_UND_CONNECTABLE,
                                app_env.adv_data, app_set_adv_data(GAP_GEN_DISCOVERABLE),
                                app_env.scanrsp_data, app_set_scan_rsp_data(app_get_local_service_flag()),
                                GAP_ADV_FAST_INTV1, GAP_ADV_FAST_INTV2);

#if (QN_DEEP_SLEEP_EN)
                        // prevent entering into deep sleep mode
                        sleep_set_pm(PM_SLEEP);
#endif
                    }//if APP_IDLE && KEY_LONG_PRESS && Server no working on,start adv.
                    else
                    {
                        //tchip add
                        uint8_t cmd = 0x02;
                        if (KEY_ALERT_FLAG == KEY_ALERT_PRESS)
                        {
                            cmd = 0x03;
                        }
                            //QPRINTF("cmd is %d\r\n",cmd);
                            app_qpps_data_send(app_qpps_env->conhdl, 0, 1, &cmd);
                            
                    }//if APP_IDLE && Server is working on,sent msg to alert.
                    //end
                }
                else if(APP_ADV == ke_state_get(TASK_APP))
                {
                    // stop adv
                    app_gap_adv_stop_req();

#if (QN_DEEP_SLEEP_EN)
                    // allow entering into deep sleep mode
                    sleep_set_pm(PM_DEEP_SLEEP);
#endif
                }//if APP_ADV ,stop ADV and sleep.
                //tchip add
                //close the abnormal alert
//                if (KEY_ALERT_FLAG == KEY_ALERT_PRESS)
//                {
//                    //QPRINTF("KEY_ALERT_PRESS!\r\n");
//                    buzz_env.st = BUZZER_OFF;
//										ke_timer_set(APP_SYS_BUZZ_TIMER,TASK_APP,1);
//                }
                //end
            }
            else
            {
                //tchip add
                if (APP_IDLE == ke_state_get(TASK_APP))
                {
										app_proxr_env->alert_lvl = 0;
										led_set(1, LED_OFF);
										ke_timer_clear(APP_SYS_LED_1_TIMER, TASK_APP);
                   	buzz_env.st = BUZZER_OFF;
										ke_timer_set(APP_SYS_BUZZ_TIMER,TASK_APP,1);
										uint8_t cmd = 0x10;
                    app_qpps_data_send(app_qpps_env->conhdl, 0, 1, &cmd);
                }
//               if (buzz_env.st != BUZZER_OFF && buzz_env.st != BUZZER_IDLE)
//               {
//                    usr_env.alert_st = true;
//										led_set(1, LED_OFF);
//										ke_timer_clear(APP_SYS_LED_1_TIMER, TASK_APP);
//                   	buzz_env.st = BUZZER_OFF;
//										ke_timer_set(APP_SYS_BUZZ_TIMER,TASK_APP,1);
//										ke_timer_set(APP_PROXR_ALERT_STOP_TIMER,TASK_APP,1);								 
//               }
                //end
								if (app_proxr_env->alert_lvl != 0 || led_get(1) == LED_ON)
								{
                   	buzz_env.st = BUZZER_OFF;
										ke_timer_set(APP_SYS_BUZZ_TIMER,TASK_APP,1);
								}
						}
    return (KE_MSG_CONSUMED);
}

/**
 ****************************************************************************************
 * @brief   All GPIO interrupt callback
 ****************************************************************************************
 */
void gpio_interrupt_callback(enum gpio_pin pin)
{
    switch(pin)
    {
        case BUTTON1_PIN:
            //Button 1 is used to enter adv mode.
            usr_button1_cb();
            break;


#if (defined(QN_TEST_CTRL_PIN))
        case QN_TEST_CTRL_PIN:
            //When test controll pin is changed to low level, this function will reboot system.
            gpio_disable_interrupt(QN_TEST_CTRL_PIN);
            syscon_SetCRSS(QN_SYSCON, SYSCON_MASK_REBOOT_SYS);
            break;
#endif

        default:
            break;
    }
}


/**
 ****************************************************************************************
 * @brief   User initialize
 ****************************************************************************************
 */
void usr_init(void)
{
	usr_env_init();

	if(KE_EVENT_OK != ke_evt_callback_set(EVENT_BUTTON1_PRESS_ID, 
                                            app_event_button1_press_handler))
    {
        ASSERT_ERR(0);
    }
    //tchip add
    task_usr_desc_register();
    
    ke_state_set(TASK_USR, USR_DISABLE);
   
    
#ifdef CFG_PRF_BASS
    // Register button ADC sample event callback
    if(KE_EVENT_OK != ke_evt_callback_set(EVENT_ADC_SAMPLE_CMP_ID,
                                            app_event_adc_sample_cmp_handler))
    {
        ASSERT_ERR(0);
    }
#endif //end
}

//t-chip
#if (defined(LED_BREATH))
int usr_led_breath_handler(ke_msg_id_t const msgid, void const *param,
                               ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
                if (usr_env.led_breath_enable == LED_BREATH_ON)
                {            	
                    usr_env.vol++;
                    led_breath_on(led_breath_array[usr_env.vol%32]);
                    ke_timer_set(USR_LED_BREATH,TASK_USR,LED_MEG_PERIOD);
                }
                else
                {
                    led_breath_off();
                    led_set(1,LED_OFF);
                    ke_timer_clear(USR_LED_BREATH,TASK_USR);
                }
				return(KE_MSG_CONSUMED);
}
#endif

int usr_key_st_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    if(!(check_button_state(BUTTON1_PIN)))  //press: 0  ;  idle:  1;
    {
        Button_key_count++;
        ke_timer_set(USR_KEY_ST,TASK_USR,10);
    }
    else
    {
        if (Button_key_count < KEY_SHORT_GATE)
            {
                KEY_LONG_PRESS_FLAG = KEY_NO_LONG_PRESS;
#if (defined(LED_BREATH))
                if (APP_IDLE == ke_state_get(TASK_APP))
                    usr_env.led_breath_enable = LED_BREATH_ON;
                else
                    usr_env.led_breath_enable = LED_BREATH_OFF;   
#endif
                ke_timer_set(APP_SYS_BUTTON_1_TIMER, TASK_APP, 2);           
            }
        if (Button_key_count > KEY_LONG_GATE)
        {
             if (Button_key_count > KEY_ALERT_GATE)
             {
                 KEY_ALERT_FLAG = KEY_ALERT_PRESS;
             }
             else
             {
                 KEY_ALERT_FLAG = KEY_NO_ALERT_PRESS;       
             }
             KEY_LONG_PRESS_FLAG = KEY_LONG_PRESS;
						 usr_led1_set(LED_ON_DUR_ADV_FAST, LED_OFF_DUR_ADV_FAST);
#if (defined(LED_BREATH))
             usr_env.led_breath_enable = LED_BREATH_OFF;  
#endif             
             ke_timer_set(APP_SYS_BUTTON_1_TIMER, TASK_APP, 2);
        }
        Button_key_count = 0;
#if (defined(LED_BREATH))
        ke_timer_set(USR_LED_BREATH,TASK_USR,LED_MEG_PERIOD);
#endif
        ke_timer_clear(USR_KEY_ST,TASK_USR);
    }  
    return(KE_MSG_CONSUMED);
}

/// @} USR

