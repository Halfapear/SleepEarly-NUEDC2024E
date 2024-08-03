/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "Kine.h"
#include "servo.h"
#include "stm32f1xx_ll_rcc.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
//70 32 32 32 32 32 32 FF FF FF
uint8_t data[5] = {0};
/* USER CODE END 0 */

void checkHexArray(uint8_t data[], int length) {

    switch(data[0]) {
    case 0x31:
      Arm_Init();
      HAL_Delay(2000);
      Arm_Move_fake(11);
      HAL_Delay(2000);
      Arm_Move_fake(10);
      HAL_Delay(2000);
      Arm_Init();
      HAL_Delay(2000);
      Arm_Move_fake(5);
      HAL_Delay(2000);
      break;
    case 0x32:
      for(int i = 1; i < length; i++) {
        switch(data[i]) {
        case 0x32:
          Arm_Init();
          HAL_Delay(2000);
          Arm_Move_fake(9+i*2);
          HAL_Delay(2000);
          Arm_Move_fake(8+i*2);
          HAL_Delay(2000);
          Arm_Init();
          HAL_Delay(2000);
          Arm_Move_fake(1);
          HAL_Delay(2000);
          break;
        case 0x33:
          Arm_Init();
          HAL_Delay(2000);
          Arm_Move_fake(9+i*2);
          HAL_Delay(2000);
          Arm_Move_fake(8+i*2);
          HAL_Delay(2000);
          Arm_Init();
          HAL_Delay(2000);
          Arm_Move_fake(2);
          HAL_Delay(2000);
          break;
        case 0x34:
          Arm_Init();
          HAL_Delay(2000);
          Arm_Move_fake(9+i*2);
          HAL_Delay(2000);
          Arm_Move_fake(8+i*2);
          HAL_Delay(2000);
          Arm_Init();
          HAL_Delay(2000);
          Arm_Move_fake(3);
          HAL_Delay(2000);
          break;
        case 0x35:
          Arm_Init();
          HAL_Delay(2000);
          Arm_Move_fake(9+i*2);
          HAL_Delay(2000);
          Arm_Move_fake(8+i*2);
          HAL_Delay(2000);
          Arm_Init();
          HAL_Delay(2000);
          Arm_Move_fake(4);
          HAL_Delay(2000);
          break;
        case 0x36:
          Arm_Init();
          HAL_Delay(2000);
          Arm_Move_fake(9+i*2);
          HAL_Delay(2000);
          Arm_Move_fake(8+i*2);
          HAL_Delay(2000);
          Arm_Init();
          HAL_Delay(2000);
          Arm_Move_fake(5);
          HAL_Delay(2000);
          break;
        case 0x37:
          Arm_Init();
          HAL_Delay(2000);
          Arm_Move_fake(9+i*2);
          HAL_Delay(2000);
          Arm_Move_fake(8+i*2);
          HAL_Delay(2000);
          Arm_Init();
          HAL_Delay(2000);
          Arm_Move_fake(6);
          HAL_Delay(2000);
          break;
        case 0x38:
          Arm_Init();
          HAL_Delay(2000);
          Arm_Move_fake(9+i*2);
          HAL_Delay(2000);
          Arm_Move_fake(8+i*2);
          HAL_Delay(2000);
          Arm_Init();
          HAL_Delay(2000);
          Arm_Move_fake(7);
          HAL_Delay(2000);
          break;
        case 0x39:
          Arm_Init();
          HAL_Delay(2000);
          Arm_Move_fake(9+i*2);
          HAL_Delay(2000);
          Arm_Move_fake(8+i*2);
          HAL_Delay(2000);
          Arm_Init();
          HAL_Delay(2000);
          Arm_Move_fake(8);
          HAL_Delay(2000);
          break;
        case 0x09:
          Arm_Init();
          HAL_Delay(2000);
          Arm_Move_fake(9+i*2);
          HAL_Delay(2000);
          Arm_Move_fake(8+i*2);
          HAL_Delay(2000);
          Arm_Init();
          HAL_Delay(2000);
          Arm_Move_fake(9);
          HAL_Delay(2000);
          break;
        }
      }
      break;
    }

}

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  Servo_Init();
  HAL_UART_Transmit(&huart2,"enter start to start\n",21,0xff);
  //HAL_UART_Receive_IT(&huart2,data,5);
  // unsigned char data[5] = {0};
  // HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

     Arm_Init();
     HAL_Delay(2000);
     Arm_Move_fake(11);
     HAL_Delay(2000);
     Arm_Move_fake(10);
     HAL_Delay(2000);
     Arm_Init();
     HAL_Delay(2000);
     Arm_Move_fake(5);
     HAL_Delay(2000);

    //  __HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_2,99);


    // HAL_UART_Receive(&huart2,data,5,0xff);
    // HAL_UART_Transmit(&huart2,data,5,0xff);
    // HAL_Delay(20);
    // checkHexArray(data, 5);



    //  HAL_UART_Transmit(&huart2,data,5,0xff);
    //  HAL_Delay(200);
    // Arm_Init();
    // Arm_Test();


    //Arm_Exe();
    //Arm_Init();

    // __HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_2,99);
    // HAL_UART_Receive(&huart2,data,5,0xff);
    // HAL_Delay(20);
    // HAL_UART_Transmit(&huart2,data,5,0xff);
    // HAL_Delay(200);
    //Arm_Init();
     //Arm_Test();
    // switch (data[0])
    // {
    // case '1':
    //   Arm_Exe_1();
    //   data[0] = 0;
    //   break;
    // case '2':
    //   Arm_Exe_2(data);
    //   data[0] = 0;
    //   break;
    // case '3':
    //   Arm_Exe_3();
    //   break;
    // default:
    //   HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);
    //   HAL_Delay(200);
    //   break;
    // }

    //Arm_Exe();
    //Arm_Init();
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if(huart == &huart2)
  {
    HAL_UART_Receive(&huart2,data,5,0xff);
  }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
