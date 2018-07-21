#include "schedule.h"
struct _schedule schedule;

int64_t loop_5hz;
void Loop_1000Hz(void)//1msִ��һ�� 
{

}
void Rad2Deg()
{

}

void Loop_500Hz(void)	//2msִ��һ��
{	
	float loop_time_500hz;
	loop_time_500hz = Get_Cycle_T(0);     /*��ȡ2ms׼ȷʱ��*/
//	Mpu6050_Read();
//	Mpu6050_Data_Prepare();
}

void Loop_200Hz(void)	//5msִ��һ��
{
		float loop_time_200hz;
//			loop_time_200hz = Get_Cycle_T(1);
//			/*��Ԫ����̬����*/
//			Control_Outer(loop_time_200hz);       									/*��̬�⻷����*/
//			Update_Velocities_Positions_Z(loop_time_200hz); 				/*����Z�����ٶȺ�λ��*/
	  TestRPM();
}

void Loop_100Hz(void)	//10msִ��һ��
{
	float loop_time_100hz;
	loop_time_100hz = Get_Cycle_T(2);     				/*��ȡ10ms׼ȷʱ��*/
//	MS5611_Update();
//	
// 	Height_Acceleration_Control(loop_time_100hz); /*���ٶ����Ų�����ʵʱ���У���������*/
			

	//MS5611_Update();
	RC_Check(loop_time_100hz,200);      					/*ң����ʧ�ؼ�飬��200ms��û���յ�������Ϊʧ��*/
	

}

void Loop_50Hz(void)	//20msִ��һ��
{

}

void Loop_20Hz(void)	//50msִ��һ��1
{
//		TIM3->CCR1=MIN_MOTOR_PWM;
//		TIM3->CCR2=MIN_MOTOR_PWM;
//		TIM3->CCR3=MIN_MOTOR_PWM;
//		TIM3->CCR4=MIN_MOTOR_PWM;
	float loop_time_20hz;
	loop_time_20hz = Get_Cycle_T(4);            /*��ȡ50ms��׼ȷʱ��*/
	static u8 timer_50ms = 0;//��¼50ms����
	if(++timer_50ms > 10)
	 {
		  timer_50ms = 0;
		 
			//LED_One_TOGGLE;   //�����ƣ�1s��˸1��
	 }
 }
void Loop_5Hz(void)
{

	
}

void Loop(void)
{
	if(schedule.cnt_1ms >= 1)
	{
		Loop_1000Hz();	
		schedule.cnt_1ms = 0;
	}
	if(schedule.cnt_2ms >= 2)
	{
		Loop_500Hz();
		schedule.cnt_2ms = 0;
	}
	if(schedule.cnt_5ms >= 5)
	{
		Loop_200Hz();
		schedule.cnt_5ms = 0;
	}
	if(schedule.cnt_10ms >= 10)
	{
		Loop_100Hz();
		schedule.cnt_10ms = 0;
	}
	if(schedule.cnt_20ms >= 20)	
	{
		Loop_50Hz();
		schedule.cnt_20ms = 0;
	}
	if(schedule.cnt_50ms >= 50)
	{
		Loop_20Hz();
		schedule.cnt_50ms = 0;
	}
	
	
	///////////////////////////////////////////
	if(loop_5hz >= 1000)
	{
		Loop_5Hz();
		loop_5hz=0;
	}
}
