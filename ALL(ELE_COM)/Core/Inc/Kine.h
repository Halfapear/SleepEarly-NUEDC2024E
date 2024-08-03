//
// Created by yiquanfeng on 24-7-29.
//

#ifndef KINE_H
#define KINE_H

#define ARM_1 104
#define ARM_2 141
#define BASE 47
#define HEIGHT 70 //固定高度

#include <stdlib.h>
#include <stdint.h>
// #include "servo.h"
#include <math.h>

typedef struct node
{
    double angle_1;
    double angle_2;
    double angle_3;
    double angle_4;
    double x;
    double y;
}node;

// enum phase
// {
//     get = 1,
//     put
// };
//node arm_all;
// node chess_1;
// node chess_2;
// node chess_3;
// node chess_4;
// node chess_5;
// node chess_6;
// node chess_7;
// node chess_8;
// node chess_9;

void Arm_Move(double x,double y);
void Arm_Exe_1();
void Arm_Exe_2(uint8_t *data);
void Arm_Exe_3();
void Arm_Move_fake(int num);
void Arm_Get(double x,double y);
void Arm_Put(unsigned char No);
void Arm_Test();
void Arm_Init();

#endif //KINE_H
