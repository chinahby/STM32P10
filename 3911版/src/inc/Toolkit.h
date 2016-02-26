/// @file   Toolkit.h
/// This is for module @ref Toolkit
/// @author Travis Tu
/// @date   2006-Sep-1st

#ifndef _TOOLKIT_H_
#define _TOOLKIT_H_

#ifdef DEBUG
    #define assert(test)
#else
    #define assert(test)
#endif

#define Min(a,b) (a<b?a:b)
#define Max(a,b) (a<b?b:a)

#define SYSTEM_PLL_CLOCK 72 //MHz
///enumeration for the three LEDs
typedef enum
{

    BLUE_LED = 0x01,
    ORANGE_LED = 0x02,
    RED_LED = 0x03 ,
    GREEN_LED = 0x04
}LEDType;


///CRC const variable
extern const u16 CRC16Table[256];
///Peripheral Frequency
extern u32 PCLK2;
///Calulate CRC 16 IBM mode Polynomial Representations = 0x8005
void CalulateCRC16(const u8* data, u16 length, u8* cs);
///Wait for N u seconds
void WaitNuS(u32 N);
///Init the api for init the send wait process
void SendWaitInit(void);
///����APDU�����������ݵ����һ�����Ч���ֽ�
u8 ComputeLRC(const u8 *buf,u16 len);

/**
 * @addtogroup TaxBox_COMMAND_HANDLER
 * @ingroup TaxBox
 * @{
 */

/**
 * @defgroup Declare
 * @ingroup TaxBox_COMMAND_HANDLER
 * @{
 */
///Judge the user card have declare record
bool TOOLKIT_Judge_Declared(u8 *pStatus);
///�����������һ��
void TOOLKIT_DatePlusOneDay(unsigned char *pucDate);
///���㵱�µ����һ��
void TOOLKIT_MonthDay(unsigned char *pucDate, unsigned char *pucDay);

/**@}*/
/**@}*/

///����CS ���ҷ�������
void AddCSAndSendUARTRsp(u8* cmd, u8 length);
///Initialization the system CLK
void SYSCLOCKInit(void);
void NVIC_Configuration(void);
void SysTick_Config(void);
//void Set_USBClock(void);
///Setup the interrupt for PowerLost
void InitExtInt(void);
///flash the lamp on board for user
void Flashing(void);

//void USB_Interrupts_Disable(void);
//void USB_Interrupts_Config(void);
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
///����ʱ���Ƿ�Ϸ�
bool TOOLKIT_ValideTimeInt(u8 *pTime);
///BCD convention LONG
u32 TOOLKIT_BcdLong(u8 *pucSrc, u16 uiSrcLen);
void Protect_Switch(u8 State);
///Initialize the three LEDs
void LED_Init(void);
///Set LED show status
void LED_Show(LEDType LEDValue, u8 ShowType);
///Check the input data string
bool DATE_ValideDateStr(u8 *pDate);
void TOOLKIT_DateCompress(u8 *pPlainDate, u8 *pCompressDate);
u32 TOOLKIT_NumberCodeConv(u32 Data, u8 ResumeFlag);
void TOOLKIT_LongAsc(unsigned char  *pucDest, unsigned int uiLen, unsigned long pulSrc);
unsigned long TOOLKIT_AscLong(unsigned char *pucSrc, unsigned int uiLen);

#endif
