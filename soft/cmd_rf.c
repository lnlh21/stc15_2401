/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ���������������޹�˾

 ******************************************************************************
  �� �� ��   : cmd_rf.c
  �� �� ��   : ����
  ��    ��   : linhao
  ��������   : 2015��1��11��
  ����޸�   :
  ��������   : rf�����
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2015��1��11��
    ��    ��   : linhao
    �޸�����   : �����ļ�

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
