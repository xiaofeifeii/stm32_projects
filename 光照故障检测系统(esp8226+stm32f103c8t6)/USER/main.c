#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "usart3.h"
#include "esp8266.h"
#include "string.h"
#include "timer.h"
#include "led.h"
 #include "adc.h"
 #include "rtc.h"
 #include "stm32f10x_rtc.h" 
 #include "stm32f10x_pwr.h" 
 #include "stm32f10x_bkp.h" 
 #include "stdio.h"
 #include "DHT11.h"


int led1(void);
void led(void);
void RTC_Configuration(void);
void Clock_Init();
void time();

 int main(void)
 {		
	u16 value=0;
	float vol;
 u32 TimeData=0,HH=0,MM=0,SS=0;
	char strbuf[100],a[100];
	
	u8 t,h;
	delay_init();	    	 			//延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 			//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 //串口初始化为115200
	LED_Init(); 	
	Clock_Init();
	usart3_init(115200);	 				//串口初始化为115200
   //esp8266_start_trans();	   //esp8266进行初始化 ，其实初始化一次就可以了，后面调试程序的时候这个成功了就直接可以注释，不然每次都重启WiFi，效率低
	
						
	
	/*while(esp8266_send_cmd("AT+CIPSTART=\"TCP\",\"192.168.4.2\",8080","OK",200));//这命令要等到手机端与esp8266建立连接后才会返回0，结束循环
   USART3_RX_STA=0; //清空接收缓存区
	 	memset(USART3_RX_BUF,0,sizeof(USART3_RX_BUF));
	 
	

	ADCx_Init();
 	*/
while(1)
	{
		//DHT11_Init();
 		if(tim_bz==1)
	{
		tim_bz=0;
		TimeData=RTC_GetCounter();
		HH=TimeData/3600;
		MM= (TimeData % 3600)/60;
		SS=TimeData%60;
	}
	u3_printf("time %0.2d:%0.2d:%0.2d",HH,MM,SS);
	printf("time %0.2d:%0.2d:%0.2d",HH,MM,SS);
		/*	if(USART3_RX_STA&0X8000){
      printf("USART3_RX_BUF=%s",USART3_RX_BUF);//打印，并提取有效数据
			sprintf(strbuf,"%s",USART3_RX_BUF);
			printf("strbuf=%s",strbuf);	
	    if(strstr((const char*)strbuf,"AT+SETTIME")){
    char *str1=strstr(strbuf,"AT+SETTIME");
		HH = (str1[strlen(cmd_SetTime)+1]-'0')*10 +(str1[strlen(cmd_SetTime)+2]-'0');

		MM = (str1[strlen(cmd_SetTime)+4]-'0')*10 +(str1[strlen(cmd_SetTime)+5]-'0');

		SS = (str1[strlen(cmd_SetTime)+7]-'0')*10 +(str1[strlen(cmd_SetTime)+8]-'0');
     esp8266_send_cmd("AT+CIPSEND=17","",500);
		u3_printf("set time %0.2d:%0.2d:%0.2d",HH,MM,SS);

		RTC_WaitForLastTask();

		RTC_SetCounter(HH*3600+MM*60+SS);//设置RTC当前计数寄存器的值

		RTC_WaitForLastTask();
    esp8266_send_cmd("AT+CIPSEND=17","",50);
		u3_printf("set time success!");
   
	    }
	if(strstr((const char*)strbuf,"AT+SETALARM")){
    char *str1=strstr((const char*)strbuf,"AT+SETALARM");
		HH = (str1[strlen(cmd_SetAlarm)+1]-'0')*10 +(str1[strlen(cmd_SetAlarm)+2]-'0');

		MM = (str1[strlen(cmd_SetAlarm)+4]-'0')*10 +(str1[strlen(cmd_SetAlarm)+5]-'0');

		SS = (str1[strlen(cmd_SetAlarm)+7]-'0')*10 +(str1[strlen(cmd_SetAlarm)+8]-'0');
      esp8266_send_cmd("AT+CIPSEND=23","",50);
		u3_printf("set alarm time %0.2d:%0.2d:%0.2d",HH,MM,SS);

		RTC_WaitForLastTask();

		
		
		RTC_SetAlarm(HH*3600+MM*60+SS);//设置RTC闹钟值,记得使能闹钟中断

		RTC_WaitForLastTask();
    esp8266_send_cmd("AT+CIPSEND=23","",50);
		u3_printf("set alarm time success!");
		
        }

			if(strstr((const char*)strbuf,"off")) {
				GPIO_ResetBits(GPIOB,GPIO_Pin_1);
				GPIO_ResetBits(GPIOC,GPIO_Pin_13);
			}
			else if(strstr((const char*)strbuf,"on")) {  
				GPIO_SetBits(GPIOB,GPIO_Pin_1);
				GPIO_SetBits(GPIOC,GPIO_Pin_13);
			}
		
		}
		
			USART3_RX_STA=0;
			memset(USART3_RX_BUF,0,sizeof(USART3_RX_BUF));
			value=Get_ADC_Value(ADC_Channel_1,20);
	  vol=(float)value*(3.3/4096);
			DHT11_Read_Data(&t,&h);	//读取温湿度值		
		if(vol<1.34){
		GPIO_ResetBits(GPIOB,GPIO_Pin_5);
		sprintf(a,"%0.2d:%0.2d:%0.2d tem:%0.2d hum:%0.2d LED normal",HH,MM,SS,t,h);
		esp8266_send_cmd("AT+CIPSEND=33","",50);
		esp8266_send_cmd((u8 *)a,"",50);
		USART3_RX_STA=0;
		memset(USART3_RX_BUF,0,sizeof(USART3_RX_BUF));
		}
		else{
			GPIO_SetBits(GPIOB,GPIO_Pin_5);
		sprintf(a,"%0.2d:%0.2d:%0.2d tem:%0.2d hum:%0.2d LED  error",HH,MM,SS,t,h);
		esp8266_send_cmd("AT+CIPSEND=33","",50);
		esp8266_send_cmd((u8 *)a,"",50);
		USART3_RX_STA=0;
		memset(USART3_RX_BUF,0,sizeof(USART3_RX_BUF));
		
		}*/
	/*	USART3_RX_STA=0;
		memset(USART3_RX_BUF,0,sizeof(USART3_RX_BUF));
		
			delay_ms(1500);*/
	
  }
 
}
/****************************************/
/* RTC时钟配置                                    */
/******************************************/

void RTC_Configuration(void)	
{
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP,ENABLE);
	  PWR_BackupAccessCmd(ENABLE); 
	  BKP_DeInit();
	RCC_LSEConfig(RCC_LSE_ON);
	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);
	 RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);//选择LSE时钟作为RTC时钟，此外还可以选择：LSI、HSE_Div128
  RCC_RTCCLKCmd(ENABLE);//使能RTC时钟
  RTC_WaitForSynchro();//等待 RTC 寄存器与RTC的APB时钟同步
  RTC_WaitForLastTask();//等待RTC操作完成
  RTC_ITConfig(RTC_IT_SEC, ENABLE);
	  RTC_WaitForLastTask();
	RTC_SetPrescaler(32767); //设置RTC预分频的值
  RTC_WaitForLastTask();
	
}

void Clock_Init()
{
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    /* Allow access to BKP Domain */
    PWR_BackupAccessCmd(ENABLE); 
    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
	 

  RTC_ITConfig(RTC_IT_ALR, ENABLE);//闹钟中断使能,用来产生一个软件可编程的闹钟中断。

  RTC_WaitForLastTask();
  if(BKP_ReadBackupRegister(BKP_DR1)!=0xA5A5)
 {
	 RTC_Configuration();
  RTC_WaitForLastTask();	
  RTC_SetCounter(0);
	  RTC_WaitForLastTask();
	  BKP_WriteBackupRegister(BKP_DR1, 0xA5A5); 
	
 }
 else
 {
    RTC_WaitForSynchro();//等待 RTC 寄存器与RTC的APB时钟同步
    RTC_WaitForLastTask();//等待RTC操作完成
	  RTC_ITConfig(RTC_IT_SEC, ENABLE);
	  RTC_WaitForLastTask();
	  
 }
 RCC_ClearFlag();
}





