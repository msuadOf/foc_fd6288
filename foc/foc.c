//
// Created by 21312 on 2021/10/3.
//
#include "foc.h"
FOC motor;
extern uint32_t adc_data[3];
uint8_t foc_init(FOC*foc)
{
    //encoder_init
//    if(as5600_init()==AS5600_OK)
//        UART_printf(&huart1,"AS_OK\n\n");
//    else
//    {
//        while(as5600_init()!=AS5600_OK)
//        {
//            UART_printf(&huart1,"err\n\n");
//            HAL_IIC_BusyErrorHandler();
//        }
//    }
    AS5047_Init();
    //adc_init
    my_adc_init();
    svpwm_init();
    return FOC_INIT_OK;
}
void foc_zero_offset(FOC *foc)
{
    //pos offset
    //angle_offset
    foc->u_alpha=U_dc*0.3f;
    foc->u_beta=0;
    svpwm_process(foc);
    set_pwm((int)foc->svm_v.duty_a,(int)foc->svm_v.duty_b,(int)foc->svm_v.duty_c);
    HAL_Delay(500);
    as5047_read_angle(foc);
    foc->start_angle=foc->angle;
    //current offset
    set_pwm(0,0, 0); //关闭上桥，打开下桥 校准电流
    TIM1->CCR4=500;
    HAL_Delay(200);
    motor.adc.so1_offset=(float)adc_data[0]*3.3f/16384.0f-1.65f;
    motor.adc.so2_offset=(float)adc_data[1]*3.3f/16384.0f-1.65f;
}
void foc_loop(FOC *foc,float aim_q,float aim_d,float aim_p)
{
    //encoder_get_angle
   // as5600_read_angle(foc);
    float position_temp=as5047_read_angle(foc);
    if(position_temp<20&&foc->last_position>340)
        foc->round+=1;
    else if(position_temp>340&&foc->last_position<20)
        foc->round-=1;
    else
    {
        foc->speed_w=(position_temp-foc->last_position)*10000/360.0f;
        foc->speed_w=foc->speed_w*0.4f+foc->last_speed*0.6f;
        foc->last_speed=foc->speed_w;
    }
    foc->position=foc->round*360+position_temp;
    foc->last_position=position_temp;
    //dsp
    arm_sin_cos_f32(foc->angle,&foc->sin_val,&foc->cos_val);
    //adc_get_data
    adc_getdata(foc);
    //current
    motor.i_a=1*(1.65f-motor.adc.so1)/0.002f/200.0f;
    motor.i_b=1*(1.65f-motor.adc.so2)/0.002f/200.0f;
    motor.i_c=-1*motor.i_b-motor.i_a;
    //clarke
    //clarke_transform(foc);
    //park
   // park_transform(foc);
    //pi_control
    //aim_q=position_pi_process(foc,aim_p);
    current_pi_process(foc,aim_d,aim_q);
    foc->u_q=3;
    foc->u_d=0;
    //inverse park
    inverse_park_transform(foc);
    //inverse clarke
    //inverse_clarke_transform(foc);
    //calculate duty
    svpwm_process(foc);


}
void current_pi_process(FOC*foc,float aim_d,float aim_q)
{
    foc->Id_pi.error=aim_d-foc->i_d;
    foc->Id_pi.sum_error+=foc->Id_pi.error;
    if(fabsf(foc->Id_pi.sum_error)>800)
        foc->Id_pi.sum_error=foc->Id_pi.sum_error>0?800:-800;
    foc->u_d=foc->Id_pi.k_p* foc->Id_pi.error+foc->Id_pi.k_i* foc->Id_pi.sum_error;
    if(fabsf(foc->u_d)>2)
        foc->u_d=foc->u_d>0?2:-2;
    foc->Iq_pi.error=aim_q-foc->i_q;
    foc->Iq_pi.sum_error+=foc->Iq_pi.error;
    if(fabsf(foc->Iq_pi.sum_error)>800)
        foc->Iq_pi.sum_error=foc->Iq_pi.sum_error>0?800:-800;
    foc->u_q=foc->Iq_pi.k_p* foc->Iq_pi.error+foc->Iq_pi.k_i* foc->Iq_pi.sum_error;
    if(fabsf(foc->u_q)>2)
        foc->u_q=foc->u_q>0?2:-2;
}
float position_pi_process(FOC*foc,float aim_position)
{
    foc->Pos_pid.error=aim_position-foc->position;
    foc->Pos_pid.sum_error+=foc->Pos_pid.error;
    if(fabsf(foc->Pos_pid.sum_error)>1600)
        foc->Pos_pid.sum_error=foc->Pos_pid.sum_error>0?1600:-1600;
    if(fabsf(foc->Pos_pid.k_p* foc->Pos_pid.error+foc->Pos_pid.k_i* foc->Pos_pid.sum_error)>1.3f)
        return (foc->Pos_pid.k_p* foc->Pos_pid.error+foc->Pos_pid.k_i* foc->Pos_pid.sum_error)>0?1.3f:-1.3f;
    else
        return foc->Pos_pid.k_p* foc->Pos_pid.error+foc->Pos_pid.k_i* foc->Pos_pid.sum_error;
}
void speed_pi_process(FOC*foc,float aim_w)
{
    foc->Speed_pid.error=aim_w-foc->speed_w;
    foc->Speed_pid.sum_error+=foc->Speed_pid.error;
    if(fabsf(foc->Speed_pid.sum_error)>500)
        foc->Speed_pid.sum_error=foc->Speed_pid.sum_error>0?500:-500;
    foc->u_q=foc->Speed_pid.k_p* foc->Speed_pid.error+foc->Speed_pid.k_i* foc->Speed_pid.sum_error;
    if(fabsf(foc->u_q)>8)
        foc->u_q=foc->u_q>0?8:-8;
}
void clarke_transform(FOC*foc)  //3static->2static
{
    foc->i_alpha=foc->i_a;
    foc->i_beta=1*(foc->i_a+2*foc->i_b)/(float)SQRT3;
}
void park_transform(FOC*foc) //2static->2rotation
{
    foc->i_d=foc->i_alpha*foc->cos_val+foc->i_beta*foc->sin_val;
    foc->i_q=foc->i_beta*foc->cos_val-foc->i_alpha*foc->sin_val;
}
void inverse_park_transform(FOC*foc) //2rotation->2static
{
    foc->u_alpha=foc->u_d*foc->cos_val-foc->u_q*foc->sin_val;
    foc->u_beta=foc->u_d*foc->sin_val+foc->u_q*foc->cos_val;
}
void inverse_clarke_transform(FOC*foc) //2static->3static
{
    foc->u_a=foc->u_alpha;
    foc->u_b=(-1*foc->u_alpha+foc->u_beta*(float)SQRT3)/2;
    foc->u_c=(-1*foc->u_alpha-(float)SQRT3*foc->u_beta)/2;
}
