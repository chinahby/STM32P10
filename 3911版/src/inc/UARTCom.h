/// @file   UARTCOM.h
/// This is for module @ref UART_COM
/// @author Travis Tu
/// @date   2006-Sep-1st

#ifndef _UARTCOM_H_
#define _UARTCOM_H_

#define UART2_Rx_Pin (0x0001<<13)  ///<TQFP 64: pin N?5  , TQFP 144 pin N?9
#define UART2_Tx_Pin (0x0001<<14)  ///<TQFP 64: pin N?6  , TQFP 144 pin N?10

#define UART_OK           0  //��������
#define ERR_INCOMP       1  //����δ���
#define ERR_PORT         2  //�˿ںŴ���
#define ERR_NO_OPEN      3  //�˿�δ��
#define ERR_PARA         0xfe  //�˿����ò�������
#define ERR_OCCUPY       4  //�˿ڱ�ռ��
#define ERR_RX_TIMEOUT   0xff  //�ȴ����ݳ�ʱ


#define COM3	         2
#define COM2	         1
#define COM1	         0

#define MAX_PORT_NUM	 COM2


unsigned char  PortOpen(unsigned char,unsigned long,unsigned char,unsigned char,unsigned char);
unsigned char PortReset(unsigned char);
unsigned char PortRecv(unsigned char,  unsigned char*, unsigned int);
unsigned char PortSends(unsigned char, unsigned char*, unsigned short);
unsigned char PortRx(unsigned char, unsigned char*);
unsigned char PortSend(unsigned char, unsigned char);
unsigned char PortClose(unsigned char);

void debug_printf(u8 col,u8 row,u8 mode,char *str,...);

void UARTCom_IT(void);

#endif
