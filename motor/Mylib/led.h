#ifndef __LED_H__
#define __LED_H__

#include "board.h"

#define  LED_One_ON      LED_PORT->BSRRH = LED_Pin_One
#define  LED_One_OFF     LED_PORT->BSRRL = LED_Pin_One
#define  LED_One_TOGGLE  LED_PORT->ODR ^= LED_Pin_One

#define  LED_Two_ON        LED_PORT->BSRRH = LED_Pin_Two
#define  LED_Two_OFF       LED_PORT->BSRRL = LED_Pin_Two
#define  LED_Two_TOGGLE    LED_PORT->ODR ^= LED_Pin_Two

#define  LED_Three_ON        LED_PORT->BSRRH = LED_Pin_Three
#define  LED_Three_OFF       LED_PORT->BSRRL = LED_Pin_Three
#define  LED_Three_TOGGLE    LED_PORT->ODR ^= LED_Pin_Three

#define  LED_Four_ON        LED_PORT->BSRRH = LED_Pin_Four
#define  LED_Four_OFF       LED_PORT->BSRRL = LED_Pin_Four
#define  LED_Four_TOGGLE    LED_PORT->ODR ^= LED_Pin_Four


enum  {
				LED_GREEN,
				LED_RED,
				LED_BLUE,
				OUT_LED_RED,
				OUT_LED_GREEN
			};

void Led_Init(void);

#endif
