
#include "var.h"
#include "stdio.h"

//typedef unsigned short  ushort;
#define ushort unsigned short
extern void lcdshow( int line_num, int place, int clear_line_flag, unsigned char *str );

#if 0
//���ļ��ж�ȡ����ĳ�����¼��ȫ�ֱ���,
uchar ReadReversalData(void)
{
	int		fd;

	fd = open(REVERSAL_FILE, O_RDWR);
	if( fd<0 )
	{
		return E_MEM_ERR;
	}

	if( read(fd, (uchar *)&gtPosCom, COM_SIZE)!=COM_SIZE )
	{
		close(fd);
		return E_MEM_ERR;
	}
	close(fd);

	return OK_MIS;
}

//���������¼��Ҫ��ȫ�ֱ������ļ���
uchar SaveReversalData(void)
{
	int fd;
	int len;

	fd = open(REVERSAL_FILE,  O_RDWR);
	if( fd<0 )
	{
		//creat(REVERSAL_FILE,0777);
		fd = open(REVERSAL_FILE, O_RDWR | O_CREAT | O_SYNC);
		if(fd<0)
			return (E_MEM_ERR);
	}

	len = write(fd, (uchar *) &gtPosCom, COM_SIZE);
	if( len!=COM_SIZE )
	{
		close(fd);
		return (E_MEM_ERR);
	}
	close(fd);

	return (OK_MIS);
}

// called only if EMVProcTrans() failed,kernel�ĳ�����¼tag��ʵ����APP(��BankUnion, JCB)��һ��,����APP���Լ�����
uchar UpdateEmvDupFile(void)
{
	uchar	ucRet;
	int		iLength;
	uchar	sReversalDE55[219];
	extern T_EMVTAG gtReversalTagList[];
	
	// ����ű�������
	SaveScriptResult();
	
	// Issuer approved, but ICC declined!
	if( fexist(REVERSAL_FILE)<0 )
	{
		return OK_MIS;
	}

	memset(sReversalDE55, 0, sizeof(sReversalDE55));
	sReversalDE55[0] = 6;
	ucRet = SetOnlineData(gtReversalTagList, sReversalDE55, &iLength);
	if( ucRet!=OK_MIS )
	{
		return E_TRANS_FAIL;
	}

	ReadReversalData();
	memcpy(gtPosCom.sReversalDE55, sReversalDE55, iLength);
	gtPosCom.iReversalDE55Len = iLength;
	if( gtPosCom.szRespCode[0]==0 || memcmp(gtPosCom.szRespCode, "00", 2)==0 )
	{
		sprintf((char *)gtPosCom.szRespCode, "06"); 
	}
	SaveReversalData();

	return OK_MIS;
}

#endif

// ֻ�����������Ԫ��Tag,�������ṹ/ģ�����Tag
void BuildTLVString(ushort uiEmvTag, uchar *psData, int iLength, uchar **ppsOutData)
{
	uchar	*psTemp;

	// ���Գ���Ϊ0��TAG
	if( iLength<=0 )
	{
		return;
	}

	// ����TAG
	psTemp = *ppsOutData;
	if( uiEmvTag & 0xFF00 )
	{
		*psTemp++ = (uchar)(uiEmvTag >> 8);
	}
	*psTemp++ = (uchar)uiEmvTag;
	
	// ����Length
	if( iLength<=127 )	// Ŀǰ���ݳ��Ⱦ�С��127�ֽ�,���԰���׼���д���
	{
		*psTemp++ = (uchar)iLength;
	}
	else
	{	// EMV�涨���255�ֽڵ�����
		*psTemp++ = 0x81;
		*psTemp++ = (uchar)iLength;
	}
	
	// ����Value
	memcpy(psTemp, psData, iLength);
	psTemp += iLength;
	
	*ppsOutData = psTemp;
}

//port



/* YYYYMMDD HHMMSS*/
void settimebybuf (unsigned char *sDate)
{
	#pragma pack(1) 
	typedef struct
	{
		uchar sYear[2];
		uchar sMon[2];
		uchar sDay[2];
		uchar sHour[2];
		uchar sMin[2];
		uchar sSec[2];

	}T_PART;
	#pragma pack() 
	//T_PART tPart;
	//struct tm sTime;

	//memcpy(&tPart,&sDate[2],
	//sTime.tm_year = 
}


unsigned char DispDateTime()
{
	struct tm  timestru;
	short offset;
	char sDisp[100]; 

	gettime(&timestru);
	//gotoxy(0,5);
	sprintf(sDisp,"%02d:%02d:%02d",timestru.tm_hour,timestru.tm_min,timestru.tm_sec);
	offset = strlen(sDisp)<MAX_COL_M?(MAX_COL_M-strlen(sDisp))/2:0;
	
	//gotoxy(offset,gety());
	//printf("%s\n",sDisp);
	
	lcdshow(1,MIDDLE,1, (unsigned char *)sDisp);
	
	memset(sDisp,0,50);
//	if(timestru.tm_year>49)
//		sprintf(sDisp,"19%02d %02d %02d",timestru.tm_year,timestru.tm_mon,timestru.tm_mday);
//	else
		sprintf(sDisp,"20%02d %02d %02d",timestru.tm_year,timestru.tm_mon,timestru.tm_mday);
	offset = strlen(sDisp)<MAX_COL_M?(MAX_COL_M-strlen(sDisp))/2:0;
	
	//gotoxy(offset,gety());
	//printf(sDisp);

	lcdshow(2,MIDDLE,1, (unsigned char *)sDisp);
    (void)offset;
    return 0;
}



