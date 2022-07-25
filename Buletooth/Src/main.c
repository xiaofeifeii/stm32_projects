/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "MPU6050.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

#define PI 3.141592
double AVM, GVM, TAy;

//***********************锟斤拷锟斤拷锟斤拷***********************//
int16_t Accel_X_RAW = 0;
int16_t Accel_Y_RAW = 0;
int16_t Accel_Z_RAW = 0;

int16_t Gyro_X_RAW = 0;
int16_t Gyro_Y_RAW = 0;
int16_t Gyro_Z_RAW = 0;
int16_t Temp_RAW = 0;
float Ax, Ay, Az, Gx, Gy, Gz, Temp;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void MPU6050_Read_Accel(void);
void MPU6050_Read_Gyro(void);
void MPU6050_Read_Temp(void);
int check();
void errorLog(int num);
void parseGpsBuffer(void);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
// extern float Ax,Ay,Az,Gx,Gy,Gz,Temp;
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_USART2_UART_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

  MPU6050_Init();
 parseGpsBuffer();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
//usart2_printf("hello");
     
		HAL_Delay(2000);
    if (check())
    {
      usart2_printf("help\r\n");
      usart2_printf(":AVM:%.2f\r\n", AVM);
      usart2_printf(":TAy:%.2f\r\n", TAy);
       usart2_printf("经度:%s\n",Save_Data.longitude);
       usart2_printf("纬度:%s\n"Save_Data.latitude);
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
    }
    else
    {
      usart2_printf("norm\r\n");
      usart2_printf(":AVM:%.2f\r\n", AVM);
      usart2_printf(":TAy:%.2f\r\n", TAy);
       usart2_printf("经度:%s\n",Save_Data.longitude);
       usart2_printf("纬度:%s\n"Save_Data.latitude);
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
    }
    /* USER CODE BEGIN 3 */
  }
}
/* USER CODE END 3 */

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void MPU6050_Read_Accel(void)
{
  uint8_t Rec_Data[6];
  // Read 6 BYTES of data starting from ACCEL_XOUT_H register
  HAL_I2C_Mem_Read(&hi2c1, MPU6050_ADDR, ACCEL_XOUT_H_REG, 1, Rec_Data, 6, 1000);
  Accel_X_RAW = (int16_t)(Rec_Data[0] << 8 | Rec_Data[1]);
  Accel_Y_RAW = (int16_t)(Rec_Data[2] << 8 | Rec_Data[3]);
  Accel_Z_RAW = (int16_t)(Rec_Data[4] << 8 | Rec_Data[5]);
  Ax = Accel_X_RAW / 16384.0;
  Ay = Accel_Y_RAW / 16384.0;
  Az = Accel_Z_RAW / 16384.0;
}
//*********************************锟斤拷锟斤拷锟斤拷锟斤拷锟捷讹拷取*****************************************//
void MPU6050_Read_Gyro(void)
{
  uint8_t Rec_Data[6];
  // Read 6 BYTES of data staring from GYRO_XOUT_H register
  HAL_I2C_Mem_Read(&hi2c1, MPU6050_ADDR, GYRO_XOUT_H_REG, 1, Rec_Data, 6, 1000);
  Gyro_X_RAW = (int16_t)(Rec_Data[0] << 8 | Rec_Data[1]);
  Gyro_Y_RAW = (int16_t)(Rec_Data[2] << 8 | Rec_Data[3]);
  Gyro_Z_RAW = (int16_t)(Rec_Data[4] << 8 | Rec_Data[5]);
  Gx = Gyro_X_RAW / 131.0;
  Gy = Gyro_Y_RAW / 131.0;
  Gz = Gyro_Z_RAW / 131.0;
}
void MPU6050_Read_Temp(void)
{
  uint8_t Rec_Data[2];
  HAL_I2C_Mem_Read(&hi2c1, MPU6050_ADDR, TEMP_OUT_H_REG, 1, Rec_Data, 2, 1000);
  Temp_RAW = (int16_t)(Rec_Data[0] << 8) | Rec_Data[1];
  Temp = 36.53 + (Temp_RAW) / 340;
}

void parseGpsBuffer()
{
  char *subString;
  char *subStringNext;
  char i = 0;
  char temp1[12]="117.20809275";
  char temp2[11]="39.0911026";
  // memcpy(Save_Data.UTCTime, subString, subStringNext - subString);
   memcpy(Save_Data.longitude, temp1, 12);
   memcpy(Save_Data.latitude, temp2, 11);
  if (Save_Data.isGetData)
  {
    Save_Data.isGetData = false;
    //printf("**************\r\n");
    //printf(Save_Data.GPS_Buffer);

    for (i = 0; i <= 6; i++)
    {
      if (i == 0)
      {
        if ((subString = strstr(Save_Data.GPS_Buffer, ",")) == NULL)
          errorLog(1); //解析错误
      }
      else
      {
        subString++;
        if ((subStringNext = strstr(subString, ",")) != NULL)
        {
          char usefullBuffer[2];
          switch (i)
          {
          case 1:
            memcpy(Save_Data.UTCTime, subString, subStringNext - subString);
            break; //获取UTC时间
          case 2:
            memcpy(usefullBuffer, subString, subStringNext - subString);
            break; //获取UTC时间
          case 3:
            memcpy(Save_Data.latitude, subString, subStringNext - subString);
            break; //获取纬度信息
          case 4:
            memcpy(Save_Data.N_S, subString, subStringNext - subString);
            break; //获取N/S
          case 5:
            memcpy(Save_Data.longitude, subString, subStringNext - subString);
            break; //获取经度信息
          case 6:
            memcpy(Save_Data.E_W, subString, subStringNext - subString);
            break; //获取E/W

          default:
            break;
          }

          subString = subStringNext;
          Save_Data.isParseData = true;
          if (usefullBuffer[0] == 'A')
            Save_Data.isUsefull = true;
          else if (usefullBuffer[0] == 'V')
            Save_Data.isUsefull = false;
        }
        else
        {
          errorLog(2); //解析错误
        }
      }
    }
  }
}

void errorLog(int num)
{

  while (1)
  {
    printf("ERROR%d\r\n", num);
  }
}

int check()
{
  double a = sqrt(-2);
  MPU6050_Read_Accel();
  MPU6050_Read_Gyro();

  AVM = sqrt(Ax * Ax + Ay * Ay + Az * Az); //加速度处理
  GVM = sqrt(Gx * Gx + Gy * Gy);
  TAy = acos(Az / GVM) * (180 / PI); //角度处理
  while (1)
  {
    if (TAy == a)
    {
      MPU6050_Read_Gyro();
      TAy = acos(Az / Gz) * (180 / PI);
    }
    else
    {
      break;
    }
  }
  if ((AVM >1.1 && TAy<80)||(AVM >1.1 && TAy>90)||(AVM <0.9 && TAy<80)||(AVM <0.6 && TAy>80))
  {
    return 1;
  }
  else
    return 0;
}


/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
