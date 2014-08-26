#ifndef __USR_BUTTON_H_
#define __USR_BUTTON_H_

#define EVENT_BUTTON1_PRESS_ID						0
#define EVENT_BUTTON2_PRESS_ID						1

extern void app_event_button1_press_handler(void);
extern void usr_button1_cb(void);
extern void app_event_button2_press_handler(void);
extern void usr_button2_cb(void);

#endif

