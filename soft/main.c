#include "public_def.h"
#include "stc15.h"
#include "unit.h"
#include "nrf24l01.h"
#include <intrins.h>
#include "rf.h"

UCHAR ucRxBuf[32];

/*****************************************************************************
 �� �� ��  : main
 ��������  : ������
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
void main()
{
    UCHAR ucKeyCount = 0;
	UCHAR i;
	USHORT usIdleTime = 0;
	UCHAR ucCount = 0;
	
	DEV_Init();
	DEV_Flash(3);
#if (BOARD_TYPE_MAINBOARD == BOARD_TYPE)

    DRV_NRF24L01_RX_Mode();

	while(1)
	{
		ucCount++;
	    ucRxBuf[0] = 1;
		ucRxBuf[1] = 0;
		ucRxBuf[2] = 0;
		ucRxBuf[3] = 0;
	    ucRxBuf[4] = ucCount % 2;
	    ucRxBuf[5] = 0;

        DRV_NRF24L01_TX_Mode(ucRxBuf[0]);
		DEV_DelayNms(5);
                
		if (VOS_OK == DRV_NRF24L01_TxPkt(ucRxBuf))
		{
           DEV_Uart_SendString("\r\n ok");
			if (ucRxBuf[4])
			{
				DEV_LED1_ON();
			}
			else
			{
				DEV_LED1_OFF();
			}
		}
        else
        {
           DEV_Uart_SendString("\r\n err");
		   DEV_LED1_ON();
		   DEV_DelayNms(50);
		   DEV_LED1_OFF();
        }
 
		DRV_NRF24L01_RX_Mode();
		DEV_DelayNms(500);
	}
#else
    DRV_NRF24L01_RX_Mode();

	while(1)
	{
		if (VOS_OK == DRV_NRF24L01_RxPkt(ucRxBuf))
		{
			ucCount++;
			if (ucCount%2)
			{
				DEV_LED1_ON();
			}
			else
			{
				DEV_LED1_OFF();
			}

			DEV_DelayNms(10);
		}
		
		DEV_DelayNms(10);
	}
#endif

}