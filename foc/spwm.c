//
// Created by 21312 on 2021/10/2.
//
#include "spwm.h"
int duty_h_a,
        duty_h_b,
        duty_h_c,
        duty_l_a,
        duty_l_b,
        duty_l_c;
void spwm_init(void)
{
    HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
    HAL_TIMEx_PWMN_Start(&htim1,TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);
    HAL_TIMEx_PWMN_Start(&htim1,TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_3);
    HAL_TIMEx_PWMN_Start(&htim1,TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_4);  //ADC INJECT TIM
}
void set_pwm(int out_a,int out_b,int out_c)
{
    TIM1->CCR1=out_a;
    TIM1->CCR2=out_b;
    TIM1->CCR3=out_c;
}