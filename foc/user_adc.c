//
// Created by 21312 on 2021/10/12.
//
#include "user_adc.h"
uint32_t adc_data[3];
void my_adc_init(void)
{
    __HAL_ADC_ENABLE_IT(&hadc1,ADC_IT_JEOC);
    HAL_ADC_Start_DMA(&used_adc,adc_data+2,1);
    HAL_ADCEx_InjectedStart_IT(&hadc1);
}
void adc_getdata(FOC*foc)
{
//    foc->adc.so1=(float)adc_data[0]*3.3f/4096.0f-foc->adc.so1_offset;
//    foc->adc.so2=(float)adc_data[1]*3.3f/4096.0f-foc->adc.so2_offset;
    foc->adc.pvdd_sense=(float)adc_data[2]*3.3f/4096.0f;
}

