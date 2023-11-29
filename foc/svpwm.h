//
// Created by 21312 on 2021/10/6.
//
#include "spwm.h"
#include "header.h"
#ifndef SVPWM_SVPWM_H
#define SVPWM_SVPWM_H

#define U_dc 12
#define Ts 0.001
#define SECTION1 3
#define SECTION2 1
#define SECTION3 5
#define SECTION4 4
#define SECTION5 6
#define SECTION6 2

void svpwm_init(void);
uint8_t judge_section(FOC*foc);
void svpwm_process(FOC*foc);
void svpwm_process2(FOC*foc);
#endif //SVPWM_SVPWM_H
