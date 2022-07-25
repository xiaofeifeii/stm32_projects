#include "esp8266.h"
#include "string.h"
#include "usart.h"
#include "usart3.h"
#include "stm32f10x.h"
#include "sys.h" 
#include "delay.h"
#include "led.h"

void esp8266_start_trans(void)
{
	  
	//设置工作模式 1：station模式   2：AP模式  3：兼容 AP+station模式
		esp8266_send_cmd("AT","OK",200);
	esp8266_send_cmd("AT+CWMODE=3","OK",200);
	//Wifi模块重启
	esp8266_send_cmd("AT+RST","OK",200);
	delay_ms(1000);         //延时3S等待重启成功
	delay_ms(1000);
	
    //AP模式
	esp8266_send_cmd("AT+CWSAP=\"TEST123\",\"12345678\",11,3","OK",200);
	esp8266_send_cmd("AT+CIPMODE=1","OK",20);
	esp8266_send_cmd("AT+CIPMUX=1","OK",20);

}
//向ESP8266发送命令
//cmd:发送的命令字符串;ack:期待的应答结果,如果为空,则表示不需要等待应答;waittime:等待时间(单位:10ms)
//返回值:0,发送成功(得到了期待的应答结果);1,发送失败
u8 esp8266_send_cmd(u8 *cmd,u8 *ack,u16 waittime)
{
	u8 res=0; 
	USART3_RX_STA=0;
	u3_printf("%s\r\n",cmd);	//发送命令
	if(ack&&waittime)		//需要等待应答
	{
		while(--waittime)	//等待倒计时
		{
			delay_ms(10);
			if(USART3_RX_STA&0X8000)//接收到期待的应答结果
			{
				if(esp8266_check_cmd(ack))
				{
					printf("ack:%s\r\n",(u8*)ack);
					break;//得到有效数据 
				}
					USART3_RX_STA=0;
			} 
		}
		if(waittime==0)res=1; 
	}
	return res;
} 
//ESP8266发送命令后,检测接收到的应答
//str:期待的应答结果
//返回值:0,没有得到期待的应答结果;其他,期待应答结果的位置(str的位置)
u8* esp8266_check_cmd(u8 *str)
{
	char *strx=0;
	if(USART3_RX_STA&0X8000)		//接收到一次数据了
	{ 
		USART3_RX_BUF[USART3_RX_STA&0X7FFF]=0;//添加结束符
		strx=strstr((const char*)USART3_RX_BUF,(const char*)str);
	} 
	return (u8*)strx;
}

