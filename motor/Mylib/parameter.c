#include "parameter.h"

struct _parameter parameter;

void Get_parameter(void)
{
//��̬�⻷����
		parameter.outer.pit.kp = 0.8;//0.5
		parameter.outer.pit.ki = 0.11;
		parameter.outer.pit.kd = 2.0;//2.0

		parameter.outer.rol.kp = 0.8;//0.5
		parameter.outer.rol.ki = 0.11;
		parameter.outer.rol.kd = 2.0;//2.0

	  parameter.outer.yaw.kp = 1.0;
		parameter.outer.yaw.ki = 0.0;
		parameter.outer.yaw.kd = 0.0;
	
//��̬�ڻ�����	
	  parameter.inner.pit.kp = 0.40;
	  parameter.inner.pit.ki = 0.0;
		parameter.inner.pit.kd = 0.02;

		parameter.inner.rol.kp = 0.40;
		parameter.inner.rol.ki = 0.0;
		parameter.inner.rol.kd = 0.02;

	  parameter.inner.yaw.kp = 0.6;
		parameter.inner.yaw.ki = 0.0;
		parameter.inner.yaw.kd = 0.035;
		
		ctrl.inner.FB = 0.06;

//��ѹ���߲���
/*���ٶȿ��ƣ�P���ƣ���������*/
		hgt_ctrl.acc.kp = 0.35;

/*�ٶȿ��ƣ�P����*/
		hgt_ctrl.vel.kp = 2.5;

/*λ�ÿ��ƣ�PI����*/
		hgt_ctrl.pos.kp = 1.0;
		hgt_ctrl.pos.ki = 0.10;//0.10
		hgt_ctrl.pos.inc_max = 10.0;//����ʱ��Ҫ�仯
}


u8 Param_Save(void)
{
	u8 act_page_num = 255;	//����ʹ�õ�ҳ��
	uint32_t act_page_start = 0;	//��ҳ��ʼ��ַ
	uint16_t data_cnt = 0;
	
	act_page_num = GetActPage();
	if(act_page_num>(PAGE_NUM-2))
	{
		if(ClearDataFlash())
			act_page_num = 0;
		else
			return False;	//����ʧ��
	}
	
	act_page_start = FLASH_USER_START_ADDR + (act_page_num * PAGE_BYTES);
	FLASH_Unlock();
	
//	if(!WriteInt(act_page_start, data_cnt++, mag.max.x)) return False;	//д��ʧ��
//	if(!WriteInt(act_page_start, data_cnt++, mag.max.y)) return False;	//д��ʧ��
//	if(!WriteInt(act_page_start, data_cnt++, mag.max.z)) return False;	//д��ʧ��
//	if(!WriteInt(act_page_start, data_cnt++, mag.min.x)) return False;	//д��ʧ��
//	if(!WriteInt(act_page_start, data_cnt++, mag.min.y)) return False;	//д��ʧ��
//	if(!WriteInt(act_page_start, data_cnt++, mag.min.z)) return False;	//д��ʧ��

	FLASH_Lock(); 

	return True;
}

u8 Param_Read(void)
{
	u8 act_page_num = 255;	//����ʹ�õ�ҳ��
	uint32_t act_page_start = 0;	//��ҳ��ʼ��ַ
	uint16_t data_cnt = 0;
	
	act_page_num = GetActPage();
	if(act_page_num==0)
		return False;
	else
		act_page_num -= 1;
	act_page_start = FLASH_USER_START_ADDR + (act_page_num * PAGE_BYTES);
	
//	mag.max.x = ReadInt(act_page_start, data_cnt++);
//	mag.max.y = ReadInt(act_page_start, data_cnt++);
//	mag.max.z = ReadInt(act_page_start, data_cnt++);
//	mag.min.x = ReadInt(act_page_start, data_cnt++);
//	mag.min.y = ReadInt(act_page_start, data_cnt++);
//	mag.min.z = ReadInt(act_page_start, data_cnt++);

	return True;
}
