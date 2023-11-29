//
// Created by 21312 on 2021/10/2.
//

#ifndef SVPWM_SPWM_H
#define SVPWM_SPWM_H
#include "header.h"
#define INH_A_TIM htim3
#define INH_A_CH TIM_CHANNEL_1
#define INL_A_TIM htim3
#define INL_A_CH TIM_CHANNEL_2
#define INH_B_TIM htim4
#define INH_B_CH TIM_CHANNEL_1
#define INL_B_TIM htim4
#define INL_B_CH TIM_CHANNEL_2
#define INH_C_TIM htim4
#define INH_C_CH TIM_CHANNEL_3
#define INL_C_TIM htim4
#define INL_C_CH TIM_CHANNEL_4
void spwm_init(void);
void set_pwm(int out_a,int out_b,int out_c);
#endif
