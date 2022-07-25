#include "MPU6050.h"
#include "main.h"
//***************************************MPU6050�ǵ�ַ***********************************//


//************************************���ٶȴ�������ȡ*************************************//

//***********************************оƬ�¶ȶ�ȡ************************************//


void MPU6050_Init(void )
{
	/*
  HAL_I2C_Mem_Write(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout);
   ��1������ΪI2C�������
   ��2������Ϊ�ӻ��豸��ַ
   ��3������Ϊ�ӻ��Ĵ�����ַ
   ��4������Ϊ�ӻ��Ĵ�����ַ����
   ��5������Ϊ���͵����ݵ���ʼ��ַ
   ��6������Ϊ�������ݵĴ�С
   ��7������Ϊ������ʱʱ�� ����
*/
	uint8_t check,Data;
	// check device ID WHO_AM_I
	//����豸ID 
	HAL_I2C_Mem_Read (&hi2c1 ,MPU6050_ADDR,WHO_AM_I_REG,1,&check ,1,1000);
	if(check == 104)		//if the device is present,�Ĵ�����ַĬ��0x68
	{
		//power management register 0x6B we should write all 0's to wake the sensor up
		//��Դ����Ĵ���������Ӧ����0x6B��д������0�����Ѵ�����
		Data = 0;
		HAL_I2C_Mem_Write (&hi2c1 ,MPU6050_ADDR ,PWR_MGMT_1_REG ,1,&Data ,1,1000);
		//Set DATA RATE of 1KHz by writing SMPLRT_DIV register
		//ͨ��д��SMPLRT_DIV�Ĵ�������1KHz����������
		Data = 0x07;
		HAL_I2C_Mem_Write (&hi2c1 ,MPU6050_ADDR ,SMPLRT_DIV_REG ,1 ,&Data,1,1000);
		// Set accelerometer configuration in ACCEL_CONFIG Register
		// XA_ST=0,YA_ST=0,ZA_ST=0,  FS_SEL=0 ->��2g
		//�ڼ��ٶȼĴ������޸�����
		Data = 0x00;
		HAL_I2C_Mem_Write (&hi2c1 ,MPU6050_ADDR, ACCEL_CONFIG_REG, 1, &Data, 1, 1000);
		// Set Gyroscopic configuration in GYRO_CONFIG Register
		// XG_ST=0,YG_ST=0,  FS_SEL=0 ->�� 250 ��/s
		//�޸��������ټ�����
		Data = 0x00;
		HAL_I2C_Mem_Write (&hi2c1 ,MPU6050_ADDR, GYRO_CONFIG_REG, 1, &Data, 1, 1000);
	}
}


