#ifndef _MIFARE_TMDEF_H_
#define	_MIFARE_TMDEF_H_

/**************************************************************
*common section
**************************************************************/

//#define RF_L1_RECDBG//emv test switch

//#define Debug_Isr//����512�жϼ�����������
//#define PN512_Debug_Isr//����512�жϼ�����������

//#define FOR_LINUX_SYS  

#define TERM_KF900  1
#define TERM_KF322  2


#define TERM_TYPE TERM_KF322


extern unsigned char _glbPiccSetPara;

//extern void DelayMs(unsigned int ms);

#ifdef FOR_LINUX_SYS
	#include <linux/types.h>
	#include <linux/module.h>
	#include <linux/moduleparam.h>
	
	#include <linux/timer.h>
	#include <linux/cdev.h>
	#include <linux/delay.h>
	#include <linux/watchdog.h>
	#include <linux/fs.h>
	#include <linux/init.h>
	#include <linux/platform_device.h>
	#include <linux/interrupt.h>
	#include <linux/clk.h>
	#include <linux/kthread.h>
	#include <linux/wait.h>
#endif

#ifndef uchar
#define uchar unsigned char
#endif
#ifndef ulong
#define ulong unsigned long
#endif
#ifndef uint
#define uint unsigned int
#endif
#ifndef ushort
#define ushort unsigned short
#endif

typedef struct
{
    volatile    uchar ucRFOpen;
    volatile	ulong ulSendBytes;      // Ԥ�Ʒ������ݳ���
    volatile    ulong ulBytesSent;      // ��ǰ�ѷ����ֽ���
    volatile    ulong ulBytesReceived;  // ��ǰ�Ѿ������ֽ���
    volatile    long lBitsReceived;    // ��ǰ�Ѿ�����λ��
    volatile    uchar ucAnticol;       // ����ͻ������־
    volatile    uchar ucCurResult;     // �����������еĽ��
    volatile    uchar ucINTSource;     // ���������������з������ж�
    volatile    uchar ucErrFlags;      // ������ִ���в��������־
    volatile    uchar ucSaveErrState;  // ����������ִ���в��������־
    volatile    uchar ucMifCardType;
    volatile	uchar ucCurType;        // ��ǰʹ�õĿ����ͺ�: EM_mifs_TYPEA-TYPE A, EM_mifs_TYPEB-TYPE B
    volatile    uchar ucCurPCB;         // ֡��
    
    volatile  	uchar ucFWI;            // ��ʱ�ȴ�ʱ������
    volatile	uchar ucSAK1;           // ��һ��SELECT����SAKֵ
    volatile	uchar ucSAK2;           // �ڶ���SELECT����SAKֵ
    volatile	uchar ucSAK3;           // ������SELECT����SAKֵ
    
    volatile	uchar ucMifActivateFlag; // ��Ƶ�������־ 0 - δ����   1 - �Ѽ���
    volatile	uchar ucUIDLen;          // ��Ƶ����UID���ȣ�TypeA�Ϳ�������4��7��10�ֽڣ�TypeB��ֻ��4�ֽ�
    volatile	uchar ucATQA[2];         // ��Ƶ����WUPA�����з��ص�ATQA����
    
    volatile	uchar ucATQB[12];        // ��Ƶ����WUPB�����з��صĲ���
    volatile	uchar ucUIDCL1[5];       // TypeA��һ�����кţ�����BCC�ֽ�
    volatile	uchar ucUIDCL2[5];       // TypeA�ڶ������кţ�����BCC�ֽ�
    volatile	uchar ucUIDCL3[5];       // TypeA���������кţ�����BCC�ֽ�
    volatile  	uchar ucUIDB[4];         // TypeB���к�
    volatile	uchar ucSFGI;            // TypeA����Ƶ����RATS�������֡�ȴ�ʱ������
    
            // ����PICC�Ƿ�֧��CID��NAD���ƣ�
            // ����PAYPASSҪ����PCD��PICC֮�����ʹ��CID��NAD����
    volatile	uchar ucCIDFlag;         // PICC�Ƿ�֧��CID���ƣ�0 - ��֧�֣� 1 - ֧��
    volatile    uchar ucNADFlag;         // PICC�Ƿ�֧��NAD���ƣ�0 - ��֧�֣� 1 - ֧��
    volatile	uchar ucWTX;             // ֡�ȴ�ʱ����չ������������TypeA��TypeB��
    
    volatile	ulong ulSFGT;            // TypeA����Ƶ����RATS�������֡�ȴ�ʱ�䣬 ��λ��ETU��128/13.56MHz��
    volatile	ulong ulFWT;             // ֡�ȴ�ʱ�䣬������TypeA��TypeB������λ��ETU��128/13.56MHz��
    volatile	ulong ulFWTTemp;         // ֡�ȴ�ʱ����չ��������TypeA��TypeB������λ��ETU��128/13.56MHz��
    
    volatile	uint  FSC;      		 // FSC��ֵ,���ܽ��յ�֡��󳤶�
    volatile  	uint  FSD;      		 // FSD��ֵ���ն��ܽ��յ�֡��󳤶�
    
    volatile	uchar aucBuffer[272];   // �շ�������
    volatile	uchar ucPN512Cmd;      // transmit, idle, transceive��������
    
            // added by liuxl 20070928 ���ڼ�¼ѯ��ʱ���ֵĴ���λ�ã��Ա㶨λ����ԭ��
    volatile	ushort usErrNo; 
    volatile	uchar  ucInProtocol;    // ��Ƭ����ISO14443Э��̬��־(��Ҫ�����ж�PRO_S50/PRO_S70�ǽ���M1�������ǽ���TYPE A����)
    volatile	uchar  ucCollPos;       // ��ͻλ��
    
    volatile	uchar  ucCmd;
    volatile	uchar  ucHalt;
    volatile	uchar  ucRemove_Step;
    volatile	uchar  ucStatus;
    volatile	uchar  ucRemoved;
    volatile	uchar  ucM1Stage;
            
    volatile	uchar ucPollMode; // added by liuxl����������emv��paypass 

    //����Ϊfelica������س�Ա
    volatile	uchar f_ispoll;//��ʾ�Ƿ�Ϊpolling����
}RF_WORKSTRUCT;

typedef RF_WORKSTRUCT PN512_RF_WORKSTRUCT;//����һ����


typedef struct
{
	int a_conduct_w;  	//A�Ϳ�����絼д������1--��������ֵ��������
	int a_conduct_val;  // A�Ϳ�����絼���Ʊ�������Ч��Χ0~63,����ʱ��Ϊ63
	
	int m_conduct_w;  	//M�Ϳ�����絼д������1--��������ֵ��������
	int m_conduct_val;  // M�Ϳ�����絼���Ʊ�������Ч��Χ0~63,����ʱ��Ϊ63
	
	int b_modulate_w;  
	int b_modulate_val;  // B�Ϳ�����ָ�����Ʊ�������Ч��Χ0~63,����ʱ��Ϊ63
	
	int card_RxThreshold_w;   // ����������д������1����������ֵ��������*/
	int card_RxThreshold_val;  // ���������ȣ���Ч��Χ0��255��Ĭ��ֵ�������� */
	
	int f_modulate_w;   
	int f_modulate_val;  //FeliCa������ָ�����Ʊ�������Ч��Χ0~63,����ʱ��Ϊ63
	
	int card_buffer_w;    //��Ƭ���ջ�������Сд������1--��������ֵ��������
	int card_buffer_val;   //��Ƭ���ջ�������С��������λ���ֽڣ�����Чֵ1~256��                       
	                   
	int card_type_check_w;   // ��Ƭ���ͼ��д��������ֵ���ɶ�
	int card_type_check_val;  // 0-��鿨Ƭ���ͣ�����������鿨Ƭ����(Ĭ��Ϊ��鿨  

	uchar wait_retry_limit_w;//added in V1.00F,20071212
	ushort wait_retry_limit_val;//max retry count for WTX block requests,default 3
        
	int Reserved[20];       //�����ֽڣ����ڽ�����չ��д��ʱӦȫ����
}ST_PICC_PARA;

#define PICC_PARA  ST_PICC_PARA//��������ǰ����һ��


	
typedef struct
{
   uchar   	    Command[ 4 ]; // CLA INS  P1 P2
   ushort  	    Lc;           // P3
   uchar   	    DataIn[512];
   ushort  	    Le;
} APDU_SEND;

typedef struct
{
   ushort       LenOut;    // length of dataout 
   uchar 	    DataOut[512];
   uchar        SWA;
   uchar        SWB;
} APDU_RESP;



#if (TERM_TYPE==TERM_KF900)
 
#include <asm/uaccess.h>
#include<asm/arch/irqs.h>//�жϺ�
#include<asm/mach/irq.h>//�жϽṹ�嶨��
#include <asm/arch/io.h>//�����ַӳ�䷽ʽ
//#include<asm/arch/regs-gpio.h>//gpio�Ĵ�����ʵ��ַת��
//#include<asm/arch/regs-irq.h>//interrupt�Ĵ�����ʵ��ַת��
//#include<asm/arch/regs-clock.h>

#define SPI0_BASE       0xFFFEE000
#define SPI1_BASE       0xFFFEF000

#define GPIO0_BASE      0xFFFF5000
#define GPIO1_BASE      0xFFFF6000
#define GPIO2_BASE      0xFFFF7000

#define PMU_BASE        0xFFFFE000

//FOR SPI

#define rSPI1_DAT_REG	(*(volatile unsigned *) IO_ADDRESS(SPI1_BASE+0X000))
#define rSPI1_CTL_REG	(*(volatile unsigned *) IO_ADDRESS(SPI1_BASE+0X004))
#define rSPI1_STA_REG	(*(volatile unsigned *) IO_ADDRESS(SPI1_BASE+0X008))
#define rSPI1_MOD_REG	(*(volatile unsigned *) IO_ADDRESS(SPI1_BASE+0X00C))
#define rSPI1_DIAG_REG	(*(volatile unsigned *) IO_ADDRESS(SPI1_BASE+0X010))
#define rSPI1_BRG_REG	(*(volatile unsigned *) IO_ADDRESS(SPI1_BASE+0X014))
#define rSPI1_DMA_REG	(*(volatile unsigned *) IO_ADDRESS(SPI1_BASE+0X018))

//FOR INTERRUPT
#define rGPIO2_EN		(*(volatile unsigned *) IO_ADDRESS(GPIO2_BASE+0X000))//1-GPIO FUN 0-OTHER FUN
#define rGPIO2_OE		(*(volatile unsigned *) IO_ADDRESS(GPIO2_BASE+0X00C))//1-OUTPUT EN 0-OUTPUT DISABLE
#define rGPIO2_OUT		(*(volatile unsigned *) IO_ADDRESS(GPIO2_BASE+0X018))//1-SET 1 0-SET 0
#define rGPIO2_IN		(*(volatile unsigned *) IO_ADDRESS(GPIO2_BASE+0X024))//0-READ 0 1-READ 1
#define rGPIO2_IMOD		(*(volatile unsigned *) IO_ADDRESS(GPIO2_BASE+0X028))//0-LEVEL INT 1-EDGE INT
#define rGPIO2_IPOL		(*(volatile unsigned *) IO_ADDRESS(GPIO2_BASE+0X02C))//0-FALLING 1-RISING
#define rGPIO2_ISEL		(*(volatile unsigned *) IO_ADDRESS(GPIO2_BASE+0X030))//0-USE CHANNEL A INT 1-USE CHANNEL B INT
#define rGPIO2_IEN		(*(volatile unsigned *) IO_ADDRESS(GPIO2_BASE+0X034))//0-DISABLE INT 1-ENABLE INT
#define rGPIO2_IAST		(*(volatile unsigned *) IO_ADDRESS(GPIO2_BASE+0X040))//0- NO A INT 1-HAVE A INT
#define rGPIO2_IBST		(*(volatile unsigned *) IO_ADDRESS(GPIO2_BASE+0X044))//0- NO B INT 1-HAVE B INT
#define rGPIO2_ICLR		(*(volatile unsigned *) IO_ADDRESS(GPIO2_BASE+0X048))//1-CLEAN INT
#define rGPIO2_WKEN		(*(volatile unsigned *) IO_ADDRESS(GPIO2_BASE+0X04C))//0-PMU WAKE DISABLE 1-PMU WAKE ENABLE




#define rGPIO1_EN		(*(volatile unsigned *) IO_ADDRESS(GPIO1_BASE+0X000))//1-GPIO FUN 0-OTHER FUN
#define rGPIO1_OE		(*(volatile unsigned *) IO_ADDRESS(GPIO1_BASE+0X00C))//1-OUTPUT EN 0-OUTPUT DISABLE
#define rGPIO1_OUT		(*(volatile unsigned *) IO_ADDRESS(GPIO1_BASE+0X018))//1-SET 1 0-SET 0


//FOR PMU CLK
#define rPMU_CKEN_REG	(*(volatile unsigned *) IO_ADDRESS(PMU_BASE+0X008))//CLK ENABLE
#define	PMU_SPI1_CKEN	(0X1<<15)

#define BIT_EINT1      (0x1<<1)
#define BIT_EINT3      (0x1<<3)

#endif

#if (TERM_TYPE==TERM_KF322)

#include "stm32f10x_it.h"
#endif

#ifndef PICC_LED_RED
#define PICC_LED_RED    0x01  //���
#endif
#ifndef PICC_LED_GREEN
#define PICC_LED_GREEN  0x02//�̵�
#endif
#ifndef PICC_LED_YELLOW
#define PICC_LED_YELLOW 0x04 //�Ƶ�
#endif
#ifndef PICC_LED_BLUE
#define PICC_LED_BLUE   0x08//����
#endif


// �Ĵ���λ����
#ifndef BIT0
 #define BIT0    ((uint)1<<0)
#endif
#ifndef BIT1
 #define BIT1    ((uint)1<<1)
#endif
#ifndef BIT2
 #define BIT2    ((uint)1<<2)
#endif
#ifndef BIT3
 #define BIT3    ((uint)1<<3)
#endif
#ifndef BIT4
 #define BIT4    ((uint)1<<4)
#endif
#ifndef BIT5
 #define BIT5    ((uint)1<<5)
#endif
#ifndef BIT6
 #define BIT6    ((uint)1<<6)
#endif
#ifndef BIT7
 #define BIT7    ((uint)1<<7)
#endif
#ifndef BIT8
 #define BIT8   ((uint)1<<8)
#endif
#ifndef BIT9
 #define BIT9   ((uint)1<<9)
#endif
#ifndef BIT10
 #define BIT10    ((uint)1<<10)
#endif
#ifndef BIT11
 #define BIT11    ((uint)1<<11)
#endif
#ifndef BIT12
 #define BIT12    ((uint)1<<12)
#endif
#ifndef BIT13
 #define BIT13    ((uint)1<<13)
#endif
#ifndef BIT14
 #define BIT14    ((uint)1<<14)
#endif
#ifndef BIT15
 #define BIT15    ((uint)1<<15)
#endif
#ifndef BIT16
 #define BIT16    ((uint)1<<16)
#endif
#ifndef BIT17
 #define BIT17    ((uint)1<<17)
#endif
#ifndef BIT18
 #define BIT18   ((uint)1<<18)
#endif
#ifndef BIT19
 #define BIT19   ((uint)1<<19)
#endif
#ifndef BIT20
 #define BIT20    ((uint)1<<20)
#endif
#ifndef BIT21
 #define BIT21    ((uint)1<<21)
#endif
#ifndef BIT22
 #define BIT22    ((uint)1<<22)
#endif
#ifndef BIT23
 #define BIT23    ((uint)1<<23)
#endif
#ifndef BIT24
 #define BIT24    ((uint)1<<24)
#endif
#ifndef BIT25
 #define BIT25    ((uint)1<<25)
#endif
#ifndef BIT26
 #define BIT26    ((uint)1<<26)
#endif
#ifndef BIT27
 #define BIT27    ((uint)1<<27)
#endif
#ifndef BIT28
 #define BIT28   ((uint)1<<28)
#endif
#ifndef BIT29
 #define BIT29   ((uint)1<<29)
#endif
#ifndef BIT30
 #define BIT30   ((uint)1<<30)
#endif
#ifndef BIT31
 #define BIT31   ((uint)1<<31)
#endif
#ifndef BIT32
 #define BIT32   ((uint)1<<32)
#endif

#ifndef NULL
  #define NULL   ((void*)0)
#endif


#include "MifreHal.h"
#include "MifreCmd.h"
#include "Mifre.h"


#if (TERM_TYPE == TERM_KF322)
//#define	RFINT	BIT0
#define	RFINT	BIT1
#define RFRESET BIT0

#define  DelayMs     delay_ms
#define  picc_open PiccOpen
#define  picc_close PiccClose
#define  picc_detect PiccDetect
#define  picc_isocommand PiccIsoCommand
#define  piccremove PiccRemove
#define  picc_setup PiccSetup
#define  picc_cmdexchange PiccCmdExchange

#endif

#endif





