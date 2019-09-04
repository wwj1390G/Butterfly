#include "pair_freq.h"
#include "NRF24L01.h"
#include "NRFCommunication.h"
#include "led.h"
_PAIR pair = {1,1,{0x34,0x43,0x10,0x10,0x01},{4,14,24,34}};


//��ȡоƬID
static unsigned int get_chip_id(void)
{
		unsigned int  chip_id[3] = {0};  
    chip_id[0] = *(__IO u32 *)(0X1FFFF7F0); //���ֽ�
    chip_id[1] = *(__IO u32 *)(0X1FFFF7EC); //
    chip_id[2] = *(__IO u32 *)(0X1FFFF7E8); //���ֽ�
    
    return  (chip_id[0] | chip_id[1] | chip_id[2]); 
}


//��Ƶ
void wait_pairing(void)
{
		unsigned int ID = 0;
		ID = get_chip_id();
    while(1)
    {    
        //�������� ���ж�Ƶ
        if(_RC.thr>900)
        {
            pair.com_mode = 1;
            
            //װ���ַ
            pair.addr[0] = *((uint8_t *)&ID);		 
            pair.addr[1] = *(((uint8_t *)&ID)+1);	     
            pair.addr[2] = *(((uint8_t *)&ID)+2);		
            pair.addr[3] = *(((uint8_t *)&ID)+3);		
            pair.addr[4] = pair.addr[0];		
            
            //װ��Ƶ��
            pair.freq_channel[0] = pair.addr[0]%100;
            pair.freq_channel[1] = pair.addr[1]%100;
            pair.freq_channel[2] = pair.addr[2]%100;
            pair.freq_channel[3] = pair.addr[3]%100;            
        }
        else if(_RC.thr<100)
        {
            if(pair.com_mode==1)
            {
							pair.com_mode = 2;

							NRF24L01_CE=0;	

							//д���µ�ͨ�ŵ�ַ
							NRF_set_Addr(pair.addr,pair.addr);
							break;
            }
        }            
    }
}



