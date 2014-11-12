#ifndef __USR_TASK_H__
#define __USR_TASK_H__

#include "app_env.h"

#undef TASK_USR
#define TASK_USR    TASK_USER

#define USR_IDX_MAX                                 0x01

/// states of USR task
enum
{
    /// Init state
    USR_DISABLE,
    /// Idle state
    USR_IDLE,
    /// Scan state
    USR_CONNECT,
    /// Number of defined states.
    USR_STATE_MAX
};

/// Usr Message Interface
enum
{
    	//tchip
		USR_ALERT_STOP_TIMER,
    USR_BUZZER_TIMER,
		USR_BUZZER_MUSIC_TIMER,
		USR_LED_STOP_TIMER,
	//USR_TCHIP_TEST_TIMER,
#if (defined(LED_BREATH))
    USR_LED_BREATH,
#endif
    USR_KEY_ST,
    USR_MSG_MAX,
};

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
extern const struct ke_state_handler usr_state_handler[USR_STATE_MAX];

extern const struct ke_state_handler usr_default_handler;

extern ke_state_t usr_state[USR_IDX_MAX];

extern void task_usr_desc_register(void);

#endif  
//end of usr_task.h
