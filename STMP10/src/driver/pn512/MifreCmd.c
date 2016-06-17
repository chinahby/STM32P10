/**************************************************************
			������ΪPN512�ײ�������������2010-06-08
			�����ϸ����㻮��
			
**************************************************************/
#define FOR_PN512_DEVICE
#include "Mifre_TmDef.h"
#include <string.h>
#include "comm.h"
#include "kf701dh.h"
#include "systimer.h"


extern PICC_PARA c_para;
extern PN512_RF_WORKSTRUCT PN512_RF_WorkInfo;
ulong PN512_gl_RF_ulFSList[16] = {16,24,32,40,48,64,96,128,256,256,256,256,256,256,256,256};

extern void WaitNuS(u32 x);
extern void EXTI_DisableITBit(u32 EXTI_Line);

//uchar PN512_B_gl_RxThreshold = 0x84;//���������ȿɵ��� add by skx


// ��ʼ����������
void PN512_s_RF_vInitWorkInfo(void)
{
	uchar ucRFOpenFlg = 0;

	ucRFOpenFlg = PN512_RF_WorkInfo.ucRFOpen;
	memset((uchar *)&PN512_RF_WorkInfo, 0x00, sizeof(PN512_RF_WorkInfo));
	PN512_RF_WorkInfo.ucRFOpen = ucRFOpenFlg;
	
	PN512_RF_WorkInfo.ucMifCardType = RF_TYPE_S50;
	PN512_RF_WorkInfo.ucCurType = RF_TYPEA; 
	PN512_RF_WorkInfo.ucMifActivateFlag = 0;       
	PN512_RF_WorkInfo.FSC = 32;
	PN512_RF_WorkInfo.FSD = 256;
	
	c_para.card_buffer_val = PN512_RF_WorkInfo.FSC;
}


void PN512_s_vRFSelectType(uchar ucRFType)
{
     uchar ucTemp = 0x00;

     switch(ucRFType)
   	 {
     case 'a':
		 case 'A':
			ucTemp = 0x80;
			PN512_s_vRFWriteReg(1,PN512_TXMODE_REG,&ucTemp); // ISO14443A, 106kBits/s, TxCRCEn
			ucTemp = 0x80;
			PN512_s_vRFWriteReg(1,PN512_RXMODE_REG,&ucTemp); // ISO14443A, 106KBits/s, RxCRCEn, RxNoErr
			ucTemp = 0x40;
			PN512_s_vRFWriteReg(1,PN512_TXAUTO_REG,&ucTemp); // Enable Force100%ASK

			ucTemp = 0x84;
			PN512_s_vRFWriteReg(1,PN512_RXSEL_REG,&ucTemp); // RxWait = 6, Modulation signal from the internal analog part

			//ucTemp = c_para.card_RxThreshold_val; // 0x84;
			ucTemp = 0x84; // 0x84;	
			//ucTemp = 0x80; // 0x84;	
                        
			PN512_s_vRFWriteReg(1, PN512_RXTHRESHOLD_REG, &ucTemp);    //* MinLevel = 5; CollLevel = 5 *
						
			ucTemp = c_para.a_conduct_val;
			PN512_s_vRFWriteReg(1,PN512_CWGSP_REG,&ucTemp);//�絼ϵ��

			// added by liuxl according to RC522 20070918
			ucTemp = 0x39;// 01 "6363" CRC_PRE
			PN512_s_vRFWriteReg(1, PN512_MODE_REG,&ucTemp);//����CRC_PRE
 
			ucTemp = 0x26; // Pulse width
			// LOW value: #clocksLOW   = (ModWidth % 8)+1.
			// HIGH value: #clocksHIGH = 16 - #clocksLOW.
			PN512_s_vRFWriteReg(1,PN512_MODWIDTH_REG,&ucTemp);

			ucTemp = 0x00; // TYPEB: RxSOF, RxEOF, TxSOF, TxEOF, TxEGT = 1(2ETU), SOF Width = 11ETU
			PN512_s_vRFWriteReg(1,PN512_TYPEB_REG,&ucTemp);
                        
                        ucTemp = 0x40;//50->40 VAx_reg_RFCfgReg_RxGain_value
                        PN512_s_vRFWriteReg(1,PN512_RFCFG_REG,&ucTemp);
                        //13/07/08 3cm
                        //trace_debug_printf("_glbPiccSetPara[%02x]\n",_glbPiccSetPara);

                        
                        
                        
			ucTemp = c_para.b_modulate_val; // c_para.B_ModGsP_Val;//0x17;
			PN512_s_vRFWriteReg(1,PN512_MODGSP_REG,&ucTemp);// �������
                        
                        if(  (_glbPiccSetPara&0x20)==0 )
                        {
			ucTemp = 0x94; // 0x84->94;	
			//ucTemp = 0x80; // 0x84;	
			PN512_s_vRFWriteReg(1, PN512_RXTHRESHOLD_REG, &ucTemp);    //* MinLevel = 5; CollLevel = 5 *
                          
                        
                        ucTemp = 0x40;//50->40 VAx_reg_RFCfgReg_RxGain_value
                        PN512_s_vRFWriteReg(1,PN512_RFCFG_REG,&ucTemp);
                        
			ucTemp = 0x3f; // 3a->3d->4d->3f�Խ��ս����������
			PN512_s_vRFWriteReg(1, PN512_DEMOD_REG, &ucTemp); //VAx_reg_DemodReg_value
                        
                        ucTemp = 0;
			PN512_s_vRFWriteReg(1,PN512_MODGSP_REG,&ucTemp);// �������
                        
                        }
                        
			break;
			
		 case 'b':
		 case 'B':
			ucTemp = 0x83;
			PN512_s_vRFWriteReg(1,PN512_TXMODE_REG,&ucTemp); // ISO14443B, 106kBits/s, TxCRCEn
			ucTemp = 0x83;
			PN512_s_vRFWriteReg(1,PN512_RXMODE_REG,&ucTemp); // ISO14443B, 106KBits/s, RxCRCEn, RxNoErr
			ucTemp = 0x00;
			PN512_s_vRFWriteReg(1,PN512_TXAUTO_REG,&ucTemp); // Disable  Force100%ASK
			ucTemp = 0x84;
			PN512_s_vRFWriteReg(1,PN512_RXSEL_REG,&ucTemp); // RxWait = 3

			//ucTemp = PN512_B_gl_RxThreshold;//����������
			ucTemp=c_para.card_RxThreshold_val;//����������
			PN512_s_vRFWriteReg(1,PN512_RXTHRESHOLD_REG,&ucTemp);

			ucTemp = c_para.a_conduct_val;
			PN512_s_vRFWriteReg(1,PN512_CWGSP_REG,&ucTemp);//�絼ϵ�� VAx_reg_CWGsP_value_B

			//  for debug according to RC522
			ucTemp = 0x3B;// 11 "FFFF" CRC_PRE
			PN512_s_vRFWriteReg(1, PN512_MODE_REG,&ucTemp);		
			// debug end*/  
  
			ucTemp = 0xC1; //0xD1; // TYPEB: RxSOF, RxEOF, TxSOF, TxEOF, TxEGT = 1(2ETU), SOF Width = 11ETU
//			ucTemp = 0xD1; // TYPEB: RxSOF, RxEOF, TxSOF, TxEOF, TxEGT = 1(2ETU), SOF Width = 11ETU
			PN512_s_vRFWriteReg(1,PN512_TYPEB_REG,&ucTemp);

			ucTemp = c_para.b_modulate_val;//0x17; VAx_reg_ModGsP_value_B
			PN512_s_vRFWriteReg(1,PN512_MODGSP_REG,&ucTemp);// �������
                        

                        //13/07/06
                        //trace_debug_printf("B _glbPiccSetPara[%02x]\n",_glbPiccSetPara);
//#if 0   //3cm
                        //if(  (_glbPiccSetPara&0x04)==0 )
                        {      
//#if 0
                        //test
                        PN512_s_vRFReadReg(1,PN512_DEMOD_REG,&ucTemp);
                        trace_debug_printf("PN512_DEMOD_REG val[%x]\n",ucTemp);
                          
			ucTemp = 0x4d; // 3a->3d->4d�Խ��ս����������
			PN512_s_vRFWriteReg(1, PN512_DEMOD_REG, &ucTemp); //VAx_reg_DemodReg_value_B
//#endif                    
                        
			ucTemp = 0x54; //58->68->58->48->50->52->51->53->54->4d->50->54->64(3cm)����Ĭ��ֵ PN512���ⲿRF��ǿ�����ȣ�ע����������Ľ������������֣� 12/09/21 48->58
			PN512_s_vRFWriteReg(1,PN512_RFCFG_REG,&ucTemp);//VAx_reg_RFCfgReg_RxGain_value_B|
                        
#if 0                       
			//ucTemp=c_para.card_RxThreshold_val;//����������
                        ucTemp = 0x55;
			PN512_s_vRFWriteReg(1,PN512_RXTHRESHOLD_REG,&ucTemp);
#endif                        
                        }
//#endif       
      

                        
//#if 0
                        
                        //13/07/08
                        //if(  (_glbPiccSetPara&0x01)==0 )
                        {
                        //0cm
//#if 0
			//ucTemp=c_para.card_RxThreshold_val;//����������
                        ucTemp = 0x75; //75->65->85->a5->55 VAx_reg_RxThreshold_MinLevel_value_B|VAx_reg_RxThreshold_CollLevel_value_B
			PN512_s_vRFWriteReg(1,PN512_RXTHRESHOLD_REG,&ucTemp);
//#endif
                        
                        ucTemp = 0x84;//85->84->86->83->85->80->83->88->84
                        PN512_s_vRFWriteReg(1,PN512_GSNON_REG,&ucTemp);//VAx_reg_CWGsN_value_B|VAx_reg_ModGsN_value_B
                        }
//#endif                        
                        //13/07/09 3cm
                        if(  (_glbPiccSetPara&0x04)==0 )
                        {
                          ucTemp = 0x55; //75->65->85->a5->55 VAx_reg_RxThreshold_MinLevel_value_B|VAx_reg_RxThreshold_CollLevel_value_B
			  PN512_s_vRFWriteReg(1,PN512_RXTHRESHOLD_REG,&ucTemp);
                          
			  ucTemp = 0x58; //58->68->58->48->50->52->51->53->54->4d->50->54->64(3cm)����Ĭ��ֵ PN512���ⲿRF��ǿ�����ȣ�ע����������Ľ������������֣� 12/09/21 48->58
			  PN512_s_vRFWriteReg(1,PN512_RFCFG_REG,&ucTemp);//VAx_reg_RFCfgReg_RxGain_value_B|
                          
			  ucTemp = 0x3f; // 3a->3d->4d�Խ��ս����������
			  PN512_s_vRFWriteReg(1, PN512_DEMOD_REG, &ucTemp); //VAx_reg_DemodReg_value_B
                          
                          ucTemp = 0x88;//85->84->86->83->85->80->83->88->84->88
                          PN512_s_vRFWriteReg(1,PN512_GSNON_REG,&ucTemp);//VAx_reg_CWGsN_value_B|VAx_reg_ModGsN_value_B
                          
                        }
                        
                        //13/07/10 1.5cm
                        if(  (_glbPiccSetPara&0x02)==0 )
                        {
                          ucTemp = 0x70; //75->65->85->a5->55->70 VAx_reg_RxThreshold_MinLevel_value_B|VAx_reg_RxThreshold_CollLevel_value_B
			  PN512_s_vRFWriteReg(1,PN512_RXTHRESHOLD_REG,&ucTemp);
			  ucTemp = 0x48; //58->68->58->48->50->52->51->53->54->4d->50->54->64(3cm)->58����Ĭ��ֵ PN512���ⲿRF��ǿ�����ȣ�ע����������Ľ������������֣� 12/09/21 48->58
			  PN512_s_vRFWriteReg(1,PN512_RFCFG_REG,&ucTemp);//VAx_reg_RFCfgReg_RxGain_value_B|
                          
			  ucTemp = 0x4d; // 3a->3d->4d�Խ��ս����������
			  PN512_s_vRFWriteReg(1, PN512_DEMOD_REG, &ucTemp); //VAx_reg_DemodReg_value_B
                          
                        ucTemp = 0x88;//85->84->86->83->85->80->83->88->84->88
                        PN512_s_vRFWriteReg(1,PN512_GSNON_REG,&ucTemp);//VAx_reg_CWGsN_value_B|VAx_reg_ModGsN_value_B
                          
                        }
                        
			ucTemp = 0x26; // Pulse width
			// LOW value: #clocksLOW   = (ModWidth % 8)+1.
			// HIGH value: #clocksHIGH = 16 - #clocksLOW.
			PN512_s_vRFWriteReg(1,PN512_MODWIDTH_REG,&ucTemp);
			// set TxLastBits to 0
			ucTemp = 0x00; 
			PN512_s_vRFWriteReg(1,PN512_BITFRAMING_REG, &ucTemp);
			// ��ͻ�����ݶ���0
			PN512_s_vRFSetBitMask(PN512_COLL_REG,0x80); // Zero After Coll		 	
			break;
			
		 case 'f':
		 case 'F':
			
			//ucTemp = 0x10;//��s_init��ͳһ������
			//PN512_s_vRFWriteReg(1,PN512_CONTROL_REG,&ucTemp); // set PN512 as a Initiator Mode
			
			ucTemp = 0x00;
			PN512_s_vRFWriteReg(1,PN512_BITFRAMING_REG,&ucTemp); // set felica send with a standard 8 bit framing

			ucTemp = (0x38|0x80);//Not MSBFirst,����felica����������ΪMSBFirst,��ץȡ���ο���Ӱ��
			PN512_s_vRFWriteReg(1,PN512_MODE_REG,&ucTemp); // set CRC_PER is 0x00,0x00 for felica,MSBFirst 
			
			// ucTemp = 0xA2;  // FeliCa, 424kBits/s, TxCRCEn
			ucTemp = 0x92;
			PN512_s_vRFWriteReg(1,PN512_TXMODE_REG,&ucTemp); // FeliCa, 212kBits/s, TxCRCEn

			// ucTemp = 0xAA; // FeliCa, 424KBits/s, RxCRCEn, RxNoErr
			ucTemp = 0x9A;//Ŀǰ����Ϊ��֡���գ���felica����polling�ڼ��������Ϊ��֡���գ�
			PN512_s_vRFWriteReg(1,PN512_RXMODE_REG,&ucTemp); // FeliCa, 212KBits/s, RxCRCEn, RxNoErr
			
			ucTemp = 0x00;
			PN512_s_vRFWriteReg(1,PN512_TXAUTO_REG,&ucTemp); // Disable  Force100%ASK

			//ucTemp = 0x10;//��s_init��ͳһ������
			//PN512_s_vRFWriteReg(1,PN512_TXSEL_REG,&ucTemp); // TX1��TX2 Modulation signal (envelope) from the internal coder

			ucTemp = 0x84;//Rxwait=6����felica����Ҫȷ�ϵ���
			PN512_s_vRFWriteReg(1,PN512_RXSEL_REG,&ucTemp); // RxWait = 6, Modulation signal from the internal analog part
			
			//ucTemp = PN512_B_gl_RxThreshold;//����������
			ucTemp=c_para.card_RxThreshold_val;
			PN512_s_vRFWriteReg(1,PN512_RXTHRESHOLD_REG,&ucTemp);

			ucTemp = c_para.a_conduct_val;
			PN512_s_vRFWriteReg(1,PN512_CWGSP_REG,&ucTemp);//�絼ϵ��			
		
			ucTemp = 0x4D; // ����Ĭ��ֵ���Խ��ս����������
			PN512_s_vRFWriteReg(1, PN512_DEMOD_REG, &ucTemp); 


			ucTemp = 0x00; // felicaר������
			PN512_s_vRFWriteReg(1, PN512_FELNFC1_REG, &ucTemp);//FelSyncLen��0xB2,0x4D;DataLenMin:0x00(��С������0)

			ucTemp = 0x00; // felicaר������
			PN512_s_vRFWriteReg(1, PN512_FELNFC2_REG, &ucTemp);//DataLenMax:0x00(��������256,�����ʵ������)

			//AT
			//ucTemp = (0x00 |0x10); //disable Parity�������ö�felica��Ӱ�죬���������ò���Ĭ��ֵ
			//PN512_s_vRFWriteReg(1, PN512_MANUALRCV_REG, &ucTemp);//enable Parity
 
			//ucTemp = 0x48; //����Ĭ��ֵ PN512���ⲿRF��ǿ�����ȣ�ע����������Ľ������������֣�
			ucTemp = 0x60; //����Ĭ��ֵ PN512���ⲿRF��ǿ�����ȣ�ע����������Ľ������������֣� 12/09/21 48->60
                        //ucTemp = 0x70;
			PN512_s_vRFWriteReg(1,PN512_RFCFG_REG,&ucTemp);

			ucTemp = c_para.f_modulate_val;//0x17;
			PN512_s_vRFWriteReg(1,PN512_MODGSP_REG,&ucTemp);// ������ȣ�felica������ȣ�10%��

			ucTemp = 0x00; // TYPEB: RxSOF, RxEOF, TxSOF, TxEOF, TxEGT = 1(2ETU), SOF Width = 11ETU
			PN512_s_vRFWriteReg(1,PN512_TYPEB_REG,&ucTemp);

			ucTemp = 0x26; // Pulse width
			// LOW value: #clocksLOW   = (ModWidth % 8)+1.
			// HIGH value: #clocksHIGH = 16 - #clocksLOW.
			PN512_s_vRFWriteReg(1,PN512_MODWIDTH_REG,&ucTemp);
			break;
		 default:
		 	break;
   	 }

	 DelayMs(6);
}

uchar PN512_s_RF_ucCardTypeCheck(uchar *pCardType)
{
	uchar ucSAK = 0;

	if (PN512_RF_WorkInfo.ucUIDLen == 4)
		ucSAK = PN512_RF_WorkInfo.ucSAK1;
	else if(PN512_RF_WorkInfo.ucUIDLen == 7)
		ucSAK = PN512_RF_WorkInfo.ucSAK2;
	else if(PN512_RF_WorkInfo.ucUIDLen == 10)
		ucSAK = PN512_RF_WorkInfo.ucSAK3;
	else
		return(RET_RF_ERR_PARAM);

	if((ucSAK & 0x20) != 0x20)
	{
		if((ucSAK & 0x18) == 0x08)
		{									
			*pCardType = RF_TYPE_S50;
		}
		else if((ucSAK & 0x18) == 0x18)
		{
			*pCardType = RF_TYPE_S70;
		}
		else
		{
			return RET_RF_ERR_TRANSMIT;
		}
	}
	else
	{
		if((ucSAK & 0x18) == 0x08)
		{
			*pCardType = RF_TYPE_PRO_S50;
		}
		else if((ucSAK & 0x18) == 0x18)
		{
			*pCardType = RF_TYPE_PRO_S70;
		}
		else
		{
			*pCardType = RF_TYPE_PRO;
		}				   
	}
	
	return(RET_RF_OK);
}


uchar PN512_s_RF_ucWUPA(uchar *pucResp)
{
     uchar ucRet = RET_RF_OK;
     uchar ucTempData = 0x00;

     // �Ƚ��ز�����ΪTypeA��
     PN512_s_vRFSelectType('A');
	 
	 // ���ö�ʱ��
	 //PN512_s_RFSetTimer(RF_DEFAULT); //��ʱʱ���ӳ�
     //PN512_s_RFSetTimer(RF_FWTMIN);
	 PN512_s_RFSetTimer(50);
 
	 // ��ֹ RxCRC �� TxCRC , ������żУ��
     ucTempData = 0x00;
     PN512_s_vRFWriteReg(1, PN512_TXMODE_REG, &ucTempData);
	 PN512_s_vRFWriteReg(1, PN512_RXMODE_REG, &ucTempData);
	 // RxNoErr If set to logic 1, a not valid received data stream (less than 4 bits received)
	 // will be ignored. The receiver will remain active.
	 // If the bit RxNoErr in register RxModeReg is set to logic 1, bit RxIRq is only set
	 // to logic 1 when data bytes are available in the FIFO.
	 PN512_s_vRFSetBitMask(PN512_RXMODE_REG, 0x08); //���Խ���޿�ʱ���������жϵ�����

     // ��ֹcrypto1��Ԫ
     PN512_s_vRFClrBitMask(PN512_STATUS2_REG, 0x08);

//	 ucTempData = 0x83;
//	 PN512_s_vRFWriteReg(1, PN512_TXCONTROL_REG, &ucTempData);// for debug liuxl 20071008
//	 PN512_s_vRFSetBitMask(PN512_TXCONTROL_REG, 0x03);// Tx2RF-En, Tx1RF-En enable
	 
     // set TxLastBits to 7, ��֡��ʽ
     ucTempData = 0x07; 
     PN512_s_vRFWriteReg(1,PN512_BITFRAMING_REG, &ucTempData);

	 // ��ͻ�����ݶ���0
     PN512_s_vRFClrBitMask(PN512_COLL_REG,0x80); // Zero After Coll

	 // ׼����������: ������
     PN512_RF_WorkInfo.ulSendBytes = 1;    // how many bytes to send
	 	 memset((uchar*)PN512_RF_WorkInfo.aucBuffer, 0, sizeof(PN512_RF_WorkInfo.aucBuffer));
     PN512_RF_WorkInfo.aucBuffer[0] = PICC_WUPA; // WUPA���� 

	 ucRet = PN512_s_RFExchangeCmd(PN512_TRANSCEIVE_CMD);

 	 if(ucRet == RET_RF_ERR_TIMEOUT)
     {     	  
     	  PN512_RF_WorkInfo.usErrNo = 0xA1; // timeout err
          return RET_RF_ERR_TIMEOUT;
     }

	 
	 // �����������
     // �������յ����������ֽ�: ������ʶ���(2�ֽ�)
     if((ucRet == RET_RF_OK) && (PN512_RF_WorkInfo.lBitsReceived != 16))
     {
     	 PN512_RF_WorkInfo.usErrNo = 0x14; // received data != 16 bits
         ucRet = RET_RF_ERR_TRANSMIT;
     }

     if(ucRet != RET_RF_OK)
     {
          if(ucRet == RET_RF_ERR_MULTI_CARD)
		  {     	  	  
			  ucRet =  RET_RF_ERR_MULTI_CARD;
		  }
		  else
		  {
			  // ͨ�Ŵ���
			  ucRet = RET_RF_ERR_TRANSMIT;
		  }
     }
     else
     {
		// 1. ATQA��һ���ֽڵĵ�B8��B7λ����Ϊ00 01 10֮һ������Э�������
		// 2. ATQA��һ���ֽڵĵ�B6Ϊ����Ϊ0������Э�������
		// 3. ATQA��һ���ֽڵĵ�B5��B1λ����Ϊ10000 01000 00100 00010 00001֮һ������Э�������
		// 4. ATQA�ڶ����ֽڵĵ�B8��B5λ����Ϊ0000������Э�������

          if(((PN512_RF_WorkInfo.aucBuffer[0] & 0x1f) != 0x10) && ((PN512_RF_WorkInfo.aucBuffer[0] & 0x1f) != 0x08)
             && ((PN512_RF_WorkInfo.aucBuffer[0] & 0x1f) != 0x04) && ((PN512_RF_WorkInfo.aucBuffer[0] & 0x1f) != 0x02)
             && ((PN512_RF_WorkInfo.aucBuffer[0] & 0x1f) != 0x01))
          {
               PN512_RF_WorkInfo.usErrNo = 0x10; // ATQA BYTE 1 bit 5~1 err 
               ucRet = RET_RF_ERR_PROTOCOL; // Э�����
          }
          else if((PN512_RF_WorkInfo.aucBuffer[0] & 0x20) != 0x00)
          {
               PN512_RF_WorkInfo.usErrNo = 0x11; // ATQA BYTE 1 bit 6 err 
               ucRet = RET_RF_ERR_PROTOCOL; // Э�����
          }
          else if((PN512_RF_WorkInfo.aucBuffer[0] & 0xC0) == 0xC0)
          {
               PN512_RF_WorkInfo.usErrNo = 0x12; // ATQA BYTE 1 bit 8~7 err 
               ucRet = RET_RF_ERR_PROTOCOL; // Э�����
          }
          else if((PN512_RF_WorkInfo.aucBuffer[1] & 0xf0) != 0x00)
          {
               PN512_RF_WorkInfo.usErrNo = 0x13; // ATQA BYTE 2 err 
               ucRet = RET_RF_ERR_PROTOCOL; // Э�����
          }
          else
          {
				switch(PN512_RF_WorkInfo.aucBuffer[0] & 0xC0)
				{
					case 0x00:
						 PN512_RF_WorkInfo.ucUIDLen = 4;
						 break;
					case 0x40:
						 PN512_RF_WorkInfo.ucUIDLen = 7;
						 break;
					default:
						 PN512_RF_WorkInfo.ucUIDLen = 10;
						 break;
				}
                memcpy(pucResp, (uchar*)PN512_RF_WorkInfo.aucBuffer, 2);
          }
    }
	 //for debug ������detect�ӿ�����������ʱ���ִ���
//	PN512_s_RFDelay10ETUs(RF_TPDEALY/10);//(RF_FDTMIN/10); // ��ʱ500us�ٷ�����һ������
	//s_RFDelay100Us(6);// 20071224 ��ʱ600us
    return ucRet;
}



uchar PN512_s_RF_ucAnticoll(uchar ucSEL, uchar* pucUID)
{
     uchar ucRet = RET_RF_OK;
     uchar ucTempData = 0x00;
     uchar ucTempVal = 0x00;
     uchar i = 0;

	 // ���ö�ʱ��
     //PN512_s_RFSetTimer(RF_FWTMIN);
	 PN512_s_RFSetTimer(RF_DEFAULT); //��ʱʱ���ӳ�
	 
     // ��ͻ�����ݶ���0,
     PN512_s_vRFClrBitMask(PN512_COLL_REG,0x80); // Zero After Coll

     // ��ֹcrypto1��Ԫ
     PN512_s_vRFClrBitMask(PN512_STATUS2_REG, 0x08);

	 // ��ֹ RxCRC �� TxCRC
     ucTempData = 0x00;
     PN512_s_vRFWriteReg(1, PN512_TXMODE_REG, &ucTempData);
	 PN512_s_vRFWriteReg(1, PN512_RXMODE_REG, &ucTempData);

     // set TxLastBits to 0, ��׼֡��ʽ
     ucTempData = 0x00;   
     PN512_s_vRFWriteReg(1,PN512_BITFRAMING_REG, &ucTempData);
	 // added end

     PN512_RF_WorkInfo.ucAnticol = ucSEL;
	 PN512_RF_WorkInfo.aucBuffer[0] = ucSEL; // SEL
     PN512_RF_WorkInfo.aucBuffer[1] = 0x20;
     PN512_RF_WorkInfo.ulSendBytes = 2;    // how many bytes to send
	 // ִ������
     ucRet = PN512_s_RFExchangeCmd(PN512_TRANSCEIVE_CMD);
	
	 PN512_s_vRFSetBitMask(PN512_COLL_REG,0x80); // 20080421

	 if(ucRet == RET_RF_ERR_TIMEOUT)  // ��ʱ����Ӧ��ֱ�ӷ���
     {
          PN512_RF_WorkInfo.usErrNo = 0xA2; // timeout err
          return RET_RF_ERR_TIMEOUT;
     }

     if(ucRet == RET_RF_OK)
     {
           if(PN512_RF_WorkInfo.lBitsReceived != 40) // not 5 bytes answered
           {
                ucRet = RET_RF_ERR_TRANSMIT;
           }
           else
           {
                switch(PN512_RF_WorkInfo.ucUIDLen)
				{
				    case 4:
					{
                       if(ucSEL == PICC_ANTICOLL1)
                       {
                             if(PN512_RF_WorkInfo.aucBuffer[0] == 0x88)
                             {
                             		PN512_RF_WorkInfo.usErrNo = 0x20; // UIDLEN = 4, BYTE0=88 err
                                    ucRet = RET_RF_ERR_PROTOCOL; // Э�����
                             }
                       }
                       break;
					}

					case 7:
					{
                       if(ucSEL == PICC_ANTICOLL1)
                       {
                             if(PN512_RF_WorkInfo.aucBuffer[0] != 0x88)
                             {
                             		PN512_RF_WorkInfo.usErrNo = 0x21; // UIDLEN = 7, BYTE0!=88 err
                                    ucRet = RET_RF_ERR_PROTOCOL; // Э�����
                             }
                       }

					   if(ucSEL == PICC_ANTICOLL2)
                       {
                             if(PN512_RF_WorkInfo.aucBuffer[0] == 0x88)
                             {
                                    PN512_RF_WorkInfo.usErrNo = 0x22; // UIDLEN = 7, BYTE4=88 err
                                    ucRet = RET_RF_ERR_PROTOCOL; // Э�����
                             }
                       }
					   break;
                    }
                    default:
					{
                       if(ucSEL == PICC_ANTICOLL1)
                       {
                             if(PN512_RF_WorkInfo.aucBuffer[0] != 0x88)
                             {
                             		PN512_RF_WorkInfo.usErrNo = 0x23; // UIDLEN = 10, BYTE0!=88 err
                                    ucRet = RET_RF_ERR_PROTOCOL; // Э�����
                             }
                       }

					   if(ucSEL == PICC_ANTICOLL2)
                       {
                             if(PN512_RF_WorkInfo.aucBuffer[0] != 0x88)
                             {
                             		PN512_RF_WorkInfo.usErrNo = 0x24; // UIDLEN = 10, BYTE4!=88 err
                                    ucRet = RET_RF_ERR_PROTOCOL; // Э�����
                             }
                       }
					   break;
					}
				}

                // У��BCC
                ucTempVal = 0;
                for(i = 0; i < 4; i++)
                {
                      ucTempVal ^= PN512_RF_WorkInfo.aucBuffer[i];
                }
                if(ucTempVal != PN512_RF_WorkInfo.aucBuffer[4])
                {
                      PN512_RF_WorkInfo.usErrNo = 0x25; // uid bcc err
                      ucRet = RET_RF_ERR_TRANSMIT; // ͨ�Ŵ���
                }
         }

         memcpy(pucUID,(uchar*)PN512_RF_WorkInfo.aucBuffer,5);
     }
     else if(ucRet == RET_RF_ERR_MULTI_CARD)
     {
         ucRet = RET_RF_ERR_MULTI_CARD;
     }
	 else
	 {
		 ucRet = RET_RF_ERR_TRANSMIT;
	 }

	 PN512_RF_WorkInfo.ucAnticol = 0;
	 
	 //for debug ������detect�ӿ�����������ʱ���ִ���
	 //PN512_s_RFDelay10ETUs(RF_TPDEALY/10);*/
//	 PN512_s_RFDelay10ETUs(RF_FDTMIN/10); // ��ʱ500us�ٷ�����һ������
	 //s_RFDelay100Us(6);// 20071224 ��ʱ600us
     return ucRet;
}


uchar PN512_s_RF_ucSelect(uchar ucSEL, uchar* pucUID, uchar* pucSAK)
{
     uchar ucRet = RET_RF_OK;
     uchar ucTempData = 0x00;

     // ���ö�ʱ��
	 PN512_s_RFSetTimer(RF_DEFAULT); //��ʱʱ���ӳ�
//     PN512_s_RFSetTimer(RF_FWTMIN);
	 
     // RxCRC,TxCRC, ISO14443A
     ucTempData = 0x80;
     PN512_s_vRFWriteReg(1, PN512_TXMODE_REG, &ucTempData);
	 PN512_s_vRFWriteReg(1, PN512_RXMODE_REG, &ucTempData);

     // ��ֹcrypto1��Ԫ
     PN512_s_vRFClrBitMask(PN512_STATUS2_REG, 0x08);    // disable crypto 1 unit


     // set TxLastBits to 0, ��׼֡��ʽ
     ucTempData = 0x00;   
     PN512_s_vRFWriteReg(1,PN512_BITFRAMING_REG, &ucTempData);
	 // added end

     PN512_RF_WorkInfo.ulSendBytes = 7;
     // ����UID����
     memcpy((uchar*)(PN512_RF_WorkInfo.aucBuffer+2), pucUID, 5);

	 PN512_RF_WorkInfo.aucBuffer[0] = ucSEL;
     PN512_RF_WorkInfo.aucBuffer[1] = 0x70;       // number of bytes send

	 // �������������Ӧ
     ucRet = PN512_s_RFExchangeCmd(PN512_TRANSCEIVE_CMD);

     if(ucRet == RET_RF_ERR_TIMEOUT) // ��ʱ����Ӧ��ֱ�ӷ���
     {
          PN512_RF_WorkInfo.usErrNo = 0xA3; // timeout err
          return RET_RF_ERR_TIMEOUT;
     }

     if(ucRet != RET_RF_OK)
     {
          ucRet = RET_RF_ERR_TRANSMIT; // ����ͨ�Ŵ���
     }
     else
     {
           if(PN512_RF_WorkInfo.lBitsReceived != 8)
           {
           		PN512_RF_WorkInfo.usErrNo = 0x30; // received bits err
               // һ���ֽ�û������
               ucRet = RET_RF_ERR_TRANSMIT;
           }
           else
           {
			   // B3λ�ж�UID�Ƿ�����
			   switch(PN512_RF_WorkInfo.ucUIDLen)
			   {
			       case 4:
				   {
                       if((PN512_RF_WorkInfo.aucBuffer[0] & 0x04) == 0x04)
					   {
					   		 PN512_RF_WorkInfo.usErrNo = 0x31; // UID err
							 ucRet = RET_RF_ERR_PROTOCOL;
					   }
					   PN512_RF_WorkInfo.ucSAK1 = PN512_RF_WorkInfo.aucBuffer[0];
					   break;
				   }

				   case 7:
				   {
                       if(ucSEL == PICC_ANTICOLL1)
					   {
						   if((PN512_RF_WorkInfo.aucBuffer[0] & 0x04) != 0x04)
						   {
					   		     PN512_RF_WorkInfo.usErrNo = 0x32; // UID err
								 ucRet = RET_RF_ERR_PROTOCOL;
						   }
						   PN512_RF_WorkInfo.ucSAK1 = PN512_RF_WorkInfo.aucBuffer[0];
					   }

					   if(ucSEL == PICC_ANTICOLL2)
					   {
						   if((PN512_RF_WorkInfo.aucBuffer[0] & 0x04) == 0x04)
						   {
					   		     PN512_RF_WorkInfo.usErrNo = 0x33; // UID err
								 ucRet = RET_RF_ERR_PROTOCOL;
						   }
						   PN512_RF_WorkInfo.ucSAK2 = PN512_RF_WorkInfo.aucBuffer[0];
					   }
					   break;
				   }

				   default:
				   {
					   if(ucSEL == PICC_ANTICOLL1)
					   {
						   if((PN512_RF_WorkInfo.aucBuffer[0] & 0x04) != 0x04)
						   {
					   		     PN512_RF_WorkInfo.usErrNo = 0x34; // UID err
								 ucRet = RET_RF_ERR_PROTOCOL;
						   }
						   PN512_RF_WorkInfo.ucSAK1 = PN512_RF_WorkInfo.aucBuffer[0];
					   }

					   if(ucSEL == PICC_ANTICOLL2)
					   {
						   if((PN512_RF_WorkInfo.aucBuffer[0] & 0x04) != 0x04)
						   {
					   		     PN512_RF_WorkInfo.usErrNo = 0x35; // UID err
								 ucRet = RET_RF_ERR_PROTOCOL;
						   }
						   PN512_RF_WorkInfo.ucSAK2 = PN512_RF_WorkInfo.aucBuffer[0];
					   }

					   if(ucSEL == PICC_ANTICOLL3)
					   {
						   if((PN512_RF_WorkInfo.aucBuffer[0] & 0x04) == 0x04)
						   {
					   		     PN512_RF_WorkInfo.usErrNo = 0x36; // UID err
								 ucRet = RET_RF_ERR_PROTOCOL;
						   }
						   PN512_RF_WorkInfo.ucSAK3 = PN512_RF_WorkInfo.aucBuffer[0];
					   }
					   break;
				   }
			   }
           }
      }
	  *pucSAK = PN512_RF_WorkInfo.aucBuffer[0];
	  
	 //for debug ������detect�ӿ�����������ʱ���ִ���
//	 PN512_s_RFDelay10ETUs(RF_TPDEALY/10);//PN512_s_RFDelay10ETUs(RF_FDTMIN/10); // ��ʱ500us�ٷ�����һ������
	 //s_RFDelay100Us(6);// 20071224 ��ʱ600us
      return ucRet;
}


uchar PN512_s_RF_ucGetUID(void)
{
       uchar ucRet = RET_RF_OK;
       uchar ucTempData = 0x00;
       uchar ucUIDTemp[5];
       uchar ucSAK = 0x00;
       uchar ucReSendNo = 0x00;

       memset(ucUIDTemp, 0x00, sizeof(ucUIDTemp));

       for(ucReSendNo = 0; ucReSendNo < 3; ucReSendNo++)
	   {
		   ucTempData = PICC_ANTICOLL1; // ��һ����ͻ
		   ucRet = PN512_s_RF_ucAnticoll(ucTempData, ucUIDTemp);
		   if(ucRet != RET_RF_ERR_TIMEOUT)
		   {
				break;
		   }
	   }

       if(ucRet != RET_RF_OK)
       {
		   return ucRet;
       }

	   for(ucReSendNo = 0; ucReSendNo < 3; ucReSendNo++)
	   {
			ucRet = PN512_s_RF_ucSelect(ucTempData, ucUIDTemp, &ucSAK);
			if(ucRet != RET_RF_ERR_TIMEOUT)
			{
				 break;
			}
	   }

	   if(ucRet != RET_RF_OK)
	   {
		   return ucRet;
	   }

	   // �����һ��UID����
       memcpy((uchar*)PN512_RF_WorkInfo.ucUIDCL1, ucUIDTemp, 5);
	   if(PN512_RF_WorkInfo.ucUIDLen == 4)
	   {
            return RET_RF_OK;
	   }

       // ��ȡ�ڶ���UID����
       for(ucReSendNo = 0; ucReSendNo < 3; ucReSendNo++)
	   {
		   ucTempData = PICC_ANTICOLL2; // �ڶ�����ͻ
		   ucRet = PN512_s_RF_ucAnticoll(ucTempData, ucUIDTemp);
		   if(ucRet != RET_RF_ERR_TIMEOUT)
		   {
			   break;
		   }
	   }

	   if(ucRet != RET_RF_OK)
	   {
		   return ucRet;
	   }

	   for(ucReSendNo = 0; ucReSendNo < 3; ucReSendNo++)
	   {
			ucRet = PN512_s_RF_ucSelect(ucTempData, ucUIDTemp, &ucSAK);
			if(ucRet != RET_RF_ERR_TIMEOUT)
			{
				 break;
			}
	   }

       if(ucRet != RET_RF_OK)
	   {
		   return ucRet;
	   }
       // ����ڶ���UID����
       memcpy((uchar*)PN512_RF_WorkInfo.ucUIDCL2, ucUIDTemp, 5);

       if(PN512_RF_WorkInfo.ucUIDLen == 7)
	   {
		    return RET_RF_OK;
	   }

       // ��ȡ������UID����
	   for(ucReSendNo = 0; ucReSendNo < 3; ucReSendNo++)
	   {
			  ucTempData = PICC_ANTICOLL3; // ��������ͻ
			  ucRet = PN512_s_RF_ucAnticoll(ucTempData, ucUIDTemp);
			  if(ucRet != RET_RF_ERR_TIMEOUT)
			  {
				  break;
			  }
	   }

	   if(ucRet != RET_RF_OK)
	   {
			return ucRet;
	   }

       for(ucReSendNo = 0; ucReSendNo < 3; ucReSendNo++)
	   {
			ucRet = PN512_s_RF_ucSelect(ucTempData, ucUIDTemp, &ucSAK);
			if(ucRet != RET_RF_ERR_TIMEOUT)
			{
				 break;
			}
	   }

       if(ucRet != RET_RF_OK)
	   {
		   return ucRet;
	   }

	   // ���������UID����
       memcpy((uchar*)PN512_RF_WorkInfo.ucUIDCL3, ucUIDTemp, 5);

	   return RET_RF_OK;
}


uchar PN512_s_RF_ucActPro(uchar* pucOutLen, uchar* pucATSData)
{
       uchar ucRet = RET_RF_OK;
       uchar ucTempData = 0x00;
       uchar ucSAK = 0x00;
       uchar ucResp[2];
       uchar ucReSendNo = 0;

       // ��ʱ����£���෢������ͬһ������
       for(ucReSendNo = 0; ucReSendNo < 3; ucReSendNo++)
       {
           // �Ƚ���WUPA����

           ucRet = PN512_s_RF_ucWUPA(ucResp);
		   if(ucRet != RET_RF_ERR_TIMEOUT)
           {
                 break;
           }
       }

       // ��������������ط���ֱ�ӷ��ظ��ն�
       if(ucRet != RET_RF_OK)
       {
           PN512_RF_WorkInfo.usErrNo = 0x70; // ActPro WUPA err
		   return ucRet;
       }
       else
       {
             // ���жϻ�ȡ��ATQA�����Ƿ��ѱ����ATQA����һ��
             if(memcmp(ucResp, (uchar*)PN512_RF_WorkInfo.ucATQA, 2))
             {
             		PN512_RF_WorkInfo.usErrNo = 0x71; // ActPro ATQA err
                    return RET_RF_ERR_PROTOCOL;
             }
       }

       // ���е�һ��SELECT
	   for(ucReSendNo = 0; ucReSendNo < 3; ucReSendNo++)
	   {
           ucTempData = PICC_ANTICOLL1;
           ucRet = PN512_s_RF_ucSelect(ucTempData, (uchar*)PN512_RF_WorkInfo.ucUIDCL1, &ucSAK);
		   if(ucRet != RET_RF_ERR_TIMEOUT)
           {
                break;
           }
	   }

       if(ucRet != RET_RF_OK)
       {
       	   PN512_RF_WorkInfo.usErrNo |= 0x100; // ActPro SELECT err
		   return ucRet;
	   }
       // UIDΪ4���ֽڣ�һ��Select��������ʹPICC����ACTIVATE״̬
	   if(PN512_RF_WorkInfo.ucUIDLen == 4)
	   {
		   goto ACTEND;
	   }

       // ���еڶ���SELECT
       for(ucReSendNo = 0; ucReSendNo < 3; ucReSendNo++)
	   {
          ucTempData = PICC_ANTICOLL2;
          ucRet = PN512_s_RF_ucSelect(ucTempData, (uchar*)PN512_RF_WorkInfo.ucUIDCL2, &ucSAK);
          if(ucRet != RET_RF_ERR_TIMEOUT)
          {
                break;
          }
	   }

       if(ucRet != RET_RF_OK)
	   {
	       PN512_RF_WorkInfo.usErrNo |= 0x200; // ActPro SELECT err
		   return ucRet;
	   }
       // UIDΪ7���ֽڣ�����Select��������ʹPICC����ACTIVATE״̬
       if(PN512_RF_WorkInfo.ucUIDLen == 7)
	   {
		   goto ACTEND;
	   }

       // ���е�����SELECT
       // UIDΪ10���ֽڣ�����Select��������ʹPICC����ACTIVATE״̬
       for(ucReSendNo = 0; ucReSendNo < 3; ucReSendNo++)
	   {
          ucTempData = PICC_ANTICOLL3;
          ucRet = PN512_s_RF_ucSelect(ucTempData, (uchar*)PN512_RF_WorkInfo.ucUIDCL3, &ucSAK);
          if(ucRet != RET_RF_ERR_TIMEOUT)
          {
                break;
          }
	   }

       if(ucRet != RET_RF_OK)
	   {
	       PN512_RF_WorkInfo.usErrNo |= 0x300; // ActPro SELECT err
		   return ucRet;
	   }

ACTEND:
	   // SELECT����ʹPICC����ACTIVATE״̬��Ӧ�ٷ���RATS��ȡATS����
       for(ucReSendNo = 0; ucReSendNo < 3; ucReSendNo++)
       {
            ucRet = PN512_s_RF_ucRATS(pucOutLen, pucATSData);
			if(ucRet != RET_RF_ERR_TIMEOUT)
            {
                 break;
            }
       }

       if(ucRet != RET_RF_OK)
	   {
		   return ucRet;
	   }

	   PN512_RF_WorkInfo.ucMifActivateFlag = 1;
	   PN512_RF_WorkInfo.ucCurPCB = 1;

	   return RET_RF_OK;
}


uchar PN512_s_RF_ucRATS(uchar* pucOutLen, uchar* pucATSData)
{
    uchar ucTempData = 0x00;
    uchar ucRet = RET_RF_OK;
    uchar tucTemp[2];
    uchar i = 0;
    uint  uiFWITemp = 0x00, uiSFGITemp = 0x00;
    ulong ulTemp = 0;
    
    *pucOutLen = 0;

    PN512_RF_WorkInfo.FSC    = 32;
    PN512_RF_WorkInfo.FSD    = 256;
	PN512_RF_WorkInfo.ucFWI  = 4;
    PN512_RF_WorkInfo.ulFWT  = 560; // (256*16/Fc)*2^4 + 3*2^4ETU
    PN512_RF_WorkInfo.ucSFGI = 0;
    PN512_RF_WorkInfo.ulSFGT = 60; // PayPassҪ�󣬵�SFGI=0ʱ��Ӧ������ʱ500us
    // Ĭ�ϲ�����CID��NAD
    PN512_RF_WorkInfo.ucCIDFlag = 0;
    PN512_RF_WorkInfo.ucNADFlag = 0;

    tucTemp[0] = 0xE0;
    tucTemp[1] = 0x80;  // FSD = 256Bytes

    // ���ö�ʱ��
    PN512_s_RFSetTimer(RF_FWTRATS); // ��ʱ560ETU 
    // RxCRC,TxCRC, ISO14443
    ucTempData = 0x80;
    PN512_s_vRFWriteReg(1, PN512_TXMODE_REG, &ucTempData);
	PN512_s_vRFWriteReg(1, PN512_RXMODE_REG, &ucTempData);

    // ׼����������: ������
    PN512_RF_WorkInfo.ulSendBytes = 2;     // how many bytes to send
    memcpy((uchar*)PN512_RF_WorkInfo.aucBuffer, tucTemp, PN512_RF_WorkInfo.ulSendBytes);

	// �������������Ӧ
    ucRet = PN512_s_RFExchangeCmd(PN512_TRANSCEIVE_CMD);

    if(ucRet == RET_RF_ERR_TIMEOUT)  // ��ʱ����Ӧ��ֱ�ӷ���
    {
          PN512_RF_WorkInfo.usErrNo = 0xA4; // timeout err
          return RET_RF_ERR_TIMEOUT;
    }

	if(ucRet != RET_RF_OK)
	{
		  return RET_RF_ERR_TRANSMIT;
	}
	else
    {
           i = 0;
           ulTemp = PN512_RF_WorkInfo.ulBytesReceived;
           //if(PN512_RF_WorkInfo.ulBytesReceived != PN512_RF_WorkInfo.aucBuffer[0]) // TL�ֽڴ���
           if(ulTemp != PN512_RF_WorkInfo.aucBuffer[0]) // TL�ֽڴ���
           {
                PN512_RF_WorkInfo.usErrNo = 0x80; // ATS TL err
                ucRet = RET_RF_ERR_TRANSMIT; // ͨ�Ŵ���
				goto RATSEND;
           }
           // ����ATS����
           *pucOutLen = PN512_RF_WorkInfo.aucBuffer[0];
           memcpy(pucATSData,(uchar*)PN512_RF_WorkInfo.aucBuffer,PN512_RF_WorkInfo.ulBytesReceived);

           // ���ATS������T0��TA1��TB1����TC1����Ĭ�ϲ�������
           if(PN512_RF_WorkInfo.aucBuffer[0] <= 1) // ֻ����TL�ֽ�
           {
               goto RATSEND;
           }

           i = 1; // ȡT0�ֽ�

           // T0�ֽڶ���FSC��TA1��TB1��TC1�Ĵ������
           if((PN512_RF_WorkInfo.aucBuffer[1] & 0x80) != 0x00) // Bit8����Ϊ0
           {
                PN512_RF_WorkInfo.usErrNo = 0x81; // ATS T0 err
			    ucRet = RET_RF_ERR_PROTOCOL; // Э�����
                goto RATSEND;
           }
           else
           {
                // ��Ƭ�ɽ������֡����
			    PN512_RF_WorkInfo.FSC = PN512_gl_RF_ulFSList[PN512_RF_WorkInfo.aucBuffer[1] & 0x0F];
           }
           // TA1�ֽڶ����豸�뿨֮���ͨ�����ʣ�Ĭ��Ϊ106KBits/s
		   // ����TA1�����κ�ֵ������оƬ�뿨Ƭ֮���԰�106KBits/s����ͨ��
           if((PN512_RF_WorkInfo.aucBuffer[1] & 0x10) == 0x10)  // TA1�ֽ�
           {
                i++;
				if((PN512_RF_WorkInfo.aucBuffer[i] & 0x08) != 0x00)
				{
				     PN512_RF_WorkInfo.usErrNo = 0x82; // ATS TA1 err
					 ucRet = RET_RF_ERR_PROTOCOL; // Э�����
                     goto RATSEND;
				}
           }
           // TB1�ֽڶ���FWT��SFGT
           if((PN512_RF_WorkInfo.aucBuffer[1] & 0x20) == 0x20) // TB1�ֽ�
           {
               i++;
               PN512_RF_WorkInfo.ucFWI = (PN512_RF_WorkInfo.aucBuffer[i] & 0x0F0) >> 4;

			   uiFWITemp = (uint)0x01 << PN512_RF_WorkInfo.ucFWI;
			   PN512_RF_WorkInfo.ulFWT = (ulong)(32 + 3) * uiFWITemp; //(256*16/Fc)*2^FWI + 3*2^FWIETU

			   PN512_RF_WorkInfo.ucSFGI = PN512_RF_WorkInfo.aucBuffer[i] & 0x0F;

			   uiSFGITemp = (uint)0x01 << PN512_RF_WorkInfo.ucSFGI;
			   PN512_RF_WorkInfo.ulSFGT = (ulong)(32 + 3) * uiSFGITemp; //(256*16/Fc)*2^SFGI + 3*2^SFGI ETU;
           }

           if((PN512_RF_WorkInfo.aucBuffer[1] & 0x40) == 0x40)  // TC1
           {
               i++;
               // TC1�ĸ�6λ����Ϊ000000
               if((PN512_RF_WorkInfo.aucBuffer[i] & 0xFC) != 0x00)
               {
                   PN512_RF_WorkInfo.usErrNo = 0x83; // ATS TC1 err
				   ucRet = RET_RF_ERR_PROTOCOL; // Э�����
				   goto RATSEND;
               }
               // ����PICC����ʲô��������֧��NAD��CID����
               // �ж�CID
               if(PN512_RF_WorkInfo.aucBuffer[i] & 0x02)
               {
                   PN512_RF_WorkInfo.ucCIDFlag = 1;
               }
               else
               {
                   PN512_RF_WorkInfo.ucCIDFlag = 0;
               }
               // �ж�NAD
               if(PN512_RF_WorkInfo.aucBuffer[i] & 0x01)
               {
                   PN512_RF_WorkInfo.ucNADFlag = 1;
               }
               else
               {
                   PN512_RF_WorkInfo.ucNADFlag = 0;
               }

               ucRet = RET_RF_OK;
           }
    }

RATSEND:

	// liuxl 20070929
	if (ucRet == RET_RF_OK)
	{
		PN512_RF_WorkInfo.ucInProtocol = 1;
		if(c_para.card_buffer_w == 1 && c_para.card_buffer_val)
			PN512_RF_WorkInfo.FSC = c_para.card_buffer_val;
	}
	// end
	
    PN512_s_RFDelay10ETUs(PN512_RF_WorkInfo.ulSFGT/10); // ��ʱSFGT�ٷ�����һ������
//    s_RF_DelayUs(100);
    //s_RFDelay100Us(6);// 20071224 ��ʱ600us
    return ucRet;
}


void PN512_s_RF_vHALTA(void)
{
    //uchar ucRet = RET_RF_OK;
    uchar ucTempData;

	// set TxLastBits to 0
	ucTempData = 0x00; 
	PN512_s_vRFWriteReg(1,PN512_BITFRAMING_REG, &ucTempData);
	
    // ��ʱ��ʱ������
    PN512_s_RFSetTimer(560); // ��ʱ560ETU

    // TxCRC, Parity enable
    ucTempData = 0x80;
    PN512_s_vRFWriteReg(1, PN512_TXMODE_REG, &ucTempData);

    // ׼����������: ֹͣ������, 0
    PN512_RF_WorkInfo.ulSendBytes = 2;
    PN512_RF_WorkInfo.aucBuffer[0] = PICC_HALT ;     // Halt command code
    PN512_RF_WorkInfo.aucBuffer[1] = 0x00;
	// dummy address
	
    // ִ������   
    //ucRet = PN512_s_RFExchangeCmd(PN512_TRANSCEIVE_CMD);
    PN512_s_RFExchangeCmd(PN512_TRANSCEIVE_CMD);
    // ��PCD������HLTA��ʼ����ΪPICC�ѳɹ����ղ���ȷִ����HLTA���������ȥ���
	// PICC������κ���Ӧ��

    // ��λ����Ĵ���
  //  ucTempData = 0x00;
  //  PN512_s_vRFWriteReg(1, PN512_COMMAND_REG, &ucTempData);

}



uchar  PN512_s_RF_ucWUPB(uchar* pucOutLen, uchar* pucOut)
{
     uchar ucRet = RET_RF_OK;
     uchar ucTempData;
	 uint uiFWITemp = 0x00;

	 PN512_s_vRFFlushFIFO();        //empty FIFO

     // ���ز�����ΪTypeB��
     PN512_s_vRFSelectType('B');
     
	 // RxCRC and TxCRC enable, ISO14443B
     ucTempData = 0x83;
     PN512_s_vRFWriteReg(1, PN512_TXMODE_REG, &ucTempData);
     ucTempData = 0x83;
	 PN512_s_vRFWriteReg(1, PN512_RXMODE_REG, &ucTempData);

 //    ucTempData = 0x83;
//	 PN512_s_vRFWriteReg(1, PN512_TXCONTROL_REG, &ucTempData);// for debug
//	 PN512_s_vRFSetBitMask(PN512_TXCONTROL_REG, 0x03);// Tx2RF-En, Tx1RF-En enable

     // ��ֹcrypto1��Ԫ
     PN512_s_vRFClrBitMask(PN512_STATUS2_REG, 0x08);
     PN512_s_RFSetTimer(RF_FWTWUPB*3);  //RF_DEFAULT);// ��ʱ�ȴ�60ETU     
     
     

	//׼����������: ������
     PN512_RF_WorkInfo.ulSendBytes = 3;    // how many bytes to send
     PN512_RF_WorkInfo.aucBuffer[0] = PICC_APF;
     PN512_RF_WorkInfo.aucBuffer[1] = 0x00;       // AFI = 0x00
     PN512_RF_WorkInfo.aucBuffer[2] = PICC_WUPB;  // TimeSlot = 0, WUPB

     // ִ������
     ucRet = PN512_s_RFExchangeCmd(PN512_TRANSCEIVE_CMD);

     if(ucRet == RET_RF_ERR_TIMEOUT) // ��ʱ����Ӧ
     {
           PN512_RF_WorkInfo.usErrNo = 0xA5; // timeout err
           return RET_RF_ERR_TIMEOUT;
     }
     if(ucRet != RET_RF_OK)
     {
           ucRet = RET_RF_ERR_TRANSMIT;
     }
     else
     {
		    if(PN512_RF_WorkInfo.ulBytesReceived != 12)
            {
             	  PN512_RF_WorkInfo.usErrNo = 0x50; // received bytes err
                  ucRet = RET_RF_ERR_TRANSMIT;
				  goto WUPBEND;
            }
            if(PN512_RF_WorkInfo.aucBuffer[0] != 0x50)
            {
             	  PN512_RF_WorkInfo.usErrNo = 0x51; // received byte 1 err
				  ucRet = RET_RF_ERR_PROTOCOL;
                  goto WUPBEND;
            }

		    *pucOutLen = PN512_RF_WorkInfo.ulBytesReceived;
            memcpy(pucOut, (uchar*)PN512_RF_WorkInfo.aucBuffer, PN512_RF_WorkInfo.ulBytesReceived);

            PN512_RF_WorkInfo.ucUIDLen = 4;
            // ����TypeB��UID���к� ��2~5�ֽ�
            memcpy((uchar*)PN512_RF_WorkInfo.ucUIDB, (uchar*)(PN512_RF_WorkInfo.aucBuffer + 1), 4);

            // ��6~9�ֽ�ΪApplication Data,һ��Ϊ0000���ն˿��Բ������

            // ��10�ֽ�ΪBitRate��PCD��PICCһ��ֻ֧��106KBit
            // ���ܸ��ֽڻ����κ�ֵ������106KBits/s����ͨ��
            if((PN512_RF_WorkInfo.aucBuffer[9] & 0x08) != 0x00)
            {
             	  PN512_RF_WorkInfo.usErrNo = 0x52; // received byte 10 err
				   ucRet = RET_RF_ERR_PROTOCOL;
                   goto WUPBEND;
			}
            // ��11�ֽڷ���֡��С��Э�� ��4λΪ֡��С����4λΪЭ��ֵ
			PN512_RF_WorkInfo.FSC = PN512_gl_RF_ulFSList[(PN512_RF_WorkInfo.aucBuffer[10] >> 4) & 0x0F];

            // ��4λ��ʾ�Ƿ�֧��ISO14443Э�飬Ĭ��֧��
            if((PN512_RF_WorkInfo.aucBuffer[10] & 0x0F) != 0x01)
            {
             	   PN512_RF_WorkInfo.usErrNo = 0x53; // received byte 11 err
                   ucRet = RET_RF_ERR_PROTOCOL;
				   goto WUPBEND;
            }

            // ��12�ֽڰ���FWI��CID��NAD����Ϣ
            // ��4λ����FWI
            PN512_RF_WorkInfo.ucFWI = (PN512_RF_WorkInfo.aucBuffer[11] & 0x0F0) >> 4;

			uiFWITemp = (uint)0x01 << PN512_RF_WorkInfo.ucFWI;
			PN512_RF_WorkInfo.ulFWT = (ulong)(32 + 3) * uiFWITemp; //(256*16/Fc)*2^FWI + 3*2^FWI ETU

            // ��3~4λ��ʾADC�����迼��
            // ��1~2λ��ʾCID��NAD���ƣ�����PICC����ʲô��������֧�ִ���
            // �ж�NAD
            if(PN512_RF_WorkInfo.aucBuffer[11] & 0x01)
            {
                 PN512_RF_WorkInfo.ucCIDFlag = 1;
            }
            else
            {
                 PN512_RF_WorkInfo.ucCIDFlag = 0;
            }
            // �ж�CID
            if(PN512_RF_WorkInfo.aucBuffer[11] & 0x02)
            {
                 PN512_RF_WorkInfo.ucNADFlag = 1;
            }
            else
            {
                 PN512_RF_WorkInfo.ucNADFlag = 0;
            }
     }

WUPBEND:

//	 PN512_s_RFDelay10ETUs(RF_FDTMIN/10); // ��ʱ500us�ٷ�����һ������
	 if(ucRet == 0)
	 {
		 if(c_para.card_buffer_w == 1 && c_para.card_buffer_val)
			 PN512_RF_WorkInfo.FSC = c_para.card_buffer_val;
	 }
     return ucRet;
}


uchar PN512_s_RF_ucAttrib(uchar* pucResp)
{
     uchar ucRet = RET_RF_OK;
     uchar ucTempData = 0x00;

     PN512_s_vRFFlushFIFO();        //empty FIFO

     // RxCRC and TxCRC enable
     ucTempData = 0x83;
     PN512_s_vRFWriteReg(1, PN512_TXMODE_REG, &ucTempData);
	 PN512_s_vRFWriteReg(1, PN512_RXMODE_REG, &ucTempData);

     PN512_s_RFSetTimer(PN512_RF_WorkInfo.ulFWT);  // ���ó�ʱʱ��

	 PN512_RF_WorkInfo.aucBuffer[0] = PICC_ATTRIB; // Attrib������
     // ��2~5�ֽ�ΪPUPI
     PN512_RF_WorkInfo.aucBuffer[1] = PN512_RF_WorkInfo.ucUIDB[0];
     PN512_RF_WorkInfo.aucBuffer[2] = PN512_RF_WorkInfo.ucUIDB[1];
     PN512_RF_WorkInfo.aucBuffer[3] = PN512_RF_WorkInfo.ucUIDB[2];
     PN512_RF_WorkInfo.aucBuffer[4] = PN512_RF_WorkInfo.ucUIDB[3];
     // ��6�ֽ�Ϊparam1������TR0��TR1����SOF��EOF
     // PAYPASSҪ�󣺲���Ĭ��TR0��TR1��֧��SOF��EOF
     PN512_RF_WorkInfo.aucBuffer[5] = 0x00;
     // ��7�ֽ�Ϊparam2������BitRate��Frame Size
     // PAYPASSҪ��BitRateΪ106KBits��Frame Size = 256Bytes
     PN512_RF_WorkInfo.aucBuffer[6] = 0x08;
     // ��8�ֽ�Ϊparam3������PCD�Ƿ�֧��ISO14443-4
     // PAYPASSҪ���ܹ�֧��ISO14443-4
     PN512_RF_WorkInfo.aucBuffer[7] = 0x01;
     // ��9�ֽ�Ϊparam4��������CID
     // PAYPASSҪ��֧��CID
     PN512_RF_WorkInfo.aucBuffer[8] = 0x00;

     PN512_RF_WorkInfo.ulSendBytes = 9;    // how many bytes to send

     ucRet = PN512_s_RFExchangeCmd(PN512_TRANSCEIVE_CMD);

     if(ucRet == RET_RF_ERR_TIMEOUT)
     {
           // ��ʱ����Ӧ��ֱ�ӷ���
           return RET_RF_ERR_TIMEOUT;
     }
     else if(ucRet != RET_RF_OK)
     {
           // ���������ͨ�Ŵ�����
           ucRet = RET_RF_ERR_TRANSMIT;
     }
     else
     {
           if(PN512_RF_WorkInfo.ulBytesReceived != 1)
           {
           		PN512_RF_WorkInfo.usErrNo = 0x95; // ACTB attrib err
                ucRet = RET_RF_ERR_TRANSMIT;
           }
           else
           {
			    if((PN512_RF_WorkInfo.aucBuffer[0] & 0x0F) != 0x00)
                {
                		PN512_RF_WorkInfo.usErrNo = 0x96; // ACTB attrib err
                       // CID����Ϊ0
                       ucRet = RET_RF_ERR_PROTOCOL;
                }
           }

           *pucResp = PN512_RF_WorkInfo.aucBuffer[0];
     }

	if (ucRet == RET_RF_OK)
	{
		PN512_RF_WorkInfo.ucInProtocol = 1;
	}
	
//     PN512_s_RFDelay10ETUs(RF_FDTMIN/10); // ��ʱ500us�ٷ�����һ������
     return ucRet;
}


uchar PN512_s_RF_ucHALTB(uchar* pucResp)
{
     uchar ucRet = RET_RF_OK;
     uchar ucTempData = 0x00;

     PN512_s_vRFFlushFIFO();        // empty FIFO

     // RxCRC and TxCRC enable
     ucTempData = 0x83;
     PN512_s_vRFWriteReg(1, PN512_TXMODE_REG, &ucTempData);
	 PN512_s_vRFWriteReg(1, PN512_RXMODE_REG, &ucTempData);

     PN512_s_RFSetTimer(PN512_RF_WorkInfo.ulFWT);  // ���ó�ʱʱ��

     // ׼����������: �����룫PUPI
     PN512_RF_WorkInfo.ulSendBytes = 5;    // how many bytes to send
     PN512_RF_WorkInfo.aucBuffer[0] = PICC_HALT; // HALT������
     // ��2~5�ֽ�ΪPUPI
     PN512_RF_WorkInfo.aucBuffer[1] = PN512_RF_WorkInfo.ucUIDB[0];
     PN512_RF_WorkInfo.aucBuffer[2] = PN512_RF_WorkInfo.ucUIDB[1];
     PN512_RF_WorkInfo.aucBuffer[3] = PN512_RF_WorkInfo.ucUIDB[2];
     PN512_RF_WorkInfo.aucBuffer[4] = PN512_RF_WorkInfo.ucUIDB[3];

	 ucRet = PN512_s_RFExchangeCmd(PN512_TRANSCEIVE_CMD);

     if(ucRet == RET_RF_OK)
     {
         if(PN512_RF_WorkInfo.ulBytesReceived != 1)
		 {
               ucRet = RET_RF_ERR_TRANSMIT;
		 }
		 else if(PN512_RF_WorkInfo.aucBuffer[0] == 0x0)
         {
               *pucResp = PN512_RF_WorkInfo.aucBuffer[0];
         }
         else
         {
               ucRet = RET_RF_ERR_PROTOCOL;
         }
     }
     else if(ucRet == RET_RF_ERR_TIMEOUT)
	 {
		 ucRet = RET_RF_ERR_TIMEOUT;
	 }
	 else
     {
         ucRet = RET_RF_ERR_TRANSMIT;
     }

//	 PN512_s_RFDelay10ETUs(RF_FDTMIN/10); // ��ʱ500us�ٷ�����һ������
     return ucRet;
}


uchar PN512_s_RF_ucActTypeB(uchar* pucOutLen, uchar* pucOut)
{
      uchar ucRet = RET_RF_OK;
      uchar ucTempData = 0x00;
      uchar ucReSendNo = 0;

      // ��ִ��WUPB����
      for(ucReSendNo = 0; ucReSendNo < 3; ucReSendNo++)
      {
            ucRet = PN512_s_RF_ucWUPB(pucOutLen, pucOut);
            if(ucRet != RET_RF_ERR_TIMEOUT)
            {
                  break;
            }
      }

      if(ucRet != RET_RF_OK)
	  {
           PN512_RF_WorkInfo.usErrNo = 0x90; // ACTB WUPA err
		   return ucRet;
	  }

      // �Ƚ�ATQB������ԭ�ȱ����ATQB����
      if(memcmp(pucOut, (uchar*)PN512_RF_WorkInfo.ucATQB, 12))
	  {
	  	   PN512_RF_WorkInfo.usErrNo = 0x91; // ACTB ATQB err
           return RET_RF_ERR_PROTOCOL;
	  }

      // ������ִ��ATTRIB����ʹTypeB������ACTIVATE״̬
      for(ucReSendNo = 0; ucReSendNo < 3; ucReSendNo++)
      {
           ucRet = PN512_s_RF_ucAttrib(&ucTempData);
           if(ucRet != RET_RF_ERR_TIMEOUT)
           {
                 break;
           }
      }

      if(ucRet != RET_RF_OK)
	  {
	  	   PN512_RF_WorkInfo.usErrNo |= 0x400; // ACTB  err
		   return ucRet;
	  }

      PN512_RF_WorkInfo.ucMifActivateFlag = 1;
	  PN512_RF_WorkInfo.ucCurPCB = 1;

      return RET_RF_OK;
}


uchar PN512_s_RF_ucPOLL(uchar* pucMifType)
{
       uchar ucRet = RET_RF_OK;
       uchar ucTempData = 0x00;
       uchar ucATQA[2];  // ���ڱ���ATQA����
       uchar ucATQB[12]; // ���ڱ���ATQB����
	   uchar i = 0;
	   

       memset(ucATQA, 0x00, sizeof(ucATQA));
       memset(ucATQB, 0x00, sizeof(ucATQB));

       // ����WUPA����жϸ�Ӧ���Ƿ���TypeA��
       ucRet = PN512_s_RF_ucWUPA(ucATQA);
	   if(ucRet == RET_RF_OK)
       {
            // �����ȡ��ATQA����
            PN512_RF_WorkInfo.ucATQA[0] = ucATQA[0];
            PN512_RF_WorkInfo.ucATQA[1] = ucATQA[1];

			// ��ȡUID��Ϣ�����ж��Ƿ��ж࿨����
            ucRet = PN512_s_RF_ucGetUID();
			if(ucRet == RET_RF_OK)
            {
                    // ���ÿ���ΪHALT̬
                    PN512_s_RF_vHALTA();
					//del ��ʱ���������ز���ʽѡ����ִ��,�����
					//PN512_s_RFDelay10ETUs(RF_TPDEALY/10); // ��ʱ500ETU
                    // �������ж��Ƿ���TypeB������
                    ucRet = PN512_s_RF_ucWUPB(&ucTempData,ucATQB);
					if(ucRet != RET_RF_ERR_TIMEOUT)
                    {                    
                        PN512_RF_WorkInfo.usErrNo = 0x41; // Type A + Type B exist err
                        // ������Ӧ��˵����Ӧ���ڳ���TypeA�⣬����TypeB��
                        // Ӧ�÷��ض࿨��ͻ
						return RET_RF_ERR_MULTI_CARD;
                    }

                    // ����˵����Ӧ����ֻ��һ��TypeA��
//                    PN512_s_RFDelay10ETUs(RF_TPDEALY/10); // ��ʱ500ETU
                    
					//����SAK����ϸ�ֿ�Ƭ����
					PN512_s_RF_ucCardTypeCheck(pucMifType);
					PN512_RF_WorkInfo.ucMifCardType = *pucMifType;
					PN512_RF_WorkInfo.ucCurType = RF_TYPEA;
					
                    return RET_RF_OK;
            }
            else
            {
//		        PN512_s_RFResetCarrier();
				return ucRet;
            }
       }
       else if(ucRet == RET_RF_ERR_TIMEOUT)
       {
            // ��Ӧ��û��TypeA����Ӧ��˵����Ӧ����û��TypeA������
            // ������Ӧ���ж��Ƿ�TypeB������

			//del by liuxl 20071223 ��ʱ���������ز���ʽѡ����ִ��,�����
            //PN512_s_RFDelay10ETUs(10);//RF_TPDEALY/10); // ��ʱ500ETU

            // �������ж��Ƿ���TypeB������
            ucRet = PN512_s_RF_ucWUPB(&ucTempData,ucATQB);
			if(ucRet == RET_RF_OK)
            {
                   // ����ATQB����
                   memcpy((uchar*)PN512_RF_WorkInfo.ucATQB, ucATQB, ucTempData);
                   // ��������TypeB����ΪHALT״̬
				   for(i = 0; i < 3; i++)
				   {
                        ucRet = PN512_s_RF_ucHALTB(&ucTempData);
						if(ucRet != RET_RF_ERR_TIMEOUT)
						{
							break;
						}
				   }

                   if(ucRet == RET_RF_OK)
                   {
						//del ��ʱ���������ز���ʽѡ����ִ��,�����
						//PN512_s_RFDelay10ETUs(RF_TPDEALY/10); // ��ʱ500ETU
                        ucRet = PN512_s_RF_ucWUPA(ucATQA);
                        if(ucRet != RET_RF_ERR_TIMEOUT)
                        {                        
             	            PN512_RF_WorkInfo.usErrNo = 0x60; // Type B & Type A exist err
                            // ��TypeA����Ӧ����˵������TypeA���࿨��ͻ
							return RET_RF_ERR_MULTI_CARD;
                        }

 //                       PN512_s_RFDelay10ETUs(RF_TPDEALY/10); // ��ʱ500ETU
                        // ��Ӧ����ֻ��һ��TypeB��
                        PN512_RF_WorkInfo.ucMifCardType = RF_TYPE_B;
						*pucMifType = PN512_RF_WorkInfo.ucMifCardType;
						PN512_RF_WorkInfo.ucCurType = RF_TYPEB;// 'B';
                        return RET_RF_OK;
                   }
                   else
				   {
                        //����λPICC�����������ִ��䡢Э�顢��ʱ����ʱִ��
//		                PN512_s_RFResetCarrier();
                        return ucRet;
				   }
            }
            else if(ucRet == RET_RF_ERR_TIMEOUT)
            {
                 //del��ʱ���������ز���ʽѡ����ִ��,�����
				//PN512_s_RFDelay10ETUs(RF_TPDEALY/10); // ��ʱ500ETU
            }
            else
            {
                // ����λPICC�����������ִ��䡢Э�顢��ʱ����ʱִ��
//		        PN512_s_RFResetCarrier();
            }
			return ucRet;
       }
       else
       {
            // ����λPICC�����������ִ��䡢Э�顢��ʱ����ʱִ��
//		    PN512_s_RFResetCarrier();
       }
	   // �������������ֱ�ӷ���
	   return ucRet;

}


uchar  PN512_s_RF_ucIfCardExit(void)
{
       uchar ucRet = RET_RF_OK;
       uchar ucTempData[12];
       uchar ucTemp = 0x00;
       uchar ucReSendNo = 0;

       if(PN512_RF_WorkInfo.ucCurType == RF_TYPEA)
       { 
            // �ȷ���WUPAѰ��������п���Ӧ����˵����δ�뿪��Ӧ��
            for(ucReSendNo = 0; ucReSendNo < 3; ucReSendNo++)
            {
                  ucRet = PN512_s_RF_ucWUPA(ucTempData);
                  if(ucRet != RET_RF_ERR_TIMEOUT)
                  { 
                       break;
                  }

                  if((ucRet == RET_RF_ERR_TIMEOUT) && (ucReSendNo == 2))
                  { 
                        // �޿���Ӧ����˵�������뿪��Ӧ��
                        return RET_RF_OK;
                  }
            }
            // ������Ӧ��˵����δ�뿪��Ӧ��
            // �Ƚ���Ƭ��ΪHALT̬
            PN512_s_RF_vHALTA(); 
            return RET_RF_ERR_CARD_EXIST;
       }
       else
       {
            // �ȷ���WUPBѰ��������п���Ӧ����˵����δ�뿪��Ӧ��
            for(ucReSendNo = 0; ucReSendNo < 3; ucReSendNo++)
            { 
                  ucRet = PN512_s_RF_ucWUPB(&ucTemp, ucTempData);
				  if(ucRet != RET_RF_ERR_TIMEOUT)
                  {
                       break;
                  }
                  if((ucRet == RET_RF_ERR_TIMEOUT) && (ucReSendNo == 2))
                  {
                        // �޿���Ӧ����˵�������뿪��Ӧ��
                        return RET_RF_OK;
                  }
            }
           // ������Ӧ��˵����δ�뿪��Ӧ��
           // �Ƚ���Ƭ��ΪHALT̬
//           for(ucReSendNo = 0; ucReSendNo < 3; ucReSendNo++)
           {
                 ucRet = PN512_s_RF_ucHALTB(&ucTemp);
                 if(ucRet != RET_RF_ERR_TIMEOUT)
                 {          
					 return RET_RF_ERR_CARD_EXIST;
					 // break;
                 }				 
                 else //if((ucRet == RET_RF_ERR_TIMEOUT) && (ucReSendNo == 2))  
                 {
                        // �޿���Ӧ����˵�������뿪��Ӧ��
                        return RET_RF_OK;
                 }
           }
       }

	   //return RET_RF_ERR_CARD_EXIST;
}


uchar PN512_s_RF_ucProcess(uint uiSendLen, uint* puiRecLen)
{
       uchar ucRet = RET_RF_OK;
       uchar ucTempData = 0x00;

       // ���ö�ʱ��
       PN512_s_RFSetTimer(PN512_RF_WorkInfo.ulFWT);

       if(PN512_RF_WorkInfo.ucCurType == RF_TYPEA)
       {
            ucTempData = 0x80;
            PN512_s_vRFWriteReg(1, PN512_TXMODE_REG, &ucTempData);
	        PN512_s_vRFWriteReg(1, PN512_RXMODE_REG, &ucTempData);
       }
       else
       {
            ucTempData = 0x83;
            PN512_s_vRFWriteReg(1, PN512_TXMODE_REG, &ucTempData);
	        PN512_s_vRFWriteReg(1, PN512_RXMODE_REG, &ucTempData);
       }

        // ׼����������: ������
        PN512_RF_WorkInfo.ulSendBytes = uiSendLen;    // how many bytes to send
        ucRet = PN512_s_RFExchangeCmd(PN512_TRANSCEIVE_CMD);

        if(ucRet == RET_RF_ERR_TIMEOUT)
		{
			ucRet = RET_RF_ERR_TIMEOUT;
		}
		else if(ucRet != RET_RF_OK)
        {
			ucRet = RET_RF_ERR_TRANSMIT;
        }

        if(ucRet == RET_RF_OK)
        {
		    *puiRecLen = PN512_RF_WorkInfo.ulBytesReceived;
		    if(PN512_RF_WorkInfo.ulBytesReceived == 0)
			{
				// ��������ȷ������ʵ�ʽ��ճ���Ϊ0ʱ�������������
				ucRet = RET_RF_ERR_TRANSMIT;
			}
            // ����TypeB������PICC����TR0Min + TR1Min��ʼ�������ݵĻ�
            // ����PAYPASSҪ�󣬿����϶�Ϊ�������Ҳ�����������գ�
            // �ڱ���������ж����������
        }

		PN512_s_RFDelay10ETUs(6);//PN512_s_RFDelay10ETUs(RF_FDTMIN/10);  // ����֮����ʱ500us���Ա�֤��С���򱣻�ʱ��
        return( ucRet);
}


uchar PN512_s_RF_ucDeselect(void)
{
      uchar ucRet = RET_RF_OK;
      uchar ucTempData = 0x00;
      uchar i = 0;

      if(PN512_RF_WorkInfo.ucCurType == RF_TYPEA)
      {
            ucTempData = 0x80;
            PN512_s_vRFWriteReg(1, PN512_TXMODE_REG, &ucTempData);
	        PN512_s_vRFWriteReg(1, PN512_RXMODE_REG, &ucTempData);
      }
      else
      {
            ucTempData = 0x83;
            PN512_s_vRFWriteReg(1, PN512_TXMODE_REG, &ucTempData);
	        PN512_s_vRFWriteReg(1, PN512_RXMODE_REG, &ucTempData);
      }

      while(1)
      {
          // ���ö�ʱ��
          PN512_s_RFSetTimer(RF_FWTDESELECT);

          PN512_RF_WorkInfo.aucBuffer[0] = 0xC2; // 1100 0010

          // ׼����������: ������
          PN512_RF_WorkInfo.ulSendBytes = 1;   // how many bytes to send
          ucRet = PN512_s_RFExchangeCmd(PN512_TRANSCEIVE_CMD);


          if((ucRet != RET_RF_OK) && (ucRet != RET_RF_ERR_TIMEOUT))
          {
                 ucRet = RET_RF_ERR_TRANSMIT;
          }

          if(ucRet == RET_RF_ERR_TIMEOUT)
		  {
			  ucRet = RET_RF_ERR_TIMEOUT;
		  }

		  if((ucRet == RET_RF_ERR_TRANSMIT) || (ucRet == RET_RF_ERR_TIMEOUT))
          {
			    i++;
                if(i > 2)
                {
                       break;
                }
                // �ط�DESELECT����
                continue;
          }

          if(ucRet == RET_RF_OK)
          {
                if(PN512_RF_WorkInfo.ulBytesReceived != 1)
                {
                       ucRet = RET_RF_ERR_TRANSMIT;
                }
				else if(PN512_RF_WorkInfo.aucBuffer[0] != 0xC2)
				{
					   ucRet = RET_RF_ERR_PROTOCOL;
				}
                break;
          }
      }

      // ����λPICC�����������ִ��䡢Э�顢��ʱ����ʱִ��
      if(ucRet != RET_RF_OK)
      {
          PN512_s_RFResetCarrier();
      }

      PN512_RF_WorkInfo.ucMifActivateFlag = 0;
      PN512_RF_WorkInfo.ucHalt = 1;

	  return  ucRet;
}


uchar  PN512_s_RF_ucMifRBlock(uint* puiRecLen)
{
       uchar result = RET_RF_OK;       
	   uchar ucReSendNo = 0x00;
	   
	   // ��෢��NAK����
       for(ucReSendNo = 0; ucReSendNo < 2; ucReSendNo++)
       {
             // ����NAK
             PN512_RF_WorkInfo.aucBuffer[0] = PN512_RF_WorkInfo.ucCurPCB | 0xB2;
             result = PN512_s_RF_ucProcess(1, puiRecLen);
             if((result != RET_RF_ERR_TIMEOUT) && (result != RET_RF_ERR_TRANSMIT))
             {              
                    // ������ֳ�ʱ���ߴ��������Ӧ�ط�NAK
                    break;
             }
       }
       return result;      
}


uchar PN512_s_RF_ucMifSBlock( uint* puiRecLen)
{
    uchar ucTempData = 0;
    uchar ucRet = RET_RF_OK;	
    ulong ulTemp1, ulTemp2 = 0;

	if ((PN512_RF_WorkInfo.aucBuffer[0] & 0x30) != 0x30)
	{
		// ����WTX������
		return RET_RF_ERR_PROTOCOL;
	}
	if ((PN512_RF_WorkInfo.aucBuffer[0] & 0x08) == 0x08) 
	{
		// PICC��Ӧ��CID�ֽ�
		return RET_RF_ERR_PROTOCOL;
	}
	else
	{ 
		ucTempData = PN512_RF_WorkInfo.aucBuffer[1];
		if((ucTempData & 0xC0) != 0x00)
		{
			// ��֧��Power Level
			return RET_RF_ERR_PROTOCOL;
		}
		// ��ȡWTXM����
		PN512_RF_WorkInfo.ucWTX = ucTempData & 0x3F;
		if(PN512_RF_WorkInfo.ucWTX == 0x00)
		{
			return RET_RF_ERR_PROTOCOL;
		}
		if(PN512_RF_WorkInfo.ucWTX  > 59)
		{
			PN512_RF_WorkInfo.ucWTX = 59;
		}
    
		// ������ʱFWTʱ��
		ulTemp1 = PN512_RF_WorkInfo.ucWTX;
		ulTemp2 = PN512_RF_WorkInfo.ulFWT;
		//PN512_RF_WorkInfo.ulFWTTemp = PN512_RF_WorkInfo.ucWTX * PN512_RF_WorkInfo.ulFWT;
		PN512_RF_WorkInfo.ulFWTTemp = ulTemp1 * ulTemp2;
	}

	PN512_RF_WorkInfo.aucBuffer[0] = 0xf2;  
	PN512_RF_WorkInfo.aucBuffer[1] = ucTempData  & 0x3F;        

//	if(PN512_RF_WorkInfo.ulFWTTemp > MAXFWT)
//	{
//		 // ���ö�ʱ��,��󲻳���FWT_MAX
//		 return RET_RF_ERR_PROTOCOL;
//	}
//	else
//	{
		 PN512_s_RFSetTimer(PN512_RF_WorkInfo.ulFWTTemp);
//  }
    if(PN512_RF_WorkInfo.ucCurType == RF_TYPEA)
    {
         ucTempData = 0x80;// TxCRC��RxCRC��Parity Enable
         PN512_s_vRFWriteReg(1, PN512_TXMODE_REG, &ucTempData);
	     PN512_s_vRFWriteReg(1, PN512_RXMODE_REG, &ucTempData);
    }
    else
    {
         ucTempData = 0x83;// TxCRC��RxCRC Enable
         PN512_s_vRFWriteReg(1, PN512_TXMODE_REG, &ucTempData);
	     PN512_s_vRFWriteReg(1, PN512_RXMODE_REG, &ucTempData);
    }
	   
	// ׼����������: ������
	PN512_RF_WorkInfo.ulSendBytes = 2;     // how many bytes to send
        
	ucRet = PN512_s_RFExchangeCmd(PN512_TRANSCEIVE_CMD);
	
	if(ucRet == RET_RF_ERR_TIMEOUT)
	{
			ucRet = RET_RF_ERR_TIMEOUT;
	}
	else if(ucRet != RET_RF_OK)
	{	
		ucRet = RET_RF_ERR_TRANSMIT;
	}
	else
	{
			*puiRecLen = PN512_RF_WorkInfo.ulBytesReceived;
			if(PN512_RF_WorkInfo.ulBytesReceived == 0)
			{
				// ��������ȷ���Ƿ���ʵ�ʽ��ճ���Ϊ0ʱ�������������				
				ucRet = RET_RF_ERR_TRANSMIT;
			}
			
	}

	//PN512_s_RFDelay10ETUs(RF_FDTMIN/10);// ��ʱ500us�ٷ�����һ������
	//for debug 20071224 should be del
	//PN512_s_RFDelay10ETUs(RF_TPDEALY/10);
    
    return( ucRet); 
}


uchar PN512_s_RF_ucExchange(uint uiSendLen, uchar* pucInData, uint* puiRecLen, uchar* pucOutData)
{	  
    uint  uiSendLeftLen = 0, uiTempLen = 0, uiSendCurrPos = 0,uiCurrRecvLen = 0;
	uchar ucRet = RET_RF_OK;
	uchar tempResult = 0x00;
	uchar i = 0;
    uchar ucResendNo = 0x00;   
	ulong ulTemp = 0;

	*puiRecLen = 0; 

	// ����Ҫ���͵����ݳ���
	uiSendLeftLen =	uiSendLen;	  
	// �����֡���Ȱ�������CRC�ֽں�PCB�ֽ�
	// PAYPASSҪ�󲻰���CID�ֽں�NAD�ֽ�
	while(uiSendLeftLen	> (PN512_RF_WorkInfo.FSC - 3))
	{		   
		 uiTempLen = PN512_RF_WorkInfo.FSC - 3;
		 PN512_RF_WorkInfo.ucCurPCB = (PN512_RF_WorkInfo.ucCurPCB + 1) & 0x01;
		 PN512_RF_WorkInfo.aucBuffer[0] = PN512_RF_WorkInfo.ucCurPCB | 0x12;		   
		 memcpy((uchar*)(&PN512_RF_WorkInfo.aucBuffer[1]), pucInData + uiSendCurrPos,	uiTempLen);
		 ucRet = PN512_s_RF_ucProcess(uiTempLen+1, &uiCurrRecvLen);
		 
		 //	�����ʱû�н��յ�����,����NAKҪ��PICC��������
		 //	������չ����г��ִ���Ҳ����NAKҪ��PICC��������
		 if((ucRet	== RET_RF_ERR_TIMEOUT) ||	(ucRet	== RET_RF_ERR_TRANSMIT))
		 {
			    tempResult = ucRet;
			    ucRet = PN512_s_RF_ucMifRBlock(&uiCurrRecvLen);
				if(ucRet != RET_RF_OK)
				{				
					// ��Ȼû�н��յ���Ч��Ӧ���ݣ���֪ͨ�豸
					 return tempResult;
				}
		 }		 
		 
		 if(ucRet != RET_RF_OK)
		 {
			 return ucRet;
		 }

		 if(uiCurrRecvLen >	(PN512_RF_WorkInfo.FSD - 2))
		 {
				// ���ճ��ȳ�����FSD
				return RET_RF_ERR_PROTOCOL;
		 }
		 
		 if (uiCurrRecvLen == 0)
		 { 
			    // ���ݽ���ʧ��
				return RET_RF_ERR_TRANSMIT;
		 }

		 if((PN512_RF_WorkInfo.aucBuffer[0] & 0xE2) == 0x02)
		 {
				// ���յ�I_Block, ��Э�������
				return RET_RF_ERR_PROTOCOL;
		 }
		 else if((PN512_RF_WorkInfo.aucBuffer[0] & 0xE2) == 0xA2)
		 {
				// ���յ�R_Block
				if((PN512_RF_WorkInfo.aucBuffer[0] & 0x10) == 0x10)
				{
					 // PICC��Ӧ����NAK
					 return RET_RF_ERR_PROTOCOL;
				}
				
				if((PN512_RF_WorkInfo.aucBuffer[0] & 0x08) == 0x08)
				{
					 // PICC��Ӧ����CID
					 return	RET_RF_ERR_PROTOCOL;
				}
				ulTemp = PN512_RF_WorkInfo.ucCurPCB;
				//if((PN512_RF_WorkInfo.aucBuffer[0] & 0x01) == PN512_RF_WorkInfo.ucCurPCB)
				if((PN512_RF_WorkInfo.aucBuffer[0] & 0x01) == ulTemp)
				{
					 // �յ�R_block(ACK)Ӧ�������ȷ, ׼��������һ����Ϣ��
				     uiSendCurrPos += uiTempLen;
				     uiSendLeftLen -= uiTempLen;
				   
				     // �ط���������
				     ucResendNo = 0;
				}
				else
			    {   
				     if(ucResendNo++ > 3)
				     {
						 //	PCD�ط�һ��I_Block��������
						   return RET_RF_ERR_PROTOCOL;
				     }
				     // �����Ų���ȷ, ���ط�
				     PN512_RF_WorkInfo.ucCurPCB	= (PN512_RF_WorkInfo.ucCurPCB + 1) & 0x01;				  
			   }
		 }
		 else if((PN512_RF_WorkInfo.aucBuffer[0] & 0xC7) == 0xC2)
		 {
				// ���յ�S_Block
				// ����S_Block
				while(1)
				{
						ucRet = PN512_s_RF_ucMifSBlock(&uiCurrRecvLen);
						if((ucRet == RET_RF_ERR_TIMEOUT)	|| (ucRet == RET_RF_ERR_TRANSMIT))
						{
							tempResult = ucRet;
							// ������Ӧ�����ͨ�Ŵ�������NAK
							ucRet = PN512_s_RF_ucMifRBlock(&uiCurrRecvLen);
							if(ucRet != RET_RF_OK)
							{
								   // ��Ȼû�н��յ���Ч��Ӧ���ݣ���֪ͨ�豸
								   return tempResult;
							}
						}						
						
						if(ucRet != RET_RF_OK)
						{
							 return ucRet;
						}

						if(uiCurrRecvLen > (PN512_RF_WorkInfo.FSD - 2))
						{
								// ���ճ��ȳ�����FSD
								return RET_RF_ERR_PROTOCOL;
						}

						if (uiCurrRecvLen == 0)
						{  
							    // ���ݽ���ʧ��
								return RET_RF_ERR_TRANSMIT;
						}
										
						if((PN512_RF_WorkInfo.aucBuffer[0] & 0xE2) == 0x02)
						{
								// ���յ�I_Block, ��Э�������
								return RET_RF_ERR_PROTOCOL;
						}
						else if((PN512_RF_WorkInfo.aucBuffer[0]	& 0xE2)	== 0xA2)
						{						
								// ���յ�R_Block
								if((PN512_RF_WorkInfo.aucBuffer[0] & 0x10) == 0x10)
								{
									   // PICC��Ӧ����NAK
									   return RET_RF_ERR_PROTOCOL;
								}
								
								if((PN512_RF_WorkInfo.aucBuffer[0] & 0x08) == 0x08)
								{
									   // PICC��Ӧ����CID
									   return	RET_RF_ERR_PROTOCOL;
								}
								ulTemp = PN512_RF_WorkInfo.ucCurPCB;
								//if((PN512_RF_WorkInfo.aucBuffer[0] & 0x01) == PN512_RF_WorkInfo.ucCurPCB)
                                if((PN512_RF_WorkInfo.aucBuffer[0] & 0x01) == ulTemp)
								{
								   // �յ�R_block(ACK)Ӧ�������ȷ, ׼��������һ����Ϣ��
								   uiSendCurrPos +=	uiTempLen;
								   uiSendLeftLen -=	uiTempLen;
								   
								   // �ط���������
								   ucResendNo =	0;
								}
								else
								{	 
									 // �����Ų���ȷ, �ط��ϸ�I_BLOCk						  
									 if(ucResendNo++ > 3)
									 {
										   // PCD�ط�һ��I_Block��������
										   return RET_RF_ERR_PROTOCOL;
									 }
									 // �����Ų���ȷ, ���ط�
									 PN512_RF_WorkInfo.ucCurPCB	= (PN512_RF_WorkInfo.ucCurPCB + 1) &	0x01;						  
								}
								break;
						 }
						 else if((PN512_RF_WorkInfo.aucBuffer[0] & 0xC7) != 0xC2)
						 {
                               // ���յ���������Block
							   return	RET_RF_ERR_PROTOCOL;
						 }
				}                
		 }
		 else
		 {
				// PCBֵ����ȷ						  
				return RET_RF_ERR_PROTOCOL;
		 }
	}
	
	ucResendNo = 0; // �ط���������
	// �������һ֡����
	while(1)
	{		   
		 PN512_RF_WorkInfo.ucCurPCB = (PN512_RF_WorkInfo.ucCurPCB + 1) & 0x01;
		   
		 memcpy((uchar*)(&PN512_RF_WorkInfo.aucBuffer[1]), pucInData + uiSendCurrPos, uiSendLeftLen);
		 PN512_RF_WorkInfo.aucBuffer[0] = PN512_RF_WorkInfo.ucCurPCB | 0x02;	// PCB��ֵ,��ǰ��NAD��CID		   
		
		 ucRet	= PN512_s_RF_ucProcess(uiSendLeftLen+1, &uiCurrRecvLen); 		
 		 //	�����ʱû�н��յ�����,����NAKҪ��PICC��������
		 //	������չ����г��ִ���Ҳ����NAKҪ��PICC��������
		 if((ucRet	== RET_RF_ERR_TIMEOUT) || (ucRet	== RET_RF_ERR_TRANSMIT))
		 {	   
			    tempResult = ucRet;
				/*/for debug 070915 for display
				ScrPrint(0,2,1,"S RBlock: 01:%02X ", result);
				getkey();
				// debug end*/
				
			    ucRet = PN512_s_RF_ucMifRBlock(&uiCurrRecvLen);
				
				/*for debug 070915 for display
				ScrPrint(0,2,1,"S RBlock: 02:%02X ", result);
				getkey();
				debug end*/
				if(ucRet != RET_RF_OK)
				{ 
					   // ��Ȼû�н��յ���Ч��Ӧ���ݣ���֪ͨ�豸
					   return tempResult;
				}
		 }
		 
		 if(ucRet != RET_RF_OK)
		 {	 
			 return ucRet;
		 }

		 if(uiCurrRecvLen >	(PN512_RF_WorkInfo.FSD - 2))
		 {
				// ���ճ��ȳ�����FSD
				return RET_RF_ERR_PROTOCOL;
		 }

		 if (uiCurrRecvLen == 0)
		 { 	  
			    // ���ݽ���ʧ��			 
			 return RET_RF_ERR_TRANSMIT;
		 }
		 
		 if((PN512_RF_WorkInfo.aucBuffer[0] & 0xE2) == 0xA2)
		 {						
				// ���յ�R_Block
				if((PN512_RF_WorkInfo.aucBuffer[0] & 0x10) == 0x10)
				{
					   // PICC��Ӧ����NAK
					   return	RET_RF_ERR_PROTOCOL;
				}
				
				if((PN512_RF_WorkInfo.aucBuffer[0] & 0x08) == 0x08)
				{
					   // PICC��Ӧ����CID
					   return	RET_RF_ERR_PROTOCOL;
				}
				ulTemp = PN512_RF_WorkInfo.ucCurPCB;
				//if((PN512_RF_WorkInfo.aucBuffer[0] & 0x01) == PN512_RF_WorkInfo.ucCurPCB)
                if((PN512_RF_WorkInfo.aucBuffer[0] & 0x01) == ulTemp)
				{
					   // PICC���ʹ�����
					   return	RET_RF_ERR_PROTOCOL;
				}
				else
				{
					   if(ucResendNo++ > 3)
				       {
						   // PCD�ط�һ��I_Block��������
						   return	RET_RF_ERR_PROTOCOL;
				       }
					   // �����ط�
					   PN512_RF_WorkInfo.ucCurPCB = (PN512_RF_WorkInfo.ucCurPCB + 1) & 0x01; 
				}
		 }
		 else if((PN512_RF_WorkInfo.aucBuffer[0] & 0xC7) == 0xC2)
		 {
				// ���յ�S_Block
				// ����S_Block
				while(1)
				{	
					ucRet = PN512_s_RF_ucMifSBlock(&uiCurrRecvLen);
					if((ucRet == RET_RF_ERR_TIMEOUT)	|| (ucRet == RET_RF_ERR_TRANSMIT))
					{
					
						/*for debug  for display
						ScrPrint(0,2,1,"S SBlock: 01:%02X ", result);
						getkey();
						debug end*/
						tempResult = ucRet;
						// ������Ӧ�����ͨ�Ŵ�������NAK
						ucRet = PN512_s_RF_ucMifRBlock(&uiCurrRecvLen);
				
						/*for debug  for display
						ScrPrint(0,2,1,"S SBlock: 02:%02X ", result);
						getkey();
						debug end*/
						
						if(ucRet != RET_RF_OK)
						{
							   // ��Ȼû�н��յ���Ч��Ӧ���ݣ���֪ͨ�豸
							   return tempResult;
						}
					}
					
					if(ucRet != RET_RF_OK)
					{
						 return ucRet;
					}

					if (uiCurrRecvLen == 0)
					{ 
						    // ���ݽ���ʧ��
							return RET_RF_ERR_TRANSMIT;
					}

					if(uiCurrRecvLen > (PN512_RF_WorkInfo.FSD - 2))
					{
						  // ���ճ��ȳ�����FSD
						  return RET_RF_ERR_PROTOCOL;
					}
					  
					if((PN512_RF_WorkInfo.aucBuffer[0] & 0xE2) == 0x02)
					{	
						// ���յ�I_Block
						break;
					}
					else if((PN512_RF_WorkInfo.aucBuffer[0]	& 0xE2)	== 0xA2)
					{
						// ���յ�R_Block
						if((PN512_RF_WorkInfo.aucBuffer[0] & 0x10) == 0x10)
						{
							   // PICC��Ӧ����NAK
							   return	RET_RF_ERR_PROTOCOL;
						}
						
						if((PN512_RF_WorkInfo.aucBuffer[0] & 0x08) == 0x08)
						{
							   // PICC��Ӧ����CID
							   return	RET_RF_ERR_PROTOCOL;
						}
						ulTemp = PN512_RF_WorkInfo.ucCurPCB;
				        //if((PN512_RF_WorkInfo.aucBuffer[0] & 0x01) == PN512_RF_WorkInfo.ucCurPCB)
                        if((PN512_RF_WorkInfo.aucBuffer[0] & 0x01) == ulTemp)
						{
							   // PICC���ʹ�����
							   return	RET_RF_ERR_PROTOCOL;
						}
						else
						{
							   if(ucResendNo++ > 3)
							   {
								   // PCD�ط�һ��I_Block��������
								   return	RET_RF_ERR_PROTOCOL;
							   }
							   // �����ط�
							   PN512_RF_WorkInfo.ucCurPCB = (PN512_RF_WorkInfo.ucCurPCB +	1) & 0x01; 
						}
						break;
					}
					else if((PN512_RF_WorkInfo.aucBuffer[0] & 0xC7) != 0xC2)
					{
                        // ���յ���������Block
						return RET_RF_ERR_PROTOCOL;
					}
				}

				if((PN512_RF_WorkInfo.aucBuffer[0] & 0xE2) == 0x02)
				{	
					// ���յ�I_Block
					break;
				}
		 }
		 else if((PN512_RF_WorkInfo.aucBuffer[0] & 0xE2) == 0x02)
		 {
		 	/*for debug  
		 	ScrPrint(0,6,0,"Rec I Block:%02X",uiCurrRecvLen);
			getkey();
			debug end*/
			
				// ���յ�I_Block
				break;
		 }
		 else
		 {
				// ���յ�������֧�ֵ�PCB
				return RET_RF_ERR_PROTOCOL;
		 }		 
	}
	
 	/*for debug  
 	ScrPrint(0,6,0,"Rec I Block out");
	getkey();
	debug end*/
	// ����PICC���͵�I_Block
	while(1)
	{
		 	/*for debug liuxl 
		 	ScrPrint(0,6,0,"Save In %02X %02X %02X",PN512_RF_WorkInfo.aucBuffer[0],PN512_RF_WorkInfo.ucCurPCB,uiCurrRecvLen);
			getkey();
			debug end*/
			ulTemp = PN512_RF_WorkInfo.ucCurPCB;
		   //if((PN512_RF_WorkInfo.aucBuffer[0] & 0x01) != PN512_RF_WorkInfo.ucCurPCB)
            if((PN512_RF_WorkInfo.aucBuffer[0] & 0x01) != ulTemp)
		   {
				 // PICC���ʹ�����
				 return	RET_RF_ERR_PROTOCOL;
		   }
		   
		   if((PN512_RF_WorkInfo.aucBuffer[0] & 0x08) == 0x08)
		   {
				  // PICC��Ӧ����CID
				  return RET_RF_ERR_PROTOCOL;
		   }
		   // ����Խ���жϣ� 
		   if(uiCurrRecvLen < 1)
		   {
			   return RET_RF_ERR_TRANSMIT;
		   }
		   //	������յ���I_Block����
		   memcpy(&pucOutData[*puiRecLen], (uchar*)(&PN512_RF_WorkInfo.aucBuffer[1]), uiCurrRecvLen-1);
		   *puiRecLen += uiCurrRecvLen - 1;			 
		   			
		   if((PN512_RF_WorkInfo.aucBuffer[0] & 0x10) == 0x00)
		   {
				 //	���յ����һ��I_Block���򷵻�
				 break;
		   }
		   // ��תBlock��
		   PN512_RF_WorkInfo.ucCurPCB = (PN512_RF_WorkInfo.ucCurPCB + 1) & 0x01;
		   // PICC��������I_Block
		   i = 0;
		   while(1)
		   {
				// ����ACK��ȡ����I_Block
				PN512_RF_WorkInfo.aucBuffer[0] = PN512_RF_WorkInfo.ucCurPCB | 0xA2; //	PCB��ֵ,��ǰ��NAD��CID
			    ucRet = PN512_s_RF_ucProcess(1,&uiCurrRecvLen);
				
				if((ucRet == RET_RF_ERR_TIMEOUT) || (ucRet == RET_RF_ERR_TRANSMIT))
				{
						/*for debug  for display
						ScrPrint(0,2,1,"R IBlock: 01:%d %02X ",i, result);
						getkey();
						debug en*/
					   i++;
					   if(i	== 3)
					   {
						    // ACK�����������������
							return ucRet;
					   }
					   // û����Ӧ����ͨ�Ŵ������ط�ACK
					   continue;
				}				
				
				if(ucRet != RET_RF_OK)
				{
					 return ucRet;
				}

				if (uiCurrRecvLen == 0)
				{  
					    // ���ݽ���ʧ��
						return RET_RF_ERR_TRANSMIT;
				}

				if(uiCurrRecvLen > (PN512_RF_WorkInfo.FSD - 2))
				{
					  // ���ճ��ȳ�����FSD
					  return RET_RF_ERR_PROTOCOL;
				}
				  
				if((PN512_RF_WorkInfo.aucBuffer[0] & 0xE2) == 0x02)
				{
					// I_Block
					break;  
				}
				else if((PN512_RF_WorkInfo.aucBuffer[0] & 0xE2) == 0xA2)
				{	   
					   // ���յ�R_Block
					   if((PN512_RF_WorkInfo.aucBuffer[0] & 0x10) == 0x10)
					   {
							// PICC��Ӧ����NAK
							return RET_RF_ERR_PROTOCOL;
					   }
						
					   if((PN512_RF_WorkInfo.aucBuffer[0] & 0x08) == 0x08)
					   {
							// PICC��Ӧ����CID
							return RET_RF_ERR_PROTOCOL;
					   }

					    ulTemp = PN512_RF_WorkInfo.ucCurPCB;
					   //if((PN512_RF_WorkInfo.aucBuffer[0] & 0x01) != PN512_RF_WorkInfo.ucCurPCB)
					   if((PN512_RF_WorkInfo.aucBuffer[0] & 0x01) != ulTemp)
                       {
							// PICC���ʹ�����
							return RET_RF_ERR_PROTOCOL;
					   }

					   i++;
					   if(i	== 3)
					   {
						   // ACK�����������������
						   return RET_RF_ERR_PROTOCOL;
					   }					   
					   continue;
				}
				else if((PN512_RF_WorkInfo.aucBuffer[0] & 0xC7) == 0xC2)
				{
				    // ���յ�S_Block
					// ����S_Block
					while(1)
					{
						ucRet = PN512_s_RF_ucMifSBlock(&uiCurrRecvLen);
						if((ucRet == RET_RF_ERR_TIMEOUT)	|| (ucRet == RET_RF_ERR_TRANSMIT))
						{
					
						/*for debug 070915 for display
						ScrPrint(0,2,1,"R SBlock: 01:%02X ", result);
						getkey();
						debug end*/
							tempResult = ucRet;
							// ������Ӧ�����ͨ�Ŵ�������NAK
							ucRet = PN512_s_RF_ucMifRBlock(&uiCurrRecvLen);
					
						/*for debug 070915 for display
						ScrPrint(0,2,1,"R SBlock: 02:%02X ", result);
						getkey();
						debug en*/
							if(ucRet != RET_RF_OK)
							{
								   // ��Ȼû�н��յ���Ч��Ӧ���ݣ���֪ͨ�豸
								   return tempResult;
							}
						}
						
						if(ucRet != RET_RF_OK)
						{
							 return ucRet;
						}

						if (uiCurrRecvLen == 0)
						{ 
							    // ���ݽ���ʧ��
								return RET_RF_ERR_TRANSMIT;
						}
						
						if(uiCurrRecvLen > (PN512_RF_WorkInfo.FSD - 2))
						{
							  // ���ճ��ȳ�����FSD
							  return RET_RF_ERR_PROTOCOL;
						}
						  
						if((PN512_RF_WorkInfo.aucBuffer[0] & 0xE2) == 0x02)
						{	
							// ���յ�I_Block
							break;
						}
						else if((PN512_RF_WorkInfo.aucBuffer[0]	& 0xE2)	== 0xA2)
						{
							// ���յ�R_Block
							if((PN512_RF_WorkInfo.aucBuffer[0] & 0x10) == 0x10)
							{
								   // PICC��Ӧ����NAK
								   return	RET_RF_ERR_PROTOCOL;
							}
							
							if((PN512_RF_WorkInfo.aucBuffer[0] & 0x08) == 0x08)
							{
								   // PICC��Ӧ����CID
								   return	RET_RF_ERR_PROTOCOL;
							}

                            ulTemp = PN512_RF_WorkInfo.ucCurPCB;
							//if((PN512_RF_WorkInfo.aucBuffer[0] & 0x01) != PN512_RF_WorkInfo.ucCurPCB)
							if((PN512_RF_WorkInfo.aucBuffer[0] & 0x01) != ulTemp)
							{
								   // PICC���ʹ�����
								   return	RET_RF_ERR_PROTOCOL;
							}
							else
							{
								   if(i++ > 3)
								   {
									   // PCD�ط�һ��I_Block��������
									   return	RET_RF_ERR_PROTOCOL;
								   }
							}
							break;
						}
						else if((PN512_RF_WorkInfo.aucBuffer[0] & 0xC7) != 0xC2)
						{
							// ���յ���������Block
							return RET_RF_ERR_PROTOCOL;
						}
					}

					if((PN512_RF_WorkInfo.aucBuffer[0] & 0xE2) == 0x02)
					{	
						// ���յ�I_Block
						break;
					}
				}
				else
				{
                     // ���յ���������Block
					 return RET_RF_ERR_PROTOCOL;
				}
		   }
	       // ���յ�I_Block, ��������н���
		   i = 0; 
	}
	
	// ���մ���
	if ((*puiRecLen > 1) && (ucRet == RET_RF_OK))
	{
	  ucRet = RET_RF_OK;
	}
	else
	{
         *puiRecLen = 0;
		 if (ucRet == RET_RF_OK) 
		 {
			 ucRet = RET_RF_ERR_TRANSMIT;
		 }
	}
	
	return ucRet;
}

void PN512_s_RF_vGetDetectAInfo(uchar *SerialInfo, uchar *Other)
{
	switch(PN512_RF_WorkInfo.ucUIDLen)
	{
	    case 4:
			memcpy(SerialInfo+1, (uchar*)PN512_RF_WorkInfo.ucUIDCL1, 4);
			break;
		case 7:
			//modify by skx
			memcpy(SerialInfo+1, (uchar*)(PN512_RF_WorkInfo.ucUIDCL1+1), 3);
			memcpy(SerialInfo+4, (uchar*)(&PN512_RF_WorkInfo.ucUIDCL2), 4);
			break;
		case 10:
			memcpy(SerialInfo+1, (uchar*)(PN512_RF_WorkInfo.ucUIDCL1+1), 3);
			memcpy(SerialInfo+4, (uchar*)(&PN512_RF_WorkInfo.ucUIDCL2+1), 3);
			memcpy(SerialInfo+7, (uchar*)(&PN512_RF_WorkInfo.ucUIDCL3), 4);
			break;
	}

	//����A�Ϳ�������len+ errcode[2] +ATQA[2] + SAK1/SAK2/SAK3
	// mifare one ����ATS���ݣ�ATS����Ϊ0��
	memcpy(Other+3, (uchar*)PN512_RF_WorkInfo.ucATQA, 2);
	switch(PN512_RF_WorkInfo.ucUIDLen)
	{
	   case 4:				   	  
	   	  Other[0] = 5;
		  Other[5] = PN512_RF_WorkInfo.ucSAK1;
		  break;
	   case 7:	   	  
	   	  Other[0] = 6;
		  Other[5] = PN512_RF_WorkInfo.ucSAK1;
		  Other[6] = PN512_RF_WorkInfo.ucSAK2;
		  break;
	   case 10:	   	  
	   	  Other[0] = 7;
		  Other[5] = PN512_RF_WorkInfo.ucSAK1;
		  Other[6] = PN512_RF_WorkInfo.ucSAK2;
		  Other[7] = PN512_RF_WorkInfo.ucSAK3;
		  break;
	}	
	
	return;
}



void PN512_s_RFDelay10ETUs(ulong nETU)
{
    ulong  TmpETU = 0;
	uchar ucTemp = 0;
	ushort usTimeValue = 0;

	TmpETU = nETU;

    PN512_s_vRFSetBitMask(PN512_CONTROL_REG, 0x80); // �رն�ʱ��
    ucTemp = 0;
    // TPrescale = 0x00
    // ��ʱ��Ƶ��Ϊ6.78MHz
	PN512_s_vRFWriteReg(1,PN512_TMODE_REG, &ucTemp);
	PN512_s_vRFWriteReg(1,PN512_TPRESCALER_REG, &ucTemp);

    while(1)
    {
        // Treload = 0x500, ԼΪ20ETU
		ucTemp = 0x05;
		PN512_s_vRFWriteReg(1,PN512_TRELOAD_H_REG, &ucTemp);
		ucTemp = 0x00;
		PN512_s_vRFWriteReg(1,PN512_TRELOAD_L_REG, &ucTemp);

		PN512_s_vRFSetBitMask(PN512_CONTROL_REG, 0x40);  // ������ʱ��

		while(1)
		{
		   PN512_s_vRFReadReg(1,PN512_TCOUNTERVAL_H_REG,&ucTemp);
		   usTimeValue = ucTemp << 8;
		   PN512_s_vRFReadReg(1,PN512_TCOUNTERVAL_L_REG,&ucTemp);
		   usTimeValue |= ucTemp;

		   if(usTimeValue <= 0x280)
		   {
		        PN512_s_vRFSetBitMask(PN512_CONTROL_REG, 0x80); // �رն�ʱ��
		        break;
		   }
		}

		TmpETU--;

		if(TmpETU == 0)
		{
		     break;
		}
    }

}


void PN512_s_RFSetTimer(ulong nETU)
{
    uchar ucTemp = 0;
	uint uiTempClk = 0;

	if(nETU == RF_FWTMIN)
	{
	    // TPrescale = 0x00
        // ��ʱ��Ƶ��Ϊ6.78MHz
        ucTemp = 0;
	    PN512_s_vRFWriteReg(1,PN512_TMODE_REG, &ucTemp);
	    PN512_s_vRFWriteReg(1,PN512_TPRESCALER_REG, &ucTemp);

		// TReload = 0x26A, ԼΪ618CLk
		ucTemp = 0x02;
		PN512_s_vRFWriteReg(1,PN512_TRELOAD_H_REG, &ucTemp);
		ucTemp = 0x6A;
		PN512_s_vRFWriteReg(1,PN512_TRELOAD_L_REG, &ucTemp);
	}
	else if(nETU < 1024)
	{
	    // TPrescale = 0x00
        // ��ʱ��Ƶ��Ϊ6.78MHz
        ucTemp = 0;
	    PN512_s_vRFWriteReg(1,PN512_TMODE_REG, &ucTemp);
	    PN512_s_vRFWriteReg(1,PN512_TPRESCALER_REG, &ucTemp);

		// TReload
		uiTempClk = nETU*64 - 1;  // 64ClkΪһ��ETU
		ucTemp = uiTempClk >> 8;
		PN512_s_vRFWriteReg(1,PN512_TRELOAD_H_REG, &ucTemp);
		ucTemp = uiTempClk & 0x0FF;
		PN512_s_vRFWriteReg(1,PN512_TRELOAD_L_REG, &ucTemp);
	}
	else if(nETU < 2048)
	{
	    // TPrescale = 0x01
        // ��ʱ��Ƶ��Ϊ3.39MHz
        ucTemp = 0;
	    PN512_s_vRFWriteReg(1,PN512_TMODE_REG, &ucTemp);
		ucTemp = 0x01;
	    PN512_s_vRFWriteReg(1,PN512_TPRESCALER_REG, &ucTemp);

		// TReload
		uiTempClk = nETU*32 - 1;  // 32ClkΪһ��ETU
		ucTemp = uiTempClk >> 8;
		PN512_s_vRFWriteReg(1,PN512_TRELOAD_H_REG, &ucTemp);
		ucTemp = uiTempClk & 0x0FF;
		PN512_s_vRFWriteReg(1,PN512_TRELOAD_L_REG, &ucTemp);
	}
	else if(nETU < 4096)
	{
	    // TPrescale = 0x03
        // ��ʱ��Ƶ��Ϊ1.695MHz
        ucTemp = 0;
	    PN512_s_vRFWriteReg(1,PN512_TMODE_REG, &ucTemp);
		ucTemp = 0x03;
	    PN512_s_vRFWriteReg(1,PN512_TPRESCALER_REG, &ucTemp);

		// TReload
		uiTempClk = nETU*16 - 1;  // 16ClkΪһ��ETU
		ucTemp = uiTempClk >> 8;
		PN512_s_vRFWriteReg(1,PN512_TRELOAD_H_REG, &ucTemp);
		ucTemp = uiTempClk & 0x0FF;
		PN512_s_vRFWriteReg(1,PN512_TRELOAD_L_REG, &ucTemp);
	}
	else if(nETU < 8192)
	{
		// TPrescale = 0x07
		// ��ʱ��Ƶ��Ϊ847.5KHz
		ucTemp = 0;
		PN512_s_vRFWriteReg(1,PN512_TMODE_REG, &ucTemp);
		ucTemp = 0x07;
		PN512_s_vRFWriteReg(1,PN512_TPRESCALER_REG, &ucTemp);

		// TReload
		uiTempClk = nETU*8 - 1;  // 8ClkΪһ��ETU
		ucTemp = uiTempClk >> 8;
		PN512_s_vRFWriteReg(1,PN512_TRELOAD_H_REG, &ucTemp);
		ucTemp = uiTempClk & 0x0FF;
		PN512_s_vRFWriteReg(1,PN512_TRELOAD_L_REG, &ucTemp);
	}
	else if(nETU < 16384)
	{
		// TPrescale = 0x0F
		// ��ʱ��Ƶ��Ϊ423.75KHz
		ucTemp = 0;
		PN512_s_vRFWriteReg(1,PN512_TMODE_REG, &ucTemp);
		ucTemp = 0x0F;
		PN512_s_vRFWriteReg(1,PN512_TPRESCALER_REG, &ucTemp);

		// TReload
		uiTempClk = nETU*4 - 1;  // 4ClkΪһ��ETU
		ucTemp = uiTempClk >> 8;
		PN512_s_vRFWriteReg(1,PN512_TRELOAD_H_REG, &ucTemp);
		ucTemp = uiTempClk & 0x0FF;
		PN512_s_vRFWriteReg(1,PN512_TRELOAD_L_REG, &ucTemp);
	}
	else if(nETU < 32768)
	{
		// TPrescale = 0x1F
		// ��ʱ��Ƶ��Ϊ211.875KHz
		ucTemp = 0;
		PN512_s_vRFWriteReg(1,PN512_TMODE_REG, &ucTemp);
		ucTemp = 0x1F;
		PN512_s_vRFWriteReg(1,PN512_TPRESCALER_REG, &ucTemp);

		// TReload
		uiTempClk = nETU*2 - 1;  // 2ClkΪһ��ETU
		ucTemp = uiTempClk >> 8;
		PN512_s_vRFWriteReg(1,PN512_TRELOAD_H_REG, &ucTemp);
		ucTemp = uiTempClk & 0x0FF;
		PN512_s_vRFWriteReg(1,PN512_TRELOAD_L_REG, &ucTemp);
	}
	else if(nETU < 65536)
	{
		// TPrescale = 0x3F
		// ��ʱ��Ƶ��Ϊ105.9375KHz
		ucTemp = 0;
		PN512_s_vRFWriteReg(1,PN512_TMODE_REG, &ucTemp);
		ucTemp = 0x3F;
		PN512_s_vRFWriteReg(1,PN512_TPRESCALER_REG, &ucTemp);

		// TReload
		uiTempClk = nETU - 1;  // 1ClkΪһ��ETU
		ucTemp = uiTempClk >> 8;
		PN512_s_vRFWriteReg(1,PN512_TRELOAD_H_REG, &ucTemp);
		ucTemp = uiTempClk & 0x0FF;
		PN512_s_vRFWriteReg(1,PN512_TRELOAD_L_REG, &ucTemp);
	}
	else if(nETU < 131072)
	{
	    // TPrescale = 0x7F
		// ��ʱ��Ƶ��Ϊ52.96875KHz
		ucTemp = 0;
		PN512_s_vRFWriteReg(1,PN512_TMODE_REG, &ucTemp);
		ucTemp = 0x7F;
		PN512_s_vRFWriteReg(1,PN512_TPRESCALER_REG, &ucTemp);

		// TReload
		uiTempClk = nETU/2 - 1;  // 1ClkΪ2��ETU
		ucTemp = uiTempClk >> 8;
		PN512_s_vRFWriteReg(1,PN512_TRELOAD_H_REG, &ucTemp);
		ucTemp = uiTempClk & 0x0FF;
		PN512_s_vRFWriteReg(1,PN512_TRELOAD_L_REG, &ucTemp);
	}
	else if(nETU < 262144)
	{
	    // TPrescale = 0xFF
		// ��ʱ��Ƶ��Ϊ26.484375KHz
		ucTemp = 0;
		PN512_s_vRFWriteReg(1,PN512_TMODE_REG, &ucTemp);
		ucTemp = 0xFF;
		PN512_s_vRFWriteReg(1,PN512_TPRESCALER_REG, &ucTemp);

		// TReload
		uiTempClk = nETU/4 - 1;  // 1ClkΪ4��ETU
		ucTemp = uiTempClk >> 8;
		PN512_s_vRFWriteReg(1,PN512_TRELOAD_H_REG, &ucTemp);
		ucTemp = uiTempClk & 0x0FF;
		PN512_s_vRFWriteReg(1,PN512_TRELOAD_L_REG, &ucTemp);
	}
	else if(nETU < 524288)
	{
	    // TPrescale = 0x1FF
		// ��ʱ��Ƶ��Ϊ13.2421875KHz
		ucTemp = 0x01;
		PN512_s_vRFWriteReg(1,PN512_TMODE_REG, &ucTemp);
		ucTemp = 0xFF;
		PN512_s_vRFWriteReg(1,PN512_TPRESCALER_REG, &ucTemp);

		// TReload
		uiTempClk = nETU/8 - 1;  // 1ClkΪ8��ETU
		ucTemp = uiTempClk >> 8;
		PN512_s_vRFWriteReg(1,PN512_TRELOAD_H_REG, &ucTemp);
		ucTemp = uiTempClk & 0x0FF;
		PN512_s_vRFWriteReg(1,PN512_TRELOAD_L_REG, &ucTemp);
	}
	else if(nETU < 1048576)
	{
	    // TPrescale = 0x3FF
		// ��ʱ��Ƶ��Ϊ6.62KHz
		ucTemp = 0x03;
		PN512_s_vRFWriteReg(1,PN512_TMODE_REG, &ucTemp);
		ucTemp = 0xFF;
		PN512_s_vRFWriteReg(1,PN512_TPRESCALER_REG, &ucTemp);

		// TReload
		uiTempClk = nETU/16 - 1;  // 1ClkΪ16��ETU
		ucTemp = uiTempClk >> 8;
		PN512_s_vRFWriteReg(1,PN512_TRELOAD_H_REG, &ucTemp);
		ucTemp = uiTempClk & 0x0FF;
		PN512_s_vRFWriteReg(1,PN512_TRELOAD_L_REG, &ucTemp);
	}
	else if(nETU < 2095152)
	{
	    // TPrescale = 0x7FF
		// ��ʱ��Ƶ��Ϊ3.31KHz
		ucTemp = 0x07;
		PN512_s_vRFWriteReg(1,PN512_TMODE_REG, &ucTemp);
		ucTemp = 0xFF;
		PN512_s_vRFWriteReg(1,PN512_TPRESCALER_REG, &ucTemp);

		// TReload
		uiTempClk = nETU/32 - 1;  // 1ClkΪ32��ETU
		ucTemp = uiTempClk >> 8;
		PN512_s_vRFWriteReg(1,PN512_TRELOAD_H_REG, &ucTemp);
		ucTemp = uiTempClk & 0x0FF;
		PN512_s_vRFWriteReg(1,PN512_TRELOAD_L_REG, &ucTemp);
	}
	else
	{
	    // TPrescale = 0xFFF
		// ��ʱ��Ƶ��Ϊ1.655KHz
		ucTemp = 0x0F;
		PN512_s_vRFWriteReg(1,PN512_TMODE_REG, &ucTemp);
		ucTemp = 0xFF;
		PN512_s_vRFWriteReg(1,PN512_TPRESCALER_REG, &ucTemp);

		// TReload
		uiTempClk = nETU/64 - 1;  // 1ClkΪ64��ETU
		ucTemp = uiTempClk >> 8;
		PN512_s_vRFWriteReg(1,PN512_TRELOAD_H_REG, &ucTemp);
		ucTemp = uiTempClk & 0x0FF;
		PN512_s_vRFWriteReg(1,PN512_TRELOAD_L_REG, &ucTemp);
	}


}


void PN512_s_RFOpenCarrier(void)
{	
	uchar ucTemp = 0;
	ucTemp = 0xA3;
	PN512_s_vRFWriteReg(1,PN512_TXCONTROL_REG,&ucTemp); 
}


void PN512_s_RFCloseCarrier(void)
{
	uchar ucTemp = 0;
	ucTemp = 0x30;
	PN512_s_vRFWriteReg(1,PN512_TXCONTROL_REG,&ucTemp);
    PN512_s_RFDelay10ETUs(RF_TRESET/10);
}


void PN512_s_RFResetCarrier(void)
{	uchar ucTemp = 0;
	ucTemp = 0x30; 
	PN512_s_vRFWriteReg(1,PN512_TXCONTROL_REG,&ucTemp); 
    PN512_s_RFDelay10ETUs(RF_TRESET/10);
	ucTemp = 0xA3;
	PN512_s_vRFWriteReg(1,PN512_TXCONTROL_REG,&ucTemp);
}




#ifdef PN512_Debug_Isr
uint gl_LoAlert=0,gl_TxIRQ=0,gl_RxIRQ=0,gl_HiAlert_Idle=0,gl_HiAlert=0,gl_Idle=0,gl_timer=0;
uchar gl_err_flg1=0,gl_err_flg2=0,gl_err_flg3=0,gl_err_flg4=0,gl_loalertNum=0;
uchar gl_err_flg5=0,gl_err_flg6=0,gl_err_flg5_1=0,gl_err_flg6_1=0,gl_err_flg5_2=0,gl_err_flg6_2=0,gl_err_flg5_3=0,gl_err_flg6_3=0;
uchar isr_test_arr[20];
extern volatile unsigned char rf_get_value;
#endif

#ifdef FOR_LINUX_SYS
DECLARE_WAIT_QUEUE_HEAD(pn512_rf_wq);//����һ���ȴ�����ͷ
irqreturn_t PN512_s_RFIsr(int irq, void *dev_id, struct pt_regs *regs)
#else
void PN512_s_RFIsr(void)
#endif
//
{
	uchar ucTempData;
	uchar ucIrqPending;
	ulong ulTemp;
	ulong i = 0;//for debug 
	//AT skx
	//uint for_spi_test=0;
    ulong ulTemp1 = 0;

	PN512_sHal_MaskCpuInt();//��ֹ����CPU�ж�
	
	PN512_s_vRFSwitchNCS(1);//Ƭѡ����һ��
	WaitNuS(100);
	//while( Get_Spi1_Status()&0x01 )  Get_Spi1_Data();
	
	// ��ȡ����Ĵ���
	PN512_s_vRFReadReg(1,PN512_ERROR_REG,(uchar*)&PN512_RF_WorkInfo.ucErrFlags);
	
	PN512_RF_WorkInfo.ucErrFlags &= 0x1f;
 
	// ����ǳ�ͻ����, ��Ҫ��ȡ��ͻλ��
	if(((PN512_RF_WorkInfo.ucAnticol == PICC_ANTICOLL1) && ((PN512_RF_WorkInfo.ucErrFlags & 0x08) != 0))
	   || ((PN512_RF_WorkInfo.ucAnticol ==PICC_ANTICOLL2) && ((PN512_RF_WorkInfo.ucErrFlags & 0x08) != 0))
	   || ((PN512_RF_WorkInfo.ucAnticol == PICC_ANTICOLL3) && ((PN512_RF_WorkInfo.ucErrFlags & 0x08) != 0)))
	{
		PN512_s_vRFReadReg(1, PN512_COLL_REG, (uchar*)&PN512_RF_WorkInfo.ucCollPos); 
	}
	
	if((PN512_RF_WorkInfo.ucErrFlags & 0x08) == 0x08)
	{
		// �ڲ�����ͻ��������£�������żУ�����
		PN512_RF_WorkInfo.ucErrFlags &= 0xfd;
	}

	#ifdef PN512_Debug_Isr
		
	 		if(gl_loalertNum==0)
	 				gl_err_flg1=PN512_RF_WorkInfo.ucErrFlags;
	 		if(gl_loalertNum==1)
	 				gl_err_flg2=PN512_RF_WorkInfo.ucErrFlags;
	 		if(gl_loalertNum==2)
	 				gl_err_flg3=PN512_RF_WorkInfo.ucErrFlags;
	 		if(gl_loalertNum==3)
	 				gl_err_flg4=PN512_RF_WorkInfo.ucErrFlags;

	 		gl_loalertNum++;
	#endif	
	
	
	// �������У�ֱ�������������ж�����
    while (1)
    {
        PN512_s_vRFReadReg(1,PN512_STATUS1_REG,&ucTempData);
				
        if ((ucTempData & 0x10) == 0)
        {
        	#ifdef PN512_Debug_Isr
		debug_printf(0,0,0,"isr in but not isr happen !,stat:%02x",ucTempData);
		#endif
            // û��δ�����ж�
            break;
        }
				// ��ȡ�ж�����λ�����ε�����Ҫ���жϣ�������
        PN512_s_vRFReadReg(1,PN512_COMMIEN_REG,&ucTempData);
        PN512_s_vRFReadReg(1,PN512_COMMIRQ_REG,&ucIrqPending);

			
        ucIrqPending &= ucTempData;
        PN512_RF_WorkInfo.ucINTSource |= ucIrqPending;

        // ���δ�������ж�����
        if((ucIrqPending & 0x04) != 0)
        {
					#ifdef PN512_Debug_Isr
					gl_LoAlert++;
					#endif
            // �鿴������ʣ�೤��
            PN512_s_vRFReadReg(1, PN512_FIFOLEVEL_REG, &ucTempData);
            ulTemp = 64 - ucTempData;
			
            // ��������Ҫ�������ݳ���
            ulTemp1 = PN512_RF_WorkInfo.ulBytesSent;
            //if( (PN512_RF_WorkInfo.ulSendBytes - PN512_RF_WorkInfo.ulBytesSent) <= ulTemp)
            if( (PN512_RF_WorkInfo.ulSendBytes - ulTemp1) <= ulTemp)
            {
                // �����Ҫ���ͳ���С�ڻ�����ʣ��ռ䣬��ȫ������
                ulTemp1 = PN512_RF_WorkInfo.ulBytesSent;
                //ulTemp = PN512_RF_WorkInfo.ulSendBytes - PN512_RF_WorkInfo.ulBytesSent;
                ulTemp = PN512_RF_WorkInfo.ulSendBytes - ulTemp1;
                ucTempData = 0x04;
			
                // �������, ��ֹ�ж�LoAlert
                PN512_s_vRFClrBitMask(PN512_COMMIEN_REG, ucTempData);				
            }
					
            // ��������         
			for(i = 0; i<ulTemp; i++) 
			{
				PN512_s_vRFWriteReg(1,PN512_FIFODATA_REG,(uchar*)(PN512_RF_WorkInfo.aucBuffer+PN512_RF_WorkInfo.ulBytesSent));
				PN512_RF_WorkInfo.ulBytesSent++;
			}

			PN512_s_vRFClrBitMask(PN512_BITFRAMING_REG, 0x80);
			//������������ж�������������ֹ�ظ����������PN512״̬�����ҽ��ճ�ʱ modify by  skx 2009-05-04
			if(PN512_RF_WorkInfo.ucCmd==PN512_MFAuthent_CMD)//��PN512_MFAuthent_CMD����������ж�������
			PN512_s_vRFWriteReg(1,PN512_COMMAND_REG,(uchar*)(&PN512_RF_WorkInfo.ucCmd));
					
			if(PN512_RF_WorkInfo.ucCmd == PN512_TRANSCEIVE_CMD)
			{
				// ����StartSend Bit 
				//��TRANSCEIVE������һ����Ҫ��������λ
				//��TRANS�����²���Ҫ��������λҲ���Զ�����
				PN512_s_vRFSetBitMask(PN512_BITFRAMING_REG, 0x80);
			}
			
            // ����ж�����
            ucTempData = 0x04;
            PN512_s_vRFWriteReg(1, PN512_COMMIRQ_REG, &ucTempData);		
        }

        if((ucIrqPending & 0x40 ) != 0)
        {
			#ifdef PN512_Debug_Isr
			gl_TxIRQ++;
			#endif
            // TxIRQ
					// ����ж�����
            ucTempData = 0x40;
            PN512_s_vRFWriteReg(1,PN512_COMMIRQ_REG,&ucTempData);

            // ����HiAlert�ж�����׼��������Ӧ
            ucTempData = 0x08;
            PN512_s_vRFSetBitMask(PN512_COMMIEN_REG,ucTempData);
			
        }

        if((ucIrqPending & 0x20) != 0)
        {
			#ifdef PN512_Debug_Isr
			gl_RxIRQ++;
			#endif
            // RxIRQ
						// û�з�������, �����ֹͣ����
           if (PN512_RF_WorkInfo.ucErrFlags == 0 || PN512_RF_WorkInfo.ucM1Stage == 1)//20080422
            {
				ucTempData = 0;
				PN512_s_vRFWriteReg(1,PN512_COMMAND_REG,&ucTempData);
				//���ÿ����ж������Ա����������
				ucIrqPending |= 0x10;
            }
           else
            {
            	// �д������
                // �������
                PN512_RF_WorkInfo.ucSaveErrState = PN512_RF_WorkInfo.ucErrFlags;
                // ���FIFO
                PN512_s_vRFFlushFIFO();

                // û�н��յ�����
                PN512_RF_WorkInfo.ulBytesReceived = 0x00;
                // ���治�ٴ��� RxIRQ����
                ucIrqPending &= ~0x20;
                // ����ж�����
                ucTempData = 0x20;
                PN512_s_vRFWriteReg(1,PN512_COMMIRQ_REG,&ucTempData);
            }
        }

        if((ucIrqPending & 0x38) != 0)
        {
			#ifdef PN512_Debug_Isr
			gl_HiAlert_Idle++;
			#endif
    		// HiAlert, Idle or valid RxIRQ
			if((ucIrqPending & 0x28)!=0)
			{		 
				// ��ȡ����������
				PN512_s_vRFReadReg(1,PN512_FIFOLEVEL_REG,&ucTempData);
				
				if(ucTempData > 0 && PN512_RF_WorkInfo.ucCurResult == RET_RF_OK)
				{
					for(i = 0; i<ucTempData; i++)
					{				
						PN512_s_vRFReadReg(1,PN512_FIFODATA_REG,
					  (uchar*)(PN512_RF_WorkInfo.aucBuffer+PN512_RF_WorkInfo.ulBytesReceived));
					  PN512_RF_WorkInfo.ulBytesReceived++;				
					}
				}
				
				//  ���RxIRQ��HiAlert����
				ucTempData = 0x28 & ucIrqPending;
				PN512_s_vRFWriteReg(1,PN512_COMMIRQ_REG,&ucTempData);
			}
        }

        if((ucIrqPending & 0x08) != 0)
        {
			#ifdef PN512_Debug_Isr
			gl_HiAlert++;
			#endif
          // HiAlertIRQ
          PN512_s_vRFReadReg(1,PN512_STATUS2_REG,&ucTempData);
          if((ucTempData & 0x07) == 0x06) // ��������Ȼ��
          {
              //ֹͣ��ʱ��
              PN512_s_vRFSetBitMask(PN512_CONTROL_REG, 0x80); // �رն�ʱ��		
          }
        }
        // ���մ�������ֹ����
		if(PN512_RF_WorkInfo.ucSaveErrState != 0 && PN512_RF_WorkInfo.ucM1Stage == 0)//20080422
		{
			ucTempData = 0;
      		PN512_s_vRFWriteReg(1,PN512_COMMAND_REG,&ucTempData);
      		//���ÿ����ж������Ա����������
			ucIrqPending |= 0x10;
		}//

		if((ucIrqPending & 0x10) != 0)
		{
			#ifdef PN512_Debug_Isr
			gl_Idle++;
			#endif
		    // Idle IRQ
		    ucTempData = 0x01;
		    // ��ֹ��ʱ���ж�
		    PN512_s_vRFClrBitMask(PN512_COMMIEN_REG,ucTempData);
		
		    ucTempData = 0x11;
		    // �����ʱ���ж�����Ϳ����ж�����
		    PN512_s_vRFWriteReg(1,PN512_COMMIRQ_REG,&ucTempData);
		
		    ucIrqPending &= ~0x01;   // ���治����ʱ���ж�
		    // �������ʱ���ж�, ����IDLE�ж�
		    PN512_RF_WorkInfo.ucINTSource = (PN512_RF_WorkInfo.ucINTSource & ~0x01)|0x10;	
		    #ifdef FOR_LINUX_SYS
                    wake_up_interruptible(&pn512_rf_wq);//���������ȴ��˳�����
                    #endif		    
		}

    if((ucIrqPending & 0x01) != 0)
    {
		#ifdef PN512_Debug_Isr
		gl_timer++;
		#endif
      // timer IRQ
			// �����ʱ���ж�����
		ucTempData = 0x01;
		PN512_s_vRFWriteReg(1,PN512_COMMIRQ_REG,&ucTempData);

      // ��ʱ����
      if(PN512_RF_WorkInfo.ucCurResult == RET_RF_OK)
      {
		#ifdef PN512_Debug_Isr
			#ifdef FOR_LINUX_SYS
			printk("/*****Isr TimerOut!******/\n");
			#endif
		#endif
		PN512_RF_WorkInfo.ucCurResult = RET_RF_ERR_TIMEOUT;
      }
      #ifdef FOR_LINUX_SYS
      wake_up_interruptible(&pn512_rf_wq);//���������ȴ��˳�����
      #endif		    
        // otherwise ignore the interrupt
 	}
			
  }// while (1)
 
	PN512_sHal_EnCpuInt();//ʹ��CPU�ж�
    
        #ifdef FOR_LINUX_SYS
	return IRQ_HANDLED;
        #else
        return ;
        #endif
}

extern void ledall_off(void);
extern void ledall_on(void);
extern void ledred_on(void);
extern void ledred_off(void);
extern void ledgreen_on(void);
extern void ledgreen_off(void);

uchar PN512_s_RFExchangeCmd(uchar ucCmd)
{
	ulong ulTemp = 0;
	uchar ucTempData;		 // �ݴ�Ҫ�����ļĴ�������
	uchar ucWaterLevelBak;	 // ����FIFO��WaterLevelֵ
	uchar ucTimerCtl;		 // �������õ��Ķ�ʱ�����Ʒ�ʽ
	uchar ucIntWait;		 // �������ʱ�ڴ��������ж�
	uchar ucIntEnable;		 // ���������������ж�
	//uchar ucrxMultiple; 	 // �Ƿ�Ҫ���ն��֡
	
	//ulong timeout=0;//for linux

	//AT skx
	//uint intreg[10];

	// ��������������ʼֵ
	PN512_RF_WorkInfo.ucCurResult		=	RET_RF_OK;
	PN512_RF_WorkInfo.ucINTSource		=	0;
	PN512_RF_WorkInfo.ulBytesSent		=	0;		// how many bytes already sent
	PN512_RF_WorkInfo.ulBytesReceived	=	0;		// how many bytes received
	PN512_RF_WorkInfo.lBitsReceived 	=	0;		// how many bits received
	PN512_RF_WorkInfo.ucErrFlags		=	0;
	PN512_RF_WorkInfo.ucSaveErrState	=	0x00;
	PN512_RF_WorkInfo.ucCollPos     =   0;
	PN512_RF_WorkInfo.ucCmd = PN512_IDLE_CMD;

	//ucrxMultiple		   = 0x00;
	ucIntEnable 		   = 0x00;
	ucIntWait			   = 0x00;
	ucTimerCtl			   = 0x00;

	#ifdef PN512_Debug_Isr
	gl_loalertNum=0;
	gl_LoAlert=gl_TxIRQ=gl_RxIRQ=gl_HiAlert_Idle=gl_HiAlert=gl_Idle=gl_timer=0;
	gl_err_flg1=gl_err_flg2=gl_err_flg3=gl_err_flg4=0;
	gl_err_flg5=gl_err_flg6=gl_err_flg5_1=gl_err_flg6_1=gl_err_flg5_2=gl_err_flg6_2=gl_err_flg5_3=gl_err_flg6_3=0;
	#endif
	
	// ��ֹ����������ж�Դ
	ucTempData = 0x7F;
	PN512_s_vRFClrBitMask(PN512_COMMIEN_REG,ucTempData);
	
	PN512_s_vRFWriteReg(1,PN512_COMMIRQ_REG,&ucTempData);
	
	ucTempData = 0x1F;
	PN512_s_vRFClrBitMask(PN512_DIVIEN_REG,ucTempData);	
	PN512_s_vRFWriteReg(1,PN512_DIVIRQ_REG,&ucTempData);
	
   	// ������ǰ�������е�����
	ucTempData = PN512_IDLE_CMD;
	PN512_s_vRFWriteReg(1,PN512_COMMAND_REG,&ucTempData); 

        // ���ö�ʱ�����Ʒ�ʽ
        PN512_s_vRFSetBitMask(PN512_CONTROL_REG,0x80);// �رն�ʱ��		
	// ���FIFO������
	PN512_s_vRFFlushFIFO();

	// ����FIFO����ֵ
	PN512_s_vRFReadReg(1,PN512_WATERLEVEL_REG,&ucWaterLevelBak);
	ucTempData = 0x10;
	PN512_s_vRFWriteReg(1,PN512_WATERLEVEL_REG,&ucTempData);
		
	// ���ݲ�ͬ������д���
	switch(ucCmd)
	{
	    case PN512_SOFTRESET_CMD: // for debug
		{
			ucTimerCtl	= 0x00; //�ֶ���ʼ, ���ݽ������ʱ��ʱ���Զ�ֹͣ
			ucIntEnable = 0x18; // HiAlert and IdleIRq
			ucIntWait	= 0x10; // IdleIRq
			break;
		}
		
		case PN512_IDLE_CMD:
		{
			// ����
			ucTimerCtl	= 0x00;  //��ʹ�ö�ʱ��
			ucIntEnable = 0x00;  //�������ж�
			ucIntWait	= 0x00;  //���ȴ��ж�
			break;
		}

		case PN512_CONFIG_CMD:
		{
			// ��������
			ucTimerCtl	= 0x00; // �ֶ�������ֹͣ��ʱ��
			ucIntEnable = 0x14; // IdleIRq and LoAlert
			ucIntWait	= 0x10; // IdleIRq
			break;
		}
		case PN512_GRAMID_CMD:
		{
			ucTimerCtl	= 0x80;
			ucIntEnable = 0x10; // IdleIRq and LoAlert
			ucIntWait	= 0x10; // IdleIRq
			break;
		}
		case PN512_CALCRC_CMD:
		{
			//CRCУ��
			ucTimerCtl	= 0x00; // �ֶ�������ֹͣ��ʱ��
			ucIntEnable = 0x11; // LoAlert and TxIRq
			ucIntWait	= 0x10; // TxIRq
			break;
		}
		case PN512_RECEIVE_CMD:
		{
			//��������
			//���ݽ�����ʼλ��λ��������ʵ���յ�λ��
			PN512_s_vRFReadReg(1,PN512_BITFRAMING_REG,&ucTempData);
			PN512_RF_WorkInfo.lBitsReceived = -(long)(ucTempData >> 4);

			ucTimerCtl	= 0x00; //�ֶ���ʼ, ���ݽ������ʱ��ʱ���Զ�ֹͣ
			ucIntEnable = 0x18; // HiAlert and IdleIRq
			ucIntWait	= 0x10; // IdleIRq
			break;
		}
		case PN512_MFAuthent_CMD:
		{
			ucTimerCtl	= 0x80;
			ucIntEnable = 0x14;// changed by liuxl 20071228
			ucIntWait	= 0x10; // IdleIRq
			break;
		}
		case PN512_TRANSMIT_CMD:
		{
			//��������
			ucTimerCtl	= 0x00; // �ֶ�������ֹͣ��ʱ��
			ucIntEnable = 0x14; // LoAlert and IdleIRq
			ucIntWait	= 0x10; // IdleIRq
			break;
		}
		case PN512_TRANSCEIVE_CMD:
		{	
			// ��������
			// ���ݽ�����ʼλ��λ��������ʵ���յ�λ��
			PN512_s_vRFReadReg(1,PN512_BITFRAMING_REG,&ucTempData);
			PN512_RF_WorkInfo.lBitsReceived = -(long)(ucTempData >> 4);
    		ucTimerCtl	= 0x80; // ���ݷ������ʱ��ʱ���Զ���ʼ
			ucIntEnable = 0x74; // TxIrq, RxIrq, IdleIRq and LoAlert
			ucIntWait	= 0x10; // IdleIRq			
			break;
		}
		default:
		{
			// �Ƿ�����
			PN512_RF_WorkInfo.ucCurResult = RET_RF_ERR_PARAM;
			break;
		}
	}

	// �ղ�û�д���
	if (PN512_RF_WorkInfo.ucCurResult == RET_RF_OK)
	{
		
		//AT
		//if(PN512_RF_WorkInfo.f_ispoll)//�����felica��polling��������ö�֡����ģʽ
		{
			//PN512_s_vRFSetBitMask(PN512_RXMODE_REG,0X04);//���ö�֡ģʽ
		}
		//else
		{
			PN512_s_vRFClrBitMask(PN512_RXMODE_REG,0X04);//�ָ�Ϊ��֡
		}

	    // ���ö�ʱ�����Ʒ�ʽ
        PN512_s_vRFSetBitMask(PN512_TMODE_REG,ucTimerCtl);
		
        // �����ʱ��Ҫ�˹���������������
        if ((~ucTimerCtl & 0x80) != 0 )
        {
			PN512_s_vRFSetBitMask(PN512_CONTROL_REG,0x40);// ������ʱ��			
        }
      
		PN512_RF_WorkInfo.ucCmd = ucCmd;

		//���������εĴ���ת�Ƶ��������ɺ󣬱�����Ϊ�����жϻ������ȼ��ߵ�����������Ͳ���û��ִ�ж�ʹ����ʧ��		
		// ��������ʱ����ʱ�ж�,���ҵȴ������ж�
		ucIntEnable |= 0x01;
		ucIntWait	|= 0x01;
 
		
		//���������PN512_MFAuthent_CMD������FIFO����������M1����֤ʧ�ܣ���PN512_MFAuthent_CMD�����ж�����дFIFO������
		if(PN512_RF_WorkInfo.ucCmd!=PN512_MFAuthent_CMD)
		PN512_s_vRFWriteReg(1,PN512_COMMAND_REG,(uchar*)(&PN512_RF_WorkInfo.ucCmd));//�������� add by skx 2009-05-04
		
		//���ȿ�CPU�жϣ��ٿ�PN521�ж�
		//PN512_sHal_EnCpuInt();//ʹ��CPU�ж�
                
                //AT skx
		//EXTI_DisableITBit(BIT8);//mask BIT8 first
		EXTI_DisableITBit(BIT1);//mask BIT1 first
                
		//EXTI_ReadAllReg(intreg);
                //debug_printf(0,0,0,"first in_1,%08X/%08x/%08x/%08x/%08x",
		//			intreg[0],intreg[1],intreg[2],intreg[3],intreg[4],intreg[5]);
                //PN512_s_vRFReadReg(1,PN512_COMMIEN_REG,&isr_test_arr[0]);
                //debug_printf(0,0,0,"first in,IEN:%02X/%02x",ucIntEnable,isr_test_arr[0]);
		PN512_s_vRFSetBitMask(PN512_COMMIEN_REG,ucIntEnable);//��PN512�жϣ������жϵ������һ��CSû�б����ߣ������жϽ�������һ��CS���ߴ���
		//EXTI_EnableEventBit(BIT8);//do not need enable eventbit
		PN512_sHal_EnCpuInt();//ʹ��CPU�ж�
		//EXTI_GenerateSWInterrupt(BIT8);
		EXTI_GenerateSWInterrupt(BIT1);
		
		// �ȴ��������ж��������������Ϳ��Խ�������
		// �������10��û�д����򷵻ش���
		
    	//timeout= HZ*10;//10S��ʱ
    	//for debug
    	#ifdef FOR_LINUX_SYS
	    	timeout= HZ;//1S��ʱ
	    	wait_event_interruptible_timeout(pn512_rf_wq, (PN512_RF_WorkInfo.ucINTSource & ucIntWait), timeout);//ʹ�������˯��,ֱ����ʱ������������
		#else

                    unsigned int uiBeginTime = GetTimerCount();
			while(1)
			{			
				if((PN512_RF_WorkInfo.ucINTSource & ucIntWait) != 0)
				{		
					break;
				}
                		if(( GetTimerCount()-uiBeginTime )>(100*5))
			        {
				    break;
			        }
				//AT skx
                                
                                /*
				#ifdef PN512_Debug_Isr
				isr_test_arr[10]=0x35;
				PN512_s_vRFWriteReg(1,0x0b,&isr_test_arr[10]);

				PN512_s_vRFReadReg(1,PN512_COMMIEN_REG,&isr_test_arr[1]);
				debug_printf(0,0,0,"*****COMMIEN:%02x***",isr_test_arr[1]);
				
				PN512_s_vRFReadReg(1,0x03,&isr_test_arr[0]);
				debug_printf(0,0,0,"isr out_0,Div:%02x",isr_test_arr[0]);
				
		        	PN512_s_vRFReadReg(1,PN512_COMMIRQ_REG,&isr_test_arr[2]);
		        	debug_printf(0,0,0,"INTEN:%02x",isr_test_arr[2]);
					
					PN512_s_vRFReadReg(1,0x06,&isr_test_arr[3]);
		        	debug_printf(0,0,0,"Err:%02x",isr_test_arr[3]);
					PN512_s_vRFReadReg(1,0X07,&isr_test_arr[4]);
		        	debug_printf(0,0,0,"Stat1:%02x",isr_test_arr[4]);
					PN512_s_vRFReadReg(1,0x08,&isr_test_arr[5]);
		        	debug_printf(0,0,0,"Stat2:%02x",isr_test_arr[5]);
					PN512_s_vRFReadReg(1,0x09,&isr_test_arr[6]);
		        	debug_printf(0,0,0,"FIFOD:%02x",isr_test_arr[6]);
					PN512_s_vRFReadReg(1,0X0A,&isr_test_arr[7]);
		        	debug_printf(0,0,0,"FIFOLen:%02x",isr_test_arr[7]);
					PN512_s_vRFReadReg(1,0x0b,&isr_test_arr[8]);
		        	debug_printf(0,0,0,"FIFO W:%02x",isr_test_arr[8]);
				#endif
				//delay_ms(10);
				//
				if((GetTimerCount() - uiBeginTime) >= 1*1000*10)	// 10s
				{
				  	PN512_RF_WorkInfo.usErrNo = 0xA0; // timeout err
					PN512_RF_WorkInfo.ucCurResult = RET_RF_ERR_TIMEOUT;
					ScrPrint(0,1,0,"cpu not int");
					getkey();
					break;
				}
				*/
			}
		#endif
		 
		PN512_sHal_MaskCpuInt();//��ֹ����CPU�ж�
		

		#ifdef PN512_Debug_Isr
			#ifdef FOR_LINUX_SYS
			{
				printk(KERN_INFO"int:%d/%d/%d/%d/%d/%d/%d\n",gl_LoAlert,gl_TxIRQ,gl_RxIRQ,gl_HiAlert_Idle,gl_HiAlert,gl_Idle,gl_timer);
				printk(KERN_INFO"errFlg:%02x/%02x/%02x/%02x;%d\n",gl_err_flg1,gl_err_flg2,gl_err_flg3,gl_err_flg4,gl_loalertNum);
				printk(KERN_INFO"int:%02x,%02x/%02x,%02x/%02x,%02x/%02x,%02x\n",
					gl_err_flg5,gl_err_flg6,gl_err_flg5_1,gl_err_flg6_1,gl_err_flg5_2,gl_err_flg6_2,gl_err_flg5_3,gl_err_flg6_3);
				getkey();
			}//end
			#else
			{
				debug_printf(0,0,0,"int:%d/%d/%d/%d/%d/%d/%d\n",gl_LoAlert,gl_TxIRQ,gl_RxIRQ,gl_HiAlert_Idle,gl_HiAlert,gl_Idle,gl_timer);
				debug_printf(0,0,0,"errFlg:%02x/%02x/%02x/%02x;%d\n",gl_err_flg1,gl_err_flg2,gl_err_flg3,gl_err_flg4,gl_loalertNum);
				debug_printf(0,0,0,"int:%02x,%02x/%02x,%02x/%02x,%02x/%02x,%02x\n",
					gl_err_flg5,gl_err_flg6,gl_err_flg5_1,gl_err_flg6_1,gl_err_flg5_2,gl_err_flg6_2,gl_err_flg5_3,gl_err_flg6_3);
				//getkey();
			}
			#endif
		#endif
		
		// �����������
		PN512_s_vRFSetBitMask(PN512_CONTROL_REG, 0x80); // �رն�ʱ��
		ucTempData = 0x7F;
		PN512_s_vRFClrBitMask(PN512_COMMIEN_REG,ucTempData); // ��ֹ�����ж�
		PN512_s_vRFWriteReg(1,PN512_COMMIRQ_REG,&ucTempData); // ��������ж�����λ

		// ���ص�IDLEģʽ
		ucTempData = PN512_IDLE_CMD;
		PN512_s_vRFWriteReg(1, PN512_COMMAND_REG,&ucTempData); 

		//�ռ������Ĵ���
		ulTemp = PN512_RF_WorkInfo.ucSaveErrState;
		//PN512_RF_WorkInfo.ucErrFlags |= PN512_RF_WorkInfo.ucSaveErrState;
        PN512_RF_WorkInfo.ucErrFlags |= ulTemp;

		// ���ش���
		if(PN512_RF_WorkInfo.ucErrFlags != 0)
		{
			if((PN512_RF_WorkInfo.ucErrFlags & 0x08) != 0)
			{
				// ��ͻ				
     	  		PN512_RF_WorkInfo.usErrNo = 0x03; // collision err
				PN512_RF_WorkInfo.ucCurResult = RET_RF_ERR_MULTI_CARD;

			}
			else if((PN512_RF_WorkInfo.ucErrFlags & 0x02) != 0)
			{
				// ��żУ���
				PN512_RF_WorkInfo.usErrNo = 0x04; // Parity err
				PN512_RF_WorkInfo.ucCurResult = RET_RF_ERR_TRANSMIT;
			}
			else if((PN512_RF_WorkInfo.ucErrFlags & 0x01) != 0)
			{
				// ֡����
				PN512_RF_WorkInfo.usErrNo = 0x05; // framing err
				PN512_RF_WorkInfo.ucCurResult = RET_RF_ERR_TRANSMIT;
			}
			else if((PN512_RF_WorkInfo.ucErrFlags & 0x10) != 0)
			{
				// FIFO���
				PN512_s_vRFFlushFIFO();
				PN512_RF_WorkInfo.usErrNo = 0x06; // FIFO Over err
				PN512_RF_WorkInfo.ucCurResult = RET_RF_ERR_TRANSMIT;
			}
			else if((PN512_RF_WorkInfo.ucErrFlags & 0x04) != 0)
			{
				// CRC����
				PN512_RF_WorkInfo.usErrNo = 0x07; // CRC err 
				PN512_RF_WorkInfo.ucCurResult = RET_RF_ERR_TRANSMIT;
			}
			else if(PN512_RF_WorkInfo.ucCurResult == RET_RF_OK) //?
			{
				PN512_RF_WorkInfo.usErrNo = 0x08; // other err 
				PN512_RF_WorkInfo.ucCurResult = RET_RF_ERR_TRANSMIT;
			}
		}

		//�Ȼ�ȡ���ݳ�����ִ�д����� 
		// ��������ǽ��ջ����Ƿ��ͽ��գ������¼���յ��ֽ���������
		if((ucCmd == PN512_TRANSCEIVE_CMD) || (ucCmd== PN512_RECEIVE_CMD))
		{
			// ���յ����һ���ֽ�λ��
			PN512_s_vRFReadReg (1, PN512_CONTROL_REG, &ucTempData);
			ucTempData &= 0x07;
			if (ucTempData != 0)
			{
				// ���һ���ֽ�û�н�����
				ulTemp = PN512_RF_WorkInfo.ulBytesReceived;
				//PN512_RF_WorkInfo.lBitsReceived += (PN512_RF_WorkInfo.ulBytesReceived-1) * 8 + ucTempData;
				PN512_RF_WorkInfo.lBitsReceived += (ulTemp-1) * 8 + ucTempData;
			}
			else
			{
				// ���һ���ֽڽ�����
				ulTemp = PN512_RF_WorkInfo.ulBytesReceived;
				//PN512_RF_WorkInfo.lBitsReceived += PN512_RF_WorkInfo.ulBytesReceived * 8;
				PN512_RF_WorkInfo.lBitsReceived += ulTemp * 8;
			}
		}
	}

	PN512_s_vRFWriteReg(1,PN512_WATERLEVEL_REG,&ucWaterLevelBak);
		
	return PN512_RF_WorkInfo.ucCurResult;

}



void PN512_s_vRFSetBitMask (uchar ucRegAddr, uchar ucBitMask)
{
    uchar ucTemp;

    PN512_s_vRFReadReg(1, ucRegAddr, &ucTemp);
    ucTemp |= ucBitMask;

    PN512_s_vRFWriteReg(1, ucRegAddr, &ucTemp);
}


void PN512_s_vRFClrBitMask(uchar ucRegAddr, uchar ucBitMask)
{
    uchar ucTemp;

    PN512_s_vRFReadReg(1, ucRegAddr, &ucTemp); 
    ucTemp &= ~ucBitMask;

    PN512_s_vRFWriteReg(1, ucRegAddr, &ucTemp); 
}


void PN512_s_vRFFlushFIFO(void)
{
    uchar ucTemp;

    uchar ucRegAddr = PN512_FIFOLEVEL_REG;
    PN512_s_vRFReadReg(1, ucRegAddr, &ucTemp);
    ucTemp |= 0x80;
    PN512_s_vRFWriteReg(1, ucRegAddr, &ucTemp);
}


//���һREQB�����
uchar  PN512_s_RF_ucREQB(uchar* pucOutLen, uchar* pucOut)
{
     uchar ucRet = RET_RF_OK;
     uchar ucTempData;
	 uint uiFWITemp = 0x00;
	 

    PN512_RF_WorkInfo.ucSFGI = 0;
    PN512_RF_WorkInfo.ulSFGT = 60; 
    
	 PN512_s_vRFFlushFIFO();        //empty FIFO

     // ���ز�����ΪTypeB��
     PN512_s_vRFSelectType('B');
	 
	 // RxCRC and TxCRC enable, ISO14443B
	 ucTempData = 0x83;
     PN512_s_vRFWriteReg(1, PN512_TXMODE_REG, &ucTempData);
     ucTempData = 0x83;
	 PN512_s_vRFWriteReg(1, PN512_RXMODE_REG, &ucTempData);
       
     // ��ֹcrypto1��Ԫ
     PN512_s_vRFClrBitMask(PN512_STATUS2_REG, 0x08);

     PN512_s_RFSetTimer(65);  //RF_DEFAULT);// ��ʱ�ȴ�60ETU

	  
	 // ׼����������: ������
	 #define PICC_REQB           0x00        // request ALL
     PN512_RF_WorkInfo.ulSendBytes = 3;    // how many bytes to send
     PN512_RF_WorkInfo.aucBuffer[0] = PICC_APF;
     PN512_RF_WorkInfo.aucBuffer[1] = 0x00;       // AFI = 0x00
     PN512_RF_WorkInfo.aucBuffer[2] = PICC_REQB;  // TimeSlot = 0,  REQB
    

     // ִ������
     ucRet = PN512_s_RFExchangeCmd(PN512_TRANSCEIVE_CMD);

     if(ucRet == RET_RF_ERR_TIMEOUT) // ��ʱ����Ӧ
     {
           PN512_RF_WorkInfo.usErrNo = 0xA5; // timeout err
           return RET_RF_ERR_TIMEOUT;
     }
     if(ucRet != RET_RF_OK)
     {
           ucRet = RET_RF_ERR_TRANSMIT;
     }
     else
     { 
		    if(PN512_RF_WorkInfo.ulBytesReceived != 12)
            {
             	  PN512_RF_WorkInfo.usErrNo = 0x50; // received bytes err
                  ucRet = RET_RF_ERR_TRANSMIT;
				  goto WUPBEND;
            }
            if(PN512_RF_WorkInfo.aucBuffer[0] != 0x50)
            {
             	  PN512_RF_WorkInfo.usErrNo = 0x51; // received byte 1 err
				  ucRet = RET_RF_ERR_PROTOCOL;
                  goto WUPBEND;
            }

		    *pucOutLen = PN512_RF_WorkInfo.ulBytesReceived;
            memcpy(pucOut, (uchar*)PN512_RF_WorkInfo.aucBuffer, PN512_RF_WorkInfo.ulBytesReceived);

            PN512_RF_WorkInfo.ucUIDLen = 4;
            // ����TypeB��UID���к� ��2~5�ֽ�
            memcpy((uchar*)PN512_RF_WorkInfo.ucUIDB, (uchar*)(PN512_RF_WorkInfo.aucBuffer + 1), 4);

            // ��6~9�ֽ�ΪApplication Data,һ��Ϊ0000���ն˿��Բ������

            // ��10�ֽ�ΪBitRate��PCD��PICCһ��ֻ֧��106KBit
            // ���ܸ��ֽڻ����κ�ֵ������106KBits/s����ͨ��
          
            // ��11�ֽڷ���֡��С��Э�� ��4λΪ֡��С����4λΪЭ��ֵ
			PN512_RF_WorkInfo.FSC = PN512_gl_RF_ulFSList[(PN512_RF_WorkInfo.aucBuffer[10] >> 4) & 0x0F];

            // ��4λ��ʾ�Ƿ�֧��ISO14443Э�飬Ĭ��֧��
            
            if((PN512_RF_WorkInfo.aucBuffer[10] & 0x01) != 0x01) 
            {
             	   PN512_RF_WorkInfo.usErrNo = 0x53; // received byte 11 err
                   ucRet = RET_RF_ERR_PROTOCOL;
				   goto WUPBEND;
            }

            // ��12�ֽڰ���FWI��CID��NAD����Ϣ
            // ��4λ����FWI
            PN512_RF_WorkInfo.ucFWI = (PN512_RF_WorkInfo.aucBuffer[11] & 0x0F0) >> 4;

			uiFWITemp = (uint)0x01 << PN512_RF_WorkInfo.ucFWI;
			PN512_RF_WorkInfo.ulFWT = (ulong)(32 + 3) * uiFWITemp; //(256*16/Fc)*2^FWI + 3*2^FWI ETU
            // ��3~4λ��ʾADC�����迼��
            // ��1~2λ��ʾCID��NAD���ƣ�����PICC����ʲô��������֧�ִ���
            // �ж�NAD
            if(PN512_RF_WorkInfo.aucBuffer[11] & 0x01)
            {
                 PN512_RF_WorkInfo.ucCIDFlag = 1;
            }
            else
            {
                 PN512_RF_WorkInfo.ucCIDFlag = 0;
            }
            // �ж�CID
            if(PN512_RF_WorkInfo.aucBuffer[11] & 0x02)
            {
                 PN512_RF_WorkInfo.ucNADFlag = 1;
            }
            else
            {
                 PN512_RF_WorkInfo.ucNADFlag = 0;
            } 
     }

WUPBEND:
	 if(ucRet == 0)
	 {
	 	if(c_para.card_buffer_w == 1 && c_para.card_buffer_val)
			PN512_RF_WorkInfo.FSC = c_para.card_buffer_val;
	 }
	
     return ucRet;
}


/*********************************************************
����Ϊfelica������غ���
*********************************************************/

uchar PN512_felica_poll(uchar rate,uchar polarity,uchar *cmd,uint slen,uchar *Resp,ulong *rlen)
{
     uchar ucRet = RET_RF_OK;
     uchar ucTemp;

	 

	
	 //��ʼ����������
	 PN512_s_RF_vInitWorkInfo();
	 
     // �Ƚ��ز�����ΪfelicaĬ�����ã�212Kbps,�������
     PN512_s_vRFSelectType('f');
	 if(rate)//��ʾΪ424����
	 {
		 ucTemp = 0xA2;  // FeliCa, 424kBits/s, TxCRCEn
		 PN512_s_vRFWriteReg(1,PN512_TXMODE_REG,&ucTemp); 
		 ucTemp = 0xAA; // FeliCa, 424KBits/s, RxCRCEn, RxNoErr��//Ŀǰ����Ϊ��֡����	
		PN512_s_vRFWriteReg(1,PN512_RXMODE_REG,&ucTemp); 
	 }
	 if(polarity)//��ʾ����������
	 {
		PN512_s_vRFSetBitMask(PN512_TXMODE_REG, 0x08);//�������ݷ���
	 }

	 PN512_s_RFOpenCarrier();//���ز�

	 //PN512_RF_WorkInfo.f_ispoll=1;//��ʾΪpolling�������Ҫ����Ϊ��֡����ģʽ
	 
	 // ���ö�ʱ��
	 PN512_s_RFSetTimer(4100);//��ʱʱ��Ϊ38MS

     // ��ֹcrypto1��Ԫ
     PN512_s_vRFClrBitMask(PN512_STATUS2_REG, 0x08);

	 // ׼����������: ������
	
     PN512_RF_WorkInfo.ulSendBytes = slen;    // how many bytes to send
	 memset((uchar*)PN512_RF_WorkInfo.aucBuffer, 0, sizeof(PN512_RF_WorkInfo.aucBuffer));
	//felica��ͬ���ź�0x00,0x00,0x00,0x00,0x00,0x00+0xb2,0x4d�Ѿ����ú�FelNFC1Reg,PN512���Զ���� 
	 memcpy((uchar*)PN512_RF_WorkInfo.aucBuffer,cmd,slen);//��Ҫ���͵�����
	 
	 ucRet = PN512_s_RFExchangeCmd(PN512_TRANSCEIVE_CMD);

	//if(ucRet )//����������˳�
		//return ucRet;
	
	 
	*rlen=PN512_RF_WorkInfo.ulBytesReceived;
	memcpy(Resp,(uchar*)PN512_RF_WorkInfo.aucBuffer,PN512_RF_WorkInfo.ulBytesReceived);//������Ƭ��������
	Resp[*rlen]=PN512_RF_WorkInfo.usErrNo;
	(*rlen)++;

	PN512_RF_WorkInfo.f_ispoll=0;//��ʾpolling����������ָ���֡����ģʽ

	return ucRet;
}

//for felica
// ucRate: 0-212k 1-424k,default = 0
// ucPol: 0-����1-����,default = 0
uchar PN512_s_RF_ucInit_Felica(uchar ucRate, uchar ucPol)
{	
	uchar ucTemp = 0;
	
	if(PN512_RF_WorkInfo.ucRFOpen == 0)
		return 0x02;// ģ��δ����

	PN512_s_RF_vInitWorkInfo();
	
	// �Ƚ��ز�����Ϊfelica����
	PN512_s_vRFSelectType('f');
	
	if (ucRate == RF_RATE_424_FELICA) 
	{
		ucTemp = 0xA2;  // FeliCa, 424kBits/s, TxCRCEn
		PN512_s_vRFWriteReg(1,PN512_TXMODE_REG,&ucTemp); 
		ucTemp = 0xAA; // FeliCa, 424KBits/s, RxCRCEn, RxNoErr��//Ŀǰ����Ϊ��֡����	
		PN512_s_vRFWriteReg(1,PN512_RXMODE_REG,&ucTemp); 
	}
	if(ucPol == RF_POL_1_FELICA)//��ʾ����������
	{
		PN512_s_vRFSetBitMask(PN512_TXMODE_REG, 0x08);//�������ݷ���
	}
	
	PN512_s_RFOpenCarrier();//���ز�
	
	PN512_RF_WorkInfo.ucCurType = RF_TYPEF;
	return 0;
}

uchar PN512_s_RF_ucExchange_Felica(uint uiCmdLen, uchar* paucCmd, uint* puiRspLen, uchar* paucResp)
{
	uchar ucRet = RET_RF_OK;
		
	// ���ö�ʱ��
	PN512_s_RFSetTimer(4100);//��ʱʱ��Ϊ38MS
	
	// ��ֹcrypto1��Ԫ
	PN512_s_vRFClrBitMask(PN512_STATUS2_REG, 0x08);
	
	// ׼����������: ������	
	PN512_RF_WorkInfo.ulSendBytes = uiCmdLen;    // how many bytes to send
	memset((uchar*)PN512_RF_WorkInfo.aucBuffer, 0, sizeof(PN512_RF_WorkInfo.aucBuffer));
	//felica��ͬ���ź�0x00,0x00,0x00,0x00,0x00,0x00+0xb2,0x4d�Ѿ����ú�FelNFC1Reg,PN512���Զ���� 
	memcpy((uchar*)PN512_RF_WorkInfo.aucBuffer,paucCmd,uiCmdLen);//��Ҫ���͵�����
	
	ucRet = PN512_s_RFExchangeCmd(PN512_TRANSCEIVE_CMD);
	if (ucRet == RET_RF_OK)
	{
		*puiRspLen = PN512_RF_WorkInfo.ulBytesReceived;
		memcpy(paucResp, (uchar*)PN512_RF_WorkInfo.aucBuffer,PN512_RF_WorkInfo.ulBytesReceived);//������Ƭ��������
	}
	else
	{
		paucResp[0] = PN512_RF_WorkInfo.usErrNo & 0xFF;
		paucResp[1] = PN512_RF_WorkInfo.usErrNo >> 8;
		*puiRspLen = 2;
	}
	return ucRet;
}

