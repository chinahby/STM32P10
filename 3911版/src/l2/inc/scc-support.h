/*
 * Author: 
 *  Owner: KaiFa Technology Corporation.
 *   Date: 2008.01.02
 */
 
#ifndef SCC_SUPPORT_H
#define SCC_SUPPORT_H


#define BUF_LEN         300 //512->300
#define MAX_BUF_LEN     512
#define MAX_CARD_NUM    6

#define VCC_3V 					0
#define VCC_1_8V 				1
#define VCC_5V 					2

extern unsigned char   slotnumber;

#pragma pack(1)
struct sc_s {
   int           id;            // SC interface number (0-4)
   int	         fd;            // File handle for SC device ( e.g. "/dev/sc0")
} ;
extern struct sc_s scs[6];

struct scc_cmd
{
    unsigned char   data[BUF_LEN];
    unsigned int   len;	// Length of "data" in byte
    unsigned char   name[64];
};
extern struct scc_cmd packed;
#pragma pack() 

//extern int      scc_filedesc;	// file descriptor for POSIX standard I/O operation 

//�� �� ��: int      scc_init(void)
//��    ��:	��dev file
//�������:	��
//�������: 
//�� �� ֵ:	�ɹ�����0,ʧ�ܷ��ش�����
//ע    ��:     ��������ʱ����һ�ο���




extern int      scc_init(void);	/* SmartCard initialization */
//�� �� ��: void     scc_fini(void)
//��    ��:	�ر�dev file
//�������:	��
//�������: 
//�� �� ֵ:	��
//ע    ��:     

extern void     scc_fini(void);

/*
//�� �� ��: int      scc_select_slot(unsigned char slot)
//��    ��:	ѡ��IC����slot
//�������:	slot:0-6
                    0-�û���
                    1~6 SAM��
//�������:     
//�� �� ֵ:	�ɹ�����0,ʧ�ܷ��ش�����
//ע    ��:     
*/
extern int      scc_select_slot(unsigned char slot);	/* Choose a slot to operate */

/*
//�� �� ��: unsigned char inserted_card(void)
//��    ��:	��鿨�Ƿ�λ
//�������:	��

//�������:     
//�� �� ֵ:	0:�п�����
                1:�޿�����
//ע    ��:  �����û���   
*/

extern unsigned char inserted_card(void);

/*
//�� �� ��: int      scc_reset(unsigned char *voltage, unsigned char *atr, unsigned int *len)
//��    ��:	���ϵ縴λ
//�������: *voltage:��ѹֵ,��Χ0-2
                      0:3V
                      1:1.8V
                      2:5V
                    ȱʡΪ5V
//�������:     *atr:��λ��,���buffer����Ϊ512
                *len:��λ���ĳ���
//�� �� ֵ:	�ɹ�����0,ʧ�ܷ��ش�����
                
//ע    ��:     
*/
extern int      scc_reset(unsigned char *voltage, unsigned char *atr, unsigned int *len);	/* Reset and get the ATR */

/*
//�� �� ��: int SendCmdT0_APDU(struct scc_cmd *command, unsigned char *rbuf,unsigned int *rlen)
//��    ��:	������,֧��T0,T1Э��
//�������: *command:������
//�������:     *rbuf:��������(SW1,SW2,Data)
                *len:�������ݳ���
//�� �� ֵ:	�ɹ�����0,ʧ�ܷ��ش�����
                
//ע    ��:  

struct scc_cmd
{
    unsigned char   data[BUF_LEN];//������:case 1:CLA��INS��P1��P2
                                           case 2:CLA��INS��P1��P2,Le
                                           case 3:CLA��INS��P1��P2,LC Data
                                           case 4:CLA��INS��P1��P2,LC Data Le
                                           
    unsigned int   len;	// Length of "data" in byte
    unsigned char   name[64]; //�ɲ�����������
}__attribute__ ((packed));


  ��    ��:

  	struct scc_cmd tIccCmd;
    
	unsigned char OutBuf[1024];
	unsigned int nLen;
	//memcpy(ApduSend.Cmd,"\x00\x24\x00\x00",4);
	
	memcpy(tIccCmd.data,"\x00\x24\x00\x00",4);
	tIccCmd.data[0] = cla;
	tIccCmd.data[3] = p2;
	tIccCmd.data[4] = lc;
	memcpy(&tIccCmd.data[5],pindata,lc);
	tIccCmd.len = lc+5;
	if(scc_command_APDU(&tIccCmd,OutBuf,&nLen)!=0)
		return 0xff;
		
	status[0] = OutBuf[0];
	status[1] = OutBuf[1];
   
*/
/*Ϊ�˼��ݣ����Ѱ���T1,����scc_command_APDUһ����֧��T0,T1Э��*/
extern int SendCmdT0_APDU(struct scc_cmd *command, unsigned char *rbuf,unsigned int *rlen);

/*
//�� �� ��: int scc_command_APDU(struct scc_cmd *command, unsigned char *rbuf, unsigned int *rlen)
//��    ��:	������,֧��T0,T1Э��,��ͬSendCmdT0_APDU
*/
extern int scc_command_APDU(struct scc_cmd *command, unsigned char *rbuf, unsigned int *rlen);

/*
//�� �� ��: int scc_close(void)
//��    ��:	���µ�,�رյ�ǰ��ѡslot(scc_select_slot)
//�������:     ��
//�������:     ��
                
//�� �� ֵ:	��
                
//ע    ��:     �ɹ�����0,ʧ�ܷ��ش�����
*/
extern int scc_close(void);

#endif /*  SCC_SUPPORT_H  */
