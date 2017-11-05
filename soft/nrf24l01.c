#include "public_def.h"
#include "stc15.h"
#include "nrf24l01.h"
#include "unit.h"


/* 广播地址 */
const UCHAR g_aucBcAddr[DRV_NRF24L01_RX_ADR_WIDTH]={0xff,0x43,0x10,0x10,0x00};
UCHAR g_aucTxAddr[DRV_NRF24L01_TX_ADR_WIDTH]={0x00,0x43,0x10,0x10,0x00}; //发送地址

#if (BOARD_TYPE_MAINBOARD == BOARD_TYPE)
const UCHAR g_aucMyAddr[DRV_NRF24L01_RX_ADR_WIDTH]={0xfe,0x43,0x10,0x10,0x00}; //接收地址
#elif (BOARD_TYPE_SUB1 == BOARD_TYPE)
const UCHAR g_aucMyAddr[DRV_NRF24L01_RX_ADR_WIDTH]={0x01,0x43,0x10,0x10,0x00}; //接收地址
#endif 


void delay1us(UCHAR t)
{
	while(--t);
} 

UCHAR DRV_NRF2401_SpiSend(UCHAR byte)
{
    UCHAR bit_ctr;
	
   	for (bit_ctr = 0; bit_ctr < 8; bit_ctr++)   // output 8-bit
   	{
   		MOSI = (byte & 0x80);         // output 'byte', MSB to MOSI
   		byte = (byte << 1);           // shift next bit into MSB..
   		SCK = 1;                      // Set SCK high..
   		delay1us(1);
   		byte |= MISO;       		  // capture current MISO bit
   		SCK = 0;            		  // ..then set SCK low again
   		delay1us(1);
   	}
    return(byte);           		  // return read byte
}

#if DESC("寄出器操作")

UCHAR DRV_NRF24L01_WriteReg(UCHAR ucReg, UCHAR ucValue)
{
	UCHAR ucStatus;	
    
    /* 片选 */
	DRV_NRF24L01_CSN_LOW();
	
	/* 发送寄存器号 */
  	ucStatus = DRV_NRF24L01_SPI_WRITE(ucReg);

	/* 写入寄存器的值 */
  	DRV_NRF24L01_SPI_WRITE(ucValue);

	/* 取消片选 */
  	DRV_NRF24L01_CSN_HIGH();
	
  	return ucStatus;
}

UCHAR DRV_NRF24L01_ReadReg(UCHAR ucReg)
{
	UCHAR ucValue;
	
    /* 片选 */
	DRV_NRF24L01_CSN_LOW();

	/* 发送寄存器号 */
  	DRV_NRF24L01_SPI_WRITE(ucReg);
  	
	/* 读取寄存器内容 */
  	ucValue = DRV_NRF24L01_SPI_READ();

	/* 取消片选 */
  	DRV_NRF24L01_CSN_HIGH();
	
  	return ucValue;
}	

UCHAR DRV_NRF24L01_ReadBuf(UCHAR reg, UCHAR *pBuf, UCHAR ucLen)
{
	UCHAR ucStatus;
	UCHAR ucLoop;	  

    /* 片选 */
	DRV_NRF24L01_CSN_LOW();

    /* 发送寄存器值(位置),并读取状态值 */
  	DRV_NRF24L01_SPI_WRITE(reg);

	/* 读出数据 */
 	for(ucLoop = 0; ucLoop < ucLen; ucLoop++)
	{
		pBuf[ucLoop] = DRV_NRF24L01_SPI_READ();
 	}
 	
 	/* 取消片选 */
  	DRV_NRF24L01_CSN_HIGH();
	
  	return ucStatus;
}

UCHAR DRV_NRF24L01_WriteBuf(UCHAR ucReg, UCHAR *pBuf, UCHAR ucLen)
{
	UCHAR ucStatus;
	UCHAR ucLoop;
	
    /* 片选 */
	DRV_NRF24L01_CSN_LOW();

	/* 发送寄存器值(位置),并读取状态值 */
  	DRV_NRF24L01_SPI_WRITE(ucReg);

	/* 写入数据 */
  	for (ucLoop = 0; ucLoop < ucLen; ucLoop++)
	{
		DRV_NRF24L01_SPI_WRITE(*pBuf++);
  	}
  	
	/* 取消片选 */
  	DRV_NRF24L01_CSN_HIGH();

  	return ucStatus;
}				   
#endif

#if DESC("自检")
UCHAR DRV_NRF24L01_Check(VOID)
{
	UCHAR buf[5] = {0XA5, 0XA5, 0XA5, 0XA5, 0XA5};
	UCHAR i;

	/* 写入5个字节的地址 */
	DRV_NRF24L01_WriteBuf(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_TX_ADDR, buf, 5);

	/* 读出写入的地址 */
	DRV_NRF24L01_ReadBuf(DRV_NRF24L01_TX_ADDR, buf, 5);
	for (i = 0; i < 5; i++)
	{
		if(buf[i] != 0XA5)
		{
			break;
		}
	}

	if (i != 5)
	{
		return VOS_ERR;
	}

	return VOS_OK;
}	 

#endif

#if DESC("收发包")
UCHAR DRV_NRF24L01_TxPkt(UCHAR *txbuf)
{
	UCHAR ucStatus;
 
 	/* 拉低CE */
 	DRV_NRF24L01_CE_LOW();

	/* 写数据到TX BUF  32个字节 */
  	DRV_NRF24L01_WriteBuf(DRV_NRF24L01_WR_TX_PLOAD, txbuf, DRV_NRF24L01_TX_PLOAD_WIDTH);

 	/* 启动发送	*/
 	DRV_NRF24L01_CE_HIGH();

	/* 等待发送完成 */
	while(DRV_NRF24L01_GET_IRQ() != 0);
	
	/* 读取状态寄存器的值 */
	ucStatus = DRV_NRF24L01_ReadReg(DRV_NRF24L01_STATUS);
	
	/* 清除TX_DS或MAX_RT中断标志 */
	DRV_NRF24L01_WriteReg(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_STATUS, ucStatus);

	if (ucStatus & DRV_NRF24L01_STATUS_BIT_MAX_TX)//达到最大重发次数
	{
		/* 清除TX FIFO寄存器 */
		DRV_NRF24L01_WriteReg(DRV_NRF24L01_FLUSH_TX, 0xff);

		return ucStatus; 
	}
	if (ucStatus & DRV_NRF24L01_STATUS_BIT_TX_OK)//发送完成
	{
		return VOS_OK;
	}
	
	return VOS_ERR;
}

//启动NRF24L01发送一次数据
//txbuf:待发送数据首地址
//返回值:0，接收完成；其他，错误代码
UCHAR DRV_NRF24L01_RxPkt(UCHAR *rxbuf)
{
	UCHAR ucStatus;

	/* 如果没有中断 */
	if (0 != DRV_NRF24L01_GET_IRQ())
	{
		return VOS_ERR;
	}

	DEV_Uart_SendString("\r\n have interrupt");

	/* 读取状态寄存器的值 */
	ucStatus = DRV_NRF24L01_ReadReg(DRV_NRF24L01_STATUS);

	/* 清除TX_DS或MAX_RT中断标志 */
	DRV_NRF24L01_WriteReg(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_STATUS, ucStatus);
    
	/* 接收到数据 */
	if (ucStatus & DRV_NRF24L01_STATUS_BIT_RX_OK)
	{
		/* 读取数据 */
		DRV_NRF24L01_ReadBuf(DRV_NRF24L01_RD_RX_PLOAD, rxbuf, DRV_NRF24L01_RX_PLOAD_WIDTH);

		/* 清除RX FIFO寄存器 */
		DRV_NRF24L01_WriteReg(DRV_NRF24L01_FLUSH_RX, 0xff);
		return VOS_OK; 
	}

	return VOS_ERR;//没收到任何数据
}

//该函数初始化NRF24L01到RX模式
//设置RX地址,写RX数据宽度,选择RF频道,波特率和LNA HCURR
//当CE变高后,即进入RX模式,并可以接收数据了		   
void DRV_NRF24L01_RX_Mode(void)
{
    /* 拉低CE */
	DRV_NRF24L01_CE_LOW();

	/* 使能通道1的接收地址 */
  	DRV_NRF24L01_WriteReg(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_EN_RXADDR, 0x06);

	/* 配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式 */
  	DRV_NRF24L01_WriteReg(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_CONFIG, 0x0f);

	/* CE为高,进入接收模式 */
	DRV_NRF24L01_CE_HIGH();
}		

//该函数初始化NRF24L01到TX模式
//设置TX地址,写TX数据宽度,设置RX自动应答的地址,填充TX发送数据,选择RF频道,波特率和LNA HCURR
//PWR_UP,CRC使能
//当CE变高后,即进入RX模式,并可以接收数据了		   
//CE为高大于10us,则启动发送.	 
VOID DRV_NRF24L01_TX_Mode(UCHAR ucNodId)
{														 
    /* 拉低CE */
	DRV_NRF24L01_CE_LOW();

	g_aucTxAddr[0] = ucNodId;

	if (0xff == ucNodId)
	{
		/* 设置自动重发间隔时间:500us + 86us;最大自动重发次数:10次 */
  		DRV_NRF24L01_WriteReg(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_SETUP_RETR, 0x00);
	}
	else
	{
		/* 设置自动重发间隔时间:500us + 86us;最大自动重发次数:10次 */
  		DRV_NRF24L01_WriteReg(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_SETUP_RETR, 0x1a);
	}

	/* 写TX节点地址 */
  	DRV_NRF24L01_WriteBuf(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_TX_ADDR, (UCHAR*)g_aucTxAddr, DRV_NRF24L01_TX_ADR_WIDTH);

	/* 设置TX节点地址,主要为了使能ACK */
  	DRV_NRF24L01_WriteBuf(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_RX_ADDR_P0, (UCHAR*)g_aucTxAddr, DRV_NRF24L01_TX_ADR_WIDTH);

	/* 使能通道0的接收地址 */
  	DRV_NRF24L01_WriteReg(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_EN_RXADDR, 0x01);

	/* 配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式,开启所有中断 */
  	DRV_NRF24L01_WriteReg(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_CONFIG, 0x0e);
}	

VOID DRV_NRF24L01_Init(VOID)
{	
	/* 设置TX节点地址,主要为了使能ACK */
  	//DRV_NRF24L01_WriteBuf(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_RX_ADDR_P0, (UCHAR*)g_aucTxAddr, DRV_NRF24L01_TX_ADR_WIDTH);

	/* 设置TX节点地址,主要为了使能ACK */
	//DRV_NRF24L01_WriteBuf(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_RX_ADDR_P1, (UCHAR*)g_aucRxAddr, DRV_NRF24L01_TX_ADR_WIDTH);

	/* 选择通道0的有效数据宽度 */
  	//DRV_NRF24L01_WriteReg(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_RX_PW_P0, DRV_NRF24L01_RX_PLOAD_WIDTH);

    /* 通道0用于发送时接收ACK，地址在发送时设置 */
	/* 通道1用于接收单播数据，带自动回复 */
	/* 通道2yo用于广播数据，无自动回复 */
	DRV_NRF24L01_WriteBuf(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_RX_ADDR_P1, (UCHAR*)g_aucMyAddr, DRV_NRF24L01_TX_ADR_WIDTH);
    DRV_NRF24L01_WriteBuf(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_RX_ADDR_P2, (UCHAR*)g_aucBcAddr, 1);

	/* 选择通道1和2的有效数据宽度 */
  	DRV_NRF24L01_WriteReg(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_RX_PW_P1, DRV_NRF24L01_RX_PLOAD_WIDTH);
	DRV_NRF24L01_WriteReg(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_RX_PW_P2, DRV_NRF24L01_RX_PLOAD_WIDTH);

	/* 使能通道0和1的自动应答 */
  	DRV_NRF24L01_WriteReg(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_EN_AA, 0x07);

	/* 使能通道2和3的接收地址 */
  	DRV_NRF24L01_WriteReg(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_EN_RXADDR, 0x06);

	/* 设置RF通道为40 */
  	DRV_NRF24L01_WriteReg(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_RF_CH, 40);

	/* 设置TX发射参数,0db增益,2Mbps,低噪声增益开启 */
	DRV_NRF24L01_WriteReg(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_RF_SETUP, 0x0f);

}

#endif


