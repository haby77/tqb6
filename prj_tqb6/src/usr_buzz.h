/**
 ****************************************************************************************
 *
 * @file usr_buzz.h
 *
 * @brief Header file of buzz driver for tqb6.
 *
 * Copyright (C) TChip 2014
 *
 * $Rev: 1.0 $
 *
 ****************************************************************************************
 */
#ifndef __USR_BUZZ_H__
#define	__USR_BUZZ_H__

#include "app_env.h"
#include "usr_task.h"
#include "pwm.h"
#include	"buzz.h"


/*
 * MICRO DEFINITIONS
 ****************************************************************************************
 */

#define BUZZ_ON_L     0
#define BUZZ_ON_S     1
#define	BUZZ_MUSIC		2
#define BUZZ_OFF    	3

//priod of note 
#define	N07P			2024			//note low 7
#define	N1P				1912			//note mid 1
#define	N2P				1703
#define	N3P				1517
#define	N4P				1432
#define	N5P				1275
#define	N6P				1136
#define	N7P				1012
#define	N11P			956
#define	N12P			851
#define	N13P			758
#define	N14P			715
#define	N15p			637

//impulse of note * 2 = priod of note
#define	N07I			1012
#define	N1I				956
#define	N2I				851
#define	N3I				758
#define	N4I				716
#define	N5I				637
#define	N6I				568
#define	N7I				506
#define	N11I			478
#define	N12I			425
#define	N13I			379
#define	N14I			357
#define	N15I			318

#define		MUSIC_ALERT_NB			136
#define		MUSIC_START_NB			8
#define 	MuSIC_END_NB				8


/*
 * ENUMERATION DEFINITIONS
 ****************************************************************************************
 */

enum buzz_status
{
    BUZZER_BLE_CON = 1,
		BUZZER_BLE_DISCON,
		BUZZER_APP_NORMAL_EXIT,
    BUZZER_BLE_IN_DANGEROUS,
    BUZZER_BLE_AlERT,
		BUZZER_BLE_IDLE
};

enum buzz_music_choice	
{
		MUSIC_ALERT = 1,
		MUSIC_START,
		MUSIC_END,
		MUSIC_OFF
};

/*
 * STRUCT DEFINITIONS
 ****************************************************************************************
 */

struct buzz_env_tag
{   
    //enum buzz_vol volume;         
    enum buzz_status  status;       //buzzer work state
    enum buzz_music_choice	music_choice;           //
		//uint8_t priod;          //alert_mode   long alert or short alert
		///////music mode
		//uint8_t note_count;		//usr for control play music
		uint8_t meter_Count;
		//////count mode
    //uint8_t count;          //times count    
};

extern struct buzz_env_tag buzz_env;

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
void	buzzer_music_on(uint16_t priod_width,uint16_t pluse_width);


/*
 * EXTERN FUNCTION DECLARATIONS
 ****************************************************************************************
 */
extern 	int usr_buzzer_music_timer_handle(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id, ke_task_id_t const src_id);
extern	void 	usr_buzzer_config(uint8_t buzz_state,uint8_t music_choice);
extern 	int usr_buzz_process_timer_handle(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id, ke_task_id_t const src_id);

#endif
