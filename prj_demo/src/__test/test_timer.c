
/********************************
 *  ²âÊÔ¶¨Ê±Æ÷Á·Ï°  2014-08-15
 ********************************/

#include "test_timer.h"

void app_tchip_send_package(tchip_frame_buff_t *data_frame_buff);

uint8_t tem_value = 'a';

void stop_tchip_test_timer()
{
	ke_timer_clear(APP_TCHIP_TEST_TIMER, TASK_APP);
}

void start_tchip_test_timer(void const *param)
{
	struct qpps_cfg_indntf_ind * mparam = (struct qpps_cfg_indntf_ind *)param ;//wengbj
	QPRINTF("---->char_index:0x%x \t\n",mparam->char_index);
	stop_tchip_test_timer();
	if (mparam->cfg_val == PRF_CLI_START_NTF)
	{
		ke_timer_set(APP_TCHIP_TEST_TIMER, TASK_APP, 100);
	}
}

int app_tchip_test_timer_handler(ke_msg_id_t const msgid, void const *param,
                               ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    if(msgid == APP_TCHIP_TEST_TIMER)
    {
        QPRINTF("helloworld \r\n");
        uint8_t str[] = "hi";
			
        app_qpps_data_send(app_qpps_env->conhdl, 0, sizeof(str), str);
        app_qpps_data_send(app_qpps_env->conhdl, 1, sizeof(str), str);
        
			
        ke_timer_set(APP_TCHIP_TEST_TIMER, TASK_APP, 100);
    }

    return (KE_MSG_CONSUMED);
}

void app_tchip_send_package(tchip_frame_buff_t *data_frame_buff)
{
	QPRINTF("app_tchip_send_package SEND %d \r\n",data_frame_buff->len);
		if(data_frame_buff->len) //have data
		{    
				//calculate page num;
			uint8_t pagket_res = data_frame_buff->len%QPP_DATA_MAX_LEN;
			uint8_t pagket_num;
			if(pagket_res)
				pagket_num = data_frame_buff->len/QPP_DATA_MAX_LEN + 1;
			else
				pagket_num = data_frame_buff->len/QPP_DATA_MAX_LEN;
				 
			uint8_t cnt=0,sent_pagket=0; 

			QPRINTF("app_qpps_env->tx_char_num %d \r\n",app_qpps_env->tx_char_num);
			for (cnt = 0; (sent_pagket<pagket_num) && cnt < app_qpps_env->tx_char_num ; cnt++)
			{
				QPRINTF("aapp_qpps_env->char_status %d \r\n",app_qpps_env->char_status);
				if ((app_qpps_env->char_status >> cnt) & QPPS_VALUE_NTF_CFG)
				{
					app_qpps_env->char_status &= ~(QPPS_VALUE_NTF_CFG << cnt);
							 
					if((pagket_res)&&(pagket_num-sent_pagket==1))
						app_qpps_data_send(app_qpps_env->conhdl, cnt, pagket_res, (data_frame_buff->data+sent_pagket*20));
					else
						app_qpps_data_send(app_qpps_env->conhdl, cnt, QPP_DATA_MAX_LEN, (data_frame_buff->data+sent_pagket*20)); 
						
						sent_pagket++;
				}
			}
		}
		QPRINTF("\r\n");
}


