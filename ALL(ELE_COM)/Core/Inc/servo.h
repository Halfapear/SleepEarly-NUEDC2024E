//
// Created by yiquanfeng on 24-7-29.
//

#ifndef SERVO_H
#define SERVO_H

#include "tim.h"

#define PI 3.1416

void Servo_Init();
void Servo_angle(double angle,int id);
double Rad_To_Angle(double rad);
double LimitToPI(double angle);
double LimitTo90(double angle);

#endif //SERVO_H
