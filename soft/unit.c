#include "public_def.h"
#include "stc15.h"
#include "unit.h"
#include "nrf24l01.h"
#include <intrins.h>

bit busy = 0;

#if DESC("时延")
/*****************************************************************************
 函 数 名  : DEV_DelayNms
 功能描述  : 延时
 输入参数  : unsigned short n  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年7月3日
    作    者   : linhao
    修改内容   : 新生成函数

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




void DEV_Timer0Start(void)		//10毫秒@11.0592MHz
{
	AUXR &= 0x7F;		//定时器时钟12T模式
	TMOD &= 0xF0;		//设置定时器模式
	TMOD |= 0x01;		//设置定时器模式
	TL0 = 0x00;		//设置定时初值
	TH0 = 0xDC;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
}



#endif

#if DESC("LED")


/*****************************************************************************
 函 数 名  : DEV_Flash
 功能描述  : 闪灯
 输入参数  : UCHAR n  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年7月3日
    作    者   : linhao
    修改内容   : 新生成函数

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

#if DESC_SW("串口", DBG_UART)

/*****************************************************************************
 函 数 名  : DEV_UartInit
 功能描述  : 串口初始化
 输入参数  : void  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年7月3日
    作    者   : linhao
    修改内容   : 新生成函数

*****************************************************************************/
void DEV_UartInit(void)
{   
    SCON = 0x50;                //8位可变波特率

    T2L = (65536 - (FOSC/4/BAUD));   //设置波特率重装值
    T2H = (65536 - (FOSC/4/BAUD))>>8;
    AUXR = 0x14;                //T2为1T模式, 并启动定时器2
    AUXR |= 0x01;               //选择定时器2为串口1的波特率发生器
    ES = 1;                     //使能串口1中断
    EA = 1;


    ES = 1;                     //??????
    EA = 1;
}

/*****************************************************************************
 函 数 名  : DEV_Uart_Isr
 功能描述  : 串口中断
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年7月3日
    作    者   : linhao
    修改内容   : 新生成函数

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
 函 数 名  : DEV_Uart_SendData
 功能描述  : 串口发送
 输入参数  : unsigned char dat  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年7月3日
    作    者   : linhao
    修改内容   : 新生成函数

*****************************************************************************/
void DEV_Uart_SendData(unsigned char dat)
{
    while (busy);           //Wait for the completion of the previous data is sent
    ACC = dat;              //Calculate the even parity bit P (PSW.0)
    busy = 1;
    SBUF = ACC;             //Send data to UART buffer
}

/*****************************************************************************
 函 数 名  : DEV_Uart_SendString
 功能描述  : 串口发送字符串
 输入参数  : char *s  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年7月3日
    作    者   : linhao
    修改内容   : 新生成函数

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

#if DESC("按键")

void DEV_KeyExInt0() interrupt 0
{

}

#endif

#if DESC("初始化")

/*****************************************************************************
 函 数 名  : DEV_Init
 功能描述  : 初始化
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年7月3日
    作    者   : linhao
    修改内容   : 新生成函数

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

