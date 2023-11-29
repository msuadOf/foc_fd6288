//
// Created by 21312 on 2021/10/6.
//
#include "svpwm.h"
void svpwm_init(void)
{
    spwm_init();
}
uint8_t judge_section(FOC*foc)
{
   static float u1,u2,u3;
   static uint8_t a,b,c;
   u1=foc->u_beta;
   u2=(foc->u_alpha*(float)SQRT3-foc->u_beta);///2;
   u3=(foc->u_alpha*(float)SQRT3+foc->u_beta)/(-2);
   if(u1>0)
       a=1;
   else
       a=0;
   if(u2>0)
        b=1;
   else
        b=0;
   if(u3>=0)
        c=1;
   else
        c=0;
   return 4*c+2*b+a;
}
void svpwm_process(FOC*foc) {
    static float m=0;
    m=2*sqrtf(powf(foc->u_alpha,2)+powf(foc->u_beta,2))/SQRT3/U_dc*0.8;

    switch (judge_section(foc))
    {
        case SECTION1:
            foc->svm_v.tx=m*(float)((float)SQRT3*Ts/U_dc*((float)SQRT3*foc->u_alpha-foc->u_beta)/2);
            foc->svm_v.ty=m*(float)((float)SQRT3*Ts/U_dc*foc->u_beta);
            if(foc->svm_v.tx+foc->svm_v.ty>Ts)
            {
                foc->svm_v.tx=Ts*foc->svm_v.tx/(foc->svm_v.tx+foc->svm_v.ty);
                foc->svm_v.ty=Ts*foc->svm_v.ty/(foc->svm_v.tx+foc->svm_v.ty);
            }
            foc->svm_v.Ta=(float)(Ts-foc->svm_v.tx-foc->svm_v.ty)/4;
            foc->svm_v.Tb=(float)(Ts+foc->svm_v.tx-foc->svm_v.ty)/4;
            foc->svm_v.Tc=(float)(Ts+foc->svm_v.tx+foc->svm_v.ty)/4;
            break;
        case SECTION2:
            foc->svm_v.tx=m*(float)((float)SQRT3*Ts/U_dc*(-1*(float)SQRT3*foc->u_alpha+foc->u_beta)/2);
            foc->svm_v.ty=m*(float)((float)SQRT3*Ts/U_dc*((float)SQRT3*foc->u_alpha+foc->u_beta)/2);
            if(foc->svm_v.tx+foc->svm_v.ty>Ts)
            {
                foc->svm_v.tx=Ts*foc->svm_v.tx/(foc->svm_v.tx+foc->svm_v.ty);
                foc->svm_v.ty=Ts*foc->svm_v.ty/(foc->svm_v.tx+foc->svm_v.ty);
            }
            foc->svm_v.Ta=(float)(Ts+foc->svm_v.tx-foc->svm_v.ty)/4;
            foc->svm_v.Tb=(float)(Ts-foc->svm_v.tx-foc->svm_v.ty)/4;
            foc->svm_v.Tc=(float)(Ts+foc->svm_v.tx+foc->svm_v.ty)/4;
//            foc->svm_v.Ta=(float)(Ts-foc->svm_v.tx-foc->svm_v.ty)/4;
//            foc->svm_v.Tb=(float)(Ts+foc->svm_v.tx-foc->svm_v.ty)/4;
//            foc->svm_v.Tc=(float)(Ts+foc->svm_v.tx+foc->svm_v.ty)/4;
            break;
        case SECTION3:
            foc->svm_v.tx=m*(float)((float)SQRT3*Ts/U_dc*foc->u_beta);
            foc->svm_v.ty=m*(float)(-1*(float)SQRT3*Ts/U_dc*((float)SQRT3*foc->u_alpha+foc->u_beta)/2);
            if(foc->svm_v.tx+foc->svm_v.ty>Ts)
            {
                foc->svm_v.tx=Ts*foc->svm_v.tx/(foc->svm_v.tx+foc->svm_v.ty);
                foc->svm_v.ty=Ts*foc->svm_v.ty/(foc->svm_v.tx+foc->svm_v.ty);
            }
            foc->svm_v.Ta=(float)(Ts+foc->svm_v.tx+foc->svm_v.ty)/4;
            foc->svm_v.Tb=(float)(Ts-foc->svm_v.tx-foc->svm_v.ty)/4;
            foc->svm_v.Tc=(float)(Ts+foc->svm_v.tx-foc->svm_v.ty)/4;
//            foc->svm_v.Ta=(float)(Ts-foc->svm_v.tx-foc->svm_v.ty)/4;
//            foc->svm_v.Tb=(float)(Ts+foc->svm_v.tx-foc->svm_v.ty)/4;
//            foc->svm_v.Tc=(float)(Ts+foc->svm_v.tx+foc->svm_v.ty)/4;
            break;
        case SECTION4:
            foc->svm_v.tx=m*(float)(-1*(float)SQRT3*Ts/U_dc*foc->u_beta);
            foc->svm_v.ty=m*(float)((float)SQRT3*Ts/U_dc*(-1*(float)SQRT3*foc->u_alpha+foc->u_beta)/2);
            if(foc->svm_v.tx+foc->svm_v.ty>Ts)
            {
                foc->svm_v.tx=Ts*foc->svm_v.tx/(foc->svm_v.tx+foc->svm_v.ty);
                foc->svm_v.ty=Ts*foc->svm_v.ty/(foc->svm_v.tx+foc->svm_v.ty);
            }
            foc->svm_v.Ta=(float)(Ts+foc->svm_v.tx+foc->svm_v.ty)/4;
            foc->svm_v.Tb=(float)(Ts+foc->svm_v.tx-foc->svm_v.ty)/4;
            foc->svm_v.Tc=(float)(Ts-foc->svm_v.tx-foc->svm_v.ty)/4;
//            foc->svm_v.Ta=(float)(Ts-foc->svm_v.tx-foc->svm_v.ty)/4;
//            foc->svm_v.Tb=(float)(Ts+foc->svm_v.tx-foc->svm_v.ty)/4;
//            foc->svm_v.Tc=(float)(Ts+foc->svm_v.tx+foc->svm_v.ty)/4;
            break;
        case SECTION5:
            foc->svm_v.tx=m*(float)(-1*(float)SQRT3*Ts/U_dc*((float)SQRT3*foc->u_alpha+foc->u_beta)/2);
            foc->svm_v.ty=m*(float)((float)SQRT3*Ts/U_dc*((float)SQRT3*foc->u_alpha-foc->u_beta)/2);
            if(foc->svm_v.tx+foc->svm_v.ty>Ts)
            {
                foc->svm_v.tx=Ts*foc->svm_v.tx/(foc->svm_v.tx+foc->svm_v.ty);
                foc->svm_v.ty=Ts*foc->svm_v.ty/(foc->svm_v.tx+foc->svm_v.ty);
            }
            foc->svm_v.Ta=(float)(Ts+foc->svm_v.tx-foc->svm_v.ty)/4;
            foc->svm_v.Tb=(float)(Ts+foc->svm_v.tx+foc->svm_v.ty)/4;
            foc->svm_v.Tc=(float)(Ts-foc->svm_v.tx-foc->svm_v.ty)/4;
//            foc->svm_v.Ta=(float)(Ts-foc->svm_v.tx-foc->svm_v.ty)/4;
//            foc->svm_v.Tb=(float)(Ts+foc->svm_v.tx-foc->svm_v.ty)/4;
//            foc->svm_v.Tc=(float)(Ts+foc->svm_v.tx+foc->svm_v.ty)/4;
            break;
        case SECTION6:
            foc->svm_v.tx=m*(float)((float)SQRT3*Ts/U_dc*((float)SQRT3*foc->u_alpha+foc->u_beta)/2);
            foc->svm_v.ty=m*(float)(-1*(float)SQRT3*Ts/U_dc*foc->u_beta);
            if(foc->svm_v.tx+foc->svm_v.ty>Ts)
            {
                foc->svm_v.tx=Ts*foc->svm_v.tx/(foc->svm_v.tx+foc->svm_v.ty);
                foc->svm_v.ty=Ts*foc->svm_v.ty/(foc->svm_v.tx+foc->svm_v.ty);
            }
            foc->svm_v.Ta=(float)(Ts-foc->svm_v.tx-foc->svm_v.ty)/4;
            foc->svm_v.Tb=(float)(Ts+foc->svm_v.tx+foc->svm_v.ty)/4;
            foc->svm_v.Tc=(float)(Ts+foc->svm_v.tx-foc->svm_v.ty)/4;
//            foc->svm_v.Ta=(float)(Ts-foc->svm_v.tx-foc->svm_v.ty)/4;
//            foc->svm_v.Tb=(float)(Ts+foc->svm_v.tx-foc->svm_v.ty)/4;
//            foc->svm_v.Tc=(float)(Ts+foc->svm_v.tx+foc->svm_v.ty)/4;
            break;
        default:
            foc->svm_v.Ta=0;
            foc->svm_v.Tb=0;
            foc->svm_v.Tc=0;
    }
    foc->svm_v.duty_a=(float)((Ts-foc->svm_v.Ta*2)/Ts*1000);
    foc->svm_v.duty_b=(float)((Ts-foc->svm_v.Tb*2)/Ts*1000);
    foc->svm_v.duty_c=(float)((Ts-foc->svm_v.Tc*2)/Ts*1000);
//    foc->svm_v.duty_a=foc->svm_v.Ta*100;
//    foc->svm_v.duty_b=foc->svm_v.Tb*100;
//    foc->svm_v.duty_c=foc->svm_v.Tc*100;
}


void svpwm_process2(FOC*foc)
{
    float k;
    k = SQRT3 *100 / U_dc;
    float t1,t2;
    float Ur1 = k * foc->u_beta;
    float Ur2 = k * (-foc->u_beta / 2.0f + SQRT3/2 * foc->u_alpha);
    float Ur3 = k * (-foc->u_beta / 2.0f -SQRT3/2 * foc->u_alpha);
    if (Ur1 > 0.0f)
    {
        // (xx1)
        if (Ur2 > 0.0f)
        {
            // (x11)
            t1 = Ur2;
            t2 = Ur1;

            if (t1 + t2 > 100)
            {
                t1 =t1 * 100 / (t1 + t2);
                t2 = t2 * 100 / (t1 + t2);
            }
            foc->svm_v.Ta = (100 - t1 - t2) / 4;
            foc->svm_v.Tb = (100 + t1 - t2) / 4;
            foc->svm_v.Tc = (100 + t1 + t2) / 4;

            foc->svm_v.duty_a = foc->svm_v.Ta;
            foc->svm_v.duty_b = foc->svm_v.Tb;
            foc->svm_v.duty_c  =foc->svm_v.Tc;
        } else
        {
            if (Ur3 > 0.0f)
            {
                //101 sector 5
                t1 = Ur1;
                t2 = Ur3;



                if (t1 + t2 > 100)
                {
                    t1 =t1 * 100 / (t1 + t2);
                    t2 = t2 * 100 / (t1 + t2);
                }
                foc->svm_v.Ta = (100 - t1 - t2) / 4;
                foc->svm_v.Tb = (100 + t1 - t2) / 4;
                foc->svm_v.Tc = (100 + t1 + t2) / 4;


                foc->svm_v.duty_a = foc->svm_v.Tc;
                foc->svm_v.duty_b = foc->svm_v.Ta;
                foc->svm_v.duty_c  =foc->svm_v.Tb;
            } else
            {
                //001 sector 1
                t1 = -Ur2;
                t2 = -Ur3;


                if (t1 + t2 > 100)
                {
                    t1 =t1 * 100 / (t1 + t2);
                    t2 = t2 * 100 / (t1 + t2);
                }
                foc->svm_v.Ta = (100 - t1 - t2) / 4;
                foc->svm_v.Tb = (100 + t1 - t2) / 4;
                foc->svm_v.Tc = (100 + t1 + t2) / 4;


                foc->svm_v.duty_a = foc->svm_v.Tb;
                foc->svm_v.duty_b = foc->svm_v.Ta;
                foc->svm_v.duty_c  =foc->svm_v.Tc;
            }
        }
    } else
    {
        // xx0
        if (Ur2 > 0.0f)
        {// x10
            if (Ur3 > 0.0f)
            {
                // 110
                t1 = Ur3;
                t2 =Ur2;

                if (t1 + t2 > 100)
                {
                    t1 =t1 * 100 / (t1 + t2);
                    t2 = t2 * 100 / (t1 + t2);
                }
                foc->svm_v.Ta = (100 - t1 - t2) / 4;
                foc->svm_v.Tb = (100 + t1 - t2) / 4;
                foc->svm_v.Tc = (100 + t1 + t2) / 4;


                foc->svm_v.duty_a = foc->svm_v.Tb;
                foc->svm_v.duty_b = foc->svm_v.Tc;
                foc->svm_v.duty_c  =foc->svm_v.Ta;
            } else
            {
                // 010
                t1 = -Ur3;
                t2 = -Ur1;



                if (t1 + t2 > 100)
                {
                    t1 =t1 * 100 / (t1 + t2);
                    t2 = t2 * 100 / (t1 + t2);
                }
                foc->svm_v.Ta = (100 - t1 - t2) / 4;
                foc->svm_v.Tb = (100 + t1 - t2) / 4;
                foc->svm_v.Tc = (100 + t1 + t2) / 4;


                foc->svm_v.duty_a = foc->svm_v.Ta;
                foc->svm_v.duty_b = foc->svm_v.Tc;
                foc->svm_v.duty_c  =foc->svm_v.Tb;
            }
        } else
        {
            t1 = -Ur1;
            t2 = -Ur2;

            if (t1 + t2 > 100)
            {
                t1 =t1 * 100 / (t1 + t2);
                t2 = t2 * 100 / (t1 + t2);
            }
            foc->svm_v.Ta = (100 - t1 - t2) / 4;
            foc->svm_v.Tb = (100 + t1 - t2) / 4;
            foc->svm_v.Tc = (100 + t1 + t2) / 4;


            foc->svm_v.duty_a = foc->svm_v.Tc;
            foc->svm_v.duty_b = foc->svm_v.Tb;
            foc->svm_v.duty_c  =foc->svm_v.Ta;
        }
    }
}