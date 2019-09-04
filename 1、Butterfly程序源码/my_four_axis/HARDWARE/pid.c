#include "pid.h"


_ALL_PID all;

//pid���Ʋ���
const float  controller_parameter[15][5] =
{
    //0.kp   1.ki   2.kd    3.�����޷�    4.pid����޷�
 

//��50000ת/���� ���	
    //��̬�⻷ 
    {1.2,  0,   0.01,  300 , 800 },         //pit_angle
    {1.2,  0,   0.01,  300 , 800 },         //rol_angle
    {0.001,  0,   0,  300 , 800 },          //yaw_angle

    
    //��̬�ڻ�
    {0.710,  0,   0.018,  300 , 800 }, //pit_gyro
    {0.710,  0,   0.018,  300 , 800 }, //rol_gyro
    {4.2,  0,   0.01,  300 , 800 }, //yaw_gyro

	
	
//��40000ת/���� ���
//		//��̬�⻷ 
//    {2.8,  0,   0.005,  300 , 800 },         //pit_angle
//    {2.8,  0,   0.005,  300 , 800 },         //rol_angle
//    {0.001,  0,   0,  300 , 800 },          //yaw_angle
//    //��̬�ڻ�
//    {0.775,  0.015,   0.009,  300 , 800 }, //pit_gyro
//    {0.775,  0.015,   0.009,  300 , 800 }, //rol_gyro
//    {4.2,  0.11,   0.008,  300 , 800 }, //yaw_gyro		
		
		
		
    //����
    {0  ,0  ,0  ,200,800},                //acc_high
    {3.1  ,0  ,0  ,200,800},              //vel_high
    {1.05  ,0  ,0  ,200,800},             //pos_high
    
    //����x
    {0  ,0  ,0  ,200,800},                //acc_x       
    {0.08  ,0.0  ,0.0  ,200,800},         //vel_x
    {2.2  ,0  ,0  ,200,800},              //pos_x

    //����y
    {0  ,0  ,0  ,200,800},                //acc_y       
    {0.08  ,0.0  ,0.0  ,200,800},         //vel_y
    {2.2  ,0  ,0  ,200,800},              //pos_y    
};

//pid��������
static void pid_init(_PID *controller,uint8_t label)
{
    controller->kp                       = controller_parameter[label][0];
    controller->ki                       = controller_parameter[label][1];
    controller->kd                       = controller_parameter[label][2];
    controller->iLimit             				= controller_parameter[label][3];
    controller->Out_max                  = controller_parameter[label][4];      
}


//pid��ʼ��
void all_pid_init(void)
{
		/*��̬�⻷ */
    pid_init(&all.pit_angle,0);
    pid_init(&all.rol_angle,1);
    pid_init(&all.yaw_angle,2);
    /*��̬�ڻ�*/
    pid_init(&all.pit_gyro,3);
    pid_init(&all.rol_gyro,4);
    pid_init(&all.yaw_gyro,5);
/*
		//����
    pid_init(&all.acc_high,6);
    pid_init(&all.vel_high,7);
    pid_init(&all.pos_high,8);
    
		//����x
    pid_init(&all.acc_fix_x,9);
    pid_init(&all.vel_fix_x,10);
    pid_init(&all.pos_fix_x,11);    
    //����y
    pid_init(&all.acc_fix_y,12);
    pid_init(&all.vel_fix_y,13);
    pid_init(&all.pos_fix_y,14); 
*/
} 

/*PID���ƺ���
 *expect			measure			dt
 *����ֵ      ����ֵ      ��λʱ��
 */
/************************************
*	��PID����⣺
*	P�������������⣬����������ƫ������ֱ�ӿ��ƣ�ƫ��Խ�󣬸�����Խǿ�ң�ʹϵͳ���ٿ�������ֵ��
*	   ���ǣ�����P���Ƶ�ϵͳ����ֳ��������µ�Ƶ�𵴡�
*	
*	I�����֣�����ۻ��������ǳ�Сʱ������P�޷���ϵͳ������Ч���ƣ���Ҫ��΢С�������ۼӣ������Ŵ�
*	   Ȼ���ٽ��п��ƣ���Ϊ�Ƕ������зŴ����Ե�����ͺܴ�ʱ�����ֵĿ�������޴󣬿϶��ǲ����Եġ�
*	   ���Ի���Ҫ�������������Ի��ֽ����޷���������Iֻ�ǶԱ���P�ĸ������ڣ�Ҫ�󲻸ߵ�ϵͳ����Բ��û��֣�
*	
*	D��΢�֣���λʱ��ı仯����Ҳ���Ǳ仯�ʣ��仯��Խ�������Խ������ϵͳ�ı仯��ǿ��ϵͳ���ֵ�ǰ״̬��
* 	  �������Ʊ���P�ĳ���ȱ�ݣ�����������������󣬻ᵼ��ϵͳ��Ƶ�𵴡�
**************************************/
void pid_controller(_PID *pid,float dt)
{
	//���=����-ʵ��
	pid->Error  = pid->expect - pid->measure;
	
	//΢��=(�������-�ϴ����)/����ʱ��
	pid->Deriv  = (pid->Error - pid->PreError) / dt;
	
	//����=���*����ʱ��
	pid->Integ += pid->Error * dt;
	
	//�����޷�
	if(pid->Integ > pid->iLimit) pid->Integ = pid->iLimit ;
	else if(pid->Integ < -pid->iLimit) pid->Integ = -pid->iLimit;
	
	pid->OutP = pid->kp * pid->Error;//�������������
	pid->OutI = pid->ki * pid->Integ;//���ֿ��������
	pid->OutD = pid->kd * pid->Deriv;//΢�ֿ��������
	
	pid->Output = pid->OutP + pid->OutI + pid->OutD;//PID���������
	//����޷�
	if(pid->Output >  pid->Out_max)   pid->Output =  pid->Out_max;
	if(pid->Output < -pid->Out_max)   pid->Output = -pid->Out_max;
	
	pid->PreError = pid->Error;
}


//�������
void clear_integral(_PID *controller)
{
    controller->Integ = 0.0f;
}



