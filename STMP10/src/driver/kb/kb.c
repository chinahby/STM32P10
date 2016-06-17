#include "KF701DH.h"
#include "stm32f10x_lib.h"
#include "..\..\inc\FunctionList.h"

#include  "kb.h"
#include  "base.h"
#include  "comm.h"
#include  "vosapi.h"
#include "buzzer.h"

//static  const DWORD BeepFreqTab[8]={0,493,453,403,378,328,278,253};
static  const DWORD  BeepFreqTab[8]={0,100,200,400,800,1600,2000,2500}; 

extern BYTE k_LcdBackLightMode;
#if 0
static  const BYTE Key_Tab[26]={
	KEYCANCEL, KEYCLEAR, KEYF2,    KEYF1, 
	KEY0,      KEY3,     KEY2,     KEY1,
	KEYENTER,  KEY6,     KEY5,     KEY4,
	0,         KEY9,     KEY8,     KEY7,      
	0,         0,        0,        0,  
	0,         0,        0,        0,
	0,         0
	};
#endif
static  const BYTE Key_Tab[26]={
	KEYF1, KEYF2, KEYCLEAR ,    KEYCANCEL, 
	KEY1,      KEY2,     KEY3,     KEY0,
	KEY4,  KEY5,     KEY6,     KEYENTER,
	KEY7,         KEY8,     KEY9,     KEY7,      
	0,         0,        0,        0,  
	0,         0,        0,        0,
	0,         0
	};

#define  KB_BUF_MAX      32
volatile static BYTE kb_buf[KB_BUF_MAX]; /* ��ȡ��ֵ������ */
volatile static BYTE kb_in_ptr;          /* �����ַָ��   */
volatile static BYTE kb_out_ptr;         /* ������ַָ��   */
volatile static BYTE kb_buf_over;        // ��ֵ�����������0��ʾδ�����1��ʾ���
volatile static BYTE kb_backcode;

volatile static BYTE kb_KeyCode1;        // ����������
volatile static BYTE kb_KeyCode2;        // ����������
volatile static int  kb_TimerScanHandle; // ���水��ɨ�趨ʱ�����
volatile static int  kb_TimerStep;       // ���涨ʱ��ɨ�貽�裬��ȡֵ0,1,2
volatile static int  kb_OffTimerCount;
volatile static DWORD  kb_SoundFrequency;/*��������Ƶ��*/
volatile static int  kb_SoundDelay;      /*��������������ʱ*/
//volatile static int  kb_SoundTempDelay;
volatile static int  kb_BeepIntBusy;
//volatile static int  kb_BeepFunBusy;
volatile static int  kb_Buffer_Busy; // �Ƿ������建�壬0��ʾ��1��ʾ�����建��

volatile int  g_KbIntrStatus;//����ָʾ��ǰ�����жϴ����״̬��0������

int g_iKbStopSoundFlag = 0;  // ����������ֹ��־��1��ֹ��0����ֹ
int g_iAuthForNumberKeyFlag=0;   //=1 auth success  =0  auth error or no auth
int g_iAppCallGetChFlag=0;       //=1 calling app   =0  calling vos

extern BYTE  g_byStartX, g_byStartY;
extern BYTE     g_byFontType;
extern BYTE     g_byFontAttr;
DWORD g_dwPreDisplayLen = 0;

//extern FONT_ATTR g_sFontAttr;
volatile static int g_iKeyScanFlag = 0;  // ����ɨ���־ 0��ɨ�裬1��ɨ��

void   s_TimerScanKey(void);
BYTE   kb_ScanKey(int mode);


static void Enable_KB_irq(void)
{
    EXTI_InitTypeDef EXTI_InitStructure ;
    EXTI_InitStructure.EXTI_Line = Keyboard_EXTI_Line;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt ;
    //EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising ;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling ;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE ;
    EXTI_Init(&EXTI_InitStructure) ;
  
  
}

static void Disable_KB_irq(void)
{
    EXTI_InitTypeDef EXTI_InitStructure ;
    EXTI_InitStructure.EXTI_Line = Keyboard_EXTI_Line;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt ;
    //EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising ;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling ;
    EXTI_InitStructure.EXTI_LineCmd = DISABLE ;
    EXTI_Init(&EXTI_InitStructure) ;
  
  
}

/****************************************************************************
  ������     :
  ����       :
  �������   :
  �������   :
  ����ֵ     :
  �޸���ʷ   :
      �޸���     �޸�ʱ��    �޸İ汾��   �޸�ԭ��
  1��
****************************************************************************/
int s_KbInit(void)
{
    int i;
    GPIO_InitTypeDef GPIO_InitStructure;
    
    EXTI_DeInit();
    GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8; //-in
    //GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_IN_FLOATING;
    //GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_IPD;
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    
    EXTI_ClearITPendingBit(EXTI_Line5) ;   //����жϱ�־λ
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC ,GPIO_PinSource5 ); //Pinsource����ȡ��
    EXTI_ClearITPendingBit(EXTI_Line6) ;   //����жϱ�־λ
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC ,GPIO_PinSource6 ); 
    EXTI_ClearITPendingBit(EXTI_Line7) ;   //����жϱ�־λ
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC ,GPIO_PinSource7 );
    EXTI_ClearITPendingBit(EXTI_Line8) ;   //����жϱ�־λ
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC ,GPIO_PinSource8 );

    //����Row1-Row4Ϊ�½����ж�
    Enable_KB_irq();
    
    
    
    GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12; //-out
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    //GPIO_SetBits(GPIOC,GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12);
    //GPIO_SetBits(GPIOC,GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12);
    GPIO_ResetBits(GPIOC ,GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12) ; 
  
    //��ʼ��������ֵ
    for(i=0;i<KB_BUF_MAX;i++)
    {
        kb_buf[i]=0xff;
    }

    kb_in_ptr = 0;
    kb_out_ptr = 0;
    kb_buf_over = 0;
    kb_backcode = 0;

    kb_KeyCode1 = NOKEY;
    kb_KeyCode2 = NOKEY;


    kb_TimerScanHandle = 0;
    //kb_TimerStep = 0;
    kb_OffTimerCount = 0;

    //kb_SoundFrequency = BeepFreqTab[4];
    kb_SoundFrequency = BeepFreqTab[7];
    kb_SoundDelay = 1;
    //kb_SoundTempDelay = 1;
    kb_BeepIntBusy = 0;
    //kb_BeepFunBusy = 0;
    kb_Buffer_Busy = 0;

    g_iKbStopSoundFlag = 0;
    g_iAuthForNumberKeyFlag=0;
    g_iAppCallGetChFlag=0;

    g_KbIntrStatus=0; 
    return 0;
}

void TimerScanBegin()
{
     Disable_KB_irq();
     kb_TimerStep = 0;
     kb_OffTimerCount = 0;
     g_KbIntrStatus = 1;
     g_iKeyScanFlag = 1;
     //add 12/11/16
     KillTimerEvent(kb_TimerScanHandle);
     kb_TimerScanHandle = SetTimerEvent(2, s_TimerScanKey); // ÿ��20msɨ��һ��
  
}

void LocBeep()
{
extern unsigned char BeepFlag;
extern unsigned short BeepDelay;

#if 0
    //test
       Lib_LcdPrintxy(0,0,0,"kb_SoundDelay[%d][%d]",kb_SoundDelay,kb_SoundFrequency);
        Lib_KbGetCh();
#endif        
    if(kb_SoundDelay==0)
    {
      Buzzer_Off();
      BeepFlag=0;
      
      
    }
  //Buzzer_Ring(kb_SoundFrequency); 
    //if(kb_SoundDelay)
    if(kb_SoundDelay &&  (BeepFlag==0) )
    {
      //Buzzer_Init(kb_SoundFrequency+600);
      //Buzzer_Init(1100);
      //Buzzer_Init(900);
      //Buzzer_Init(3200);
      Buzzer_Init(kb_SoundFrequency+500);
      
      //Buzzer_Init(nFrequency);
      if(kb_SoundDelay<7)
        BeepDelay = 7;  //7=
      else  
        BeepDelay = kb_SoundDelay;  //7->kb_SoundDelay*7->kb_SoundDelay
      BeepFlag = 1;
      //delay_ms(70);
      //TIM_Cmd(TIM4, DISABLE); //
    }
  
}

/****************************************************************************
  ������     :  void s_TimerScanKey(void)
  ����       :  ������Ӧ���������а���ʱ�ú��������ã�û�а����ú������ᱻ���á�
  �������   :  ��
  �������   :  ��
  ����ֵ     :  ��
  �޸���ʷ   :
      �޸���     �޸�ʱ��    �޸İ汾��   �޸�ԭ��
  1��
****************************************************************************/
void s_TimerScanKey(void)
{
    unsigned char ucTemp1, ucTemp2 = 0;
    //g_iKeyScanFlag = 1;
    //s_UartPrint(COM_DEBUG,"t1");
    switch(kb_TimerStep)
    {
    case 0:
        kb_KeyCode1=kb_ScanKey(1);
        if(kb_KeyCode1==NOKEY)
        {
            kb_OffTimerCount++;
            if (kb_OffTimerCount > 3)//3
            {
                kb_OffTimerCount = 0;
                kb_TimerStep = 2;
            }
        }
        else
        {
            kb_TimerStep++;
        }
        break;
    case 1:
        ucTemp1 = kb_backcode;
        ucTemp2 = kb_KeyCode1;
        //if(kb_backcode!=kb_KeyCode1)
        if (ucTemp1 != ucTemp2)
        {
            kb_KeyCode2=kb_KeyCode1;
        }
        else
        {
            kb_KeyCode2=kb_ScanKey(1);
        }

        ucTemp1 = kb_KeyCode2;
        ucTemp2 = kb_KeyCode1;
        //if(kb_KeyCode2==kb_KeyCode1)
        if (ucTemp1 == ucTemp2)
        {
            if(kb_KeyCode2<20)  // ����������
            {
                /*if(Key_Tab[kb_KeyCode2]==KEYPRINTERUP)
                {
                    s_ManualFeedPaper(1);
                }
                else */if((kb_buf_over==0)&&(kb_Buffer_Busy==0))
                {
                    ucTemp1 = Key_Tab[kb_KeyCode2];
                    //kb_buf[kb_in_ptr]=Key_Tab[kb_KeyCode2];
                    kb_buf[kb_in_ptr] = ucTemp1;
                    kb_in_ptr++;
                    if(kb_in_ptr>=KB_BUF_MAX)
                        kb_in_ptr=0;

                    ucTemp1 = kb_in_ptr;
                    ucTemp2 = kb_out_ptr;
                    //if(kb_in_ptr==kb_out_ptr)
                    if(ucTemp1 == ucTemp2)
                    {
                        kb_buf_over=1;
                    }
                }
                kb_backcode=kb_KeyCode2;
            }
            
            if(k_LcdBackLightMode==1)
              Lib_LcdSetBackLight(1);
            
            //s_LcdBackLightRestart();
            //Lib_LcdSetBackLight(1);
            //s_TimerKeySound(0);
            LocBeep();
#if 0            
            Lib_KbSound(0,0);
#endif            
        }
        else
        {
            kb_KeyCode1=kb_KeyCode2;
        }
        kb_TimerStep++;
        break;
    case 2:
#if 0
        LocBeep();
#endif        
        //s_TimerKeySound(1);
#if 0        
        Lib_KbSound(1,1);
#endif        
        kb_KeyCode2 = kb_ScanKey(0);
        if(kb_KeyCode2 == NOKEY)
        {
            kb_OffTimerCount++;
            if (kb_OffTimerCount > 1)
            {
                kb_OffTimerCount = 0;
#if 0                
                LocBeep();
#endif                
                //s_TimerKeySound(2);
#if 0                
                Lib_KbSound(2,2);
#endif                
                KillTimerEvent(kb_TimerScanHandle);
#if 0
                s_KillTimerEvent(2);
                s_KillTimerEvent(3);
#endif                
                g_iKeyScanFlag = 0;
                //s_UartPrint(COM_DEBUG,"t");
                //s_ManualFeedPaper(0);
                kb_TimerStep = 0;
                //AT91_write32(AT91C_PIOC_CODR, KB_OUT_MASK);
                GPIO_ResetBits(GPIOC ,Keyboard_Line) ;
                //EXTI_EnableITBit
                Enable_KB_irq();
#if 0                
                AT91_read32(AT91C_PIOC_ISR);
                AT91_write32(AT91C_AIC_ICCR, (1<<INTR_ID_PIOC));
                AT91_write32(AT91C_PIOC_IER, KB_IN_MASK);  //enable pio input interupt
#endif                
                g_KbIntrStatus=0;
            }
        }
        
        break;
    default:
        break;
    }
    //g_iKeyScanFlag = 0;
}


/****************************************************************************
  ������     :  BYTE kb_ScanKey(int mode)
  ����       :  ɨ�谴��
  �������   :  1��int mode��ɨ��ģʽ��
                     0��ֻɨ�����ް��������ܰ���ֵ
                   ��0����ʾɨ�貢���ذ����ļ�ֵ������
  �������   :  ��
  ����ֵ     :  mode=0ʱ��
                      0���а�����0xff���ް�����
                mode!=0ʱ��
                      �а���ʱ����0~19�����ص��ǰ���������������
                      �ް���ʱ����0xff
  �޸���ʷ   :
      �޸���     �޸�ʱ��    �޸İ汾��   �޸�ԭ��
  1��
****************************************************************************/
BYTE kb_ScanKey(int mode)
{
    DWORD temp,temp1,temp2,ch1,ch2;//,ch;
    DWORD i,j,k,n;
    if(mode==0)
    {
        //AT91_write32(AT91C_PIOC_CODR,KB_OUT_MASK);
        GPIO_ResetBits(GPIOC ,Keyboard_Line) ;
        
        for(j=0;j<3;j++)
        {
            for(i=0;i<100;i++);
            //for(i=0;i<1000;i++);
            //ch1=AT91_read32(AT91C_PIOC_PDSR) & KB_IN_MASK;
            ch1=GPIO_ReadInputData(GPIOC) & KB_IN_MASK;
            
            for(i=0;i<100;i++);
            //for(i=0;i<1000;i++);
            //ch2=AT91_read32(AT91C_PIOC_PDSR) & KB_IN_MASK;
            ch2=GPIO_ReadInputData(GPIOC) & KB_IN_MASK;
            
            if(ch1==ch2)
                break;
            ch1=KB_IN_MASK;

        }
        if(ch1==KB_IN_MASK)
            return NOKEY;
        else
            return 0x00;
    }
    else
    {
        //AT91_write32(AT91C_PIOC_CODR,KB_OUT_MASK);
        GPIO_ResetBits(GPIOC ,Keyboard_Line) ;
        
        for(j=0;j<3;j++)
        {
            for(i=0;i<100;i++);
            //for(i=0;i<1000;i++);
            //ch1=AT91_read32(AT91C_PIOC_PDSR) & KB_IN_MASK;
            ch1=GPIO_ReadInputData(GPIOC) & KB_IN_MASK;
            for(i=0;i<100;i++);
            //for(i=0;i<1000;i++);
            //ch2=AT91_read32(AT91C_PIOC_PDSR) & KB_IN_MASK;
            ch2=GPIO_ReadInputData(GPIOC) & KB_IN_MASK;
            
            if(ch1==ch2)
                break;
            ch1=KB_IN_MASK;

        }
        if(ch1==KB_IN_MASK)
            return NOKEY;

        //�а���*/
        for(i=0;i<4;i++)    //��
        {
            //AT91_write32(AT91C_PIOC_SODR,KB_OUT_MASK);
            GPIO_SetBits(GPIOC ,Keyboard_Line);
            
            //AT91_write32(AT91C_PIOC_CODR,(1<<(i+19)));
            GPIO_ResetBits(GPIOC ,(Keyboard_LineBase<<i)) ;
            
            for(n=0;n<10;n++)
            {
                for(k=0;k<100;k++);//200
                //for(k=0;k<0xFFFF;k++);//200
                //temp1=AT91_read32(AT91C_PIOC_PDSR) & KB_IN_MASK;
                temp1=GPIO_ReadInputData(GPIOC) & KB_IN_MASK;
                
                for(k=0;k<100;k++);//200
                //for(k=0;k<0xffff;k++);//200
                //temp2=AT91_read32(AT91C_PIOC_PDSR) & KB_IN_MASK;
                temp2=GPIO_ReadInputData(GPIOC) & KB_IN_MASK;
                
                if(temp1==temp2)
                    break;
            }
            temp=temp1;

            if(temp!=KB_IN_MASK)
            {
                if(( temp & Keyboard_Row_1 )==0)
                {
                    //AT91_write32(AT91C_PIOC_CODR,KB_OUT_MASK);
                    GPIO_ResetBits(GPIOC ,Keyboard_Line) ;
                    
                    return (i*4+3);
                }
                if(( temp & Keyboard_Row_2 )==0)
                {
                    //AT91_write32(AT91C_PIOC_CODR,KB_OUT_MASK);
                    GPIO_ResetBits(GPIOC ,Keyboard_Line) ;
                    
                    return (i*4+2);
                }
                if(( temp & Keyboard_Row_3 )==0)
                {
                    //AT91_write32(AT91C_PIOC_CODR,KB_OUT_MASK);
                    GPIO_ResetBits(GPIOC ,Keyboard_Line) ;
                    
                    return (i*4+1);
                }
                if(( temp & Keyboard_Row_4 )==0)
                {
                    //AT91_write32(AT91C_PIOC_CODR,KB_OUT_MASK);
                    GPIO_ResetBits(GPIOC ,Keyboard_Line) ;
                    
                    return (i*4);
                }
            }
        }

        //AT91_write32(AT91C_PIOC_CODR,KB_OUT_MASK);
        GPIO_ResetBits(GPIOC ,Keyboard_Line) ;
        
        return NOKEY;
    }
}


/****************************************************************************
  ������     :  void kb_sound(BYTE mode, DWORD dlytime)
  ����       :  �����ڰ���ʱ��������ָ����Ƶ�ʺͼ����������modeȡֵ����7ʱ,
                �ú�������mode%8Ϊ����Ƶ�ʡ�
  �������   :  1��BYTE mode - Ƶ��ѡ�񣬿�����0~6��ֵ
                   1    ���Ƶ�ʡ�
                   7    ���Ƶ�ʡ�
                   0    ������
                2��DWORD dlytime - ���ʱ��(��λ��ms)��0~65535��
                   0xffff---���º�һֱ�죬�ɿ�ʱ��ֹͣ
                   0x0000----������
  �������   :
  ����ֵ     :
  �޸���ʷ   :
      �޸���     �޸�ʱ��    �޸İ汾��   �޸�ԭ��
  1��
****************************************************************************/
int Lib_KbSound(BYTE mode,WORD DlyTimeMs)
{
    BYTE freq;
    while(kb_BeepIntBusy);
    freq=mode%8;
    if(freq==0)
        kb_SoundDelay=0;
    else
    {
        if(DlyTimeMs==0)
            kb_SoundDelay=0;
        //else if(DlyTimeMs<20)
        else if(DlyTimeMs<10)
            kb_SoundDelay=1;
        else if(DlyTimeMs==0xffff)
            kb_SoundDelay=DlyTimeMs;
        else
            //kb_SoundDelay=DlyTimeMs/20;
            kb_SoundDelay=DlyTimeMs/10;
    }
    kb_SoundFrequency=BeepFreqTab[freq];
    
    return 0;
}

/****************************************************************************
  ������     :  BYTE Lib_KbGetCh(void)
  ����       :  �Ӽ��̻������ж�ȡһ��ֵ,�޼�ʱ�ȴ�,����ʾ����Ļ��
  �������   :  ��
  �������   :  ��
  ����ֵ     :  ����ȡ�õļ�ֵ����
  �޸���ʷ   :
      �޸���     �޸�ʱ��    �޸İ汾��   �޸�ԭ��
  1��
****************************************************************************/
BYTE Lib_KbGetCh(void)
{
    BYTE kbcode;
    BYTE ucTemp1, ucTemp2 = 0;

    for(;;)
    {
        ucTemp1 = kb_in_ptr;
        ucTemp2 = kb_out_ptr;
        //while((kb_in_ptr==kb_out_ptr)&&(kb_buf_over==0))
        while((ucTemp1 == ucTemp2)&&(kb_buf_over==0))
        {
            ucTemp1 = kb_in_ptr;
            ucTemp2 = kb_out_ptr;
//            if(k_ICC_CardInSert || k_MCR_BrushFlag)
//             {
//                 k_ICC_CardInSert=0;
//                 k_MCR_BrushFlag=0;
// 				if(k_LcdBackLightMode == 1)
// 					Lib_LcdSetBackLight(1);
//             }
         }
        kb_buf_over = 0;
        if (kb_buf[kb_out_ptr] != NOKEY)
        {
            kbcode = kb_buf[kb_out_ptr];
            kb_buf[kb_out_ptr] = NOKEY;
            kb_out_ptr++;
            if (kb_out_ptr >= KB_BUF_MAX)
                kb_out_ptr = 0;
                
            
            //if((0 == g_iAuthForNumberKeyFlag) && (1 == g_iAppCallGetChFlag))
            //{
            //    if(kbcode>=0x30 && kbcode<=0x39) kbcode='*';
            //    //s_UartPrint(COM_DEBUG,"ch=*\r\n");
            //}
            
            //s_UartPrint(COM_DEBUG,"ch=%02x\r\n",kbcode);
            return kbcode;
        }
        kb_out_ptr++;
        if (kb_out_ptr >= KB_BUF_MAX)
            kb_out_ptr = 0;
    }  
  
}

/****************************************************************************
  ������     :  BYTE Lib_KbUnGetCh(void)
  ����       :  ���ص�ǰ���̻���ĵ�һ���ַ�����,�޼���������
  �������   :  ��
  �������   :  ��
  ����ֵ     :  ����ȡ�õļ�ֵ����
  �޸���ʷ   :
      �޸���     �޸�ʱ��    �޸İ汾��   �޸�ԭ��
  1��
****************************************************************************/
BYTE Lib_KbUnGetCh(void)
{
    BYTE ucTemp1, ucTemp2 = 0;

    ucTemp1 = kb_in_ptr;
    ucTemp2 = kb_out_ptr;
    //if((kb_in_ptr==kb_out_ptr)&&(kb_buf_over==0))
    if((ucTemp1 == ucTemp2)&&(kb_buf_over==0))
    {
        return NOKEY;
    }

    return kb_buf[kb_out_ptr];
}

/****************************************************************************
  ������     :
  ����       :  ����ֵ���������Ƿ���δ��ȡ�ļ�ֵ
  �������   :
  �������   :
  ����ֵ     :  -1000------�޼�ֵ
                0X00------�м�ֵ
  �޸���ʷ   :
      �޸���     �޸�ʱ��    �޸İ汾��   �޸�ԭ��
  1��
****************************************************************************/
int Lib_KbCheck(void)
{
	//Lib_DelayMs(1);
    if(kb_buf[kb_out_ptr]!=NOKEY)
        return 0x00;
    return KB_NoKey_Err;
}

/****************************************************************************
  ������     :
  ����       :  ���ֵ�����������尴�����еĻ�������CPU���������������̰�
                CPU����ע���ж�����ָ����
  �������   :
  �������   :
  ����ֵ     :
  �޸���ʷ   :
      �޸���     �޸�ʱ��    �޸İ汾��   �޸�ԭ��
  1��
****************************************************************************/
void Lib_KbFlush(void)
{
    int i;
    kb_Buffer_Busy=1;
    for(i=0;i<KB_BUF_MAX;i++)
        kb_buf[i]=0xff;
    kb_in_ptr=0;
    kb_out_ptr=0;
    kb_buf_over=0;
    kb_Buffer_Busy=0;
}

/****************************************************************************
  ������     :
  ����       :  ���ð������ڰ���ʱ�Ƿ���
  �������   :  BYTE mode=0;������
                         =1;����
  �������   :
  ����ֵ     :
  �޸���ʷ   :
      �޸���     �޸�ʱ��    �޸İ汾��   �޸�ԭ��
  1��
****************************************************************************/
int  Lib_KbMute(BYTE mode)
{
    if(mode==0)
        (void)Lib_KbSound(0,0);
    else
        (void)Lib_KbSound(4,20);
    return 0;
}


const BYTE g_abyVisualAsciiTable[] =
{"0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ.*, #:;+-=?$&%!~@^()|/_[]{}<>`\'\"\\"};

const BYTE g_abyKeyTableUpper[9][6]={"1.,","2ABC","3DEF","4GHI","5JKL","6MNO","7PQRS","8TUV","9WXYZ"};
const BYTE g_abyKeyTableLower[9][6]={"1.,","2abc","3def","4ghi","5jkl","6mno","7pqrs","8tuv","9wxyz"};
const BYTE g_abyKeyIndexLen[9]={3,4,4,4,4,4,5,4,5};
const BYTE g_abyKeySymbol[33]="0\\* #:;+-=?$&%!~@^()|/_[]{}<>`\'\"";


/****************************************************************************
  ������     :  void s_DisplayPwd(BYTE byLen, BYTE byMode, BYTE byDisplayLine)
  ����       :  ��ָ����ʽ��ʾ����
  �������   :  1��BYTE byLen��Ҫ��ʾ�������ַ����ȡ�
                   �������ֻ����ʾ15���ַ�������15���ַ���15���ַ���ʾ��
                   С�����ֻ����ʾ20���ַ�������20���ַ���20���ַ���ʾ��
                2��BYTE byMode����ʾ��ʽ��
                      bit6��1��д��0Сд
                      bit2��1����룬0�Ҷ���
                      bit0��1����ʾ��0����ʾ
                3��BYTE byDisplayLine����ʾ�����к�
  �������   :  ��
  ����ֵ     :  ��
  �޸���ʷ   :
      �޸���     �޸�ʱ��    �޸İ汾��   �޸�ԭ��
  1�� �ƿ���     2007-10-03  V1.0         ����
  2�� �ƿ���     2008-01-09  V1.1         �޸���ָ��λ������ʱ����˸��е�����
                                          ���ݵĴ���
****************************************************************************/
void s_DisplayPwd(BYTE x, BYTE y, BYTE byMode, BYTE byLen, BYTE byClearLastFlag)
{
    BYTE i, byStartX, byWidth, byDisplayCut;
    int iTemp;

    if (0 != (byMode&IS_FONT_WIDTH_8))  // �ַ���С
    {
        byWidth = 8;
        if (byLen > 15)
        {
            byLen = 15;
        }
        (void)Lib_LcdSetFont(8, 16, 0);
//        Lib_LcdClrLine(byDisplayLine, byDisplayLine+15);
    }
    else
    {
        byWidth = 6;
        if (byLen > 20)
        {
            byLen = 20;
        }
        (void)Lib_LcdSetFont(8, 16, 0);
//        Lib_LcdClrLine(byDisplayLine, byDisplayLine+7);
    }

    if (0 != (byMode&IS_FLUSH_LEFT))
    { // �����
        byStartX = x;
        if (0 != (byMode&IS_FONT_WIDTH_8))
        {
            byDisplayCut = (byStartX+7)/8;
            if ((byLen+byDisplayCut) > 15)
            {
                byLen = 15-byDisplayCut;
            }
        }
        else
        {
            byDisplayCut = (byStartX+5)/6;
            if ((byLen+byDisplayCut) > 20)
            {
                byLen = 20-byDisplayCut;
            }
        }
    }
    else
    { // �Ҷ���
        byStartX = 128-(byLen+1)*byWidth;
    }

    if (0 != byClearLastFlag)
    {
        iTemp = byStartX;
        iTemp -= byWidth;
        if ((0==(byMode&IS_FLUSH_LEFT)) && (iTemp>=0))
        {
            (void)Lib_LcdSetAttr(0);
            Lib_LcdGotoxy(iTemp, y);
            (void)Lib_Lcdprintf(" ");
        }
    }
    if (0 == (byMode&DISPLAY_NOT_REVER))
    {
        (void)Lib_LcdSetAttr(1);
    }
    else
    {
        (void)Lib_LcdSetAttr(0);
    }
    Lib_LcdGotoxy(byStartX, y);
    for (i=0; i<byLen; i++)
    {
        (void)Lib_Lcdprintf("*");
    }
    (void)Lib_LcdSetAttr(0);
    (void)Lib_Lcdprintf("_");
    if (0 != byClearLastFlag)
    {
        if (0 != (byMode&IS_FLUSH_LEFT))
        {
            if ((g_byStartX+byWidth) <= 128)
            {
                (void)Lib_Lcdprintf(" ");
            }
        }
    }
    g_dwPreDisplayLen = byLen+1;
}

/****************************************************************************
  ������     :  void ClearString(BYTE x, BYTE y,  BYTE byMode)
  ����       :  ���ָ��λ�õ��ַ�
  �������   :  1��BYTE x���к�
                2��BYTE y���к�
                3��BYTE byMode��ģʽ
  �������   :  ��
  ����ֵ     :  ��
  �޸���ʷ   :
      �޸���     �޸�ʱ��    �޸İ汾��   �޸�ԭ��
  1�� �ƿ���     2008-01-09   V1.0        ����
****************************************************************************/
void ClearString(BYTE x, BYTE y,  BYTE byMode)
{
    DWORD i;
    BYTE byWidth;
    if (0 != (byMode&IS_FONT_WIDTH_8))
    {
        Lib_LcdSetFont(8, 16, 0);
        byWidth = 8;
    }
    else
    {
        Lib_LcdSetFont(8, 16, 0);
        byWidth = 6;
    }

    if (0 != (byMode&IS_FLUSH_LEFT))
    { // �����
        Lib_LcdGotoxy(x, y);
    }
    else
    { // �Ҷ���
        Lib_LcdGotoxy(128-g_dwPreDisplayLen*byWidth, y);
    }

    for (i=0; i<g_dwPreDisplayLen; i++)
    {
        Lib_Lcdprintf(" ");
    }
}

/****************************************************************************
  ������     :  int s_KbGetStrPwd(BYTE *strOutPwd, BYTE byMinLen,
                    BYTE byMaxLen, BYTE byMode, DWORD dwTimeOut10ms)
  ����       :  ��ȡ�û��������뺯��
  �������   :  1��BYTE byMinLen���������С����
                2��BYTE byMaxLen���������󳤶�
                3��BYTE byMode������ģʽ
                      bit7��1�����볤��Ϊ0ʱ�ɰ�ENTER���˳���0�����볤��Ϊ0
                            ���ɰ�ENTER���˳�
                      bit6��1��д��0Сд
                      bit2��1����룬0�Ҷ���
                      bit0��1����ʾ��0����ʾ
                4��DWORD dwTimeOut10ms�����볬ʱʱ�䳤�ȣ���λ10����
  �������   :  1��BYTE *strOutPwd���û����������
  ����ֵ     :  0���ɹ���
                -1011����ʱ�˳���
                -1012���û���ȡ�����˳���
                -1013���û���ENTER���˳���
  �޸���ʷ   :
      �޸���     �޸�ʱ��    �޸İ汾��   �޸�ԭ��
  1�� �ƿ���     2007-10-07   V1.0        ����
  2�� �ƿ���     2008-01-09  V1.1         �޸���ָ��λ������ʱ����˸��е�����
                                          ���ݵĴ���
****************************************************************************/
int s_KbGetStrPwd(BYTE *strOutPwd, BYTE byMinLen, BYTE byMaxLen, BYTE byMode,
                 DWORD dwTimeOut10ms)
{
    BYTE byRet, byLen, byStartY, byStartX, byBackSpaceFlag=0;
    int iFlag;

    byStartY = g_byStartY;
    byStartX = g_byStartX;

    strOutPwd[0] = 0;
    iFlag = 0;
    byLen = 0;
    s_SetTimer(1, dwTimeOut10ms);
    while (1)
    {
        if(0 == s_CheckTimer(1))
        {
            return KB_InputTimeOut_Err;
        }
#if 0 //for pci enter pin
        if(0 == s_CheckTimer(3))
        {
            return KB_InputTimeOut_Err;
        }
#endif        
        if (0 == iFlag)
        {
            s_DisplayPwd(byStartX, byStartY, byMode, byLen, byBackSpaceFlag);
            byBackSpaceFlag = 0;
            iFlag = 1;
        }
        if (0 != Lib_KbCheck())
        {
            continue;
        }

        s_SetTimer(1, dwTimeOut10ms);  // ��ʱ�����¼�ʱ
        byRet = Lib_KbGetCh();
        switch(byRet)
        {
        case KEYENTER:
            if ((0==byLen) && (0!=(byMode&CAN_ENTER_EXIT)))
            {
                strOutPwd[0] = 0;
                return KB_UserEnter_Exit;
            }
            if((byLen>=byMinLen) && (byLen<=byMaxLen))
            {
                strOutPwd[byLen] = 0;
                return 0;
            }
            Lib_Beep();
            Lib_Beep();
            break;
        case KEYCANCEL:
            strOutPwd[0] = KEYCANCEL;
            strOutPwd[1] = 0;
            return KB_UserCancel_Err;
        case KEYCLEAR:      // ���
            if (0 == byLen)
            {
                Lib_Beep();
                Lib_Beep();
            }
            else
            {
                byLen = 0;
                strOutPwd[0] = 0;
                ClearString(byStartX, byStartY, byMode);
            }
            break;
        case KEYBACKSPACE:  // �˸�
            if (0 != byLen)
            {
                byLen--;
                byBackSpaceFlag = 1;
                strOutPwd[byLen] = 0;
            }
            else
            {
                Lib_Beep();
                Lib_Beep();
            }
            break;
        default:
            if ((byRet>='0') && (byRet<='9') && (byLen<byMaxLen))
            {
                strOutPwd[byLen++] = byRet;
                strOutPwd[byLen] = 0;
            }
            else
            {
                Lib_Beep();
                Lib_Beep();
            }
            break;
        }
        iFlag = 0;
    }
}

/****************************************************************************
  ������     :
  ����       :
  �������   :
  �������   :
  ����ֵ     :
  �޸���ʷ   :
      �޸���     �޸�ʱ��    �޸İ汾��   �޸�ԭ��
  1�� �ƿ���     2007-10-07   V1.0        ����
  2�� �ƿ���     2008-01-09  V1.1         �޸���ָ��λ������ʱ����˸��е�����
                                          ���ݵĴ���
****************************************************************************/
void s_DisplayString(BYTE x, BYTE y, BYTE mode, BYTE *pbyStr, BYTE byClearLastFlag)
{
    BYTE byStartX, byWidth, byLen, byDisplayOff, byDisplayCut;
    int iTemp;

    byDisplayOff = 0;
    byLen = strlen((char*)pbyStr);
    if (0 != (mode&IS_FONT_WIDTH_8))  // �ַ���С
    {
        byWidth = 8;
        if (byLen > 15)
        {
            byDisplayOff = byLen - 15;
        }
        (void)Lib_LcdSetFont(8, 16, 0);
    }
    else
    {
        byWidth = 6;
        if (byLen > 20)
        {
            byDisplayOff = byLen - 20;
        }
        (void)Lib_LcdSetFont(8, 16, 0);
    }

    byDisplayCut = 0;
    if (0 != (mode&IS_FLUSH_LEFT))
    {  // �����
        byStartX = x;
        if (0 != (mode&IS_FONT_WIDTH_8))
        {
            byDisplayCut = (byStartX+7)/8;
            if ((byLen+byDisplayCut) > 15)
            {
                byDisplayOff = byLen - 15 + byDisplayCut;
            }
        }
        else
        {
            byDisplayCut = (byStartX+5)/6;
            if ((byLen+byDisplayCut) > 20)
            {
                byDisplayOff = byLen - 20 + byDisplayCut;
            }
        }
    }
    else
    { // �Ҷ���
        byStartX = 128-(byLen+1-byDisplayOff)*byWidth;
    }

    if (0 != byClearLastFlag)
    {
        iTemp = byStartX;
        iTemp -= byWidth;
        if ((0==(mode&IS_FLUSH_LEFT)) && (iTemp>=0))
        {
            (void)Lib_LcdSetAttr(0);
            Lib_LcdGotoxy(byStartX-byWidth, y);
            (void)Lib_Lcdprintf(" ");
        }
    }
    if (0 == (mode&DISPLAY_NOT_REVER))
    {
        (void)Lib_LcdSetAttr(1);
    }
    else
    {
        (void)Lib_LcdSetAttr(0);
    }
    Lib_LcdGotoxy(byStartX, y);
    (void)Lib_Lcdprintf("%s", &pbyStr[byDisplayOff]);
    (void)Lib_LcdSetAttr(0);
    (void)Lib_Lcdprintf("_");
    if (0 != byClearLastFlag)
    {
        if (0 != (mode&IS_FLUSH_LEFT))
        {
            if ((g_byStartX+byWidth) <= 128)
            {
                (void)Lib_Lcdprintf(" ");
            }
        }
    }
    g_dwPreDisplayLen = strlen((char*)(&pbyStr[byDisplayOff]))+1;
}

/****************************************************************************
  ������     :  void GetEyeableString(BYTE *pbyInString, BYTE *pbyOutString)
  ����       :  ��ȡ����ʾ���ַ���
  �������   :  1��BYTE *pbyInString��Ҫת�����ַ���
  �������   :  1��BYTE *pbyOutString��ת����Ŀ���ʾ���ַ���
  ����ֵ     :  ��
  �޸���ʷ   :
      �޸���     �޸�ʱ��    �޸İ汾��   �޸�ԭ��
  1�� �ƿ���     2007-10-07   V1.0        ����
  2�� �ƿ���     2008-01-03  V1.1         ��VOS302��ֲ��VOS305
****************************************************************************/
void GetEyeableString(BYTE *pbyInString, BYTE *pbyOutString)
{
    int iLen, i, j, iTemp, k;

    iTemp = strlen((char*)g_abyVisualAsciiTable);
    iLen = strlen((char*)pbyInString);
    k = 0;
    for (i=0; i<iLen; i++)
    {
        for (j=0; j<iTemp; j++)
        {
            if (pbyInString[i] == g_abyVisualAsciiTable[j])
            {
                pbyOutString[k++] = pbyInString[i];
                break;
            }
        }
    }
    pbyOutString[k] = 0;
}

/****************************************************************************
  ������     :  void StringToNumber(BYTE *pbyInString, BYTE *pbyOutNumber)
  ����       :  ��������ַ���ת����ÿ���ַ����ڵ������ַ���
  �������   :  1��BYTE *pbyInString��Ҫת�����ַ���
  �������   :  1��BYTE *pbyOutNumber��ת��������ִ�
  ����ֵ     :  ��
  �޸���ʷ   :
      �޸���     �޸�ʱ��    �޸İ汾��   �޸�ԭ��
  1�� �ƿ���     2007-11-05   V1.0        ����
  2�� �ƿ���     2008-01-03  V1.1         ��VOS302��ֲ��VOS305
****************************************************************************/
void StringToNumber(BYTE *pbyInString, BYTE *pbyOutNumber)
{
    int i = 0, j, k, l = 0;

    while (0 != pbyInString[i])
    {
        for (j=0; j<9; j++)
        {
            for (k=0; k<g_abyKeyIndexLen[j]; k++)
            {
                if (g_abyKeyTableUpper[j][k] == pbyInString[i])
                {
                    pbyOutNumber[l++] = g_abyKeyTableUpper[j][0];
                    goto MyStop;
                }
                else if (g_abyKeyTableLower[j][k] == pbyInString[i])
                {
                    pbyOutNumber[l++] = g_abyKeyTableLower[j][0];
                    goto MyStop;
                }
            }
        }
        for (j=0; j<32; j++)
        {
            if (g_abyKeySymbol[j] == pbyInString[i])
            {
                pbyOutNumber[l++] = g_abyKeySymbol[0];
                goto MyStop;
            }
        }
MyStop:
        i++;
    }
    pbyOutNumber[l] = 0;
}

/****************************************************************************
  ������     :  int s_KbGetString(BYTE *strOut, BYTE byMinLen, BYTE byMaxLen,
                        BYTE byMode, DWORD dwTimeOut10ms)
  ����       :  ��ȡ�û�������ַ���
  �������   :  1��BYTE *strOut��Ҫ��ʾ���ַ������û�����������ϸ��ģ��ַ�����
                   0x00��β���ַ����в����԰���С��0x20���ߴ���0x7e�������ַ���
                   �����������Щ�ַ�����Щ�ַ��ᱻ����0x00������
                2��BYTE byMinLen���ַ�������С���ȣ�ע��minlen>0��minlen<=maxlen
                3��BYTE byMaxLen���ַ�������󳤶ȣ�ע��LCD_WIDTH_MINI>=maxlen>0
                4��BYTE byMode�����뷽ʽ�Ķ���:(���ȼ�˳��Ϊbit3>bit4>bit5��
                   ��mode&0x38���ܵ���0)
                        bit7    1��0�� �ܣ��񣩻س��˳������볤��Ϊ0ʱ����Ч��
                        bit6    1��0�� ��С������  8X16/6X8
                        bit5    1��0�� �ܣ���������
                        bit4    1��0�� �ܣ������ַ�
                        bit3    1��0�� �ǣ������뷽ʽ
                        bit2    1��0�� ���ң���������
                        bit1    1��0�� �У���С����
                        bit0    1��0�� ����������ʾ
                5��DWORD dwTimeOut10ms����ʱʱ�䣬��λ���롣
                   ��ȡֵΪ0ʱ����ʾȡĬ��ֵ120�롣
  �������   :  1��BYTE *strOut�����ָ�����ȷ�Χ���ַ�������0x00��β���ְ���
                   ȡֵ��Χ��0x20��0x7e
  ����ֵ     :  0:�ɹ�����,-1013:�û���Enter���˳�,-1011:���볬ʱ,-1012:�û�ȡ������
  �޸���ʷ   :
      �޸���     �޸�ʱ��    �޸İ汾��   �޸�ԭ��
  1�� �ƿ���     2007-10-07  V1.0         ����
  2�� �ƿ���     2008-01-09  V1.1         �޸���ָ��λ������ʱ����˸��е�����
                                          ���ݵĴ���
****************************************************************************/
int s_KbGetString(BYTE *strOut, BYTE byMinLen, BYTE byMaxLen, BYTE byMode,
                 DWORD dwTimeOut10ms)
{
    BYTE byRet, byLen, byStartY, byStartX, byBackSpaceFlag = 0,
        strTemp[130]; // �����ַ�����ÿ���ַ����ڰ���������
    int iFlag, iAlphaLowerFlag, iIndex=0;

    byStartY = g_byStartY;
    byStartX = g_byStartX;

    strOut[byMaxLen] = 0;
    iFlag = 0;
    iAlphaLowerFlag = 0;

    // ȥ�����ɼ��ַ�
    GetEyeableString(strOut, strTemp);
    strcpy((char*)strOut, (char*)strTemp);
    byLen = strlen((char*)strOut);

    g_dwPreDisplayLen = byLen;

    StringToNumber(strOut, strTemp);

    s_SetTimer(1, dwTimeOut10ms);
    while (1)
    {
        if(0 == s_CheckTimer(1))
        {
            return KB_InputTimeOut_Err;
        }
        if (0 == iFlag)
        {
            s_DisplayString(byStartX, byStartY, byMode, strOut, byBackSpaceFlag);
            byBackSpaceFlag = 0;
            iFlag = 1;
        }
        if (0 != Lib_KbCheck())
        {
            continue;
        }

        s_SetTimer(1, dwTimeOut10ms);  // ��ʱ�����¼�ʱ
        byRet = Lib_KbGetCh();
        switch(byRet)
        {
        case KEYENTER:
            if ((0==byLen) && (0!=(byMode&CAN_ENTER_EXIT)))
            {
                strOut[0] = 0;
                return KB_UserEnter_Exit;
            }
            if((byLen>=byMinLen) && (byLen<=byMaxLen))
            {
                strOut[byLen] = 0;
                return 0;
            }
            Lib_Beep();
            Lib_Beep();
            break;
        case KEYCANCEL:
            strOut[0] = KEYCANCEL;
            strOut[1] = 0;
            return KB_UserCancel_Err;
        case KEYCLEAR:      // ���
            if (0 == byLen)
            {
                Lib_Beep();
                Lib_Beep();
            }
            else
            {
                byLen = 0;
                strOut[0] = 0;
                strTemp[byLen] = 0;
                iIndex = 0;
                ClearString(byStartX, byStartY, byMode);
            }
            break;
        case KEYBACKSPACE:  // �˸�
            if (0 != byLen)
            {
                byLen--;
                byBackSpaceFlag = 1;
                strOut[byLen] = 0;
                strTemp[byLen] = 0;
                iIndex = 0;
            }
            else
            {
                Lib_Beep();
                Lib_Beep();
            }
            break;
        case KEYFN:
            if (0 == iAlphaLowerFlag)
            {
                if ('0' != strTemp[byLen-1])
                {
                    strOut[byLen-1] = g_abyKeyTableLower[strTemp[byLen-1]-'1'][iIndex];
                }
                iAlphaLowerFlag = 1;
            }
            else
            {
                if ('0' != strTemp[byLen-1])
                {
                    strOut[byLen-1] = g_abyKeyTableUpper[strTemp[byLen-1]-'1'][iIndex];
                }
                iAlphaLowerFlag = 0;
            }
            break;
        case KEYALPHA:
		case KEYSTAR:
            if (0 == byLen)
            {
                Lib_Beep();
                Lib_Beep();
            }
            else
            {
                if ('0' == strTemp[byLen-1])
                {
                    iIndex++;
                    iIndex %= 32;
                    strOut[byLen-1] = g_abyKeySymbol[iIndex];
                }
                else if ((strTemp[byLen-1]>='1') || (strTemp[byLen-1]<='9'))
                {
                    iIndex++;
                    iIndex %= g_abyKeyIndexLen[strTemp[byLen-1]-'1'];
                    if (0 == iAlphaLowerFlag)
                    {
                        strOut[byLen-1] = g_abyKeyTableUpper[strTemp[byLen-1]-'1'][iIndex];
                    }
                    else
                    {
                        strOut[byLen-1] = g_abyKeyTableLower[strTemp[byLen-1]-'1'][iIndex];
                    }
                }
                else
                {
                    Lib_Beep();
                    Lib_Beep();
                }
            }
            break;
        default:
            if ((byRet>='0') && (byRet<='9') && (byLen<byMaxLen))
            {
                strTemp[byLen] = byRet;  // ����
                strOut[byLen] = byRet;   // �ַ�
                byLen++;
                strOut[byLen] = 0;
                strTemp[byLen] = 0;
                iIndex = 0;
            }
            else
            {
                Lib_Beep();
                Lib_Beep();
            }
            break;
        }
        iFlag = 0;
    }
}

/****************************************************************************
  ������     :  void s_Kb_DataStrConvAmt(BYTE *pbyAmtIn, BYTE byInlen, BYTE *pbyDispOut)
  ����       :  ���������ַ����޸ĳ���λС�����ַ���
  �������   :  1��BYTE *pbyAmtIn������Ľ���ַ���
                2��BYTE byInlen�����볤��
  �������   :  1��BYTE *pbyDispOut��������ַ�����ʽ
                     pbyDispOut[0]������ַ�������
                     pbyDispOut[1~pbyDispOut[0]]��������ַ�������
  ����ֵ     :  ��
  �޸���ʷ   :
      �޸���     �޸�ʱ��    �޸İ汾��   �޸�ԭ��
  1��
****************************************************************************/
void s_Kb_DataStrConvAmt(BYTE *pbyAmtIn, BYTE byInlen, BYTE *pbyDispOut)
{
    BYTE i;

    // ȥ������ߵ�'0'
    for (i=0; i<byInlen; i++)
    {
        if (pbyAmtIn[i] != '0')
            break;
    }
    byInlen -= i;
    pbyAmtIn += i;

    strcpy((char*)pbyDispOut, "0.00");
    switch (byInlen)
    {
    case 0: // "0.00"
        return ;
    case 1:  // "0.0X"
        pbyDispOut[3] = pbyAmtIn[0];
        return ;
    case 2: // "0.XX"
        memcpy(&pbyDispOut[2], &pbyAmtIn[0], 2);
        return ;
    default:  // XXXX.XX
        memcpy(pbyDispOut, pbyAmtIn, byInlen-2);
        pbyDispOut[byInlen-2] = '.';
        memcpy(&pbyDispOut[byInlen-1], &pbyAmtIn[byInlen-2], 2);
        pbyDispOut[byInlen+1] = 0;
        return ;
    }
}

/****************************************************************************
  ������     :
  ����       :
  �������   :
  �������   :
  ����ֵ     :
  �޸���ʷ   :
      �޸���     �޸�ʱ��    �޸İ汾��   �޸�ԭ��
  1�� �ƿ���     2007-10-07  V1.0         ����
  2�� �ƿ���     2008-01-09  V1.1         �޸���ָ��λ������ʱ����˸��е�����
                                          ���ݵĴ���
****************************************************************************/
int s_KbGetStrDotNumber(BYTE *strOut, BYTE byMinLen, BYTE byMaxLen, BYTE byMode,
                 DWORD dwTimeOut10ms)
{
    BYTE byRet, byLen, byStartY, byStartX, abyTemp[130], byBackSpaceFlag = 0;
    int iFlag;

    byStartY = g_byStartY;
    byStartX = g_byStartX;

    strOut[byMaxLen] = 0;
    iFlag = 0;
    byLen = strlen((char*)strOut);
    s_SetTimer(1, dwTimeOut10ms);
    while (1)
    {
        if(0 == s_CheckTimer(1))
        {
            return KB_InputTimeOut_Err;
        }
        if (0 == iFlag)
        {
            s_Kb_DataStrConvAmt(strOut, byLen, abyTemp);
            s_DisplayString(byStartX, byStartY, byMode, abyTemp, byBackSpaceFlag);
            byBackSpaceFlag = 0;
            iFlag = 1;
        }
        if (0 != Lib_KbCheck())
        {
            continue;
        }

        s_SetTimer(1, dwTimeOut10ms);  // ��ʱ�����¼�ʱ
        byRet = Lib_KbGetCh();
        switch(byRet)
        {
        case KEYENTER:
            if ((0==byLen) && (0!=(byMode&CAN_ENTER_EXIT)))
            {
                strOut[0] = 0;
                return KB_UserEnter_Exit;
            }
            if((byLen>=byMinLen) && (byLen<=byMaxLen))
            {
                strOut[byLen] = 0;
                return 0;
            }
            Lib_Beep();
            Lib_Beep();
            break;
        case KEYCANCEL:
            strOut[0] = KEYCANCEL;
            strOut[1] = 0;
            return KB_UserCancel_Err;
        case KEYCLEAR:      // ���
            if (0 == byLen)
            {
                Lib_Beep();
                Lib_Beep();
            }
            else
            {
                ClearString(byStartX, byStartY, byMode);
                byLen = 0;
                strOut[0] = 0;
            }
            break;
        case KEYBACKSPACE:  // �˸�
            if (0 != byLen)
            {
                byLen--;
                byBackSpaceFlag = 1;
                strOut[byLen] = 0;
            }
            else
            {
                Lib_Beep();
                Lib_Beep();
            }
            break;
        default:
            if ((byRet>='0') && (byRet<='9') && (byLen<byMaxLen))
            {
                strOut[byLen++] = byRet;
                strOut[byLen] = 0;
            }
            else
            {
                Lib_Beep();
                Lib_Beep();
            }
            break;
        }
        iFlag = 0;
    }
}


/****************************************************************************
  ������     :  int s_KbGetStrNumber(BYTE *strOut, BYTE byMinLen,
                     BYTE byMaxLen, BYTE byMode, DWORD dwTimeOut10ms)
  ����       :  ��ȡ�û���������ִ�
  �������   :  1��BYTE byMinLen���������С����
                2��BYTE byMaxLen���������󳤶�
                3��BYTE byMode������ģʽ
                      bit7��1�����볤��Ϊ0ʱ�ɰ�ENTER���˳���0�����볤��Ϊ0
                            ���ɰ�ENTER���˳�
                      bit6��1��д��0Сд
                      bit2��1����룬0�Ҷ���
                      bit0��1����ʾ��0����ʾ
                4��DWORD dwTimeOut10ms�����볬ʱʱ�䳤�ȣ���λ10����
  �������   :  1��BYTE *strOut���û���������ִ����ò�������������������������
  ����ֵ     :  0���ɹ�
                -1011����ʱ�˳�
                -1012���û�ȡ������
                -1013���û���ENTER���˳�
  �޸���ʷ   :
      �޸���     �޸�ʱ��    �޸İ汾��   �޸�ԭ��
  1�� �ƿ���     2007-10-07   V1.0        ����
  2�� �ƿ���     2008-01-09  V1.1         �޸���ָ��λ������ʱ����˸��е�����
                                          ���ݵĴ���
****************************************************************************/
int s_KbGetStrNumber(BYTE *strOut, BYTE byMinLen, BYTE byMaxLen, BYTE byMode,
                 DWORD dwTimeOut10ms)
{
    BYTE byRet, byLen, byStartY, byStartX, byBackSpaceFlag=0;
    int iFlag, iDotFlag = 0;

    byStartY = g_byStartY;
    byStartX = g_byStartX;

    strOut[byMaxLen] = 0;
    iFlag = 0;
    byLen = strlen((char*)strOut);
    s_SetTimer(1, dwTimeOut10ms);
    while (1)
    {
        if(0 == s_CheckTimer(1))
        {
            return KB_InputTimeOut_Err;
        }
        if (0 == iFlag)
        {
            s_DisplayString(byStartX, byStartY, byMode, strOut, byBackSpaceFlag);
            byBackSpaceFlag = 0;
            iFlag = 1;
        }
        if (0 != Lib_KbCheck())
        {
            continue;
        }

        s_SetTimer(1, dwTimeOut10ms);  // ��ʱ�����¼�ʱ
        byRet = Lib_KbGetCh();
        switch(byRet)
        {
        case KEYENTER:
            if ((0==byLen) && (0!=(byMode&CAN_ENTER_EXIT)))
            {
                strOut[0] = 0;
                return KB_UserEnter_Exit;
            }
            if((byLen>=byMinLen) && (byLen<=byMaxLen) && (1!=iDotFlag))
            {
                strOut[byLen] = 0;
                return 0;
            }
            Lib_Beep();
            Lib_Beep();
            break;
        case KEYCANCEL:
            strOut[0] = KEYCANCEL;
            strOut[1] = 0;
            return KB_UserCancel_Err;
        case KEYCLEAR:      // ���
            if (0 == byLen)
            {
                Lib_Beep();
                Lib_Beep();
            }
            else
            {
                byLen = 0;
                strOut[0] = 0;
                ClearString(byStartX, byStartY, byMode);
            }
            break;
        case KEYBACKSPACE:  // �˸�
            if (0 != byLen)
            {
                byLen--;
                byBackSpaceFlag = 1;
                if ('.' == strOut[byLen])
                {
                    iDotFlag = 0;
                }
                strOut[byLen] = 0;
            }
            else
            {
                Lib_Beep();
                Lib_Beep();
            }
            break;
        case KEYALPHA:
		case KEYSTAR:
            if (   (2!=iDotFlag)
                && (0!=byLen)
                && (('.'==strOut[byLen-1])||('1'==strOut[byLen-1])))
            {
                if ('.' == strOut[byLen-1])
                {
                    strOut[byLen-1] = '1';
                    iDotFlag = 0;
                }
                else
                {
                    strOut[byLen-1] = '.';
                    iDotFlag = 1;
                }
            }
            else
            {
                Lib_Beep();
                Lib_Beep();
            }
            break;
        default:
            if ((byRet>='0') && (byRet<='9') && (byLen<byMaxLen))
            {
                strOut[byLen++] = byRet;
                strOut[byLen] = 0;
                if (1 == iDotFlag)
                {
                    iDotFlag = 2;
                }
            }
            else
            {
                Lib_Beep();
                Lib_Beep();
            }
            break;
        }
        iFlag = 0;
    }
}


/****************************************************************************
  ������     :  int Lib_KbGetStr(BYTE *str, BYTE minlen, BYTE maxlen, BYTE mode,
                   WORD timeoutsec)
  ����       :  ��ȡ�û������ַ�������
  �������   :  1��BYTE *str��Ҫ��ʾ���ַ������û�����������ϸ��ģ��ַ�����
                   0x00��β���ַ����в����԰���С��0x20���ߴ���0x7e�������ַ���
                   �����������Щ�ַ�����Щ�ַ��ᱻ����0x00������
                2��BYTE minlen������ַ���С���ȣ�ע��minlen>0��minlen<=maxlen
                3��BYTE maxlen������ַ���󳤶ȣ�ע��LCD_WIDTH_MINI>=maxlen>0
                4��BYTE mode�����뷽ʽ�Ķ���:(���ȼ�˳��Ϊbit3>bit4>bit5��
                   ��mode&0x38���ܵ���0)
                        bit7    1��0�� �ܣ��񣩻س��˳������볤��Ϊ0ʱ����Ч��
                        bit6    1��0�� ��С������  8X16/6X8
                        bit5    1��0�� �ܣ���������
                        bit4    1��0�� �ܣ������ַ�
                        bit3    1��0�� �ǣ������뷽ʽ
                        bit2    1��0�� ���ң���������
                        bit1    1��0�� �У���С����
                        bit0    1��0�� ����������ʾ
                5��WORD timeoutsec����ʱʱ�䳤�ȣ���λ���롣
                   ��ȡֵΪ0ʱ����ʾȡĬ��ֵ120�롣
  �������   :  1��BYTE *str�����ָ�����ȷ�Χ���ַ�������0x00��β���ְ���ȡֵ
                   ��Χ��0x20��0x7e
  ����ֵ     :  -1010�������������
  ��    ע   :  �����ϡ�����Ч��ȥ�ġ��˸񡱼���
                �����¡�����Ч��ȥ�ġ���ĸ������
                �����ܡ����͹�ȥ��һ������ʹ������ĸ��Сд�л�
  �޸���ʷ   :
      �޸���     �޸�ʱ��    �޸İ汾��   �޸�ԭ��
  1�� �ƿ���     2007-11-02   V1.0        ����
  2�� �ƿ���     2008-01-09  V1.1         �޸���ָ��λ������ʱ����˸��е�����
                                          ���ݵĴ���
****************************************************************************/
int Lib_KbGetStr(BYTE *str, BYTE minlen, BYTE maxlen, BYTE mode,
                   WORD timeoutsec)
{
    DWORD dwTimeOut10ms;
    int iRet;
    BYTE byOldFontYpe, byOldFontAttr, byStartX, byStartY;

    byStartX = g_byStartX;
    byStartY = g_byStartY;
    byOldFontYpe = g_byFontType;
    byOldFontAttr = g_byFontAttr;
    if ((minlen>maxlen) || (maxlen>128) || (0==maxlen) || (NULL==str)
        || (0==(mode&0x38)))
    {
        return KB_InputPara_Err;
    }

    if (0 == timeoutsec)
    {
		dwTimeOut10ms = 6000;  // 60��
    }
    else
    {
        dwTimeOut10ms = timeoutsec*100;
    }

    if (0 != (mode&CAN_INPUT_PASSWORD))
    {
        iRet = s_KbGetStrPwd(str, minlen, maxlen, (mode&0xc5), dwTimeOut10ms);
    }
    else if (0 != (mode&CAN_INPUT_CHAR))
    {
        iRet = s_KbGetString(str, minlen, maxlen, mode, dwTimeOut10ms);
    }
    else if (0 != (mode&CAN_INPUT_DECIMAL))
    {
        iRet = s_KbGetStrDotNumber(str, minlen, maxlen, mode, dwTimeOut10ms);
    }
    else
    {
        iRet = s_KbGetStrNumber(str, minlen, maxlen, mode, dwTimeOut10ms);
    }
    g_byStartX = byStartX;
    g_byStartY = byStartY;
    g_byFontType = byOldFontYpe;
    g_byFontAttr = byOldFontAttr;
    return iRet;
}

void TestKbGetch()
{
    BYTE byKey;

    Lib_LcdCls();
    //Lib_LcdPrintxyCE(0, 0, 0x81, "��������  ", "Key Test");
	Lib_LcdPrintxy(0, 8*0,0x80, "     Key   Test    ");
    while (1)
    {
        if (0 == Lib_KbCheck())
        {
            byKey = Lib_KbGetCh();
//test
//trace_debug_printf("KbGetCh[%02x]",byKey);
            
           /* Lib_LcdClrLine(16/2, 31);
            Lib_LcdGotoxy(0, 16/2);
            Lib_Lcdprintf("  %x", byKey);*/
			Lib_LcdPrintxy(0, 8*2,0x00,"  %02x", byKey);
            if (KEYCANCEL == byKey)
            {
              
              //  return ;
            }
        }
    }
}

