/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ���������������޹�˾

 ******************************************************************************
  �� �� ��   : drv_nrf2401.h
  �� �� ��   : ����
  ��    ��   : linhao
  ��������   : 2014��12��28��
  ����޸�   :
  ��������   : drv_nrf2401.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2014��12��28��
    ��    ��   : linhao
    �޸�����   : �����ļ�

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


ģ��ӿ�ͼ

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

/* 2401���� */
#define DRV_NRF24L01_READ_REG        0x00  //�����üĴ���,��5λΪ�Ĵ�����ַ
#define DRV_NRF24L01_WRITE_REG       0x20  //д���üĴ���,��5λΪ�Ĵ�����ַ
#define DRV_NRF24L01_RD_RX_PLOAD     0x61  //��RX��Ч����,1~32�ֽ�
#define DRV_NRF24L01_WR_TX_PLOAD     0xA0  //дTX��Ч����,1~32�ֽ�
#define DRV_NRF24L01_FLUSH_TX        0xE1  //���TX FIFO�Ĵ���.����ģʽ����
#define DRV_NRF24L01_FLUSH_RX        0xE2  //���RX FIFO�Ĵ���.����ģʽ����
#define DRV_NRF24L01_REUSE_TX_PL     0xE3  //����ʹ����һ������,CEΪ��,���ݰ������Ϸ���.
#define DRV_NRF24L01_NOP             0xFF  //�ղ���,����������״̬�Ĵ��� 

//SPI(NRF24L01)�Ĵ�����ַ
#define DRV_NRF24L01_CONFIG          0x00  //���üĴ�����ַ;bit0:1����ģʽ,0����ģʽ;bit1:��ѡ��;bit2:CRCģʽ;bit3:CRCʹ��;
                              //bit4:�ж�MAX_RT(�ﵽ����ط������ж�)ʹ��;bit5:�ж�TX_DSʹ��;bit6:�ж�RX_DRʹ��
#define DRV_NRF24L01_EN_AA           0x01  //ʹ���Զ�Ӧ����  bit0~5,��Ӧͨ��0~5
#define DRV_NRF24L01_EN_RXADDR       0x02  //���յ�ַ����,bit0~5,��Ӧͨ��0~5
#define DRV_NRF24L01_SETUP_AW        0x03  //���õ�ַ����(��������ͨ��):bit1,0:00,3�ֽ�;01,4�ֽ�;02,5�ֽ�;
#define DRV_NRF24L01_SETUP_RETR      0x04  //�����Զ��ط�;bit3:0,�Զ��ط�������;bit7:4,�Զ��ط���ʱ 250*x+86us
#define DRV_NRF24L01_RF_CH           0x05  //RFͨ��,bit6:0,����ͨ��Ƶ��;
#define DRV_NRF24L01_RF_SETUP        0x06  //RF�Ĵ���;bit3:��������(0:1Mbps,1:2Mbps);bit2:1,���书��;bit0:�������Ŵ�������
#define DRV_NRF24L01_STATUS          0x07  //״̬�Ĵ���;bit0:TX FIFO����־;bit3:1,��������ͨ����(���:6);bit4,�ﵽ�����ط�
                              //bit5:���ݷ�������ж�;bit6:���������ж�;
#define DRV_NRF24L01_STATUS_BIT_MAX_TX  	0x10  //�ﵽ����ʹ����ж�
#define DRV_NRF24L01_STATUS_BIT_TX_OK   	0x20  //TX��������ж�
#define DRV_NRF24L01_STATUS_BIT_RX_OK   	0x40  //���յ������ж�

#define DRV_NRF24L01_OBSERVE_TX      0x08  //���ͼ��Ĵ���,bit7:4,���ݰ���ʧ������;bit3:0,�ط�������
#define DRV_NRF24L01_CD              0x09  //�ز����Ĵ���,bit0,�ز����;
#define DRV_NRF24L01_RX_ADDR_P0      0x0A  //����ͨ��0���յ�ַ,��󳤶�5���ֽ�,���ֽ���ǰ
#define DRV_NRF24L01_RX_ADDR_P1      0x0B  //����ͨ��1���յ�ַ,��󳤶�5���ֽ�,���ֽ���ǰ
#define DRV_NRF24L01_RX_ADDR_P2      0x0C  //����ͨ��2���յ�ַ,����ֽڿ�����,���ֽ�,����ͬRX_ADDR_P1[39:8]���;
#define DRV_NRF24L01_RX_ADDR_P3      0x0D  //����ͨ��3���յ�ַ,����ֽڿ�����,���ֽ�,����ͬRX_ADDR_P1[39:8]���;
#define DRV_NRF24L01_RX_ADDR_P4      0x0E  //����ͨ��4���յ�ַ,����ֽڿ�����,���ֽ�,����ͬRX_ADDR_P1[39:8]���;
#define DRV_NRF24L01_RX_ADDR_P5      0x0F  //����ͨ��5���յ�ַ,����ֽڿ�����,���ֽ�,����ͬRX_ADDR_P1[39:8]���;
#define DRV_NRF24L01_TX_ADDR         0x10  //���͵�ַ(���ֽ���ǰ),ShockBurstTMģʽ��,RX_ADDR_P0��˵�ַ���
#define DRV_NRF24L01_RX_PW_P0        0x11  //��������ͨ��0��Ч���ݿ���(1~32�ֽ�),����Ϊ0��Ƿ�
#define DRV_NRF24L01_RX_PW_P1        0x12  //��������ͨ��1��Ч���ݿ���(1~32�ֽ�),����Ϊ0��Ƿ�
#define DRV_NRF24L01_RX_PW_P2        0x13  //��������ͨ��2��Ч���ݿ���(1~32�ֽ�),����Ϊ0��Ƿ�
#define DRV_NRF24L01_RX_PW_P3        0x14  //��������ͨ��3��Ч���ݿ���(1~32�ֽ�),����Ϊ0��Ƿ�
#define DRV_NRF24L01_RX_PW_P4        0x15  //��������ͨ��4��Ч���ݿ���(1~32�ֽ�),����Ϊ0��Ƿ�
#define DRV_NRF24L01_RX_PW_P5        0x16  //��������ͨ��5��Ч���ݿ���(1~32�ֽ�),����Ϊ0��Ƿ�
#define DRV_NRF24L01_FIFO_STATUS     0x17  //FIFO״̬�Ĵ���;bit0,RX FIFO�Ĵ����ձ�־;bit1,RX FIFO����־;bit2,3,����
                              //bit4,TX FIFO�ձ�־;bit5,TX FIFO����־;bit6,1,ѭ��������һ���ݰ�.0,��ѭ��;

//24L01���ͽ������ݿ��ȶ���
#define DRV_NRF24L01_TX_ADR_WIDTH    5   //5�ֽڵĵ�ַ����
#define DRV_NRF24L01_RX_ADR_WIDTH    5   //5�ֽڵĵ�ַ����
#define DRV_NRF24L01_TX_PLOAD_WIDTH  32  //20�ֽڵ��û����ݿ���
#define DRV_NRF24L01_RX_PLOAD_WIDTH  32  //20�ֽڵ��û����ݿ���

UCHAR DRV_NRF24L01_Check(VOID);
UCHAR DRV_NRF24L01_TxPkt(UCHAR *txbuf);
UCHAR DRV_NRF24L01_RxPkt(UCHAR *rxbuf);
VOID DRV_NRF24L01_TX_Mode(UCHAR ucNodId);
void DRV_NRF24L01_RX_Mode(void);




#endif /* __DRV_NRF2401_H__ */
