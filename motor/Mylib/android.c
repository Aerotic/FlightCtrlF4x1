/**
***Author:Aero
***Created in 2018 Aprl
**/
#include "android.h"
#define RadToDeg 57.295779
struct _RC RC;
struct _SENSOR phone;


u8 tx_buffer[9];
u8 rx_buffer[9];
int32_t cnter=0;

union char2float{
	char buf[2];
	short dest;
}bf;

void Android_Serialport_Config(void)
{
	  USART_InitTypeDef USART_InitStructure;
	  GPIO_InitTypeDef  GPIO_InitStructure;
    NVIC_InitTypeDef  NVIC_InitStructure;
    DMA_InitTypeDef   DMA_InitStructure;
	
	    
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_DMA2,ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 ,ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);

		GPIO_PinAFConfig(GPIOA,GPIO_PinSource11 ,GPIO_AF_USART6);
	  GPIO_PinAFConfig(GPIOA,GPIO_PinSource10 ,GPIO_AF_USART1);
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 |  GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	
		USART_DeInit(USART1);
		USART_DeInit(USART6);
		
		//usart1 rx
		USART_InitStructure.USART_BaudRate = 512000;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No ;
		USART_InitStructure.USART_Mode = USART_Mode_Rx;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_Init(USART1,&USART_InitStructure);
		
		//usart6 tx
		USART_InitStructure.USART_BaudRate = 512000;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No ;
		USART_InitStructure.USART_Mode = USART_Mode_Tx;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_Init(USART6,&USART_InitStructure);
    
		USART_Cmd(USART1,ENABLE);
		USART_Cmd(USART6,ENABLE);
//		DelayUs(200);
//		USART_SendData(USART6,0XAA);
//		DelayUs(200);
//		USART_SendData(USART6,0XA7);
//		DelayUs(200);
//		USART_SendData(USART6,0XA4);
//		DelayUs(200);
		USART_DMACmd(USART6,USART_DMAReq_Tx,ENABLE);
		USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);
    
		//DMA_USART1_RX
    NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream6_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
		
    		
		//DMA_USART6_Tx
		DMA_DeInit(DMA2_Stream6);
    DMA_InitStructure.DMA_Channel= DMA_Channel_5;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(USART6->DR);
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)tx_buffer;
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
    DMA_InitStructure.DMA_BufferSize = 9;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
    DMA_InitStructure.DMA_MemoryBurst = DMA_Mode_Normal;
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_Init(DMA2_Stream6,&DMA_InitStructure);


    DMA_ITConfig(DMA2_Stream6,DMA_IT_TC,ENABLE);
    DMA_Cmd(DMA2_Stream6,DISABLE);	
		
		
		//DMA_USART3_Rx
		USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
    
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    		
		
		DMA_DeInit(DMA2_Stream2);
    DMA_InitStructure.DMA_Channel= DMA_Channel_4;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(USART1->DR);
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)rx_buffer;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
    DMA_InitStructure.DMA_BufferSize = 128;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
    DMA_InitStructure.DMA_MemoryBurst = DMA_Mode_Normal;
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_Init(DMA2_Stream2,&DMA_InitStructure);

		
    //DMA_ITConfig(DMA1_Stream5,DMA_IT_TC,ENABLE);
    DMA_Cmd(DMA2_Stream2,ENABLE);	
}
void DMA2_Stream6_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA2_Stream6, DMA_IT_TCIF6))
	{
		DMA_Cmd(DMA2_Stream6,DISABLE);	
		DMA_ClearFlag(DMA2_Stream6, DMA_FLAG_TCIF6);
		DMA_ClearITPendingBit(DMA2_Stream6, DMA_IT_TCIF6);
		
		
  }
}
void USART1_IRQHandler(void)  
{
	volatile uint8_t num_1;
	volatile uint16_t data_len_1;
	data_len_1 = 128 - DMA_GetCurrDataCounter(DMA2_Stream2);
  num_1 = USART1->SR;
  num_1 = USART1->DR;
	DMA_Cmd(DMA2_Stream2, DISABLE);   
  DMA_ClearFlag(DMA2_Stream2,DMA_FLAG_TEIF2 | DMA_FLAG_TCIF2 | DMA_FLAG_DMEIF2 | DMA_FLAG_HTIF2 | DMA_FLAG_FEIF2);
	DMA_SetCurrDataCounter(DMA2_Stream2,128);
  USART_ClearITPendingBit(USART1,USART_IT_IDLE);
	AndroidRPM();
  DMA_Cmd(DMA2_Stream2, ENABLE);
}
void SendToAndroid()
{
	DMA_Cmd(DMA2_Stream6,DISABLE);
	tx_buffer[0] = (u8)(0x0ff&(RC.CH[0]>>8));
	tx_buffer[1] = (u8)(0x0ff&(RC.CH[0]));
	tx_buffer[2] = (u8)(0x0ff&(RC.CH[1]>>8));
	tx_buffer[3] = (u8)(0xff&(RC.CH[1]));
	tx_buffer[4] = (u8)(0x0ff&(RC.CH[2]>>8));
	tx_buffer[5] = (u8)(0x0ff&(RC.CH[2]));
	tx_buffer[6] = (u8)(0x0ff&(RC.CH[3]>>8));
	tx_buffer[7] = (u8)(0x0ff&(RC.CH[3]));
	tx_buffer[8] = (u8)(0x0ff&(RC.CH[4]));
	tx_buffer[8] |= (u8)(0x0ff&(RC.CH[5]<<2));
	DMA_SetCurrDataCounter(DMA2_Stream6,9);
	DMA_Cmd(DMA2_Stream6,ENABLE);
	LED_One_TOGGLE;
}
