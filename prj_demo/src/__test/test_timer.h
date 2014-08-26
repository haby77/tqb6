#ifndef __TEST_TIMER_H__
#define __TEST_TIMER_H__

#include "app_env.h"

typedef struct tchip_frame_buff
{
	uint8_t len;
	uint8_t *data;
}tchip_frame_buff_t;

extern int app_tchip_test_timer_handler(ke_msg_id_t const msgid, void const *param,
                               ke_task_id_t const dest_id, ke_task_id_t const src_id);
extern void stop_tchip_test_timer(void);
extern void start_tchip_test_timer(void const *param);
#endif
