#ifndef _UARTPROTOCOL_H_
#define _UARTPROTOCOL_H_

#define COMM_BANKUNION

#ifdef COMM_BANKUNION

#define POSPICC_INIT_M     		0
#define POSPICC_POLL_M     		1
#define POSPICC_NORMAL_M		2
#define POSPICC_DEBUG_M        		3
#define POSPICC_PAY_M        		4
#define POSPICC_MAGAGER_M      		5

//command code
#define CMD_POLL_POLL	        		0x07  //POLL
#define CMD_POLL_ECHO	        		0x08  //echo

#define CMD_DEBUG_ENTER	        		0x10  //���õ��Ժ��Ż�����ģʽ
#define CMD_DEBUG_SETPARAMETER			0x12  //���ò���

#define CMD_AUTH_COMM_INIT      		0x20  //ͨѶ��ʼ��
#define CMD_AUTH_BOTH           		0x21  //˫����֤
#define CMD_AUTH_KEY_GENERATE                   0x22  //��Կ����
#define CMD_AUTH_DISABLE_READER                 0x23  //���ζ�����

#define CMD_TRAN_READYFORPAY    		0x30  //׼��֧������
#define CMD_TRAN_RESET          		0x31  //��λ����
#define CMD_TRAN_DISP_STATUS    		0x32  //��ʾ״̬
#define CMD_TRAN_ONLINE         		0x6A  //������������
#define CMD_TRAN_UPCARD					0x6B  //UPCARD

#define CMD_MANAGER_ENTER       		0x40  //�������ģʽ
#define CMD_MANAGER_GET_CAPA    		0x41  //��ȡ����
#define CMD_MANAGER_SET_CAPA    		0x42  //��������
#define CMD_MANAGER_GET_TIMEDATE		0x43  //��ȡʱ�������
#define CMD_MANAGER_SET_TIMEDATE		0x44  //����ʱ�������
#define CMD_MANAGER_GET_PARAMETER   	0x45  //��ȡ����
#define CMD_MANAGER_GET_COMM_BPS    	0x52  //��ȡ����ͨ������
#define CMD_MANAGER_SET_COMM_BPS    	0x53  //���ô���ͨ������
#define CMD_MANAGER_RESET_ACQUIRE_KEY	0x54  //�����յ�����Կ
#define CMD_MANAGER_RESTORE_READER      0x55  //�ָ�������
#define CMD_MANAGER_GET_TAG 		    0x56  //��ȡPBOC��ǩֵ
#define CMD_MANAGER_SET_TAG				0x57  //����PBOC��ǩֵ
#define CMD_MANAGER_GET_DISP  			0x58  //��ȡ��ʾ��Ϣ
#define CMD_MANAGER_SET_DISP  			0x59  //������ʾ��Ϣ
#define CMD_MANAGER_GET_CVM				0x5A  //��ȡCVM����
#define CMD_MANAGER_SET_CVM				0x5B  //����CVM����
#define CMD_MANAGER_SET_CAPK            0x61  //����PBOC��Կ
#define CMD_MANAGER_FIND_CAPK			0x62  //��ѯPBOC��Կ
#define CMD_MANAGER_SET_REVOCLIST       0x63  //���û��չ�Կ֤��
#define CMD_MANAGER_FIND_REVOCLIST      0x64  //��ѯ���չ�Կ֤��
#define CMD_MANAGER_SET_EXCEPTIONLIST   0x65  //���ú�����
#define CMD_MANAGER_FIND_EXCEPTIONLIST  0x66  //��ѯ������
#define CMD_MANAGER_SET_PBOC_PARAMETER  0x67  //����PBOC�̶�����
#define CMD_MANAGER_SET_AID				0x68  //����AID��ز���

//RFU
#define CMD_MANAGER_SET_ALL_AID			0x80  //��������AID��ز���

//respone code

#define RC_SUCCESS             	0x00  	  //ͨ��Ӧ����ָʾ�������ɹ�ִ����������                   
#define RC_DATA                	0x01  	  //�������ӷǽӴ���Ƭ��õ�������Ч��������������         
#define RC_POLL_A              	0x02  	  //������ȷ��Ӧ���ն˺Ͷ�����֮����˫����֤             
#define RC_POLL_P              	0x03  	  //������ȷ��Ӧ���ն˺Ͷ�����δ����˫����֤               
#define RC_SCHEME_SUPPORTED    	0x04  	  //������֧�ָ�֧������                                       
#define RC_SIGNATURE           	0x05  	  //��Ҫǩ��                                                   
#define RC_ONLINE_PIN          	0x06  	  //��Ҫ����PIN                                             
#define RC_OFFLINE_PIN         	0x07  	  //��Ҫ�ѻ�PIN                                            
#define RC_SECOND_APPLICATION  	0x08  	  //��ʾ������֧�ֵڶ���Ӧ��(��֧��Ӧ��) 
//add 
#define RC_DECLINE              0x20      //�ܾ�����
#define RC_FILE_ERR             0x21      //�����ļ�����
#define RC_USE_CONTACT			0x22      //��ʹ�ýӴ�ʽ����
#define RC_READER_ERR    		0x23       //��Ƶģ�����
#define RC_EMV_NO_APP           0x24       //Ӧ�ú�ѡ�б�Ϊ��
#define RC_ICC_BLOCK			0x25	   //������
#define RC_EMV_APP_BLOCK		0x26	   //��Ӧ�ñ���
#define RC_TRAN_FAILURE			0x27	   //���״���
#define RC_EXCEED_OFFLINEAMT	0x28	   //�����ѻ��޶�
#define RC_ONLINE				0x29	   //���߽���
#define RC_TERMINATE 			0x2A	   //������ֹ
#define RC_ICC_CMD_ERR			0x2B       //IC����ͨѶ��

#define RC_CARD_EXPIRED			0x2C       //���ڿ�

#define RC_FAILURE             	0xFF  	  //������������Ĵ��ڴ���                                  
#define RC_ACCESS_NOT_PERFORMED	0xFE  	  //�򿪹���ģʽ�ķ��ʿ���δִ��                             
#define RC_ACCESS_FAILURE      	0xFD  	  //�򿪹���ģʽ�ķ��ʿ��ƴ���                               
#define RC_AUTH_FAILURE       	0xFC  	  //˫����֤ʧ��                                           
#define RC_AUTH_NOT_PERFORMED  	0xFB  	  //δ˫����֤                                               
#define RC_DDA_AUTH_FAILURE    	0xFA  	  //DDA��֤ʧ��                                              
#define RC_INVALID_COMMAND     	0xF9  	  //���������                                                  
#define RC_INVALID_DATA        	0xF8  	  //�����ĵ����������                                       
#define RC_INVALID_PARAM       	0xF7  	  //�޴˲���                                               
#define RC_INVALID_KEYINDEX    	0xF6  	  //��δ����Asession��AEKʱ���ն��������������Msession��Կ   
#define RC_INVALID_SCHEME      	0xF5  	  //��������֧���ն˼���ķ���                                
#define RC_MORE_CARDS          	0xF3  	  //�࿨                                                       
#define RC_NO_CARD             	0xF2  	  //δ��ʾ�ǽӴ���                                         
#define RC_NO_PBOC_TAGS        	0xF1  	  //��������֧�ָñ�ǩ                                          
#define RC_NO_PARAMETER        	0xF0  	  //�޴˲���                                                  
#define RC_POLL_N              	0xEF  	  //������ȷ��Ӧ�𣬶�����δ׼����                           
#define RC_Other_AP_CARDS      	0xEE  	  //������̫�������ҵ�Visa Wave��                            
#define RC_US_CARDS            	0xED  	  //�ϰ������ǽӴ���                                             
#define RC_NO_PIN              	0xEC  	  //δ����PIN                                                   
#define RC_NO_SIG              	0xEB  	  //������δ���ǩ��                                        

//parameter
#define P_MSG_TIMEOUT     0x0001   //���뼶�ĳ�ʱʱ�䣬������������ȱʡʱ���ڶ��ն˷��͵ı��ģ���׼��֧�����ױ��ģ�����Ӧ��Ĭ�ϵ�ֵΪ500ms��0x01F4
#define P_SALE_TIMEOUT    0x0002   //���뼶�ĳ�ʱʱ�䣬���ն˷���׼��֧�����ױ��ĺ�ĳ�ʱ�ȴ�ʱ�䡣ȱʡֵΪ15000ms��0x3A98
#define P_POLL_MSG        0x0003   //�ն˷�����һ��POLL�����Ѽ��������Ƿ���ڵ��뼶��ʱʱ�䡣Ĭ�ϵ�ֵΪ30s��0x001E
#define P_BUF_TIMEOUT     0x0004   //���ն�û����Ӧ��������׼��֧�����ױ��ĵ�Ӧ���ģ��������ָʾ����������������ĵȴ�ʱ�䡣ȱʡֵΪ5000ms��0x1388
#define P_ENCRYPTION      0x0005   //0x00 TDEA�ѽ�ֹ�����ն˺Ͷ�����֮�䶼�������ĵ���ʽ�������ݣ�ֻ���ڲ��Ժ͵��ԡ�?	0x01 TDEA�ѿ���
#define P_DISPLAY         0x0006   //0x00 �ն�Ӧ��ʾ���ж�������Ӧ���룬ֻ���ڲ��Ժ͵�ʽ.	0x01 �ն�Ӧ����������Ӧ����ת�����ʾ��Ϣ����ʾ����
#define P_MAX_BUF_SIZE    0x0007   //�������������ĺ�����ķ�������Ļ�������ȱʡֵΪ1024�ֽڻ�0x0400
#define P_DOUBLE_DIP      0x0008   //��ֵΪ����������ͬһ�ſ�Ƭ�����������ν���֮��ļ��ʱ��.ȱʡֵΪ5000ms��0x1388
#define P_READER_INDEX    0x0009   //������������
#define P_LANGUAGE        0x000A   //�ò���ָ���˶�����֧�ֵ��������ͺ���ʾ��Ϣ����������.��ǩ 5F2D + ���� + �������� + ״̬(01�����00: ��ֹ
#define P_DISPLAY_S_MSG   0x000B   //��ʾ����ʾ��Ϣ�ĺ��뼶��ʱʱ�� ȱʡֵΪ2000ms��0x07D0
#define P_DISPLAY_L_MSG   0x000C   //��ʾ����ʾ��Ϣ�ĺ��뼶��ʱʱ��.ȱʡֵΪ5000ms��0x1388
#define P_DISPLAY_SS_MSG  0x000D   //�ڴ�������ǩ���ĺ��뼶��ʱʱ��.ȱʡֵΪ10000ms��0x2710
#define P_DISPLAY_SR_MSG  0x000E   //���վ���ǩ�ֵĺ��뼶��ʱʱ��.ȱʡֵΪ5000ms��0x1388
#define P_DISPLAY_PIN_MSG 0x000F   //PIN����ĺ��뼶��ʱ�ȴ�ʱ��.ȱʡֵΪ10000ms��0x2710
#define P_DISPLAY_E_MSG   0x0010   //��ʾ������Ϣ�ĺ��뼶��ʱʱ��.ȱʡֵΪ3000ms��0x0BB8


#endif

unsigned char GetXorCheck(unsigned char *, unsigned int );
//s16 PortRecvPack(unsigned char *);
unsigned short Protocol_Pro(unsigned char* ,signed short );
unsigned short Uart_Protocol(void);

#endif
