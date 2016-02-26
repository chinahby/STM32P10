
#ifndef _EMV_CORE_H
#define _EMV_CORE_H

#define STRUCT_PACK __attribute__((packed))




#define PART_MATCH  0
#define FULL_MATCH  1

//���״����������붨��
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

//other
#define EMV_COMM_ERR      -50      

#define EMV_ONLINE         0xc8
//typedef unsigned char       uchar;
#define uchar unsigned char

#define DE55_MUST_SET	1	// �������
#define DE55_OPT_SET	2	// ��ѡ�����
#define DE55_COND_SET	3	// ������������

#pragma pack(1) 

typedef struct T_EMV_APP_PARAMETER
{
	uchar LocalAppLabel[33];   //��������,���ַ������
	uchar len;
	uchar aid[16];		       //Ӧ�ñ�ʾ
	uchar bSelectIndicator;    //ѡ���־(0xff:partial_MATCH ����ƥ��,full_Match)	
	uchar Priority;            //���ȼ���־
	uchar TargetPercentage;    
	uchar MaxTargetPercentage;
	uchar sTermFloor[4]; 
    uchar sOptional;
/*
 bit 8           7           6             5        4                 3    2   1
    FloorLimit RandTransSel VelocityCheck OnlinePin Selectable Kernel
    check
*/    	
	unsigned int  ThresholdValue;
	uchar TACDefault[5]; //b  5
	uchar TACDenial[5];  //b  5
	uchar TACOnline[5];  //b  5
	uchar AcquirerID[6];  //�յ���
	unsigned int DdolLen;
	uchar Ddol[252];             //b,move
	unsigned int TdolLen;
	uchar Tdol[252];             //b,move
	uchar TermAVM[2];  //application version number 
	uchar sTmp[20];  //risk data(8data) (len(1byte)+data) AmountX(9-12 Byte) AmountY(13-16Byte)
}T_EMV_APP_PARAMETER;

typedef struct T_EMV_TERM_CAPK
{
	uchar RID[5];
	uchar Index;
	uchar HashIndicator;
	uchar CAPKIndicator;
	uchar Len;
	uchar Modulus[248];
	uchar ExponentLen;
	uchar Exponent[ 3];
	uchar ExpDate[3]; //(yymmdd)
	uchar CheckSum[20];
	
}T_EMV_TERM_CAPK;

/*
at DICTCONTENT have
    Terminal Type:   
    Terminal Country Code
    Terminal Capabilities
    Additional Terminal Capabilities
    Point-of-Service (POS) Entry Mode
*/
typedef struct T_EMV_PARAMETER
{
	unsigned int MerchantNameLen;
	uchar MerchantNameLocation[256]; //?	
	uchar MerchantCode[2];
	uchar MerchantID[15];
	uchar TermID[8];
	uchar TermType;						//9f35 �ն�����
	uchar TermCapa[3];                  
	uchar AddTermCapa[5];
	uchar TranRefCurrExp;				//9f3d �ο�����ָ��
	uchar TranRefCurr[2];				//9f3c �ο����Ҵ���
	uchar TranCurrExp;					//5F36 ���׻��Ҵ���ָ��
	uchar TranCurrCode[2];				//5F2A ���׻��Ҵ���
	unsigned int AmtReferRate;			//-     �ο����Ҵ���ͽ��״����ת��ϵ��(���׻��ҶԲο����ҵĻ���*1000)
	uchar TerminalCountry[2];
	uchar TransType; //
	uchar sOptions[10];
/*
 BIT 8          7             6                5              4              3                2                   1
BYTE PSE        Cardholder   Preferred        Partial AID    Multi          Common            RFU                RFU          
 1   Selection  Confirmation  Order           (Application)  Language       Character Set              
                                              Selection              
                                        
 BYTE revocation of  terminal PINByPass       GetDataPINTC   FAILCVM        Are amounts       RFU                RFU
  2   issuer public  contains                                               known before
     key certificate a default DDOL                                         CVM processing
      
 BYTE Floor      RandomTrans  Velocity        TransLog       Exception      TerminalRisk      RFU                RFU
  3   LimitCheck Select       Check                          file           Management
                                                                            BasedOnAip

 BYTE support process default skip/process    Detect CDA     request/never  request/never     RFU                RFU
  4     TAC   AC prior/after  TAC/IAC default Failure Before in 1st         in 2nd Generate
              1st GenAC    unable to go online TAAnalysis    GenerateAC(ARQC)  AC(TC)         
              
 BYTE ForceOnline Force      Advice         IssuerReferral           OnlineCapture/BatchCapture DefaultTDOL        RFU      
  5             Acceptance  
  
 BYTE equipped amount and PIN ICC/magstripe  magstripe      AccountType  issuer script      internate Date       RFU
  6   with     enter at the   reader         reader         Select       >128               management
      PINPAD   same keypad    combined       first
      
 BYTE FallBack   FallBack    FallBack
  7   not Select Terminate   1st GAC
  
 BYTE
  8
 
 BYTE
  9    PBOC    ETEC(Europay  VSDC(VISA ICC   VISANET      JCB(JCB EMV      AMEX(AMEX
               Terminal      Terminal                     specify)         Terminal
               Require)      specify)                                      Require)
 BYTE
  10
        
Terminal Capabilities include                                   
         Card Data Input Capabilities        
         CVM Capabilities
         Security Capabilities
Additional Terminal Capabilities include
		 Transaction type Capabilities           
		 Transaction type Capabilities           
		 Terminal Data input Capabilities
		 Terminal Data output Capabilities
		 Terminal Data output Capabilities
		 
*/
	
}T_EMV_PARAMETER;


typedef struct T_EMV_REVOCATIONLIST
{
	uchar RID[5];	
	uchar Index;
	uchar CertiSerial[3];
	uchar sRFU[1];
}T_EMV_REVOCATIONLIST; 

#pragma pack() 

extern int 	 EMV_KernelInit (void);
extern void	 EMV_KernelVersion(char *sVer);
extern void  EMV_SetParameter(T_EMV_PARAMETER *Param);
extern void  EMV_GetParameter(T_EMV_PARAMETER *Param);
extern int   EMV_GetTLVData(unsigned short Tag, unsigned char *DataOut, int *OutLen);
extern int   EMV_SetTLVData(unsigned short Tag, unsigned char *DataIn, int DataLen);
extern int   EMV_ClearALLTLVData();
extern int   EMV_GetScriptResult(unsigned char *Result, int *RetLen);
	
extern int   EMV_AddApp(T_EMV_APP_PARAMETER *App);
extern int   EMV_GetApp(int Index, T_EMV_APP_PARAMETER *App);
extern int   EMV_DelApp(unsigned char *AID, int AidLen);
extern int   EMV_AddCAPK(T_EMV_TERM_CAPK  *capk );
extern int   EMV_GetCAPK(int Index, T_EMV_TERM_CAPK  *capk );
extern int   EMV_DelCAPK(unsigned char KeyID, unsigned char *RID);

extern int   EMV_DelAllCAPK(); //2011/01/07 add

extern int   EMV_CheckCAPK(unsigned char *KeyID, unsigned char *RID);
extern int   EMV_AddRevocList(T_EMV_REVOCATIONLIST *pRevocList);
extern int   EMV_DelRevocList(unsigned char ucIndex, unsigned char *pucRID,unsigned char *CertiSerial);
extern void  EMV_DelAllRevocList(void);
//exception file(black list)��Ӧ���������

extern int   EMV_AppSelect(int Slot,unsigned long TransNo);
extern int   EMV_ReadAppData(void);
extern int   EMV_OfflineDataAuth(void);
extern int   EMV_ProcessRestrictions(void);
extern int   EMV_CardholderVerification(void);
extern int   EMV_TerminalRiskManagement(void);
extern int   EMV_TerminalAndCardActionAnalysis(void);
extern int   EMV_AppSelectForLog(int Slot); //pboc
//extern int  EMV_ReadCardLogRecord(int RecordNo);   //pboc
extern int   EMV_ReadCardLogRecord(int RecordNo,uchar *sFormat,uchar *sRecord);   //pboc,RecordNo>=1
extern int   EMV_OnlineProcessing();

//extern int  EMV_GetItemFromLog(unsigned short Tag, unsigned char *TagData, int *TagLen); //pboc

//call back
extern int   EMVMMI_MenuAppSel(int TryCnt, char *List[], int AppNum);
extern int   EMVMMI_InputAmount(unsigned char *AuthAmt, unsigned char *CashBackAmt);
extern int   EMVMMI_InputPIN(unsigned char TryFlag, int RemainCnt, unsigned char *pin);

extern int   EMVIF_CertVerify(void);
extern int   EMVIF_ReferProc(void);  //only for disp,ex boolean EmvIF_GetReferralResults(EMVLanguages *pelsLangs,boolean blRemove);
extern int   EMVIF_OnlineProc(unsigned char *RspCode, unsigned char *AuthCode, unsigned int *AuthCodeLen, unsigned char *IAuthData,unsigned int *IAuthDataLen, unsigned char *script71,unsigned int *ScriptLen71,unsigned char *script72,unsigned int *ScriptLen72);
extern void  EMVIF_AdviceProc(void);
extern void  EMVMMI_VerifyPINOK(unsigned char bFlag);
extern int   EMVIF_UnknowTLVData(unsigned short Tag, unsigned char *dat, int *pnLen);
extern void  EMVIF_ClearDsp(void);
extern int  EMVIF_DisplayWait(int line_num, int place, int clear_line_flag, unsigned char *str,int iTimeInt);
extern uchar EMVIF_SearchExceptionList(unsigned char bLen,unsigned char *rdPAN,unsigned char  *rdPANSeq);
//log.save at App.
extern uchar EMVIF_GetsumLogByPAN(unsigned char bLen,unsigned char *sPAN,unsigned int *nAmount);
extern void  EMVIF_GetRandNumber(unsigned int *prdRand);
extern void EMVIF_PrintSignature();

extern int EMVIF_FileSize(char *sName); 
//Transaction Sequence Counter(9f41)

#endif

