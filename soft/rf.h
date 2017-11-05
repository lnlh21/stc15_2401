#ifndef __DRV_RF_H__
#define __DRV_RF_H__

typedef struct
{
    UCHAR ucSrcAddr[5];
    UCHAR ucDir;
    UCHAR ucCmd;
	UCHAR ucType;
	UCHAR ucSn;
	UCHAR ucRet;
	UCHAR ucLen;
	UCHAR aucVal[1];
}RF_CMD_HEAD_S;


void RF_Init();
void RF_AddCmdGetSn(UCHAR*pucBuf);
#endif