#include "hmc5883.h"

struct _mag mag;
u8 hmc5883_buffer[6];

/**************************************
//��ʼ��Hmc5883
**************************************/
void Hmc5883_Init(void)
{
	 I2C_Soft_Single_Write(HMC5883L_ADDRESS,HMC5883L_MODE,0x00);
}
/**************************ʵ�ֺ���********************************************
//��iic��ȡ�������ݷֲ�,������Ӧ�Ĵ���,����hmc5883_Last
******************************************************************************/
void Hmc5883_Read(void)
{
   hmc5883_buffer[0]=I2C_Soft_Single_Read(HMC5883L_ADDRESS, HMC5883L_DATA_X_H);
	 hmc5883_buffer[1]=I2C_Soft_Single_Read(HMC5883L_ADDRESS, HMC5883L_DATA_X_L);
	 hmc5883_buffer[2]=I2C_Soft_Single_Read(HMC5883L_ADDRESS, HMC5883L_DATA_Z_H);
	 hmc5883_buffer[3]=I2C_Soft_Single_Read(HMC5883L_ADDRESS, HMC5883L_DATA_Z_L);
	 hmc5883_buffer[4]=I2C_Soft_Single_Read(HMC5883L_ADDRESS, HMC5883L_DATA_Y_H);
	 hmc5883_buffer[5]=I2C_Soft_Single_Read(HMC5883L_ADDRESS, HMC5883L_DATA_Y_L);
}
/**************************ʵ�ֺ���********************************************
//��iic��ȡ�������ݷֲ�,������Ӧ�Ĵ���
*******************************************************************************/
void Get_Mag(void)
{   
	  Hmc5883_Read();
	
	  mag.origin.x=(hmc5883_buffer[0]<<8)|hmc5883_buffer[1];
	  mag.origin.z=-((hmc5883_buffer[2]<<8)|hmc5883_buffer[3]);
	  mag.origin.y=-((hmc5883_buffer[4]<<8)|hmc5883_buffer[5]);
	
	  if(mag.origin.x>0x7fff) mag.origin.x-=0xffff;
		if(mag.origin.y>0x7fff) mag.origin.y-=0xffff;
	  if(mag.origin.z>0x7fff) mag.origin.z-=0xffff;
}
/**************************ʵ�ֺ���********************************************
//ԭʼ����ת����k_l��k_h
*******************************************************************************/
int moxin(int origin,int max,int min,int k_h,int k_l)
{
    int out;
	  out=(k_h-k_l)*(origin-min)/(max-min)+k_l;
	  return out;
}
/**************************ʵ�ֺ���********************************************
//��ȡ��ǰ����
*******************************************************************************/
void Get_Direction(float T)
{ 
	  float ctpx=cos((float)angle.pitch/57.3f);
		float stpx=sin((float)angle.pitch/57.3f);
	  float ctpy=cos((float)angle.roll/57.3f);
	  float stpy=sin((float)angle.roll/57.3f);
/*������ԭʼֵ��ȡ----------------------*/	
    Get_Mag();
/*��Բ��ת�Ƶ���0,0��-------------------*/		
    mag.moxin.x=(float)(mag.origin.x*mag.gain.x-mag.offset.x);
    mag.moxin.y=(float)(mag.origin.y*mag.gain.y-mag.offset.y);
    mag.moxin.z=(float)(mag.origin.z*mag.gain.z-mag.offset.z);
/*��������ϵ�Ĵų�*/  
	  mag.out.x=mag.moxin.x*ctpx+mag.moxin.y*stpy*stpx-mag.moxin.z*ctpy*stpx;
	  mag.out.y=mag.moxin.y*ctpy+mag.moxin.z*stpy;
/*heading�ɴ����Ƹ���*/  
		mag.heading=-fast_atan2((double)mag.out.y,(double)mag.out.x)*57.3f + mag.heading_offset;
		if(mag.heading<-180) mag.heading+=360;
		if(mag.heading>180)  mag.heading-=360;	
/*heading_filter*/  
	  if(mag.heading>-180&&mag.heading<180)
		mag.heading_filter = mag.heading;
}

/**************************ʵ�ֺ���********************************************
//������У׼
*******************************************************************************/
void Mag_Correct(void)
{
	  mag.max.x=395;
	  mag.max.y=377;
	  mag.max.z=362;
	  mag.min.x=-182;
	  mag.min.y=-174;
	  mag.min.z=-233;
/*���������ֵ��Сֵ*/
// 	  mag.max.x=-32768;
// 	  mag.max.y=-32768;
// 	  mag.max.z=-32768;
// 	  mag.min.x=32767;
// 	  mag.min.y=32767;
// 	  mag.min.z=32767;	
// 	
// 	  if(RC.CH[3] <= 0)
// 		{
// 			while(!Param_Read());
// 				LED_RED_ON;
// 			  DelayMs(1000);
// 			  LED_RED_OFF;
// 		}
// 		else if(RC.CH[3] > 620)
// 		{
// 			while(1)
// 			{
// 				  LED_RED_ON;
// 					Get_Mag();
// 					DelayMs(10);
// 				  if(mag.origin.x>mag.max.x) mag.max.x = mag.origin.x;
// 				  if(mag.origin.y>mag.max.y) mag.max.y = mag.origin.y;
// 					if(mag.origin.x<mag.min.x) mag.min.x = mag.origin.x;
// 				  if(mag.origin.y<mag.min.y) mag.min.y = mag.origin.y;
// 				  if(RC.CH[3]<-650)
// 					{
// 						LED_RED_OFF;
// 						break;
// 					}
// 			}
// 			while(!(RC.CH[3]>620));
// 			while(1)
// 			{
// 				  LED_RED_ON;
// 					Get_Mag();
// 					DelayMs(10);
// 				  if(mag.origin.z>mag.max.z) mag.max.z = mag.origin.z;
// 					if(mag.origin.z<mag.min.z) mag.min.z = mag.origin.z;
// 				  if(RC.CH[3]<-650)
// 					{
// 						while(!Param_Save());
// 						LED_RED_OFF;
// 						break;
// 					}
// 			}
// 		}
			
/*��������*/		
		mag.gain.x=1.0f;
		mag.gain.y=(float)(mag.max.x-mag.min.x)/(float)(mag.max.y-mag.min.y);
		mag.gain.z=(float)(mag.max.x-mag.min.x)/(float)(mag.max.z-mag.min.z);
/*��ƫ����*/
		mag.offset.x=(mag.max.x+mag.min.x)*0.5f*mag.gain.x;
		mag.offset.y=(mag.max.y+mag.min.y)*0.5f*mag.gain.y;
		mag.offset.z=(mag.max.z+mag.min.z)*0.5f*mag.gain.z;
/*ʹ��ͷָ�򱱷�Ϊ0�ȣ���90�ȣ���-90��*/	
	  mag.heading_offset = 175.0f;
}
