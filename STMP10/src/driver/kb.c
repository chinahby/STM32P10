#include "stm32f10x_lib.h"
#include "Toolkit.h"
#include "kb.h"

unsigned int Keyboard_Val = 0 ;
unsigned char Keyboard_Change_Flag = 0 ;

//IN:KB[0]-KB[3](pc5-pc8) out KB[4]-KB[7](pc9-pc12)

int s_KbInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure ;
    unsigned char sBuf[10];
    
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

    //����Row1-Row4Ϊ�������ж�
    EXTI_InitStructure.EXTI_Line = Keyboard_EXTI_Line;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt ;
    //EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising ;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling ;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE ;
    EXTI_Init(&EXTI_InitStructure) ;
    
    
    GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12; //-out
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    //GPIO_SetBits(GPIOC,GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12);
    //GPIO_SetBits(GPIOC,GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12);
    GPIO_ResetBits(GPIOC ,GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12) ; 
#if 0
    //test
    GPIO_SetBits(GPIOC,GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12);
    
     sBuf[0] = GPIO_ReadInputDataBit(GPIOC ,GPIO_Pin_9);
     sBuf[1] = GPIO_ReadInputDataBit(GPIOC ,GPIO_Pin_10);
     sBuf[2] = GPIO_ReadInputDataBit(GPIOC ,GPIO_Pin_11);
     sBuf[3] = GPIO_ReadInputDataBit(GPIOC ,GPIO_Pin_12);
    
     sBuf[0] = GPIO_ReadInputDataBit(GPIOC ,GPIO_Pin_5);
     sBuf[1] = GPIO_ReadInputDataBit(GPIOC ,GPIO_Pin_6);
     sBuf[2] = GPIO_ReadInputDataBit(GPIOC ,GPIO_Pin_7);
     sBuf[3] = GPIO_ReadInputDataBit(GPIOC ,GPIO_Pin_8);
  
     GPIO_ResetBits(GPIOC,GPIO_Pin_9);
     sBuf[0] = GPIO_ReadInputDataBit(GPIOC ,GPIO_Pin_9);
     sBuf[1] = GPIO_ReadInputDataBit(GPIOC ,GPIO_Pin_10);
     sBuf[2] = GPIO_ReadInputDataBit(GPIOC ,GPIO_Pin_11);
     sBuf[3] = GPIO_ReadInputDataBit(GPIOC ,GPIO_Pin_12);
    
     sBuf[0] = GPIO_ReadInputDataBit(GPIOC ,GPIO_Pin_5);
     sBuf[1] = GPIO_ReadInputDataBit(GPIOC ,GPIO_Pin_6);
     sBuf[2] = GPIO_ReadInputDataBit(GPIOC ,GPIO_Pin_7);
     sBuf[3] = GPIO_ReadInputDataBit(GPIOC ,GPIO_Pin_8);
#endif     
}

/****************************************************************
��������: Delay
��    ��: ��ɨ�谴��ʱ��������ʱ����
��    ��: nCount -- ��ʱ����
����ֵ  : ��
*****************************************************************/

void Delay(vu32 nCount)
{
  for(; nCount!= 0;nCount--);
}


//û�еõ���ֵ����0�����򷵻���Ӧ�ļ�ֵ
unsigned char Get_KeyValue(void)//ʹ��PA0~PA7
{//ʹ���߷�ת��
  GPIO_InitTypeDef GPIO_InitStructure;
  
unsigned char const Key_Tab[4][4]=//���̱����
{
  {'1','2','3','4'},
  {'5','6','7','8'},
  {'9','0','A','B'},
  {'C','D','E','F'}
};  
  u8 i=5,j=5;
  u16 temp1,temp2;
  
  //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  
  //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;                            
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8;                            
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  //GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  //GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_Init(GPIOC, &GPIO_InitStructure);
   
  //GPIO_Write(GPIOA,0xfff0);//ɨ����ֵ
  GPIO_Write(GPIOC,0xfE1f);//ɨ����ֵ
  //if((GPIO_ReadInputData(GPIOA)&0x00f0)==0x00f0)
  if((GPIO_ReadInputData(GPIOC)&0x1e00)==0x1e00)
    return 0;
  else
  {
    delay_ms(70);//��������
    //if((GPIO_ReadInputData(GPIOA)&0x00f0)==0x00f0)
    if((GPIO_ReadInputData(GPIOC)&0x1e00)==0x1e00)
      return 0;
    else
      //temp1=GPIO_ReadInputData(GPIOA)&0x00f0;
      temp1=GPIO_ReadInputData(GPIOC)&0x1e00;
  }
  
  switch(temp1)
  {
  //case 0x00e0:j=0;break;
  case 0x1c00:j=0;break;
  //case 0x00d0:j=1;break;
  case 0x1a00:j=1;break;
  //case 0x00b0:j=2;break;
  case 0x1600:j=2;break;
  //case 0x0070:j=3;break;
  case 0x0e00:j=3;break;
  default:break;
  }
  //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  //GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  //GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  //GPIO_Write(GPIOA,0xff0f);//ɨ����ֵ
  GPIO_Write(GPIOC,0xe1ff);//ɨ����ֵ
  //if((GPIO_ReadInputData(GPIOA)&0x000f)==0x000f)
  if((GPIO_ReadInputData(GPIOC)&0x01e0)==0x01e0)
    return 0;
  else
  {//���ﲻ����ʱ��ɨ�裬��Ϊ�Ѿ�ȷ���˲��Ƕ����Ż���뱾������
    //temp2=GPIO_ReadInputData(GPIOA)&0x000f;
    temp2=GPIO_ReadInputData(GPIOC)&0x01e0;
  }
  
  switch(temp2)
  {
  //case 0x000e:i=0;break;
  case 0x01c0:i=0;break;
  //case 0x000d:i=1;break;
  case 0x01a0:i=1;break;
  //case 0x000b:i=2;break;
  case 0x0160:i=2;break;
  //case 0x0007:i=3;break;
  case 0x00e0:i=3;break;
  default:break;
  }
   
  if((i==5)||(j==5))
    return 0;
  else
    return (Key_Tab[i][j]);
}
