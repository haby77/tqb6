#ifndef __USR_LED_H__
#define __USR_LED_H__

#include "stdint.h"


 
//t-chip
#define LED_PWM_PERIOD 255
#define LED_MEG_PERIOD 20
//end

 //t-chip
extern unsigned char vol;
extern unsigned char vol_breath[32];


//t-chip
extern void led_breath_on(int volume);
extern void usr_led1_set(uint16_t timer_on, uint16_t timer_off);
extern void usr_led1_process(void);

#endif
