
/*  ���Ŷ���     */
//sbit SIO1	=P1^2 ;  //ID��IO����
//sbit SCLK1	=P1^7  ; //ID��ͬ��ʱ��SCLK����
//sbit VPP	=P3^3 ; //ID��VPP����
//sbit FUSE	=P1^6 ; //ID��FUSE����
//sbit PGM	=P1^3 ; //ID��PGM����     
//sbit SIO2	=P1^5 ; //SAM��IO����
//
//sbit SIO	=P3^2;
//sbit SCLK	=P3^4;
//
//sbit AB23	=P4^0;//HXW V1.1 2004-10-9 
//sbit AB24	=P4^1;//HXW V1.1 2004-10-9

/* �����õ���ȫ�ֱ���  */
//uchar   bdata  r_w_byte;
//sbit    HBIT=r_w_byte^7;
//sbit    LBIT=r_w_byte^0;
//
//uchar bdata wrdata;
//sbit  LWR=wrdata^0;
//sbit  HWR=wrdata^7;

//cpu card
//bit     logic_mode_id;     //����cpu����ȫ��λ����,�������߼�Լ��
//bit     logic_mode_sam;     //С����cpu����ȫ��λ����
//bit     re_TS_f;
 uint    g_ui_wait;     //��ʱ��T0�õ���ȫ�ֱ���,ÿ�ж�һ�μ�1,ֱ��0
 uchar   re_wait_time, guard_time,TIME;
 uint  G_UI_TIME;    //�ȴ�ʱ�俪�أ�0ʱ�رն�ʱ�жϣ�����ʱʱ��Ϊ5ms��G_UI_TIME
 uint   HAVE_SEND_WT;  //�Ƿ�����WT��������������ڷ���OKʱ�ж��Ƿ���Ҫ��ʱ��

//volatile uchar xdata HC574A_PORT _at_ 0x8000;
//volatile uchar xdata HC574B_PORT _at_ 0x9000;
//
//uchar bdata hc574a_byte;
//sbit AB15	=hc574a_byte^0;
//sbit AB16	=hc574a_byte^1;
//sbit AB17	=hc574a_byte^2;
//sbit AB18	=hc574a_byte^3;
//sbit AB19	=hc574a_byte^4;
//sbit AB20	=hc574a_byte^5;
//HXW V1.1 2004-10-9 sbit BEEP	=hc574a_byte^6;
//HXW V1.1 2004-10-9 sbit LED	=hc574a_byte^7;
//sbit AB21	=hc574a_byte^6;//HXW V1.1 2004-10-9 
//sbit AB22	=hc574a_byte^7;//HXW V1.1 2004-10-9
//
//uchar bdata hc574b_byte;
//sbit ICC_PWR1	=hc574b_byte^0;
////HXW V1.1 2004-10-9 sbit ICC_PWR2	=hc574b_byte^1;
//sbit BEEP	=hc574b_byte^1;//HXW V1.1 2004-10-9
//sbit ICC_RST1	=hc574b_byte^2;
//sbit ICC_RST2	=hc574b_byte^3;
//sbit ICC1_SEL1	=hc574b_byte^4;		//ID��ʱ���źŵĿ�����,0--ON,1--OFF
//sbit ICC1_SEL0	=hc574b_byte^5;		//ID��ʱ���źŵ�ѡ��ˣ�0--�첽ʱ�ӣ�1--ͬ��ʱ��
//sbit ICC2_SEL1	=hc574b_byte^6;		//SAM��ʱ���źŵĿ�����,0--ON,1--OFF
//sbit LED	=hc574b_byte^7;//HXW V1.1 2004-10-9
//


