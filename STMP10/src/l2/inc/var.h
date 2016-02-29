/*******************************************************************************
* File Name			 : var.h
* Author             : guohonglv
* Date First Issued  : 2010��11��12��9:55:48
* Description        : ֱ������QPBOCTEST��Ŀ���ļ���Ϊ��׼C���Ըı���һ�·�񣬲�������ͷ�ļ�������
********************************************************************************/

#define uchar unsigned char

#include <stdlib.h>
#include <string.h>

#include "misc.h"
#include "emvcore.h"
#include "contactless_common.h"
#include "entrycore.h"
#include "qpboccore.h"
//#include "keymap.h"
#include "fcntl.h"
#include "emvif.h"
#include "struct.h"
//#include "logic_error.h"
//#include "errno.h"
#include "stat.h"
//#include "kf322_picc.h"
//#include "SystemFunction.h"
#include "scc-support.h"

#include "UartProtocol.h"

#pragma pack(1) 
typedef struct _POS_PARAM_STRC 
{
	uchar ucCommWaitTime; 	/*ͨѶ�ȴ�ʱ��*/
	uchar szUnitNum[16];
	uchar szPosId[9];
	uchar szUnitChnName[41];
	int  iMaxTransTotal;  	/*��ˮ���������ױ���������*/
	uchar ucEmvSupport;
	uchar ucSelectCardType;	// PARAM_OPEN: oper select card type, PARAM_CLOSE: auto-select
	long lNowTraceNo; 		/*��ǰ�ն˽�����ˮ��*/
	long lNowBatchNum;		/*��ǰ���κ� */
	uchar ucOprtLimitTime;  /* ������ʱʱ�� 30S-60S*/
		

}T_POS_PARAM_STRC;

typedef struct T_CTRL_PARAM 
{

	uchar bPosStatus; /*'0' ���� '1'����״̬ '2'������״̬*/
	uchar bBatchUpStatus; 	/*��ǰ������״̬��RMBLOG ��������ҿ� FRNLOG�����⿨ ALLLOG������״̬ */
	uchar bLoadTask;  /*�Ƿ������������־ 1 - ��Ҫ 0 - ����Ҫ*/
	uchar sLastTransDate[9]; /*���һ�ʽ��׵�����*/
	int   nTransNum;  	/*��ǰ�����ܱ���*/
	int   nBatchNum;  	/*��ǰBatch�ܱ���*/
	uchar ucClearLog;	/*������Ƿ�δ����־    TURE:δ��*/
	uchar bEmvCapkLoaded;	// CAPK�Ƿ����سɹ�
	uchar bEmvAppLoaded;	// EMV AID�����Ƿ����سɹ�

}T_CTRL_PARAM;

typedef struct _STRUCT_PACK
{
    unsigned char  TransType;        /* 9cTransaction type */
	unsigned char  TranDate[3];      //9a
    unsigned char  PanLen;
    unsigned char  Pan[10];          /* 5aPrimary account No. */
	unsigned char  PANSN;            //5f34
//    unsigned char  AmountLen;  //20090523 disable
    unsigned char  sAmount[6];        /* 9f02Trancaction amount */
//	unsigned char  AACLen;
	unsigned char  AAC[8];           //9f26 80Application Cryptogram (ARQC)
	unsigned char  CryData;          //9f27 81Cryptogram Information Data
	unsigned char  IssuerAppLen;
	unsigned char  IssuerAppData[32];//9f10 61Issuer Application Data (IAD)
	unsigned char  UnpredictNum[4];  //9f37 90Unpredictable Number
	unsigned char  ATC[2];           //9f36 89Application transaction counter (ATC)
	unsigned char  TVR[5];           //95 40Terminal Verification Result
	unsigned char  TranCurrency[2];  //5f2a 9Transaction Currency Code
	unsigned char  AIP[2];           //82 23Application Interchange Profile
	unsigned char  TerminalCountry[2];//9f1a 70Terminal Country Code
//	unsigned char  AmountOtherLen;
	unsigned char  AmountOtherNum[6];//9f03 50Amount Other
	unsigned char  POSEntry;         //9f39 92POS Entry Mode 9f39
	unsigned char  TranSeq[4];       //9f41 Transaction Sequence Counter
	unsigned char  MsgType[2];       //Advice - \x01\x20
	unsigned char  EC_Trans;
	unsigned char  Ec_Balance[6];
}T_EMVTRANSREC;

typedef struct POS_COM_STRUCT 
{    
	long  lBalance;			/*  ���ʻ����          N(9)    */
///	uchar  szOldTrnDate[5];	/*  ԭ��������                  */
	uchar  szRespCode[2+1];       /*  ���׷�����              */
	uchar  szUnitNum[15+1];		/*  ��Լ��λ��          N(8)   */
	uchar  sPIN[8];				/*  ��������                    */
	uchar  szNetInfoCode[3+1];
	uchar  szTrack2[TRACK2_LEN+1];
	uchar  szTrack3[TRACK3_LEN+1];
	
	//disable 2010/12/29
	//uchar  sField48[512];
	//int	   iField48Len;
	//uchar  sField62[512];
	//uchar  sField61[512];
	
	int    iField61Len;
	uchar  ucPrtflag;			//���׽������Ƿ��ӡƾ�� 
	uchar  ucWriteLog;			//���׽������Ƿ����־
//	LOG_STRC stTrans;			///����Ҫ����Ϣ����Ҫ��¼����־�е�����  09/06/29 
    uchar szCardNo[21];       //move from stTrans;
    T_EMVTRANSREC tEmvTrans; // 09/06/30 add
//	POS_8583PACKET_STRC stTrnTbl;  ///���״�����Ƿ��ӡ�ȵ���Ϣ   09/06/29
	uchar  ucAmtSign;	///���������� '+' '-'
	uchar  ucFallBack;	// TRUE , FALSE
	uchar  ucSwipedFlag; // ˢ���ı�־
	uchar  ucPinEntry;	// TRUE: PIN enter, FALSE: No PIN enter
	//uchar  sReversalDE55[128];	// �����ͽű�֪ͨ��BIT 55����

	//disable 2010/12/30
//	uchar  sReversalDE55[219];	//reverse(test)��󳤶�Ϊ218,add type �����ͽű�֪ͨ��BIT 55����
//	int    iReversalDE55Len;
//	uchar  sScriptDE55[128];
//	int    iScriptDE55Len;
	
	uchar  bOnlineTxn;	// ICC online falg
	uchar  szCertData[2+20+1];	// type(2)+ID no(20)
	uchar  bIsFirstGAC;
//	STISO8583	stOrgPack;
	uchar	ucIsWriteRevLog;		// �Ƿ��Ѿ���¼������ˮ�ı�־(���ڼ�¼������ˮ�ļ��ı�־����ֹ�ظ��ǳ�����ˮ)
	uchar  bSkipEnterAmt;	// for process TIP fallback when 1st GAC return 6985 [7/26/2006 Tommy]

}POS_COM;





/* Terminal supports exception file*/
typedef struct
{
	uchar bLen;
	uchar PAN[10];
	uchar PANSeqNum;
	
}T_TERMEXCEPTIONFILE;


typedef struct temp_global_var_strc
{
	int	 iTransNo;			//���ױ��,
	int  iLogFilePosi;		/*�������Ľ������ļ��е�λ��*/
	uchar ucPINTimes;	//�����������,�ն˿��Ʋ�Ҫ����3��
	uchar ucRmbSettRsp;		/*����ҿ�������*/
	uchar ucFrnSettRsp;		/*��ҿ�������*/
	uchar szDate[9];
	uchar szTime[7];
	uchar sPacketHead[7];
	uchar szSendId; /*ԭ�����Ƿ��Ѿ����� 1���� 0����*/
	uchar szDispBuf[33];
	uchar ucPrnVer; /* ��ӡ���ͺ� 'S' ��� 'T' ������ӡ*/
}T_TEMP_VAR;

typedef struct 
{
	uchar bId;
	uchar sAbbr[3];    //abbreviation
	uchar sName[20];
	
}T_LANGUAGE;

typedef struct 
{
	char sAmount[10];
	char sAmountOK[20];
	char sApproved[20];
	char sCallYourBank[30];
	char sCancelOrEnter[30];
	char sCardError[10];
	char sDeclined[10];
	char sEnterAmount[20];
	char sEnterPIN[20];
	char sIncorrectPIN[20];
	char sInsertCard[12];
	char sNotAccepted[20];
	char sPINOK[10];
	char sPleaseWait[20];
	char sProcessingError[20];
	char sRemoveCard[20];
	char sUseChipReader[20];
	char sUseMagStripe[20];
	char sTryAgain[10];
	
}T_STANDMESS;


typedef struct 
{
	uchar Buff[1024];
	uchar Len;
	uchar RecNum;
	uchar SFI;
}GLSTACK;

typedef struct TERMAPPLIST
{
	uchar Aid[17];
	uchar Aidlen;
	uchar onlinepin;
	uchar ucSelflag;
	long ulECTTLVal;
	uchar ucRdClssTxnLmtFlg;			// �Ƿ���ڶ������ǽӴ������޶�
	unsigned long ulRdClssTxnLmt;	//�������ǽӴ������޶� ��n12��
	uchar ucRdCVMLmtFlg;				// �Ƿ���ڶ������ǽӴ�CVM�޶�
	unsigned long ulRdCVMLmt;		// �������ǽӴ�CVM�޶n12��
	uchar ucRdClssFLmtFlg;		// �Ƿ���ڶ������ǽӴ��޶�
	unsigned long ulRdClssFLmt;		//�������ǽӴ��޶� ��n12�� 
	unsigned char sRFU[1];   
}TERMAPPLIST;

#pragma pack() 

enum 
{
	LOCALE,  //0
	SUPPORT, //1
};
#define COM_SIZE  sizeof(struct POS_COM_STRUCT)

extern POS_COM gtPosCom;

extern T_EMV_PARAMETER gtEmvParameter;
extern T_CTRL_PARAM    gtCtrlParameter;
extern T_POS_PARAM_STRC gtPosParameter;

extern POS_COM gtPosCom;
extern T_TEMP_VAR gtTemp;

//contactless
extern  Contactless_ReaderParam ClssParam;
//extern	Clss_TransParam_stru ClssEcParam;

//extern	PICC_PARA PiccPara; 
//	EMV_REVOCLIST EmvRevoclist;
//extern	Clss_PreProcInfo_Stru qvClssParam;
//extern	Clss_PreProcInterInfo ClssProcInterInfo;
//	Clss_VisaAidParam ClssVisaAidParam;
//	Clss_PbocAidParam ClssPbocAidParam;
//	Clss_SchemeID_Info ClssSchemeIDInfo;
//	uchar Mob_Pboc_Support,qpboc_stau;

//extern TERMAPPLIST Term_Applist[AppListStoredNum];



extern T_STANDMESS glTStandMess[2];
extern T_LANGUAGE gltLanguage[2];
extern int glbLanguage;
extern char sDispEnterPINAgain[2][25];
extern char sDispCancelThisTran[2][30];
extern char sDispForcedOnline[2][50];
extern char sDispPAN[2][10];
extern char sDispSelect[2][10];
extern char sDispReferralCard[2][90];
extern char sDispReferralIssuer[2][50];
extern char sDispPINTryLimitExceeded[2][30];
extern char sDispFinishRemoveCard[2][30];
extern char sDispPleaseSelectLanguage[2][30];
extern char sDispEnter1Or2[2][15];
extern char sDispEnterAmountAuthorised[2][30];
extern char sDispEnterAmountOther[2][30];
extern char sDispPressAnyKeyContinue[2][30];
extern char sDispEnterPurchaseAmount[2][25];
extern char sDispEnterCashBackAmount[2][30];
extern char sDispEnterPINAgain[2][25];
extern char sDispIccInitErr[2][15];
extern char sDispCardholderConfirm[2][30];

typedef struct
{
    long tv_sec;
    long tv_usec;
}struct_timeval;

extern struct_timeval      g_key_start_tv; //��¼���һ�ΰ�����ʱ��
extern struct_timeval      g_key_cur_tv;   //��ǰʱ�䣬���ں����һ�ΰ�������ʱ��Ƚ�

extern unsigned char g_nCol,g_nRow;

extern int gnfd_rng;
extern uchar _gnRespCode[2];
extern uchar gbTimeout;
extern uchar gbPrintSignature;

//inter call
extern void RemoveTailChars(uchar *pszString, uchar ucRemove);
extern uchar MatchTrack2AndPan(uchar *pszTrack2, uchar *pszPan);
extern uchar GetCardFromTrack(uchar *szCardNo,uchar *track2,uchar *track3);
extern void  gettime(struct tm *tDate);





//extern void DebugTraceDisp(char *sDisp,uchar* sHex,int nHexlen);
extern void debug_printf(u8 col,u8 row,u8 mode,char *str,...);
extern void trace_debug_printf(char *str,...);
#ifdef DEBUG
	//#define TraceDisp(sMsg,sHex,nHexlen)		DebugTraceDisp(sMsg,sHex,nHexlen)
	#define TraceDisp		trace_debug_printf

#else
	#define TraceDisp		trace_debug_printf
#endif

