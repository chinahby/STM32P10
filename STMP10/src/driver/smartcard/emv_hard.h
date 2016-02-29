#ifndef KF323_EMV_HARD_H
#define KF323_EMV_HARD_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * �ṩ����Ƭ�Ĺ���ʱ��Ƶ��
 */
#define FCLK_CARD     ( 3600000UL )

/**
 * ��Ƭͨ����Ŷ���
 */
#define USC           ( 0 )/*channel 0*/
#define SAM1          ( 1 )/*channel 1*/
#define SAM2          ( 2 )/*channel 2*/
#define SAM3          ( 3 )/*channel 3*/
#define SAM4          ( 4 )/*channel 4*/

#define SC_BIG_UART   USART2
#define SC_SML_UART   USART1

/* 
 * �û���Ӳ��PIN�Ŷ���
 */
#define		ICC_IO_PIN			GPIO_Pin_2		/* IC������ */
#define		ICC_CLK_PIN			GPIO_Pin_4		/* IC��ʱ�� */
#define		ICC_DET_PIN			GPIO_Pin_15		/* IC��������, 	H:�޿� L:�п����� */
#define		ICC_RST_PIN			GPIO_Pin_2		/* IC����λ,		L:��λ */
#define		ICC_VCC_PIN			GPIO_Pin_6		/* IC�����µ����,	H:�µ� L:�ϵ� */
                                                
#define		ICC_GPIO_PORT_1		GPIOA			/*	IC��- IO/CLK/DET -PORT */
#define		ICC_GPIO_PORT_2		GPIOE			/*	IC��- RST/VCC 	-PORT */

/**	
 * SAMͨ���л���ϵ
 * ---------------------------------------------------------------------------
 *	SAM Card 	CSH	:	CSL
 * ---------------------------------------------------------------------------
 *	SAM1		L		L
 *	SAM2		L		H
 *	SAM3		H		L
 *	SAM4		H		H
 * ---------------------------------------------------------------------------
 */
#define		SAM_RST_PIN			GPIO_Pin_0		/*	SAM����λ,		Ƭѡ���� */
#define		SAM_VCC_PIN			GPIO_Pin_1		/*	SAM���ϵ����	ͳһ����    L���ϵ� H���µ� */
#define		SAM_CLK_PIN			GPIO_Pin_8		/*	SAM��ʱ��,		ͳһ���� */
#define		SAM_IO_PIN			GPIO_Pin_9		/*	SAM������,		Ƭѡ���� */
#define		SAM_CSL_PIN			GPIO_Pin_10		/*	SAM��ѡ���λ,	H:Ƭѡ (*)�ɰ汾��LED���������ʹ��������� */
#define		SAM_CSH_PIN			GPIO_Pin_11		/*	SAM��ѡ���λ,	H:Ƭѡ (*)�ɰ汾��LED���������ʹ��������� */
                                                
#define		SAM_GPIO_PORT_1		GPIOA			/*	RST/VCC/CLK/IO Port */
#define		SAM_GPIO_PORT_2		GPIOF			/*	SL/SH Port */


#ifdef __cplusplus
}
#endif

#endif
