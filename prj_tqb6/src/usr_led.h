#ifndef __USR_LED_H__
#define __USR_LED_H__

#include "stdint.h"
#include "app_config.h"
#include "ke_task.h"

#define LED_FLASH

#if (defined(LED_BREATH))

/**********************************************
*    MACRO DEFINITION
 *********************************************/   
#define LED_PWM_PERIOD 255
#define LED_MEG_PERIOD 20

//breath array
extern uint8_t led_breath_array[32];

//t-chip
extern void led_breath_on(int volume);
extern void led_breath_off(void);
#endif

#if (LED_FALSH)
extern void usr_led1_set(uint16_t timer_on, uint16_t timer_off);
extern void usr_led1_process(void);
#endif

#endif

