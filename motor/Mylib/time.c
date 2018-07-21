#include "time.h"

volatile float Cycle_T[GET_TIME_NUM][3];


/* 
	 0:loop_time_500hz
   1:loop_time_200hz
	 2:loop_time_100hz
	 3:loop_time_50hz
	 4:loop_time_20hz
	 5:dbus_loop_time
	 6:gps_loop_time
	 7:Extra
	 8:Extra
	 9:Extra
*/
float Get_Cycle_T(u8 item)	
{
	Cycle_T[item][OLD] = Cycle_T[item][NOW];	//��һ�ε�ʱ��
	Cycle_T[item][NOW] = GetSysTime_us()/1000000.0f; //���ε�ʱ��
	Cycle_T[item][NEW] = ( ( Cycle_T[item][NOW] - Cycle_T[item][OLD] ) );//�����ʱ�䣨���ڣ�
	return Cycle_T[item][NEW];
}

void Cycle_Time_Init()
{
	u8 i;
	for(i=0;i<GET_TIME_NUM;i++)
	{
		Get_Cycle_T(i);
	}
}
