#include "main.h"
#include "stm32f1xx_hal.h"

#define MAX_FRAME_LENGTH 64

extern UART_HandleTypeDef huart1;

uint8_t frame[MAX_FRAME_LENGTH];
uint8_t temp[MAX_FRAME_LENGTH];
uint8_t frameIndex = 0;
uint8_t dataLength = 0;
uint8_t validFrame = 0;
uint8_t data = 0;

void USART1_IRQHandler(void)
{
    HAL_UART_IRQHandler(&huart1);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        processReceivedByte(data);
        HAL_UART_Receive_IT(&huart1, &data, 1); // 重新使能接收中断
    }
}

void processReceivedByte(uint8_t data)
{
    if (frameIndex == 0 && data == 0x00)
    {
        frame[frameIndex++] = data;
    }
    else if (frameIndex < 3 && data == 0x00)
    {
        frame[frameIndex++] = data;
    }
    else if (frameIndex >= 3 && frameIndex < 6 && data == 0xFF)
    {
        frame[frameIndex++] = data;
        if (frameIndex == 6)
        {
            frameIndex = 0; // Reset if it's the unwanted frame 00 00 00 FF FF FF
        }
    }
    // 排除88 FF FF FF
    else if (frameIndex == 0 && data == 0x88)
    {
        frame[frameIndex++] = data;
    }
    else if (frameIndex > 0 && frameIndex < 4 && data == 0xFF)
    {
        frame[frameIndex++] = data;
        if (frameIndex == 4)
        {
            frameIndex = 0; // Reset if it's the unwanted frame 88 FF FF FF
        }
    }
    // 处理70开头的数据帧
    else if (frameIndex == 0 && data == 0x70)
    {
        frame[frameIndex++] = data;
    }
    else if (frameIndex == 1)
    {
        dataLength = data; // Store the length of the data to be extracted
        frame[frameIndex++] = data;
        if (dataLength == 1 || dataLength == 5)
        {
            validFrame = 1; // 标记为有效帧
        }
        else
        {
            frameIndex = 0; // 如果长度不是1或5，重置
        }
    }
    else if (frameIndex > 1 && frameIndex <= dataLength + 1)
    {
        if (validFrame)
        {
            temp[frameIndex - 2] = data; // Store the data in temp
        }
        frame[frameIndex++] = data;
    }
    else if (frameIndex == dataLength + 2 && data == 0xFF)
    {
        frame[frameIndex++] = data;
    }
    else if (frameIndex == dataLength + 3 && data == 0xFF)
    {
        frame[frameIndex++] = data;
    }
    else if (frameIndex == dataLength + 4 && data == 0xFF)
    {
        if (validFrame)
        {
            // 成功接收到有效帧
            // 处理temp中的数据，例如打印或其他操作
        }
        frameIndex = 0; // 重置以接收下一帧
        validFrame = 0; // 重置有效帧标记
    }
    else
    {
        frameIndex = 0; // Reset if the frame doesn't match the expected pattern
        validFrame = 0; // 重置有效帧标记
    }
}
