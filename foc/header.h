//
// Created by 21312 on 2021/10/7.
//

#ifndef SVPWM_HEADER_H
#define SVPWM_HEADER_H
typedef struct PID
{
    float k_p;
    float k_i;
    float k_d;
    float error;
    float last_error;
    float sum_error;
    float change_error;
}PID;
typedef struct svpwm
{
    float rad;
    float angle;
    float tx;
    float ty;
    float Ta;
    float Tb;
    float Tc;
    float duty_a;
    float duty_b;
    float duty_c;
}svm_vector;
typedef struct adc
{
    float so1;
    float so2;
    float so1_offset;
    float so2_offset;
    float asense;
    float bsense;
    float csense;
    float pvdd_sense;
    float enc_adc;

}adc;
typedef struct FOC
{
    float angle;
    float sin_val;
    float cos_val;
    float start_angle;
    float last_angle;
    float position;
    float last_position;
    float round;
    float i_a;
    float i_b;
    float i_c;
    float i_alpha;
    float i_beta;
    float i_d;
    float i_q;
    PID Iq_pi;
    PID Id_pi;
    PID Pos_pid;
    PID Speed_pid;
    float u_d;
    float u_q;
    float u_alpha;
    float u_beta;
    float u_a;
    float u_b;
    float u_c;
    float speed_w;
    float last_speed;
    float aim_w;
    svm_vector svm_v;
    adc adc;
}FOC;
#define SQRT3 1.7320508075688773
#define Pi 3.1415926
#define POLE 7
#define delta_t 0.004
#include "tim.h"
#include "i2c.h"
#include "math.h"
#include "adc.h"
#include "arm_math.h"
#include "main.h"
#include "usart.h"
#include "AS5047P.h"
#endif //SVPWM_HEADER_H
