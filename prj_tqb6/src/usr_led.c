#include "usr_led.h"
#include "led.h"
#include "pwm.h"
#include "gpio.h"
#include "lib.h"

//t-chip
#if (defined(LED_BREATH))
uint8_t led_breath_array[] = {25,50,74,97,120,142,162,181,197,212,225,236,245,251,254,255,254,
                              251,244,236,225,212,197,181,162,142,120,97,74,50,25,0};//the sin(x) array for LED_BREATH

void led_breath_on(int volume)
{
    QN_SYSCON->PMCR1 &= ~(P26_MASK_PIN_CTRL);
    QN_SYSCON->PMCR1 |= P26_PWM1_PIN_CTRL;//open the LED_BREATH need config P26 to PWM_CH1
    pwm_init(PWM_CH1);
	pwm_config(PWM_CH1, PWM_PSCAL_DIV, PWM_COUNT_US(LED_PWM_PERIOD, PWM_PSCAL_DIV), PWM_COUNT_US(volume, PWM_PSCAL_DIV));
	pwm_enable(PWM_CH1, MASK_ENABLE);		
}


void led_breath_off(void)
{
    pwm_enable(PWM_CH1, MASK_DISABLE);
    QN_SYSCON->PMCR1 &= ~(P26_MASK_PIN_CTRL);//close LED_BREATH to config P26 reback.
    QN_SYSCON->PMCR1 |= P26_GPIO_22_PIN_CTRL;
}
#endif

#if (defined(LED_FLASH))
/**
 ****************************************************************************************
 * @brief   Led 1 flash process
 ****************************************************************************************
 */
void usr_led1_process(void)
{
    if(led_get(1) == LED_ON)
    {
        led_set(1, LED_OFF);
        ke_timer_set(APP_SYS_LED_1_TIMER, TASK_APP, usr_env.led1_off_dur);
    }
    else
    {
        led_set(1, LED_ON);
        ke_timer_set(APP_SYS_LED_1_TIMER, TASK_APP, usr_env.led1_on_dur);
    }
}

/**
 ****************************************************************************************
 * @brief   Led1 for BLE status
 ****************************************************************************************
 */
void usr_led1_set(uint16_t timer_on, uint16_t timer_off)
{
    usr_env.led1_on_dur = timer_on;
    usr_env.led1_off_dur = timer_off;

    if (timer_on == 0 || timer_off == 0)
    {
        if (timer_on == 0)
        {
            led_set(1, LED_OFF);
        }
        if (timer_off == 0)
        {
            led_set(1, LED_ON);
        }
        ke_timer_clear(APP_SYS_LED_1_TIMER, TASK_APP);
    }
    else
    {
        led_set(1, LED_OFF);
        ke_timer_set(APP_SYS_LED_1_TIMER, TASK_APP, timer_off);
    }
}

#endif

