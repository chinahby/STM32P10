PC0:Ӳ���汾
  adc:
  
2013.06.26(035)
   1)�����flash ����.
   2)û�ùܽų�ʼ����

2013.04.03
1)����Ӧ�ó���󲻻�ɾ����Կ

2013.03.25
1)update boot
  ��v10pupdateboot.bin����v10pboot2.0.bin
2)�޸�stm32f10x_flash.icf
  0x08009000��Ϊ0x8006800
3)���µĿ�v10plib02.a

2013.02.21
  1)��������(Lib_SetDateTime)
2013.02.18
1)int  Lib_PiccCheck(uchar mode,uchar *CardType,uchar *SerialNo)
    mode����ʲôֵ����ˢA��/B��
2013.1.25
  1)�������Ϊ146K
  2)�û�����Ϊ66K
  3)��ʽ�� �����Լ����ȣ�asc�룩 +  ���к�
stm32f10x_flash.icf:0x08009000��Ϊ0x8006800  

2013.1.4
                  //Address:0-(66*1024-1)  size:66k                  
int Lib_AppShareRead(ulong Address,uchar * buff,ulong len);                  
int Lib_AppShareWrite(ulong Address,uchar * buff,ulong len);
