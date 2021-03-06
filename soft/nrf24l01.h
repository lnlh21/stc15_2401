/******************************************************************************

                  版权所有 (C), 2001-2011, 神州数码网络有限公司

 ******************************************************************************
  文 件 名   : drv_nrf2401.h
  版 本 号   : 初稿
  作    者   : linhao
  生成日期   : 2014年12月28日
  最近修改   :
  功能描述   : drv_nrf2401.c 的头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2014年12月28日
    作    者   : linhao
    修改内容   : 创建文件

******************************************************************************/
#ifndef __DRV_NRF2401_H__
#define __DRV_NRF2401_H__


/* 
               _________   
		P1.2 -|         |- P1.1
		P1.3 -|         |- P1.0
		P1.4 -|  STC15  |- P3.7
		P1.5 -|   W201  |- P3.6
		P5.4 -|         |- P3.3 INT1
		VCC  -|         |- P3.2 INT0
		P5.5 -|         |- P3.1 TX
		GND  -|         |- P3.0 RX
              -----------


模块接口图

--------------------------------
|                 |            |
| |               |IRQ  MISO   |
| |__     ___     |MOSI SCK    |
|    | | |  |     |CSN  CE     |
| |~~  | ~~~      |VCC  GND    |
|  ~~~~~~~        |            |
--------------------------------


*/

sbit CE =  P1^2;
sbit CSN=  P1^3;
sbit SCK=  P1^4;
sbit MOSI= P1^5;
sbit MISO= P1^0;
sbit IRQ = P1^1;

// P1M1 0000 0011
// P1M0 0011 1100

#define DRV_NRF24L01_CE_HIGH()   		(CE=1)
#define DRV_NRF24L01_CE_LOW()    		(CE=0)
#define DRV_NRF24L01_CSN_HIGH()  		(CSN=1)
#define DRV_NRF24L01_CSN_LOW()   		(CSN=0)
#define DRV_NRF24L01_SCK_LOW()   		(SCK=0)

#define DRV_NRF24L01_GET_IRQ()   		(IRQ)
#define DRV_NRF24L01_SPI_WRITE(reg) 	DRV_NRF2401_SpiSend(reg)
#define DRV_NRF24L01_SPI_READ()    	DRV_NRF2401_SpiSend(0xff)

/* 2401命令 */
#define DRV_NRF24L01_READ_REG        0x00  //读配置寄存器,低5位为寄存器地址
#define DRV_NRF24L01_WRITE_REG       0x20  //写配置寄存器,低5位为寄存器地址
#define DRV_NRF24L01_RD_RX_PLOAD     0x61  //读RX有效数据,1~32字节
#define DRV_NRF24L01_WR_TX_PLOAD     0xA0  //写TX有效数据,1~32字节
#define DRV_NRF24L01_FLUSH_TX        0xE1  //清除TX FIFO寄存器.发射模式下用
#define DRV_NRF24L01_FLUSH_RX        0xE2  //清除RX FIFO寄存器.接收模式下用
#define DRV_NRF24L01_REUSE_TX_PL     0xE3  //重新使用上一包数据,CE为高,数据包被不断发送.
#define DRV_NRF24L01_NOP             0xFF  //空操作,可以用来读状态寄存器 

//SPI(NRF24L01)寄存器地址
#define DRV_NRF24L01_CONFIG          0x00  //配置寄存器地址;bit0:1接收模式,0发射模式;bit1:电选择;bit2:CRC模式;bit3:CRC使能;
                              //bit4:中断MAX_RT(达到最大重发次数中断)使能;bit5:中断TX_DS使能;bit6:中断RX_DR使能
#define DRV_NRF24L01_EN_AA           0x01  //使能自动应答功能  bit0~5,对应通道0~5
#define DRV_NRF24L01_EN_RXADDR       0x02  //接收地址允许,bit0~5,对应通道0~5
#define DRV_NRF24L01_SETUP_AW        0x03  //设置地址宽度(所有数据通道):bit1,0:00,3字节;01,4字节;02,5字节;
#define DRV_NRF24L01_SETUP_RETR      0x04  //建立自动重发;bit3:0,自动重发计数器;bit7:4,自动重发延时 250*x+86us
#define DRV_NRF24L01_RF_CH           0x05  //RF通道,bit6:0,工作通道频率;
#define DRV_NRF24L01_RF_SETUP        0x06  //RF寄存器;bit3:传输速率(0:1Mbps,1:2Mbps);bit2:1,发射功率;bit0:低噪声放大器增益
#define DRV_NRF24L01_STATUS          0x07  //状态寄存器;bit0:TX FIFO满标志;bit3:1,接收数据通道号(最大:6);bit4,达到最多次重发
                              //bit5:数据发送完成中断;bit6:接收数据中断;
#define DRV_NRF24L01_STATUS_BIT_MAX_TX  	0x10  //达到最大发送次数中断
#define DRV_NRF24L01_STATUS_BIT_TX_OK   	0x20  //TX发送完成中断
#define DRV_NRF24L01_STATUS_BIT_RX_OK   	0x40  //接收到数据中断

#define DRV_NRF24L01_OBSERVE_TX      0x08  //发送检测寄存器,bit7:4,数据包丢失计数器;bit3:0,重发计数器
#define DRV_NRF24L01_CD              0x09  //载波检测寄存器,bit0,载波检测;
#define DRV_NRF24L01_RX_ADDR_P0      0x0A  //数据通道0接收地址,最大长度5个字节,低字节在前
#define DRV_NRF24L01_RX_ADDR_P1      0x0B  //数据通道1接收地址,最大长度5个字节,低字节在前
#define DRV_NRF24L01_RX_ADDR_P2      0x0C  //数据通道2接收地址,最低字节可设置,高字节,必须同RX_ADDR_P1[39:8]相等;
#define DRV_NRF24L01_RX_ADDR_P3      0x0D  //数据通道3接收地址,最低字节可设置,高字节,必须同RX_ADDR_P1[39:8]相等;
#define DRV_NRF24L01_RX_ADDR_P4      0x0E  //数据通道4接收地址,最低字节可设置,高字节,必须同RX_ADDR_P1[39:8]相等;
#define DRV_NRF24L01_RX_ADDR_P5      0x0F  //数据通道5接收地址,最低字节可设置,高字节,必须同RX_ADDR_P1[39:8]相等;
#define DRV_NRF24L01_TX_ADDR         0x10  //发送地址(低字节在前),ShockBurstTM模式下,RX_ADDR_P0与此地址相等
#define DRV_NRF24L01_RX_PW_P0        0x11  //接收数据通道0有效数据宽度(1~32字节),设置为0则非法
#define DRV_NRF24L01_RX_PW_P1        0x12  //接收数据通道1有效数据宽度(1~32字节),设置为0则非法
#define DRV_NRF24L01_RX_PW_P2        0x13  //接收数据通道2有效数据宽度(1~32字节),设置为0则非法
#define DRV_NRF24L01_RX_PW_P3        0x14  //接收数据通道3有效数据宽度(1~32字节),设置为0则非法
#define DRV_NRF24L01_RX_PW_P4        0x15  //接收数据通道4有效数据宽度(1~32字节),设置为0则非法
#define DRV_NRF24L01_RX_PW_P5        0x16  //接收数据通道5有效数据宽度(1~32字节),设置为0则非法
#define DRV_NRF24L01_FIFO_STATUS     0x17  //FIFO状态寄存器;bit0,RX FIFO寄存器空标志;bit1,RX FIFO满标志;bit2,3,保留
                              //bit4,TX FIFO空标志;bit5,TX FIFO满标志;bit6,1,循环发送上一数据包.0,不循环;

//24L01发送接收数据宽度定义
#define DRV_NRF24L01_TX_ADR_WIDTH    5   //5字节的地址宽度
#define DRV_NRF24L01_RX_ADR_WIDTH    5   //5字节的地址宽度
#define DRV_NRF24L01_TX_PLOAD_WIDTH  32  //20字节的用户数据宽度
#define DRV_NRF24L01_RX_PLOAD_WIDTH  32  //20字节的用户数据宽度

UCHAR DRV_NRF24L01_Check(VOID);
UCHAR DRV_NRF24L01_TxPkt(UCHAR *txbuf);
UCHAR DRV_NRF24L01_RxPkt(UCHAR *rxbuf);
VOID DRV_NRF24L01_TX_Mode(UCHAR ucNodId);
void DRV_NRF24L01_RX_Mode(void);




#endif /* __DRV_NRF2401_H__ */

