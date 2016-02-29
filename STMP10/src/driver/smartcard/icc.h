#ifndef  _ICCARD_H
#define  _ICCARD_H
 
#ifndef   uchar
#define   uchar   unsigned char
#endif
#ifndef   uint
#define   uint    unsigned int
#endif
#ifndef   ushort  
#define   ushort  unsigned short
#endif
#ifndef   ulong
#define   ulong   unsigned long
#endif

#define   IN               1
#define   OUT              0
#define   LOW              0
#define   HIGH             1
#define   ON               1
#define   OFF              0
#define   Enable           1
#define   DisEnable        0
#define   CLEAR            0
/*******************************************
             IC������״̬
 *******************************************/
#define SUCCESS                    0
#define ICC_VCCMODEERR             (-2500)   //   ��ѹģʽ���� 
#define ICC_INPUTSLOTERR           (-2501)   //   ѡ��ͨ���ڴ��� 
#define ICC_VCCOPENERR             (-2502)   //
#define ICC_ICCMESERR              (-2503)   //   ��ͨѶʧ�� 

#define ICC_T0_TIMEOUT             (-2200)   //   �ȴ���Ƭ��Ӧ��ʱ   
#define ICC_T0_MORESENDERR         (-2201)   //   �ط����� 
#define ICC_T0_MORERECEERR         (-2202)   //   ���մ��� 
#define ICC_T0_PARERR              (-2203)   //   �ַ���ż���� 
#define ICC_T0_INVALIDSW           (-2204)   //   ״̬�ֽ���Ч 

#define ICC_DATA_LENTHERR          (-2400)   //   ���ݳ��ȴ���   
#define ICC_PARERR                 (-2401)   //   ��ż����       
#define ICC_PARAMETERERR           (-2402)   //   ����ֵΪ�� 
#define ICC_SLOTERR                (-2403)   //   ��ͨ������    
#define ICC_PROTOCALERR            (-2404)   //   Э�����      
#define ICC_CARD_OUT               (-2405)   //   ������        
#define ICC_NO_INITERR             (-2406)   //   û�г�ʼ��    
#define ICC_ICCMESSOVERTIME        (-2407)   //   ��ͨѶ��ʱ 

#define ICC_ATR_TSERR              (-2100)   //   ������Լ������,TS���� 
#define ICC_ATR_TCKERR             (-2101)   //   ��λУ��(T=1,TCK����)����     
#define ICC_ATR_TIMEOUT            (-2102)   //   ��λ�ȴ���ʱ     
#define ICC_TS_TIMEOUT             (-2115)   //   �ַ������ʱ     
#define ICC_ATR_TA1ERR             (-2103)   //   TA1����          
#define ICC_ATR_TA2ERR             (-2104)   //   TA2����          
#define ICC_ATR_TA3ERR             (-2105)   //   TA3����          
#define ICC_ATR_TB1ERR             (-2106)   //   TB1����          
#define ICC_ATR_TB2ERR             (-2107)   //   TB2����          
#define ICC_ATR_TB3ERR             (-2108)   //   TB3����          
#define ICC_ATR_TC1ERR             (-2109)   //   TC1����          
#define ICC_ATR_TC2ERR             (-2110)   //   TC2����          
#define ICC_ATR_TC3ERR             (-2111)   //   TC3����          
#define ICC_ATR_TD1ERR             (-2112)   //   TD1����          
#define ICC_ATR_TD2ERR             (-2113)   //   TD2����          
#define ICC_ATR_LENGTHERR          (-2114)   //   ATR���ݳ��ȴ���   

#define ICC_T1_BWTERR              (-2300)   //   T=1����ȴ�ʱ�����   
#define ICC_T1_CWTERR              (-2301)   //   T=1�ַ��ȴ�ʱ�����  
#define ICC_T1_ABORTERR            (-2302)   //   �쳣(ABORT)ͨ�Ŵ���  
#define ICC_T1_EDCERR              (-2303)   //   ����У����(EDC)����  
#define ICC_T1_SYNCHERR            (-2304)   //   ͬ��ͨ�Ŵ��� 
#define ICC_T1_EGTERR              (-2305)   //   �ַ�����ʱ�����   
#define ICC_T1_BGTERR              (-2306)   //   ���鱣��ʱ�����    
#define ICC_T1_NADERR              (-2307)   //   ������NAD����        
#define ICC_T1_PCBERR              (-2308)   //   ����PCB����          
#define ICC_T1_LENGTHERR           (-2309)   //   ����LEN����          
#define ICC_T1_IFSCERR             (-2310)   //   IFSC����             
#define ICC_T1_IFSDERR             (-2311)   //   IFSD����             
#define ICC_T1_MOREERR             (-2312)   //   ��δ��ʹ��󲢷���   
#define ICC_T1_PARITYERR           (-2313)   //   �ַ���ż���� 
#define ICC_T1_INVALIDBLOCK        (-2314)   //   ��Ч������ 

#define ICC_ER_DAIN                (-2600)   //   ERror: Driver Already INitialized 
#define ICC_ER_DNIN                (-2601)   //   ERror: Driver Not INitialized 
#define ICC_ER_NOCD                (-2602)   //   ERror: NO sim CarD is present  
#define ICC_ER_SYSF                (-2603)   //   ERror: SYStem Failure  
#define ICC_ER_TMOT                (-2604)   //   ERror: TiMeOuT  
#define ICC_ER_AFTM                (-2605)   //   ERror: Atr arrived at False TiMe  
#define ICC_ER_INVA                (-2606)   //   ERror: INValid Atr  
#define ICC_ER_PAER                (-2607)   //   ERror: PArity ERror ��żЧ����� 
#define ICC_ER_FRAM                (-2608)   //   ERror: FRAMing error 
#define ICC_ER_EDCO                (-2609)   //   ERror: Error Detection Code error  
#define ICC_ER_INFR                (-2610)   //   ERror: INvalid FRequency         
#define ICC_ER_INFN                (-2611)   //   ERror: INvalid FN                
#define ICC_ER_INDN                (-2612)   //   ERror: INvalid DN                
#define ICC_ER_INPA                (-2613)   //   ERror: INvalid PArameter         
#define ICC_ER_TOPS                (-2614)   //   ERror: Timeout for PpS response  
#define ICC_ER_INPS                (-2615)   //   ERror: INvalid PpS response      
#define ICC_ER_DOVR                (-2616)   //   ERror: Data OVeRflow on fifo reception buffer �����ַ���� 
#define ICC_ER_NSFN                (-2617)   //   ERror: Not Supported FN          
#define ICC_ER_NSDN                (-2618)   //   ERror: Not Supported DN          
#define ICC_ER_NSPR                (-2619)   //   ERror: Not Supported PRotocol    
#define ICC_ER_MEMF                (-2620)   //   ERror: MEMory Failure            

#define  SEND_OK            0          //SEND data OK  
#define  RECE_OK            0          // RECE data OK  

#define   IC_MAXSLOT       3   //4->3
#define   USER_CARD        0
#define   SAM1_CARD        1
#define   SAM2_CARD        2
#define   SAM3_CARD        3

#define   VCC_OPEN         1
#define   VCC_CLOSE        0

#define   CRD_VCC_0V       0
#define   CRD_VCC_5V       1
#define   CRD_VCC_3V       2
#define   CRD_VCC_1_8V     3

#define TWT_READY  			       0
#define TWT_START  			       1
#define TWT_STOP   			       2

#define SC_UART_WAIT_TS          ( 1 << 0 )    //
#define SC_UART_RECV_EV          ( 1 << 1 )    //
#define SC_UART_SEND_EV          ( 1 << 2 )    //

#define SC_RECEIVE_TIMEOUT 0x4000  /* Direction to reader */

/* Define the STM32F10x hardware depending on the used evaluation board */

 #define CARD_V2

  //hardware
  #define SC_USART                 USART3

  #define SC_SML_UART              USART3

  #define SC_USART_GPIO            GPIOB
  #define SC_USART_CLK             RCC_APB1Periph_USART3
  #define SC_USART_GPIO_CLK        RCC_APB2Periph_GPIOB
  #define SC_USART_PIN_TX          GPIO_Pin_10
  #define SC_USART_PIN_CK          GPIO_Pin_12
  //#define SC_USART_IRQn            USART3_IRQn
  #define SC_USART_IRQn            USART3_IRQChannel

  #define SC_USART_IRQHandler      USART3_IRQHandler
  /* Smartcard Inteface GPIO pins */
  #define SC_PIN_3_5V_SAM1               GPIO_Pin_0
  #define SC_PIN_3_5V_SAM2               GPIO_Pin_1

  #define SC_PIN_3_5V_GPIO          GPIOB
  #define SC_PIN_3_5V_GPIO_CLK      RCC_APB2Periph_GPIOB
  #define SC_PIN_RESET_SAM1              GPIO_Pin_3
#ifndef CARD_V2
  #define SC_PIN_RESET_SAM2              GPIO_Pin_4
#endif
  #define SC_PIN_RESET_GPIO         GPIOC
  #define SC_PIN_RESET_GPIO_CLK     RCC_APB2Periph_GPIOC
  #define SC_PIN_CMDVCC             GPIO_Pin_2
  #define SC_PIN_CMDVCC_GPIO        GPIOC
  #define SC_PIN_CMDVCC_GPIO_CLK    RCC_APB2Periph_GPIOC

//���ܿ����
  #define SC_PIN_OFF                GPIO_Pin_7 
  #define SC_PIN_OFF_GPIO           GPIOC
  #define SC_PIN_OFF_GPIO_CLK       RCC_APB2Periph_GPIOC


  #define SC_DETECT_EXTI            EXTI_Line7
  #define SC_DETECT_PIN             GPIO_PortSourceGPIOC
  #define SC_DETECT_GPIO            GPIO_PinSource7
  #define SC_DETECT_IRQ             EXTI9_5_IRQn
  #define SC_DETECT_IRQHandler      EXTI9_5_IRQHandler


typedef struct //__attribute__ ((__packed__)) 
{
     uchar open;         //   0 �ر� 1 ��   

     uchar existent;     //   0 �޿� 1 �п�  
     uchar resetstatus;  //   0-û�и�λ��1-���临λ��2-���ȸ�λ  
     uchar autoresp;     //   �Զ�Ӧ�����ã�0=�Զ�Ӧ��  1=���Զ�Ӧ��  
     
     uchar D;            //   �޸��첽ͨ�Ų����ʱ�������    
     uchar T;            //   ��ƬЭ������  ֵΪ0��1  
     uchar TS;           //   ����������Լ��  3B=���� 3F=����  

     uchar ATR_len;      //   ��Ƭ��λӦ�𳤶�  
     uchar ATR[40];      //   ��Ƭ��λӦ������  
     
     uchar TA;           //   ��4λ��Ч����Ӧ��TA1��TA2��TA3,TA4�Ĵ���   
     uchar TA1;          //   ��Ƭ��λ�ӿ��ַ�    
     uchar TA2;          //   ��Ƭ��λ�ӿ��ַ�   
     uchar TA3;          //   ��Ƭ��λ�ӿ��ַ�   
     uchar TA4;          //   ��Ƭ��λ�ӿ��ַ�    
     
     uchar TB;           //   ��4λ��Ч����Ӧ��TB1��TB2��TB3,TB4�Ĵ���   
     uchar TB1;          //   ��Ƭ��λ�ӿ��ַ�    
     uchar TB2;          //   ��Ƭ��λ�ӿ��ַ�    
     uchar TB3;          //   ��Ƭ��λ�ӿ��ַ�   
     uchar TB4;          //   ��Ƭ��λ�ӿ��ַ�   
     
     uchar TC;           //   ��4λ��Ч����Ӧ��TC1��TC2��TC3,TC4�Ĵ���   
     uchar TC1;          //   ��Ƭ��λ�ӿ��ַ�    
     uchar TC2;          //   ��Ƭ��λ�ӿ��ַ�    
     uchar TC3;          //   ��Ƭ��λ�ӿ��ַ�    
     uchar TC4;          //   ��Ƭ��λ�ӿ��ַ�   
     
     uchar TD;           //   ��4λ��Ч����Ӧ��TD1��TD2��TD3,TD4�Ĵ���     
     uchar TD1;          //   ��Ƭ��λ�ӿ��ַ�    
     uchar TD2;          //   ��Ƭ��λ�ӿ��ַ�    
     uchar TD3;          //   ��Ƭ��λ�ӿ��ַ�    
          
     uchar IFSC;         //   ��Ƭ�������������Ϣ����    
     uchar IFSD;         //   �ն��������������Ϣ����   
     
     uchar term_pcb;     //   T=1Э�����ն˷����¸��ֶθ�ʽ�����к�   
     uchar card_pcb;     //   T=1Э���¿������¸��ֶθ�ʽ�����к�    
     
     uchar sw1;          //   ״̬�ֽ�1   
     uchar sw2;          //   ״̬�ֽ�2   
}ASYNCARD_INFO;

typedef struct
{
    uchar qbuf[300];
    uchar ip;
    uchar op;
    uchar sf;
    uchar pf;
}emv_queue;

#if 0
struct emv_core:

terminal_ch  //slot
terminal_ptype //protocol
queue
terminal_di
terminal_implict_di
terminal_fi
terminal_implict_fi
terminal_conv  //����
terminal_cgt //? guard
terminal_bwt
terminal_cwt
terminal_wwt
terminal_vcc
terminal_state //EMV_COLD_RESET
terminal_spec //emv 7816
#endif
//hard
#define FCLK_CARD     ( 3600000UL )


#endif