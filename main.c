#include "stm32f10x.h"   
#include "tjc_usart_hmi.h"
#include "stm32f10x_rcc.h"

#define FRAMELENGTH 6

void NVIC_Configuration(void);
void RCC_Configuration(void);

void USART1_IRQHandler(void);

uint8_t temp[5]; // 定义变量temp用于存储提取的数据

int main(void)
{
	RCC_Configuration();
	NVIC_Configuration();
	USART1_Init(115200);	  	 //串口初始化为115200
	TJCPrintf("\x00");          //为确保串口HMI正常通信
	
	
	while(1)
	{
	

		
	}
	
}


void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* 嵌套向量中断控制器组选择 */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* 配置USART为中断源 */
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  /* 抢断优先级*/
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* 子优先级 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* 使能中断 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* 初始化配置NVIC */
  NVIC_Init(&NVIC_InitStructure);
}


void RCC_Configuration(void){ //RCC时钟的设置  
	ErrorStatus HSEStartUpStatus;   
	RCC_DeInit();              /* RCC system reset(for debug purpose) RCC寄存器恢复初始化值*/   
	RCC_HSEConfig(RCC_HSE_ON); /* Enable HSE 使能外部高速晶振*/   
	HSEStartUpStatus = RCC_WaitForHSEStartUp(); /* Wait till HSE is ready 等待外部高速晶振使能完成*/   
	if(HSEStartUpStatus == SUCCESS){   
		/*设置PLL时钟源及倍频系数*/   
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9); //RCC_PLLMul_x（枚举2~16）是倍频值。当HSE=8MHZ,RCC_PLLMul_9时PLLCLK=72MHZ   
		/*设置AHB时钟（HCLK）*/   
		RCC_HCLKConfig(RCC_SYSCLK_Div1); //RCC_SYSCLK_Div1——AHB时钟 = 系统时钟(SYSCLK) = 72MHZ（外部晶振8HMZ）   
		/*注意此处的设置，如果使用SYSTICK做延时程序，此时SYSTICK(Cortex System timer)=HCLK/8=9MHZ*/   
		RCC_PCLK1Config(RCC_HCLK_Div2); //设置低速AHB时钟（PCLK1）,RCC_HCLK_Div2——APB1时钟 = HCLK/2 = 36MHZ（外部晶振8HMZ）   
		RCC_PCLK2Config(RCC_HCLK_Div1); //设置高速AHB时钟（PCLK2）,RCC_HCLK_Div1——APB2时钟 = HCLK = 72MHZ（外部晶振8HMZ）   
		/*注：AHB主要负责外部存储器时钟。APB2负责AD，I/O，高级TIM，串口1。APB1负责DA，USB，SPI，I2C，CAN，串口2，3，4，5，普通TIM */  
		FLASH_SetLatency(FLASH_Latency_2); //设置FLASH存储器延时时钟周期数   
		/*FLASH时序延迟几个周期，等待总线同步操作。   
		推荐按照单片机系统运行频率：
		0—24MHz时，取Latency_0；   
		24—48MHz时，取Latency_1；   
		48~72MHz时，取Latency_2*/   
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable); //选择FLASH预取指缓存的模式，预取指缓存使能   
		RCC_PLLCmd(ENABLE);	//使能PLL
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET); //等待PLL输出稳定   
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK); //选择SYSCLK时钟源为PLL
		while(RCC_GetSYSCLKSource() != 0x08); //等待PLL成为SYSCLK时钟源   
	}  
	/*开始使能程序中需要使用的外设时钟*/   
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOD, ENABLE); //APB2外设时钟使能      
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); 
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
}  


void USART1_IRQHandler(void)
{
	static uint8_t frame[10];
	static uint8_t frameIndex = 0;
	static uint8_t dataLength = 0;
	static uint8_t validFrame = 0; // 标记帧是否有效
	
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		uint8_t data = USART_ReceiveData(USART1);
		
		// 排除00 00 00 FF FF FF
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
}












