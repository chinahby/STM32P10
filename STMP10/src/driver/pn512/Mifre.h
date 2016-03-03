#ifndef _MYFRE_H_
#define	_MYFRE_H_

/**********************************************************************

                         ��������
                          
***********************************************************************/
// ISO14443 
#define RF_TYPEA				0x00
#define RF_TYPEB				0x08
#define RF_TYPEF        		0x0F

#define RF_TYPE_S50          0
#define RF_TYPE_S70          1
#define RF_TYPE_PRO          2
#define RF_TYPE_PRO_S50      3
#define RF_TYPE_PRO_S70      4
#define RF_TYPE_B            5

// ���������붨��
#define RET_RF_OK                  0x00    // �����ɹ�
#define RET_RF_ERR_PARAM           0x01    // 0x09 ��������
#define RET_RF_ERR_NO_OPEN         0x02    // 0x08 ��Ƶ��ģ��δ����

#define RET_RF_ERR_NOT_ACT         0x13    // 0x0B ��Ƭδ���� // added by liuxl 20070925
#define RET_RF_ERR_MULTI_CARD      0x14    // �࿨��ͻ
#define RET_RF_ERR_TIMEOUT         0x15    // ��ʱ����Ӧ
#define RET_RF_ERR_PROTOCOL        0x16    // Э�����

#define RET_RF_ERR_TRANSMIT        0x17    // ͨ�Ŵ������
#define RET_RF_ERR_AUTH            0x18    // ��֤ʧ��
#define RET_RF_ERR_NO_AUTH         0x19    // ����δ��֤
#define RET_RF_ERR_VAL             0x1A    // ��ֵ�����ݸ�ʽ����
#define RET_RF_ERR_CARD_EXIST      0x1B    // ��Ƭ���ڸ�Ӧ����
#define RET_RF_ERR_STATUS          0x1C    // ��Ƭδ����Э��̬ʱ����APDU����,���ѽ���Э��̬ʱ����M1����// added by liuxl 20070925
//#define RET_RF_ERR_NO_MODULE       0x1D    // ��RFģ��
//end
//end
// p80 �еĶ���
enum DETECT_ERROR{E_INVALID_PARA=1,E_NO_POWER,E_NOT_DETECTED,E_MULTI_CARDS,E_ACT_FAILED,E_PROTOCOL};

enum ISOCMD_ERROR{EE_INVALID_PARA=1,EE_NO_POWER,EE_NOT_DETECTED,EE_TX,EE_PROTOCOL};


// ��ʱ��������
#define  RF_FWTMIN       9           // ����WUPA��ANTICOLLISION��SELECT����ĵȴ���Ӧʱ��
                                     // ʵ����Ӧ����9.2ETU
#define  RF_FWTRATS      560         // ���ڵȴ�RATS֡
#define  RF_FWTDESELECT  560         // ���ڵȴ�DESELECT��Ӧ֡
#define  RF_FWTWUPB      60          // ���ڵȴ�WUPB��Ӧ֡
#define  RF_MAXFWT       8960        // FWT���ʱ��Ϊ32 * 2^8 + 3 * 2^8 = 8960ETU
#define  RF_DEFAULT      560         // ȱʡ֡�ȴ�ʱ��Ϊ560ETU
#define  RF_TPDEALY     (uint)500    // �л����Ʒ�ʽʱ��ʱ500��ETU
#define  RF_TRESET      (uint)1000   // ��λPICCʱ����ʱ1000ETU
#define  RF_FDTMIN      (uint)70     // ���ڱ�֤PCD������һ������֡ǰ��500us��ʱ

// �ڲ���������
#define RECEIVE_SBLOCK       0x31
#define RECEIVE_IBLOCK       0x32
#define SEND_NEXTBLOCK       0x33
#define RESEND_IBLOCK        0x34
#define RESEND_ACK           0x35

#define IBLOCK_UNCHAINING    0x00
#define IBLOCK_CHAINING      0x01
#define IBLOCK_SEND          0x00
#define IBLOCK_RECEIVE       0x01

#define RF_BUFFER_LEN        272  // liuxl ���ջ�������󳤶�

#define RF_RATE_212_FELICA 0 //default = 0;
#define RF_RATE_424_FELICA 1
#define RF_POL_0_FELICA    0 // default = 0;
#define RF_POL_1_FELICA    1


extern uchar PiccOpen(void);
extern uchar PiccDetect(uchar Mode,uchar *CardType,uchar *SerialInfo,uchar *CID,uchar *Other);
extern uchar PiccIsoCommand(uchar cid,APDU_SEND *ApduSend,APDU_RESP *ApduRecv);
extern void PiccClose(void);

#endif

