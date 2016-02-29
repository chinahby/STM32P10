#ifndef _VEMV_H_
#define _VEMV_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*emv_lib.h*/
#define ENGLISH				0
#define CHINESE				1

#define TYPE_CASH			0x01	//��������(�ֽ�)
#define TYPE_GOODS			0x02	//��������(����)
#define TYPE_SERVICE		0x04	//��������(����)
#define TYPE_CASHBACK		0x08	//��������(����)
#define TYPE_INQUIRY		0x10	//��������(��ѯ)
#define TYPE_PAYMENT		0x20	//��������(֧��)
#define TYPE_ADMINISTRATIVE	0x40	//��������(����)
#define TYPE_TRANSFER		0x80	//��������(ת��)

#define MAX_APP_NUM			32		//Ӧ���б����ɴ洢��Ӧ����
#define MAX_CAPK_NUM		64		//��֤��Կ�����ɴ洢�Ĺ�Կ��
#define MAX_CAPKREVOKE_NUM	96		//��֤��Կ�����б��������32*3

#define PART_MATCH			0x00	//ASI(����ƥ��)
#define FULL_MATCH			0x01	//ASI(��ȫƥ��)

#define EMV_GET_POSENTRYMODE			0 
#define EMV_GET_BATCHCAPTUREINFO		1 
#define EMV_GET_ADVICESUPPORTINFO		2 

//���״����������붨��
#define EMV_OK              0      //�ɹ�  
#define ERR_EMVRSP         (-1)      //���������
#define ERR_APPBLOCK       (-2)      //Ӧ������
#define ERR_NOAPP          (-3)      //��Ƭ��û��EMVӦ��
#define ERR_USERCANCEL     (-4)      //�û�ȡ����ǰ��������
#define ERR_TIMEOUT        (-5)      //�û�������ʱ
#define ERR_EMVDATA        (-6)      //��Ƭ���ݴ���
#define ERR_NOTACCEPT      (-7)      //���ײ�����
#define ERR_EMVDENIAL      (-8)      //���ױ��ܾ�
#define ERR_KEYEXP         (-9)      //��Կ����
#define ERR_NOPINPAD       (-10)     //û��������̻���̲�����
#define ERR_NOPIN          (-11)     //û��������û���������������
#define ERR_CAPKCHECKSUM   (-12)     //��֤������ԿУ��ʹ���
#define ERR_NOTFOUND       (-13)     //û���ҵ�ָ�������ݻ�Ԫ��
#define ERR_NODATA         (-14)     //ָ��������Ԫ��û������
#define ERR_OVERFLOW       (-15)     //�ڴ����
#define ERR_NOTRANSLOG     (-16)     //�޽�����־
#define ERR_NORECORD       (-17)     //�޼�¼
#define ERR_NOLOGITEM      (-18)     //Ŀ־��Ŀ����
#define ERR_ICCRESET       (-19)     //IC����λʧ��
#define ERR_ICCCMD         (-20)     //IC����ʧ��
#define ERR_ICCBLOCK       (-21)     //IC������ 
#define ERR_ICCNORECORD    (-22)     //IC���޼�¼
#define ERR_GENAC1_6985    (-23)     //GEN AC�����6985
#define ERR_USECONTACT     (-24)     //�ǽ�ʧ�ܣ����ýӴ�����
#define ERR_APPEXP         (-25)     //qPBOC��Ӧ�ù���
#define ERR_BLACKLIST      (-26)     //qPBOC��������
#define ERR_GPORSP         (-27)     //err from GPO
//#define ERR_TRANSEXCEEDED  (-28)     //�ǽӽ��׳���
#define ERR_TRANSEXCEEDED  (-29)     //�ǽӽ��׳���
#define ERR_QPBOCFDDAFAIL  (-30)     //�ǽ�qPBOC fDDAʧ��

#define REFER_APPROVE		0x01	//�ο�������(ѡ����׼)
#define REFER_DENIAL		0x02	//�ο�������(ѡ��ܾ�)
#define ONLINE_APPROVE		0x00	//����������(������׼)     
#define ONLINE_FAILED		0x01	//����������(����ʧ��) 
#define ONLINE_REFER		0x02	//����������(�����ο�)
#define ONLINE_DENIAL		0x03	//����������(�����ܾ�)
#define ONLINE_ABORT		0x04	//����PBOC(������ֹ)
#define ONLINE_REFERANDFAIL 0x05	//�����ܾ��������ο�


#define PATH_PBOC			0x00	//Ӧ��·������׼PBOC
#define PATH_QPBOC			0x01	//Ӧ��·����qPBOC
#define PATH_MSD			0x02	//Ӧ��·����MSD
#define PATH_ECash			0x03	//Ӧ��·���������ֽ�

//STRUCTOR DEF
typedef struct _EMV_APPLIST_{
    unsigned char AppName[33];       //����Ӧ��������'\x00'��β���ַ���
    unsigned char AID[17];           //Ӧ�ñ�־
    unsigned char AidLen;            //AID�ĳ���
    unsigned char SelFlag;           //ѡ���־( ����ƥ��/ȫƥ��)      
    unsigned char Priority;          //���ȼ���־
    unsigned char TargetPer;         //Ŀ��ٷֱ���
    unsigned char MaxTargetPer;      //���Ŀ��ٷֱ���
    unsigned char FloorLimitCheck;   //�Ƿ�������޶�
    unsigned char RandTransSel;      //�Ƿ�����������ѡ��
    unsigned char VelocityCheck;     //�Ƿ����Ƶ�ȼ��
    unsigned long FloorLimit;        //����޶�
    unsigned long Threshold;         //��ֵ
    unsigned char TACDenial[6];      //�ն���Ϊ����(�ܾ�)
    unsigned char TACOnline[6];      //�ն���Ϊ����(����)
    unsigned char TACDefault[6];     //�ն���Ϊ����(ȱʡ)
    unsigned char AcquierId[7];      //�յ��б�־
    unsigned char dDOL[256];         //�ն�ȱʡDDOL
    unsigned char tDOL[256];         //�ն�ȱʡTDOL
    unsigned char Version[3];        //Ӧ�ð汾
    unsigned char RiskManData[10];   //���չ�������
	unsigned char EC_bTermLimitCheck;      //�Ƿ�֧���ն˽����޶�
	unsigned long EC_TermLimit;            //�ն˽����޶
	unsigned char CL_bStatusCheck;         //�Ƿ�֧��qPBOC״̬���
	unsigned long CL_FloorLimit;        //�ǽӴ��ն�����޶�
	unsigned long CL_TransLimit;        //�ǽӴ��ն˽����޶�
	unsigned long CL_CVMLimit;          //�ǽӴ��ն�CVM��
	unsigned char TermQuali_byte2;      //���׽����ÿ��AID�޶���жϽ������ˢ��ǰ����ͨ���˱��������жϽ��
}EMV_APPLIST;

typedef struct {
    unsigned char RID[5];            //Ӧ��ע�������ID
    unsigned char KeyID;             //��Կ����
    unsigned char HashInd;           //HASH�㷨��־
    unsigned char ArithInd;          //RSA�㷨��־
    unsigned char ModulLen;          //ģ����
    unsigned char Modul[248];        //ģ
    unsigned char ExponentLen;       //ָ������
    unsigned char Exponent[3];       //ָ��
    unsigned char ExpDate[3];        //��Ч��(YYMMDD)
    unsigned char CheckSum[20];      //��ԿУ���
}EMV_CAPK;

typedef struct{
    unsigned char MerchName[256];    //�̻���
    unsigned char MerchCateCode[2];  //�̻������(ûҪ��ɲ�����)
    unsigned char MerchId[15];       //�̻���־(�̻���)
    unsigned char TermId[8];         //�ն˱�־(POS��)
    unsigned char TerminalType;      //�ն�����
    unsigned char Capability[3];     //�ն�����
    unsigned char ExCapability[5];   //�ն���չ����
    unsigned char TransCurrExp;      //���׻��Ҵ���ָ��
    unsigned char ReferCurrExp;      //�ο�����ָ��
    unsigned char ReferCurrCode[2];  //�ο����Ҵ���
    unsigned char CountryCode[2];    //�ն˹��Ҵ���
    unsigned char TransCurrCode[2];  //���׻��Ҵ���
    unsigned long ReferCurrCon;      //�ο����Ҵ���ͽ��״����ת��ϵ��(���׻��ҶԲο����ҵĻ���*1000)
    unsigned char TransType;         //��ǰ�������� 
    unsigned char ForceOnline;       //�̻�ǿ������(1 ��ʾ������������)
    unsigned char GetDataPIN;        //������ǰ�Ƿ�����Դ���
    unsigned char SupportPSESel;     //�Ƿ�֧��PSEѡ��ʽ
	unsigned char TermTransQuali[4]; //�ն˽�������
	unsigned char TermIsSupEct;       //�ն��Ƿ�֧�ֵ����ֽ���
	unsigned char ECTSI;             //�����ֽ��ն�֧��ָʾ��
	unsigned char EC_bTermLimitCheck;//�Ƿ�֧���ն˽����޶�
	unsigned long EC_TermLimit;		//�ն˽����޶
	unsigned char CL_bStatusCheck;	//�Ƿ�֧��qPBOC״̬���
	unsigned long CL_FloorLimit;	//�ǽӴ��ն�����޶�
	unsigned long CL_TransLimit;	//�ǽӴ��ն˽����޶�
	unsigned long CL_CVMLimit;		//�ǽӴ��ն�CVM�޶�
}EMV_PARAM;

//////////////////////////////////////////////////////////////////////////////////
//callback programe
int  cEmvLib_WaitAppSel(int TryCnt, EMV_APPLIST *List, int AppNum);  
int  cEmvLib_InputAmt(unsigned long *AuthAmt, unsigned long *CashBackAmt);
int  cEmvLib_GetHolderPwd(int TryFlag, int RemainCnt, unsigned char *pin);
int  cEmvLib_ReferProc(void);
int  cEmvLib_GetUnknowTLV(unsigned short Tag, unsigned char *dat, int len);
void cEmvLib_AdviceProc(void);
void cEmvLib_VerifyPINOK(void);
int  cEmvLib_VerifyCert(void);
int  cEmvLib_GetDateTime(unsigned char *datetime);
void cEmvLib_IoCtrl(unsigned char ioname,unsigned char *iovalue);
unsigned long  cEmvLib_GetAllAmt(unsigned char *PANData,int PANDataLen, unsigned long AuthAmt);
//////////////////////////////////////////////////////////////////////////////////
//EXPORT FUNCRION
void  Rsa_ItWell(unsigned char* Modul,unsigned int ModulLen,unsigned char* Exp,unsigned int ExpLen,unsigned char* DataIn,unsigned char* DataOut);
void  Hash(unsigned char* DataIn, unsigned long DataInLen, unsigned char* DataOut);

//int EmvLib_AppSelForLog(int Slot);
//int EmvLib_ReadLogRecord(int RecordNo);
//int EmvLib_GetLogItem(unsigned short Tag, unsigned char *TagData, int *TagLen);

/*************************************************************************************
��    ��: ������
��    Ȩ: �����������Ӽ���(����)���޹�˾
��������: ���ö�IC�������Ŀ�����
��ڲ���: Mode 0x01:����IC��  0x02:����IC��  0x3:����PICC 0x04:����PICC 	
�� �� ֵ: 0x00 �ɹ�  0x01:Mode��������
��    ��: 
��	  ע:  
*************************************************************************************/
int EmvLib_SetIcCardType(int Mode);

/*************************************************************************************
��    �ߣ�������
��    Ȩ�������������Ӽ��������������޹�˾
��    �ܣ�ȡemv��汾��
���������
�����������
��    �أ�int�Ͱ汾��
��    ע��
*************************************************************************************/
int  EmvLib_GetVer(void);

/*************************************************************************************
��    �ߣ�������
��    Ȩ�������������Ӽ��������������޹�˾
��    �ܣ���ȡEMV����
���������
		tParam:�����ṹָ��
�������������ָ��
��    �أ���
��    ע��
*************************************************************************************/
void EmvLib_GetParam(EMV_PARAM *tParam);

/*************************************************************************************
��    �ߣ�������
��    Ȩ�������������Ӽ��������������޹�˾
��    �ܣ�����EMV����
���������
		tParam:�����ṹָ��
�����������
��    �أ���
��    ע��
*************************************************************************************/
void EmvLib_SetParam(EMV_PARAM *tParam);

/*************************************************************************************
��    �ߣ�������
��    Ȩ�������������Ӽ��������������޹�˾
��    �ܣ�����tagȡ������
���������
		Tag:���
		DataOut:����ָ��
		OutLen:����ָ��
�������������ָ��
��    �أ�0=�ɹ�
��    ע��
*************************************************************************************/
int EmvLib_GetTLV(unsigned short Tag, unsigned char *DataOut, int *OutLen);

/*************************************************************************************
��    �ߣ�������
��    Ȩ�������������Ӽ��������������޹�˾
��    �ܣ�����tag��������
���������
		Tag:���
		DataIn:����ָ��
		DataLen:����
�����������
��    �أ�0=�ɹ�
��    ע��
*************************************************************************************/
int EmvLib_SetTLV(unsigned short Tag, unsigned char *DataIn, int DataLen);

/*************************************************************************************
��    �ߣ�������
��    Ȩ�������������Ӽ��������������޹�˾
��    �ܣ�ȡ�ýű�ִ�н��
���������
		Result:ִ�н��ָ��
		RetLen:����ָ��
�������������ָ��
��    �أ�0=�ɹ�
��    ע��
*************************************************************************************/
int EmvLib_GetScriptResult(unsigned char *Result, int *RetLen);

/*************************************************************************************
��    �ߣ�������
��    Ȩ�������������Ӽ��������������޹�˾
��    �ܣ�����֧�ֵ�Ӧ��
���������
		App:Ӧ������
�����������
��    �أ�0=�ɹ�
��    ע��
*************************************************************************************/
int EmvLib_AddApp(EMV_APPLIST *App);

/*************************************************************************************
��    �ߣ�������
��    Ȩ�������������Ӽ��������������޹�˾
��    �ܣ�����Ӧ������ȡ��֧��Ӧ������
���������
		Index:Ӧ������
		App:Ӧ������
�������������ָ��
��    �أ�0=�ɹ�
��    ע��
*************************************************************************************/
int EmvLib_GetApp(int Index, EMV_APPLIST *App);

/*************************************************************************************
��    �ߣ�������
��    Ȩ�������������Ӽ��������������޹�˾
��    �ܣ�ɾ��֧��Ӧ��
���������
		AID:Ӧ������
		AidLen:
�����������
��    �أ�0=�ɹ�
��    ע��
*************************************************************************************/
int EmvLib_DelApp(unsigned char *AID, int AidLen);

/*************************************************************************************
��    �ߣ�������
��    Ȩ�������������Ӽ��������������޹�˾
��    �ܣ�����cap��Կ
���������
		capk:cap��Կ���ݽṹָ��
�����������
��    �أ�0=�ɹ�
��    ע��
*************************************************************************************/
int EmvLib_AddCapk(EMV_CAPK  *capk );

/*************************************************************************************
��    �ߣ�������
��    Ȩ�������������Ӽ��������������޹�˾
��    �ܣ�ȡ��cap��Կ
���������
		capk:cap��Կ���ݽṹָ��
�������������ָ��
��    �أ�0=�ɹ�
��    ע��
*************************************************************************************/
int EmvLib_GetCapk(int Index, EMV_CAPK  *capk );

/*************************************************************************************
��    �ߣ�������
��    Ȩ�������������Ӽ��������������޹�˾
��    �ܣ�ɾ��cap��Կ
���������
		KeyID:��Կ���
		RID:Ӧ��ע�������ID
�����������
��    �أ�0=�ɹ�
��    ע��
*************************************************************************************/
int EmvLib_DelCapk(unsigned char KeyID, unsigned char *RID);

/*************************************************************************************
��    �ߣ�������
��    Ȩ�������������Ӽ��������������޹�˾
��    �ܣ����cap ��Կ�Ƿ���Ч
���������
		KeyID:��Կ���
		RID:Ӧ��ע�������ID
�����������
��    �أ�0=�ɹ�
��    ע��
*************************************************************************************/
int EmvLib_CheckCapk(unsigned char *KeyID, unsigned char *RID);

/*************************************************************************************
��    �ߣ�������
��    Ȩ�������������Ӽ��������������޹�˾
��    �ܣ�Ӧ��ѡ��
���������
		Slot: ���ۺ�
		TransNo:�������ͺ�
�����������
��    �أ�0=�ɹ�
��    ע��
*************************************************************************************/
int EmvLib_AppSel(int Slot,unsigned long TransNo);

/*************************************************************************************
��    �ߣ�������
��    Ȩ�������������Ӽ��������������޹�˾
��    �ܣ�EMV��Ƭ��ȡӦ������
�����������
�������������ȫ�ֽṹ
��    �أ�0=�ɹ�
��    ע��
*************************************************************************************/
int EmvLib_ReadAppData(void);

/*************************************************************************************
��    �ߣ�������
��    Ȩ�������������Ӽ��������������޹�˾
��    �ܣ�EMV��Ƭ��֤
�����������
�����������
��    �أ�0=�ɹ�
��    ע��
*************************************************************************************/
int EmvLib_CardAuth(void);

/*************************************************************************************
��    ��: ������
��    Ȩ: �����������Ӽ���(����)���޹�˾
��������: ������ɺ���ݷ��ؽ���Կ�������ɲ���
�������: 
	ucResult:����ȡֵ
		REFER_APPROVE     0x01     //�ο�������(ѡ����׼)
		REFER_DENIAL      0x02     //�ο�������(ѡ��ܾ�)
		ONLINE_APPROVE    0x00     //����������(������׼)
		ONLINE_FAILED     0x01     //����������(����ʧ��)
		ONLINE_REFER      0x02     //����������(�����ο�)
		ONLINE_DENIAL     0x03     //����������(�����ܾ�)
		ONLINE_ABORT      0x04     //����PBOC(������ֹ)
	RspCode: ��̨���ش����룬=PosCom.stTrans.szRespCode
	AuthCode:��̨������Ȩ�룬=PosCom.stTrans.AuthCode
	AuthCodeLen:
	IAuthData:��������֤����;
	IAuthDataLen:
	script:�������ű�
	ScriptLen:
����������� 
�� �� ֵ: 0=�ɹ�
��   ע:  
*************************************************************************************/
int EmvLib_ProcTransComplete(unsigned char ucResult, unsigned char *RspCode, 
									 unsigned char *AuthCode, int AuthCodeLen, 
									 unsigned char *IAuthData, int IAuthDataLen, 
									 unsigned char *script, int ScriptLen);

/*************************************************************************************
��    �ߣ�������
��    Ȩ�������������Ӽ��������������޹�˾
��    �ܣ���ʼ��EMV��
�����������
���������0
��    �أ�0=�ɹ�
��    ע��
*************************************************************************************/
int EmvLib_Init(void);

/*************************************************************************************
��    �ߣ�������
��    Ȩ�������������Ӽ��������������޹�˾
��    �ܣ�EMV�����������
�����������
		pStrMoney:��
		pPin:������ܺ�
		bIfGoOnline:�Ƿ�������
�������������ָ�� bIfGoOnline
��    �أ�0=�ɹ�
��    ע��
*************************************************************************************/
int EmvLib_ProcTrans(unsigned long nConsumeMoney, unsigned long nCashBackMoney, unsigned char *bIfGoOnline);

/*************************************************************************************
��    �ߣ�������
��    Ȩ�������������Ӽ��������������޹�˾
��    �ܣ�ȡ�����ֽ����
�����������
		BcdBalance:��
�������������ָ�� BcdBalance
��    �أ�0=�ɹ� ��-1ʧ��
��    ע��
*************************************************************************************/
int  EmvLib_GetBalance(unsigned char* BcdBalance);


//
//qPBOC����Ԥ����
int EmvLib_qPBOCPreProcess(void);
//qPBOC���״���
int EmvLib_ProcCLTrans(void);
//
int EmvLib_AppSelForLog(int Slot);


#ifdef __cplusplus
}
#endif



#endif

