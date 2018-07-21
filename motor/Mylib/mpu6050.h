#ifndef _MPU6050_H
#define	_MPU6050_H

#include "board.h"

#define ANGLE_TO_RAD 0.01745329f

#define TO_ANGLE     0.061036f

#define MPU6050_FILTER_NUM   10

#define A_X    0
#define A_Y    1
#define A_Z    2
#define G_X    3
#define G_Y    4
#define G_Z    5																	

#define ITEMS  6

struct _float{
	      float x;
	 	    float y;
		    float z;
             };

struct _int16{
         int16_t x;
	       int16_t y;
	       int16_t z;
             };		

struct _trans{
       struct _int16 origin;      //ԭʼֵ
			 struct _float offset_max;  //��ƫֵ���ֵ
			 struct _float offset_min;  //��ƫֵ��Сֵ	
	     struct _float offset;      //��ƫֵ 
		   struct _float calibration; //У׼ֵ
	     struct _float filter;      //����ƽ���˲�ֵ
	     struct _float dps;         //��ÿ�� 
	     struct _float radps;       //����ÿ��
              };

struct _sensor{   
	   struct _trans acc;
	   struct _trans gyro;
              };

extern struct _sensor sensor;


//��ʼ��6050
void Mpu6050_Init(uint16_t sample_rate, uint16_t lpf);
//��ȡ���ٶ�
void Mpu6050_Read_Acc_Data(void);
//��ȡ���ٶ�
void Mpu6050_Read_Gyro_Data(void);
//��ȡ���ٶȺͽ��ٶ�����
void Mpu6050_Read(void);
//���ٶ���ƫ����
void Mpu6050_CalOffset_Acc(void);
//��������ƫ����
void Mpu6050_CalOffset_Gyro(void);
//mpu6050����׼��
void Mpu6050_Data_Prepare(void);


#endif
