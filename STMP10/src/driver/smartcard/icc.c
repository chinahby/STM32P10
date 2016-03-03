#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h> 
#if 0
#include "icc.h"
#include "base.h"
#include "vosapi.h"
#endif
//#include "usip.h"
#include "KF701DH.h"

#include "..\..\inc\FunctionList.h"

#include  "base.h"
#include  "icc.h"
#include  "vosapi.h"
#include  "comm.h"
//#if 0

#define  SW1      asyncard_info[current_slot].sw1
#define  SW2      asyncard_info[current_slot].sw2 
#define  ICMAXBUF    300 //512->300

/********************************************************************
 ----------------------------ȫ�ֱ�������----------------------------
 ********************************************************************/ 

//volatile
static uchar   current_slot;

volatile ASYNCARD_INFO  asyncard_info[IC_MAXSLOT];
//static volatile  uchar  k_Icc_PR[3];
//static volatile  uchar  k_Icc_CR[3];  //reset
//static volatile  uchar  k_Icc_EISSR[3]; //clk
//static volatile  uchar  k_Icc_SR[3];  //io


/*���SAM�����õ�Դ��ʱ���ź�*/
static unsigned int                sam_vcc_used_flag = 0;
static unsigned int                sam_clk_used_flag = 0;
//static unsigned int                sam_slot_last_flag = 0;


//volatile emv_queue      	  scs_queue[ 1 ]; 

//static volatile uchar gbScsQbuf[10];
//static volatile uchar gbScsIp=0;

static   uchar    card_Outputdata[ICMAXBUF];

static   uchar    rstbuf[40];
volatile int      k_timeover,k_total_timeover; //�ַ���ʱ���ܳ�ʱ��־  

volatile static uchar   Parity_Check_Enable; // żУ����󵥸��ַ��ط������־  //
volatile static uchar   k_IccComErr;         // ͨ�Ŵ���                        //
volatile static uchar   k_IccErrPar;         // ��żУ��                        //
volatile static uchar   k_IccErrTimeOut;     // ������ʱ                        // 
volatile ulong    k_Ic_TimeCount;              //�ַ������ʱ�����м���� 
volatile ulong    k_IcMax_TimeCount;           //�ַ������ʱ����ֵ 
volatile ulong    k_Ic_TotalTimeCount;         //�ܳ�ʱ�����м���� 
volatile ulong    k_IcMax_TotalTimeCount;      //�ܳ�ʱ����ֵ 

//hard
//static volatile int               scs_stat;       /**/
volatile int                       twt_state;       /*��ʱ���*/
//static volatile int                wait_counter;/* �ȴ������� */ 
volatile unsigned int              scs_bwt;     /* ��ȴ�ʱ���'TS'�ȴ�ʱ�� */ 
volatile unsigned int              scs_cwt;     /* T=1 CWT and T=0 WWT */ 
volatile unsigned int              scs_cgt;     //guard
//static volatile int               work_step;    /*��Ƭ��λ�׶α��*/

#define SC_UART_WAIT_EVENT       ( 1 << 2 )
#define SC_UART_CLKS_EVENT       ( 1 << 3 )
#define SC_UART_RSTL_EVENT       ( 1 << 4 )
#define SC_UART_RSTH_EVENT       ( 1 << 5 )
#define SC_UART_IGDT_EVENT       ( 1 << 6 )

volatile unsigned int              scs_crg = 0; /* ������ƼĴ��� */
#define CONV_EN                  ( 1 << 0 )     /* ͨ���߼� */ 
#define CRPT_EN                  ( 1 << 1 )     /* �����ط�/�ձ�� */
#define TTSC_EN                  ( 1 << 2 )     /* TS�ַ��Ƿ��Ѿ��յ� */         
#define PARC_B                   ( 1 << 3 )     /* żУ��λ */
#define PARE_SB                  ( 1 << 4 )     /* У������� */

static volatile int               repeat_chc = 0; /*�����ط�����*/

volatile static uchar   WTX;
volatile static ulong   WWT,CWT,BWT; 



//static   uchar          vcc_mode;

static volatile uint8_t SCData = 0;

void s_IccDelayEtus_Send(int n_etus,int r_etu,int mend11,int mend12,int mend13);
void s_IccSaveIntr(void);
void Icc_SetWaitTime(void);
void Icc_StopWaitTime(void);
int GetAtr(uchar *Buff);
int  T1_Send_Block(uchar *Inbuf);
int  T1_Rece_Block(uchar *Outbuf);
int  T1_Send_Block(uchar *Inbuf);

void   Icc_Vcc(uchar channel,uchar mode);

/****************************
 -------   ��������  --------
 ****************************/ 
int   SelectSlot(uchar slot);
void ClearAtr(uchar Channel);
//static int  emv_hard_reset_signal( int channel, int level ); 
static void Icc_RST( uchar channel, uchar level );/*��Ƭ��λ�źſ���*/
static  void   Icc_Clk(uchar channel,uchar enable);
void  Dlynetu(ushort count);
void Dly42500CLK(void);
void s_IccRestoreIntr(void);
void Icc_Io(uchar channel,uchar mode);
static void USART3_ISR_Clr();
int T1_send_command(uchar nad,uchar *inbuf,uchar *outbuf);
uchar Rece_Byte_T1(void);
uchar Send_Byte(uchar dat);
void s_SetGTnetu(ushort count);
int T0_send_command(uchar Class,uchar Ins,uchar P1,uchar P2,uchar P3, uchar *Data_Buf,uchar *Out_Buf,uchar *rslt);
static void SC_Init(void);
int  T1_IFSD_command(uchar slot);

/**
  * @brief  Sets or clears the Smartcard reset pin.
  * @param  ResetState: this parameter specifies the state of the Smartcard 
  *   reset pin. BitVal must be one of the BitAction enum values:
  *     @arg Bit_RESET: to clear the port pin.
  *     @arg Bit_SET: to set the port pin.
  * @retval None
  */
void SC_Reset(BitAction ResetState)
{
#if 0
  if(ResetState==0)
  {
    
    GPIO_ResetBits(SC_PIN_RESET_GPIO,SC_PIN_RESET);
    
  }
  else
    GPIO_SetBits(SC_PIN_RESET_GPIO,SC_PIN_RESET);
#endif
  //ȡ��
  if(ResetState==Bit_RESET)
    GPIO_WriteBit(SC_PIN_RESET_GPIO, SC_PIN_RESET_SAM1, Bit_SET);
  else
    GPIO_WriteBit(SC_PIN_RESET_GPIO, SC_PIN_RESET_SAM1, Bit_RESET);
#if 0    
  if(current_slot==1)
    GPIO_WriteBit(SC_PIN_RESET_GPIO, SC_PIN_RESET, ResetState);
  else if(current_slot==2)
    GPIO_WriteBit(SC_PIN_RESET_GPIO, SC_PIN_RESET_SAM2, ResetState);
#endif    
}

void s_IccClkOut(void)
{
#if 0
    k_Icc_PR[0]=0;
    k_Icc_PR[1]=0;
    k_Icc_PR[2]=0;
    k_Icc_SR[0]=0;
    k_Icc_SR[1]=0;
    k_Icc_SR[2]=0;
    k_Icc_CR[0]=0;
    k_Icc_CR[1]=0;
    k_Icc_CR[2]=0;
    k_Icc_EISSR[0]=0;
    k_Icc_EISSR[1]=0;
    k_Icc_EISSR[2]=0; 
#endif
    //��λ���ܿ�����ģ�� 
}

/**
 * ���ܿ�������2����
 */
static void USART3_Config( )
{
	GPIO_InitTypeDef       GPIO_InitStructure;
	USART_InitTypeDef      USART_InitStructure;
	NVIC_InitTypeDef       NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART3, ENABLE );  // enable usart3 clock				   

	//AFIO->MAPR &= ~(1 << 3); 				 // no remap
	
	NVIC_InitStructure.NVIC_IRQChannel                  = USART3_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority       = 1;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd               = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
   
	GPIO_InitStructure.GPIO_Pin   = SC_USART_PIN_CK;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init( SC_USART_GPIO, &GPIO_InitStructure );
  
	GPIO_InitStructure.GPIO_Pin   = SC_USART_PIN_TX;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_OD;
	GPIO_Init( SC_USART_GPIO, &GPIO_InitStructure );		
	
	USART_SetPrescaler( SC_USART, 0x05 );// USART Clock 3.6MHz
#if 0
	USART_SetGuardTime( SC_USART, 0x12 );// USART Guard Time 12 Bit 
#endif  
	USART_SetGuardTime( SC_USART, 12 );// USART Guard Time 12 Bit 

	USART_InitStructure.USART_Clock      = USART_Clock_Disable;//*USART_Clock_Enable
	USART_InitStructure.USART_CPOL       = USART_CPOL_Low;
	USART_InitStructure.USART_CPHA       = USART_CPHA_1Edge;
	USART_InitStructure.USART_LastBit    = USART_LastBit_Enable;
	USART_InitStructure.USART_BaudRate   = FCLK_CARD / 372;
	USART_InitStructure.USART_WordLength = USART_WordLength_9b;
	USART_InitStructure.USART_StopBits   = USART_StopBits_1_5;
	USART_InitStructure.USART_Mode 	     = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_Parity     = USART_Parity_No;	//*USART_Parity_Even
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init( SC_USART, &USART_InitStructure );
#if 0	
	USART_ITConfig( SC_USART, USART_IT_IDLE, DISABLE ); //add
	USART_ITConfig( SC_USART, USART_IT_TXE, DISABLE ); //add
	USART_ITConfig( SC_USART, USART_IT_RXNE, DISABLE ); //add
	USART_ITConfig( SC_USART, USART_IT_TC, DISABLE ); //add
	USART_ITConfig( SC_USART, USART_IT_FE, DISABLE );//add 
	USART_ITConfig( SC_USART, USART_IT_PE, DISABLE ); //*ENABLE
#endif
	USART_ITConfig( SC_USART, USART_IT_PE, ENABLE ); //*ENABLE
        //USART_ITConfig(SC_USART, USART_IT_ERR, ENABLE);

	USART_Cmd( SC_USART, ENABLE );                  // Enable SC_BIG_UART
#if 0        
	USART_SmartCardNACKCmd( SC_USART, DISABLE ); // *Enable the NACK Transmission */ 	
#endif        
	USART_SmartCardNACKCmd( SC_USART, ENABLE ); // *Enable the NACK Transmission */ 	

	USART_SmartCardCmd( SC_USART, ENABLE );      // Enable the Smartcard Interface */
#if 0
        Set_IRQProcessFunc( USART2_IRQChannel, Usart2_Isr );
#endif        
	return;
}

/**
 * ============================ Ӳ����ʼ�� ================================
 */
int  emv_hard_init( void )
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
#if 0    
    /*��ʼ��SAM�����õ�Դ��ʱ�ӱ�־�ź�*/
    sam_vcc_used_flag  = 0;
    sam_clk_used_flag  = 0;    
    sam_slot_last_flag = 0;
#endif
	/*�������õ�������ʱ��*/
        /* Enable GPIO clocks */
        RCC_APB2PeriphClockCmd(SC_PIN_3_5V_GPIO_CLK | SC_PIN_RESET_GPIO_CLK |
                         SC_PIN_CMDVCC_GPIO_CLK | SC_USART_GPIO_CLK |
                         RCC_APB2Periph_AFIO, ENABLE);
                         
        /* Enable USART clock */
        RCC_APB1PeriphClockCmd(SC_USART_CLK, ENABLE);
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM5, ENABLE );
	
    /*���ö�ʱ���ж�ϵͳ*/
        TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
        
        NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQChannel;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
    
	/*���ܿ����û�����λ����ж�ʹ��ϵͳ��ʱ��ɨ�跽ʽ*/
	//;
	
	/*�����û����ź���*/
#if 0       
	GPIO_InitStructure.GPIO_Pin   = ICC_VCC_PIN;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init( ICC_GPIO_PORT_2, &GPIO_InitStructure );
	GPIO_SetBits( ICC_GPIO_PORT_2, ICC_VCC_PIN );

	GPIO_InitStructure.GPIO_Pin   = ICC_RST_PIN;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init( ICC_GPIO_PORT_2, &GPIO_InitStructure );
	GPIO_ResetBits( ICC_GPIO_PORT_2, ICC_RST_PIN );
   
    /*�û���Ƭ��λ����ź�,����γ�Jtag����һֱΪ�͵�ƽ*/
    //AFIO->MAPR &= ~( 7 << 24 );
    //AFIO->MAPR |=  ( 1 << 26 );
    
        GPIO_InitStructure.GPIO_Pin   = ICC_DET_PIN;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init( ICC_GPIO_PORT_1, &GPIO_InitStructure );
	GPIO_SetBits( ICC_GPIO_PORT_1, ICC_DET_PIN );
#endif    
	/*����SAM���ź�*/
	GPIO_InitStructure.GPIO_Pin   = SC_PIN_CMDVCC;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init( SC_PIN_CMDVCC_GPIO, &GPIO_InitStructure );
	GPIO_SetBits( SC_PIN_CMDVCC_GPIO, SC_PIN_CMDVCC );

	GPIO_InitStructure.GPIO_Pin   = SC_PIN_RESET_SAM1;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init( SC_PIN_RESET_GPIO, &GPIO_InitStructure );
	GPIO_ResetBits( SC_PIN_RESET_GPIO, SC_PIN_RESET_SAM1 );

    /*����SAM��Ƭѡ�ź�*/
    GPIO_InitStructure.GPIO_Pin   = SC_PIN_3_5V_SAM1;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init( SC_PIN_3_5V_GPIO, &GPIO_InitStructure );
	GPIO_ResetBits( SC_PIN_3_5V_GPIO, SC_PIN_3_5V_SAM1 );

    GPIO_InitStructure.GPIO_Pin   = SC_PIN_3_5V_SAM2;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init( SC_PIN_3_5V_GPIO, &GPIO_InitStructure );
	GPIO_ResetBits( SC_PIN_3_5V_GPIO, SC_PIN_3_5V_SAM2 );
#if 0    
    USART1_Config();
    USART2_Config();
#endif    
    USART3_Config();
#if 0    
    Set_IRQProcessFunc( USART1_IRQChannel, Usart1_Isr );
    Set_IRQProcessFunc( USART2_IRQChannel, Usart2_Isr );
    
    Set_IRQProcessFunc( TIM5_IRQChannel, Timer5_Isr );
#endif    
	return 0;
}


int s_IccInit(void)
{
    uchar i;
    //��ʼ�� CLK����첽ʱ��Ƶ���벨��  
    s_IccClkOut();  
#if 0    
    emv_hard_init();
#endif
    SC_Init();
    
    //��������е��첽��Ƭ��Ϣ
    for(i=0;i<IC_MAXSLOT;i++)
    {
        memset((uchar*)&asyncard_info[i] ,0, sizeof(ASYNCARD_INFO));
        Icc_Vcc(i,OFF);
        Icc_RST(i,LOW);
        
        Icc_Io(i,IN);
        
        Icc_Clk(i,DisEnable); 
        
        asyncard_info[i].D=1; 
    } 
 
 
    return 0;  
}

/*************************************************
 *˵������CHANNELͨ�������ĸ�λ�źŽ��п���
 *      MODE��   00---------LOW
 *               01---------HIGH
 *************************************************/
void   Icc_RST(uchar channel,uchar mode)
{
    BitAction bState;
    //ȡ��
    if(mode)
      bState = Bit_RESET;
    else
      bState = Bit_SET;
      
    switch(channel)
    {
      case 1:
      case 2:
        GPIO_WriteBit(SC_PIN_RESET_GPIO, SC_PIN_RESET_SAM1, bState);
        break;
#if 0        
      case 2:
        GPIO_WriteBit(SC_PIN_RESET_GPIO, SC_PIN_RESET_SAM2, ResetState);
        break;
#endif        
        
      default: 
        break;
    }
}

#if 0
/*************************************************
 *˵������CHANNELͨ�������ĵ�Դ���п���
 *      MODE��   00     �µ�------OFF
 *               01     �ϵ�------ON
 *************************************************/
void   Icc_Vcc(uchar channel,uchar mode)
{
    switch(channel)
    {
      case 1:
      case 2:
        if(mode==ON)
        {
          GPIO_SetBits(SC_PIN_CMDVCC_GPIO, SC_PIN_CMDVCC);
          
        }
        else
          GPIO_ResetBits(SC_PIN_CMDVCC_GPIO, SC_PIN_CMDVCC);
        break;
      default:
        break;
    }  
}
#endif

/**
  * @brief  Initializes all peripheral used for Smartcard interface.
  * @param  None
  * @retval None
  */
static void SC_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
  //USART_ClockInitTypeDef USART_ClockInitStructure;  
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Enable GPIO clocks */
  RCC_APB2PeriphClockCmd(SC_PIN_3_5V_GPIO_CLK | SC_PIN_RESET_GPIO_CLK |
                         SC_PIN_CMDVCC_GPIO_CLK | SC_USART_GPIO_CLK |
                         RCC_APB2Periph_AFIO, ENABLE);
                         
  /* Enable USART clock */
  RCC_APB1PeriphClockCmd(SC_USART_CLK, ENABLE);
#if 0
//#ifdef USE_STM3210C_EVAL
  /* Enable the USART3 Pins Software Full Remapping */
  GPIO_PinRemapConfig(GPIO_FullRemap_USART3, ENABLE);
//#endif
#endif
  
  /* Configure USART CK pin as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = SC_USART_PIN_CK;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(SC_USART_GPIO, &GPIO_InitStructure);
  
  /* Configure USART Tx pin as alternate function open-drain */
  GPIO_InitStructure.GPIO_Pin = SC_USART_PIN_TX;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
  GPIO_Init(SC_USART_GPIO, &GPIO_InitStructure);

  /* Configure Smartcard Reset pin */
  GPIO_InitStructure.GPIO_Pin = SC_PIN_RESET_SAM1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(SC_PIN_RESET_GPIO, &GPIO_InitStructure);

  /* Configure Smartcard 3/5V pin */
  /*����SAM��Ƭѡ�ź�*/
  GPIO_InitStructure.GPIO_Pin = SC_PIN_3_5V_SAM1|SC_PIN_3_5V_SAM2;
  GPIO_Init(SC_PIN_3_5V_GPIO, &GPIO_InitStructure);

  /* Configure Smartcard CMDVCC pin */
  GPIO_InitStructure.GPIO_Pin = SC_PIN_CMDVCC;
  GPIO_Init(SC_PIN_CMDVCC_GPIO, &GPIO_InitStructure);

  /* Enable USART IRQ */
  NVIC_InitStructure.NVIC_IRQChannel = SC_USART_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  
  NVIC_Init(&NVIC_InitStructure);
  
  //add
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM5, ENABLE );
	
    /*���ö�ʱ���ж�ϵͳ*/
        TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
        
        NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQChannel;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
  
/* SC_USART configuration ----------------------------------------------------*/
  /* SC_USART configured as follow:
        - Word Length = 9 Bits
        - 0.5 Stop Bit
        - Even parity
        - BaudRate = 9677 baud
        - Hardware flow control disabled (RTS and CTS signals)
        - Tx and Rx enabled
        - USART Clock enabled
  */

  /* USART Clock set to 3.6 MHz (PCLK1 (36 MHZ) / 10) */
  USART_SetPrescaler(SC_USART, 0x05);
  
  /* USART Guard Time set to 16 Bit */
  USART_SetGuardTime(SC_USART, 12);
#if 0  
  USART_ClockInitStructure.USART_Clock = USART_Clock_Enable;
  USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
  USART_ClockInitStructure.USART_CPHA = USART_CPHA_1Edge;
  USART_ClockInitStructure.USART_LastBit = USART_LastBit_Enable;
  USART_ClockInit(SC_USART, &USART_ClockInitStructure);
#endif
  
  USART_InitStructure.USART_Clock = USART_Clock_Enable;
  USART_InitStructure.USART_CPOL = USART_CPOL_Low;
  USART_InitStructure.USART_CPHA = USART_CPHA_1Edge;
  USART_InitStructure.USART_LastBit = USART_LastBit_Enable;
  
  USART_InitStructure.USART_BaudRate = 9677;
  USART_InitStructure.USART_WordLength = USART_WordLength_9b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1_5;
  USART_InitStructure.USART_Parity = USART_Parity_Even;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_Init(SC_USART, &USART_InitStructure); 

  /* Enable the SC_USART Parity Error Interrupt */
  USART_ITConfig(SC_USART, USART_IT_PE, ENABLE);

  /* Enable the SC_USART Framing Error Interrupt */
  USART_ITConfig(SC_USART, USART_IT_ERR, ENABLE);

  /* Enable SC_USART */
  USART_Cmd(SC_USART, ENABLE);

  /* Enable the NACK Transmission */
  USART_SmartCardNACKCmd(SC_USART, ENABLE);

  /* Enable the Smartcard Interface */
  USART_SmartCardCmd(SC_USART, ENABLE);
  
  /* Set RSTIN HIGH */  
  //SC_Reset(Bit_SET);
  SC_Reset(Bit_RESET);
  
#if 0 
  /* Select 5V */ 
  SC_VoltageConfig(SC_VOLTAGE_5V);
#endif
#if 0  
  /* Disable CMDVCC */
  SC_PowerCmd(DISABLE);
#endif
  
  Icc_Vcc(current_slot,OFF);
 
  
}

/***********************************************
   ����Ƭ��ռ�õĶ�ʱ���ָ���ʹ��ǰ��״̬
************************************************/
void s_IccRestoreIntr(void)
{
    //��ָ������жϣ��簴����ͨ�ţ���ʱ
    //�ж�ʹ�� 
 
 
} 

int IccColdRest(uchar Channel)
{
    int ret;//,i;
    current_slot=Channel;
    //Clear all datas in SIMIF Device Reception FIFO
    s_IccSaveIntr(); 
    ClearAtr(Channel);
#if 0
    SC_Init();
#endif    
    Icc_RST(Channel,LOW);
    
    Icc_Vcc(Channel,ON); //ON
   

    
    Lib_DelayMs(10);
    Icc_Io(Channel,OUT);
    
    Icc_Io(Channel,IN); 
    
    Icc_Clk(Channel,Enable); 
    
    Dly42500CLK();
    
    
    Icc_RST(Channel,HIGH);
     
//test
#if 0     
     for(;;)
     {
      Icc_RST(Channel,LOW);
      Lib_DelayMs(1);
      Icc_RST(Channel,HIGH);
      Lib_DelayMs(2);
      
     }
#endif
     
#if 0     
    scs_stat         =   SC_UART_WAIT_TS | SC_UART_RECV_EV;         	    	
    work_step        =   SC_UART_CLKS_EVENT;                       
    USART3_ISR_Clr();			
    USART_ITConfig( SC_SML_UART, USART_IT_TC, DISABLE );		
    USART_ITConfig( SC_SML_UART, USART_IT_RXNE, ENABLE );   
#endif     
     trace_debug_printf("GetAtr");
     
    ret = GetAtr(rstbuf);
    s_IccRestoreIntr();
    return ret;    
}

static uchar USART_ByteReceive(uint8_t *Data, uint32_t TimeOut)
{
  uint32_t Counter = 0;

  //while((USART_GetFlagStatus(SC_USART, USART_FLAG_RXNE) == RESET) && (Counter != TimeOut))
  while(1)
  {
    
    if(USART_GetFlagStatus(SC_USART, USART_FLAG_RXNE) != RESET )
      break;

#if 0    
    if(gbScsIp)
    {
      //test
      trace_debug_printf("gbScsIp[%d]",gbScsIp);
      
      break;
    }
#endif
    
    if( (k_timeover==1)||(k_total_timeover==1) || k_IccErrPar)
    {
      Counter = TimeOut;
      break;
    }
  }

  if(Counter != TimeOut)
  {
#if 0
    if(asyncard_info[current_slot].TS==0xff)//receive TS
    {
      *Data =gbScsQbuf[0];
    }
    else
#endif      
      *Data = (uint8_t)USART_ReceiveData(SC_USART);
#if 0
    *Data = gbScsQbuf[0];
    gbScsIp--;
    if(gbScsIp<0)
      gbScsIp = 0;
#endif    
    return SUCCESS;    
  }
  else 
  {
      k_IccComErr=1;
      k_IccErrTimeOut=1;
      //k_IccErrPar = 0;
    
      return 1;
  }
}


uchar Rece_Byte(void)
{
  uchar combuffer; 
  uchar slot;
  k_IccComErr=0;
  k_IccErrTimeOut=0;
  k_IccErrPar=0;
  k_timeover=0;
  k_Ic_TimeCount=0x00;
  combuffer=0; 
    
  slot=current_slot;
  
    Icc_Io(slot,IN);
    //test
#if 0    
    USART_ITConfig( SC_SML_UART, USART_IT_RXNE, ENABLE );  	   
#endif
    k_IccComErr=0;
    k_IccErrTimeOut=0;
    k_IccErrPar=0;
    k_timeover=0;
    k_Ic_TimeCount=0x00;
    combuffer=0; 

    
    Icc_SetWaitTime();
    
  if((USART_ByteReceive(&combuffer, SC_RECEIVE_TIMEOUT)) != SUCCESS)
  {
    k_IccComErr=1; 
    k_IccErrTimeOut=1; 
//test
    trace_debug_printf("333");
    
    Icc_StopWaitTime();

    
    return 0xff;
  }
#if 0  
//test
    trace_debug_printf("444rece[%02x]",combuffer);
#endif
  Icc_StopWaitTime();  

    if(asyncard_info[slot].TS==0xff)
    {
        if(combuffer==0x03){
            asyncard_info[slot].TS=0x3f;
            combuffer=0x3f; 
            //s_sc_waitack(channel); 
            //k_Icc_CR[slot] |= USIP_SCI_CR_CONV;
            //Mips_write8(cr,k_Icc_CR[slot]);  
        }
        else if(combuffer==0x3b){
            asyncard_info[slot].TS=0x3b;
        }
        else k_IccComErr=1;
    } 
    
    return combuffer;   
  
}

/*******************************************************
 *  ��ȡIC���ĸ�λӦ���ź�ATR��
 *  ����ֵ��0X00------�ɹ���
 *          0XFF------ͨ�Ŵ���
 *          BUFF------��Ϣ���ȣ�BUFF[0]��+��λӦ����Ϣ��
 *
 *          ICC_ ATR_TSERR          ������Լ������
 *          ICC_ATR_TCKERR          ��λУ�����
 *          ICC_ATR_TIMEOUT         ��λ�ȴ���ʱ
 *          ICC_ATR_TA1ERR          TA1����
 *          ICC_ATR_TA2ERR          TA2����
 *          ICC_ATR_TA3ERR          TA3����
 *          ICC_ATR_TB1ERR          TB1����
 *          ICC_ATR_TB2ERR          TB2����
 *          ICC_ATR_TB3ERR          TB3����
 *          ICC_ATR_TC1ERR          TC1����
 *          ICC_ATR_TC2ERR          TC2����
 *          ICC_ATR_TC3ERR          TC3����
 *          ICC_ATR_TD1ERR          TD1����
 *          ICC_ATR_TD2ERR          TD2����
 *          ICC_ATR_LENGTHERR       ATR���ݳ��ȴ���
 *******************************************************/
int GetAtr(uchar *Buff)
{
     //uchar   *ptr_atr;
     uchar   T,edc,TCK;
     uchar   i,Flag,x;
     uchar  Lenth=2;
     uchar   *Lenth_Point;
  
     edc=0;T=0;
     
     asyncard_info[current_slot].TA=0;
     asyncard_info[current_slot].TB=0;
     asyncard_info[current_slot].TC=0;
     asyncard_info[current_slot].TD=0;
     asyncard_info[current_slot].TS=0xff;

     Lenth_Point=Buff;

     Parity_Check_Enable=0x00; /* �ڸ�λӦ���н�ֹżУ������ط� */
     *Buff=0x00;

     k_Ic_TotalTimeCount=0;
     k_IcMax_TotalTimeCount=0;
     k_total_timeover=0x00;

     //k_IcMax_TimeCount=11;   //12*(TB2*10)=10.28MS 11*(TB2*10)=9.52MS 
     k_IcMax_TimeCount=100;//��Է�EMV��Ƭ�����ӵȴ�TS��ʱʱ�� 
     // ���õȴ�TS�ַ���ʱ������42000CLK=113etu  //     
     //test
     trace_debug_printf("rece ts");
     
     *++Buff=Rece_Byte(); 
     
     if(k_IccComErr){   //receive TS of ATR
          if(k_IccErrTimeOut==1)  return ICC_ATR_TIMEOUT;
          else if(k_IccErrPar==1) return ICC_PARERR;
          else return ICC_ATR_TSERR;
     }
     edc=0;
     //ptr_atr=Buff;
     k_IcMax_TimeCount=1010;
     k_IcMax_TotalTimeCount=2020;

     // ��ʼ�����ܳ�ʱ�������� 19200ETU���ַ������ʱ���� 9600ETU  //
     *++Buff=Rece_Byte();
     if(k_IccComErr){    //receive T0 of ATR
          if(k_IccErrTimeOut==1)  return ICC_ATR_TIMEOUT;
          else  return ICC_PARERR;
     }
     edc=edc^*Buff;
     Flag=*Buff;
     x=Flag&0x0f;

     i = 1;
     for(;;){
          if((Flag&0x10)==0x10){  // TA
               *++Buff=Rece_Byte();
               if(k_IccComErr){
                    if(k_IccErrTimeOut)  return ICC_ATR_TIMEOUT;
                    else  return ICC_PARERR;
               }
               if(i==1){
                    asyncard_info[current_slot].TA1 = *Buff;
                    asyncard_info[current_slot].TA|=0X01;
               }
               else if(i==2){
                    asyncard_info[current_slot].TA2 = *Buff;
                    asyncard_info[current_slot].TA|=0X02;
               }
               else if(i==3){
                    asyncard_info[current_slot].TA3 = *Buff;
                    asyncard_info[current_slot].TA|=0X04;
               }
               else    asyncard_info[current_slot].TA4 = *Buff;
               edc=edc^*Buff;
               Lenth++;
          }
          if((Flag&0x20)==0x20){   //TB
               *++Buff=Rece_Byte();
               if(k_IccComErr){
                    if(k_IccErrTimeOut)  return ICC_ATR_TIMEOUT;
                    else  return ICC_PARERR;
               }
               if(i==1){
                    asyncard_info[current_slot].TB1 = *Buff;
                    asyncard_info[current_slot].TB|=0X01;
               }
               else if(i==2){
                    asyncard_info[current_slot].TB2 = *Buff;
                    asyncard_info[current_slot].TB|=0X02;
               }
               else if(i==3) {
                    asyncard_info[current_slot].TB3 = *Buff;
                    asyncard_info[current_slot].TB|=0X04;
               }
               else asyncard_info[current_slot].TB4 = *Buff;
               edc=edc^*Buff;
               Lenth++;
          }
          if((Flag&0x40)==0x40){ // TC
               *++Buff=Rece_Byte();
               if(k_IccComErr){
                    if(k_IccErrTimeOut)  return ICC_ATR_TIMEOUT;
                    else  return ICC_PARERR;
               }
               if(i==1){
                    asyncard_info[current_slot].TC1 = *Buff;
                    asyncard_info[current_slot].TC|=0X01;
               }
               else if(i==2){
                    asyncard_info[current_slot].TC2 = *Buff;
                    asyncard_info[current_slot].TC|=0X02;
               }
               else if(i==3){
                    asyncard_info[current_slot].TC3 = *Buff;
                    asyncard_info[current_slot].TC|=0X04;
               }
               else asyncard_info[current_slot].TC4 = *Buff;
               edc=edc^*Buff;
               Lenth++;
          }
          if((Flag&0x80)==0x00){  //  no TD
               for(i=0;i<x;i++){
                    *++Buff=Rece_Byte();
                    if(k_IccComErr){
                         if(k_IccErrTimeOut)  return ICC_ATR_TIMEOUT;
                         else  return ICC_PARERR;
                    }
                    edc=edc^*Buff;
               }
               Lenth=Lenth+x;

               if(T){  //���ΪT<>1Э�鿨Ƭһ����TCK
                    *++Buff=Rece_Byte();
                    if(k_IccComErr){
                         if(k_IccErrTimeOut==1)  return ICC_ATR_TIMEOUT;
                         else  return ICC_PARERR;
                    }
                    TCK=*Buff;
                    if(edc!=TCK)   return ICC_ATR_TCKERR;
                    Lenth++;
               }
               *Lenth_Point=Lenth;
               if(current_slot==0){ 

               }
               else {
                    
 
                    if((asyncard_info[current_slot].TA&0X02)==0X00) asyncard_info[current_slot].TA1=0x11; 


                    if(asyncard_info[current_slot].TA1==0x11)
                         asyncard_info[current_slot].D=1;
                    else if(asyncard_info[current_slot].TA1==0x12)
                         asyncard_info[current_slot].D=2;
                    else if(asyncard_info[current_slot].TA1==0x13)
                         asyncard_info[current_slot].D=4;
                    else {
                         asyncard_info[current_slot].TA1=0x11;
                         asyncard_info[current_slot].D=1;
                    }
    
                    if(asyncard_info[current_slot].TD&0X01){
                         if((asyncard_info[current_slot].TD1 & 0x0f)>1) return ICC_ATR_TD1ERR;
                         if(asyncard_info[current_slot].TD1 & 0x0f){
                              if(((asyncard_info[current_slot].TD&0X02)==0X02)&&((asyncard_info[current_slot].TD2 & 0x0f)!=0x01)) return ICC_ATR_TD2ERR;
                         }
                         else{
                              if((asyncard_info[current_slot].TD & 0x02) && ((asyncard_info[current_slot].TD2 & 0x0f)!=0x01)
                                                  &&((asyncard_info[current_slot].TD2 & 0x0f)!=0x0e))
                              return ICC_ATR_TD2ERR;
                         }
                    }

                    if(!asyncard_info[current_slot].TC2)  
                         asyncard_info[current_slot].TC2=0x0a;


                    if(!(asyncard_info[current_slot].TA & 0X04)) asyncard_info[current_slot].TA3=0x20;

                    asyncard_info[current_slot].IFSC=asyncard_info[current_slot].TA3;
               }


               return SUCCESS;
          }
          if(Flag & 0x80){ //  TD1  TD2
               *++Buff=Rece_Byte();
               if(k_IccComErr){
                    if(k_IccErrTimeOut==1)  return ICC_ATR_TIMEOUT;
                    else  return ICC_PARERR;
               }
               if(i==1){
                    asyncard_info[current_slot].TD1=*Buff;
                    asyncard_info[current_slot].TD|=0X01;
                    T=*Buff&0x0f;
               }
               else if(i==2){
                    asyncard_info[current_slot].TD2=*Buff;
                    asyncard_info[current_slot].TD|=0X02;
                    T|=*Buff&0x0f;  
               }
               else{
                    asyncard_info[current_slot].TD3=*Buff;
                    T|=*Buff&0x0f;
               }

               edc=edc^*Buff;
               Lenth++;
               Flag=*Buff;
          }
          i++;
     }     
}

/**********************************************
 *  ��λSLOTͨ����IC����������ATR��λӦ���źţ�
 *  BUFFER------��λӦ���ź�
 *  ����ֵ��0X00----�ɹ���
 *          SLOTERR---ѡ��ͨ������
 *          0XFF----ͨ��ʧ��
 ***********************************************/
int IccHotReset(uchar slot,uchar *Buffer)
{
     int ret;
     ClearAtr(slot);
     s_IccSaveIntr();
     s_IccDelayEtus_Send(2,0,0,0,0);
     Icc_RST(slot,LOW);
     Dly42500CLK();
     Icc_RST(slot,HIGH);
     ret = GetAtr(Buffer);
     s_IccRestoreIntr();
     return ret;
}

/****************************************
 *  �Կ���IC����IO��д��һ��״ֵ̬
 ****************************************/
void   Write_Io(uchar channel,uchar status)
{ 
}

/*************************************************
 *  �ر�SLOTͨ��IC�������ָ���һ��ͨ�������趨��
 *  ��������״̬��RST=LOW��CLK=LOW��IO=LOW��VCC=OFF
 *************************************************/
int Lib_IccClose(uchar slot)
{
  //test
  
     int i;
     slot=slot&0x7f;
     
     slot=slot&0xbf;   
     
     if(slot>=IC_MAXSLOT)
          return ICC_SLOTERR;
     for(i=0;i<200;i++);   
     Icc_RST(slot,LOW);
     for(i=0;i<100;i++);
     Icc_Clk(slot,DisEnable);
     Icc_Io(slot,OUT);
     Write_Io(slot,LOW);
     for(i=0;i<8000;i++);
     Icc_Vcc(slot,OFF);
     asyncard_info[slot].open =VCC_CLOSE; 
     return SUCCESS;
     
}

//test
void delayetu(int nCout)
{
        k_Ic_TimeCount=0;
        k_IcMax_TimeCount = nCout;
        k_timeover = 0;
        //Icc_SetWaitTime();
    	//TIM_Cmd( TIM5, ENABLE );
        Icc_RST(1,LOW);
        
        for(;;)
        {
          if( k_timeover)
          {
            //Icc_StopWaitTime();
        //Icc_RST(1,HIGH);
            
            //TIM_Cmd( TIM5, DISABLE );
            
            break;
          }
        }
  
}

/*********************************************************
 * ��ʼ��SLOTͨ����IC���������Կ��ϵ磬��λ���жϿ���Э�����ͣ�
 * ��T=1��T=0��������ATR����λӦ�𣩶���*ATRָ���⡣
 *  ����ֵ��0X00-----�ɹ���ʼ����
 *          0X01-----ѡ��SLOTͨ��ʧ�ܣ�
 *          0X02-----���γ�
 *          0X06-----��Э�鲻����
 *          0XFF-----ͨ�Ż�λ����

 *          slot - ��Ҫ��ʼ����ͨ����
 **********************************************************/
int Lib_IccOpen(uchar slot,uchar VCC_Mode,uchar *ATR)
{
    //uchar *ptr;
    int ret;
    //test
    //uchar i;
#if 0    
    //test
    ret=SelectSlot(slot);
    s_IccInit();
    for(;;);
#endif    
  
  //��ʱ ������
    s_IccInit();

    slot=slot&0x7f; 
    ret=SelectSlot(slot);
    if(ret) 
      return ICC_SLOTERR;  
    
#if 0
//test
  USART_TOV_Timer(1,340);
  //TIM_Cmd( TIM5, DISABLE );
  
  for(;;)
  {
        //Icc_RST(slot,LOW);
        delayetu(10);
        //Lib_DelayMs(1);
        //Icc_RST(slot,HIGH);
        delayetu(20);
        //Lib_DelayMs(2);

  }   
#endif
  
    if((VCC_Mode!=CRD_VCC_5V)&&(VCC_Mode!=CRD_VCC_3V)&&(VCC_Mode!=CRD_VCC_1_8V))
        return ICC_VCCMODEERR;
    
    if(ATR==NULL)
        return ICC_PARAMETERERR;
    if(asyncard_info[slot].open ==1)
    {
        Lib_IccClose(slot);
        //nDelayMs(200);
        Lib_DelayMs(200);
    }

    //test
    Lib_IccClose(slot);
    Lib_DelayMs(200);
    
#if 0
    if(slot==0)
    {
        Lib_IccCheck(slot);   //del 2007/12/21
        if(asyncard_info[slot].existent==0x00)
            return ICC_CARD_OUT;
        //nDelayMs(200);        //add 2007/12/21
    }
#endif
    
    memset(rstbuf,0x00,sizeof(rstbuf));
    //vcc_mode=VCC_Mode;    // WLB
    asyncard_info[slot].T=0;
    asyncard_info[slot].resetstatus=0x00;
      //test
      trace_debug_printf("IccColdRest begin\n");
      
    ret=IccColdRest(slot);
    asyncard_info[slot].resetstatus=0x01;    
    if (ret)
    {
      //test
      trace_debug_printf("IccColdRest[%d]\n",ret);
      
        //s_UartPrint("reset ret=%d %02x %02x %02x %02x %02x %02x",ret,rstbuf[0],rstbuf[1],rstbuf[2],rstbuf[3],rstbuf[4],rstbuf[5]);
        //s_UartPrint(" %02x %02x %02x %02x %02x %02x \r\n",rstbuf[6],rstbuf[7],rstbuf[8],rstbuf[9],rstbuf[10],rstbuf[11]);
        if((ret!=ICC_ATR_TSERR)&&(ret!=ICC_ATR_TIMEOUT)&&(ret!=ICC_PARERR)&&(ret!=ICC_ATR_TCKERR))          // ����TS���󣬳�ʱ��żУ�顢TCK�������������д����ͷ�����
        {
            ret=IccHotReset(slot,rstbuf);
            Icc_StopWaitTime();
        }
        asyncard_info[slot].resetstatus=0x02;
    }
    if (ret)
    {
        Dlynetu(5);
        Lib_IccClose(slot);
        return ret;
    }
    // RSTBUF��ʽ:����+TS+TO+TA......
    memcpy(ATR,rstbuf,rstbuf[0]+1);
    asyncard_info[slot].T=0;
    //ptr=&rstbuf[3];
    asyncard_info[slot].term_pcb=0x00;
    if((asyncard_info[slot].TD1&0x0f)==0x01)
    {
        asyncard_info[slot].T=1;
    }
    else if((asyncard_info[slot].TD1&0x0f)==0x00)
    {
        asyncard_info[slot].T=0;
    }
    else
        return ICC_PROTOCALERR;
    asyncard_info[slot].open =1;
    if(asyncard_info[slot].T==1)
    {
        //Ԥ�ô�IC�����յ�I����ȷ��PCB�����֣�˳�����1
        asyncard_info[slot].card_pcb=0x40; 
 
        //����T=1��Ƭ���������IFSC�����������ֽ��ճ�ʱ���ҷ��������ҡ���������
        s_IccSaveIntr();
//#if 0    //!!!    
        ret=T1_IFSD_command(current_slot);
//#endif        
        s_IccRestoreIntr();
        return ret;
    }
#if 0   
    //test
    trace_debug_printf("ATR:");
    for(i=0;i<ATR[0];i++)
      trace_debug_printf("%02x",ATR[i+1]);
#endif    
    return 0;
  
}

/************************************
 *  ���ѡ��IC����ATR����
 ************************************/
void ClearAtr(uchar Channel)
{
    asyncard_info[Channel].TA1 = 0x11;
    asyncard_info[Channel].D   = 0x01;
  
            
    asyncard_info[Channel].TA2 = 0x00;
    asyncard_info[Channel].TA3 = 0x00;
    asyncard_info[Channel].TA4 = 0x00;

    asyncard_info[Channel].TB1 = 0x00;
    asyncard_info[Channel].TB2 = 0x00;
    asyncard_info[Channel].TB3 = 0x00;
    asyncard_info[Channel].TB4 = 0x00;

    asyncard_info[Channel].TC1 = 0x00;
    asyncard_info[Channel].TC2 = 0x00;
    asyncard_info[Channel].TC3 = 0x00;
    asyncard_info[Channel].TC4 = 0x00;

    asyncard_info[Channel].TD1 = 0x00;
    asyncard_info[Channel].TD2 = 0x00;
    asyncard_info[Channel].TD3 = 0x00;
}

/**************************************************
 *  ѡ��ǰӦ�õ�IC��ͨ��SLOT��������Ƿ������ϵ��λ��
 *  ���أ�0X00-------�ɹ���
 *        0X01-------ͨ���������
 *        0XFF-------ѡ��ʧ�ܣ�δ�ϵ���޿���
 **************************************************/
int SelectSlot(uchar slot)
{
    //if(slot>=IC_MAXSLOT) 
    if( (slot>=IC_MAXSLOT) || (slot==0)) 
      return ICC_INPUTSLOTERR;
    //sam��ֻ�ܼ���һ��
    if(slot==1)
    {
    //Y1
    GPIO_SetBits(SC_PIN_3_5V_GPIO, SC_PIN_3_5V_SAM1);
    GPIO_ResetBits(SC_PIN_3_5V_GPIO, SC_PIN_3_5V_SAM2);
      
    }
    else if(slot==2)
    {
    //Y2
    GPIO_ResetBits(SC_PIN_3_5V_GPIO, SC_PIN_3_5V_SAM1);
    GPIO_SetBits(SC_PIN_3_5V_GPIO, SC_PIN_3_5V_SAM2);
    }
    
    current_slot=slot;
    //if(asyncard_info[slot].open!=VCC_OPEN)
        //return ICC_VCCOPENERR;
    return 0;
}



/********************************************************************
 * �ֱ����T=1��T=0Э�齫CLA+INS+P1+P2+LC��LE��+INDATA���������ݰ���ʽ
 * ���� * ת����������Ӧ�ĸ�ʽ��IC�����н��������������״̬������
 * OUTDATA�У�
 *   T=1ʱ�� OUTDATA�� LEN��2��+DATA+SW��1��2��
 *   T=0ʱ�� OUTDATA�� LEN��1��+DATA��
 * ����ֵ��  0X00---�ɹ��� 0X01---ͨ��ʧ�ܣ�0X02---�޿�����

 * cla=0x00; ins=  0xa4;  p1=0x04;  p2=0x00; p3=0x0e(�����������ݳ���);  Le=256(������Ӧ��������);
 *  *outdata=1PAY.SYS.DDF01"
 ********************************************************************/
int IccSendCommand(uchar cla, uchar ins,uchar p1,uchar p2,ushort p3,uchar *indata,ushort le,uchar *outdata)
{

     uchar test[5];
     uchar cmd_case;
     uchar inbuf[ICMAXBUF];     //������������
     uchar outbuf[ICMAXBUF];   //������Ӧ����
     uchar *ptr,len;
     int   result;
     ushort i;

     SW1=0xff;
     SW2=0xff;
     memset(outbuf,0x00,ICMAXBUF);
     //test
     trace_debug_printf("T[%d]",asyncard_info[current_slot].T);
     Lib_DelayMs(1);
     
     //����T=1��T=0Э�������Ӧ�Ĵ���
     switch(asyncard_info[current_slot].T)
     {
     case 1:
          i=4;
          inbuf[2]=cla;
          inbuf[3]=ins;
          inbuf[4]=p1;
          inbuf[5]=p2;
          ptr=&inbuf[6];
          if((p3==0)&&(le==0))//NOT DATA//
          {
               i++;
               *ptr++=0;
          }
          if(p3>0)    //DATA//
          {
               len=(uchar)p3;
               i+=len+1;
               *ptr++=len;
               if(len>0)
               {
                    memcpy(ptr,indata,len);
                    ptr+=len;
               }
          }
          if(le>0)
          {
               if(le>=256)  le=0x00;
               i++;
               *ptr++=(uchar)le;
          }
          inbuf[0]=i/256;
          inbuf[1]=i%256;
          result=T1_send_command(0x00,inbuf,outbuf);
          if(result==0)
               memcpy(outdata,&outbuf[1],outbuf[1]*256+outbuf[2]+2);
          return result;
     case 0:
          cmd_case=0x01;
          if((p3==0)&&(le==0))  cmd_case=0x01;  // case 1 command
          if((p3==0)&&(le>0))   cmd_case=0x02;  // case 2 command
          if((p3>0)&&(le==0))   cmd_case=0x03;  // case 3 command
          if((p3>0)&&(le>0))    cmd_case=0x04;  // case 4 command
          test[0]=cmd_case; //����ĳ�����//

          if(p3>0){// ����IC������P3���ȵ�����//
               len=(uchar)p3;
               memcpy(inbuf,indata,p3);
          }
          else{  //���IC������LE���������ݡ�//
               if(le>=256)  le=0x00;
               len=(uchar)le;
          }
          result=T0_send_command(cla,ins,p1,p2,len,inbuf,outbuf,test);
          if(result)   return result;
          memcpy(outdata,outbuf,outbuf[0]*256+outbuf[1]+2);
          SW1=test[0];
          SW2=test[1];
          return SUCCESS;
     default:
          break;
     }
     return ICC_PROTOCALERR;
}

/*******************************
 *     ����2��N�η�����
 ********************************/
uchar exp2_n(uchar en)
{
     switch(en){
     case 0:  return 1;
     case 1:  return 2;
     case 2:  return 4;
     case 3:  return 8;
     case 4:  return 16;
     case 5:  return 32;
     case 6:  return 64;
     case 7:  return 128;
     }
     return 0xff;
}

/**********************************************************
 *  T=1Э���µ��������ݷ����������Ӧ���ݡ�
 *  NAD------����ַ������Ϊ0X00����
 *  INBUF----���͵���Ϣ�ֶ�LEN+INF��INBUF[0]Ϊ���ȣ�
 *  OUTBUF---����Ӧ�����ݵ�Ԫ����ʽ
 *           NAD+LEN��2�ֽڣ�+DATA+SW1+SW2
 *
 *  ����ֵ�� SUCCESS  ���ͳɹ�
 *           0XFF----��ʼ��IC��ʧ��
 *           ICC_T1-BWTERR   T1-CWTERR  ��ʱ����
 *           ICC_T1-EDCERR   T1-PARITYERR ��ż/EDC����
 *           ICC_T1_MOREERR;
 *           ICC_T1-ABORTERR ��������
 *           ICC_T1-INVALIDBLOCK ��Ч������
 ************************************************************/
int T1_send_command(uchar nad,uchar *inbuf,uchar *outbuf)
{
    uchar   sour[300],oldsour[300],respbuf[300],oldrblock[5];
    uchar   OLDIFSC;
    uchar   PCB,tmpch,tmpch1;//NAD,,slotpcb
    ushort  len,len1;//,totallen,currentlen,remainlen
    ushort  total_send_len,sent_len,remain_len;
    uchar   re_send_i_times,re_send_r_times;
    int     result;
 

    OLDIFSC=asyncard_info[current_slot].IFSC;
start_t1_exchange:
    tmpch=asyncard_info[current_slot].TB3&0x0f;
    CWT=(11+exp2_n(tmpch))-10;
    WTX=1;
    tmpch=(asyncard_info[current_slot].TB3>>4)&0x0f;
    BWT=(11+960*exp2_n(tmpch))-10;
    re_send_i_times=0;
    re_send_r_times=0;
    total_send_len=inbuf[0]*256+inbuf[1];  
    remain_len=inbuf[0]*256+inbuf[1];      
    sent_len=0;
    outbuf[0]=0x00;
    outbuf[1]=0x00;
    outbuf[2]=0x00;
    len=0;len1=0;
    asyncard_info[current_slot].sw1=0Xff;
    asyncard_info[current_slot].sw2=0xff;
    memset(respbuf,0x00,300);
    memset(sour,0x00,300);
    memset(oldsour,0x00,300);
    memset(oldrblock,0x00,5);
send_remain_data:
    if(remain_len==0)
    {
         Dlynetu(6);
         Lib_IccClose(current_slot);
         return ICC_ICCMESERR;
    }
    if(remain_len>asyncard_info[current_slot].IFSC)
    {    // ������ݰ�������ifscʱ���򽫰������зֺ��ٷ���//
         asyncard_info[current_slot].term_pcb|=0x20; //��I����������ӷ�//
         memcpy(&sour[3],&inbuf[2+sent_len],asyncard_info[current_slot].IFSC);
         //�趨��������I_BLOCK  //                       
         sour[2] = asyncard_info[current_slot].IFSC;  //�趨I_block�����ݳ���//
    }
    else{ //����ֻ��һ���Ϳ���ȫ���� //
         asyncard_info[current_slot].term_pcb &= 0xdf;
         memcpy(&sour[3],&inbuf[2+sent_len],remain_len);  
         sour[2]=remain_len;
    }
    sent_len+=sour[2]; // �趨�ѷ������ݳ���
    remain_len=total_send_len-sent_len;
    sour[1]=asyncard_info[current_slot].term_pcb; // �趨���Ͱ�ͷ��PCBЭ������ֽ�//
    sour[0]=0x00;
    memcpy(oldsour,sour,sour[2]+3); // ���ݷ��͵�I����
    asyncard_info[current_slot].term_pcb=sour[1] ^ 0x40;
    re_send_i_times=0;
re_send_i_block:
    if(re_send_i_times>2)   
    {
         Dlynetu(5);
         Lib_IccClose(current_slot);
         return ICC_T1_MOREERR;
    }
    re_send_i_times++;
    re_send_r_times=0;
    sour[0]=0x00;
    T1_Send_Block(sour);
receive_one_block:
    result=T1_Rece_Block(respbuf);
    if((result==ICC_T1_CWTERR)||(result==ICC_T1_BWTERR)||(result==ICC_T1_ABORTERR))
    {
         Dlynetu(5);
         Lib_IccClose(current_slot);
         return result;
    }
    if((result==ICC_T1_PARITYERR)||(result==ICC_T1_EDCERR)){
         tmpch = asyncard_info[current_slot].card_pcb^0x40;
         //���Ϊ���ڽ��տ�Ƭ��I����˳���(��Ϊ�ϴν��յ��¸����)
         sour[1]=((tmpch >> 2) & 0x10) | 0x81; // ����ż/EDC����
         sour[2]=0;
         goto re_send_r_block;
    }
    if(result==ICC_T1_INVALIDBLOCK)  goto set_rblock_with_othererr;
    PCB=respbuf[1]; // ��ȡ��Ӧ���ݰ�ͷ//
    if((PCB&0X80)==0X00){// ?i_block
         //re_send_r_times=0;
         if(remain_len>0) goto set_rblock_with_othererr;
         if((PCB & 0x40) != (asyncard_info[current_slot].card_pcb & 0x40))
         {    //  �յ���I�������кŲ�ͬ����ǰ�յ���I�������к�
              //  ʱ��ȷ�Ͻ�����ȷ����������յ����ݵ�OUTBUF�С�
              re_send_r_times=0;
              if(respbuf[2]>0)
              {    //����������ݳ��Ȳ�Ϊ���򱣴��������
                   len1 = outbuf[1] * 256 + outbuf[2];
                   len=len1 + respbuf[2];
                   outbuf[1]= len / 256;
                   outbuf[2]= len % 256;
                   memcpy(&outbuf[len1+3],&respbuf[3],respbuf[2]);
              }
              asyncard_info[current_slot].card_pcb=PCB;
              //�����µ�I����˳���
              if((PCB&0X20)==0X20){ // �յ�������I����
                   tmpch = asyncard_info[current_slot].card_pcb ^ 0x40;
                   // �¸�I_Block˳���
                   sour[1]=((tmpch >>2 ) & 0x10) | 0x80;
                   sour[2]=0;
                   goto re_send_r_block;
              }
              // ����ȷ���յ������ӵ�I�������˳�
              result=SUCCESS;
              goto end_t1_exchange;
              //return SUCCESS;
         }
         else goto set_rblock_with_othererr;
    }
    else if((PCB&0XC0)==0X80){// ?r_block
         tmpch=asyncard_info[current_slot].term_pcb;
         if((tmpch&0x40)==((PCB&0x10)<<2)){
         // ��ŵ����ն��¸�Ҫ���͵�I�������
              if((PCB&0xef)==0x80){// r-block with error-free
                   if((oldsour[1]&0x20)==0x20) // �ն�����
                        goto send_remain_data;
                   else {  //�ն�����
                        tmpch = asyncard_info[current_slot].card_pcb ^ 0x40;
                        tmpch1=((tmpch >> 2 ) & 0x10) | 0x82;  
                        //tmpch1=((tmpch >> 2 ) & 0x10) | 0x80;
                        //if(sour[1]!=tmpch1) 
                        re_send_r_times=0;
                        sour[1]=tmpch1;
                        sour[2]=0;
                        goto re_send_r_block;
                   }
              }
              else { // ���д����ʶ��R����
                   if(re_send_r_times==0){ // ��һ����R����
                        tmpch = asyncard_info[current_slot].card_pcb ^ 0x40; 
                        sour[1]=((tmpch >>2 ) & 0x10) | 0x80; 
                        sour[2]=0; 
                        goto re_send_r_block; 
                   }
                   else { // �޸Ĵ����ʶ
                        if((sour[1]&0x0f)!=0x00){
                             sour[1]=sour[1]&0xf0;
                             re_send_r_times=0;
                        }
                   }
              }
              // �ط�R����
              goto re_send_r_block;
         }
         // r_block with edc or parity or other errors
         if((tmpch&0x40)!=((PCB&0X10)<<2)){
         // �����ŵ����ն��ѷ��͵�I����������ط���I����
              if((asyncard_info[current_slot].card_pcb&0x20)==0x20) goto re_send_r_block; 
              if(OLDIFSC!=asyncard_info[current_slot].IFSC){
                   OLDIFSC=asyncard_info[current_slot].IFSC;
                   asyncard_info[current_slot].term_pcb=oldsour[1];
                   goto start_t1_exchange;
                   //���յ��µ�IFSCЭ�̺����յ�R���������ط�ʱ��
                   //�Ȼָ��ն�I�������кţ�����ͷ����
              }
              memcpy(sour,oldsour,oldsour[2]+3);
              goto re_send_i_block;
         }
         // �ط�R����
         goto re_send_r_block;
    }
    else if((PCB&0XC0)==0XC0){// ?s_block
         // re_send_r_times=0;  
         if((PCB & 0x20)==0X00)
         {    //����յ�����IC�����͵������ź�����ݴ�����������
              //�����޸���Ӧ���趨������S��Ӧ�����IC��
              sour[1]=0x20 | respbuf[1] ;
              sour[2]=respbuf[2];
              memcpy(&sour[3],&respbuf[3],sour[2]);
              if((respbuf[1] & 0x1f) ==0x03)
              {   // ����BWT����  BWT=BWT * respbuf[3];
                   WTX= respbuf[3];
              }
              else if ((respbuf[1] & 0x1f) == 0x01)
              {    //��Ϣ�ֶ���������
                   asyncard_info[current_slot].IFSC = respbuf[3];
              }
              else if((respbuf[1]&0x1f)==0x00)
              {    // ͬ������
              }
              sour[0]=0x00;  //����S��Ӧ�����IC��
              T1_Send_Block(sour);
              re_send_r_times=0;  
              goto receive_one_block;
         }
         else { //���յ���IC�����͵�S��Ӧ�ź�*/
              if((sour[1]&0xe0)==0xc0)
              { // ����ն˷�����������Ϣʱ�������Ӧ������Ӧ�Ĳ���
                   re_send_r_times=0;  
                   if ((respbuf[1] & 0x1f) == 0x01)
                   {
                        asyncard_info[current_slot].IFSD = respbuf[3];
                   }
                   else if((respbuf[1] & 0x1f) ==0x00)
                   {    //  Reset or deactivation
                        asyncard_info[current_slot].term_pcb=0x00;
                        asyncard_info[current_slot].card_pcb=0x40;
                        goto start_t1_exchange;
                   }
                   goto end_t1_exchange;
              }
         }
    }
set_rblock_with_othererr:
    tmpch = asyncard_info[current_slot].card_pcb ^ 0x40;
    sour[1]=((tmpch >>2 ) & 0x10) | 0x82; // �����������R����
    sour[2]=0;
re_send_r_block:
    if(re_send_r_times!=0){// change for TG250000 test case
         if((oldrblock[0]!=sour[0])||(oldrblock[1]!=sour[1])||(oldrblock[2]!=sour[2]))
              re_send_r_times=0;
    }
    if(re_send_r_times==0){
         oldrblock[0]=sour[0];
         oldrblock[1]=sour[1];
         oldrblock[2]=sour[2];
    }
    if(re_send_r_times>1){ 
         Dlynetu(5);
         Lib_IccClose(current_slot);
         return ICC_T1_MOREERR;
    }
    T1_Send_Block(sour);
    re_send_r_times++;
    goto receive_one_block;
end_t1_exchange:
    if(result!=0)
    {//���ͨ���봦��ʧ�ܣ�����SW1 SW2Ϊ0XFF��
         asyncard_info[current_slot].sw1=0Xff;
         asyncard_info[current_slot].sw2=0xff;
         return result;
    }
    len=outbuf[1]*256+outbuf[2]+3;
    // �����յ��������λ���ݱ��浽SW1��SW2
    asyncard_info[current_slot].sw1=outbuf[len-2];
    asyncard_info[current_slot].sw2=outbuf[len-1];
    return SUCCESS;
}

/**********************************************************************
 *  T=0Э���·�������ͷ��CLA+INS+P1+P2+P3�����ݰ���DATABUF��������ȡIC
 *  ����Ӧ�����ݣ�OUTBUF����״̬�ֽڣ�RSLT����
 *     DATABUF-----���ն˷��������ݰ������䳤��ΪP3����
 *     OUTBUF------��IC���������������ݣ�[0]Ϊ���ݳ��ȣ�
 *     RSLT--------��CASE����ţ�1��2��3��4��
 *                 ����ʱ����SW1��SW2״̬�ֽڡ�
 *  ���أ�  0X00----�ɹ���0X0F----��IC��0ʱ�޿���0XFF---ͨ�Ż�У��ʧ�ܡ�
 *  NS---ͬ�򱣻�ʱ��, NR---���򱣻�ʱ��
 ***********************************************************************/
int T0_send_command(uchar Class,uchar Ins,uchar P1,uchar P2,uchar P3, uchar *Data_Buf,uchar *Out_Buf,uchar *rslt)
{
     ushort len,i,len1,len2;//,Tc2;
     uchar    ret;
     uchar  tempsw1,tempsw2;
     uchar  *ptr1,*ptr2,status[2],cmdbuf[5];
     uchar  cmd_case,cardtype,auto_resp,is_rece_send;
     uchar  FirstByte_Is_Status;
     uchar  Change_Status_Word;
     ushort  NS,NR;


     if (asyncard_info[current_slot].TC1==0xff) NS=12;
     else NS=asyncard_info[current_slot].TC1+12;

     NR=16;   
     NS=NS; 

     if(asyncard_info[current_slot].TC2==0x00) i=10;
     else i=asyncard_info[current_slot].TC2;

     WWT=960*i;         
     cmd_case=*rslt;     
     cardtype = asyncard_info[current_slot].autoresp;

     if(cardtype)   auto_resp=0x00;   
     else           auto_resp=0x01;   

     FirstByte_Is_Status=1;  
     Change_Status_Word=0;   

     k_Ic_TimeCount=0;
     k_IcMax_TotalTimeCount=0x00;
     k_Ic_TotalTimeCount=0x00;
     k_timeover=0x00;
     k_total_timeover=0x00;
     k_IcMax_TimeCount=WWT/10+60; 

     is_rece_send=0;  
     if(cmd_case==0x01) is_rece_send=0;
     else if(cmd_case==0x02) is_rece_send=0;
     else if(cmd_case==0x03) is_rece_send=1;
     else is_rece_send=1;
     //������ͷ�� //
     cmdbuf[0]=Class;
     cmdbuf[1]=Ins;
     cmdbuf[2]=P1;
     cmdbuf[3]=P2;
     cmdbuf[4]=P3;
     status[0]=0xff;
     status[1]=0xff;
     len=0;
     ptr1=Data_Buf;
     ptr2=Out_Buf+2;
  
     tempsw1=0xff;
     tempsw2=0xff;
//test
     trace_debug_printf("T0_send_command begin");
     
     while(1)
     {
flg1:
          len1=(ushort)cmdbuf[4];  // �����������ݳ���  ,P3//
          len2=(ushort)cmdbuf[4];  //  ������Ӧ���ݳ��� ,P3//

          
          Dlynetu(NR);

     
          s_SetGTnetu(NR);
     
          ret=Send_Byte(cmdbuf[0]);   // ����CLA  //
          if(ret){
               Lib_IccClose(current_slot);
               return ICC_T0_MORESENDERR;
          }
          s_SetGTnetu(NS);
          ret=Send_Byte(cmdbuf[1]);  //����INS  //
          if(ret){
               Lib_IccClose(current_slot);
               return ICC_T0_MORESENDERR;
          }
          s_SetGTnetu(NS);
          ret=Send_Byte(cmdbuf[2]);   //����P1  //
          if(ret){
               Lib_IccClose(current_slot);
               return ICC_T0_MORESENDERR;
          }
          s_SetGTnetu(NS);
          ret=Send_Byte(cmdbuf[3]);   // ����P2 //
          if(ret){
               Lib_IccClose(current_slot);
              return ICC_T0_MORESENDERR;
          }
          s_SetGTnetu(NS);
          ret=Send_Byte(cmdbuf[4]);   //����P3  //
          if(ret){
               Lib_IccClose(current_slot);
               return ICC_T0_MORESENDERR;
          }
          //add
          //USART3_ISR_Clr();
            /* Flush the SC_USART DR */
          //if(cmdbuf[1]!=0xc0)
          {
          ret = USART_ReceiveData(SC_USART);
          trace_debug_printf("77rece[%x]",ret);
          }
#if 0          
    scs_stat         =   SC_UART_WAIT_TS | SC_UART_RECV_EV;         	    	
    work_step        =   SC_UART_CLKS_EVENT;                       
    USART3_ISR_Clr();			
    USART_ITConfig( SC_SML_UART, USART_IT_TC, DISABLE );		
    USART_ITConfig( SC_SML_UART, USART_IT_RXNE, ENABLE );   
#endif          

flg2:
          
        
       
          status[0]=Rece_Byte();  
          
            //test
            //goto flg2;
          
          if(k_IccComErr){
               Lib_IccClose(current_slot);
               if(k_IccErrPar) return(ICC_T0_PARERR);
               else return ICC_T0_TIMEOUT;
          }
          if(status[0]==cmdbuf[1])
          {// �����ֽڵ���INS----���ͻ������������ݰ�  //
               if(is_rece_send) { // �������ݰ�  //
                    Dlynetu(NR);
                    s_SetGTnetu(NR);
                    for(i=0;i<len1;len1--){
                         //Dlynetu(NS);  
                         ret=Send_Byte(*ptr1);
                         if(ret){
                              Lib_IccClose(current_slot);
                              return ICC_T0_MORESENDERR;
                         }
                         if(len1>1)   s_SetGTnetu(NS);
                         ptr1++;
                    }
                    //add 12/11/15
      /* Flush the SC_USART DR */
      (void)USART_ReceiveData(SC_USART);
                    
                    FirstByte_Is_Status=0;
                    goto flg2;  // �ȴ���һ�������ֽ� //
               }
               else{  //  �������ݰ�  //
                    len+=len2;
                    if(len>256){ 
                         Lib_IccClose(current_slot);
                         return ICC_DATA_LENTHERR;
                    }
                    for(i=0;i<len2;len2--){
                          
                         *ptr2=Rece_Byte();
                         if(k_IccComErr){
                              //Dlynetu(2);
                              Lib_IccClose(current_slot);
                              if(k_IccErrPar) return(ICC_T0_PARERR);
                              else return ICC_T0_TIMEOUT;
                         }
                         ptr2++;
                    }
                    FirstByte_Is_Status=0;
                    goto flg2;
               }
          }
          else if(status[0]==(uchar)(~cmdbuf[1]))
          {    // ����INS�Ĳ���  ���ջ�����һ������  //
               if(is_rece_send){// ����һ�ֽ�����  //
                    Dlynetu(NR);
                    s_SetGTnetu(NR); 
                    ret=Send_Byte(*(ptr1));
                    if(ret){
                         Lib_IccClose(current_slot);
                         return ICC_T0_MORESENDERR;
                    }
                    ptr1++;
                    len1--;
                    FirstByte_Is_Status=0;
                    goto flg2;
               }
               else {//  ������һ�ֽ�����  //
                    if (current_slot==0){
                         if(!asyncard_info[current_slot].existent)
                              return ICC_CARD_OUT;
                    }
                    *ptr2=Rece_Byte();
                    if(k_IccComErr){
                            //Dlynetu(2);
                         Lib_IccClose(current_slot);
                         if(k_IccErrPar) return(ICC_T0_PARERR);
                         else return ICC_T0_TIMEOUT;
                    }
                    ptr2++;len++;
                    len2--;
                    FirstByte_Is_Status=0;
                    if(len>256){ 
                         Lib_IccClose(current_slot);
                         return ICC_DATA_LENTHERR;
                    }
                    goto flg2;
               }
          }
          else if(status[0]==0x60)
          { //  �ն��ṩ���ӵĹ����ȴ�ʱ��  //
               FirstByte_Is_Status=0;
               goto flg2;
          }
          //if(status[0]!=0x90){
          if(((status[0]&0xf0)!=0X60)&&((status[0]&0xf0)!=0x90))
          {  //�յ�����Ĺ��̻�״̬�ֽں��ͷŴ��� //
               Dlynetu(2);
               Lib_IccClose(current_slot);
               return ICC_T0_INVALIDSW;
          }
          // ���SW1Ϊ��6X����9X������60�����⣩ʱ����ȴ��ù��̵���һ��״̬�ֽ�SW2  //
          status[1]=Rece_Byte();
          if(k_IccComErr){
             //Dlynetu(2);
               Lib_IccClose(current_slot);
               if(k_IccErrPar) return(ICC_T0_PARERR);
               else return ICC_T0_TIMEOUT;
          }
          if(current_slot==0){ //  ����Ƿ񲦿�  //
               if(!asyncard_info[current_slot].existent)
                    return ICC_CARD_OUT;
          }
          if(auto_resp) {//�Զ���Ӧ  //
               if(status[0]==0x61)
               {    // ���SW1=0X61���ն����ͳ�һ��GET RESPONSE����ͷ��
                    //��IC�������е�P3=SW2 //
                    cmdbuf[0]=0x00;cmdbuf[1]=0xc0;cmdbuf[2]=0x00;
                    cmdbuf[3]=0x00;cmdbuf[4]=status[1];
                    is_rece_send=0;  // ����Ĳ���ֻ�ܴ�IC���������ݰ�//
                    FirstByte_Is_Status=0;
                    if(cmd_case==0x04) cmd_case=0x02;  
                    goto flg1;       //�����¸�����ͷ��  //
               }
               if(status[0]==0x6c)
               {    // ���SW1=0X6C���ն˴�����ǰ������ͷ���IC����
                    //����P3=SW2�� //
                    cmdbuf[4]=status[1];
                    is_rece_send=0;
                    FirstByte_Is_Status=0;
                    if(cmd_case==0x04) cmd_case=0x02; 
                    goto flg1;
               }
               if((cmd_case==0x04)&&(FirstByte_Is_Status==0)){ 
                    if((status[0]==0x62)||(status[0]==0x63)||
                    (((status[0]&0xf0)==0x90) && (!(status[0]==0x90 && status[1]==0x00))))   
                    {    //��CASE 4������յ�62XX 63XX 9XXX(��9000�⣩�����ֽں���
                         //�ȷ���LE=0��GET RESPONSE COMMAND ��
                         //��ȡ��9000״̬�ֽ�
                         cmdbuf[0]=0x00;cmdbuf[1]=0xc0;cmdbuf[2]=0x00;
                         cmdbuf[3]=0x00;cmdbuf[4]=0x00;
                         is_rece_send=0;
                         Change_Status_Word=1;
                         cmd_case=0x02; //�޸�CASE״̬��ֹ�ط� //
                         tempsw1=status[0];
                         tempsw2=status[1];
                         goto flg1;
                    }
               }
          }
          // ���յ��������ݵĹ����ֽ�ʱ�򱣴���״̬�ֽڲ�����
          // ���Կ��Ǽ���SW1=90 SW2=00�ļ��
          if(Change_Status_Word){ 
               *rslt=tempsw1;
               *(rslt+1)=tempsw2;
          }
          else{
               *rslt=status[0];
               *(rslt+1)=status[1];
          }
          *Out_Buf=len/256;
          *(Out_Buf+1)=len%256; // ������յ����ݳ��� //
          return SUCCESS;
     }
 }



/*********************************************************
 * ���յ���ȷ�ĸ�λӦ��󣬶���T=1��Ƭ�뷢��һ��IFSD����
 * ��ָʾ�ն�����ȡ������鳤�ȡ�
 * ����ֵ�� SUCCESS
 *          ICC_T1_CWTERR,ICC_T1_BWTERR;
 *          ICC_T1_ABORTERR;
 *          ICC_T1_IFSDERR;
 *********************************************************/
int  T1_IFSD_command(uchar slot)
{
     int ret;
     uchar ifsdbuf[10],respbuf[300];
     uchar resendtimes=0;
     uchar tmpch;

     tmpch=asyncard_info[current_slot].TB3&0x0f;
     CWT=(11+exp2_n(tmpch))-10;
     WTX=1;
     tmpch=(asyncard_info[current_slot].TB3>>4)&0x0f;
     BWT=(11+960*exp2_n(tmpch))-10;
     //BWT=(11+960*exp2_n(BWT)*372*D[current-slot]/F[current_slot])

     ret=SelectSlot(slot);
     if(ret==0x01) return ICC_SLOTERR;
     if(asyncard_info[current_slot].T==0) return SUCCESS;
     memset(ifsdbuf,0,10);
     memset(respbuf,0,300);
     ifsdbuf[0]=0x00;
     ifsdbuf[1]=0xc1;
     ifsdbuf[2]=0x01;
     ifsdbuf[3]=0xfe;
     ifsdbuf[4]=0x00;
     WTX=1;
     while(1)
     {
          if(resendtimes<3)  ret=T1_Send_Block(ifsdbuf);
          else
          {
               Dlynetu(5);
               Lib_IccClose(slot);
               return ICC_T1_IFSDERR;
          }
          resendtimes++;
          ret=T1_Rece_Block(respbuf);
          if((ret==ICC_T1_CWTERR)||(ret==ICC_T1_BWTERR)||(ret==ICC_T1_ABORTERR))
          {
               Dlynetu(5);
               Lib_IccClose(slot);
               return ret;
          }
          if((ret==ICC_T1_PARITYERR)||(ret==ICC_T1_EDCERR)||(ret==ICC_T1_INVALIDBLOCK))
               continue;
          if(((respbuf[1]&0xe0)!=0xe0)||((respbuf[1]&0x0f)!=0x01))
               continue;
          if((respbuf[2]!=0x01)||(respbuf[3]!=0xfe))  
               continue;
          asyncard_info[current_slot].IFSD=254;
          return SUCCESS;
     }
}



/*************************************************
 * ���ܣ� T=1Э���·���һ����
 *************************************************/
int  T1_Send_Block(uchar *Inbuf)
{
     uchar   edc,tmpbuff[ICMAXBUF];
     ushort  i,len;//,l;
     ushort  NS,NR;


     /* ͬ��(NS)�뷴��(NR)���ͱ���ʱ��  ��λ��ETU��*/
     if(asyncard_info[current_slot].TC1==0xFF)  NS=11;
     else NS=asyncard_info[current_slot].TC1+12;

     NR=22;
     //NS=NS;  
     if(asyncard_info[current_slot].D>2)  //ADD FOR DEBUG
     {
         //NR += 10;
         //NS += 160;
     }
     len=*(Inbuf+2)+3; /*  �����ܳ�NAD+PCB+LEN+��LEN��DATA����*/
     edc=0;            /*  ��������������Ԫ��У��EDC */
     memset(tmpbuff,0x00,ICMAXBUF);
     for(i=0;i<len;i++)
     {
          tmpbuff[i]=*Inbuf++;
          edc=edc ^ tmpbuff[i];
     }
     tmpbuff[i]=edc;
     Dlynetu(NR);
     //s_SetGTnetu(NR); //�����ͱ���ʱ����  
     s_SetGTnetu(NS); 
     Send_Byte(tmpbuff[0]);   
     for(i=1;i<len+1;i++){
         Dlynetu(NS-10);
      
         Send_Byte(tmpbuff[i]);
     }
     return SUCCESS;
}

/*************************************************
 * ���ܣ� T=1Э���½���һ����
 * ���أ� T1-BWTERR   T1-CWTERR  ��ʱ����
 *        T1-EDCERR   T1-PARITYERR ��ż/EDC����
 *        T1-ABORTERR ��������
 *        T1-INVALIDBLOCK ��Ч������
 *************************************************/
int  T1_Rece_Block(uchar *Outbuf)
{
     uchar   i,ch,edc;
     uchar   Parity_Error_Flag;
     ulong   T1BWT,T1CWT;
     uchar   r_nad,r_pcb,r_len,r_inf;
     uchar   r_len_temp; 

     T1BWT=BWT*WTX+WTX*960;
     T1CWT=CWT+4;
     WTX=1;
     k_IcMax_TotalTimeCount=0x00;
     k_Ic_TotalTimeCount=0x00;
     k_total_timeover=0x00;

     k_IcMax_TimeCount=T1BWT/10+30;
     edc=0;
     Parity_Error_Flag=0;
     ch=Rece_Byte_T1();   //NAD
     k_IcMax_TimeCount=T1CWT/10+5;
     edc=edc ^ ch;
     *Outbuf++=ch;
     r_nad=ch;
     if(k_IccErrTimeOut) return ICC_T1_BWTERR;
     if(k_IccErrPar) Parity_Error_Flag=0x01;
     ch=Rece_Byte_T1();  // ����PCB�ַ�  //
     edc=edc ^ ch;
     *Outbuf++=ch;
     r_pcb=ch;
     if(k_IccErrTimeOut) return ICC_T1_CWTERR;
     if(k_IccErrPar) Parity_Error_Flag=0x01;
     ch=Rece_Byte_T1(); // ����LEN���ݳ���  //
     edc=edc ^ ch;
     *Outbuf++=ch;
     r_len=ch;
     r_len_temp=ch;
     if((r_pcb&0xc0)==0x80)  //add 2004-03-09
     {
          if(ch) r_len_temp=0;
     }
     else if((r_pcb&0xc0)==0xc0)
     {
          //if(ch!=1) r_len_temp=1;
          if(ch>1) r_len_temp=1;
     }
     if(k_IccErrTimeOut) return ICC_T1_CWTERR;
     if(k_IccErrPar) Parity_Error_Flag=0x01;
     for(i=0;i<r_len_temp;i++)
     {
          ch=Rece_Byte_T1();
          edc=edc ^ ch;
          *Outbuf++=ch;
          if(k_IccErrTimeOut) return ICC_T1_CWTERR;
          if(k_IccErrPar) Parity_Error_Flag=0x01;
     }
     r_inf=ch;
     ch=Rece_Byte_T1();
     *Outbuf=ch;
     if(k_IccErrTimeOut) return ICC_T1_CWTERR;
     if(k_IccErrPar) Parity_Error_Flag=0x01;
     if(ch!=edc) return ICC_T1_EDCERR;
     if(Parity_Error_Flag) return ICC_T1_PARITYERR;
     if(r_nad) return ICC_T1_INVALIDBLOCK;   //nad!=0
     if((r_pcb&0x80)==0){ // I_BLOCK
          if(r_len==0xff) return ICC_T1_INVALIDBLOCK;
     }
     else if((r_pcb&0xc0)==0x80){ // R_BLOCK
          if(r_len) return ICC_T1_INVALIDBLOCK;
          if((r_pcb&0x20)==0x20) return ICC_T1_INVALIDBLOCK; //rblock bit6=1  
     }
     else { // S_BLOCK
          if(r_pcb==0xc1){ // ifs request
               if((r_inf<0x10)||(r_inf>0xfe)) return ICC_T1_INVALIDBLOCK;
          }
          else if(r_pcb==0xc2)  // abort request
               return ICC_T1_ABORTERR;
          else if(r_pcb==0xe3) return ICC_T1_INVALIDBLOCK; //S(WTX response)      
          else if(r_pcb==0xe2) return ICC_T1_INVALIDBLOCK; //S(ABORT response)    
          else if(r_pcb==0xe0) return ICC_T1_INVALIDBLOCK; //S(RESYNCH response)  
          if(r_len!=1) return ICC_T1_INVALIDBLOCK;   
          if((r_pcb&0x1f)>4) return ICC_T1_INVALIDBLOCK;  
     }
     return SUCCESS;
} 



uchar Rece_Byte_T1(void)
{   
    uchar combuffer;
    combuffer=Rece_Byte();
    return combuffer;
} 

/*******************************************************
 *  IC������������֧�֣�T=0��T=1��Э�飻
 *  ���أ� 0X00---------ִ�гɹ���
 *         0X01---------ͨ�ų�ʱ��
 *         0X02---------�����п���������
 *         0x03---------��ż����
 *         0X04---------ѡ��ͨ������
 *         0X05---------��������̫����LC����
 *         0XFF---------����ͨ�Ż�û�ϵ硣
 *******************************************************/
int Lib_IccCommand(uchar slot,APDU_SEND * ApduSend,APDU_RESP * ApduResp)
{
     uchar   Cla,Ins,P1,P2;
     ushort  Le,Lc,iT1Le,recelen;
     int   result;
     //uchar   *ptr; 
     //test
     uchar i;
     
     SW1=0;     
     SW2=0;
     ApduResp->LenOut=0;
     ApduResp->SWA=0;
     ApduResp->SWB=0;
     //test
     trace_debug_printf("Lib_IccCommand begin");
     
     if((ApduSend==NULL)||(ApduResp==NULL))
          return ICC_PARAMETERERR;
     slot=slot&0x7f;
     slot=slot&0xbf;  //add 20080508
     memset(card_Outputdata,0x00,ICMAXBUF);
     result = SelectSlot(slot);
     if(result)  return ICC_SLOTERR;
     if(asyncard_info[current_slot].open==VCC_CLOSE)
           return ICC_NO_INITERR;
     if(asyncard_info[current_slot].T==0)
          Parity_Check_Enable=0x01;
     else
          Parity_Check_Enable=0x00;
     //����T=0Э�鿨ƬӦ����żУ������ط�����
     Cla=ApduSend->Command[0];
     Ins=ApduSend->Command[1];
     P1= ApduSend->Command[2];
     P2= ApduSend->Command[3];
     Lc= (ushort )(ApduSend->Lc);//���͵��������ݳ���

     if(Lc>ICMAXBUF)
     {
          Parity_Check_Enable=0x00;
          return ICC_DATA_LENTHERR;
     }
     //���յ���Ӧ���ݳ���
     Le= (ushort)(ApduSend->Le);
     if(Le>256)     Le=256;
     s_IccSaveIntr();
     result=IccSendCommand( Cla,Ins, P1, P2,  Lc,ApduSend->DataIn,Le,card_Outputdata);
     s_IccRestoreIntr();
     Parity_Check_Enable=0x00;
     //ptr=ApduResp->DataOut;
     if((asyncard_info[current_slot].T==1) && (result==0))
     {
        //���ΪT=1Э���ҽ�����ȷʱ���н������ݸ�ʽת��
        iT1Le = card_Outputdata[0]*256 +card_Outputdata[1];
          
        if(iT1Le>=2)
        {
            recelen=iT1Le-2;
            // if(Le<recelen) recelen=Le;    //2006/08/06  ȥ����������peng
            if(recelen>0)
                memcpy(ApduResp->DataOut,&card_Outputdata[2],recelen);
            ApduResp->LenOut=recelen;
            ApduResp->SWA = card_Outputdata[iT1Le];
            ApduResp->SWB = card_Outputdata[iT1Le+1];
        }
        else
        {
            ApduResp->LenOut=0;
            ApduResp->SWA = 0x00;
            ApduResp->SWB = 0x00;
        }
        
     }
     else if(result==0)
     {
          //T=0Э������ҽ�����ȷʱ���и�ʽת��
          recelen=card_Outputdata[0]*256+card_Outputdata[1];
          //if(Le<recelen) recelen=Le;   //2006/08/06  ȥ����������peng
          if(recelen>0)
               memcpy(ApduResp->DataOut,&card_Outputdata[2],recelen);
          ApduResp->LenOut=recelen;
          ApduResp->SWA=SW1;
          ApduResp->SWB=SW2;
          //test
          trace_debug_printf("IccCommand SW[%02x][%02x]",SW1,SW2);
          for(i=0;i<recelen;i++)
            trace_debug_printf("%02x",ApduResp->DataOut[i]);
          
     }
     else
     {
          SW1=0;    
          SW2=0;
          ApduResp->LenOut=0;
          ApduResp->SWA=0;
          ApduResp->SWB=0;
          if(slot==0)
          {
               if(asyncard_info[0].existent==0x00)
                    return ICC_CARD_OUT;
          }
          if((result==ICC_T0_TIMEOUT)||(result==ICC_T1_BWTERR)||(result==ICC_T1_CWTERR))
               return ICC_ICCMESSOVERTIME;
     }
     return  result;
}

/**
 * ���ڷ���������������ݴ���
 */
static unsigned char scs_inverse_ch( unsigned char ch )
{
    int           i   = 0;
    unsigned char ich = 0;
    unsigned char tempts;
    
    tempts=asyncard_info[current_slot].TS;
    
    //if( scs_crg & CONV_EN )
    if(tempts==0x3f)
    {
        ich = ch ^ 0xFF;
        ch  = 0;
        for( i = 0; i < 8; i++ )
            if( ( ich & ( 1 << i ) ) )ch |= 1 << ( 7 - i );
    }
    
    return ch;
}

uchar Send_Byte(uchar dat)
{
    volatile uchar combuffer;  
    volatile uchar sc_sr; 
    
    uchar slot;
    //unsigned int cr,sr,etu0,etu1,tx,rx;
    //uchar channel;
    u32 i;
    
#if 0    
    //add
    USART_ITConfig( SC_SML_UART, USART_IT_RXNE, DISABLE );
    
    USART_ITConfig( SC_SML_UART, USART_IT_TC, ENABLE );
#endif
    
    slot=current_slot;
    
    switch(slot)
    {
    case 0: 
        break;
    case 2:
    case 1:
	//channel=slot;
        
        USART_SendData(SC_USART, scs_inverse_ch(dat));

        i = 0;
        while(USART_GetFlagStatus(SC_USART, USART_FLAG_TC) == RESET)
        {
            if(i++==0xffff)
            {
                return 0;   // wlb
            }
        }  
        //test
        //trace_debug_printf("send date end");
#if 0        
        cr=USIP_SCI1_CR;
        sr=USIP_SCI1_SR;
        etu0=USIP_SCI1_ETU0;
        etu1=USIP_SCI1_ETU1;
        tx=USIP_SCI1_TX;
        rx=USIP_SCI1_RX;
		channel=1;
#endif                
        break;
#if 0        
    case 1:
        cr=USIP_SCI2_CR;
        sr=USIP_SCI2_SR;
        etu0=USIP_SCI2_ETU0;
        etu1=USIP_SCI2_ETU1;
        tx=USIP_SCI2_TX;
        rx=USIP_SCI2_RX;
		channel=2;
        break;
#endif        
    } 
  
#if 0    
    if(Parity_Check_Enable==0x00)
    {
        s_sc_waitack(channel); 
        k_Icc_CR[slot] &= ~USIP_SCI_CR_CREP;
        Mips_write8(cr,k_Icc_CR[slot]);   
    }
    else
    {
        s_sc_waitack(channel); 
        k_Icc_CR[slot] |= USIP_SCI_CR_CREP;
        Mips_write8(cr,k_Icc_CR[slot]);
    }
    
    if(asyncard_info[slot].D==4)
    {
        s_sc_waitack(channel); 
        Mips_write8(etu0,0x5D);
        s_sc_waitack(channel); 
        Mips_write8(etu1,0x00);
    }
    else if(asyncard_info[slot].D==2)
    {
        s_sc_waitack(channel); 
        Mips_write8(etu0,0xBA);
        s_sc_waitack(channel); 
        Mips_write8(etu1,0x00); 
    }
    else 
    {
        s_sc_waitack(channel); 
        Mips_write8(etu0,0x74);
        s_sc_waitack(channel); 
        Mips_write8(etu1,0x01);
    }
    Icc_Io(slot,OUT); 
    combuffer=dat; 
  
    s_sc_waitack(channel); 
    k_Icc_CR[slot] &= ~USIP_SCI_CR_WTEN;
    Mips_write8(cr,k_Icc_CR[slot]);  
    
    s_sc_waitack(channel); 
    Mips_write8(tx,combuffer);        
             
    while(1)
    {
        s_sc_waitack(channel); 
        sc_sr=Mips_read8(sr);
        s_sc_waitack(channel); 
        sc_sr=Mips_read8(sr); 
        //s_UartPrint(COM_DEBUG,"%02x\r\n",sc_sr); 
        if((sc_sr & USIP_SCSR_PAR) == USIP_SCSR_PAR)
        {
            s_sc_waitack(channel); 
            k_Icc_SR[slot] &= ~USIP_SCSR_PAR;
            Mips_write8(sr,k_Icc_SR[slot]);
//             s_UartPrint(COM_DEBUG,"PARERR(S)\r\n");
            return 0xff;   
        }
        
        if((sc_sr & USIP_SCSR_RXCF) == USIP_SCSR_RXCF)
        {
            s_sc_waitack(channel); 
            k_Icc_SR[slot] &= ~USIP_SCSR_RXCF;
            Mips_write8(sr,k_Icc_SR[slot]); 
            s_sc_waitack(channel); 
            combuffer=Mips_read8(rx);  
            s_sc_waitack(channel); 
            combuffer=Mips_read8(rx); 
//             s_UartPrint(COM_DEBUG,"RECV(S)\r\n"); 
        } 
        if((sc_sr & USIP_SCSR_TXCF) == USIP_SCSR_TXCF)
        {
            s_sc_waitack(channel); 
            k_Icc_SR[slot] &= ~USIP_SCSR_TXCF;
            Mips_write8(sr,k_Icc_SR[slot]); 
            //s_UartPrint(COM_DEBUG,"S2(%02x)\r\n",combuffer); 
            break;
        } 
        //if((sc_sr & USIP_SCSR_WTOV) == USIP_SCSR_WTOV)
        //{
        //     
        //} 
        //if((sc_sr & USIP_SCSR_TXBE) == USIP_SCSR_TXBE)
        //{
        //    s_sc_waitack(0); 
        //    Mips_write8(USIP_SCI0_TX,combuffer); 
        //    s_UartPrint(COM_DEBUG,"S3(%02x)\r\n",combuffer);  
        //} 
    }
#endif
    
    return 0x00; 
}

 

void Dlynetu(ushort count)
{
#if 0  
     ushort i;
     for(i=0;i<count;i++)  s_IccDelayEtus_Send(1,0,0,0,0);
     s_IccDelayEtus_Send(0,5,0,0,0);  //148  xp 20070208
#endif     
}


void s_SetGTnetu(ushort count)
{
#if 0  
  USART_SetGuardTime(SC_USART,count);
#endif  
#if 0
     uchar hetu,letu; 
     hetu=(count>>8) & 0x01;
     letu=count & 0xff;
     switch(current_slot)
     {
     case 0: 
        break;
     case 2:
        s_sc_waitack(1); 
        Mips_write8(USIP_SCI1_GT1,hetu); 
        s_sc_waitack(1); 
        Mips_write8(USIP_SCI1_GT0,letu);
        break;
     case 1:
        s_sc_waitack(2); 
        Mips_write8(USIP_SCI2_GT1,hetu); 
        s_sc_waitack(2); 
        Mips_write8(USIP_SCI2_GT0,letu);
        break;
    }
#endif    
     
}
 

/***********************************
 ***********************************/
void Dly42500CLK(void)
{
  //!!!
  Lib_DelayMs(12);
    //for delay 42500*clk(=1/4M)  
#if 0
    unsigned short tempetu;
    unsigned int cur_cnt,temp_cnt,j,cmp_cnt; 
    tempetu=42500;    
    cmp_cnt=(unsigned int)tempetu*12;   
    cur_cnt=s_ReadMipsCpuCount();
    while(1)
    { 
        temp_cnt=s_ReadMipsCpuCount();
        if(temp_cnt>cur_cnt)
        {
            j=temp_cnt-cur_cnt;
        }
        else
        {
            j=temp_cnt+(0xffffffff-cur_cnt);
        }
        if(j>=cmp_cnt) break;        
    } 
#endif    
}

//   ��ʱn+0.r NETUs����ʱ��
//   ����ʱ1.5ETUSʱ��N��1��R��5
void s_IccDelayEtus_Send(int n_etus,int r_etu,int mend11,int mend12,int mend13)
{
#if 0  
    int netu,retu;
    unsigned long  nd;
    unsigned short tempetu,tempetu1;
    volatile unsigned int cur_cnt,temp_cnt,j,cmp_cnt; 
    nd=asyncard_info[current_slot].D; 
    netu=(372*n_etus)/(nd);     
    retu=(372*r_etu)/(nd*10);
    tempetu=netu+retu;
    switch(nd)
    {
    case 1:
        tempetu+=mend11; 
        break;
    case 2:
        tempetu+=mend12; 
        break;
    case 4:
        tempetu+=mend13; 
        break;
    }
    cmp_cnt=(unsigned int)tempetu*12;    
    //if(cmp_cnt>=540) cmp_cnt -= 540;  
    cur_cnt=s_ReadMipsCpuCount();
    while(1)
    { 
        temp_cnt=s_ReadMipsCpuCount();
        if(temp_cnt>cur_cnt)
        {
            j=temp_cnt-cur_cnt;
        }
        else
        {
            j=temp_cnt+(0xffffffff-cur_cnt);
        }
        if(j>=cmp_cnt) break;        
    }
#endif    
} 
 
/************************************************************
  ԭ�ͣ�  void nDelayMs(unsigned int count)
  ����������   ��ʱcount������
  ������  count - ��ʱ�ĺ�������0~65535��
  ************************************************************/
void nDelayMs(ushort count)
{
     Lib_DelayMs(count);
}

/***********************************************
   ���浱ǰ��ʱ���ж���Դ������ʼ��Ϊ��Ƭ��ʱ��
************************************************/
void s_IccSaveIntr(void)
{
    //int i;
    //��ر������жϣ��簴����ͨ�ţ���ʱ    
    
}



#if 0
//hard
/**
 * ��λ�źſ���
 */
//static int emv_hard_reset_signal( int channel, int level )
static int Icc_RST( uchar channel, uchar level )
{
    switch( channel )
    {
#if 0      
    case USC:
        if( level )GPIO_SetBits( ICC_GPIO_PORT_2, ICC_RST_PIN ); /*RST High*/
        else GPIO_ResetBits( ICC_GPIO_PORT_2, ICC_RST_PIN ); /*RST Low*/
    break;
#endif    
    case SAM1_CARD:
    case SAM2_CARD:
        //ȡ��
    	if( level )
          GPIO_ResetBits( SC_PIN_RESET_GPIO, SAM_RST_PIN ); /*RST Low->card high*/
        else 
          GPIO_SetBits( SC_PIN_RESET_GPIO, SC_PIN_RESET_SAM1 ); /*RST High->card Low*/
#if 0        
    	if( level )GPIO_SetBits( SC_PIN_RESET_GPIO, SC_PIN_RESET_SAM1 ); /*RST High*/
        else GPIO_ResetBits( SC_PIN_RESET_GPIO, SAM_RST_PIN ); /*RST Low*/
#endif        
    break;
#if 0    
    case SAM2:
    	if( level )GPIO_SetBits( SAM_GPIO_PORT_1, SAM_RST_PIN ); /*RST High*/
        else GPIO_ResetBits( SAM_GPIO_PORT_1, SAM_RST_PIN ); /*RST Low*/
    break;
    case SAM3:
    	if( level )GPIO_SetBits( SAM_GPIO_PORT_1, SAM_RST_PIN ); /*RST High*/
        else GPIO_ResetBits( SAM_GPIO_PORT_1, SAM_RST_PIN ); /*RST Low*/
    break;
    case SAM4:
    	if( level )GPIO_SetBits( SAM_GPIO_PORT_1, SAM_RST_PIN ); /*RST High*/
        else GPIO_ResetBits( SAM_GPIO_PORT_1, SAM_RST_PIN ); /*RST Low*/
    break;
#endif    
    default:
    break;
    }

    
	return 0;
}
#endif

/**
 * �첽��ʱ�ӿ���
 */
//static int emv_hard_clock_enable( int channel, int enable )
static  void   Icc_Clk(uchar channel,uchar enable)
{
	GPIO_InitTypeDef GPIO_InitStructure;
#if 0	
    if( USC == channel )
    {
		if( enable )
		{
		    SC_BIG_UART->CR2 |= 1 << 11;
		    
			GPIO_InitStructure.GPIO_Pin   = ICC_CLK_PIN; 
			GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
			GPIO_Init( ICC_GPIO_PORT_1, &GPIO_InitStructure );
		}
		else
		{
		   SC_BIG_UART->CR2 &= ~( 1 << 11 );
		   ICC_GPIO_PORT_1->ODR  &= ~ICC_CLK_PIN;
		   
		   GPIO_InitStructure.GPIO_Pin   = ICC_CLK_PIN;
		   GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
		   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
		   GPIO_Init( ICC_GPIO_PORT_1, &GPIO_InitStructure ); 
	    }
    }
    else
#endif      
    {
    	if( enable )
		{
		    //dbg_msg( 1, "CLK-EN(%d) = %X\r\n", channel, sam_clk_used_flag );
		    if( !sam_clk_used_flag )
		    {
		        SC_USART->CR2 |= 1 << 11; //clken
		    
			    GPIO_InitStructure.GPIO_Pin   = SC_USART_PIN_CK; 
			    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
			    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
			    GPIO_Init( SC_USART_GPIO, &GPIO_InitStructure );
		    }
		    sam_clk_used_flag |= 1 << channel;
		}
		else
		{
		    //dbg_msg( 1, "CLK-DIS(%d) = %X\r\n", channel, sam_clk_used_flag );
		    if( !( sam_clk_used_flag & ( ~( 1 << channel ) ) ) )
		    {
		        SC_USART->CR2 &= ~( 1 << 11 );
		        SC_USART_GPIO->ODR  &= ~SC_USART_PIN_CK;
		   
		        GPIO_InitStructure.GPIO_Pin   = SC_USART_PIN_CK;
		        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
		        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	            GPIO_Init( SC_USART_GPIO, &GPIO_InitStructure ); 
	        }
	        sam_clk_used_flag &= ~( 1 << channel );
	    }
	}
    
    return ;
	
}

/*************************************************
 *˵������CHANNELͨ��������IO�ڷ��������п���
 * MODE ��T/R(λ3 )     0   UART �趨Ϊ����ģʽ.
 *                      1   UART �趨Ϊ����ģʽ.
 *                FIP(λ6)       1:������ż�Ե������������
 *ע�⣺���ͬʱѡ�������÷�����ʱ������Щ�Ӳ�����
 *                ��Ӧ���������á�
 *************************************************/
void Icc_Io(uchar channel,uchar mode)
{
    switch(channel)
    {
    //case 0: 
    //    break;
    case 1:
    case 2://����
      /* ��������
        if(mode==OUT)
            GPIO_SetBits( SC_USART_GPIO, SC_USART_PIN_TX ); 
        else
            GPIO_ResetBits( SC_USART_GPIO, SC_USART_PIN_TX ); 
      */
        break;
#if 0        
    case 2: 
        if(mode==OUT)
            Mips_set16bit(USIP_GPIO_DR0,SAM2_IODIR); //2009.9.16 modified by tangdy,�޸���sam2��Ӧ��GPIO��
        else
            Mips_clr16bit(USIP_GPIO_DR0,SAM2_IODIR);  
        break;
#endif        
    default:  break;
    }
}


/*************************************************
 *˵������CHANNELͨ�������ĵ�Դ���п���
 *      MODE��   00     �µ�------OFF
 *               01     �ϵ�------ON
 *************************************************/
void   Icc_Vcc(uchar channel,uchar mode)
{
    //int i;  
    switch(channel)
    {
//    case 0: 
//        break;
      case 2:
      case 1:
    	if( mode==ON)
    	{
    	    if( !sam_vcc_used_flag )
            {
#if 0  //����init?
                USART3_Config();
#endif                
                //GPIO_ResetBits( SC_PIN_CMDVCC_GPIO, SC_PIN_CMDVCC );/*��Դδ�����*/
              GPIO_SetBits( SC_PIN_CMDVCC_GPIO, SC_PIN_CMDVCC );/*�رյ�Դ*/
    	    }
            sam_vcc_used_flag |=  1 << channel;
    	}
    	else 
    	{
    	    /*û������ͨ��ռ��*/
    	    if( !( sam_vcc_used_flag & ( ~( 1 << channel ) ) ) )
              //GPIO_SetBits( SC_PIN_CMDVCC_GPIO, SC_PIN_CMDVCC );/*�رյ�Դ*/
               GPIO_ResetBits( SC_PIN_CMDVCC_GPIO, SC_PIN_CMDVCC );/*��Դδ�����*/
            sam_vcc_used_flag &= ~( 1 << channel );
        }        
        break;
#if 0      
    case 2:
        for(i=1;i<IC_MAXSLOT;i++)
        {
            if((i!=channel) && (asyncard_info[i].open==VCC_OPEN))
                break;
        }
        if(i>=IC_MAXSLOT)
        {
            if(mode==OFF)
            { 
                Mips_clr16bit(USIP_GPIO_DR0,SAM_PWR);
            } 
            else
            {
                Mips_set16bit(USIP_GPIO_DR0,SAM_PWR);
            } 
        } 
        if(mode==ON)
        { 
            s_sc_waitack(1);
            k_Icc_CR[1] |= (0x03<<4);
            Mips_write8(USIP_SCI1_CR,k_Icc_CR[1]);  
            s_sc_waitack(1);  
            k_Icc_PR[1] |= USIP_SCI_PR_CRDVCC;
            Mips_write8(USIP_SCI1_PR,k_Icc_PR[1]);
            s_sc_waitack(1);
            k_Icc_EISSR[1] |= USIP_SCI_EISSR_VCARDOK;
            Mips_write8(USIP_SCI1_EISSR,k_Icc_EISSR[1]);    
        }
        else
        { 
            s_sc_waitack(1);
            k_Icc_CR[1] &= ~(0x03<<4);
            Mips_write8(USIP_SCI1_CR,k_Icc_CR[1]); 
            s_sc_waitack(1);
            k_Icc_PR[1] &= ~USIP_SCI_PR_CRDVCC ;
            Mips_write8(USIP_SCI1_PR,k_Icc_PR[1]);
            s_sc_waitack(1);
            k_Icc_EISSR[1] &= ~USIP_SCI_EISSR_VCARDOK;
            Mips_write8(USIP_SCI1_EISSR,k_Icc_EISSR[1]);
        }  
        break; 
    case 1: 
        for(i=1;i<IC_MAXSLOT;i++)
        {
            if((i!=channel) && (asyncard_info[i].open==VCC_OPEN))
                break;
        }
        if(i>=IC_MAXSLOT)
        {
            if(mode==OFF)
            { 
                Mips_clr16bit(USIP_GPIO_DR0,SAM_PWR);
            } 
            else
            {
                Mips_set16bit(USIP_GPIO_DR0,SAM_PWR);
            } 
        } 
        if(mode==ON)
        { 
            s_sc_waitack(2);
            k_Icc_CR[2] |= (0x03<<4);
            Mips_write8(USIP_SCI2_CR,k_Icc_CR[2]); 
            s_sc_waitack(2);  
            k_Icc_PR[2] |= USIP_SCI_PR_CRDVCC;
            Mips_write8(USIP_SCI2_PR,k_Icc_PR[2]); 
            s_sc_waitack(2);
            k_Icc_EISSR[2] |= USIP_SCI_EISSR_VCARDOK;
            Mips_write8(USIP_SCI2_EISSR,k_Icc_EISSR[2]);    
        }
        else
        { 
            s_sc_waitack(2);
            k_Icc_CR[2] &= ~(0x03<<4);
            Mips_write8(USIP_SCI2_CR,k_Icc_CR[2]);
            s_sc_waitack(2);
            k_Icc_PR[2] &= ~USIP_SCI_PR_CRDVCC ;
            Mips_write8(USIP_SCI2_PR,k_Icc_PR[2]); 
            s_sc_waitack(2);
            k_Icc_EISSR[2] &= ~USIP_SCI_EISSR_VCARDOK;
            Mips_write8(USIP_SCI2_EISSR,k_Icc_EISSR[2]);
        }  
        break;
#endif        
    default:
        break;
    }
}


/**
 * ��ʱ��ʱ���жϷ�����
 */
void Timer5_Isr(void)
{
  TIM5->SR = 0;  
  //test
  //trace_debug_printf("k_Ic_TotalTimeCount[%d]k_Ic_TimeCount[%d]",k_Ic_TotalTimeCount,k_Ic_TimeCount);
  
    if((k_IcMax_TotalTimeCount>0)&&(k_total_timeover==0))
    {
	k_Ic_TotalTimeCount++;
	if(k_Ic_TotalTimeCount>k_IcMax_TotalTimeCount)
	  k_total_timeover=1;
    }
    if((k_IcMax_TimeCount>0)&&(k_timeover==0))
    {
	k_Ic_TimeCount++;
        if(k_Ic_TimeCount>k_IcMax_TimeCount)
        {
          //test
          Icc_RST(1,HIGH);

            k_timeover=1;
        }
    }
    TIM5->SR = 0;  
    
}

/**
 * ��ʱ��ʱ������
 */
//static int USART_TOV_Timer( int enable, int etu )
 int USART_TOV_Timer( int enable, int etu )
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    
    TIM_Cmd( TIM5, DISABLE );
    TIM_TimeBaseStructure.TIM_Period        = etu - 1;      
    TIM_TimeBaseStructure.TIM_Prescaler     = 0x13;       
    TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;    
    TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up; 
    TIM_TimeBaseInit( TIM5, &TIM_TimeBaseStructure );
    
    TIM_ITConfig( TIM5, TIM_IT_Update, ENABLE );
    TIM_ClearITPendingBit( TIM5, TIM_IT_Update );
    if( enable )
    {
    	TIM_Cmd( TIM5, ENABLE );
    }
    
    return 0;	
}

//���õȴ�NETUʱ��
void Icc_SetWaitTime(void)
{
  //USART_TOV_Timer(1,372);
  USART_TOV_Timer(1,371);
}

void Icc_StopWaitTime(void)
{
  USART_TOV_Timer(0,0);
  
}
/**
 * �����ܿ�������1�ж�
 */

static void USART3_ISR_Clr()
{
	unsigned char ch;

	ch = SC_SML_UART->SR;
	ch = SC_SML_UART->DR;
	
        ch = ch;
    
	return;
}

/**
  * @brief  Resends the byte that failed to be received (by the Smartcard) correctly.
  * @param  None
  * @retval None
  */
void SC_ParityErrorHandler(void)
{
  USART_SendData(SC_USART, SCData);
  while(USART_GetFlagStatus(SC_USART, USART_FLAG_TC) == RESET)
  {
  } 
}


/**
 * ���ܿ�������3���жϷ�����
 */
void Usart3_Isr()
{
  /* If a Frame error is signaled by the card */
  if(USART_GetITStatus(SC_USART, USART_IT_FE) != RESET)
  {
    USART_ReceiveData(SC_USART);

    /* Resend the byte that failed to be received (by the Smartcard) correctly */
    SC_ParityErrorHandler();
  }
  
  /* If the SC_USART detects a parity error */
  if(USART_GetITStatus(SC_USART, USART_IT_PE) != RESET)
  {
    /* Enable SC_USART RXNE Interrupt (until receiving the corrupted byte) */
    USART_ITConfig(SC_USART, USART_IT_RXNE, ENABLE);
    /* Flush the SC_USART DR register */
    USART_ReceiveData(SC_USART);
  }
  
#if 0  
  //add
  if(USART_GetITStatus(SC_USART, USART_IT_TC) != RESET)
  {
    //USART3_ISR_Clr();
    USART_ITConfig( SC_USART, USART_IT_TC, DISABLE );

  }
#endif
  
  if(USART_GetITStatus(SC_USART, USART_IT_RXNE) != RESET)
  {
    /* Disable SC_USART RXNE Interrupt */
    USART_ITConfig(SC_USART, USART_IT_RXNE, DISABLE);
    USART_ReceiveData(SC_USART);  //not idle ?
    //test
    //trace_debug_printf("rece[%x]",USART_ReceiveData(SC_USART));
  }  
  
  /* If a Overrun error is signaled by the card */
  if(USART_GetITStatus(SC_USART, USART_IT_ORE) != RESET)
  {
    USART_ReceiveData(SC_USART);
  }
  
  /* If a Noise error is signaled by the card */
  if(USART_GetITStatus(SC_USART, USART_IT_NE) != RESET)
  {
    USART_ReceiveData(SC_USART);
  }  
}

#if 0
void Usart3_Isr()
{
    unsigned char ch         = 0;	
	int           parity_bit = 0;
    
//test
//trace_debug_printf("scs_stat[%d]",scs_stat);

	if( TWT_READY == twt_state ) // to start atr-wwt counter
	{
		twt_state = TWT_START;
	}
	
	if( SC_UART_RECV_EV & scs_stat )
	{ 	    		
		if( SC_SML_UART->SR & ( 1 << 5 ) )//USART_FLAG_RXNE
		{   
			wait_counter       =   scs_cwt;
			work_step          =   SC_UART_WAIT_EVENT;

			if( scs_stat & SC_UART_WAIT_TS ) // TS character 
			{    
				parity_bit = SC_SML_UART->DR;	
				if( 0x0103 == parity_bit )
				{         
					ch        =  0x3F;
					scs_crg  |=  CONV_EN;					
					SC_SML_UART->CR1 |= 1 << 10;    // parity enable
					SC_SML_UART->CR1 |= 1 << 9;    //odd parity
				}
				else if( 0x013B == parity_bit )
				{    
					ch 	     = 0x3B;
					scs_crg &= ~CONV_EN;				
					SC_SML_UART->CR1 |=  ( 1 << 10 );    // parity enable
					SC_SML_UART->CR1 &= ~( 1 << 9 );   // even parity 
				}
				else
				{
					ch = 0xff;  // TS error
				}
#if 0												
				scs_queue[ 1 ].qbuf[ scs_queue[ 1 ].ip ] = ch;
				scs_queue[ 1 ].ip++;
#endif                                
                                gbScsQbuf[0] = ch;
                                gbScsIp++;
                                
                                //test
                                trace_debug_printf("11gbScsIp[%d]parity_bit[%x]",gbScsIp,parity_bit);
				USART_ITConfig( SC_SML_UART, USART_IT_RXNE, DISABLE );  	   
                                
				scs_stat &= ~SC_UART_WAIT_TS;
				return ;
				
			}

			if( SC_SML_UART->SR & 1 )   // parity error,USART_FLAG_PE
			{
				ch = ( unsigned char )SC_SML_UART->DR;					
				//if( scs_crg & CRPT_EN )	//T=0;
                                if(    (asyncard_info[current_slot].T==0) &&
                                     asyncard_info[current_slot].resetstatus)
  
				{
					if( repeat_chc != 0 )
					{
						repeat_chc--;							
						return;
					}
					else
					{
						//scs_crg |= PARE_SB;
                                                k_IccErrPar = 1;
					}
				}
				else 	// T=1 or ATR 
				{	
					//scs_crg |= PARE_SB;
                                        k_IccErrPar =1;
                                        
				}
			}
#if 0
			if( scs_crg & PARE_SB )	
			{
				scs_crg &= ~PARE_SB;
			
				if( 0 == scs_queue[ 1 ].sf )
				{
					scs_queue[ 1 ].sf = ICC_ERR_PARS;
					scs_queue[ 1 ].pf = scs_queue[ 1 ].ip;
				}
				return ;
			}	
#endif			
			repeat_chc = 4; 
#if 0                        
			ch = ( unsigned char )SC_SML_UART->DR;		
			scs_queue[ 1 ].qbuf[ scs_queue[ 1 ].ip ] = scs_inverse_ch(ch);
			scs_queue[ 1 ].ip++;
			if( MAX_QBUF_SIZE == scs_queue[ 1 ].ip )
			{
				scs_queue[ 1 ].ip = 0; 
			}					
#endif		
                        //test
			ch = ( unsigned char )SC_SML_UART->DR;		
                        gbScsQbuf[0] = ch;
                        gbScsIp++;
			USART_ITConfig( SC_SML_UART, USART_IT_RXNE, DISABLE );  	   
                        
		}  		
    }
	
#if 0    
    if( SC_UART_SEND_EV & scs_stat )
    {					
        if( SC_SML_UART->SR & ( 1 << 6 ) )     //TC flag is set,USART_FLAG_TC
        {      
                //USART_FLAG_RXNE  && USART_FLAG_FE
        	if( ( SC_SML_UART->SR & ( 1 << 5 ) ) && ( SC_SML_UART->SR & ( 1 << 1 ) ) ) //frame error, four time re-send
        	{    	
				USART3_ISR_Clr();
				if( scs_crg & CRPT_EN )
				{
	        		if( repeat_chc-- )
	        		{						
			            ch = scs_queue[ 1 ].qbuf[ scs_queue[ 1 ].op - 1 ];
			            SC_SML_UART->DR = scs_inverse_ch( ch );		
					}
					else
					{
						scs_queue[1].sf = ICC_ERR_PARS;
						scs_queue[1].pf = 0;
						scs_queue[1].ip = 1;
						scs_queue[1].op = 0;		
						USART_ITConfig( SC_SML_UART, USART_IT_TC,   DISABLE );
						USART_ITConfig( SC_SML_UART, USART_IT_RXNE, DISABLE );  	   
					}
				}
				
        	}
		else
		{		
			USART3_ISR_Clr();
			repeat_chc = 4;

			if( (int)( scs_queue[ 1 ].ip ) != (int)( scs_queue[ 1 ].op ) )
		        {
		            ch = scs_queue[ 1 ].qbuf[ scs_queue[ 1 ].op ];
		            scs_queue[ 1 ].op++;
					
		            if( MAX_QBUF_SIZE == scs_queue[ 1 ].op )
		            {
		                scs_queue[ 1 ].op = 0;
		            }    
					
			    if( (int)( scs_queue[ 1 ].ip ) == (int)( scs_queue[ 1 ].op ) )
			    {
				USART_SetGuardTime( SC_SML_UART, 5 ); // delay for re-sending  while frame error	
			    }
										
		            SC_SML_UART->DR = scs_inverse_ch( ch );	
					
		        }
		        else
		        {          
		            scs_stat        =  SC_UART_RECV_EV;	 // send completed, convert to RX mode 
			    wait_counter	=  scs_bwt; 
			    work_step	    =  SC_UART_WAIT_EVENT;	
					
			    USART3_ISR_Clr();	
			    USART_ITConfig( SC_SML_UART, USART_IT_TC,   DISABLE );			
			    USART_ITConfig( SC_SML_UART, USART_IT_RXNE, ENABLE );  
					
			    USART_TOV_Timer( ENABLE, scs_etu );		
			}
		}						
        }
    }
#endif
    
    return;
}
#endif


/***********************************************
 *  �趨ICCISOCOMMAND�����Ƿ��Զ�����GET RESPָ�
 *  ����AUTORESP=1����Ϊ�Զ���=0����Ϊ���Զ���
 *  �������ǰͨ��IC��״̬�ṹ�е�CARD-TYPE�е�
 *        BIT0=0�Զ���BIT0=1���Զ���
 ************************************************/
int Lib_IccSetAutoResp(uchar slot,uchar autoresp)
{
    slot=slot&0x7f;
    slot=slot&0xbf;  //add 20080508
    if(slot>=IC_MAXSLOT)
          return ICC_SLOTERR;
    //if(slot==0)
    //{
        if(autoresp == 1)
            asyncard_info[slot].autoresp=0;
        else if(autoresp ==0) //not auto
            asyncard_info[slot].autoresp=1;
    //}
    return 0;
}

/*******************************************
 *  ���ָ����SLOT�������Ƿ��п���
 *  ���SLOT=0��1ʹ�õ�λ��⣨ѡͨMC33560��/CS
 *  ����ȡRDYMOD�ź�״̬��H-�п���L-�޿�����
 *  ����SAM���������ϵ縴λ��Ӧ������⣻
 *  ����ֵ�� 0X00=�п���0xff=�޿�
 ********************************************/
int  Lib_IccCheck(uchar slot)
{
    //uchar status=0xff;
    //uchar ch1,ch2,ch3;
    int ret;
     
    slot=slot&0x7f;   //add 20080508
    slot=slot&0xbf;   //add 20080508 
     
    if(slot>=IC_MAXSLOT)
        return ICC_SLOTERR;
    switch(slot)
    {
    case USER_CARD: 
        //return ICC_CARD_OUT; 
        return ICC_SLOTERR;
    case SAM1_CARD:
    case SAM2_CARD:
        Lib_IccClose(slot);
        ret=IccColdRest(slot);
        Lib_IccClose(slot);
        if(ret)
        {
            asyncard_info[slot].existent=0x00; 
            return ICC_CARD_OUT;
        }
        else
        {
            asyncard_info[slot].existent=0x01; 
            return SUCCESS;
        }
        //break;

    default :
        break;
    }
    return ICC_CARD_OUT;
}

//#endif

