//
// Created by 21312 on 2021/10/3.
//

#ifndef SVPWM_FOC_H
#define SVPWM_FOC_H
#include "as5600.h"
#include "spwm.h"
#include "svpwm.h"
#include "header.h"
#include "user_adc.h"
#include "as5047_user.h"
#define FOC_INIT_OK 0X00
#define DRV8303_INIT_ERROR 0X01
#define AS5600_INIT_ERROR 0X02

#define MODE_LOOP_SPEED 0X00
#define MODE_LOOP_POS 0X01
#define MODE_LOOP_OPEN 0X02

uint8_t foc_init(FOC*foc);
void foc_zero_offset(FOC *foc);
void foc_loop(FOC *foc,float aim_q,float aim_d,float aim_p);
void current_pi_process(FOC*foc,float aim_d,float aim_q);
float position_pi_process(FOC*foc,float aim_position);
void speed_pi_process(FOC*foc,float aim_w);
void clarke_transform(FOC*foc);
void park_transform(FOC*foc);
void inverse_park_transform(FOC*foc);
void inverse_clarke_transform(FOC*foc);
#endif //SVPWM_FOC_H
