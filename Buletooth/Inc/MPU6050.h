#ifndef __MPU6050_H
#define __MPU6050_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

extern I2C_HandleTypeDef hi2c1;
/* USER CODE END Includes */


/* USER CODE BEGIN Private defines */
//***************************************MPU6050�ǵ�ַ***********************************//
#define MPU6050_ADDR 0xD0
//*******************************ͨ���鿴оƬ�ֲ���ش�������ַ**************************//
#define SMPLRT_DIV_REG 0x19
#define GYRO_CONFIG_REG 0x1B
#define ACCEL_CONFIG_REG 0x1c
#define ACCEL_XOUT_H_REG 0x3B
#define TEMP_OUT_H_REG 0x41
#define GYRO_XOUT_H_REG 0x43
#define PWR_MGMT_1_REG 0x6B   //��Դ����1�Ĵ���
#define WHO_AM_I_REG 0x75   


//************************���ٶȼ�*********************//


//������ȡ��ֵ������Щ������
//************************************���ٶȴ�������ȡ*******************************
/* USER CODE END Private defines */

/* USER CODE BEGIN Prototypes */
void MPU6050_Init(void );

void MPU6050_Read_Temp(void )	;
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif

