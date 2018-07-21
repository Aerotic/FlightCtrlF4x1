#include "inertialNav.h"

struct _z_ef z_ef;

extern float vx, vy, vz;//(rϵ��bϵ�ĵ�����)

int32_t baroAlt_buff[15];

void Update_Velocities_Positions_Z(float T)
{
		float baroAlt_dif;
	  static u16 run_times = 0;
	  static float accz_correction_i=0;
	
/***************Z�����ٶȺ�λ�� START***************/			

		/*��ѹ�Ʒ�������*/
		Push_baroAlt();
		/*��ѹ��΢��*/
		baroAlt_dif = (ms5611.baroAlt - baroAlt_buff[14])/(14*T);
		/*��ѹ΢�ֵ�ͨ�˲�*/		
		baroAlt_dif = LPF2pApply_2(baroAlt_dif);
		/*���ٶ�У׼������*/
		accz_correction_i += (baroAlt_dif - z_ef.velz.base)*0.1f*T;
		/*���ٶ�У׼�������޷�*/
		accz_correction_i = LIMIT(accz_correction_i,-30,30);
		/*���ٶ�У׼��*/
		z_ef.accz.correction = (baroAlt_dif - z_ef.velz.base)*0.2f + accz_correction_i;
		/*�ٶ�У׼����[-1m/s,1/s]*/
		z_ef.velz.correction = LIMIT(z_ef.velz.correction,-100,100);
		/*�õ���������ϵ��earth frame���µ���ֱ���ٶ�*/    /*��ֱ������ٶ�״̬*/
//		z_ef.accz.base = (sensor.acc.filter.x*vx + sensor.acc.filter.y*vy + sensor.acc.filter.z*vz)*980.0f/4096.0f-INTERTIALNAV_GRAVITY;
		z_ef.accz.base = 100.0f*(phone.acc[0]*vx+phone.acc[1]*vy+phone.acc[2]*vz) - INTERTIALNAV_GRAVITY;
				/*�ٶ�����*/
	  z_ef.velz.increase = (z_ef.accz.base+z_ef.accz.correction) * T;
		/*�߶ȸ��� x(k+1)=v(k)*t+0.5*a*t^2  */    /*��ֱ����߶�״̬*/
	  z_ef.posz.base += (z_ef.velz.base + z_ef.velz.correction + z_ef.velz.increase * 0.5f) * T;
		/*�ٶȸ��� v(k+1)=v(k)+a*t  */    /*��ֱ�����ٶ�״̬*/
		z_ef.velz.base += z_ef.velz.increase;
/***************Z�����ٶȺ�λ�� END***************/
}

void Push_baroAlt(void)
{
		u8 i;
	  for(i=14;i>0;i--)
			baroAlt_buff[i] = baroAlt_buff[i-1]; 
		baroAlt_buff[0] = ms5611.baroAlt;
}
