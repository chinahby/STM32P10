//================================================
#define FISCAL_CARD 0x00
#define USER_CARD   0x01
#define CHECK_CARD  0x02

//˰�ز���������ͳһ�����
//================================================
//get_register_nb �����Ĵ������Ͷ���
#define MOD_GET_REG_NB      0x01
  #define grn_SAM_COMM_ERR          0x0101  //SAM����ͨѶ����
  #define grn_ID_COMM_ERR           0x0102  //ID����ͨѶ����
  #define grn_CMD_LEN_INCORRECT     0x0105  //6700 ���ȴ���
  #define grn_APP_BLOCKED           0x0109  //6985 ʹ�����������㣨Ӧ�ñ�������
  #define grn_REGISTERED_ALREADY    0x0106  //6901 ʹ�����������㣨�Ѿ�ע�ᣬ������ע�ᣩ //INCORRECT_STATUS
  #define grn_INEXISTENT_FILE       0x010B  //6A82 �ļ�û�ҵ�
  #define grn_P1_P2_INCORRECT       0x010C  //6A86 P1��P2����ȷ
  #define grn_INS_INCORRECT         0x010E  //6D00 INS����ȷ
  #define grn_CLA_INCORRECT         0x010F  //6E00 CLA����ȷ
  #define grn_APP_BLOCKED_FOREVER   0x0111  //9303 Ӧ�ñ���������
  #define grn_GET_RESPONSE_ERR      0x0120  //ȡ��Ӧ���ݴ���
  #define grn_OTHER_ERR             0x0121  //��������
  
//terminal_register �����Ĵ������Ͷ���
#define MOD_TERMINAL_REG    0x02
  #define tr_SAM_COMM_ERR           0x0201  //SAM����ͨѶ����
  #define tr_ID_COMM_ERR            0x0202  //ID����ͨѶ����
  #define tr_CMD_LEN_INCORRECT      0x0205  //6700 ���ȴ���
  #define tr_APP_BLOCKED            0x0209  //6985 ʹ�����������㣨Ӧ�ñ�������
  #define tr_REGISTERED_ALREADY     0x0206  //6901 ʹ�����������㣨�Ѿ�ע�ᣬ������ע�ᣩ //INCORRECT_STATUS
  #define tr_INEXISTENT_FILE        0x020B  //6A82 �ļ�û�ҵ�
  #define tr_P1_P2_INCORRECT        0x020C  //6A86 P1��P2����ȷ
  #define tr_INS_INCORRECT          0x020E  //6D00 INS����ȷ
  #define tr_CLA_INCORRECT          0x020F  //6E00 CLA����ȷ
  #define tr_APP_BLOCKED_FOREVER    0x0211  //9303 Ӧ�ñ���������
  #define tr_GET_RESPONSE_ERR       0x0220  //ȡ��Ӧ���ݴ���
  #define tr_OTHER_ERR              0x0221  //��������
  
//issue_invoice �����Ĵ������Ͷ���
#define MOD_ISSUE_INVOICE   0x03
  #define ii_SAM_COMM_ERR           0x0301  //SAM����ͨѶ����
  #define ii_ID_COMM_ERR            0x0302  //ID����ͨѶ����
  #define ii_CMD_LEN_INCORRECT      0x0305  //6700 ���ȴ���
  #define ii_APP_BLOCKED            0x0309  //6985 ʹ�����������㣨һ�㲻��Ӧ�ñ�������
  #define ii_NOT_REGISTERED         0x0306  //6901 ����������ִ����������û��ע�ᣩ
  #define ii_CMD_DATA_INCORRECT     0x030A  //6A80 ���ݲ�������ȷ�������ڡ�˰��˰Ŀ����Ʊ�ţ���ӡ������ȣ�
  #define ii_INEXISTENT_FILE        0x030B  //6A82 �ļ�û�ҵ�
  #define ii_P1_P2_INCORRECT        0x030C  //6A86 P1��P2����ȷ
  #define ii_INEXISTENT_REFERENCE   0x030D  //6A88 ��������δ�ҵ�
  #define ii_INS_INCORRECT          0x030E  //6D00 INS����ȷ
  #define ii_CLA_INCORRECT          0x030F  //6E00 CLA����ȷ
  #define ii_APP_BLOCKED_FOREVER    0x0311  //9303 Ӧ�ñ���������
  #define ii_EXCEED_MAX_MONEY       0x0312  //9401 ����
  #define ii_KEY_VER_NOT_SUPPORT    0x0313  //9403 ��Կ�汾��֧��
  #define ii_GET_RESPONSE_ERR       0x0320  //ȡ��Ӧ���ݴ���
  #define ii_OTHER_ERR              0x0321  //��������

//declare_duty �����Ĵ������Ͷ���
#define MOD_DECLARE_DUTY    0x04
  #define dd_SAM_COMM_ERR           0x0401  //SAM����ͨѶ����
  #define dd_ID_COMM_ERR            0x0402  //ID����ͨѶ����
  #define dd_CARD_MEMORY_ERR        0x0404  //6581 �ڴ����
  #define dd_CMD_LEN_INCORRECT      0x0405  //6700 ���ȴ���
  #define dd_APP_BLOCKED            0x0409  //6985 ʹ�����������㣨Ӧ�ñ�������
  #define dd_NOT_REGISTERED         0x0406  //6901 ����������ִ����������û��ע��ȣ�
  #define dd_CMD_DATA_INCORRECT     0x040A  //6A80 ���ݲ�������ȷ�����걨���ڲ��Եȣ�
  #define dd_INEXISTENT_FILE        0x040B  //6A82 �ļ�û�ҵ�
  #define dd_P1_P2_INCORRECT        0x040C  //6A86 P1��P2����ȷ
  #define dd_INEXISTENT_REFERENCE   0x040D  //6A88 ��������δ�ҵ�
  #define dd_INS_INCORRECT          0x040E  //6D00 INS����ȷ
  #define dd_CLA_INCORRECT          0x040F  //6E00 CLA����ȷ
  #define dd_APP_BLOCKED_FOREVER    0x0411  //9303 Ӧ�ñ���������
  #define dd_KEY_VER_NOT_SUPPORT    0x0413  //9403 ��Կ�汾��֧��
  #define dd_DATA_XOR_ERR           0x041F  //���յ����ݿ����У��ʹ���
  #define dd_GET_RESPONSE_ERR       0x0420  //ȡ��Ӧ���ݴ���
  #define dd_OTHER_ERR              0x0421  //��������
  
//update_control �����Ĵ������Ͷ���
#define MOD_UPDATE_CTRL     0x05
  #define uc_SAM_COMM_ERR           0x0501  //SAM����ͨѶ����
  #define uc_ID_COMM_ERR            0x0502  //ID����ͨѶ����
  #define uc_CARD_MEMORY_ERR        0x0504  //6581 �ڴ����
  #define uc_CMD_LEN_INCORRECT      0x0505  //6700 ���ȴ���
  #define uc_APP_BLOCKED            0x0509  //6985 ʹ�����������㣨Ӧ�ñ�������
  #define uc_NOT_REGISTERED         0x0506  //6901 ����������ִ����������û��ע��ȣ�
  #define uc_CMD_DATA_INCORRECT     0x050A  //6A80 ���ݲ�������ȷ
  #define uc_INEXISTENT_FILE        0x050B  //6A82 �ļ�û�ҵ�
  #define uc_P1_P2_INCORRECT        0x050C  //6A86 P1��P2����ȷ
  #define uc_INS_INCORRECT          0x050E  //6D00 INS����ȷ
  #define uc_CLA_INCORRECT          0x050F  //6E00 CLA����ȷ
  #define uc_MAC_INCORRECT          0x0510  //9302 MAC����
  #define uc_APP_BLOCKED_FOREVER    0x0511  //9303 Ӧ�ñ���������
  #define uc_KEY_VER_NOT_SUPPORT    0x0513  //9403 ��Կ�汾��֧��
  #define uc_OTHER_ERR              0x0521  //��������
  
//input_invoice_nb �����Ĵ������Ͷ���
#define MOD_INPUT_INVO_NB   0x06
  #define iin_SAM_COMM_ERR          0x0601  //SAM����ͨѶ����
  #define iin_ID_COMM_ERR           0x0602  //ID����ͨѶ����
  #define iin_CARD_MEMORY_ERR       0x0604  //6581 �ڴ����
  #define iin_CMD_LEN_INCORRECT     0x0605  //6700 ���ȴ���
  #define iin_APP_BLOCKED           0x0609  //6985 ʹ�����������㣨Ӧ�ñ���������Ʊ�洢�������ȣ�
  #define iin_NOT_REGISTERED        0x0606  //6901 ����������ִ����������û��ע��ȣ�
  #define iin_LOW_SAFETY_LEVEL      0x0607  //6982 ��ȫ״̬������
  #define iin_CMD_DATA_INCORRECT    0x060A  //6A80 ���ݲ�������ȷ
  #define iin_INEXISTENT_FILE       0x060B  //6A82 �ļ�û�ҵ�
  #define iin_P1_P2_INCORRECT       0x060C  //6A86 P1��P2����ȷ
  #define iin_INS_INCORRECT         0x060E  //6D00 INS����ȷ
  #define iin_CLA_INCORRECT         0x060F  //6E00 CLA����ȷ
  #define iin_APP_BLOCKED_FOREVER   0x0611  //9303 Ӧ�ñ���������
  #define iin_OTHER_ERR             0x0621  //��������

//verify_fiscal_pin �����Ĵ������Ͷ���
#define MOD_VERIFY_FSC_PIN  0x07
  #define vfp_SAM_COMM_ERR          0x0701  //SAM����ͨѶ����
  #define vfp_ID_COMM_ERR           0x0702  //ID����ͨѶ����
  #define vfp_FISCAL_PIN_ERR        0x0703  //63CX У��ʧ�ܣ�X��ʾ�������ԵĴ���
  #define vfp_CMD_LEN_INCORRECT     0x0705  //6700 ���ȴ���
  #define vfp_LOW_SAFETY_LEVEL      0x0707  //6982 ��ȫ״̬������
  #define vfp_PIN_BLOCKED           0x0708  //6983 ��֤����(PIN)����
  #define vfp_INEXISTENT_REFERENCE  0x070D  //6A88 ��Կδ�ҵ�
  #define vfp_GET_RESPONSE_ERR      0x0720  //ȡ��Ӧ���ݴ���
  #define vfp_OTHER_ERR             0x0721  //��������

//daily_collect_sign �����Ĵ������Ͷ���
#define MOD_DAILY_COLLECT   0x08
  #define dcs_SAM_COMM_ERR          0x0801  //SAM����ͨѶ����
  #define dcs_ID_COMM_ERR           0x0802  //ID����ͨѶ����
  #define dcs_CMD_LEN_INCORRECT     0x0805  //6700 ���ȴ���
  #define dcs_APP_BLOCKED           0x0809  //6985 ʹ�����������㣨Ӧ�ñ�������
  #define dcs_NOT_REGISTERED        0x0806  //6901 ����������ִ����������û��ע��ȣ�
  #define dcs_CMD_DATA_INCORRECT    0x080A  //6A80 ���ݲ�������ȷ�������ڲ��ԡ�����У�����ȣ�
  #define dcs_INEXISTENT_FILE       0x080B  //6A82 �ļ�û�ҵ�
  #define dcs_P1_P2_INCORRECT       0x080C  //6A86 P1��P2����ȷ
  #define dcs_INS_INCORRECT         0x080E  //6D00 INS����ȷ
  #define dcs_CLA_INCORRECT         0x080F  //6E00 CLA����ȷ
  #define dcs_APP_BLOCKED_FOREVER   0x0811  //9303 Ӧ�ñ���������
  #define dcs_SIGN_TYPE_ERR         0x081E  //����ǩ�ֵ��������ʹ���
  #define dcs_DATA_XOR_ERR          0x081F  //���յ����ݿ����У��ʹ���
  #define dcs_GET_RESPONSE_ERR      0x0820  //ȡ��Ӧ���ݴ���
  #define dcs_OTHER_ERR             0x0821  //��������
  
//register_sign �����Ĵ������Ͷ���
#define MOD_REGISTER_SIGN   0x09
  #define rs_SAM_COMM_ERR           0x0901  //SAM����ͨѶ����
  #define rs_ID_COMM_ERR            0x0902  //ID����ͨѶ����
  #define rs_CARD_MEMORY_ERR        0x0904  //6581 �ڴ����
  #define rs_CMD_LEN_INCORRECT      0x0905  //6700 ���ȴ���
  #define rs_APP_BLOCKED            0x0909  //6985 ʹ�����������㣨Ӧ�ñ�������
  #define rs_CMD_DATA_INCORRECT     0x090A  //6A80 ���ݲ�������ȷ��˰�ؿ���Ŵ���ȣ�
  #define rs_INEXISTENT_FILE        0x090B  //6A82 �ļ�û�ҵ�
  #define rs_P1_P2_INCORRECT        0x090C  //6A86 P1��P2����ȷ
  #define rs_INEXISTENT_REFERENCE   0x090D  //6A88 ��������δ�ҵ�
  #define rs_INS_INCORRECT          0x090E  //6D00 INS����ȷ
  #define rs_CLA_INCORRECT          0x090F  //6E00 CLA����ȷ
  #define rs_APP_BLOCKED_FOREVER    0x0911  //9303 Ӧ�ñ���������
  #define rs_KEY_VER_NOT_SUPPORT    0x0913  //9403 ��Կ�汾��֧��
  #define rs_GET_RESPONSE_ERR       0x0920  //ȡ��Ӧ���ݴ���
  #define rs_OTHER_ERR              0x0921  //��������

//data_collect �����Ĵ������Ͷ���
#define MOD_DATA_COLLECT    0x0A
  #define udc_SAM_COMM_ERR          0x0A01  //SAM����ͨѶ����
  #define udc_ID_COMM_ERR           0x0A02  //ID����ͨѶ����
  #define udc_CARD_MEMORY_ERR       0x0A04  //6581 �ڴ����
  #define udc_CMD_LEN_INCORRECT     0x0A05  //6700 ���ȴ���
  #define udc_APP_BLOCKED           0x0A09  //6985 ʹ�����������㣨Ӧ�ñ�������
  #define udc_COLLECTED_ALREADY     0x0A06  //6901 ����������ִ�����������Ѿ����ܵȣ�
  #define udc_CMD_DATA_INCORRECT    0x0A0A  //6A80 ���ݲ�������ȷ����˰�ؿ���Ŵ���
  #define udc_INEXISTENT_FILE       0x0A0B  //6A82 �ļ�û�ҵ�
  #define udc_P1_P2_INCORRECT       0x0A0C  //6A86 P1��P2����ȷ
  #define udc_INS_INCORRECT         0x0A0E  //6D00 INS����ȷ
  #define udc_CLA_INCORRECT         0x0A0F  //6E00 CLA����ȷ
  #define udc_MAC_INCORRECT         0x0A10  //9302 MAC����
  #define udc_APP_BLOCKED_FOREVER   0x0A11  //9303 Ӧ�ñ���������
  #define udc_KEY_VER_NOT_SUPPORT   0x0A13  //9403 ��Կ�汾��֧��
  #define udc_OTHER_ERR             0x0A21  //��������

//distribute_invoice_nb �����Ĵ������Ͷ���
#define MOD_DISTRIBUTE      0x0B
  #define ud_SAM_COMM_ERR           0x0B01  //SAM����ͨѶ����
  #define ud_ID_COMM_ERR            0x0B02  //ID����ͨѶ����
  #define ud_CARD_MEMORY_ERR        0x0B04  //6581 �ڴ����
  #define ud_CMD_LEN_INCORRECT      0x0B05  //6700 ���ȴ���
  #define ud_APP_BLOCKED            0x0B09  //6985 ʹ�����������㣨Ӧ�ñ������ȣ�
  #define ud_INEXISTENT_FILE        0x0B0B  //6A82 �ļ�û�ҵ�
  #define ud_P1_P2_INCORRECT        0x0B0C  //6A86 P1��P2����ȷ
  #define ud_INS_INCORRECT          0x0B0E  //6D00 INS����ȷ
  #define ud_CLA_INCORRECT          0x0B0F  //6E00 CLA����ȷ
  #define ud_APP_BLOCKED_FOREVER    0x0B11  //9303 Ӧ�ñ���������
  #define ud_OUT_OF_INVOICE         0x0B12  //9401 �޷�Ʊ�ַ�
  #define ud_GET_RESPONSE_ERR       0x0B20  //ȡ��Ӧ���ݴ���
  #define ud_OTHER_ERR              0x0B21  //��������
//================================================

//verify_pin �����Ĵ������Ͷ���
#define MOD_VERIFY_PIN      0x0C
  #define uvp_SAM_COMM_ERR          0x0C01
  #define uvp_ID_COMM_ERR           0x0C02  //VERIFY_PIN(USER_CARD)ʱͨѶ����ID��
  #define uvp_USER_PIN_ERR          0x0C03  //63CX У��ʧ�ܣ�X��ʾ�������ԵĴ���
  #define uvp_CMD_LEN_INCORRECT     0x0C05  //6700 ���ȴ���
  #define uvp_KEY_TYPE_ERR          0x0C14  //6981 ��Կ���ʹ���
  #define uvp_LOW_SAFETY_LEVEL      0x0C07  //6982 ��ȫ״̬������
  #define uvp_PIN_BLOCKED           0x0C08  //6983 ��֤����(PIN)����
  #define uvp_INEXISTENT_REFERENCE  0x0C0D  //6A88 ��Կδ�ҵ�
  #define uvp_OTHER_ERR             0x0C21  //��������
//================================================

//get_response �����Ĵ������Ͷ���
#define MOD_GET_RESPONSE    0x20
  #define resp_SAM_COMM_ERR     0x2001//GET_RESPONSEʱSAM����ͨѶ����
  #define resp_ID_COMM_ERR      0x2002//GET_RESPONSEʱID����ͨѶ����
  //#define resp_HAS_MORE         0x2000//61XX ����'XX'���ݿɷ���
  #define resp_LEN_ERR          0x2005//6700 ���ȴ���Lc���ڿ���Ӧ�����ݳ��ȣ�
  #define resp_ACTUAL_LEN       0x2019//6CXX ���ȴ���Le����ȷ��'XX'��ʾʵ�ʳ��ȣ�
  #define resp_NO_RESPONSE      0x201B//6F00 ���������ݷ���
  #define resp_OTHER_ERR        0x2021//��������

//get_challenge �����Ĵ������Ͷ���
#define MOD_GET_CHALLENGE   0x21
  #define rand_SAM_COMM_ERR     0x2101//GET_CHALLENGEʱSAM����ͨѶ����
  #define rand_ID_COMM_ERR      0x2102//GET_CHALLENGEʱID����ͨѶ����
  #define rand_LEN_ERR          0x2105//6700 ���ȴ���
  #define rand_NOT_SUPPORT      0x2117//6A81 ��֧�ִ˹���
  #define rand_P1_P2_ERR        0x210C//6A86 ����P1-P2����ȷ
  #define rand_OTHER_ERR        0x2121//��������

//sel_file_by_id �����Ĵ������Ͷ���
#define MOD_SEL_FILE_BY_ID  0x22
  #define sel_id_SAM_COMM_ERR   0x2201//SEL_FILE_BY_IDʱSAM����ͨѶ����
  #define sel_id_ID_COMM_ERR    0x2202//SEL_FILE_BY_IDʱID����ͨѶ����
  #define sel_id_NOT_SUPPORT    0x2217//6A81 ��֧�ִ˹��ܣ���MF��Ӧ��������
  #define sel_INEXISTENT_FILE   0x220B//6A82 δ�ҵ��ļ�
  #define sel_id_P1_P2_ERR      0x220C//6A86 ����P1-P2����ȷ
  #define sel_id_OTHER_ERR      0x2221//��������

//rd_record �����Ĵ������Ͷ���
#define MOD_RD_RECORD       0x23
  #define rd_rec_SAM_COMM_ERR   0x2301//RD_RECORDʱSAM����ͨѶ����
  #define rd_rec_ID_COMM_ERR    0x2302//RD_RECORDʱID����ͨѶ����
  #define rd_rec_NOT_REC_FILE   0x2314//6981 �ļ����ʹ���
  #define rd_rec_LOW_SAFETY_L   0x2307//6982 ��ȫ״̬������
  #define rd_rec_CANNT_EXEC     0x2315//6986 ����������ִ���������޵�ǰEF�ļ���
  #define rd_rec_SAFE_MSG_ERR   0x2316//6988 ��ȫ�����������ȷ
  #define rd_rec_NOT_SUPPORT    0x2317//6A81 ��֧�ִ˹���
  #define rd_rec_NO_FILE        0x230B//6A82 δ�ҵ��ļ�
  #define rd_rec_NOT_FOUND      0x2318//6A83 δ�ҵ���¼
  #define rd_rec_ACTUAL_LEN     0x2319//6CXX ���ȴ���Le����ȷ��'XX'��ʾʵ�ʳ��ȣ�
  #define rd_rec_RESPONSE_ERR   0x2320//RD_RECORDʱGET_RESPONSE����
  #define rd_rec_OTHER_ERR      0x2321//��������

//rd_binary �����Ĵ������Ͷ���
#define MOD_RD_BINARY       0x24
  #define rd_bin_SAM_COMM_ERR   0x2401//RD_BINERYʱSAM����ͨѶ����
  #define rd_bin_ID_COMM_ERR    0x2402//RD_BINERYʱID����ͨѶ����
  #define rd_bin_NOT_BIN_FILE   0x2414//6981 ���Ƕ������ļ�
  #define rd_bin_LOW_SAFETY_L   0x2407//6982 ��ȫ״̬������
  #define rd_bin_SAFE_MSG_ERR   0x2416//6988 ��ȫ�����������ȷ
  #define rd_bin_NOT_SUPPORT    0x2417//6A81 ��֧�ִ˹���
  #define rd_bin_NO_FILE        0x240B//6A82 δ�ҵ��ļ�
  #define rd_bin_PARA_ERR       0x241A//6B00 ��������ƫ�Ƶ�ַ�������ļ����ȣ�
  #define rd_bin_ACTUAL_LEN     0x2419//6CXX ���ȴ���Le����ȷ��'XX'��ʾʵ�ʳ��ȣ�
  #define rd_bin_RESPONSE_ERR   0x2420//RD_BINERYʱGET_RESPONSE����
  #define rd_bin_OTHER_ERR      0x2421//��������

//update_record �����Ĵ������Ͷ���
#define MOD_UPDATE_RECORD   0x25
  #define updt_SAM_COMM_ERR     0x2501//UPDATE_RECORDʱSAM����ͨѶ����
  #define updt_ID_COMM_ERR      0x2502//UPDATE_RECORDʱID����ͨѶ����
  #define updt_CARD_MEM_ERR     0x2504//6581 дEEPROMʧ��
  #define updt_LEN_ERR          0x2505//6700 ���ȴ���
  #define updt_NOT_REC_FILE     0x2514//6981 ��ǰ�ļ����Ǽ�¼�ļ�
  #define updt_LOW_SAFETY_L     0x2507//6982 ��ȫ״̬������
  #define updt_SAFE_MSG_ERR     0x2516//6988 ��ȫ�����������ȷ
  #define updt_NOT_SUPPORT      0x2517//6A81 ��֧�ִ˹���(��MF��MF������)
  #define updt_NO_FILE          0x250B//6A82 δ�ҵ��ļ�
  #define updt_NOT_FOUND        0x2518//6A83 δ�ҵ���¼
  #define updt_LACK_SPACE       0x251C//6A84 �ļ��洢�ռ䲻��
  #define updt_OTHER_ERR        0x2521//��������

//internal_auth �����Ĵ������Ͷ���
#define MOD_INTERNAL_AUTH   0x26
  #define in_au_SAM_COMM_ERR    0x2601//INTERNAL_AUTHʱSAM����ͨѶ����
  #define in_au_ID_COMM_ERR     0x2602//INTERNAL_AUTHʱID����ͨѶ����
  #define in_au_LC_ERR          0x2605//6700 ����Lc����ȷ
  #define in_au_INVALID_STAT    0x2606//6901 ״̬��Ч
  #define in_au_LOW_SAFETY_L    0x2607//6982 ��ȫ״̬������
  #define in_au_CANT_USE_METH   0x2609//6985 ʹ������������
  #define in_au_NO_FILE         0x260B//6A82 δ�ҵ��ļ�
  #define in_au_NO_KEY          0x260D//6A88 ��Կδ�ҵ�
  #define in_au_RESPONSE_ERR    0x2620//INTERNAL_AUTHʱGET_RESPONSE����
  #define in_au_OTHER_ERR       0x2621//��������

//external_auth �����Ĵ������Ͷ���
#define MOD_EXTERNAL_AUTH   0x27
  #define ex_au_SAM_COMM_ERR    0x2701//EXTERNAL_AUTHʱSAM����ͨѶ����
  #define ex_au_ID_COMM_ERR     0x2702//EXTERNAL_AUTHʱID����ͨѶ����
  #define ex_au_AUTH_FAIL       0x2703//63CX ��֤ʧ�ܣ�X��ʾ�������ԵĴ���
  #define ex_au_CARD_MEM_ERR    0x2704//6581 дEEPROMʧ��
  #define ex_au_LEN_ERR         0x2705//6700 ���ȴ���
  #define ex_au_INVALID_STAT    0x2706//6901 ״̬��Ч
  #define ex_au_LOW_SAFETY_L    0x2707//6982 ��ȫ״̬������
  #define ex_au_METHOD_LOCKED   0x2708//6983 ��֤��������
  #define ex_au_CANT_USE_METH   0x2709//6985 ʹ������������
  #define ex_au_NO_FILE         0x270B//6A82 δ�ҵ��ļ�
  #define ex_au_NO_KEY          0x270D//6A88 ��Կδ�ҵ�
  #define ex_au_OTHER_ERR       0x2721//��������
//================================================


//0x00e1-0x00e9��Ϊ�ײ�ͨѶ����ֵ
//================================================
#define TIMEOUT_ERR         0x00E1//�ײ�ͨѶ ��ʱ����
#define RECE_PARITY_ERR     0x00E2//�ײ�ͨѶ ��������ʱ��żУ�����
#define SEND_PARITY_ERR     0x00E3//�ײ�ͨѶ ��������ʱ��żУ�����
#define NOT_CPU_CARD_ERR    0x00E4//�ײ�ͨѶ ��CPU��
#define ATR_LEN_ERR         0x00E5//�ײ�ͨѶ ATR���ݳ��ȴ���
#define ATR_XOR_ERR         0x00E6//�ײ�ͨѶ ATR����У�����
#define ISO_MODE_ERR        0x00E7//�ײ�ͨѶ CPU�����ʽ�ֽڴ��󣬼�����ȷ��ISO-MODEֵ
#define ISO_ACK_ERR         0x00E8//�ײ�ͨѶ CPU��Ӧ���ֽڴ���
#define hdw_CARD_TYPE_ERR   0x00E9//�ײ�ͨѶ����Ŀ����ʹ���

//����Ӧ�ò������
//================================================
#define SAM_ATR_ERR         0x1C01//SAM������λ����
#define ID_ATR_ERR          0x1C02//ID������λ����
#define CARD_TYPE_ERR       0x1C03//���Ǳ���������Ŀ�����
#define ea_SEL_FILE_ERR     0x1C04//����˰��Ӧ��ʱѡ���ļ�����
#define REGISTERED_ALREADY  0x1C05//׼��ע��ʱ�����Ѿ�ע��
#define NOT_REGISTERED      0x1C06//У�����ʱ������δע��
#define NO_UPDATE_INF       0x1C07//û�б������ļ�ع��������Ϣ
#define WRONG_TAX_PAYER_NO  0x1C08//˰���������˰�˴����˰�ؿ��в�һ��
#define ISSUE_FAIL          0x1C09//����Ʊʧ�ܣ�˰�ؿ�����˰���룬������δȡ��˰���룩
#define MACH_NO_INCORRECT   0x1C0A//У�������ʼ������ʱ���ֿ��л�����ź�˰�ػ��в�һ��

#define DATA_LEN_ERR        0x1C0B//ȡ��Ӧ����ʱ���ݳ��Ⱥ�Ԥ�ڲ���
#define RANDOM_LEN_ERR      0x1C0C//ȡ�����ʱ���ݳ��Ⱥ�Ԥ�ڲ���
#define SEL_FILE_ERR        0x1C0D//ѡ���ļ�����
#define NO_MATCHES          0x1C0E//�û�����û�д�̨�����ķ�Ʊ�洢�ļ���ʶ��Ϣ

#define NO_CARD_ERR         0x1C0F//�û�����û�в忨
#define WRONG_USER_CARD     0x1C10//���ɱ�˰����ʱ�����û�����˰�ؿ�����Ӧ
//================================================
    
//����汾�ṹ����
#define SOFTWARE_VER_LEN       18
typedef struct
{
  uint  ui_mach_type;                    //�����ͺ�
  uchar area_code[2];                    //����
  uchar major_ver;                       //���汾��
  uchar minor_ver;                       //���汾��
  uchar industry;                        //��ҵ����
  uchar modi_datetime[6];                //�������޸�ʱ��
  uchar modifier[4];                     //�޸��˼��
  uchar reserved;                        //Ԥ��
}struct_software_ver;
//�����������ݽṹ����
#define PRODUCT_LEN          57

typedef struct
{
  uchar mach_no[8];	                     //�������BCD
  uchar produce_date[4];                 //��������BCD
  uchar hardware_ver[18];                //Ӳ���汾��ASC
  struct_software_ver  st_software_ver;  //����汾��
  uchar update_datetime[6];              //�����������ʱ��
  uchar ini_flag;                        //������ʼ��ʱ��
  uint  ui_crc;                          //CRCУ���
}struct_product;

/////	
#define  	FCARD_MAX_LEN		264     //flash��һ�ζ�д����󳤶�
#define LOAD  0X00
//Ӧ������
#define OK                       1       //������ִ�е�Ӧ��
#define ER                       4       //������ִ�е�Ӧ��

/**********COMMAND DEFINE  *************/
#define SET_RTC   			0x90	
#define TEST_SRAM			0x91				
#define TEST_RTC   			0x92	
#define TEST_FLASH  		0x93	
#define TEST_EEPROM  		0x94
#define TEST_FSC_CARD  		0x96    //����С����(����˰�ؿ�)	 
#define TEST_USER_CARD  	0x97    //���Դ���
#define WRITE_MACH_NO 0x98    //д������������
#define READ_PRODUCT_INF  	0x99    //��������������
#define BAT_CHANGE  	0x9A    //����л���� HXW V1.1 2004-10-12
#define BAT_CHECK  	0x9B    //��ع����� HXW V1.1 2004-10-12
#define TEST  	0x9F    

#define CPU_CARD  			0x00
#define FLASH_CARD  	    0x01
#define SLE4442_CARD  		0x02
#define SLE4428_CARD  		0x03
	

/**********ERROR DEFINE  *************/
#define  TIME_ERR  				0x10	//ʱ�����ݸ�ʽ����
#define  WR_USER_CARD_ERR       0X11 	//д�û����쳣
#define  NOCARD_ERR  			0x12  	//������û���		
#define  FLASH_ERR     			0x13	//flash�쳣
#define  RD_USER_CARD_ERR       0X14    //���û����쳣
#define  FSC_CARD_RST_ERR       0X15    //˰�ؿ���λ�쳣
#define  RD_FSC_CARD_ERR        0X16    //��˰�ؿ��쳣
#define  USER_CARD_RST_ERR      0X17    //�û�����λ�쳣
#define  EEPROM_ERR         	0X18    //eeprom�쳣	
#define  SRAM_ERR         	    0X19    //SRAM�쳣
#define  WRITE_RTC_ERR        	0X1a    //����ʱ���쳣
#define  PACK_HEAD_ERR     	    0x1b    //�����ͷ����
#define  PACK_LENGTH_ERR    	0x1c    //��������ȴ���
#define  CHK_SUM_ERR            0x1d    //�����У�����
#define  INS_ERR             0x1e      //ָ����Դ���
#define  RS232_ERR             0x1f      //����ͨѶ����
#define  COMMAND_ERR        	0x50    //��Ч������
  

