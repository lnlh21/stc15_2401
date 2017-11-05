#include "public_def.h"
#include "stc15.h"
#include "unit.h"
#include "nrf24l01.h"
#include <intrins.h>

bit busy = 0;

#if DESC("ʱ��")
/*****************************************************************************
 �� �� ��  : DEV_DelayNms
 ��������  : ��ʱ
 �������  : unsigned short n  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��7��3��
    ��    ��   : linhao
    �޸�����   : �����ɺ���

*****************************************************************************/
void DEV_DelayNms(unsigned short n)
{
	unsigned char i, j;
	unsigned short k;

	for (k=0; k<n; k++)
	{
		i = 11;
		j = 190;
		do
		{
			while (--j);
		} while (--i);
	}
}




void DEV_Timer0Start(void)		//10����@11.0592MHz
{
	AUXR &= 0x7F;		//��ʱ��ʱ��12Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TMOD |= 0x01;		//���ö�ʱ��ģʽ
	TL0 = 0x00;		//���ö�ʱ��ֵ
	TH0 = 0xDC;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
}



#endif

#if DESC("LED")


/*****************************************************************************
 �� �� ��  : DEV_Flash
 ��������  : ����
 �������  : UCHAR n  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��7��3��
    ��    ��   : linhao
    �޸�����   : �����ɺ���

*****************************************************************************/
void DEV_Flash(UCHAR n)
{
	UCHAR i;

    for (i = 0; i < n; i++)
	{
		DEV_LED1_ON();
		DEV_DelayNms(10);
		DEV_LED1_OFF();
		DEV_DelayNms(100);
	}
}

void DEV_FlashOK()
{
	UCHAR i;

	DEV_LED1_ON();
	DEV_DelayNms(400);
	DEV_LED1_OFF();
}

	
#endif

#if DESC_SW("����", DBG_UART)

/*****************************************************************************
 �� �� ��  : DEV_UartInit
 ��������  : ���ڳ�ʼ��
 �������  : void  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��7��3��
    ��    ��   : linhao
    �޸�����   : �����ɺ���

*****************************************************************************/
void DEV_UartInit(void)
{   
    SCON = 0x50;                //8λ�ɱ䲨����

    T2L = (65536 - (FOSC/4/BAUD));   //���ò�������װֵ
    T2H = (65536 - (FOSC/4/BAUD))>>8;
    AUXR = 0x14;                //T2Ϊ1Tģʽ, ��������ʱ��2
    AUXR |= 0x01;               //ѡ��ʱ��2Ϊ����1�Ĳ����ʷ�����
    ES = 1;                     //ʹ�ܴ���1�ж�
    EA = 1;


    ES = 1;                     //??????
    EA = 1;
}

/*****************************************************************************
 �� �� ��  : DEV_Uart_Isr
 ��������  : �����ж�
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��7��3��
    ��    ��   : linhao
    �޸�����   : �����ɺ���

*****************************************************************************/
void DEV_Uart_Isr() interrupt 4 using 1
{
    if (RI)
    {
        RI = 0;

    }
    if (TI)
    {
        TI = 0;             //Clear transmit interrupt flag
        busy = 0;           //Clear transmit busy flag
    }
}

/*****************************************************************************
 �� �� ��  : DEV_Uart_SendData
 ��������  : ���ڷ���
 �������  : unsigned char dat  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��7��3��
    ��    ��   : linhao
    �޸�����   : �����ɺ���

*****************************************************************************/
void DEV_Uart_SendData(unsigned char dat)
{
    while (busy);           //Wait for the completion of the previous data is sent
    ACC = dat;              //Calculate the even parity bit P (PSW.0)
    busy = 1;
    SBUF = ACC;             //Send data to UART buffer
}

/*****************************************************************************
 �� �� ��  : DEV_Uart_SendString
 ��������  : ���ڷ����ַ���
 �������  : char *s  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��7��3��
    ��    ��   : linhao
    �޸�����   : �����ɺ���

*****************************************************************************/
void DEV_Uart_SendString(char *s)
{
    while (*s)              //Check the end of the string
    {
        DEV_Uart_SendData(*s++);     //Send current char and increment string ptr
    }
}


void DEV_Uart_ShowChar(UCHAR ucReg)
{
	UCHAR str[6];
	UCHAR ucTmp;
	str[0] = '0';
	str[1] = 'x';

	ucTmp = ucReg/16;
	ucTmp = ucTmp < 10 ? ucTmp + '0' : ucTmp + 'A' - 10;
	str[2] = ucTmp;

	ucTmp = ucReg%16;
	ucTmp = ucTmp < 10 ? ucTmp + '0' : ucTmp + 'A' - 10;
	str[3] = ucTmp;

	str[4] = ',';
	str[5] = 0;
	DBG_UART_STR(str);
}

#endif

#if DESC("����")

void DEV_KeyExInt0() interrupt 0
{

}

#endif

#if DESC("��ʼ��")

/*****************************************************************************
 �� �� ��  : DEV_Init
 ��������  : ��ʼ��
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��7��3��
    ��    ��   : linhao
    �޸�����   : �����ɺ���

*****************************************************************************/
void DEV_Init()
{
	UCHAR ucRet;
	
#if DBG_UART
		DEV_UartInit();
#endif

	DBG_UART_STR(" uart start work");

	P1M1 = 0x03; 
	P1M0 = 0x3C;
	CE = 0;			// chip enable
	CSN = 1;		// Spi disable	
	SCK = 0;		// Spi clock line init high

	DEV_DelayNms(10);

	ucRet = DRV_NRF24L01_Check();
	if (VOS_OK != ucRet)
	{
		DBG_UART_STR("\r\n 2401 check fail");
	}
	else
	{
		DBG_UART_STR("\r\n 2401 check success");
	}

	DRV_NRF24L01_Init();
		
	KEY_1 = 1;
	IT0 = 1;
	EX0 = 1;
    ES = 1;
    EA = 1;
}
#endif

