
#include "rtc.h"
#include "usart3.h"
char strbuf[50];

unsigned char HH,MM,SS;
 

int rtc(void)

{	

   
 RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);//使能电源管理时钟，后备寄存器模块时钟

    PWR_BackupAccessCmd(ENABLE);//使能RTC和后备寄存器的访问

	

    if(BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)//如果在后备数据寄存器1读取到的值不是0xA5A5 说明后备寄存器（RTC等）未初始化

   {

	RTC_Configuration();  //配置RTC，时钟选用LSE（外部低速时钟）,RTC计数器1s自增1

	RTC_WaitForLastTask();//等待RTC操作完成

	RTC_SetCounter(0);    //首次时间设置为 00:00:00

	RTC_WaitForLastTask();//等待RTC操作完成

	BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);//后备数据寄存器1写入0xA5A5，标志RTC已初始化

   } 

  else //系统复位，而后备寄存器并没有被复位时，无需再初始化RTC

  {

	if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET){

		printf("POR/PDR 复位\r\n"); //VDD掉电上电

	}else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET){

		printf("RESET引脚复位\r\n");

	}else if (RCC_GetFlagStatus(RCC_FLAG_SFTRST) != RESET){

		printf("软件复位\r\n");

	}else if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET){

		printf("独立看门狗复位\r\n");

	}else if (RCC_GetFlagStatus(RCC_FLAG_WWDGRST) != RESET){

		printf("窗口看门狗复位\r\n");

	}else if (RCC_GetFlagStatus(RCC_FLAG_LPWRRST) != RESET){

		printf("低功耗复位\r\n");

	}

	//以上复位，后备寄存器的数据仍然保持

	printf("不需要配置RTC.\r\n");

	RTC_WaitForSynchro();//等待 RTC 寄存器与RTC的APB时钟同步

  }

	

	NVIC_Configuration();//配置RTC中断优先级

	

//以下三个中断根据需要开启，三者触发时都是进入RTC_IRQHandler中断函数，通过RTC_GetITStatus判断具体是哪个中断触发

#if 1	//（可选）

  RTC_WaitForLastTask();//等待RTC操作完成，下同

  RTC_ITConfig(RTC_IT_SEC, ENABLE);//秒中断使能秒,用来产生一个可编程的周期性中断信号(最长可达1秒)。

  RTC_WaitForLastTask();

#endif

	

#if 1	//（可选）

  RTC_WaitForLastTask();

  RTC_ITConfig(RTC_IT_ALR, ENABLE);//闹钟中断使能,用来产生一个软件可编程的闹钟中断。

  RTC_WaitForLastTask();

#endif

	

#if 1	//（可选）

  RTC_WaitForLastTask();

  RTC_ITConfig(RTC_IT_OW, ENABLE);//溢出中断使能,指示内部可编程计数器溢出并回转为0的状态。

  RTC_WaitForLastTask();

#endif

 

 

#if 1 //（可选）

/*BKP_RTCOutputSource_None 	侵入检测管脚(PC.13)上无 RTC 输出

  BKP_RTCOutputSource_CalibClock     侵入检测管脚(PC.13)上输出，其时钟频率为 RTC 时钟除以 64

  BKP_RTCOutputSource_Alarm 	侵入检测管脚(PC.13)上输出 RTC 闹钟脉冲

  BKP_RTCOutputSource_Second    侵入检测管脚(PC.13)上输出 RTC 秒脉冲*/

  BKP_TamperPinCmd(DISABLE);

  BKP_RTCOutputConfig(BKP_RTCOutputSource_Second);

#endif	

	

    //清除复位标志

    RCC_ClearFlag();
   

	if(USART3_RX_STA&0X8000){

	    if(strncmp(strbuf,cmd_SetTime,strlen(cmd_SetTime))==0){

		HH = (strbuf[strlen(cmd_SetTime)+1]-'0')*10 +(strbuf[strlen(cmd_SetTime)+2]-'0');

		MM = (strbuf[strlen(cmd_SetTime)+4]-'0')*10 +(strbuf[strlen(cmd_SetTime)+5]-'0');

		SS = (strbuf[strlen(cmd_SetTime)+7]-'0')*10 +(strbuf[strlen(cmd_SetTime)+8]-'0');

		printf("设置RTC时间为 %d:%d:%d\r\n",HH,MM,SS);

		RTC_WaitForLastTask();

		RTC_SetCounter(HH*3600+MM*60+SS);//设置RTC当前计数寄存器的值

		RTC_WaitForLastTask();

		printf("设置RTC时间成功!\r\n");

	    }

			

	if(strncmp(strbuf,cmd_SetAlarm,strlen(cmd_SetAlarm))==0){

		HH = (strbuf[strlen(cmd_SetAlarm)+1]-'0')*10 +(strbuf[strlen(cmd_SetAlarm)+2]-'0');

		MM = (strbuf[strlen(cmd_SetAlarm)+4]-'0')*10 +(strbuf[strlen(cmd_SetAlarm)+5]-'0');

		SS = (strbuf[strlen(cmd_SetAlarm)+7]-'0')*10 +(strbuf[strlen(cmd_SetAlarm)+8]-'0');

		printf("设置闹钟时间为 %d:%d:%d\r\n",HH,MM,SS);

		RTC_WaitForLastTask();

		RTC_SetAlarm(HH*3600+MM*60+SS);//设置RTC闹钟值,记得使能闹钟中断

		RTC_WaitForLastTask();

		printf("设置闹钟时间成功!\r\n");

	    

        }

      }
return 0;
}

void RTC_Configuration(void)

{

  BKP_DeInit();

	

  RCC_LSEConfig(RCC_LSE_ON);

 

  while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);//等待LSE（外部低速）时钟稳定

 

  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);//选择LSE时钟作为RTC时钟，此外还可以选择：LSI、HSE_Div128

 

  RCC_RTCCLKCmd(ENABLE);//使能RTC时钟

 

  RTC_WaitForSynchro();//等待 RTC 寄存器与RTC的APB时钟同步

 

  RTC_SetPrescaler(32767); //RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1)=1s  这个决定“秒中断”触发的周期

 

  RTC_WaitForLastTask();//等待RTC操作完成

}

void NVIC_Configuration(void)//配置RTC中断优先级

{

  NVIC_InitTypeDef NVIC_InitStructure;

 

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

  NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;

  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;

  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;

  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

  NVIC_Init(&NVIC_InitStructure);

}

 


 

char *USART_GetString(char *s)//从串口阻塞等待一个字符串，遇到0x0d或者0x0a结束

{

	char code;

	char *str = s;

	if(USART_GetFlagStatus(USART3,USART_FLAG_RXNE)==RESET){

	    *s=0;

	    return NULL;

	}

	

	while(1){

	    if(USART_GetFlagStatus(USART3,USART_FLAG_RXNE)!=RESET){

		USART_ClearFlag(USART3,USART_FLAG_RXNE);

		code=USART_ReceiveData(USART3);

		if(code == 0x0D||code ==0x0A){

			*str=0;

			break;

		}else{

			*str++ = code;

		}

	    }

	}

	return s;

}

 
