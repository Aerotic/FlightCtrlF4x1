#include "board.h"
#include "led.h"
int main(void)
{
	//初始化
  FlightController_board_Init();
//	LED_BLUE_ON;
//	Led_Init();
	LED_One_OFF;
	LED_Two_OFF;
	LED_Three_OFF;
	LED_Four_OFF;
	while(1)
	{		
		

//		Uart_Wave_Send();      //串口1波形数据推送
//		Uart_Attitude_Send();


  }
}



