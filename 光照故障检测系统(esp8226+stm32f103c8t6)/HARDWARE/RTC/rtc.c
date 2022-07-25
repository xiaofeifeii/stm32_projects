
#include "rtc.h"
#include "usart3.h"
char strbuf[50];

unsigned char HH,MM,SS;
 

int rtc(void)

{	

   
 RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);//ʹ�ܵ�Դ����ʱ�ӣ��󱸼Ĵ���ģ��ʱ��

    PWR_BackupAccessCmd(ENABLE);//ʹ��RTC�ͺ󱸼Ĵ����ķ���

	

    if(BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)//����ں����ݼĴ���1��ȡ����ֵ����0xA5A5 ˵���󱸼Ĵ�����RTC�ȣ�δ��ʼ��

   {

	RTC_Configuration();  //����RTC��ʱ��ѡ��LSE���ⲿ����ʱ�ӣ�,RTC������1s����1

	RTC_WaitForLastTask();//�ȴ�RTC�������

	RTC_SetCounter(0);    //�״�ʱ������Ϊ 00:00:00

	RTC_WaitForLastTask();//�ȴ�RTC�������

	BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);//�����ݼĴ���1д��0xA5A5����־RTC�ѳ�ʼ��

   } 

  else //ϵͳ��λ�����󱸼Ĵ�����û�б���λʱ�������ٳ�ʼ��RTC

  {

	if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET){

		printf("POR/PDR ��λ\r\n"); //VDD�����ϵ�

	}else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET){

		printf("RESET���Ÿ�λ\r\n");

	}else if (RCC_GetFlagStatus(RCC_FLAG_SFTRST) != RESET){

		printf("�����λ\r\n");

	}else if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET){

		printf("�������Ź���λ\r\n");

	}else if (RCC_GetFlagStatus(RCC_FLAG_WWDGRST) != RESET){

		printf("���ڿ��Ź���λ\r\n");

	}else if (RCC_GetFlagStatus(RCC_FLAG_LPWRRST) != RESET){

		printf("�͹��ĸ�λ\r\n");

	}

	//���ϸ�λ���󱸼Ĵ�����������Ȼ����

	printf("����Ҫ����RTC.\r\n");

	RTC_WaitForSynchro();//�ȴ� RTC �Ĵ�����RTC��APBʱ��ͬ��

  }

	

	NVIC_Configuration();//����RTC�ж����ȼ�

	

//���������жϸ�����Ҫ���������ߴ���ʱ���ǽ���RTC_IRQHandler�жϺ�����ͨ��RTC_GetITStatus�жϾ������ĸ��жϴ���

#if 1	//����ѡ��

  RTC_WaitForLastTask();//�ȴ�RTC������ɣ���ͬ

  RTC_ITConfig(RTC_IT_SEC, ENABLE);//���ж�ʹ����,��������һ���ɱ�̵��������ж��ź�(��ɴ�1��)��

  RTC_WaitForLastTask();

#endif

	

#if 1	//����ѡ��

  RTC_WaitForLastTask();

  RTC_ITConfig(RTC_IT_ALR, ENABLE);//�����ж�ʹ��,��������һ������ɱ�̵������жϡ�

  RTC_WaitForLastTask();

#endif

	

#if 1	//����ѡ��

  RTC_WaitForLastTask();

  RTC_ITConfig(RTC_IT_OW, ENABLE);//����ж�ʹ��,ָʾ�ڲ��ɱ�̼������������תΪ0��״̬��

  RTC_WaitForLastTask();

#endif

 

 

#if 1 //����ѡ��

/*BKP_RTCOutputSource_None 	������ܽ�(PC.13)���� RTC ���

  BKP_RTCOutputSource_CalibClock     ������ܽ�(PC.13)���������ʱ��Ƶ��Ϊ RTC ʱ�ӳ��� 64

  BKP_RTCOutputSource_Alarm 	������ܽ�(PC.13)����� RTC ��������

  BKP_RTCOutputSource_Second    ������ܽ�(PC.13)����� RTC ������*/

  BKP_TamperPinCmd(DISABLE);

  BKP_RTCOutputConfig(BKP_RTCOutputSource_Second);

#endif	

	

    //�����λ��־

    RCC_ClearFlag();
   

	if(USART3_RX_STA&0X8000){

	    if(strncmp(strbuf,cmd_SetTime,strlen(cmd_SetTime))==0){

		HH = (strbuf[strlen(cmd_SetTime)+1]-'0')*10 +(strbuf[strlen(cmd_SetTime)+2]-'0');

		MM = (strbuf[strlen(cmd_SetTime)+4]-'0')*10 +(strbuf[strlen(cmd_SetTime)+5]-'0');

		SS = (strbuf[strlen(cmd_SetTime)+7]-'0')*10 +(strbuf[strlen(cmd_SetTime)+8]-'0');

		printf("����RTCʱ��Ϊ %d:%d:%d\r\n",HH,MM,SS);

		RTC_WaitForLastTask();

		RTC_SetCounter(HH*3600+MM*60+SS);//����RTC��ǰ�����Ĵ�����ֵ

		RTC_WaitForLastTask();

		printf("����RTCʱ��ɹ�!\r\n");

	    }

			

	if(strncmp(strbuf,cmd_SetAlarm,strlen(cmd_SetAlarm))==0){

		HH = (strbuf[strlen(cmd_SetAlarm)+1]-'0')*10 +(strbuf[strlen(cmd_SetAlarm)+2]-'0');

		MM = (strbuf[strlen(cmd_SetAlarm)+4]-'0')*10 +(strbuf[strlen(cmd_SetAlarm)+5]-'0');

		SS = (strbuf[strlen(cmd_SetAlarm)+7]-'0')*10 +(strbuf[strlen(cmd_SetAlarm)+8]-'0');

		printf("��������ʱ��Ϊ %d:%d:%d\r\n",HH,MM,SS);

		RTC_WaitForLastTask();

		RTC_SetAlarm(HH*3600+MM*60+SS);//����RTC����ֵ,�ǵ�ʹ�������ж�

		RTC_WaitForLastTask();

		printf("��������ʱ��ɹ�!\r\n");

	    

        }

      }
return 0;
}

void RTC_Configuration(void)

{

  BKP_DeInit();

	

  RCC_LSEConfig(RCC_LSE_ON);

 

  while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);//�ȴ�LSE���ⲿ���٣�ʱ���ȶ�

 

  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);//ѡ��LSEʱ����ΪRTCʱ�ӣ����⻹����ѡ��LSI��HSE_Div128

 

  RCC_RTCCLKCmd(ENABLE);//ʹ��RTCʱ��

 

  RTC_WaitForSynchro();//�ȴ� RTC �Ĵ�����RTC��APBʱ��ͬ��

 

  RTC_SetPrescaler(32767); //RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1)=1s  ������������жϡ�����������

 

  RTC_WaitForLastTask();//�ȴ�RTC�������

}

void NVIC_Configuration(void)//����RTC�ж����ȼ�

{

  NVIC_InitTypeDef NVIC_InitStructure;

 

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

  NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;

  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;

  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;

  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

  NVIC_Init(&NVIC_InitStructure);

}

 


 

char *USART_GetString(char *s)//�Ӵ��������ȴ�һ���ַ���������0x0d����0x0a����

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

 
