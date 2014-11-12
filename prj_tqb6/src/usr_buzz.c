/**
 ****************************************************************************************
 *
 * @file usr_buzz.c
 *
 * @brief buzzer driver for tqb6.
 *
 * Copyright (C) TChip 2014
 *
 * $Rev: 1.0 $
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup  BUZZ
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
 
#include	"usr_buzz.h"
#include 	"buzz.h"

/*
 * MICRO DEFINITIONS
 ****************************************************************************************
 */
#define	N07P			2024
#define	N1P				1912
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
#define	N15P			637

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
 
/*
 * CONST VARIABLE DEFINITIONS
 ****************************************************************************************
 */
const 	uint16_t	cano1[136];
const 	uint16_t	cano2[136];
const 	uint16_t	start1[8];
const 	uint16_t	start2[8];
const 	uint16_t	end1[8];
const 	uint16_t	end2[8];

struct buzz_env_tag buzz_env= {BUZZER_BLE_IDLE,MUSIC_OFF,0};

/**
 ****************************************************************************************
 * @brief   Switch buzzer music on
 * @param[in]    priod_width
*								 pulse_width	//this two control the note of music
 ****************************************************************************************
 */
int usr_buzzer_music_timer_handle(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
		//QPRINTF("music_choice : %d  meter_Count:%d \r\n",buzz_env.music_choice,buzz_env.meter_Count);
		switch(buzz_env.music_choice)
		{
				case	MUSIC_ALERT		:
							buzzer_music_on(cano1[buzz_env.meter_Count],cano2[buzz_env.meter_Count]);	
							if (buzz_env.meter_Count < MUSIC_ALERT_NB)
							{
								buzz_env.meter_Count++;
							}
							else
							{
								buzz_env.meter_Count = 0;
							}
							break;
				case	MUSIC_START		:
							buzzer_music_on(start1[buzz_env.meter_Count],start2[buzz_env.meter_Count]);
							if (buzz_env.meter_Count < MUSIC_START_NB)
							{
								buzz_env.meter_Count++;
							}
							else
							{
								buzz_env.meter_Count = 0;
								QPRINTF("debug6\r\n");
								usr_buzzer_config(BUZZER_BLE_IDLE,MUSIC_OFF);
							}
							break;
				case	MUSIC_END			:
							buzzer_music_on(end1[buzz_env.meter_Count],end2[buzz_env.meter_Count]);
							QPRINTF("buzz_env.meter_Count : %d\r\n",buzz_env.meter_Count);
							if (buzz_env.meter_Count < MuSIC_END_NB)
							{
								buzz_env.meter_Count++;
							}
							else
							{
								buzz_env.meter_Count = 0;
								QPRINTF("debug7\r\n");
								usr_buzzer_config(BUZZER_BLE_IDLE,MUSIC_OFF);
							}
							break;
				default	:
							break;
		}
		if (buzz_env.status != BUZZER_BLE_IDLE)
		ke_timer_set(USR_BUZZER_MUSIC_TIMER,TASK_USR,16);
		else
		ke_timer_clear(USR_BUZZER_MUSIC_TIMER,TASK_USR);
		return (KE_MSG_CONSUMED);
}

/**
 ****************************************************************************************
 * @brief   Switch buzzer music on
 * @param[in]    priod_width
 *								 pulse_width	//this two control the note of music
 ****************************************************************************************
 */
void buzzer_music_on(uint16_t priod_width,uint16_t pulse_width)
{
		pwm_init(PWM_CH0);
    pwm_config(PWM_CH0, PWM_PSCAL_DIV, PWM_COUNT_US(priod_width, PWM_PSCAL_DIV), PWM_COUNT_US(pulse_width, PWM_PSCAL_DIV));
    pwm_enable(PWM_CH0, MASK_ENABLE);
}

/**
 ****************************************************************************************
 * @brief   Switch buzzer music on
 * @param[in]    priod_width
 *								 pulse_width	//this two control the note of music
 ****************************************************************************************
 */
int usr_buzz_process_timer_handle(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
		{
				if((pwm_pwm_GetCR(QN_PWM) & PWM_CH0))
				{
						buzzer_off();
				}
				else
				{
						buzzer_on();//buzz_env.volume);
						//ke_timer_set(USR_BUZZER_TIMER,TASK_USR,buzz_env.priod);
				}
		}
    return(KE_MSG_CONSUMED);
}

/**
 ****************************************************************************************
 * @brief   Switch buzzer music on
 * @param[in]    buzz_state:
 *				  				BUZZER_BLE_CON = 1,
 *									BUZZER_BLE_DISCON,
 *									BUZZER_APP_NORMAL_EXIT,
 *									BUZZER_BLE_IN_DANGEROUS,
 *									BUZZER_BLE_AlERT,
 *									BUZZER_BLE_IDLE
 ****************************************************************************************
 */
void usr_buzzer_config( uint8_t buzz_state,uint8_t music_choice)
{
    QPRINTF("buzz_state : %d  music_choice:%d \r\n",buzz_state,music_choice);
		switch(buzz_state)
		{
				case	BUZZER_BLE_CON	:
							if (buzz_env.status !=BUZZER_BLE_CON)
							{
									buzz_env.status = BUZZER_BLE_CON;
									buzz_env.music_choice = MUSIC_START;
									buzz_env.meter_Count = 0;
							}
							break;
				case	BUZZER_BLE_DISCON	:
							if (buzz_env.status !=BUZZER_BLE_DISCON)
							{
									buzz_env.status = BUZZER_BLE_DISCON;
									buzz_env.music_choice = MUSIC_ALERT;
									buzz_env.meter_Count = 0;
							}
							break;
				case	BUZZER_APP_NORMAL_EXIT	:
							if (buzz_env.status !=BUZZER_APP_NORMAL_EXIT)
							{
									buzz_env.status = BUZZER_APP_NORMAL_EXIT;
									buzz_env.music_choice = MUSIC_END;
									buzz_env.meter_Count = 0;
							}
							break;
				case	BUZZER_BLE_IN_DANGEROUS	:
							if (buzz_env.status !=BUZZER_BLE_IN_DANGEROUS)
							{
									buzz_env.status = BUZZER_BLE_IN_DANGEROUS;
									buzz_env.music_choice = MUSIC_ALERT;
									buzz_env.meter_Count = 0;
							}
							break;
				case	BUZZER_BLE_AlERT	:
							if (buzz_env.status !=BUZZER_BLE_AlERT)
							{
									buzz_env.status = BUZZER_BLE_AlERT;
									buzz_env.music_choice = MUSIC_ALERT;
									buzz_env.meter_Count = 0;
							}
							break;
				case	BUZZER_BLE_IDLE	:
							if (buzz_env.status !=BUZZER_BLE_IDLE)
							{
									buzz_env.status = BUZZER_BLE_IDLE;
									buzz_env.music_choice = MUSIC_OFF;
									buzzer_off();
									buzz_env.meter_Count = 0;
							}
							break;
				default	:
					break;
		}
		if (buzz_env.status !=BUZZER_BLE_IDLE)
			ke_timer_set(USR_BUZZER_MUSIC_TIMER,TASK_USR,1);
		else
			ke_timer_clear(USR_BUZZER_MUSIC_TIMER,TASK_USR);
}

const 	uint16_t	cano1[136] = { N15P, N15P, N13P, N14P, N15P, N15P, N13P, N14P,
																 N15P,  N5P,  N6P,  N7P, N11P, N12P, N13P, N14P,
																 N13P, N13P, N11P, N12P, N13P, N13P,  N3P,  N4P,
																  N5P,  N6P,  N5P,  N4P,  N5P,  N3P,  N4P,  N5P,
																  N4P,  N4P,  N6P,  N5P,  N4P,  N4P,  N3P,  N2P,
																  N3P,  N2P,  N1P,  N2P,  N3P,  N4P,  N5P,  N6P,
																  N6P,  N6P,  N4P,  N5P,  N6P,  N6P,  N7P, N11P,
																  N5P,  N6P,  N7P, N11P, N12P, N13P, N14P, N15P,
																 N13P, N13P, N11P, N12P, N13P, N13P, N12P, N11P,
																 N12P,  N7P, N11P, N12P, N13P, N12P, N11P,  N7P,
																 N11P, N11P,  N6P,  N7P, N11P, N11P,  N1P,  N2P,
																  N3P,  N4P,  N3P,  N2P,  N3P, N11P,  N7P, N11P,
																  N6P,  N6P, N11P,  N7P,  N6P,  N6P,  N5P,  N4P,
																  N5P,  N4P,  N3P,  N4P,  N5P,  N6P,  N7P, N11P,
																  N6P,  N6P, N11P,  N7P, N11P, N11P,  N7P,  N6P,
																  N7P, N11P, N12P, N11P,  N7P, N11P,  N6P,  N7P,
																 N11P, N11P, N11P, N11P, N11P, N11P,   0,   0 };

const 	uint16_t	cano2[136] = {	N15I, N15I, N13I, N14I, N15I, N15I, N13I, N14I,
																 N15I,  N5I,  N6I,  N7I, N11I, N12I, N13I, N14I,
																 N13I, N13I, N11I, N12I, N13I, N13I,  N3I,  N4I,
																  N5I,  N6I,  N5I,  N4I,  N5I,  N3I,  N4I,  N5I,
																  N4I,  N4I,  N6I,  N5I,  N4I,  N4I,  N3I,  N2I,
																  N3I,  N2I,  N1I,  N2I,  N3I,  N4I,  N5I,  N6I,
																  N6I,  N6I,  N4I,  N5I,  N6I,  N6I,  N7I, N11I,
																  N5I,  N6I,  N7I, N11I, N12I, N13I, N14I, N15I,
																 N13I, N13I, N11I, N12I, N13I, N13I, N12I, N11I,
																 N12I,  N7I, N11I, N12I, N13I, N12I, N11I,  N7I,
																 N11I, N11I,  N6I,  N7I, N11I, N11I,  N1I,  N2I,
																  N3I,  N4I,  N3I,  N2I,  N3I, N11I,  N7I, N11I,
																  N6I,  N6I, N11I,  N7I,  N6I,  N6I,  N5I,  N4I,
																  N5I,  N4I,  N3I,  N4I,  N5I,  N6I,  N7I, N11I,
																  N6I,  N6I, N11I,  N7I, N11I, N11I,  N7I,  N6I,
																  N7I, N11I, N12I, N11I,  N7I, N11I,  N6I,  N7I,
																 N11I, N11I, N11I, N11I, N11I, N11I,   0,   0 };

const 	uint16_t	start1[8] = {   N5P, N6P, N7P, N11P, N11P, N11P, N11P, 0,
};

const 	uint16_t	start2[8] = {   N5I, N6I, N7I, N11I, N11I, N11I, N11I, 0,
};

const 	uint16_t	end1[8] = {   N4P, N3P, N2P, N1P, N1P, N1P, N1P, 0,
};

const 	uint16_t	end2[8] = {   N4I, N3I, N2I, N1I, N1I, N1I, N1I, 0,
};
