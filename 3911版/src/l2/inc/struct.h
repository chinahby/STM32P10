/*******************************************************************************
* File Name			 : struct.h
* Author             : guohonglv
* Date First Issued  : 2010/11/12
* Description        : ��ֲʱ�õ��Ľṹ�壨��΢�е��ң�û�з��ࣩ
********************************************************************************/

#ifndef STRUCT_H
#define STRUCT_H


#define  TRACK2_LEN        100 
#define  TRACK3_LEN        107

//typedef unsigned char  u8;
#define uchar unsigned char
#define u8  unsigned char
#include <time.h>

/* //time.h �����������Ѿ��ж��壬����ע�͵�
struct tm 
{ 
	int tm_sec; 
	int tm_min; 
	int tm_hour; 
	int tm_mday; 
	int tm_mon; 
	int tm_year; 
	int tm_wday; 
	int tm_yday; 
	int tm_isdst; 
}; 


typedef struct
{
  u8 sec_l;
  u8 sec_h;
  u8 min_l;
  u8 min_h;
  u8 hour_l;
  u8 hour_h;
}time_t;
*/

/*
struct ClssProcInter 
{
	uchar aucAID[17];       
	uchar ucAidLen; 
	
	// payWave
	uchar   ucZeroAmtFlg;       		// 0-���׽��!=0; 1-���׽��=0
	uchar   ucStatusCheckFlg;   		// �������Ƿ�֧��״̬���
    uchar   aucReaderTTQ[5];      // �ն˽������ܣ�����VISA/PBOC�У�tag =9F66
	uchar   ucCLAppNotAllowed; 			// 1-��AID���ܽ��зǽӴ�����
	
	// common
	uchar ucTermFLmtExceed; 
	uchar ucRdCLTxnLmtExceed; 
	uchar ucRdCVMLmtExceed;  
	uchar ucRdCLFLmtExceed;  
	
	uchar ucTermFLmtFlg;
	uchar aucTermFLmt[4];
	
	uchar aucRFU[2];
}; 
extern struct ClssProcInter ClssProcInter;  //guohonglv //�������Է�ֹ�������ظ�����


struct ClssEc
{
	unsigned long  ulAmntAuth;     // ��Ȩ���(ulong)
    unsigned long  ulAmntOther;    // �������(ulong) 
    unsigned long  ulTransNo;      // �������м�����(4 BYTE)
	uchar  ucTransType;      // �������� not  '9C' !!! (GOODS CASH)
	uchar  aucTransDate[4];  // �������� YYMMDD
	uchar  aucTransTime[4];  // ����ʱ�� HHMMSS
};  
extern struct ClssEc ClssEc; //guohonglv //�������Է�ֹ�������ظ�����

struct gtPosParamet 
{
	uchar ucCommWaitTime; 	//ͨѶ�ȴ�ʱ��
	uchar szUnitNum[16];
	uchar szPosId[9];
	uchar szUnitChnName[41];
	int  iMaxTransTotal;  	//��ˮ���������ױ���������
	uchar ucEmvSupport;
	uchar ucSelectCardType;	// PARAM_OPEN: oper select card type, PARAM_CLOSE: auto-select
	long lNowTraceNo; 		//��ǰ�ն˽�����ˮ��
	long lNowBatchNum;		//��ǰ���κ� 
	uchar ucOprtLimitTime;  // ������ʱʱ�� 30S-60S
};
extern struct gtPosParamet gtPosParamet;  


struct PiccPara 
{
	unsigned char drv_ver[5];  		//��������İ汾��Ϣ���磺��1.01A����ֻ�ܶ�ȡ��д����Ч
	unsigned char drv_date[12];  	// ���������������Ϣ���磺��2006.08.25���� ֻ�ܶ�ȡ
	unsigned char a_conduct_w;  	//A�Ϳ�����絼д������1--��������ֵ����������ֵ���ɶ�
	unsigned char a_conduct_val;	// A�Ϳ�����絼���Ʊ�������Ч��Χ0~63,����ʱ��Ϊ63
	//ȱʡֵΪ63��a_conduct_w=1ʱ����Ч�������ֵ�������ڲ������ı����ڵ�������A�Ϳ���������ʣ��ɴ��ܵ���������Ӧ����
	unsigned char b_modulate_w;
	unsigned char b_modulate_val;  // B�Ϳ�����ָ�����Ʊ�������Ч��Χ0~63,����ʱ��Ϊ63
	//;b_modulate_w=1ʱ����Ч�������ֵ�������ڲ������ı����ڵ�������B�Ϳ��ĵ���ָ�����ɴ��ܵ���������Ӧ����
	unsigned char card_buffer_w;	//��Ƭ���ջ�������Сд������1--��������ֵ��������
	unsigned short card_buffer_val;	//��Ƭ���ջ�������С��������λ���ֽڣ�����Чֵ1~256��
	//����256ʱ������256д�룻��Ϊ0ʱ��������д�롣
	//��Ƭ���ջ�������Сֱ�Ӿ������ն���Ƭ����һ�����
	//ʱ�Ƿ��������ְ����͡��Լ������ְ���������С������ 
	//���͵�������ڿ�Ƭ���ջ�������С�����轫���и��С
	//����������η��͡���Picc_detect( )����ִ�й����У���Ƭ
	//���ջ�������С֮�����ɿ�Ƭ������նˣ�һ��������Ĵ�
	//ֵ�������ڷǱ�׼����������Ҫ����˲���ֵ���Ա�֤���� ��
	//Ч���С�
	unsigned char wait_retry_limit_w;	// S(WTX)��Ӧ���ʹ���д������1����������ֵ������ ��
	unsigned short wait_retry_limit_val;	// S(WTX)��Ӧ���ʹ������ֵ��Ĭ��Ϊ3��
	unsigned char card_type_check_w;   	// ��Ƭ���ͼ��д��������ֵ���ɶ�
	unsigned char card_type_check_val;  	// 0-��鿨Ƭ���ͣ�����������鿨Ƭ����(Ĭ��Ϊ��鿨  
	//Ƭ����)
	unsigned char card_RxThreshold_w;   // card RxThreshold
	unsigned char card_RxThreshold_val;
	unsigned char reserved[20];         // �����ֽڣ����ڽ�����չ��д��ʱӦȫ����
};
//extern struct PiccPara PiccPara;  //guohonglv 
*/
//�����Ƕ���lcd.c�ĺ궨���Լ��ṹ��
#define FBIOGET_VSCREENINFO 1 //guohonglv 
#define FBIOGET_FSCREENINFO 1 //guohonglv  

#define PROT_READ 1 //guohonglv 
#define PROT_WRITE 1 //guohonglv  
#define MAP_SHARED 1 //guohonglv

struct fb_var_screeninfo
{
  int xres;
  int yres;
};

struct fb_fix_screeninfo
{
  int smem_len;
};
 
#endif