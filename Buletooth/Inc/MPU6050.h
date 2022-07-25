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
//***************************************MPU6050是地址***********************************//
#define MPU6050_ADDR 0xD0
//*******************************通过查看芯片手册相关传感器地址**************************//
#define SMPLRT_DIV_REG 0x19
#define GYRO_CONFIG_REG 0x1B
#define ACCEL_CONFIG_REG 0x1c
#define ACCEL_XOUT_H_REG 0x3B
#define TEMP_OUT_H_REG 0x41
#define GYRO_XOUT_H_REG 0x43
#define PWR_MGMT_1_REG 0x6B   //电源管理1寄存器
#define WHO_AM_I_REG 0x75   


//************************加速度计*********************//


//将最后读取的值放入这些变量中
//************************************加速度传感器读取*******************************
/* USER CODE END Private defines */

/* USER CODE BEGIN Prototypes */
void MPU6050_Init(void );

void MPU6050_Read_Temp(void )	;
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif

