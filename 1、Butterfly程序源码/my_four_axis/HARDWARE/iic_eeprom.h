#ifndef __IIC_EEPROM_H
#define __IIC_EEPROM_H
#include <stm32f10x.h>
/***************************************************************/
///*PB4(SCL) PB5(SDA)*/
//IO��������
#define SDA_IN()  	{GPIOB->CRL&=0XFF0FFFFF;GPIOB->CRL|=8<<20;}
#define SDA_OUT() 	{GPIOB->CRL&=0XFF0FFFFF;GPIOB->CRL|=3<<20;}

//IO��������	 
#define IIC_SCL    PBout(4) //SCL
#define IIC_SDA    PBout(5) //SDA	 
#define READ_SDA   PBin(5)  //����SDA 
/***************************************************************/

#define AT24C01		127
#define AT24C02		255
#define AT24C04		511
#define AT24C08		1023
#define AT24C16		2047
#define AT24C32		4095
#define AT24C64	  8191
#define AT24C128	16383
#define AT24C256	32767  
//������ʹ�õ���24c02�����Զ���EE_TYPEΪAT24C02
#define EE_TYPE AT24C02
					  
u8 AT24CXX_ReadOneByte(u16 ReadAddr);							//ָ����ַ��ȡһ���ֽ�
u32 AT24CXX_ReadLenByte(u16 ReadAddr,u8 Len);					//ָ����ַ��ʼ��ȡָ����������
void AT24CXX_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead);   	//��ָ����ַ��ʼ����ָ�����ȵ�����
float AT24CXX_ReadOneFloat(u16 ReadAddr);//��ָ����ַ��ȡһ��float������


void AT24CXX_WriteOneByte(u16 WriteAddr,u8 DataToWrite);		//ָ����ַд��һ���ֽ�
void AT24CXX_WriteLenByte(u16 WriteAddr,u32 DataToWrite,u8 Len);//ָ����ַ��ʼд��ָ�����ȵ�����
void AT24CXX_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite);	//��ָ����ַ��ʼд��ָ�����ȵ�����
void AT24CXX_WriteOneFloat(u16 WriteAddr,float DataToWrite);//���ָ����ַдһ����float������

//u8 AT24CXX_Check(void);  //�������
u8 AT24CXX_Init(void); //��ʼ��IIC
void FlashReadData(u16 ReadAddr,float *Data1,float *Data2,float *Data3,
														 	  float *Data4,float *Data5,float *Data6,
															  float *Data7,float *Data8,float *Data9);

u8 FlashWriteData(u16 ReadAddr,float Data1,float Data2,float Data3,
															 float Data4,float Data5,float Data6,
															 float Data7,float Data8,float Data9);

u8 FlashWriteData_calm(u16 ReadAddr,float Data1,float Data2,float Data3,
																		float Data4,float Data5,float Data6);

u8 FlashWriteData_gyro(u16 ReadAddr,float Data1,float Data2,float Data3);


#endif



