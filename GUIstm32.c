#include "stm32f10x.h"   
#include "tjc_usart_hmi.h"
#include "stm32f10x_rcc.h"

#define FRAMELENGTH 6

void NVIC_Configuration(void);
void RCC_Configuration(void);

void USART1_IRQHandler(void);

uint8_t temp[5]; // �������temp���ڴ洢��ȡ������

int main(void)
{
	RCC_Configuration();
	NVIC_Configuration();
	USART1_Init(115200);	  	 //���ڳ�ʼ��Ϊ115200
	TJCPrintf("\x00");          //Ϊȷ������HMI����ͨ��
	
	
	while(1)
	{
	

		
	}
	
}


void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Ƕ�������жϿ�������ѡ�� */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* ����USARTΪ�ж�Դ */
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  /* �������ȼ�*/
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* �����ȼ� */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* ʹ���ж� */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* ��ʼ������NVIC */
  NVIC_Init(&NVIC_InitStructure);
}


void RCC_Configuration(void){ //RCCʱ�ӵ�����  
	ErrorStatus HSEStartUpStatus;   
	RCC_DeInit();              /* RCC system reset(for debug purpose) RCC�Ĵ����ָ���ʼ��ֵ*/   
	RCC_HSEConfig(RCC_HSE_ON); /* Enable HSE ʹ���ⲿ���پ���*/   
	HSEStartUpStatus = RCC_WaitForHSEStartUp(); /* Wait till HSE is ready �ȴ��ⲿ���پ���ʹ�����*/   
	if(HSEStartUpStatus == SUCCESS){   
		/*����PLLʱ��Դ����Ƶϵ��*/   
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9); //RCC_PLLMul_x��ö��2~16���Ǳ�Ƶֵ����HSE=8MHZ,RCC_PLLMul_9ʱPLLCLK=72MHZ   
		/*����AHBʱ�ӣ�HCLK��*/   
		RCC_HCLKConfig(RCC_SYSCLK_Div1); //RCC_SYSCLK_Div1����AHBʱ�� = ϵͳʱ��(SYSCLK) = 72MHZ���ⲿ����8HMZ��   
		/*ע��˴������ã����ʹ��SYSTICK����ʱ���򣬴�ʱSYSTICK(Cortex System timer)=HCLK/8=9MHZ*/   
		RCC_PCLK1Config(RCC_HCLK_Div2); //���õ���AHBʱ�ӣ�PCLK1��,RCC_HCLK_Div2����APB1ʱ�� = HCLK/2 = 36MHZ���ⲿ����8HMZ��   
		RCC_PCLK2Config(RCC_HCLK_Div1); //���ø���AHBʱ�ӣ�PCLK2��,RCC_HCLK_Div1����APB2ʱ�� = HCLK = 72MHZ���ⲿ����8HMZ��   
		/*ע��AHB��Ҫ�����ⲿ�洢��ʱ�ӡ�APB2����AD��I/O���߼�TIM������1��APB1����DA��USB��SPI��I2C��CAN������2��3��4��5����ͨTIM */  
		FLASH_SetLatency(FLASH_Latency_2); //����FLASH�洢����ʱʱ��������   
		/*FLASHʱ���ӳټ������ڣ��ȴ�����ͬ��������   
		�Ƽ����յ�Ƭ��ϵͳ����Ƶ�ʣ�
		0��24MHzʱ��ȡLatency_0��   
		24��48MHzʱ��ȡLatency_1��   
		48~72MHzʱ��ȡLatency_2*/   
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable); //ѡ��FLASHԤȡָ�����ģʽ��Ԥȡָ����ʹ��   
		RCC_PLLCmd(ENABLE);	//ʹ��PLL
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET); //�ȴ�PLL����ȶ�   
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK); //ѡ��SYSCLKʱ��ԴΪPLL
		while(RCC_GetSYSCLKSource() != 0x08); //�ȴ�PLL��ΪSYSCLKʱ��Դ   
	}  
	/*��ʼʹ�ܳ�������Ҫʹ�õ�����ʱ��*/   
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOD, ENABLE); //APB2����ʱ��ʹ��      
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); 
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
}  


void USART1_IRQHandler(void)
{
	static uint8_t frame[10];
	static uint8_t frameIndex = 0;
	static uint8_t dataLength = 0;
	static uint8_t validFrame = 0; // ���֡�Ƿ���Ч
	
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		uint8_t data = USART_ReceiveData(USART1);
		
		// �ų�00 00 00 FF FF FF
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
		// �ų�88 FF FF FF
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
		// ����70��ͷ������֡
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
				validFrame = 1; // ���Ϊ��Ч֡
			}
			else
			{
				frameIndex = 0; // ������Ȳ���1��5������
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
				// �ɹ����յ���Ч֡
				// ����temp�е����ݣ������ӡ����������
			}
			frameIndex = 0; // �����Խ�����һ֡
			validFrame = 0; // ������Ч֡���
		}
		else
		{
			frameIndex = 0; // Reset if the frame doesn't match the expected pattern
			validFrame = 0; // ������Ч֡���
		}
	}
}












