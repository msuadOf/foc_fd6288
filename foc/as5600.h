//
// Created by 21312 on 2021/10/3.
//

#ifndef SVPWM_AS5600_H
#define SVPWM_AS5600_H
#include "header.h"
#define used_i2c hi2c1
#define AS5600_OK 0X00
#define AS5600_ERROR 0X01
#define AS5600_ADD 0X36<<1

//Configuration Registers
#define ZMCO_REG 0X00
#define ZPOS_REG_H 0X01
#define ZPOS_REG_L 0X02
#define MPOS_REG_H 0X03
#define MPOS_REG_L 0X04
#define MANG_REG_H 0X05
#define MANG_REG_L 0X06
#define CONF_REG_H 0X07
#define CONF_REG_L 0X08

//Output Registers
#define ROWANGLE_REG_H 0X0C
#define ROWANGLE_REG_L 0X0D
#define ANGLE_REG_H 0X0E
#define ANGLE_REG_L 0X0F

//Status Registers
#define STATUS_REG 0X0B
#define AGC_REG 0X1A
#define MAGNITUDE_REG_H 0X1B
#define MAGNITUDE_REG_L 0X1C

//Burn Commands
#define BURN_ERG 0XFF

uint8_t as5600_init(void);
float as5600_read_angle(FOC*foc);
void HAL_IIC_BusyErrorHandler(void);
void My_IICBusyErrorGPIOInit(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
#endif //SVPWM_AS5600_H
