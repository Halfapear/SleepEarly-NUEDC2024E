//
// Created by yiquanfeng on 24-7-29.
//

#include "Kine.h"
#include "servo.h"
#include "usart.h"

node arm_all = {103.2,0,90,100};
//7 10 11
node get_befchess_1 ={50,90,90,90};
node get_chess_1 ={50,100,120,110};
//1
node get_befchess_2 ={70,90,90,90};
node get_chess_2 ={70,150,40,90};
//9
node get_befchess_3 ={135,90,90,90};
node get_chess_3 ={135,95,100,110};
//3
node get_befchess_4 ={115,90,90,90};
node get_chess_4 ={115,100,120,110};
node chess_1 = {80,130,80,110};
node chess_2 = {87,130,80,100};
node chess_3 = {100,130,80,100};
node chess_4 = {75,130,105,100};
node chess_5 = {85,115,100,115};
node chess_6 = {100,120,100,100};
node chess_7 = {75,97,120,100};
node chess_8 = {87,100,113,100};
node chess_9 = {105,108,113,100};

uint8_t phase = 1;

void Arm_Init()
{
    Servo_angle(90,1);
    Servo_angle(90,2);
    Servo_angle(90,3);
    Servo_angle(90,4);
    HAL_GPIO_WritePin(MAG_GPIO_Port,MAG_Pin,RESET);
}
void Arm_Get(double x,double y)
{
    Arm_Move(x,y);
    HAL_GPIO_WritePin(MAG_GPIO_Port,MAG_Pin,RESET);
    HAL_Delay(2000);
}
void Arm_Put(unsigned char No)
{
    switch (No)
    {
    case 1:
        Arm_Move(chess_1.x,chess_1.y);
        HAL_Delay(1000);
        HAL_GPIO_WritePin(MAG_GPIO_Port,MAG_Pin,RESET);
        HAL_Delay(200);
        Arm_Init();
        break;
    case 2:
        Arm_Move(chess_2.x,chess_2.y);
        HAL_Delay(1000);
        HAL_GPIO_WritePin(MAG_GPIO_Port,MAG_Pin,RESET);
        HAL_Delay(200);
        Arm_Init();
        break;
    case 3:
        Arm_Move(chess_3.x,chess_3.y);
        HAL_Delay(1000);
        HAL_GPIO_WritePin(MAG_GPIO_Port,MAG_Pin,RESET);
        HAL_Delay(200);
        Arm_Init();
        break;
    case 4:
        Arm_Move(chess_4.x,chess_4.y);
        HAL_Delay(1000);
        HAL_GPIO_WritePin(MAG_GPIO_Port,MAG_Pin,RESET);
        HAL_Delay(200);
        Arm_Init();
        break;
    case 5:
        Arm_Move(chess_5.x,chess_5.y);
        HAL_Delay(1000);
        HAL_GPIO_WritePin(MAG_GPIO_Port,MAG_Pin,RESET);
        HAL_Delay(200);
        Arm_Init();
        break;
    case 6:
        Arm_Move(chess_6.x,chess_6.y);
        HAL_Delay(1000);
        HAL_GPIO_WritePin(MAG_GPIO_Port,MAG_Pin,RESET);
        HAL_Delay(200);
        Arm_Init();
        break;
    case 7:
        Arm_Move(chess_7.x,chess_7.y);
        HAL_Delay(1000);
        HAL_GPIO_WritePin(MAG_GPIO_Port,MAG_Pin,RESET);
        HAL_Delay(200);
        Arm_Init();
        break;
    case 8:
        Arm_Move(chess_8.x,chess_8.y);
        HAL_Delay(1000);
        HAL_GPIO_WritePin(MAG_GPIO_Port,MAG_Pin,RESET);
        HAL_Delay(200);
        Arm_Init();
        break;
    case 9:
        Arm_Move(chess_9.x,chess_9.y);
        HAL_Delay(1000);
        HAL_GPIO_WritePin(MAG_GPIO_Port,MAG_Pin,RESET);
        HAL_Delay(200);
        Arm_Init();
        break;
    default:
        HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);
        HAL_Delay(200);
    }
}
void Arm_Move(double x,double y)
{
    double angle_2,angle_1,angel_base; //three angle
    double dealt_x = x - arm_all.x;
    double dealt_y = y - arm_all.y;
    double dis_line = sqrt(dealt_x * dealt_x + dealt_y * dealt_y);
    double dis_hig = HEIGHT;
    angel_base = 90.0 + Rad_To_Angle(atan2(dealt_x,dealt_y));
    // Servo_angle(angel_base,1);
    double m = atan2((HEIGHT - HEIGHT),dis_line);//直角三角形的顶角
    double hypo = sqrt((HEIGHT - BASE) * (HEIGHT - BASE) + dis_line * dis_line);//可优化
    // double n = acos((ARM_1 * ARM_1 + hypo * hypo - ARM_2 * ARM_2) / 2 * ARM_1 * hypo);//余弦定理
    double temp = (ARM_1 * ARM_1 + hypo * hypo - ARM_2 * ARM_2) / (2 * ARM_1 * hypo);
    double n = acos(temp);
    angle_1 = 180 - Rad_To_Angle(m) - Rad_To_Angle(n);
    angle_2 = Rad_To_Angle(acos((ARM_1 * ARM_1 + ARM_2 * ARM_2 - hypo * hypo) / (2 * ARM_1 * ARM_2))) - 90;
    Servo_angle(angel_base,1);
    Servo_angle(angle_1,2);
    Servo_angle(angle_2,3);
    Servo_angle(angle_1,4);
}

void Arm_Move_fake(int num)
{
    switch (num)
    {
    case 1:
        Servo_angle(chess_1.angle_1,1);
        Servo_angle(chess_1.angle_2,2);
        Servo_angle(chess_1.angle_3,3);
        Servo_angle(chess_1.angle_4,4);
        break;
    case 2:
        Servo_angle(chess_2.angle_1,1);
        Servo_angle(chess_2.angle_2,2);
        Servo_angle(chess_2.angle_3,3);
        Servo_angle(chess_2.angle_4,4);
        break;
    case 3:
        Servo_angle(chess_3.angle_1,1);
        Servo_angle(chess_3.angle_2,2);
        Servo_angle(chess_3.angle_3,3);
        Servo_angle(chess_3.angle_4,4);
        break;
    case 4:
        Servo_angle(chess_4.angle_1,1);
        Servo_angle(chess_4.angle_2,2);
        Servo_angle(chess_4.angle_3,3);
        Servo_angle(chess_4.angle_4,4);
        break;
    case 5:
        Servo_angle(chess_5.angle_1,1);
        Servo_angle(chess_5.angle_2,2);
        Servo_angle(chess_5.angle_3,3);
        Servo_angle(chess_5.angle_4,4);
        break;
    case 6:
        Servo_angle(chess_6.angle_1,1);
        Servo_angle(chess_6.angle_2,2);
        Servo_angle(chess_6.angle_3,3);
        Servo_angle(chess_6.angle_4,4);
        break;
    case 7:
        Servo_angle(chess_7.angle_1,1);
        Servo_angle(chess_7.angle_2,2);
        Servo_angle(chess_7.angle_3,3);
        Servo_angle(chess_7.angle_4,4);
        break;
    case 8:
        Servo_angle(chess_8.angle_1,1);
        Servo_angle(chess_8.angle_2,2);
        Servo_angle(chess_8.angle_3,3);
        Servo_angle(chess_8.angle_4,4);
        break;
    case 9:
        Servo_angle(chess_9.angle_1,1);
        Servo_angle(chess_9.angle_2,2);
        Servo_angle(chess_9.angle_3,3);
        Servo_angle(chess_9.angle_4,4);
        break;
    case 10:
        Servo_angle(get_chess_1.angle_1,1);
        Servo_angle(get_chess_1.angle_2,2);
        Servo_angle(get_chess_1.angle_3,3);
        Servo_angle(get_chess_1.angle_4,4);
        break;
    case 11:
        Servo_angle(get_befchess_1.angle_1,1);
        Servo_angle(get_befchess_1.angle_2,2);
        Servo_angle(get_befchess_1.angle_3,3);
        Servo_angle(get_befchess_1.angle_4,4);
        break;
        //1
    case 12:
        Servo_angle(get_chess_2.angle_1,1);
        Servo_angle(get_chess_2.angle_2,2);
        Servo_angle(get_chess_2.angle_3,3);
        Servo_angle(get_chess_2.angle_4,4);
        break;
    case 13:
        Servo_angle(get_befchess_2.angle_1,1);
        Servo_angle(get_befchess_2.angle_2,2);
        Servo_angle(get_befchess_2.angle_3,3);
        Servo_angle(get_befchess_2.angle_4,4);
        break;
        //9
    case 14:
        Servo_angle(get_chess_3.angle_1,1);
        Servo_angle(get_chess_3.angle_2,2);
        Servo_angle(get_chess_3.angle_3,3);
        Servo_angle(get_chess_3.angle_4,4);
        break;
    case 15:
        Servo_angle(get_befchess_3.angle_1,1);
        Servo_angle(get_befchess_3.angle_2,2);
        Servo_angle(get_befchess_3.angle_3,3);
        Servo_angle(get_befchess_3.angle_4,4);
        break;
        //3
    case 16:
        Servo_angle(get_chess_4.angle_1,1);
        Servo_angle(get_chess_4.angle_2,2);
        Servo_angle(get_chess_4.angle_3,3);
        Servo_angle(get_chess_4.angle_4,4);
        break;
    case 17:
        Servo_angle(get_befchess_4.angle_1,1);
        Servo_angle(get_befchess_4.angle_2,2);
        Servo_angle(get_befchess_4.angle_3,3);
        Servo_angle(get_befchess_4.angle_4,4);
        break;
    }
}

void Arm_Exe_1()
{
    // Servo_angle()
    // Arm_Get(10,90);
    // Arm_Put(5);
    // Servo_angle(90+30,3);
    // Servo_angle()
}

void Arm_Exe_2(unsigned char *data)
{
    Arm_Get(10,100);
    Arm_Put(data[1]);
    Arm_Get(10,77);
    Arm_Put(data[2]);//two black

    Arm_Get(10,77);
    Arm_Put(data[3]);
    Arm_Get(10,77);
    Arm_Put(data[4]);//two white
}

void Arm_Exe_3()
{
    return;
}

void Arm_Test()
{
    Arm_Move(chess_5.x,chess_5.y);
}