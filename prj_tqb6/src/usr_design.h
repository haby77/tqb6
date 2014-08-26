/**
 ****************************************************************************************
 *
 * @file usr_design.h
 *
 * @brief Product related design header file.
 *
 * Copyright (C) Quintic 2012-2013
 *
 * $Rev: 1.0 $
 *
 ****************************************************************************************
 */

#ifndef USR_DESIGN_H_
#define USR_DESIGN_H_


/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "app_env.h"
#include "gpio.h"


#include "test_timer.h"

#ifdef CFG_PRF_PXPR
#include "usr_proxr.h"
#endif

#ifdef CFG_PRF_BASS
#include "usr_bass.h"
// The number of sample to battery voltage 
#define BASS_SAMPLE_NUMBER                16
#endif

/*
 * STRUCTURE DEFINITIONS
 ****************************************************************************************
 */

struct usr_env_tag
{
    uint16_t    led1_on_dur;
    uint16_t    led1_off_dur;

#if (defined(QN_ADV_WDT))
    bool        adv_wdt_enable;
    void        (*adv_wdt_to)(void);
#endif
    
#if (defined(BASS_SAMPLE_NUMBER))
    int16_t     bas_reg_buf[BASS_SAMPLE_NUMBER];
#endif
};

extern struct usr_env_tag usr_env;

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

extern void app_task_msg_hdl(ke_msg_id_t const msgid, void const *param);
extern int app_led_timer_handler(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
extern int app_gap_adv_intv_update_timer_handler(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
extern void usr_sleep_restore(void);
extern void usr_button1_cb(void);
extern int app_button_timer_handler(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
extern void usr_init(void);
extern void gpio_interrupt_callback(enum gpio_pin pin);
//t-chip
extern int app_led_breath_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id, ke_task_id_t const src_id);
extern void usr_button2_cb(void);
extern void app_event_button2_press_handler(void);
extern void usr_button1_cb(void);
extern void app_event_button1_press_handler(void);

#endif
