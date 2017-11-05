#ifndef __UNIT_H_
#define __UNIT_H_

/*             _________   
		P1.2 -|         |- P1.1
		P1.3 -|         |- P1.0
		P1.4 -|  STC15  |- P3.7
		P1.5 -|   W201  |- P3.6
		P5.4 -|         |- P3.3 INT1
		VCC  -|         |- P3.2 INT0
		P5.5 -|         |- P3.1 TX
		GND  -|         |- P3.0 RX
              -----------
*/
#define BOARD_TYPE_MAINBOARD   1
#define BOARD_TYPE_SUB1        2 
#define BOARD_TYPE_SUB2        3 
#define BOARD_TYPE_SUB3        4 
	
#define BOARD_TYPE  BOARD_TYPE_SUB1

#define ENEGY_SAVE     0
#define DBG_UART        1
#define FOSC 			11059200L          //系统频率
#define BAUD 			115200             //串口波特率

sbit LED_1 = P5^5;
sbit KEY_1 = P3^2;

#define DEV_LED1_ON()    LED_1 = 0
#define DEV_LED1_OFF()   LED_1 = 1;
#define DEV_GET_KEY1()   KEY_1

void DEV_Init();
void DEV_Flash(UCHAR n);
void DEV_FlashOK();
void DEV_DelayNms(unsigned short n);
void DEV_Timer0Start(void);

#if DBG_UART
void DEV_Uart_SendData(unsigned char dat);
void DEV_Uart_SendString(char *s);
void DEV_UartInit(void);
void DEV_Uart_ShowChar(UCHAR ucReg);
#define DBG_UART_STR(x) DEV_Uart_SendString(x)
#define DBG_UART_CHAR(x) DEV_Uart_ShowChar(x)
#else
#define DBG_UART_STR(x) _nop_()
#define DBG_UART_CHAR(x) _nop_()
#endif


extern void UartInit(void);
extern void SendData(unsigned char dat);
extern void SendString(char *s);

#endif
