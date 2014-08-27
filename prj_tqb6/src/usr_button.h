#ifndef __USR_BUTTON_H_
#define __USR_BUTTON_H_

#include "gpio.h"

#define EVENT_BUTTON1_PRESS_ID						0
#define APP_KEY_CHEAK_PRIOD                         10   //100ms对按键轮询一次

extern void app_event_button1_press_handler(void);
extern void usr_button1_cb(void);
#endif

