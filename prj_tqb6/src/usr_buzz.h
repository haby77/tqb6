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

//impulse of note * 2 = priod of note
#define	N07I			506//1012
#define	N1I				478//956
#define	N2I				425//851
#define	N3I				379//758
#define	N4I				358//716
#define	N5I				318//637
#define	N6I				284//568
#define	N7I				253//506
#define	N11I			239//478
#define	N12I			212//425
#define	N13I			189//379
#define	N14I			178//357
#define	N15I			159//318
#define	N16I			142//284
#define N17I			126//253
#define	N21I			119//239

//priod of note 
#define	N07P			(N07I*2)			//note low 7
#define	N1P				(N1I*2)			//note mid 1
#define	N2P				(N2I*2)
#define	N3P				(N3I*2)
#define	N4P				(N4I*2)
#define	N5P				(N5I*2)
#define	N6P				(N6I*2)
#define	N7P				(N7I*2)
#define	N11P			(N11I*2)
#define	N12P			(N12I*2)
#define	N13P			(N13I*2)
#define	N14P			(N14I*2)
#define	N15P			(N15I*2)
#define N16P			(N16I*2)
#define N17P			(N17I*2)
#define N21P			(N21I*2)
//#define N16P			(N16I*2)

#define		MUSIC_ALERT_NB			36
#define		MUSIC_START_NB			16
#define 	MUSIC_END_NB				16
#define		MUSIC_MERTE_PRIOD		4


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
