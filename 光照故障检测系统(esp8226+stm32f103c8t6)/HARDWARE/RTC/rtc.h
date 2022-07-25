#ifndef __rtc_H
#define __rtc_H


#include "stm32f10x.h"

#include "stdio.h"

#include "string.h"


static void RTC_Configuration(void);

static void NVIC_Configuration(void);

void USART1_Config(void);

static char *USART_GetString(char *s);

static char cmd_SetTime[]="AT+SETTIME";//设置时间的指令

static char cmd_SetAlarm[]="AT+SETALARM";//设置闹钟的指令
int rtc(void);

#endif


