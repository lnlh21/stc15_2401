#include "public_def.h"
#include "stc15.h"
#include "nrf24l01.h"
#include "unit.h"
#include "rf.h"

void RF_Init()
{
    UCHAR i;
    UCHAR idata *iptr;

	iptr = 0xf3;
	for (i = 0; i < 5; i++)
    {
		LOCAL_ADDRESS[i] = (*iptr++);
   	}
}


VOID RF_Recv(UCHAR *pucRecvData)
{
    RF_CMD_HEAD_S *pstCmdHead;
	
	pstCmdHead = (RF_CMD_HEAD_S *)pucRecvData;
	DBG_UART_STR("rx,ok:");
	DBG_UART_CHAR(pstCmdHead->ucCmd);
	DBG_UART_CHAR(pstCmdHead->ucRet);
	DBG_UART_CHAR(pstCmdHead->ucSn);
	DBG_UART_STR("\r\n");
	
	if ((1 == pstCmdHead->ucCmd) && (0 == pstCmdHead->ucRet))
	{
	    DEV_FlashOK();
	}
}

void RF_AddCmdGetSn(UCHAR*pucBuf)
{
    static UCHAR ucSn = 0;
    UCHAR i;
	UCHAR idata *iptr;
	RF_CMD_HEAD_S *pstCmdHead;
	
	pstCmdHead = (RF_CMD_HEAD_S *)g_aucRfBuf;

	for (i = 0; i < 5; i++)
	{
		pstCmdHead->ucSrcAddr[i] = LOCAL_ADDRESS[i];
	}

	pstCmdHead->ucDir = 0;
	pstCmdHead->ucType = 0;
	pstCmdHead->ucCmd = 1;
	pstCmdHead->ucSn = ucSn;
	pstCmdHead->ucLen = 8;
	ucSn++;

	pstCmdHead->aucVal[0] = 0;
	iptr = 0xf1;
    for (i = 1; i < 8; i++)
    {
		pstCmdHead->aucVal[i] = (*iptr++);
   	}
}