/******************************************************************************

                  版权所有 (C), 2001-2011, 神州数码网络有限公司

 ******************************************************************************
  文 件 名   : cmd_rf.c
  版 本 号   : 初稿
  作    者   : linhao
  生成日期   : 2015年1月11日
  最近修改   :
  功能描述   : rf命令处理
  函数列表   :
  修改历史   :
  1.日    期   : 2015年1月11日
    作    者   : linhao
    修改内容   : 创建文件

******************************************************************************/
#include "public_def.h"
#include "stc15.h"
#include "cmd_rf.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "unit.h"
#include "nrf24l01.h"
#include "usart.h"

UCHAR ucRfTxBuf[32];
extern UCHAR REMOTE_ADDRESS[];

VOID RF_Recv(UCHAR *pucRecvData)
{
}

VOID RF_Send()
{
    //NRF24L01_Send(REMOTE_ADDRESS, ucRfTxBuf);
}

#endif
