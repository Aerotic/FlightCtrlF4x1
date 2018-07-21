#include "board.h"
//�δ�ʱ���������� ,49������
volatile uint32_t sysTickUptime=0;

#define TICK_PER_SECOND 1000 
#define TICK_US	(1000000/TICK_PER_SECOND)

void SysTick_Configuration(void)
{
	RCC_ClocksTypeDef  rcc_clocks;
	uint32_t         cnts;

	RCC_GetClocksFreq(&rcc_clocks);

	cnts = (uint32_t)rcc_clocks.HCLK_Frequency / TICK_PER_SECOND;//cnts=HCLK
	cnts = cnts / 8;//cnts=HCLK/8 

	SysTick_Config(cnts);
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);//HCLK/8  set freq divider    
}

uint32_t GetSysTime_us(void) 
{
	register uint32_t ms;
	u32 value;
	ms = sysTickUptime;
	value = ms * TICK_US + (SysTick->LOAD - SysTick->VAL) * TICK_US / SysTick->LOAD;
	return value;
}

void DelayUs(uint32_t us)
{
    uint32_t now = GetSysTime_us();
    while (GetSysTime_us() - now < us);
}

void DelayMs(uint32_t ms)
{
    while (ms--)
      DelayUs(1000);
}

void FlightController_board_Init(void)
{
		/*led��ʼ��*/
		Led_Init();
		/*ģ��IIC��ʼ��*/
//		I2C_Soft_Init();
//		//LED_BLUE_ON;
//		/*�жϷ���*/
		NVIC_PriorityGroupConfig(NVIC_GROUP);
		/*�δ�ʱ������*/
		SysTick_Configuration();
//		/*��ȡ����*/
//		Get_parameter();D
		Android_Serialport_Config();
//		/*��ʱ��3���� ����pwm���Ƶ��*/
			TIM3_PWM_Init(400);
//				Mpu6050_Init(1000,20);
//				Mpu6050_CalOffset_Gyro();
//			/*?????????*/
//			Mpu6050_CalOffset_Acc();
//		/*ʱ���ʼ��*/
//		Cycle_Time_Init();
//		/*ms5611��ʼ��*/
//		MS5611_Init();
//		MS5611_CalOffset();
//		/*��ֱ������ٶȼ�ƫ���˲�*/
//		LPF2pSetCutoffFreq_1(100, 10);
//		/*ˮƽx������ٶȼ�ƫ���˲�*/
//		LPF2pSetCutoffFreq_3(100, 10);
//		/*ˮƽy������ٶȼ�ƫ���˲�*/
//		LPF2pSetCutoffFreq_4(100, 10);
//		/*�ο�����ϵx������ٶȼ�ƫ���˲�*/
//		LPF2pSetCutoffFreq_5(100, 10);
//		/*�ο�����ϵy������ٶȼ�ƫ���˲�*/
//		LPF2pSetCutoffFreq_6(100, 10);

//		/*����1��ʼ��,��������*/
//	  //Usart1_Config();
		Dbus_Configuration();
		/*ָʾ��*/
		//LED_RED_OFF;
		/*�ɻ���ʼ�����*/
		aircraft.init_ok=1;
}

