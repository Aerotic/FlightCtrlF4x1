#ifndef __HMC5883_H_
#define __HMC5883_H_

#include "board.h"

//****************************************
// ����HMC5883L�ڲ���ַ
//****************************************
#define HMC5883L_CONFIG_A 0x00
#define HMC5883L_CONFIG_B 0x01
#define HMC5883L_MODE     0x02
#define HMC5883L_DATA_X_H 0x03
#define HMC5883L_DATA_X_L 0x04
#define HMC5883L_DATA_Z_H 0x05
#define HMC5883L_DATA_Z_L 0x06
#define HMC5883L_DATA_Y_H 0x07
#define HMC5883L_DATA_Y_L 0x08
#define HMC5883L_STATUS   0x09
#define HMC5883L_IDENTIFICATION_A 0x0A
#define HMC5883L_IDENTIFICATION_B 0x0B
#define HMC5883L_IDENTIFICATION_C 0x0C

#define HMC5883L_ADDRESS 0x3C


struct _int{
				int16_t x;
				int16_t y;
				int16_t z;};

struct _FLOAT{
				float x;
				float y;
				float z;};

struct _mag{   
			 struct _int origin;	//ԭʼֵ
	     struct _FLOAT moxin; //��һ��ֵ
			 struct _FLOAT out;   //���ֵ
			 struct _int max;   	//���ֵ
	     struct _int min;   	//��Сֵ
	     struct _FLOAT gain;  //����ϵ��
	     struct _FLOAT offset;//Բ��λ��
	     float heading;     	//����
	     float heading_filter;//�����˲�
	     float heading_offset;//��֤��ͷ���򱱷�����Ϊ0
           };

extern struct _mag mag;	

void Hmc5883_Init(void);
void Hmc5883_Read(void);
void Get_Mag(void);
int moxin(int origin,int max,int min,int k_h,int k_l);
void Get_Direction(float T);
void Mag_Correct(void);

#endif
