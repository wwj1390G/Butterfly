#include "calm_acc.h"
#include "my_math.h"
#include "iic_eeprom.h"
#include "calm_gyro.h"
#include "usart.h"
#include <stdbool.h>
#include <math.h>
_ACC_CAL _acc_cal = {0};

static void Calibrate_Reset_Matrices(float dS[6], float JS[6][6])
{
    int16_t j,k;
    for( j=0; j<6; j++ )
    {
        dS[j] = 0.0f;
        for( k=0; k<6; k++ )
        {
            JS[j][k] = 0.0f;
        }
    }
}

static void Calibrate_Update_Matrices(float dS[6],
                               float JS[6][6],
                               float beta[6],
                               float data[3])
{
    int16_t j, k;
    float dx, b;
    float residual = 1.0;
    float jacobian[6];
    for(j=0;j<3;j++)
    {
        b = beta[3+j];
        dx = (float)data[j] - beta[j];
        residual -= b*b*dx*dx;
        jacobian[j] = 2.0f*b*b*dx;
        jacobian[3+j] = -2.0f*b*dx*dx;
    }

    for(j=0;j<6;j++)
    {
        dS[j]+=jacobian[j]*residual;
        for(k=0;k<6;k++)
        {
            JS[j][k]+=jacobian[j]*jacobian[k];
        }
    }
}
static void Calibrate_Find_Delta(float dS[6], float JS[6][6], float delta[6])
{
    //Solve 6-d matrix equation JS*x = dS
    //first put in upper triangular form
    int16_t i,j,k;
    float mu;
    //make upper triangular
    for( i=0; i<6; i++ ) 
    {
        //eliminate all nonzero entries below JS[i][i]
        for( j=i+1; j<6; j++ ) 
        {
            mu = JS[i][j]/JS[i][i];
            if( mu != 0.0f ) 
            {
                dS[j] -= mu*dS[i];
                for( k=j; k<6; k++ ) 
                {
                    JS[k][j] -= mu*JS[k][i];
                }
            }
        }
    }
    //back-substitute
    for( i=5; i>=0; i-- ) 
    {
        dS[i] /= JS[i][i];
        JS[i][i] = 1.0f;

        for( j=0; j<i; j++ ) 
        {
            mu = JS[i][j];
            dS[j] -= mu*dS[i];
            JS[i][j] = 0.0f;
        }
    }
    for( i=0; i<6; i++ ) 
    {
        delta[i] = dS[i];
    }
}


/*���ٶȼ�У׼
 *����true ���ɹ�
 *����false��ʧ��
 */
static uint8_t Calibrate_accel( _F32xyz accel_sample[6],
															 	_F32xyz *accel_offsets,
																_F32xyz *accel_scale)
{
    int16_t i;
    int16_t num_iterations = 0;
    float eps = 0.000000001;
    float change = 100.0;
    float data[3]={0};
    float beta[6]={0};
    float delta[6]={0};
    float ds[6]={0};
    float JS[6][6]={0};
    bool success = true;
    // reset
    beta[0] = beta[1] = beta[2] = 0;
    beta[3] = beta[4] = beta[5] = 1.0f/gravity_mss;
    while( num_iterations < 20 && change > eps ) 
    {
        num_iterations++;
        Calibrate_Reset_Matrices(ds, JS);

        for( i=0; i<6; i++ ) 
        {
            data[0] = accel_sample[i].x;
            data[1] = accel_sample[i].y;
            data[2] = accel_sample[i].z;
            Calibrate_Update_Matrices(ds, JS, beta, data);
        }
        Calibrate_Find_Delta(ds, JS, delta);
        change =    delta[0]*delta[0] +
                    delta[0]*delta[0] +
                    delta[1]*delta[1] +
                    delta[2]*delta[2] +
                    delta[3]*delta[3] / (beta[3]*beta[3]) +
                    delta[4]*delta[4] / (beta[4]*beta[4]) +
                    delta[5]*delta[5] / (beta[5]*beta[5]);
        for( i=0; i<6; i++ ) 
        {
            beta[i] -= delta[i];
        }
    }
    // copy results out
    accel_scale->x = beta[3] * gravity_mss;
    accel_scale->y = beta[4] * gravity_mss;
    accel_scale->z = beta[5] * gravity_mss;
    accel_offsets->x = beta[0] * accel_scale->x;
    accel_offsets->y = beta[1] * accel_scale->y;
    accel_offsets->z = beta[2] * accel_scale->z;

    // sanity check scale
    if(fabsf(accel_scale->x-1.0f) > 0.2f|| fabsf(accel_scale->y-1.0f) > 0.2f|| fabsf(accel_scale->z-1.0f) > 0.2f )
    {
        success = false;
    }
    // sanity check offsets (3.5 is roughly 3/10th of a G, 5.0 is roughly half a G)
    if(fabsf(accel_offsets->x) > 3.5f|| fabsf(accel_offsets->y) > 3.5f|| fabsf(accel_offsets->z) > 3.5f )
    {
        success = false;
    }
    // return success or failure
    return success;
}



#define acc_cal_sum_value 50
_F32xyz acc_cal_samples[6] = {0};
//���ٶȼ�У׼ 
void acc_cal(_F32xyz *acc_in)
{
    if(_acc_cal.single!=0)
    {
        if(_acc_cal.i<acc_cal_sum_value)//�������ݲɼ�
        {
            _acc_cal.samples_sum.x += (acc_in->x * acc_to_1g);
            _acc_cal.samples_sum.y += (acc_in->y * acc_to_1g);
            _acc_cal.samples_sum.z += (acc_in->z * acc_to_1g);
            
            _acc_cal.i++;
        }
        else//�������ݲɼ����
        {
            _acc_cal.i = 0;
                                       
            _acc_cal.samples[_acc_cal.single].x = _acc_cal.samples_sum.x / acc_cal_sum_value;
            _acc_cal.samples[_acc_cal.single].y = _acc_cal.samples_sum.y / acc_cal_sum_value;
            _acc_cal.samples[_acc_cal.single].z = _acc_cal.samples_sum.z / acc_cal_sum_value; 
					
						printf("_acc_cal.single = %d    %0.2f    %0.2f    %0.2f\r\n",_acc_cal.single,_acc_cal.samples[_acc_cal.single].x,_acc_cal.samples[_acc_cal.single].y,_acc_cal.samples[_acc_cal.single].z);
            
            //�������ۼ����ݽ�������������´δ������
            set_value(&_acc_cal.samples_sum,0.0f);   

            _acc_cal.single_finish_flag[_acc_cal.single] = 1;//��Ӧ��ɼ���ɣ���־λ
            
            _acc_cal.single = 0;
        }
        //��������ȫ���ɼ����
        if(  _acc_cal.single_finish_flag[1]
            &_acc_cal.single_finish_flag[2]
            &_acc_cal.single_finish_flag[3]
            &_acc_cal.single_finish_flag[4]
            &_acc_cal.single_finish_flag[5]
            &_acc_cal.single_finish_flag[6])
        {
            _acc_cal.all_finish_flag = 1;//���������ݲɼ���ɱ�־λ������ָʾ����ʾ
       
            //���ٶȼ�У׼����ת��
            for(uint8_t i=0;i<6;i++)
            {
                _set_val(&acc_cal_samples[i],&_acc_cal.samples[i+1]);//_acc_cal.samples  ==> acc_cal_samples
            }
            //���ٶȼ�У׼
            _acc_cal.return_flag = Calibrate_accel(acc_cal_samples,&_acc_cal.offset_flash_write,&_acc_cal.scale_flash_write);
            
            if(_acc_cal.return_flag==true)
            {
                //У׼�ɹ���д��flash      
                FlashWriteData_calm(0, _acc_cal.offset_flash_write.x,
											 _acc_cal.offset_flash_write.y,
											 _acc_cal.offset_flash_write.z,

											 _acc_cal.scale_flash_write.x,
											 _acc_cal.scale_flash_write.y,
											 _acc_cal.scale_flash_write.z);  

							
            }                        
            for(uint8_t i=1;i<7;i++)
            {
                _acc_cal.single_finish_flag[i] = 0;//�����Ӧ���־λ
            }
            
            _acc_cal.flash_finish_flag = 1;//���ٶȼ�У׼���
        }
    }
}



