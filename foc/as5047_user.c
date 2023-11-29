//
// Created by 21312 on 2022/4/10.
//
#include "as5047_user.h"
float as5047_read_angle(FOC*foc)
{
    uint16_t out;
    out=AS5047_ReadData(0,ANGLECOM_AS5047P_VOL_REG_ADD);
    foc->angle=((float)(out))/16384.0f*360*POLE;
    foc->angle= fmodf(foc->angle,360)-foc->start_angle;
    if(foc->angle<0)
        foc->angle+=360;
    if(foc->angle>360)
        foc->angle-=360;
    return fmodf(((float)(out))*360.0f/4096.0f,360);
}