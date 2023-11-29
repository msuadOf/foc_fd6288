//
// Created by 21312 on 2021/10/3.
//
#include "as5600.h"
uint8_t as5600_init(void)
{
    uint8_t data[2]={0};
    data[0]=0b00001100;
    data[1]=0b00011111;
    if( HAL_I2C_Mem_Write(&used_i2c,AS5600_ADD,CONF_REG_H,1,&data[1],1,1000)==HAL_OK)
        ;
    else
        return AS5600_ERROR;
    if( HAL_I2C_Mem_Write(&used_i2c,AS5600_ADD,CONF_REG_L,1,&data[0],1,1000)==HAL_OK)
        ;
    else
        return AS5600_ERROR;
    return AS5600_OK;
}
float as5600_read_angle(FOC*foc)
{
    uint16_t out;
    uint8_t data[2]={0};
    HAL_I2C_Mem_Read(&used_i2c,AS5600_ADD,ROWANGLE_REG_H,1,data,2,HAL_MAX_DELAY);
    out=(data[0]<<8)|data[1];
    foc->angle=((float)(out))/4096.0f*360*POLE;
    foc->angle= fmodf(foc->angle,360)-foc->start_angle;
    if(foc->angle<0)
        foc->angle+=360;
    if(foc->angle>360)
        foc->angle-=360;
    return fmodf(((float)(out))*360.0f/4096.0f,360);
}


//Init GPIO
void My_IICBusyErrorGPIOInit(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_SET);

    /*Configure GPIO pin : PB8 */
    GPIO_InitStruct.Pin = GPIO_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);

}
//Handle busy error
//IIC Pin:
//        SCL:PA8
//        SDA:PB4
void HAL_IIC_BusyErrorHandler(void) {
    //Disable the I2C peripheral
    HAL_I2C_DeInit(&hi2c1);
    //Configure the SCL and SDA I/Os as General Purpose Output Open-Drain
    My_IICBusyErrorGPIOInit(GPIOB, GPIO_PIN_6);
    My_IICBusyErrorGPIOInit(GPIOB, GPIO_PIN_7);

    //Check SCL and SDA High level in GPIOx_IDR.
    if ((HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6) == 0)
        || (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7 == 0))) {
    }

    //Configure the SDA I/O low level
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
    //Check SDA Low level in GPIOx_IDR.
    if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7)) {
    }

    //Configure the SCL I/O low level
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
    //Check SCL Low level in GPIOx_IDR.
    if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6)) {
    }

    //Configure the SCL I/O high level
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
    //Check SCL high level in GPIOx_IDR.
    if (!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6)) {
    }

    //Configure the SDA I/O high level
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);
    //Check SDA Low level in GPIOx_IDR.
    if (!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7)) {
    }

    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_6);
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_7);
    MX_I2C1_Init();
}

