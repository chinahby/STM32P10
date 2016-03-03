#ifndef __KF701DH_H__
#define __KF701DH_H__

//#include "w78e58.h"
//#include <absacc.h>
//#include <intrins.h>
#include <string.h>
#include <stdio.h>
#include "Display.h"
#include "calendar.h"

#define uchar unsigned char
#define uint  unsigned int
#define ulong unsigned long

//��ʱ����ֵ
#define	TIMER0H	  0xee		/* ��ʱ��0ʱ��,5ms */
#define	TIMER0L	  0x1
#define	TIMER2H	  0xee		/* ��ʱ��0ʱ��,5ms */
#define	TIMER2L	  0x1

#define	 SEND_WT_TIME	  5000 //25S

//Ӧ������
#define OK                       1       //������ִ�е�Ӧ��
#define NP                       2       //������һ�����ݵ�Ӧ��
#define WT                       3       //�����ٵȴ�30s��Ӧ��
#define ER                       4       //������ִ�е�Ӧ��
//extern uchar power_down_flag(void);

/***************************************
�������ܣ��ϵ����ɳ�ʼ��ϵͳ,���������²���
           1:   �رյƺͷ�����
           2:   �ر�id������Դ,��sam������Դ,�رտ���ʱ���ź�                
           3:   ��ʼ������,������9600,�򿪴����ж�
           4;   �ر������ж�, 
           5��  �رպ󱸵�Դ    

����  0: ��ʼ���ɹ�  !=0 ��ʼ��ʧ��
***************************************/
extern uchar initial_system(void);

/**************************************************************************
�������ܣ��򿪺󱸵�Դ                         
 ��ڲ�������  
 ���ڲ�������                                                 
**************************************************************************/
extern void bat_on(void);

/**************************************************************************
�������ܣ��رպ󱸵�Դ                         
 ��ڲ�������  
 ���ڲ�������                                                 
**************************************************************************/
extern void bat_off(void);

/*  ---------------------  sram  ------------------------*/
/***************************************************************************/
/* �������ܣ���SRAM��leng���ȵ����ݶ�����������                            */
/* ��ڲ�����addrΪ�����ݵ���ʼ��ַ,lengΪ���ݳ��ȣ�*bufΪ������ݵ�ָ��   */
/* ���ڲ���������ֵ0 ��ȷ                                                  */
/*                 1 ����                                                  */
/***************************************************************************/
extern uchar read_sramb(uint addr,uint leng,void *buf);
/***************************************************************************/
/* �������ܣ���leng���ȵ�����д��SRAM��                                    */
/* ��ڲ�����addrΪд�����ݵ���ʼ��ַ,lengΪ���ݳ��ȣ�*bufΪ������ݵ�ָ�� */
/* ���ڲ���������ֵ0 ��ȷ                                                  */
/*                 1 ����                                                  */
/***************************************************************************/
extern uchar write_sramb(uint addr,uint leng,void *buf);
/*  ---------------------  flash (S29GL064)------------------------*/
/***************************************
�������ܣ�����������flash
������ڣ�4M�ֽڵ�flash����64������,ÿ������64K��sectorΪ0��63
          8M�ֽڵ�flash����128������,ÿ������64K��sectorΪ0��127
          32M�ֽڵ�flash����512������,ÿ������64K��sectorΪ0��511
�������ڣ�0����ȷ      1������
***************************************/
extern uchar flash_sector_erase(uint sector);

/***************************************
�������ܣ���Ƭ����flash
����ֵ �� 0��������ȷ 1������ʧ��
***************************************/
extern uchar flash_chip_erase(void);

/***************************************
�������ܣ���wr_buf��������wr_length���ȵ�����д��flashָ����Ԫ
������ڣ�wr_addr:  д������ʼ��ַ(0---0X3FFFFF)  4M�ռ� 
                    д������ʼ��ַ(0---0X7FFFFF)  8M�ռ�
                    д������ʼ��ַ(0---0X1FFFFFF)  32M�ռ�
		 wr_length:д���ֽ���
		 wr_buf:   д�������ݻ����� 
����ֵ �� 0��д����ȷ 1��д��ʧ��
***************************************/
extern uchar write_flash(ulong wr_addr,uint wr_length,  void *wr_buf);

/***************************************
�������ܣ���FLASH��rd_length���ȵ����ݶ���rd_buf��������
������ڣ�rd_addr:  ��������ʼ��ַ (0---0X3FFFFF)  4M�ռ� 
                    ��������ʼ��ַ (0---0X7FFFFF)  8M�ռ�   
                    ��������ʼ��ַ (0---0X1FFFFFF)  32M�ռ�
		 rd_length:�����ֽ���
		 rd_buf:   ���������ݻ����� 
����ֵ �� 0����������ȷ 1��������ʧ��,��ַ���
***************************************/
extern uchar read_flash(ulong rd_addr,uint rd_leng, void *rd_buf);

/*  ---------------------      eeprom      ------------------------*/
//1��24c64,��8k�ֽڴ洢����
/***************************************
�������ܣ���eeprom
������ڣ�rd_addr:  ��������ʼ��ַ(0--0x1FFF);
		 rd_length:�����ֽ���
		 rd_buf:   ���������ݻ����� 
����ֵ �� 0����������ȷ 1��������ʧ��
***************************************/
extern uchar read_eeprom(uint rd_addr,uint rd_length,void *rd_buf);

/***************************************
�������ܣ���eepromĳ��ַ��ʼдwr_length��������
������ڣ�wr_addr:  д������ʼ��ַ(0--0x1FFF);
		  wr_length:д���ֽ���
		  wr_buf:   д�������ݻ����� 
����ֵ �� 0��д����ȷ   1��д��ʧ��
***************************************/
extern uchar write_eeprom(uint wr_addr,uint wr_length,void *wr_buf);

/*  ---------------------     ʱ�Ӻ���     ------------------------*/
/***********************
�������ܣ�����ʱ��
������ڣ�rtc_buf:����ʱ�����ݻ�����
			rtc_buf�е����ݶ������£� 

			rtc_buf[0]=���λ 
			rtc_buf[1]=�� 
			rtc_buf[2]=�� 
			rtc_buf[3]=ʱ 
			rtc_buf[4]=�� 
			rtc_buf[5]=�� 
			���ݲ���ѹ��BCD���ʽ
����ֵ��0:���óɹ�  1������ʧ��,ʱ�����ݸ�ʽ���� 
***********************/
extern uchar set_rtc(uchar *rtc_buf);	


/***********************
�������ܣ�������ʱ��
������ڣ�rtc_buf:���������ݻ�����
			rtc_buf�е����ݴ�����£� 

			rtc_buf[0]=���λ
			rtc_buf[1]=�� 
			rtc_buf[2]=�� 
			rtc_buf[3]=ʱ 
			rtc_buf[4]=�� 
			rtc_buf[5]=�� 
			���ݲ���ѹ��BCD���ʽ
***********************/
extern void read_rtc(uchar *rtc_buf);

/********************************************
��������:��֤rtc_buf��ָ�������/ʱ�����ݵĺϷ���   
���:   rtc_buf:�������ʱ����׵�ַ
            rtc_buf�е����ݶ������£� 

        	rtc_buf[0]=��
        	rtc_buf[1]=�� 
        	rtc_buf[2]=�� 
        	rtc_buf[3]=ʱ 
        	rtc_buf[4]=�� 
        	rtc_buf[5]=�� 
        
            ���ݲ���ѹ��BCD���ʽ�� 
���ڣ���                                   
����ֵ: ���ݺϷ�����0�����򷵻�1 
*********************************************/
extern uchar check_rtc(uchar *rtc_buf);

/*--------------------        ����ͨѶ         ---------------------------*/
/*----------------------------------------------------
��������:�޸Ĳ�����
��ڲ�����rate     ��Ӧ������
            1       300
            2       600
            3       1200
            4       2400
            5       4800
            6       9600
            7       14400
            8       19200
            9       33600
            10      57600

���ڲ�����0=�ɹ�   !0=��ڲ�������

�޸���:  
�޸�ʱ�䣺
-----------------------------------------------------*/
extern uchar modify_rate(uchar rate);

/***************************
��������:�Բ�ѯ��ʽ,����һ���ֽ�(8λ����,1����ʼһ��ֹͣ,��У��)
���:��
����:buf �յ��������׵�ַ
����ֵ: 
        == 0 ��ȷ �յ�����,
        != 0 ��ʱ(TIMEO)
****************************/		
extern uchar rece(uint length,uchar *buffer);
/********************************
��������:�Բ�ѯ��ʽ,����һ���ֽ�(8λ����,1����ʼһ��ֹͣ,��У��),
���:    c ���͵��ַ�
����:   
����ֵ: ��
*********************************/		
extern void send_str(uint length,uchar *buffer);

/**
��������:������ڻ����� 
*/
extern void clear_rece_buffer(void);

/*------------------------ ���������� ---------------------------------*/

/***************************************
�������ܣ�����Ƿ��п�����
����ֵ  ��0���п�   1���޿�
***************************************/
extern uchar inserted_card(void);

/***************************************
�������ܣ�С�����ϵ�
***************************************/
extern void sam_pwr_on(void);

/***************************************
�������ܣ�С�����µ�
***************************************/
extern void sam_pwr_off(void);

/***************************************
�������ܣ������ϵ�
***************************************/
extern void id_pwr_on(void);

/***************************************
�������ܣ������µ�
***************************************/
extern void id_pwr_off(void);

/***************************************
�������ܣ������ϵ�cpu����λӦ��(�����ϵ�)
������ڣ���
�������ڣ�����0����λ����  ���򣺸�λ�쳣
		 buf����Ÿ�λ��Ϣ���׵�ַ�����ݸ�ʽΪ��ATR���ȡ�ATR1��Ϣ��ATR2��Ϣ......		
***************************************/
extern uchar atr_id(uchar *buf);

/***************************************
����������������ϵ�CPU������ͨѶ
��ڲ���: 
	ptr:�������ݻ���������ʽΪ��60H,LEN,ISO_MODE,CLS,INS,P1,P2,P3,data_1,...,data_n,le
	60H --����ͷ
	LEN	--ISO_MODE,CLS,INS,P1,P2,P3,data_1,...,data_n�����ֽ���
	ISO_MODE--0DAH,ISO_IN������д��/У����
			--0DBH,ISO_OUT����������
			--��������Ϊ�Ƿ�
	ISO_IN :60H,LEN,ISO_MODE,CLS,INS,P1,P2,Lc,data_1,...,data_n
	ISO_OUT:60H,LEN,ISO_MODE,CLS,INS,P1,P2,Le
	        le���Ϊ240
���ڲ���:  ����0�������ɹ�  �������ʧ��	
	ptr��������ݻ����������ʽΪ��60H,LEN,ISO_MODE,data_1,...,data_n,SW1,SW2
	LEN	--data_1,...,data_n���ֽ���
	ISO_IN:	60H,LEN,ISO_MODE,SW1,SW2
	ISO_OUT:60H,LEN,ISO_MODE,data_1,...,data_n,SW1,SW2
	����ISO_IN����ISO_OUT,ptr���������ᱻ��д,
***************************************/
extern uchar id_iso(uchar *ptr);

/***************************************
�������ܣ�SAM�����ϵ�CPU����λ
������ڣ���
�������ڣ�����0����λ����   ���򣺸�λ�쳣
		 buf����Ÿ�λ��Ϣ���׵�ַ�����ݸ�ʽΪ��ATR���ȡ�ATR1��Ϣ��ATR2��Ϣ......
***************************************/
extern uchar atr_sam(uchar *buf);

/***************************************
������������С�����ϵ�CPU������ͨѶ
��ڲ���: 
	ptr:�������ݻ���������ʽΪ��60H,LEN,ISO_MODE,CLS,INS,P1,P2,P3,data_1,...,data_n
	60H --����ͷ
	LEN	--ISO_MODE,CLS,INS,P1,P2,P3,data_1,...,data_n�����ֽ���
	ISO_MODE--0DAH,ISO_IN������д��/У����
			--0DBH,ISO_OUT����������
			--��������Ϊ�Ƿ�
	ISO_IN :60H,LEN,ISO_MODE,CLS,INS,P1,P2,Lc,data_1,...,data_n
	ISO_OUT:60H,LEN,ISO_MODE,CLS,INS,P1,P2,Le
	        le���Ϊ240
���ڲ���:  ����0�������ɹ�  �������ʧ��	
	ptr��������ݻ����������ʽΪ��60H,LEN,ISO_MODE,data_1,...,data_n,SW1,SW2
	LEN	--data_1,...,data_n���ֽ���
	ISO_IN:	60H,LEN,ISO_MODE,SW1,SW2
	ISO_OUT:60H,LEN,ISO_MODE,data_1,...,data_n,SW1,SW2
***************************************/
extern uchar sam_iso(uchar *ptr);


//   flash card
/**********************************************
��������:AT45D041 RESET
***********************************************/
extern void at45d041_reset(void);

/*********************************
�������ܣ�дflash��
��ڲ�����  ADDR:��ʼ�ĵ�ַ (0---0x83fff)
            LENGTH:����     (1---264)
            BUF:���ݵ���ʼ��ַ
����ֵ��  д��ɹ�0������1: ��ڲ�������(length>264����length==0)
*********************************/
extern uchar f45d041_writecard(ulong addr,uint length,uchar *buf);

/************************************
��������:�ӿ��ж�length bytes��������
�������:addr(F45D041��:0..0x82fff):����ַ;
		lenght(0..263):����;
		buf:������;

���û��ͣ�
�޸�ʱ��:2002.4.12
****************************/
extern uchar f45d041_readcard(ulong addr,uint length,uchar *buf);

//   4442 card
/* ��4442����address��Ԫ��ʼ��length�����ݣ�д��buffer */
//extern void read_sle42(uchar address,uchar length,uchar *buffer);

 /* 4442���ĸ�λӦ�� */
//extern void initial_sle44xx(uchar *buffer);

/*-----------------------------------��������---------------------------------*/
/***************************************
�������ܣ��򿪷�����
***************************************/
extern void buz_on(void);		
extern void Buzzer_Ring(u16 Fre);
extern void Buzzer_Off(void);
/***************************************
�������ܣ��رշ�����
***************************************/
extern void buz_off(void);	

/***************************************
�������ܣ���ָʾ����
***************************************/
extern void ledgreen_on(void);		

/***************************************
�������ܣ���ָʾ����
***************************************/
extern void ledgreen_off(void);		

/***************************************
�������ܣ���ָʾ����
***************************************/
extern void ledorange_on(void);		

/***************************************
�������ܣ���ָʾ����
***************************************/
extern void ledorange_off(void);	

/***************************************
�������ܣ���ָʾ����
***************************************/
extern void ledred_on(void);		

/***************************************
�������ܣ���ָʾ����
***************************************/
extern void ledred_off(void);	

extern void ledall_on(void);
extern void ledall_off(void);
extern void LcdBL_Control(u8 Control);
extern void LcdReset(void);
extern void LcdWriteCmd(u8 Command);
extern u8 LcdReadStatus(void);
extern void LcdWriteData(u8 DisplayData);
extern u8 LcdReadData(void);
extern void LcdDotTest(void);
//extern u8 Char2Dot8X8(u8 *pCharSrc,u8 *pDotSrc);
extern void Lcdarray_On(u8 LineX,u8 LineY,u8 ByteLength, u8 *NewLcdData);
extern void Lcdstring_On(u8 LineX,u8 LineY,u8 *NewLcdStr);
extern void Lcdarray_clear(u8 LineBit);

extern void ChangeDate(u8 *);
extern void ChangeTime(u8 *);
extern u8 TimeGet(u8 *);
extern void DateGet(u8 *);

extern void RF1356_Init(void);
extern u8 PN512RegsWrite(u8 *pData);
extern u8 PN512RegsRead(u8 *pData);
extern u8 RF1356Reset(u8 *Infor);
extern u8 RF1356Start(u8 *Infor);
extern u8 RF1356StartTypeA(u8 *Infor);
extern u8 RF1356PollTypeA(u8 *Infor);

//extern void WeekDayNum(u32 nyear, u8 nmonth, u8 nday);
/*********************************
��������:��ʱʱ�� t ms
*********************************/
extern	void delay_ms(ulong t);

/*********************************************
�������ܣ� CRCУ���㷨 (���������㷨)                   
��ڲ����� length �μ��������ݳ���        
            clear==1 CRC��ֵ���㿪ʼ����,��������buffer�����ݵ�crcУ��ֵ
            clear==0 �ڱ���CRC��ֵ�����ϼ���buffer�����ݵ�crcУ��ֵ
            buffer ���ݵ�ַָ��			 
����ֵ�� CRCУ����                        
*********************************************/
extern uint crc_16(uint length,uchar clear,void *buffer);
//�����㷨
extern uint crcxdata_old(uint length,uchar clear,void *buffer);

/*-------------------------------------------------------------------------------
 Funtion: ��ѹ��Bcdת��ΪBin 
 Input  : TUlBinValue:BIN��ֵ  
	  TUcCompBcdLen:ѹ��bcd���ֽ��� 
	  TUcCompBcdBuf: ѹ����bcd�� 
 excemple:ʮ��������12h,34h,56h ת��Ϊ0x1e240h
 Editor : NingNeng
 Date   : 2002-11-19 
----------------------------------------------------------------------------*/
extern void bcd_to_bin(ulong *TUlBinValue,uchar TUcCompBcdLen,uchar *TUcCompBcdBuf);
/*-------------------------------------------------------------------------------
 Funtion: ��BINת��Ϊѹ��BCD 
 Input  : TUlBinValue:BIN��ֵ 
	  TUcCompBcdLen:ѹ�����bcd���ֽ���, 
	  TUcCompBcdBuf: ѹ�����bcd�� 
 excemple:ʮ��������123456 ת��Ϊ12H��34H��56H������
----------------------------------------------------------------------------*/
extern void bin_to_bcd(ulong TUlBinValue,uchar TUcCompBcdLen,uchar *TUcCompBcdBuf);

/******************
����    ����������س���78E516) 
*****************/
//extern void  prg_load();
//================================================
// �� �� ��: calc_next_day
// ��������: �ж��������ڵĺϷ��Բ�������һ�յ�����
// ȫ�ֱ���: ��
// �������: date Ҫ���������(CCYYMMDD)
// �������: ��
// �� �� ֵ: ��ָ�ַ�����ͷ4�ֽھ�Ϊ0xFF �������ڴ���;
//           ==��������һ�յ�����(CCYYMMDD)
// ��    ��: �ܽ���
// ��������: 2003-06-XX
// �� �� ��: �ܽ���
// �޸�����: 2003-06-XX
//================================================
extern uchar * calc_next_day(uchar *date);
extern uchar * calc_last_day(uchar *date);
/**********************************************
����:   ָ����Գ��� 
����ֵ�����Գɹ�0������!0
*************************************************/
extern uchar test_ins(void);

extern void dtr_on(void);

extern void dtr_off(void);

//this interrupt will defaultly be called during power fall interrupt
extern void IT_PowerOff(void); 

//this API is used when app determine to power off the tax box itself
//any code behind this API will never be involked because of power down
void TaxBox_PowerOff();
extern void usb_com_mode(void);

extern void uart_com_mode(void);

//this interrupt will defaultly be called every 5 mS
extern void IT_SendWait(void);
extern void System_SoftReset(void);
u8 UARTCOM_RecvCmd(u8 *Data);
void UARTCOM_SendString(u8 *Data, u16 DataLength);

//for big end and little end feature
///���һ��32bits��u32 ����Ϊ�����ˣ����ص�ǰƽ̨��u32
u32 GetU32(void* value);
///����һ��32bits��u32 ����Ϊ�����˵� ������ ��һ������ƽ̨��u32��
void SetU32(void* value, u32 fact);
///����һ��32bits��u32 ����Ϊ������
void IncU32(void* value);
///�ӷ�һ��32bits��u32 ����Ϊ�����˵� ������ ��һ������ƽ̨�ļ���
void AddU32(void* value, u32 fact);
///���һ��16bits��u16 ����Ϊ�����ˣ����ص�ǰƽ̨��u16
u16 GetU16(void* value);
///����һ��16bits��u16 ����Ϊ�����˵� ������ ��һ������ƽ̨��u16��
void SetU16(void* value, u16 fact);
///����һ��16bits��u16 ����Ϊ������
void IncU16(void* value);
///�ӷ�һ��16bits��u16 ����Ϊ�����˵� ������ ��һ������ƽ̨��u16����
void AddU16(void* value, u16 fact);
extern unsigned char USB_Wait_InsertDisk();
//USB_Wait_InsertDisk();

///Read a file from given offset
extern unsigned char USB_ReadFile(unsigned int uiOffset,unsigned char *pucData,unsigned int uiLen);
//USB_ReadFile("\\NEWFILE.TXT",5,temp,10);


///write a file from given offset
extern unsigned char USB_WriteFile(unsigned int uiOffset,unsigned char *pucData,unsigned int uiLen, unsigned char ucWriteMode);
//USB_WriteFile("\\NEWFILE.TXT",1,"ab",2);


///append a file from the end of current file
extern unsigned char USB_AppendFile(unsigned char *pucFileName,unsigned char *pucData,unsigned int uiLen);
//USB_AppendFile("\\NEWFILE.TXT","zzzzzzzzzz",10);


///inquire the file size of given file name (temp file handler)
extern unsigned char USB_FileSize(unsigned char *pFilename,unsigned int *piSize);



///erase the file with given file name (temp file handler)
extern unsigned char USB_EraseFile(unsigned char *pucFileName);
extern unsigned char USB_CreateFile(unsigned char *pucFileName);
extern unsigned char USB_OpenFile(unsigned char *pucFileName);
extern unsigned char USB_CloseFile(void);

/***************************************
�������ܣ�����Դ����
***************************************/
extern void power_on(void);


/***************************************
�������ܣ��ص�Դ����
***************************************/
extern void power_off(void);

extern void WriteToBackupReg(u16 index ,u16 FirstBackupData);
u32 CheckBackupReg(u16 index,u16 FirstBackupData);

#endif
