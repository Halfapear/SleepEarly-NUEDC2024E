//
// Created by yiquanfeng on 24-7-29.
//

#include "servo.h"

#include "stm32f1xx_hal_tim.h"


void Servo_Init()
{
    HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_1);
}

void Servo_angle(double angle,int id)
{
    int compare = 10 + (angle / 180.0 * 40);
    switch (id)
    {
        case 1:
            __HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_1,compare);
            break;
        case 2:
            __HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_1,compare);
            break;
        case 3:
            __HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,compare);
            break;
        case 4:
            __HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_1,compare);
            break;
        default:
            break;
    }
}

double Rad_To_Angle(double rad)
{
    return rad / PI * 180.0;
}

// double LimitToPI(double angle)
// {
//     if(angle > 180)
//     {
//         return 0;
//     }
// }

double LimitTo90(double angle)
{
    if(angle > 90)
        return 180 - angle;
    else
        return angle;
}