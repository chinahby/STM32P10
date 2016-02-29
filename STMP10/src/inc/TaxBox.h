/// @file   Taxbox.h
/// This is the Head file for module @ref TaxBox and @ref UART_Command
/// @author Travis Tu
/// @date   2006-Sep-1st


#ifndef _TAXBOX_H_
#define _TAXBOX_H_

#define BACKUPDATA _Pragma("location=\"BACKUP\"") __no_init

/**
 * @addtogroup TaxBox
 * @{
 */


/**
 * @defgroup TaxBox_CONFIGURATION
 * @ingroup TaxBox
 * @{
 */
#undef  NULL
#define NULL 0
//#define NOCSCheck             ///<Do not Check Sum on incoming Uart Command
//#define RUNINRAM              ///<All the application will be run in RAM
//#define USB_MOD                 ///<Use the USB communication
#define TaxICCardNOPPS          ///<No PPS during IC card ATR
#define WITHLASTMEM             ///<Restart the TAX Box With Last Memory
/**@}*/


#include "stm32f10x_type.h"
#include "stm32f10x_conf.h"
#include "stm32f10x_map.h"
#include "stm32f10x_lib.h"
#include "I2CRTC.h"
#include "UARTSC.h"
#include "SPIFlash.h"
#include "Mem.h"
#include "comm.h"
#include "Toolkit.h"
#include "Data.h"
#include "ICCmd.h"
#include "Comm.h"
#include "USBCom.h"
#include "Buzzer.h"
#include "DataFlash.h"
#include "USBHost.h"
#include "I2C.h"

///Structure of flags bit field set
typedef struct
{
    bool bRspWanted;         ///<��Ҫresponse
    bool bPowerLost;         ///<indicate the power lose
    bool bDclMngDataNoBack;  ///<�걨��������δ�ش�   TRUE:���걨δ��˰��FALSE:���걨����
    bool bBoxIsInitialized;  ///<��������ʼ����
    bool bDispenseAbortFlag; ///<��Ʊ�ַ��쳣�жϱ�־λ
    bool bDeclareAbortFlag;  ///<�걨�쳣�жϱ�־λ
    bool bPayTaxAbortFlag;   ///<��˰�쳣�жϱ�־λ
}AppFlags_t;

///Structure for element of UART Incomming Commands Table
typedef struct
{
    u8 InstructByte;            ///<the instruction Byte for this Uart Command
    void (*Cmd)(u16, u8* );     ///<this instruction command Handler
}UARTCmd_t;
/**
 * @addtogroup TaxBox_PARAMETER
 * @ingroup TaxBox
 * @{
 */
extern AppFlags_t AppFlag;      ///<bit fields for application flags
extern u16 UARTRspParamLength;  ///<Parameter's length which is pedding on Response buffer
/**@}*/


/**
 * @defgroup TaxBox_ERROR_CODE
 * Define the Tax Box error code in response
 * @ingroup TaxBox
 * @{
 */
#define UART_COMMAND_SUCCESS                                    0x00
#define UART_COMMAND_FISCAL_ICC_POWERON_ERR                     0x01    ///<˰�ؿ��ϵ����
#define UART_COMMAND_USER_ICC_POWERON_ERR                       0x02    ///<�û����ϵ����
#define UART_COMMAND_MANAGEMENT_ICC_POWERON_ERR                 0x03    ///<�����ϵ����
#define UART_COMMAND_NO_USER_ICC                                0x04    ///<û���û���
#define UART_COMMAND_NO_FISCAL_ICC                              0x05    ///<û��˰�ؿ�
#define UART_COMMAND_NO_FISCAL_MANAGEMENT_ICC                   0x06    ///<û�й���
#define UART_COMMAND_READ_FISCAL_ICC_ERROR                      0x07    ///<��ȡ˰�ؿ�����
#define UART_COMMAND_READ_USER_ICC_ERROR                        0x08    ///<��ȡ�û�������
#define UART_COMMAND_READ_FISCAL_MANAGEMENT_ICC_ERROR           0x09    ///<��ȡ��������
#define UART_COMMAND_WRITE_FISCAL_ICC_ERROR                     0x0A    ///<����˰�ؿ�����
#define UART_COMMAND_WRITE_USER_ICC_ERROR                       0x0B    ///<�����û�������
#define UART_COMMAND_WRITE_FISCAL_MANAGEMENT_ICC_ERROR          0x0C    ///<���¹�������
#define UART_COMMAND_ILLEGAL_FISCAL_ICC                         0x0D    ///<�Ƿ�˰�ؿ�
#define UART_COMMAND_ILLEGAL_USER_ICC                           0x0E    ///<�Ƿ��û���
#define UART_COMMAND_ILLEGAL_FISCAL_MANAGEMENT_ICC              0x0F    ///<�Ƿ�����
#define UART_COMMAND_TAX_BOX_NOT_INITIALIZED                    0x10    ///<˰�غл�δ��ʼ��
#define UART_COMMAND_ISSUER_INVOICE_DEADLINE_EXCEEDED           0x11    ///<��Ʊ�������ѹ���
#define UART_COMMAND_SINGLE_INVOICE_UP_LIMIT_EXCEEDED           0x12    ///<�ѳ������ŷ�Ʊ��Ʊ����޶�
#define UART_COMMAND_ISSUE_INVOICE_TOTAL_UP_LIMIT_EXCEEDED      0x13    ///<��Ʊ�ܶ����
#define UART_COMMAND_RETURN_INVOICE_TOTAL_UP_LIMIT_EXCEEDED     0x14    ///<��Ʊ�ܶ����
#define UART_COMMAND_CARD_MACHINE_DEFFER                        0x15    ///<˰�ؿ���ע�����˰������ע��Ų�һ��
#define UART_COMMAND_USER_CARD_MAC_ERR                          0x16    ///<�û���MAC��
#define UART_COMMAND_FISCAL_CARD_MAC_ERR                        0x17    ///<˰�ؿ�MAC��
#define UART_COMMAND_PIN_ERROR                                  0x18    ///<���PINʧЧ
#define UART_COMMAND_ILLEGAL_TAX_SPECIES_TAX_DETAILS            0x19    ///<�Ƿ�˰��˰Ŀ
#define UART_COMMAND_DECLARE_DATE_EXCEEDED_CURRENT              0x1A    ///<�걨��ֹ���ڴ��ڵ�ǰ����
#define UART_COMMAND_DECLARE_DATE_EXCEEDED_LAST                 0x1B    ///<�걨��ֹ����С���ϴ��걨����
#define UART_COMMAND_USER_CARD_LOCKED                           0x1C    ///<�û�������
#define UART_COMMAND_FISCAL_CARD_LOCKED                         0x1D    ///<˰�ؿ�����
#define UART_COMMAND_ALREADE_INIT                               0x1E    ///<˰�����Ѿ���ʼ��
#define UART_COMMAND_CHECK_SUM_ERROR                            0x1F    ///<У��λ����ȷ
#define UART_COMMAND_INSTRUCTION_TYPE_ERROR                     0x20    ///<�������ʹ���
#define UART_COMMAND_HEAD_ERROR                                 0x21    ///<����ͷ����
#define UART_COMMAND_COMMAND_LENGTH_ERROR                       0x22    ///<����ȴ���
#define UART_COMMAND_INVOICE_OVER                               0x23    ///<��Ʊ������
#define UART_COMMAND_INVOICE_MEMORY_ERR                         0x24    ///<��Ʊ�洢���쳣
#define UART_COMMAND_DATA_STORAGE_EXCEPTION                     0x25    ///<˰�ش洢���쳣
#define UART_COMMAND_DATE_OVERFLOW                              0x26    ///<ʱ�䡢���ڡ���ֵ���
#define UART_COMMAND_FISCAL_ICC_ALREADY_REGISTER                0x27    ///<˰�ؿ���ע��
#define UART_COMMAND_CARD_FULL_CHANGE                           0x28    ///<��������������
#define UART_COMMAND_UNUSERD_INVOICE_EMPTY                      0x29    ///<û���ѷַ���δʹ�õķ�Ʊ��
#define UART_COMMAND_NoMoreInvToDistribute                      0x2A    ///<�û������޷�Ʊ�ַ�
#define UART_COMMAND_ILLEGAL_SUB_ACCOUNT_NB                     0x2B    ///<�Ƿ��ֻ����
#define UART_COMMAND_NO_INVOICE_PRINT_INFO_FOUND                0x2C    ///<�˷�Ʊ�����������
#define UART_COMMAND_INVOICE_HAS_BEEN_RETURNED                  0x2D    ///<�˷�Ʊ������Ʊ����
#define UART_COMMAND_INVOICE_HAS_BEEN_DISCARDED                 0x2E    ///<�˷�Ʊ������Ʊ����
#define UART_COMMAND_ISSUE_INVOICE_FAIL                         0x40    ///<��Ʊʧ��
#define UART_COMMAND_NO_TAX_INDEXS                              0x41    ///<��˰��˰Ŀ��Ϣ
#define UART_COMMAND_DECLARE_DUTY_ERROR                         0x42    ///<�걨ʧ��
#define UART_COMMAND_FISCAL_MANAGEMENT_CARD_AUTHENTICATION_FAIL 0x43    ///<˰�������֤ʧ��
#define UART_COMMAND_SINGLE_INVOICE_SCROLL_INFO_NOT_FOUND       0x44    ///<����ط�Ʊ��ʹ����Ϣ
#define UART_COMMAND_NO_NEW_DECLARE                             0x45    ///<��ع��������ѻش�
#define UART_COMMAND_MANAGEMENT_DATA_TYPE_DIFFERE               0x46    ///<����˲鿨���Ͳ�һ��
#define UART_COMMAND_NO_LAST_DECLARE_DATA_YET                   0x47    ///<�޲�ѯ�걨����
#define UART_COMMAND_INPUT_DATE_ERR                             0x48    ///<��������ʱ�����
#define UART_COMMAND_NO_DAILY_TRANSACTION_INFO_FOUND            0x49    ///<������ս�������
#define UART_COMMAND_INPUT_INVOICE_FULL                         0x4A    ///<˰�ؿ����з�Ʊ���޷�¼��
#define UART_COMMAND_DISCARD_NUMBER_ERR                         0x4B    ///<��Ʊ�������
#define UART_COMMAND_FISCAL_CARD_NO_EXP                         0x4C    ///<˰�ؿ�������Ч����
#define UART_COMMAND_USER_CARD_NO_EXP                           0x4D    ///<�û���������Ч����
#define UART_COMMAND_INVOICE_ITEMS_NUM_EXCEEDED                 0x4E    ///<��Ʊ��Ŀ�������������
#define UART_COMMAND_ALREADY_DECLARE_RECORD                     0x4F    ///<������δ��ɵ��걨��¼
#define UART_COMMAND_INVOICE_FRACTION_LENGTH_ERR                0x50    ///<��Ʊ��Ŀ�����쳣
#define UART_COMMAND_DATA_SIGN_ERROR                            0x51    ///<����ǩ������
#define UART_COMMAND_CHECK_TYPE_ERROR                           0x52    ///<�˲�������
#define UART_COMMAND_FISCAL_MANAGEMENT_CARD_FULL                0x53    ///<˰���������
#define UART_COMMAND_USER_CARD_FULL                             0x54    ///<�û�������
#define UART_COMMAND_NO_DISTRIBUTED_INV_SCL                     0x55    ///<û�п�¼��ķ�Ʊ��
#define UART_COMMAND_FISCAL_CARD_TAXER_NAME_DIFFER              0x56    ///<˰�ؿ���˰�����Ʋ�һ��
#define UART_COMMAND_USER_CARD_TAXER_NAME_DIFFER                0x57    ///<�û�����˰�����Ʋ�һ��
#define UART_COMMAND_DISTRIBUTE_STORAGE_FULL                    0x58    ///<�ַ���Ʊ�洢������
#define UART_COMMAND_DAILY_MEMORY_FULL                          0x59    ///<�ս��״洢������
#define UART_COMMAND_USER_CARD_NO_FISCAL_CARD                   0x5A    ///<�û���������Ӧ˰�ؿ�����
#define UART_COMMAND_OVER_ITEMS_NUMBER                          0x5B    ///<������Ʊ�����Ŀ����
#define UART_COMMAND_DECLARE_DATE_EXCEEDED_CURRENT_RECORD       0x5C    ///<�걨��ֹ���ڴ��ڵ�ǰ�ս��׼�¼����
#define UART_COMMAND_POWER_LOST                                 0x5D    ///<�����쳣
#define UART_COMMAND_FISCALCODEERR                              0x5E    ///<˰�ؿ���Ŵ���
#define UART_COMMAND_CHANGECARDTIMEOUT                          0x5F    ///<������ʱ
#define UART_COMMAND_REQUESTINSERTUSB                           0x60    ///<�����USB
#define UART_COMMAND_INITIALZEUSBSTORAGE                        0x61    ///<U���쳣
#define UART_COMMAND_SELECTDECLAREDEVICE                        0x62    ///<ѡ���걨�豸
#define UART_COMMAND_CHANGEINVOICEDETAILCARD                    0x63    ///<����ϸ��
/**@}*/


/**
 * @addtogroup TaxBox_PARAMETER
 * @ingroup TaxBox
 * @{
 */
#define UART_CMD_PARAMETER_LENGTH 512                   ///<Uart Command Max parameter  Length
#define UART_RSP_PARAMETER_LENGTH 512                   ///<Uart Response Max Parameter Length
#define UART_RSP_PARAMETER_MAX_LENGTH_ONE_RSP (255-2-4) ///<Max Parameter length for each Response
/**@}*/


void TaxBoxInit();  ///<Tax Box Initializer
void TaxBoxMain(void);  ///<Tax Box Incomming UART or USB Commands Dispatcher

/**
 * @defgroup TaxBox_COMMAND_HANDLER
 * Define the function of Tax Box instruction
 * @ingroup TaxBox
 * @{
 */
///����
void X00_TryConnection(u16 ParameterLength, u8* Parameter);
///˰������ʼ��
void X30_TaxBoxInitialization(u16 ParameterLength, u8* Parameter);
///�ַ���Ʊ��
void X31_DistributeInvoiceNB(u16 ParameterLength, u8* Parameter);
///�ѷַ��ķ�Ʊ���ϴ�
void X32_SubmitDistributedInvoiceNB(u16 ParameterLength, u8* Parameter);
///��ǰ��Ʊ���ϴ�
void X33_SubmitCurrentInvoiceNB(u16 ParameterLength, u8* Parameter);
///��������Ʊ
void X34_IssueInvoice(u16 ParameterLength, u8* Parameter);
///��Ʊ
void X35_ReturnInvoice(u16 ParameterLength, u8* Parameter);
///��Ʊ
void X36_DiscardInvoice(u16 ParameterLength, u8* Parameter);
///�������������ϴ�
void X37_SubmitTaxBoxOriginalInfo(u16 ParameterLength, u8* Parameter);
///�û���Ϣ�ϴ�
void X38_SubmitUserInfo(u16 ParameterLength, u8* Parameter);
///˰��˰Ŀ��Ϣ�ϴ�
void X39_SubmitTaxSpeciesTaxDetails(u16 ParameterLength, u8* Parameter);
///����ʱ���ϴ�
void X3A_SubmitDateTime(u16 ParameterLength, u8* Parameter);
///����ʱ��
void X3B_SetTime(u16 ParameterLength, u8* Parameter);
///����Ʊʹ�û��������ϴ�
void X3C_SubmitSingleScrollInvoiceUsageaAssembleInfo(u16 ParameterLength, u8* Parameter);
///�걨���ݺͷ�Ʊ��ӡ����д��
void X3D_WriteCardWithDeclareDataAndInvoicePrintingData(u16 ParameterLength, u8* Parameter);
///�걨�����ϴ�
void X3E_SubmitDeclareData(u16 ParameterLength, u8* Parameter);
///��Ʊ�����ϴ�
void X3F_SubmitInvoiceData(u16 ParameterLength, u8* Parameter);
///��ع������ݻ���
void X40_SubmitSuperviseData (u16 ParameterLength, u8* Parameter);
///˰����״̬�ϴ�
void X41_SubmitTaxBoxStatus(u16 ParameterLength, u8* Parameter);
///����Ʊ��ֹ���ϴ���Ʊ��ӡ����
void X42_SubmitInvoicePrintingDataByInvoiceNBRange(u16 ParameterLength, u8* Parameter);
///��ʱ����ϴ���Ʊ��ӡ����
void X43_SubmitInvoicePrintingDataByInvoiceTimeRange(u16 ParameterLength, u8* Parameter);
///��ʱ����ϴ��ս�������
void X44_SubmitDailyTransactionDataByTimeRange(u16 ParameterLength, u8* Parameter);
///�˲�����д��
void X45_WriteCardWithCheckData(u16 ParameterLength, u8* Parameter);
///����ǩ��
void X46_DataSignature(u16 ParameterLength, u8* Parameter);
///���·ֻ���Ʊ����ļ�
void X47_UpdateUsersIssueInvoiceNB(u16 ParameterLength, u8* Parameter);
///¼�뷢Ʊ��
void X50_InputInvoiceNB(u16 ParameterLength, u8* Parameter);
///������ʼ��
void X51_ManufactureInit(u16 ParameterLength, u8* Parameter);
///����ڴ�
void X52_ReVergin(u16 ParameterLength, u8* Parameter);
///��Ʊ��ӡ��Ϣ����
void X53_InfoPrtInProgress(u16 ParameterLength, u8* Parameter);
///��Ʊ��ӡ���
void X54_InfoPrtFinished(u16 ParameterLength, u8* Parameter);
///
void X55_InquireUnfinishedInvData(u16 ParameterLength, u8* Parameter);
///��������-��ʼ��
void X56_PressureTestInit(u16 ParameterLength, u8* Parameter);
///��������-�ַ���Ʊ
void X57_PressureTestDistribute(u16 ParameterLength, u8* Parameter);
///��������-��Ʊ
void X58_PressureTestInvoice(u16 ParameterLength, u8* Parameter);
///IC���ϵ�
void X59_SmartCardPowerOn(u16 ParameterLength, u8* Parameter);
///IC���µ�
void X5A_SmartCardPowerOff(u16 ParameterLength, u8* Parameter);
///IC������
void X5B_SmartCardAPDUCommand(u16 ParameterLength, u8* Parameter);
///CPU�Լ�
void X5C_CPUSelfCheck(u16 ParameterLength, u8* Parameter);
///ROM�Լ�
void X5D_ROMSelfCheck(u16 ParameterLength, u8* Parameter);
///RAM�Լ�
void X5E_RAMSelfCheck(u16 ParameterLength, u8* Parameter);
///MEM�Լ�
void X5F_MEMSelfCheck(u16 ParameterLength, u8* Parameter);
///�����Լ�
void X60_SerialSelfCheck(u16 ParameterLength, u8* Parameter);
///ѭ���Լ�
void X61_CycleSelfCheck(u16 ParameterLength, u8* Parameter);
///������˰�˱���
void X62_UpdateTaxerName(u16 ParameterLength, u8* Parameter);
///�޸�����ʱ��
void X63_UpdateDateTime(u16 ParameterLength, u8* Parameter);
///����˰�����̼�
void X64_UpdataFirmware(u16 ParameterLength, u8* Parameter);
/**@}*/
/**@}*/



/**
 * @addtogroup USER_Device_Library
 * @{
 */
/**
 * @addtogroup COM
 * @{
 */
/**
 * @defgroup TaxBox_COM_RESPONSE_HANDLER
 * @ingroup COM
 * @{
 */
void SendRsp(void);             ///<Response On UART incoming Command immediatly
void Response_OK(u16 length);   ///<Response OK on Uart Incoming Command but not immediatly
void Response_ER(u8 errorCode); ///<Response ER on Uart Incomming Command but not immediatly
void Response_NP(u8 SN);        ///<Response NP on Uart Incomming Command but not immediatly
void SendRsp_WT(void);          ///<Response WT on Uart Incomming Command but not immediatly
void SendRspCancel(void);       ///<Cancel last registed Response Information
bool SendRsp_OK_Manual(u16 paraLength, u8 SN); ///<Manually Send One OK Response
bool SendRsp_CC(void);          ///<Manually Send One change card Response
/**@}*/
/**@}*/
/**@}*/

#endif /*_TAXBOX_H_*/
