/*******************************************************************************
* File Name			 : contaceless_common.h
* Author             : guohonglv
* Date First Issued  : 2010/11/12
* Description        : ��ȫ����qpboctest��Ŀ��ͷ�ļ������Ķ��˽ṹ����﷨
********************************************************************************/

#ifndef _CONTACTLESS_COMMON_H
#define _CONTACTLESS_COMMON_H

#define KERNTYPE_DEF   0
#define KERNTYPE_JCB   1	
#define KERNTYPE_MC    2
#define KERNTYPE_VIS   3
#define KERNTYPE_PBOC  4	
#define KERNTYPE_RFU 	 5


#define AC_AAC       0x00
#define AC_TC        0x01
#define AC_ARQC      0x02


//����·��
#define CLSS_PATH_NORMAL      0
#define CLSS_VISA_MSD         1   // scheme_visa_msd_20
#define CLSS_VISA_QVSDC       2   // scheme_visa_wave3
#define CLSS_VISA_VSDC        3   // scheme_visa_full_vsdc
#define CLSS_VISA_CONTACT     4 
// 5~6: MCHIP, MAGSTRIPE      
#define CLSS_MC_MAG           5
#define CLSS_MC_MCHIP         6
#define CLSS_VISA_WAVE2       7   //SCHEME_VISA_WAVE_2 // 7   // SCHEME_VISA_WAVE_2 


//�ֿ�����֤��ʽ
#define RD_CVM_NO 						0x00
#define RD_CVM_SIG 						0x10
#define RD_CVM_ONLINE_PIN 		0x11
#define RD_CVM_OFFLINE_PIN 		0x12

#define CLSS_MAX_KEY_NUM       	7          //EMV����֤���Ĺ�Կ�����ɴ洢�Ĺ�Կ��
#define CLSS_MAX_AIDLIST_NUM  	32

#define PART_MATCH  0
#define FULL_MATCH  1

#define EMV_OK             0         //�ɹ�

#define ICC_RESET_ERR     -1         //IC����λʧ��
#define ICC_CMD_ERR       -2         //IC����ʧ��
#define ICC_BLOCK         -3         //IC������    
   
#define EMV_RSP_ERR       -4         //IC���������
#define EMV_APP_BLOCK     -5         //Ӧ������
#define EMV_NO_APP        -6         //��Ƭ��û��EMVӦ��
#define EMV_USER_CANCEL   -7         //�û�ȡ����ǰ��������
#define EMV_TIME_OUT      -8         //�û�������ʱ
#define EMV_DATA_ERR      -9         //��Ƭ���ݴ���
#define EMV_NOT_ACCEPT    -10        //���ײ�����
#define EMV_DENIAL        -11        //���ױ��ܾ�
#define EMV_KEY_EXP       -12        //��Կ����


#define EMV_NO_PINPAD     -13        //û��������̻���̲�����
#define EMV_NO_PASSWORD   -14        //û��������û���������������
#define EMV_SUM_ERR       -15        //��֤������ԿУ��ʹ���
#define EMV_NOT_FOUND     -16        //û���ҵ�ָ�������ݻ�Ԫ��
#define EMV_NO_DATA       -17        //ָ��������Ԫ��û������
#define EMV_OVERFLOW      -18        //�ڴ����

//�ص��������������������붨��
#define REFER_APPROVE     0x01       //�ο�������(ѡ����׼)
#define REFER_DENIAL      0x02       //�ο�������(ѡ��ܾ�)

#define ONLINE_APPROVE    0x00       //����������(������׼)		
#define ONLINE_FAILED     0x01       //����������(����ʧ��)	
#define ONLINE_REFER      0x02       //����������(�����ο�)
#define ONLINE_DENIAL     0x03       //����������(�����ܾ�)
#define ONLINE_ABORT      0x04       //����PBOC(������ֹ)

//��������־
#define NO_TRANS_LOG      -19
#define RECORD_NOTEXIST   -20
#define LOGITEM_NOTEXIST  -21
#define ICC_RSP_6985      -22        // GAC�п�Ƭ����6985, ��Ӧ�þ����Ƿ�fallback
#define EMV_FILE_ERR      -23


#define CLSS_USE_CONTACT  			-24            // ����ʹ������������н���
//#define CLSS_READDATA_ERR -25                // ����¼�г���,Ӧ��ֹ���ײ�����ѯ��
#define CLSS_TERMINATE    			-26            // Ӧ��ֹ����       -25 
#define CLSS_FAILED       			-27            // ����ʧ�� 20081217 
#define CONTACTLESS_PARAM_ERR    			-30            // -26 // ��EMV �ں��еĲ���������Ϊ-30
#define CLSS_WAVE2_OVERSEA 			-31            // 20090418 for visa wave2 trans
#define CLSS_WAVE2_TERMINATED CLSS_TERMINATE //-32 // 20090421 for wave2 DDA response TLV format error
#define CLSS_WAVE2_US_CARD 			-33            // 20090418 for visa wave2 trans
#define CLSS_WAVE3_INS_CARD 		-34            // 20090427 FOR VISA L3
#define CLSS_RESELECT_APP   		-35
#define CLSS_CARD_EXPIRED       -36 // liuxl 20091104 for qPBOC spec updated

#pragma pack(1) 

// ÿ��AID��Ӧ�Ĳ���,���ڽ���Ԥ����
typedef struct CLSS_PREPROC_INTER_FLAG_INFO
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
}T_Clss_PreProcInterInfo;

//	CLSS_TRANSPARAM ������ز����ṹ
typedef struct CLSS_TRANSPARAM
{
    unsigned long  ulAmntAuth;     // ��Ȩ���(ulong)
    unsigned long  ulAmntOther;    // �������(ulong) 
    unsigned long  ulTransNo;      // �������м�����(4 BYTE)
	uchar  ucTransType;      // �������� not  '9C' !!! (GOODS CASH)
	uchar  aucTransDate[4];  // �������� YYMMDD
	uchar  aucTransTime[4];  // ����ʱ�� HHMMSS
}T_Clss_TransParam;

typedef struct  
{
	unsigned long   ulReferCurrCon;      	  // �ο����Ҵ���ͽ��״����ת��ϵ��(���׻��ҶԲο����ҵĻ���*1000) //*
	unsigned int    usMchLocLen;          	// �̻����Ƽ�λ��������ĳ��� 
	uchar   aucMchNameLoc[50];   	// �̻����Ƽ�λ��(1-256 �ֽ�) 257->50 2010/12/30
	uchar   aucMerchCatCode[2];   	// �̻�������'9F15'(2�ֽ�)  
	uchar   aucMerchantID[15];    	// �̻���ʶ(15�ֽ�)9F16  	
	uchar AcquirerId[6];       			//�յ��б�־ 9F01  //* download app
  uchar   aucTmID[8];           	// �ն˱�ʶ(�ն˺�)9F1C  
	uchar   ucTmType;             	// �ն�����9F35 
	uchar   ucTransType;            // Goods,Services, Cash,CashBack  10/10/18
	uchar   aucTmCap[3];          	// �ն�����9F33 
  uchar   aucTmCapAd[5];        	// �ն˸������� 
  uchar   aucTmCntrCode [2];      // �ն˹��Ҵ��� 9F1A  
	uchar   aucTmTransCur[2];       // �ն˽��׻��Ҵ���'5F2A'(2�ֽ�)  
	uchar   ucTmTransCurExp;        // �ն˽��׻���ָ��'5F36'(1�ֽ�)  
	uchar   aucTmRefCurCode[2];     // �ն˽��ײο����Ҵ���'9F3C'(2�ֽ�) 
	uchar   ucTmRefCurExp;	        // �ն˽��ײο�����ָ��'9F3D'(1�ֽ�) 
	uchar   aucRFU[3];
}Contactless_ReaderParam;

#pragma pack() 
#endif

