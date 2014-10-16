
#include "usr_bass.h"
#include "adc.h"
#include "analog.h"
#include "lib.h"

/**
 ****************************************************************************************
 * @brief callback of adc sample complete.
 ****************************************************************************************
 */
void adc_sample_complete_callback(void)
{
    ke_evt_set(1UL << EVENT_ADC_SAMPLE_CMP_ID);
}

#ifdef CFG_PRF_BASS
int app_bass_batt_level_timer_handler(ke_msg_id_t const msgid,
                                      void *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id)
{
    // Battery monitor enable
    battery_monitor_enable(MASK_ENABLE);

    // ADC initialize
    adc_init(ADC_SINGLE_WITH_BUF_DRV, ADC_CLK_1000000, ADC_INT_REF, ADC_12BIT);

    // Enable ADC decimation
    adc_decimation_enable(DECI_RATE_64, MASK_ENABLE);

    // Read voltage. use interrupt
    adc_read_configuration read_cfg;
    read_cfg.trig_src = ADC_TRIG_SOFT;
    read_cfg.mode = CONTINUE_MOD;
    read_cfg.start_ch = BATT;
    read_cfg.end_ch = BATT;
    adc_read(&read_cfg, usr_env.bas_reg_buf, BASS_SAMPLE_NUMBER, adc_sample_complete_callback);


    // Restart update timer
    ke_timer_set(APP_BASS_BATT_LEVEL_TIMER, TASK_APP, APP_BASS_BATT_LEVEL_TO);

    return (KE_MSG_CONSUMED);
}
//#endif


/**
 ****************************************************************************************
 * @brief ADC sample complete handler
 ****************************************************************************************
 */
void app_event_adc_sample_cmp_handler(void)
{
    int     bas_average_adc_value = 0;
    int     bas_voltage;
    uint8_t bas_percentage;
    int     i;

    ke_evt_clear(1UL << EVENT_ADC_SAMPLE_CMP_ID);

    // Close ADC and battery monitor
    battery_monitor_enable(MASK_DISABLE);
    adc_clock_off();
    adc_power_off();

    // Calculate average value
    for(i = 0; i < BASS_SAMPLE_NUMBER; ++i)
        bas_average_adc_value += usr_env.bas_reg_buf[i];
    bas_average_adc_value /= BASS_SAMPLE_NUMBER;

    // When enable ADC decimation, the adc value should div 4
    bas_average_adc_value /= 4;
    
    // bas voltage is 4 times
    bas_voltage = 4 * ADC_RESULT_mV(bas_average_adc_value);

    // Calculate the percentage of remaining battery
    if(bas_voltage <= BASS_FULLY_DISCHARGED_VOLTAGE)
    {
        bas_percentage = 0;
    }
    else if(bas_voltage >= BASS_FULLY_CHARGED_VOLTAGE)
    {
        bas_percentage = 100;
    }
    else
    {
        bas_percentage = (uint8_t)((bas_voltage - BASS_FULLY_DISCHARGED_VOLTAGE) * 100 /
                         (BASS_FULLY_CHARGED_VOLTAGE - BASS_FULLY_DISCHARGED_VOLTAGE));
    }

    QPRINTF("battery:%dmv(%d%%) adc:%d\r\n", bas_voltage, bas_percentage, bas_average_adc_value);

    if (app_bass_env->ntf_sending == false)
    {
        // Update the percentage of remaining battery via bluetooth
        app_bass_batt_level_upd_req(app_bass_env->conhdl, 0, bas_percentage);
        app_bass_env->ntf_sending = true;
    }
}
#endif
