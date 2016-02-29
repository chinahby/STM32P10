/// @file   Data.h
/// This is the Head file for module @ref fiscal Data
/// @author Travis Tu
/// @date   2006-Sep-1st

#ifndef _DATA_H_
#define _DATA_H_


/**
 * @defgroup TaxBox_PARAMETER
 * @ingroup TaxBox
 * @{
 */
///all on chip Data
#define TAX_INDEX_NB                        6       ///<the tax items number
#define TAX_INVOICE_SCROLL_DISTRIBUTED_NB   20 ///<To be contained in one response
#define CELL_OCUPIED                        0xEE
#define MAX_ITEM_NB_IN_ONE_INVOICE 10  ///<considerate it and promiss 4+sizeof(UARTIssInvParamType) + sizeof(InvoicePrintInfoType) < UART_CMD_PARAMETER_LENGTH
#define TAXER_PAYER_NAME_LENGH              40
#pragma pack()

///enumeration for three issue invoice types
typedef enum
{
    ISSUE_INVOICE   = 0x01,
    RETURN_INVOICE  = 0x02,
    DISCARD_INVOICE = 0x03
}IssueInvCmdModeType;

///enumeration for four check types
typedef enum
{
    DETAIL_CHECK = 0x00,
    DAILY_TRANSACTION_DATA_CHECK = 0x01,
    APPLY_DATA_CHECK = 0x02,
    MODIFY_CLOCK = 0x03
}CheckModeType;

#pragma pack(1)
///Structure for element of machine initialize data
typedef struct
{
    u8 MachineNB[8];                    ///<�������
    u8 ProduceDate[4];                  ///<��������        Big Endian
    u8 HardwareVersion[18];             ///<Ӳ���汾��
    u8 SoftwareVersion[18];             ///<����汾��
}POSInitDataType ;

///Structure for element of the user data
typedef struct
{
    u8 FiscalICCNB[8];                      ///<˰�ؿ����
    u8 FiscalICCPassword[8];                ///<˰�ؿ�ʹ�ÿ���
    u8 AppStartDate[4];                     ///<Ӧ����ʼ����            Big Endian
    u8 AppStopDate[4];                      ///<Ӧ����Ч����            Big Endian
    u8 TaxerCompanyName[TAXER_PAYER_NAME_LENGH];                ///<��˰������or��˰�˹�˾����
    u8 TaxerNB[8];                          ///<��˰�˱��
    u8 TaxerID[20];                         ///<˰��or��˰��ʶ���
    u8 AdminDepNB[4];                       ///<���ܷ־ִ���
    u8 ApplayWay;                           ///<�걨��ʽ
    u8 IssuerInvoiceDeadLine[4];            ///<��Ʊ��ֹ����            Big Endian
    u32 SingleInvoiceUpLimitation;          ///<���ŷ�Ʊ��Ʊ����޶�    Big Endian
    u32 IssueInvoiceTotalUpLimitation;      ///<��Ʊ�ۼƽ���޶�        Big Endian
    u32 ReturnedInvoiceTotalUpLimitation;   ///<��Ʊ�ۼƽ���޶�        Big Endian
}POSUserInfoType;

///Structure for element of the tax items info
typedef struct
{
    u8 TaxIndex;                        ///<˰��˰Ŀ������
    u8 TaxCode[4];                      ///<˰����Ŀ����
    u8 TaxRate[2];                      ///<˰��
    u8 TaxTypeCNName[20];               ///<��Ӫ��Ŀ��������
    u8 TaxTypeENName[20];               ///<��Ӫ��ĿӢ������
}TaxSpeciesTaxDetailsType;

///Structure for element of the invoice roll usage info
typedef struct{
	u8	InvoiceCode[10];                ///<��Ʊ����
	u32	InvoiceBeginNB;                 ///<��Ʊ��ʼ�� Big Endian
	u32	InvoiceEndNB;                   ///<��Ʊ��ֹ�� Big Endian
	u8	NomInvoiceNB;                   ///<������Ʊ����
	u8	RtnInvoiceNB;                   ///<��Ʊ����
	u8	DscInvoiceNB;                   ///<��Ʊ����
	u32 NomInvoiceTotalAmt;             ///<������Ʊ�ܽ��    Big Endian
	u32	RtnInvoiceTotalAmt;             ///<��Ʊ�ܽ��        Big Endian
	u8	IssueInvoiceBeginTime[4];       ///<��Ʊ��ʼʱ��      Big Endian
	u8	IssueInvoiceEndTime[4];         ///<��Ʊ��ֹʱ��      Big Endian
}SingleInvoiceScrollUsageDataType;

typedef struct{
    u8  TaxerCompanyName[TAXER_PAYER_NAME_LENGH];           ///<��˰������or��˰�˹�˾����
    u8  TaxerID[20];                    ///<˰��or��˰��ʶ���
    u8  MachineNB[8];                   ///<�������
	u8	InvoiceCode[10];                ///<��Ʊ����
	u32	InvoiceBeginNB;                 ///<��Ʊ��ʼ�� Big Endian
	u32	InvoiceEndNB;                   ///<��Ʊ��ֹ�� Big Endian
	u8	NomInvoiceNB;                   ///<������Ʊ����
	u8	RtnInvoiceNB;                   ///<��Ʊ����
	u8	DscInvoiceNB;                   ///<��Ʊ����
	u32 NomInvoiceTotalAmt;             ///<������Ʊ�ܽ��    Big Endian
	u32	RtnInvoiceTotalAmt;             ///<��Ʊ�ܽ��        Big Endian
	u8	IssueInvoiceBeginTime[4];       ///<��Ʊ��ʼʱ��      Big Endian
	u8	IssueInvoiceEndTime[4];         ///<��Ʊ��ֹʱ��      Big Endian
}InvoiceScrollUsagePrinterInfo;

typedef struct{
    u8  TaxerCompanyName[TAXER_PAYER_NAME_LENGH];           ///<��˰������or��˰�˹�˾����
    u8  TaxerID[20];                    ///<˰��or��˰��ʶ���
    u8  MachineNB[8];                   ///<�������
	u8	IssueInvoiceBeginTime[4];       ///<��Ʊ��ʼʱ��      Big Endian
	u8	IssueInvoiceEndTime[4];         ///<��Ʊ��ֹʱ��      Big Endian
	u8	NomInvoiceNB;                   ///<������Ʊ����
	u32 NomInvoiceTotalAmt;             ///<������Ʊ�ܽ��    Big Endian
	u8	DscInvoiceNB;                   ///<��Ʊ����
	u8	RtnInvoiceNB;                   ///<��Ʊ����
	u32	RtnInvoiceTotalAmt;             ///<��Ʊ�ܽ��        Big Endian
}InvoiceUsagePrinterInfo;

///Structure for element of the invoice info
typedef struct{
    u8  IssueInvoiceDate[4];            ///<��Ʊ����      Big Endian
    u32 InvoiceNB;                      ///<��Ʊ��        Big Endian
    u8  IssueInvoiceType;               ///<��Ʊ����
    u32 InvoiceAmount;                  ///<��Ʊ�ܽ��    Big Endian
    u8  TaxContralNB[8];                ///<˰����
    u32 OriginalInvoiceNB;              ///<ԭ��Ʊ��      Big Endian
    u8  RFU[2];                         ///<����
}InvoicePrintInfoType;

///Structure for element of the DISTRIBUTE_INVOICE_NB command APUD carry data format
typedef struct{
    u8  InvoiceCode[10];                ///<��Ʊ����
    u32 SingleScrollBeginNB;            ///<����Ʊ��ʼ��    Big Endian
    u32 SingleScrollEndNB;              ///<����Ʊ��ֹ��    Big Endian
    u8  MAC[4];                         ///<MAC
    u8  CellOccupied;                   ///<if this cell is in use , then set this byte to 0xEE CELL_OCUPIED
}SingleInvoiceScrollType;

typedef struct
{
    u8  ItemName[20];               ///<��Ʒ����
    union{
        u8  DecFractionLength;      ///<����������С���λ��
        u32 ItemQuantity;           ///<����            Big Endian
    }Quantity;
    u32 UnitPrice;		            ///<����	        Big Endian
    u32 ItemCost;                   ///<�����ܽ��      Big Endian
    u8  TaxIndex;                   ///<˰��˰Ŀ������
}INVOICEITEM;

///Structure for element of the data from the UART port
typedef struct{
    u16 SubAccNB;                       ///<�ֻ����          Big Endian
    u8 TaxerName[TAXER_PAYER_NAME_LENGH];                   ///<���λ����
    u8 ItemsNum;                        ///<��Ӫ��Ŀ����
    struct{
        u8  ItemName[20];               ///<��Ʒ����
        union{
            u8  DecFractionLength;      ///<����������С���λ��
            u32 ItemQuantity;           ///<����                Big Endian
        }Quantity;
        u8  TaxIndex;                   ///<˰��˰Ŀ������
        u32 UnitPrice;                  ///<����	Big Endian
    }Items[MAX_ITEM_NB_IN_ONE_INVOICE]; ///<���item�ṹ
}UARTIssInvParamType;

///Structure for element of the invoice detail store info
typedef struct{
    u8 TaxerName[TAXER_PAYER_NAME_LENGH];                   ///<���λ����
    u8 OperatorName[10];                ///<�տ�Ա
    u8 ItemsNum;                        ///<��Ŀ��
    struct{
        u8  ItemName[20];               ///<��Ʒ����
        union{
            u8  DecFractionLength;      ///<����������С���λ��
            u32 ItemQuantity;           ///<����            Big Endian
        }Quantity;
        u32 UnitPrice;		            ///<����	        Big Endian
        u32 ItemCost;                   ///<�����ܽ��      Big Endian
        u8  TaxIndex;                   ///<˰��˰Ŀ������
    }Items[MAX_ITEM_NB_IN_ONE_INVOICE]; ///<���item�ṹ
}INVOICEDETAILINFO;

///Structure for element of the ISSUE_INVOICE command APUD carry data format
typedef struct{
    u8 Date[4]; 		                ///<��Ʊʱ��            Big Endian
    u8 IssType;                         ///<��Ʊ���� 01 ������Ʊ 02 ��Ʊ 03 ��Ʊ
    u32 InvNB;                          ///<��Ʊ��              Big Endian
    struct{                             ///<��Ʊ������, ��˰��˰Ŀ����,�ֳ�6��
        u8 TaxIndex;                    ///<˰��˰Ŀ������
        u32 Cost;                       ///<��˰�������ܶ�      Big Endian
    }ClassifiedCost[TAX_INDEX_NB];
    u32 TotalCost;                      ///<��Ʊ�ܽ��          Big Endian
    u8 CS;                              ///<�������������У��
}ICCIssInvCarryDataType;

///Structure for element of the declare data
typedef struct{
    u8  StartDate[4];                   ///<�걨������ʼ����	4 CCYYMMDD  Big Endian
    u8  StopDate[4];                    ///<�걨���ݽ�ֹ����	4 CCYYMMDD  Big Endian
    u32 NomInvNB;                       ///<������Ʊ����	    4           Big Endian
    u16 RtnInvNB;                       ///<��Ʊ����            2           Big Endian
    u16 DscInvNB;                       ///<��Ʊ����            2           Big Endian
    u8  TaxIndexs[TAX_INDEX_NB];        ///<˰��˰Ŀ������	    6
    u32 NomInvAmt[TAX_INDEX_NB];        ///<������Ʊ�����ۼƽ��            Big Endian
    u32 RtnInvAmt[TAX_INDEX_NB];        ///<��Ʊ�����ۼƽ��                Big Endian
    u32 NomInvTotalAmt;                 ///<˰�ؿ�������Ʊ���ۼƽ��        Big Endian
    u32 RtnInvTotalAmt;                 ///<˰�ؿ���Ʊ���ۼƽ��            Big Endian
}DeclareDataType;

///Structure for element of the DATA_COLLECT command APUD carry data format
typedef struct{
    u8  FiscalCardNB[8];                ///<˰�ؿ����	        8
    u8  StartDate[4];                   ///<�걨������ʼ����	4 CCYYMMDD  Big Endian
    u8  StopDate[4];                    ///<�걨���ݽ�ֹ����	4 CCYYMMDD  Big Endian
    u32 NomInvNB;                       ///<������Ʊ����	    4           Big Endian
    u16 RtnInvNB;                       ///<��Ʊ����            2           Big Endian
    u16 DscInvNB;                       ///<��Ʊ����            2           Big Endian
    u8  TaxIndexs[TAX_INDEX_NB];        ///<˰��˰Ŀ������	    6
    u32 NomInvAmt[TAX_INDEX_NB];        ///<������Ʊ�����ۼƽ��            Big Endian
    u32 RtnInvAmt[TAX_INDEX_NB];        ///<��Ʊ�����ۼƽ��                Big Endian
    u32 NomInvTotalAmt;                 ///<˰�ؿ�������Ʊ���ۼƽ��        Big Endian
    u32 RtnInvTotalAmt;                 ///<˰�ؿ���Ʊ���ۼƽ��            Big Endian
    u8  StatusByte;                     ///<״̬��              1
    u8  MAC1[4];                        ///<�걨����MAC1        4
    u8  Signature[128];                 ///<����ǩ��            128
}ICCDataCollectCarryDataType;

typedef struct{
    u8  MachineNB[8];                   ///<�������	        8
    u8  StartDate[4];                   ///<�걨������ʼ����	4 CCYYMMDD  Big Endian
    u8  StopDate[4];                    ///<�걨���ݽ�ֹ����	4 CCYYMMDD  Big Endian
    u8  RecordFileName[8];              ///<�ս��������ļ���
    u8  XOR;                            ///<��������У��
}DeclareDailyIndexFileType;

typedef struct{
    u8  MachineNB[8];                   ///<�������	        8
    u8  StartDate[4];                   ///<�걨������ʼ����	4 CCYYMMDD  Big Endian
    u8  StopDate[4];                    ///<�걨���ݽ�ֹ����	4 CCYYMMDD  Big Endian
    u16 Recordtotal;                    ///<��Ʊ��ϸ�ܼ�¼��        Big Endian
    u8  RecordFileName[8];              ///<��Ʊ��ϸ�����ļ���
    u16 RecordStartNo;                  ///<��Ʊ��ϸ��ʼ��¼���    Big Endian
    u16 RecordEndNo;                    ///<��Ʊ��ϸ������¼���    Big Endian
    u8  XOR;                            ///<�������У��
}DeclareDetailIndexFileType;

///Structure for element of the declare store data
typedef struct{
    DeclareDataType dclData;            ///<�걨����
    u8  StatusByte;                     ///<״̬��              1
    u8  MAC1[4];                        ///<�걨����MAC1        4
    u8  Signature[128];                 ///<����ǩ��            128
}DeclareDataLogType;

///Structure for element of the invoice print store data
typedef struct{
    InvoicePrintInfoType PrintInfo;
    u8 InvoiceScrollCode[10];		    ///<��Ʊ����
    INVOICEDETAILINFO    InvoiceDetailInfo;
    u8                      TaxerCompanyName[TAXER_PAYER_NAME_LENGH];           ///<��˰������or��˰�˹�˾����
    u8                      Time[2];                        ///<��Ʊʱ��
}InvoiceDetailInfoType;


typedef struct{
    InvoicePrintInfoType    PrintInfo;
    u8                      InvoiceScrollCode[10];		    ///<��Ʊ����
    INVOICEDETAILINFO       InvoiceDetailInfo;
    u8                      TaxerCompanyName[TAXER_PAYER_NAME_LENGH];           ///<��˰������or��˰�˹�˾����
//    BankingInfoType         BankingData[2];
    u8                      Time[2];                        ///<��Ʊʱ��
}InvoiceCounterfoilType;

typedef struct{
    InvoicePrintInfoType    PrintInfo;
    u8                      InvoiceScrollCode[10];		    ///<��Ʊ����
    INVOICEDETAILINFO       InvoiceDetailInfo;
    u8                      TaxerCompanyName[TAXER_PAYER_NAME_LENGH];           ///<��˰������or��˰�˹�˾����
//    BankingInfoType         BankingData[2];
    u8                      TaxerID[20];                    ///<˰��or��˰��ʶ���
    u8                      MachineNB[8];                   ///<�������
}InvoicePrintInfo;
///Structure for element of the daily transaction data
typedef struct{
    u8  CurDate[4];                     ///<��ǰ����                Big Endian
    u16 NomInvNB;                       ///<����������Ʊ����        Big Endian
    u16 RtnInvNB;                       ///<������Ʊ����            Big Endian
    u16 DscInvNB;                       ///<�����Ʊ����            Big Endian
                                        ///<���췢Ʊ����ͳ�ƽ��
    u8  TaxIndexs[TAX_INDEX_NB];        ///<˰��˰Ŀ������6B
    u32 NomInvAmt[TAX_INDEX_NB];        ///<������Ʊ�����ۼƽ��    Big Endian
    u32 RtnInvAmt[TAX_INDEX_NB];        ///<��Ʊ�����ۼƽ��        Big Endian
    u8  Sign[128];                      ///<����ǩ��
}DailyTransctionDataType;

///Structure for element of the tax box management data
typedef struct
{
    u8 TaxIndexsNb;					
    u8 TaxIndexs[TAX_INDEX_NB];         ///<˰����������

    ICCDataCollectCarryDataType DclData;                ///<��ǰ�걨����

    SingleInvoiceScrollUsageDataType CurInvScl;         ///<��ǰ��Ʊ����ϸ��Ϣ
    u32                              CurInvSclCurInvNB; ///<��ǰ��Ʊ��֮��ǰ��δʹ�õĵ�һ�ŷ�Ʊ�� Big endian

    DailyTransctionDataType          DayTscData;        ///<�ս�������

    SingleInvoiceScrollType          InvScl[TAX_INVOICE_SCROLL_DISTRIBUTED_NB]; ///<��ǰ���ַ���δ��¼��ķ�Ʊ����Ϣ
    SingleInvoiceScrollType          DispenseAbortData;
    u8  RandomPassword[8];                              ///<˰�ؿ����������
    u32 CurrNomInvTotalAmt;             ///<˰�ؿ�������Ʊ���ۼƽ��        Little Endian
    u32 CurrRtnInvTotalAmt;             ///<˰�ؿ���Ʊ���ۼƽ��            Little Endian

    u32 CurPrtingInvNB;                 ///<��ǰ���ڴ�ӡ�ķ�Ʊ��            Big Endian
    u8 DeclareType;                     ///<�걨����
    u8 DistributeIndex;                 ///<���һ�ηַ��ķ�Ʊ��λ��
}POSInterInfoType;

typedef struct
{
    u8 FiscalICCNB[8];                  ///<˰�ؿ����
    u8 EncryptSymbol;                   ///<���ݼ�����Կ��ʶ��
    u8 Cryptograph[32];                 ///<˰����������
    u8 MAC[4];                          ///<MAC2
}USERCARDINFOSFI01;

typedef struct
{
    u8 CardType;                        ///<�����ͱ�־
    u8 MachineNb;                       ///<˰���տ������
    u8 TaxerNB[8];                      ///<��˰�˱��
    u8 AppStartDate[4];                 ///<Ӧ����ʼ����            Big Endian
    u8 AppStopDate[4];                  ///<Ӧ����Ч���� }USERCARDINFOSFI02;
    u8 AppEdition;                      ///<Ӧ�ð汾
    u8 FCI;                             ///<�������Զ���FCI����
    u8 TaxerCompanyName[TAXER_PAYER_NAME_LENGH];            ///<��˰������or��˰�˹�˾����
}USERCARDINFOSFI02;

typedef struct
{
    u8 MachineNB[8];                    ///<�������
    u8 FiscalICCNB[8];                  ///<˰�ؿ����
    u8 SFI[2];                          ///<�ս��������ļ���ʶ��
}USERCARDINFOSFI09;

typedef struct
{
    u8  MachineNB[8];                   ///<�������
    u8  StartDate[4];                   ///<�걨������ʼ����	4 CCYYMMDD  Big Endian
    u8  StopDate[4];                    ///<�걨���ݽ�ֹ����	4 CCYYMMDD  Big Endian
    u16 Recordtotal;                    ///<��Ʊ��ϸ�ܼ�¼��        Big Endian
    u8  SFI[2];                         ///<��Ʊ��ϸ�����ļ���ʾ��
    u16 RecordStartNo;                  ///<��Ʊ��ϸ��ʼ��¼���    Big Endian
    u16 RecordEndNo;                    ///<��Ʊ��ϸ������¼���    Big Endian
    u8  XOR;                            ///<�������У��
}USERCARDINFOSFI10;

typedef struct
{
    u8  SFI[2];                         ///<��Ʊ��ϸ�����ļ���ʾ��
    u16 FileLen;                        ///<��Ʊ��ϸ�����ļ�����
}USERCARDINFOSFI11;

typedef struct
{
    u8  Flag;                                   ///<��Ʊ��ϸ���ݱ�־
    u16 Len;                                    ///<��Ʊ��ϸ���ݳ���

    u8  IssueInvoiceDate[4];                    ///<��Ʊ����      Big Endian
    u8  IssueInvoiceType;                       ///<��Ʊ����
    u32 InvoiceNB;                              ///<��Ʊ��        Big Endian
    u32 InvoiceAmount;                          ///<��Ʊ�ܽ��    Big Endian
    u8  TaxContralNB[8];                        ///<˰����
    u32 OriginalInvoiceNB;                      ///<ԭ��Ʊ��      Big Endian
    
    u8 InvoiceScrollCode[10];		    ///<��Ʊ����
    INVOICEDETAILINFO    InvoiceDetailInfo;
    
    u8  XOR;                                    ///<��Ʊ��ϸ�������У��
}USERCARDINFOINVOICEDETAIL;


typedef struct
{
    u8  InvoiceBlackFlag : 1;                   ///<�ڱ��Ƿ���
    u8  PrintDirection : 1;                     ///<����ӡ����
    u32 PageMoveStep;                           ///<�����ֽ����
}PRINTERINFO;

#pragma pack()
/**@}*/

#define SF_ADDR_INVOICE_SCROLL_INFO     0x50000000
#define SF_ADDR_INVOICE_PRINT_INFO      (SF_ADDR_INVOICE_SCROLL_INFO+POS_INVOICE_SCROLL_INFO_NB*sizeof(SingleInvoiceScrollUsageDataType))
#define SF_ADDR_DAILY_TRANSACTION_DATA  (SF_ADDR_INVOICE_PRINT_INFO+POS_INVOICE_PRINT_INFO_NB*sizeof(InvoiceCounterfoilType))
#define SF_ADDR_DECLARE_DATA            (SF_ADDR_DAILY_TRANSACTION_DATA+POS_DAILY_TRANSACTION_DATA_NB*sizeof(DailyTransctionDataType))

/**
 * @addtogroup TaxBox_PARAMETER
 * @ingroup TaxBox
 * @{
 */
//all on SPI Flash Data
#define POS_INVOICE_SCROLL_INFO_NB      300    ///<�洢����Ʊʹ�û������ݵ�����¼��
#define POS_INVOICE_PRINT_INFO_NB       5718   ///<�洢��Ʊ��ӡ���ݵ�����¼��
#define POS_DAILY_TRANSACTION_DATA_NB   (5 * 366)///<�洢�ս�����Ϣ������¼��
#define POS_DECLARE_DATA_NB             300    ///<�洢�걨���ݵ�����¼��


extern POSInitDataType POSInitData;             ///<the machine initialize info
extern POSUserInfoType POSUserInfo;             ///<the user info
extern TaxSpeciesTaxDetailsType POSTSTD[TAX_INDEX_NB];///<tax Species item info
extern POSInterInfoType POSInterInfo;           ///<the Tax Box management info

extern SingleInvoiceScrollUsageDataType * POSInvScl;    ///<max POSInvScl[POS_INVOICE_SCROLL_INFO_NB-1]
extern u32  POSInvSclNB;        ///<�Ѿ�ʹ�õ� ��ʷ����Ʊʹ�û������� ������

extern InvoiceCounterfoilType *POSInvPrt;               ///<max POSInvPrt[POS_INVOICE_PRINT_INFO_NB-1]
extern u32  POSInvPrtNB;        ///<�Ѿ�ʹ�õ� ��Ʊ��ӡ�Լ���ϸ��Ϣ ������

extern DailyTransctionDataType * POSDayData;            ///<max POSDayData[POS_DAILY_TRANSACTION_DATA_NB-1]
extern u32  POSDayDataNB;       ///<�Ѿ�ʹ�õ� �ս������� ������

extern DeclareDataLogType * POSDclData;                 ///<max POSDclData[POS_DECLARE_DATA_NB-1]
extern u32  POSDclDataNB;       ///<�Ѿ�ʹ�õ� �걨����log ������

extern u32  PowerLostPrintLineNum;
extern PRINTERINFO PrintMemoryInfo;
/**@}*/

extern bool Comm_Flag;
extern InvoiceCounterfoilType prtInfo;


/*all data related API*/
///Store the daily transaction data
bool storeDailyData(RTCTimeType *pCurrentTime);

/**
 * @addtogroup TaxBox_COMMAND_HANDLER
 * @ingroup TaxBox
 * @{
 */

/**
 * @defgroup Fiscal_Initialize
 * The module is initialize the Tax Box according to GB18240.2
 * @ingroup TaxBox_COMMAND_HANDLER
 * @{
 */
///Authentication ID from Fiscal Card and Tax Box
bool RightCardsInstered_Fiscal(void);
///Authentication ID from User Card and Tax Box
bool RightCardsInstered_User(void);

/**@}*/

/**
 * @defgroup Invoice_Rolls
 * The module provide integrated invoice rolls distribution and management function
 * @ingroup TaxBox_COMMAND_HANDLER
 * @{
 */

///get the index inside POSInterInfo.InvScl[] which is empty for usage
u8 getEmptyInvSclCell(void);
///whether exist appointed invoice scroll and return index
u8 existInvSclCell(u32 invoiceNB);
///load a new invoice scroll to the current invoice scroll uasge information
void loadNewInvScl(SingleInvoiceScrollType* InvScl);
///list the occupied invoice scroll on given buffer one SingleInvoiceScrollType by one SingleInvoiceScrollType (excluding u8 CellOccupied)
u16 listOccupiedInvSclCell(u8* buff);

/**@}*/

/**
 * @defgroup Issue_Invoice
 * @ingroup TaxBox_COMMAND_HANDLER
 * @{
 */
///Check Current invoice roll used out
bool curInvSclUsedOut(void);
///Adjust the invoice detail data
bool Adjust_Invoice_Detail(UARTIssInvParamType *pInputData, INVOICEDETAILINFO *pDetailData);
///Judge the tax item
bool checkTSTDLegal(INVOICEDETAILINFO* InpData);
///Search the Species tax item No
u8 getTSTD(u8 TaxIndex);
///Check exist of the account
bool checkSubAccountExist(UARTIssInvParamType* InpData);
///build input data of ICC command ISSUE_INVOICE
void buildICCIssInvCarryData(INVOICEDETAILINFO * InpData,
                             ICCIssInvCarryDataType* CryData, IssueInvCmdModeType mode);
///Save the invoice print data
void SaveInvoice(InvoiceCounterfoilType *pInputInfo);
///Update Current Invoice Roll data
void Data_UpdateCurInvScl(u32 amount, RTCTimeType *pRTCTime,
                          IssueInvCmdModeType mode);
///Update the daily transaction data
void updataDailyData_IssInv(INVOICEDETAILINFO* InpData,IssueInvCmdModeType mode);
/**@}*/


///Read the user information from user card, and update the relate data
bool UpdateUserInformation(u8 FromUserCardFlag);
/**@}*/


/**
 * @defgroup Tax_Management
 * @ingroup TaxBox_COMMAND_HANDLER
 * @{
 */
///Authentication safely by the fiscal management card
bool authFiscalManageCard(u8* CheckType, RTCTimeType *pCurrentTime, u8* pCheckLevel);
///Change the fiscal Management card
bool Change_Management(u8 CheckTypeOld);

u8  Data_CheckInvoiceContent(u8 *pContent, u32 *pInvoiceAmount);

/**@}*/
/**@}*/

///Search the single roll invoice usage data
bool getHistorySglInvSclInfo(u32 BeginNB,SingleInvoiceScrollUsageDataType* SclInfo);
///Ϊ��X3F_SubmitInvoiceData�ıȽϺ���
bool X3F_SubmitInvoiceData_COMPARE(u8* Parameter,InvoicePrintInfoType* prtData);
///Ϊ��X42_SubmitInvoicePrintingDataByInvoiceNBRange�ıȽϺ���
bool X42_SubmitInvoicePrintingDataByInvoiceNBRange_COMPARE(u8* Parameter,InvoicePrintInfoType* prtData);
///Ϊ��X43_SubmitInvoicePrintingDataByInvoiceTimeRange�ıȽϺ���
bool X43_SubmitInvoicePrintingDataByInvoiceTimeRange_COMPARE(u8* Parameter,InvoicePrintInfoType* prtData);
///Ϊ��X44_SubmitDailyTransactionDataByTimeRange�ıȽϺ���
bool X44_SubmitDailyTransactionDataByTimeRange_COMPARE(u8* Parameter,DailyTransctionDataType* dayData);
///�������������ҷ�Ʊ��ӡ����
void submitInvPrtDataOnCondition(u8* Parameter, bool (*cmp)(u8*,InvoicePrintInfoType*));
///���������������ս�������
void submitDailyDataOnCondition(u8* Parameter, bool (*cmp)(u8*,DailyTransctionDataType*));
#endif

