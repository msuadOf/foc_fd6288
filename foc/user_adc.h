//
// Created by 21312 on 2021/10/12.
//

#ifndef SVPWM_USER_ADC_H
#define SVPWM_USER_ADC_H
#include "header.h"
#define used_adc hadc1
void my_adc_init(void);
void adc_getdata(FOC*foc);
#endif //SVPWM_USER_ADC_H
