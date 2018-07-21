#ifndef __GPS_H
#define __GPS_H

#include "board.h"

#define GPS_BUF_LEN      2048
#define LATLON_TO_CM     11319500       //γ�ȣ��ȣ�ת����   PI*RADIUS_OF_EARTH/180  
#define FILTER_K1        0.2f

#define MAX_GPS_SPEED    250.0f

struct _gps 
{		
	double lat;			//γ��
	double lon;			//����
	double homelat; //��γ��
	double homelon; //�Ҿ���
	double rellat;  //γ���������
	double rellon;  //�����������
	double leader_lat;//leader��γ��
	double leader_lon;//leader�ľ���
	double disx_cm_to_leader;//��leader��x����
	double disy_cm_to_leader;//��leader��y����
	double rellat_aim;//Ŀ�����γ��
	double rellon_aim;//Ŀ����Ծ���
	double speed;   //�ƶ�����
	double azimuth; //�ƶ�����
	float  latacc;  //γ�ȷ�����ٶ�
	float  lonacc;	//���ȷ�����ٶ�
	float  latvel;  //γ�ȷ����ٶ�
	float  lonvel;  //���ȷ����ٶ�
	float  leader_pitch;//leader����
	float  leader_roll; //leader���
	float  leader_yaw;  //leaderƫ��
	float  leader_latvel;//leaderγ�ȷ�����ٶ�
	float  leader_lonvel;//leader���ȷ�����ٶ�
	float  leader_hight; //leader�߶�
	u8 homeisok;		//��λ��ȷ��				 
};
		
struct _GPSINFO
{
	u8 UtcTime[11]; 	//ʱ��
	u8 Statue;   			//��λ״̬
	u8 Latitude[10];	//γ��
	u8 LatitudeNS; 		//γ�Ȱ���
	u8 Longitude[11];	//����
	u8 LongitudeEW;		//���Ȱ���
	u8 Speed[6];	    //��������
	u8 Azimuth[6];	  //����
	u8 UtcData[7]; 		//����  
	u8 Altitude[8];   //�߶�
};

struct _xy_efdata
			 {
				 double base;
				 double dis_cm;
				 double dif;
       };
			 
struct _xy_ef
			 {
				 struct _xy_efdata accx;
				 struct _xy_efdata accy;
				 struct _xy_efdata velx;
				 struct _xy_efdata vely;
         struct _xy_efdata posx;
				 struct _xy_efdata posy;
			 };
			 		 
struct _gps_control
			{
				float exp;
				float err;
				float out_p;
				float out_i;
				float out_d;
				float out;
				float kp;
				float ki;
				float kd;
				float inc_max;
				double rc;
			};

struct _gps_pid_level
      {
         struct _gps_control vel_x;
				 struct _gps_control vel_y;
				 struct _gps_control pos_x;
				 struct _gps_control pos_y;
				 float PITCH_OUT;
				 float ROLL_OUT;
      };
			
extern struct _gps_pid_level gpslevelcon;
extern struct _gps gps;
extern struct _GPSINFO GpsInfo;
extern struct _xy_ef xy_ef;			 
			 
extern u8 gps_rx_buffer[GPS_BUF_LEN];

u16 FindStr(u8 *str,u8 *ptr);
void GPSParse(void);
double LatToRad(void);
double LonToRad(void);
double GetSpeed(void);
double GetAzimuth(void);
void GPS_Read(void);
void Update_GPS_states(float T);
void Push_Pos_Dis(double x ,double y);
void Cal_Distance_to_Leader(void);
void Update_Leader_State(float T);			
void GPSlevel_Velocity_Control(float T);
void GPSlevel_Position_Control(float T);		
		

#endif
