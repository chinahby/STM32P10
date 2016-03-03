
/**************************************************************
			������ΪPN512�ײ�������
			�����ϸ����㻮��
			
**************************************************************/
#define FOR_PN512_DEVICE
#include "Mifre_TmDef.h"
#include "kf701dh.h"

extern PN512_RF_WORKSTRUCT PN512_RF_WorkInfo;
extern PICC_PARA c_para;  

//extern const uchar PN512_PCD_VER[5];
//extern const uchar PN512_PCD_DATE[12];



#define RF_SPI_BAUDRATE   (3*1000*1000)

/**********************************************************************

                          ����ȫ�ֱ�������

***********************************************************************/
#define RF_SPI_SEND   1//SPI����
#define RF_SPI_RECV   0//SPI����

/******************************************************************************
* �������ã�ʹ��SPI�ӿ�ʱ��Ҫ�������ݵĻ�����ָ��
* ȡֵ��Χ��ָ����
* ������Ϣ��
*           ����                ���ʷ�ʽ            ���ʷ���
*       PN512_s_vRFReadReg             ��д        ������pucRegAddr��ֵ��RF_pucSendBuf,
*                                                            ������ȡ���ݽ��з���
*       PN512_s_vRFWriteReg             ��д        ������pucData��ֵ��RF_pucSendBuf
*       PN512_s_RF_SPIIrq                ֻ��        ��RF_pucSendBuf��ʼ�Ļ�������ȡ����
*                                                            ���з���
******************************************************************************/
//static uchar RF_pucSendBuf[10];

/******************************************************************************
* �������ã�SPIģʽ��, ���Ĵ�������ʱ�ļĴ�����ַ
* ȡֵ��Χ��ָ����
* ������Ϣ��
*           ����                ���ʷ�ʽ            ���ʷ���
*       PN512_s_vRFReadReg               ֻд        ������ucRegAddr��ֵ��RF_ucRegAddr,
*       PN512_s_RF_SPIIrq                  ֻ��        ��������RF_ucRegAddr
******************************************************************************/
static volatile uchar  RF_ucRegAddr = 0;

/******************************************************************************
* �������ã�ʹ��SPI�ӿ�ʱ����Ž������ݵĻ�����ָ��
* ȡֵ��Χ��ָ����
* ������Ϣ��
*           ����                ���ʷ�ʽ            ���ʷ���
*       PN512_s_vRFReadReg               ��д        ������pucData��ֵ��RF_pucSendBuf,
*                                                              ������ȡ���ݽ��з���
*       PN512_s_RF_SPIIrq                  ֻ��        ���յ����ݴ���RF_pucRecvBuf��ʼ
*                                                              �Ļ�������
******************************************************************************/
//static uchar RF_pucRecvBuf[10];

/******************************************************************************
* �������ã�ʹ��SPI�ӿ�ʱ��ָ��ǰҪ���պͷ���λ��
* ȡֵ��Χ��ָ����
* ������Ϣ��
*           ����                ���ʷ�ʽ            ���ʷ���
*       PN512_s_vRFReadReg                 д          ����ʼֵ0
*       PN512_s_vRFWriteReg                 д          ����ʼֵ0
*       PN512_s_RF_SPIIrq                   ��д      ���յ����ݴ���RF_pucRecvBuf��ʼ��
*                                                             ��������RF_ulBufCurָ���λ�ã�
*                                                             ��������Ȼ����RF_pucSendBuf
*                                                             ��ʼ�Ļ�������RF_ulBufCurָ��
*                                                             ��λ�õ�����
******************************************************************************/
static volatile uint  RF_ulBufCur;

/******************************************************************************
* �������ã�ʹ��SPI�ӿ�ʱ��������Ҫ���ͺͽ��յ����ݳ���
* ȡֵ��Χ������0
* ������Ϣ��
*           ����                ���ʷ�ʽ            ���ʷ���
*       PN512_s_vRFReadReg                  д          ����ʼֵΪ����len
*       PN512_s_vRFWriteReg                  д          ����ʼֵΪ����len
*       PN512_s_RF_SPIIrq                  ֻ��        �����ͺͽ������ݳ��ȴﵽRF_ulBufLen
*                                                              ʱ��ֹͣ�շ�����
******************************************************************************/
static volatile uint  RF_ulBufLen;    //���ݳ���

/******************************************************************************
* �������ã�ʹ��SPI�ӿ�ʱ����ΪSPIģʽ���ͽ�����־
* ȡֵ��Χ��ָ����
* ������Ϣ��1������SIO���͹��� 0-SIO���ͽ���
*           ����                ���ʷ�ʽ            ���ʷ���
*       PN512_s_vRFReadReg               ��д        ��ʼ����Ϊ1, Ȼ��ȴ���Ϊ0
*       PN512_s_vRFWriteReg               ��д        ��ʼ����Ϊ1, Ȼ��ȴ���Ϊ0
*       PN512_s_RF_SPIIrq                   ֻд        �����ͺͽ������ݳ��ȴﵽRF_ulBufLen
*                                                               ʱ������Ϊ1
******************************************************************************/
static volatile uchar RF_ulSPIEnd;

/******************************************************************************
* �������ã�ʹ��SPI�ӿ�ʱ�������Ƿ������ݻ��ǽ�������
* ȡֵ��Χ��RF_SPI_SEND: ����ģʽ   RF_SPI_RECV: ����ģʽ
* ������Ϣ��1������SIO���͹��� 0-SIO���ͽ���
*           ����                ���ʷ�ʽ            ���ʷ���
*       PN512_s_vRFReadReg               ֻд        ����ΪRF_SPI_RECV
*       PN512_s_vRFWriteReg               ֻд        ����RF_SPI_SEND
*       PN512_s_RF_SPIIrq                   ֻ��        ����RF_ulSPIMode�����в�ͬ����
******************************************************************************/

//ʵ��SPI�����������ñ�����ָ�����

struct Spi_Ctl_Pro_t
{
	volatile unsigned int rGPIO_EN;
	volatile unsigned int rSPI_BRG_REG;
	volatile unsigned int rSPI_MOD_REG;
	volatile unsigned int rSPI_DMA_REG;
	volatile unsigned int rSPI_CTL_REG;
	volatile unsigned int rSPI_STA_REG;
	volatile unsigned int rSPI_DIAG_REG;
	volatile unsigned int rSPI_DAT_REG;
};

static struct Spi_Ctl_Pro_t  Spi_Ctl_Pro;

void inline Spi_Ctl_Protect(unsigned char mode)//mode=0 ���� mode=1 �ָ�
{
	#if (TERM_TYPE == TERM_KF900)
		if( !mode )
		{
			Spi_Ctl_Pro.rGPIO_EN = rGPIO1_EN;
			Spi_Ctl_Pro.rSPI_BRG_REG = rSPI1_BRG_REG;
			Spi_Ctl_Pro.rSPI_MOD_REG = rSPI1_MOD_REG;
			Spi_Ctl_Pro.rSPI_DMA_REG = rSPI1_DMA_REG;
			Spi_Ctl_Pro.rSPI_CTL_REG = rSPI1_CTL_REG;
			Spi_Ctl_Pro.rSPI_STA_REG = rSPI1_STA_REG;
			Spi_Ctl_Pro.rSPI_DIAG_REG= rSPI1_DIAG_REG;
			Spi_Ctl_Pro.rSPI_DAT_REG = rSPI1_DAT_REG;
		}
		else
		{
			rGPIO1_EN &=~(BIT12 | BIT14 | BIT16 | BIT18);//��0
			rGPIO1_EN|= Spi_Ctl_Pro.rGPIO_EN&(BIT12 | BIT14 | BIT16 | BIT18);
			rSPI1_BRG_REG=Spi_Ctl_Pro.rSPI_BRG_REG;
			rSPI1_MOD_REG=Spi_Ctl_Pro.rSPI_MOD_REG;
			rSPI1_DMA_REG=Spi_Ctl_Pro.rSPI_DMA_REG;
			rSPI1_CTL_REG=Spi_Ctl_Pro.rSPI_CTL_REG;
			rSPI1_STA_REG=Spi_Ctl_Pro.rSPI_STA_REG;
			rSPI1_DIAG_REG=Spi_Ctl_Pro.rSPI_DIAG_REG;
			rSPI1_DAT_REG=Spi_Ctl_Pro.rSPI_DAT_REG;
		}
	#endif
}



static volatile uchar RF_ulSPIMode;    //���ͻ��߽���

/******************************************************************************
��д��������ʹ�����л���

******************************************************************************/
void PN512_IS_Select(uchar flag)//0-diselect 1-select
{
	#if (TERM_TYPE == TERM_KF900)
		rGPIO2_EN |= ( BIT6| BIT7);
		// Configure the GPIO2[7:6]as OUTPUT Pin
		rGPIO2_OE |= ( BIT6| BIT7);
		
		if(flag)
		rGPIO2_OUT &=~( BIT6| BIT7 );//SELECT MIFARE
		else
		rGPIO2_OUT |=( BIT6| BIT7 );//SELECT NOTHING
	#endif
}


void PN512_s_RF_SPIProc(void)
{
#if (TERM_TYPE == TERM_KF900)
	// ��λ���ռ�����ָ��
	uint i=0;
    rSPI1_DMA_REG |= (BIT4 | BIT20);

	if (RF_ulSPIMode == RF_SPI_SEND)
	{
		rSPI1_DAT_REG = RF_ucRegAddr <<8;//д,ע��S80/S90��SPI���ݼĴ�����SPI1_DAT_REG����Ҫ��������8λ�ٸ�ֵ����		
	}
	else
	{
		rSPI1_DAT_REG = (RF_ucRegAddr|(uchar)0x80) <<8;//�����һ����ַ��MSB����Ϊ1������ĵ�ַMSB����Ϊ0 
	}
	while (RF_ulSPIEnd)
	{
		while(1)//�ȴ�ֱ��SPI����,modify by skx
		{
			if( !(rSPI1_STA_REG & BIT1) )break;
			i++;
			if(i>1000)break;
		}
		if(i>1000)
		{
			//for debug 
			//printk("/***KF900 RF SPI Busy err***/ \n");
			return ;//SPIͨѶ����
		}
		
		//for debug 
		//printk("/***KF900 RF SPI LOOP[%d]***/ \n",i);

	    if (RF_ulSPIMode == RF_SPI_SEND)
        {
			// ���ڷ�������
            if (RF_ulBufCur == RF_ulBufLen)
            {
                PN512_s_vRFSwitchNCS(1);//����SS��Ƭѡ��Ϊ��
                RF_ulSPIEnd = 0;
            }
            else
            {
				rSPI1_DAT_REG = RF_pucSendBuf[RF_ulBufCur]<<8; // ��������
                RF_ulBufCur++;
            }
        }
        else
        {
			if (RF_ulBufCur > 0)
            {
                RF_pucRecvBuf[RF_ulBufCur-1] = rSPI1_DAT_REG;//��ȡSPI1_DAT_REG���ݲ���Ҫ����8λ����
                if (RF_ulBufCur == RF_ulBufLen)
                {
                    PN512_s_vRFSwitchNCS(1);//����SS��Ƭѡ��Ϊ��
                    RF_ulSPIEnd = 0;
                }
            }

            if (RF_ulBufCur < RF_ulBufLen)
            {
                rSPI1_DMA_REG |= (BIT20);
				if (RF_ulBufCur == (RF_ulBufLen - 1))
                {
					rSPI1_DAT_REG = 0;
                }
                else
                {
                    rSPI1_DAT_REG = RF_ucRegAddr<<8;
                }
                RF_ulBufCur++;
            }
        }
	}
     // �����״̬��־
	 rSPI1_STA_REG |= (BIT3|BIT4 | BIT5 | BIT6 | BIT7);
#endif 
}


void PN512_s_InitRFSPI(void)
{

#if (TERM_TYPE == TERM_KF900)
	//can not config mode err detection fun for zilog9
	
	#define	HCLK	96*1000*1000	
	// Enable the SPI1 CLk
     if(!(rPMU_CKEN_REG & PMU_SPI1_CKEN))
     {
          rPMU_CKEN_REG |= PMU_SPI1_CKEN;
     }

	 // ʹ�ܸ�SPI����
	 rGPIO1_EN &= ~(BIT12 | BIT14 | BIT16 | BIT18);
	
	 // ����SPI������,4MHz
	 rSPI1_BRG_REG = HCLK / (2*RF_SPI_BAUDRATE);

	 // ����ģʽ�Ĵ���
	 rSPI1_MOD_REG |= (BIT0 | BIT1);  // Master Mode
	 
     // 8-bit data transfer
	 rSPI1_MOD_REG &= ~(BIT2 | BIT3 | BIT4 | BIT5);
	 rSPI1_MOD_REG |= BIT5;

	 // ��λ���ռ�����ָ��
     rSPI1_DMA_REG |= (BIT4 | BIT20);
	 // ����SPI1���ƼĴ���
	 rSPI1_CTL_REG |= (BIT0 | BIT1); // SPI enable and Master mode selected
	 rSPI1_CTL_REG &= ~(BIT3 | BIT4);//select polarity and phase
	 rSPI1_CTL_REG &= ~BIT7;//disable spi intterrupt
	 
	 //AT
	 rSPI1_DMA_REG &=~(BIT0 | BIT1 | BIT16 |BIT17);//set tx rx fifo is 1
	 
     // �����״̬��־
	 rSPI1_STA_REG |= (BIT3 | BIT4 | BIT5 | BIT6 | BIT7);
	 

#endif

#if (TERM_TYPE == TERM_KF322)
	//form RF1356.c
	extern void SPI1_Init(void);
	SPI1_Init();
#endif
}


void PN512_s_InitRFGPIO(void)
{
#if (TERM_TYPE == TERM_KF900)
	int ucTemp=-1;
    // ��Ҫ��RF_IRQ�ж���NSSƬѡ
	rGPIO2_EN |= BIT5;
	// Configure the GPIO2[5]as Input Pin
	rGPIO2_OE &= ~BIT5;

    // Disable GPIO2[5] as Interrupt Pin first
    rGPIO2_IEN &= ~BIT5;
    // Configure as Edge Interrupt
    rGPIO2_IMOD |= BIT5;
	//rGPIO2_IMOD &= ~(BIT5); //����Ϊ��ƽ��������ֹ��ʧ�ж�

    // Configure as Falling Interrupt
    rGPIO2_IPOL &= ~BIT5;
	// Configure as A channel Interrupt
    rGPIO2_ISEL &= ~BIT5;
    
    rGPIO2_ICLR |=	BIT5;//CLEAN INT

    //set IRQ type and priority
	set_irq_type(ZILOG_GPIO_IRQ(32*2+5),IRQT_FALLING);
	ucTemp=request_irq(ZILOG_GPIO_IRQ(32*2+5), (irq_handler_t)PN512_s_RFIsr, SA_INTERRUPT, "Mifre", NULL);//SA_INTERRUPT-��ʾΪ�����ж�,����ʱ���������ж�
	if(ucTemp)
	{
		printk("RF card pn512 irq[%d] request Fail:[%d] !!!\n",ZILOG_GPIO_IRQ(32*2+5),ucTemp);
	}
	
	PN512_IS_Select(0);//disselect
		
#endif

#if (TERM_TYPE == TERM_KF322)
	//form RF1356.c
	extern void RFinterrupt_Init(void);
	RFinterrupt_Init();
#endif

} 

// NRSTPWD: NOT REST AND POWER DOWN
// LOW: hard power down; low->high: reset
void PN512_s_vRFReset(void)
{	
#if (TERM_TYPE == TERM_KF900)

//do nothing,beause reset have do by cpu
	
#endif

#if (TERM_TYPE == TERM_KF322)

//do nothing,beause reset have do by cpu
    GPIO_InitTypeDef GPIO_InitStructure;

    //GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    GPIO_SetBits(GPIOA,RFRESET);
   
    DelayMs(2);
    GPIO_ResetBits(GPIOA,RFRESET);
    DelayMs(3);
    GPIO_SetBits(GPIOA,RFRESET);
    DelayMs(2);
    

#endif
}


void PN512_s_vRFSwitchNCS(char chHigh)
{
#if (TERM_TYPE == TERM_KF900)
	// ncs 
	if(chHigh)
	{
		PN512_IS_Select(0);
		
		//for debug
		//rGPIO1_EN_SET |= ( BIT18);
		//rGPIO1_OE_SET |= (BIT18);
		//rGPIO1_OUT |=BIT18;
		rSPI1_MOD_REG |= BIT0;
		
	}
	else 
	{
		PN512_s_InitRFSPI();//add api config,����
		PN512_IS_Select(1);
		
		//for debug
		//rGPIO1_EN_SET |= ( BIT18);
		//rGPIO1_OE_SET |= (BIT18);
		//rGPIO1_OUT &=~BIT18;
		rSPI1_MOD_REG &= ~BIT0;		
	}		
#endif

#if (TERM_TYPE == TERM_KF322)
	//form RF1356.c
	extern void PN512_RFSwitchNCS(char chHigh);
	PN512_RFSwitchNCS(chHigh);
#endif

}

int PN512_s_vRFReadReg (ulong ulLen, uchar ucRegAddr, uchar* pucData)
{ 
#if (TERM_TYPE != TERM_KF322)
	if (ulLen == 0)
		return 0xFF;
		
		
	//for debug
	local_irq_disable();
	//Spi_Ctl_Protect(0);
	
	PN512_s_vRFSwitchNCS(0);//CSƬѡ����
	RF_ulSPIMode = RF_SPI_RECV;
	memset(RF_pucRecvBuf, 0, sizeof(RF_pucRecvBuf));
	RF_ucRegAddr = (ucRegAddr<<1)&(uchar)0x7E;//���ö�ȡRC531�ĵ�ַ����1/0+6λ��ַ+0��
	
	RF_ulBufCur = 0;
	RF_ulBufLen = ulLen;
	RF_ulSPIEnd = 1;       
	
	// �ȴ����ͺͽ������
	PN512_s_RF_SPIProc();  
	memcpy(pucData, RF_pucRecvBuf, ulLen);
	
	//for debug
	//Spi_Ctl_Protect(1);
	local_irq_enable();
    return 0;
#else
    extern uchar PN512GetReg(uchar  RegAddr);

    if (ulLen != 1)
		return 0xFF;
    *pucData = PN512GetReg(ucRegAddr);
    return 0;
#endif
}

int PN512_s_vRFWriteReg (ulong ulLen, uchar ucRegAddr, uchar* pucData)
{
#if (TERM_TYPE != TERM_KF322)
	if (ulLen == 0)
		return 0xFF; 
	
	//for debug
	local_irq_disable();
	//Spi_Ctl_Protect(0);
	
	PN512_s_vRFSwitchNCS(0);//CS����
	RF_ulSPIMode = RF_SPI_SEND; 
	memset(RF_pucSendBuf, 0, sizeof(RF_pucSendBuf));
	memcpy(RF_pucSendBuf,pucData, ulLen);
	RF_ulBufCur = 0;
	RF_ulBufLen = ulLen;
	RF_ulSPIEnd = 1;
	RF_ucRegAddr = (ucRegAddr<<1)&(uchar)0x7E;//���ö�ȡRC531�ĵ�ַ����1/0+6λ��ַ+0��
	
	// �ȴ����ͺͽ������
	PN512_s_RF_SPIProc();
	
	//for debug
	//Spi_Ctl_Protect(1);
	local_irq_enable();
    return 0;
#else
    extern void PN512SetReg(uchar RegAddr,uchar RegValue);
    if (ulLen != 1)
		return 0xFF;
    PN512SetReg(ucRegAddr,*pucData);
    
    return 0;
#endif
}


int PN512_sPcdInit(void)
{
	int ret=0;
	//char pucData;
	
	 ret=PN512_s_RF_Init();//��ʼ������

	 PN512_sHal_Module_Disable();//��PN512��SPI�����µ紦��
	 
	 //memset(&Spi_Ctl_Pro,0,sizeof(struct Spi_Ctl_Pro_t));//����SPI����������
	
	return ret;
}

int InitNotUseGpio(void)
{
  GPIO_InitTypeDef   GPIO_InitStructure;
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5| GPIO_Pin_6| GPIO_Pin_7 ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12| GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15 ;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12| GPIO_Pin_15 ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_Init(GPIOB, &GPIO_InitStructure);


  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  return 0;
}

int PN512_s_RF_Init(void)
{
        unsigned int i = 0;
	uchar ucReadVal = 0;
	uchar ucWriteVal = 0;
	
	PN512_s_InitRFSPI(); // ��ʼ��SPI����
	PN512_s_InitRFGPIO();
	PN512_s_vRFReset();				
	
        
	//beause reset is do for cpu so do not do below
	//--detect the end of initial phase,idle state
	
	
	for(i=0;i<0x4000;i++)
	{
		PN512_s_vRFReadReg(1,0x01,&ucReadVal);  
		if(ucReadVal==0x20)break;
	}
	if(i == 0x4000)
	    return -1;
	
	    
	ucReadVal = 0x00;
  	PN512_s_vRFWriteReg(1,PN512_PAGE_REG,&ucReadVal); //SPIģʽ,����ƽ����ʼĴ���

	PN512_s_vRFReadReg(1,PN512_TXSEL_REG,&ucReadVal);  
	ucWriteVal = ucReadVal+0x01;
	PN512_s_vRFWriteReg(1,PN512_TXSEL_REG,&ucWriteVal); 
	PN512_s_vRFReadReg(1,PN512_TXSEL_REG,&ucReadVal);  
	if(ucReadVal != ucWriteVal)
	{
		#ifdef FOR_LINUX_SYS
		printk("SPI ERR_1,%02x/%02x\n",ucWriteVal,ucReadVal);
		#else
		
		#endif
		return -2;
	}
		
	/*
	ucReadVal=0xff;   
	PN512_s_vRFReadReg(1,PN512_DIVIEN_REG,&ucReadVal);//��ȡ0x03�Ĵ���ֵ����������RC531��PN512
	if( ucReadVal )
	{
		printk("SPI ERR_2,%02x\n",ucReadVal);
		return -3;
	}
	*/
	
	ucReadVal = 0x10;
	PN512_s_vRFWriteReg(1,PN512_TXSEL_REG,&ucReadVal); // Modulation signal (envelope) from the internal coder

	ucReadVal = 0x80;
	PN512_s_vRFWriteReg(1, PN512_COMMIEN_REG,&ucReadVal); // Irq Inv		
	PN512_s_vRFWriteReg(1, PN512_DIVIEN_REG,&ucReadVal);  // close pcd interrupts 2,IRQPushPull=1

	
	PN512_s_vRFReadReg(1,PN512_CONTROL_REG,&ucReadVal);
	ucReadVal |= 0x10;
	PN512_s_vRFWriteReg(1,PN512_CONTROL_REG,&ucReadVal);//����PN512Ϊ��������������

	memset((uchar *)&PN512_RF_WorkInfo, 0x00, sizeof(PN512_RF_WORKSTRUCT));
	memset(&c_para,0x00,sizeof(c_para));
	
	
	c_para.a_conduct_val = 0x3f;
	c_para.m_conduct_val = 0x3f;
	c_para.b_modulate_val = 0x0a;//0x0a;0x0c;0x0f//0x08;0x0b 0x17
	c_para.card_RxThreshold_val=0x70; //70->75(13/07/06 0cm)->55(13/07/06 2cm)
	c_para.f_modulate_val=0x08;
	
	
	c_para.card_buffer_val = 32;	

	return 0;	
}



/*******************************************************************************
  
****************************************************************************/
uchar PN512_sHal_Module_Disable(void)
{

	PN512_sHal_MaskCpuInt();//��ֹ����CPU�ж�
	PN512_s_vRFSwitchNCS(1);//CS ����
    
	#if (TERM_TYPE ==  TERM_KF900) 
    	// �ر�SPI�ж�
    	// �����״̬��־
    	rSPI1_STA_REG |= (BIT4 | BIT5 | BIT6 | BIT7);
    	rSPI1_CTL_REG &= ~BIT0; // SPI ENABLE-DEASSERTED
   
    	// NRSTPWD LOW(Control by cpu)
    	free_irq(ZILOG_GPIO_IRQ(32*2+5),NULL);//�ͷ��ж�����RC531��������ɹ� 
	#endif
    
	return 0;
}

/*******************************************************************************
  
****************************************************************************/
//#define EXTI_INTERRUPT_LINE		BIT8
#define EXTI_INTERRUPT_LINE		BIT1

void PN512_sHal_EnCpuInt(void)//ʹ��CPU�ж�
{
	
#if (TERM_TYPE == TERM_KF900)
	//PN512_s_InitRFGPIO();
	// Enable the Interrupt
    rGPIO2_IEN |= BIT5;		
#endif

#if (TERM_TYPE == TERM_KF322)
	// Enable the Interrupt
	//from stm3210x_exti.c
    extern void EXTI_EnableITBit(unsigned int EXTI_Line);
    EXTI_EnableITBit(EXTI_INTERRUPT_LINE);	
#endif

}


/*******************************************************************************
  
****************************************************************************/
void PN512_sHal_MaskCpuInt(void)//��ֹ����CPU�ж�
{

#if (TERM_TYPE == TERM_KF900)   		
	// Disable the Interrupt
	rGPIO2_IEN &= ~BIT5;
	rGPIO2_ICLR|=  BIT5;
#endif

#if (TERM_TYPE == TERM_KF322)
	// Enable the Interrupt
	//from stm3210x_exti.c
    extern void EXTI_DisableITBit(unsigned int EXTI_Line);
    //extern void EXTI_ClearITPendingBit(unsigned int EXTI_Line);
    //EXTI_DisableITBit(EXTI_INTERRUPT_LINE);
    EXTI_ClearITPendingBit(EXTI_INTERRUPT_LINE);
#endif
}

