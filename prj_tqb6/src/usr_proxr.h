#ifndef __USR_PROXR_H__
#define __USR_PROXR_H__


#include "app_env.h"

extern int app_proxr_alert_to_handler(ke_msg_id_t const msgid,
                               void *param,
                               ke_task_id_t const dest_id,
                               ke_task_id_t const src_id);
															 
extern void usr_proxr_alert(struct proxr_alert_ind *param);															 


#endif
