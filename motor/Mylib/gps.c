#include "gps.h"

struct _gps_pid_level gpslevelcon;
struct _gps gps;
struct _GPSINFO GpsInfo;
struct _xy_ef xy_ef;

u8 gps_rx_buffer[GPS_BUF_LEN];

u16 FindStr(u8 *str,u8 *ptr)
{
	u16 index=0;
	u8 *STemp=NULL;
	u8 *PTemp=NULL;
	u8 *MTemp=NULL;
	if(0==str||0==ptr)
		return 0;
	for(STemp=str;*STemp!='\0';STemp++)	 //���β����ַ���
	{
		index++;  //��ǰƫ������1
		MTemp=STemp; //ָ��ǰ�ַ���
		//�Ƚ�
		for(PTemp=ptr;*PTemp!='\0';PTemp++)
		{	
			if(*PTemp!=*MTemp)
			break;
			MTemp++;
		}
		if(*PTemp=='\0')  //�����������ҵ��ַ������˳�
			break;
	}
	return index;
}

void GPSParse(void)
{
 	u8 CommaNum=0; //������
	u8 BufIndex=0; //������
 	u8 Sbuf;
	u8 *Pstr;
	u16 index;
	index= FindStr(gps_rx_buffer,"$GNRMC,");//����
	if(index)
	{
		CommaNum=0;
		Pstr=gps_rx_buffer+index+6;	 //�ҵ�GPRMC,����ĵ�ַ
		do
		{
			Sbuf=*Pstr++;	
			switch(Sbuf)
			{
				case ',':CommaNum++;  //ͨ�����ŵ���Ŀ������״̬����
						 BufIndex=0;
						 break;
				default:
						switch(CommaNum)
						{
							case 0:GpsInfo.UtcTime[BufIndex]=Sbuf;break; 
							case 1:GpsInfo.Statue=Sbuf;break;
							case 2:GpsInfo.Latitude[BufIndex]=Sbuf;break;
							case 3:GpsInfo.LatitudeNS=Sbuf;break;
							case 4:GpsInfo.Longitude[BufIndex]=Sbuf;break;
							case 5:GpsInfo.LongitudeEW=Sbuf;break;
							case 6:GpsInfo.Speed[BufIndex]=Sbuf;break;
							case 7:GpsInfo.Azimuth[BufIndex]=Sbuf;break;
							case 8:GpsInfo.UtcData[BufIndex]=Sbuf;break;
							default:break;
						}
						BufIndex++;
						break;
			}
		}while(Sbuf!='*'&&Pstr<gps_rx_buffer+GPS_BUF_LEN);
	}	
}

double LatToRad(void)
{
 	double Rad;
	u16 Data;
	Data=(GpsInfo.Latitude[0]-0x30)*10+(GpsInfo.Latitude[1]-0x30);
	Rad=(GpsInfo.Latitude[2]-0x30)*10+(GpsInfo.Latitude[3]-0x30)+(GpsInfo.Latitude[5]-0x30)*0.1+(GpsInfo.Latitude[6]-0x30)*0.01+(GpsInfo.Latitude[7]-0x30)*0.001+(GpsInfo.Latitude[8]-0x30)*0.0001+(GpsInfo.Latitude[9]-0x30)*0.00001;
	Rad=Rad/60;
	Rad=Rad+Data;
	return Rad;			//xx��
}
 
double LonToRad(void)
{
 	double Rad;
	u16 Data;
	Data=(GpsInfo.Longitude[0]-0x30)*100+(GpsInfo.Longitude[1]-0x30)*10+(GpsInfo.Longitude[2]-0x30);
	Rad=(GpsInfo.Longitude[3]-0x30)*10+(GpsInfo.Longitude[4]-0x30)+(GpsInfo.Longitude[6]-0x30)*0.1+(GpsInfo.Longitude[7]-0x30)*0.01+(GpsInfo.Longitude[8]-0x30)*0.001+(GpsInfo.Longitude[9]-0x30)*0.0001+(GpsInfo.Longitude[10]-0x30)*0.00001;
	Rad=Rad/60;
	Rad=Rad+Data;
	return Rad;			//xx��
}

double GetSpeed(void)
{
	u8 i,index=0;
	double speed=0;

	for(i=0;i<6;i++)
	{
      if(GpsInfo.Speed[i]!='.'&&GpsInfo.Speed[i]!=0x00)
         speed=speed*10+(GpsInfo.Speed[i]-0x30);
			if(GpsInfo.Speed[i]=='.')
				 index=i;
  }
	switch(index)
	{
			case 1:speed=speed/10000.0f;break;
			case 2:speed=speed/1000.0f;break;
			case 3:speed=speed/100.0f;break;
			case 4:speed=speed/10.0f;break;
      default:break;
  }
	speed = speed*185.2f/3.6f;//����ÿ��
	return speed;
}

double GetAzimuth(void)
{
	u8 i,index=0;
	double azimuth=0;

	for(i=0;i<6;i++)
	{
      if(GpsInfo.Azimuth[i]!='.'&&GpsInfo.Azimuth[i]!=0x00)
         azimuth=azimuth*10+(GpsInfo.Azimuth[i]-0x30);
			if(GpsInfo.Azimuth[i]=='.')
				 index=i;
  }
	switch(index)
	{
			case 1:azimuth=azimuth/10000.0f;break;
			case 2:azimuth=azimuth/1000.0f;break;
			case 3:azimuth=azimuth/100.0f;break;
			case 4:azimuth=azimuth/10.0f;break;
      default:break;
  }
	if(azimuth>180) azimuth-=360;
	
	return azimuth;
}

void GPS_Read(void)
{
  GPSParse();
	gps.lat = LatToRad();
	gps.lon = LonToRad();
	gps.speed = GetSpeed();//��������
	gps.azimuth = GetAzimuth();//���溽�� ��0 ��90 ��-90
	
	gps.latvel = gps.speed*cos(gps.azimuth/57.3f);
	gps.lonvel = gps.speed*sin(gps.azimuth/57.3f);
	
	if(GpsInfo.Statue == 'A' && gps.homeisok)//��Ч��λ��home�Ѿ�ȷ��
	{
			gps.rellat = gps.lat - gps.homelat;//�����home��γ�ȣ������ϸ�
			gps.rellon = gps.lon - gps.homelon;//�����home�ľ��ȣ���������
			LED_BLUE_ON;
	}
	else 
				LED_BLUE_OFF;
	
	if(gps.lat!=0 && gps.lon!=0 && GpsInfo.Statue == 'A' && !gps.homeisok)
	{
			gps.homelat = gps.lat;//homeγ��
			gps.homelon = gps.lon;//home����
			gps.homeisok = 1;//home�Ѿ�ȷ��
  }
}

double posx_dis_cm_buff[30];
double posy_dis_cm_buff[30];

extern float ux, uy, uz;//(rϵ��bϵ�ĵ�һ��)
extern float wx, wy, wz;//(rϵ��bϵ�ĵڶ���)
extern float vx, vy, vz;//(rϵ��bϵ�ĵ�����)

void Update_GPS_states(float T)
{
	  /*����ο�����ϵ�ļ��ٶȣ���������ϵ�ļ��ٶ�*/
	  float accx_ef = 0,accy_ef = 0; 
	  float accx_bf = 0,accy_bf = 0;
	
		/*��γ��ת�ɾ��루���ף�*/
		xy_ef.posx.dis_cm = gps.rellon *LATLON_TO_CM *cos(gps.lat/57.3f);
	  xy_ef.posy.dis_cm = gps.rellat *LATLON_TO_CM;

		/*���루���ף���������*/
		Push_Pos_Dis(xy_ef.posx.dis_cm , xy_ef.posy.dis_cm);

    /*����΢��*/
		xy_ef.posx.dif = (xy_ef.posx.dis_cm - posx_dis_cm_buff[29])/(29*T);
		xy_ef.posy.dif = (xy_ef.posy.dis_cm - posy_dis_cm_buff[29])/(29*T);
	
		/*�ο�����ϵ�ļ��ٶȣ��ο�����ϵ�ļ��ٶȣ���ͷĬ�ϱ���������ı������Ǿ��Եı�����*/
		accy_ef = (sensor.acc.filter.x*ux + sensor.acc.filter.y*uy + sensor.acc.filter.z*uz)*980.0f/4096.0f; //�����ϸ�
		accx_ef = -(sensor.acc.filter.x*wx + sensor.acc.filter.y*wy + sensor.acc.filter.z*wz)*980.0f/4096.0f; //��������	 
		
		/*�ɻ����ٶȣ���ͷ����Ϊy���ҷ�Ϊx*/    /*ˮƽ������ٶ�״̬*/
		accx_bf =  accx_ef * cos(angle.yaw/57.3f) + accy_ef * sin(angle.yaw/57.3f);//������
		accy_bf = -accx_ef * sin(angle.yaw/57.3f) + accy_ef * cos(angle.yaw/57.3f);//ǰ����	
		
		/*�õ���������ϵ��earth frame���µ�ˮƽ���ٶ�*/    /*ˮƽ������ٶ�״̬*/
		xy_ef.accx.base = accx_bf * cos(mag.heading_filter/57.3f) - accy_bf * sin(mag.heading_filter/57.3f);//��������
	  xy_ef.accy.base = accx_bf * sin(mag.heading_filter/57.3f) + accy_bf * cos(mag.heading_filter/57.3f);//�����ϸ�

		/*�����˲��ٶȸ���*/
		xy_ef.velx.base = FILTER_K1 * xy_ef.posx.dif+ (1-FILTER_K1) * (xy_ef.velx.base + xy_ef.accx.base * T);
		xy_ef.vely.base = FILTER_K1 * xy_ef.posy.dif+ (1-FILTER_K1) * (xy_ef.vely.base + xy_ef.accy.base * T);
}

void Push_Pos_Dis(double x ,double y)
{
    u8 i;
	  for(i=29;i>0;i--)
		{
			posx_dis_cm_buff[i] = posx_dis_cm_buff[i-1];
			posy_dis_cm_buff[i] = posy_dis_cm_buff[i-1]; 	
		}
		posx_dis_cm_buff[0] = x;
		posy_dis_cm_buff[0] = y;
}

void Cal_Distance_to_Leader(void)
{
	  if(RADIO_RX_DATA[0] == 0x88 && RADIO_RX_DATA[1] == 0xA1)
		{
				gps.leader_lat=(double)((RADIO_RX_DATA[3] << 24) | (RADIO_RX_DATA[4] << 16) | (RADIO_RX_DATA[5] << 8) | RADIO_RX_DATA[6])/10000000.0;
				gps.disy_cm_to_leader=(gps.leader_lat - gps.lat)*LATLON_TO_CM;

				gps.leader_lon=(double)((RADIO_RX_DATA[7] << 24) | (RADIO_RX_DATA[8] << 16) | (RADIO_RX_DATA[9] << 8) | RADIO_RX_DATA[10])/10000000.0;
				gps.disx_cm_to_leader=(gps.leader_lon - gps.lon)*LATLON_TO_CM *cos((gps.lat)/57.3f);
		}
}

void Update_Leader_State(float T)
{
		float Follower_position_exp_x=0,Follower_position_exp_y=0;
	  static float Follower_position_tra_x=0,Follower_position_tra_y=0;
	
	  if(RADIO_RX_DATA[0] == 0x88 && RADIO_RX_DATA[1] == 0xA1)
		{	
			  /*��������λ�ø���*/
			  switch(RADIO_RX_DATA[23])
				{
						case 1:Follower_position_exp_x=0;Follower_position_exp_y=0;break;
						case 2:Follower_position_exp_x=2000;Follower_position_exp_y=-2000;break;
						case 3:Follower_position_exp_x=2000;Follower_position_exp_y=-4000;break;
						default:Follower_position_exp_x=0;Follower_position_exp_y=0;break;
				}
				/*��ǰ�켣λ�ø��£�50msִ��1�Σ���Ӧ2m/s*/
				if(Follower_position_tra_x>Follower_position_exp_x)
					 Follower_position_tra_x-=10;
				else if(Follower_position_tra_x<Follower_position_exp_x)
					 Follower_position_tra_x+=10;
				
				if(Follower_position_tra_y>Follower_position_exp_y)
					 Follower_position_tra_y-=10;
				else if(Follower_position_tra_y<Follower_position_exp_y)
					 Follower_position_tra_y+=10;
			
				/*�궨λ��*/
				gpslevelcon.pos_x.rc += (RC.CH[0]*MAX_GPS_SPEED/660 * T * cos(mag.heading_filter/57.3f) - RC.CH[1]*MAX_GPS_SPEED/660 * T * sin(mag.heading_filter/57.3f));
			  gpslevelcon.pos_y.rc += (RC.CH[1]*MAX_GPS_SPEED/660 * T * cos(mag.heading_filter/57.3f) + RC.CH[0]*MAX_GPS_SPEED/660 * T * sin(mag.heading_filter/57.3f));			
				
				gps.leader_lat=(double)((RADIO_RX_DATA[3] << 24) | (RADIO_RX_DATA[4] << 16) | (RADIO_RX_DATA[5] << 8) | RADIO_RX_DATA[6])/10000000.0;
				gpslevelcon.pos_y.exp = (gps.leader_lat - gps.homelat) *LATLON_TO_CM - gps.disy_cm_to_leader + Follower_position_tra_y + gpslevelcon.pos_y.rc;

				gps.leader_lon=(double)((RADIO_RX_DATA[7] << 24) | (RADIO_RX_DATA[8] << 16) | (RADIO_RX_DATA[9] << 8) | RADIO_RX_DATA[10])/10000000.0;
				gpslevelcon.pos_x.exp = (gps.leader_lon - gps.homelon)*LATLON_TO_CM *cos((gps.lat)/57.3f) - gps.disx_cm_to_leader + Follower_position_tra_x + gpslevelcon.pos_x.rc;

//leader�ĸ߶�
			  gps.leader_hight = (RADIO_RX_DATA[11] << 8) | RADIO_RX_DATA[12];
				if(gps.leader_hight>0x7fff) gps.leader_hight-=0xffff;
//leader����̬			
			  gps.leader_pitch = (RADIO_RX_DATA[13] << 8) | RADIO_RX_DATA[14];
				gps.leader_roll = (RADIO_RX_DATA[15] << 8) | RADIO_RX_DATA[16];
				gps.leader_yaw = (RADIO_RX_DATA[17] << 8) | RADIO_RX_DATA[18];
			
			  if(gps.leader_pitch>0x7fff) gps.leader_pitch-=0xffff;
			  if(gps.leader_roll>0x7fff) gps.leader_roll-=0xffff;
				if(gps.leader_yaw>0x7fff) gps.leader_yaw-=0xffff;
			
 			  gps.leader_pitch = LIMIT(gps.leader_pitch/10.0f,-20,20);
 			  gps.leader_roll = LIMIT(gps.leader_roll/10.0f,-20,20);
				gps.leader_yaw = LIMIT(gps.leader_yaw/10.0f,-180,180);

//leader���ٶ�
				gps.leader_lonvel = (RADIO_RX_DATA[19] << 8) | RADIO_RX_DATA[20];
			  gps.leader_latvel = (RADIO_RX_DATA[21] << 8) | RADIO_RX_DATA[22];
			
				if(gps.leader_lonvel>0x7fff) gps.leader_lonvel-=0xffff;
			  if(gps.leader_latvel>0x7fff) gps.leader_latvel-=0xffff;
			
			 	gps.leader_lonvel = LIMIT(gps.leader_lonvel/10.0f,-200,200);	
 			  gps.leader_latvel = LIMIT(gps.leader_latvel/10.0f,-200,200);
		}
}

void GPSlevel_Velocity_Control(float T)
{
	  float velx_ef_diff = LPF2pApply_5(xy_ef.accx.base);
	  float vely_ef_diff = LPF2pApply_6(xy_ef.accy.base);
		if(RC.CH[5]==1||RC.CH[5]==2)
		{
			if(RC.CH[5]==2)//����ģʽ��leader����̬���ٶ���0
			{
				gps.leader_pitch = 0;
				gps.leader_roll = 0;
				gps.leader_lonvel = 0;
				gps.leader_latvel = 0;
			}
			
			gpslevelcon.vel_x.err = gpslevelcon.pos_x.out - xy_ef.velx.base;
			gpslevelcon.vel_y.err = gpslevelcon.pos_y.out - xy_ef.vely.base;
			
			gpslevelcon.vel_x.out_p = gpslevelcon.vel_x.err * gpslevelcon.vel_x.kp;
			gpslevelcon.vel_x.out_d = -velx_ef_diff * gpslevelcon.vel_x.kd;
			gpslevelcon.vel_x.out = gpslevelcon.vel_x.out_p + gpslevelcon.vel_x.out_d;
			gpslevelcon.vel_x.out = LIMIT(gpslevelcon.vel_x.out , -15.0f , 15.0f);
			
			gpslevelcon.vel_y.out_p = gpslevelcon.vel_y.err * gpslevelcon.vel_y.kp;
			gpslevelcon.vel_y.out_d = -vely_ef_diff * gpslevelcon.vel_y.kd;
			gpslevelcon.vel_y.out = gpslevelcon.vel_y.out_p + gpslevelcon.vel_y.out_d;
			gpslevelcon.vel_y.out = LIMIT(gpslevelcon.vel_y.out , -15.0f , 15.0f);
			
			/*�ڲο�����ϵ����PID����������ת����������ϵ��mag.heading_filter�Ա�Ϊ0��������תΪ�����˴���0.4fΪleader����̬ǰ��ϵ��*/
			gpslevelcon.PITCH_OUT = gps.leader_pitch*0.6f-gpslevelcon.vel_x.out*sin(mag.heading_filter/57.3f) + gpslevelcon.vel_y.out*cos(mag.heading_filter/57.3f);
			gpslevelcon.ROLL_OUT = gps.leader_roll*0.6f+gpslevelcon.vel_x.out*cos(mag.heading_filter/57.3f) + gpslevelcon.vel_y.out*sin(mag.heading_filter/57.3f);
		
			gpslevelcon.PITCH_OUT = LIMIT(gpslevelcon.PITCH_OUT,-20.0f,20.0f);
			gpslevelcon.ROLL_OUT = LIMIT(gpslevelcon.ROLL_OUT,-20.0f,20.0f);

		}
		if(RC.CH[5]==3)
		{
			gpslevelcon.PITCH_OUT = 0;
			gpslevelcon.ROLL_OUT = 0;
			gpslevelcon.pos_x.rc = 0;
			gpslevelcon.pos_y.rc = 0;
		} 	
}

void GPSlevel_Position_Control(float T)
{
		/*����ģʽ�����仯*/
    if(RC.CH[5]!=RC.CH_LAST[5])
		{  
       if(RC.CH[5]==2)  //����ģʽ,���ڵ�ǰλ��
			 { 
				gpslevelcon.pos_x.exp = xy_ef.posx.dis_cm;
				gpslevelcon.pos_y.exp = xy_ef.posy.dis_cm;
			 }
			 if(RC.CH[5]==1)  //����ģʽ��������leader֮��ľ���
			 {
				Cal_Distance_to_Leader();
       }
    }	  
		if(RC.CH[5]==1)//����ģʽ�£�����leader״̬
		{
			Update_Leader_State(T);
    }
		if(RC.CH[5]==1||RC.CH[5]==2)
		{
			/*��ң��������������λ��ƫ����*/
			gpslevelcon.pos_x.exp += (RC.CH[0]*MAX_GPS_SPEED/660 * T * cos(mag.heading_filter/57.3f) - RC.CH[1]*MAX_GPS_SPEED/660 * T * sin(mag.heading_filter/57.3f));
			gpslevelcon.pos_y.exp += (RC.CH[1]*MAX_GPS_SPEED/660 * T * cos(mag.heading_filter/57.3f) + RC.CH[0]*MAX_GPS_SPEED/660 * T * sin(mag.heading_filter/57.3f));
			
			gpslevelcon.pos_x.err = gpslevelcon.pos_x.exp - xy_ef.posx.dis_cm;//cm
			gpslevelcon.pos_y.err = gpslevelcon.pos_y.exp - xy_ef.posy.dis_cm;//cm

			/*���100cm����55cm/s���ٶ���Ŀ��λ�ÿ����������ٶ���[-300cm/s,300cm/s]*/
			gpslevelcon.pos_x.out_p = gpslevelcon.pos_x.err * gpslevelcon.pos_x.kp;
			gpslevelcon.pos_x.out_i += gpslevelcon.pos_x.err * gpslevelcon.pos_x.ki * T;
			gpslevelcon.pos_x.out_i = LIMIT(gpslevelcon.pos_x.out_i , -80.0f , 80.0f);
			gpslevelcon.pos_x.out = gpslevelcon.pos_x.out_p + gpslevelcon.pos_x.out_i;
			gpslevelcon.pos_x.out = LIMIT(gpslevelcon.pos_x.out , -300.0f , 300.0f);
			
			gpslevelcon.pos_y.out_p = gpslevelcon.pos_y.err * gpslevelcon.pos_y.kp;
			gpslevelcon.pos_y.out_i += gpslevelcon.pos_y.err * gpslevelcon.pos_y.ki * T;
			gpslevelcon.pos_y.out_i = LIMIT(gpslevelcon.pos_y.out_i , -80.0f , 80.0f);
			gpslevelcon.pos_y.out = gpslevelcon.pos_y.out_p + gpslevelcon.pos_y.out_i;
			gpslevelcon.pos_y.out = LIMIT(gpslevelcon.pos_y.out , -300.0f , 300.0f);
		}
		if(RC.CH[5]==3)
		{
			  gpslevelcon.pos_x.out_i = 0;
				gpslevelcon.pos_y.out_i = 0;
				gpslevelcon.pos_x.out = 0;
			  gpslevelcon.pos_y.out = 0;
    }
		RC.CH_LAST[5]=RC.CH[5];
}
