#include "MPU6050.h"
#include "main.h"
//***************************************MPU6050是地址***********************************//


//************************************加速度传感器读取*************************************//

//***********************************芯片温度读取************************************//


void MPU6050_Init(void )
{
	/*
  HAL_I2C_Mem_Write(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout);
   第1个参数为I2C操作句柄
   第2个参数为从机设备地址
   第3个参数为从机寄存器地址
   第4个参数为从机寄存器地址长度
   第5个参数为发送的数据的起始地址
   第6个参数为传输数据的大小
   第7个参数为操作超时时间 　　
*/
	uint8_t check,Data;
	// check device ID WHO_AM_I
	//检查设备ID 
	HAL_I2C_Mem_Read (&hi2c1 ,MPU6050_ADDR,WHO_AM_I_REG,1,&check ,1,1000);
	if(check == 104)		//if the device is present,寄存器地址默认0x68
	{
		//power management register 0x6B we should write all 0's to wake the sensor up
		//电源管理寄存器，我们应该在0x6B中写入所有0来唤醒传感器
		Data = 0;
		HAL_I2C_Mem_Write (&hi2c1 ,MPU6050_ADDR ,PWR_MGMT_1_REG ,1,&Data ,1,1000);
		//Set DATA RATE of 1KHz by writing SMPLRT_DIV register
		//通过写入SMPLRT_DIV寄存器设置1KHz的数据速率
		Data = 0x07;
		HAL_I2C_Mem_Write (&hi2c1 ,MPU6050_ADDR ,SMPLRT_DIV_REG ,1 ,&Data,1,1000);
		// Set accelerometer configuration in ACCEL_CONFIG Register
		// XA_ST=0,YA_ST=0,ZA_ST=0,  FS_SEL=0 ->±2g
		//在加速度寄存器中修改配置
		Data = 0x00;
		HAL_I2C_Mem_Write (&hi2c1 ,MPU6050_ADDR, ACCEL_CONFIG_REG, 1, &Data, 1, 1000);
		// Set Gyroscopic configuration in GYRO_CONFIG Register
		// XG_ST=0,YG_ST=0,  FS_SEL=0 ->± 250 °/s
		//修改重力加速计配置
		Data = 0x00;
		HAL_I2C_Mem_Write (&hi2c1 ,MPU6050_ADDR, GYRO_CONFIG_REG, 1, &Data, 1, 1000);
	}
}


