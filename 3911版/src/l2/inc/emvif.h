/*******************************************************************************
* File Name			 : emvif.h
* Author             : guohonglv
* Date First Issued  : 2010��11��15��14:23:46
* Description        : ��ȫ����qpboctest��Ŀ��ͷ�ļ� 
* ע�⣺             ����һ���includeע����
********************************************************************************/

//#include <sys/ioctl.h> //guohonglv

//#include "SystemFunction.h"

#include "misc.h"

#define uchar unsigned char

#define TAG_NULL_1				0x00	// null tag
#define TAG_NULL_2				0xFF	// null tag

extern uchar WaitKey(unsigned int iTimeInt);

#pragma pack(1) 
typedef struct
{
	unsigned int  nTag;
	unsigned char bOption;
}T_EMVTAG;
#pragma pack() 

/* Values for the WHENCE argument to lseek.  */

#ifndef	_STDIO_H		/* <stdio.h> has the same definitions.  */

# define SEEK_SET	0	/* Seek from beginning of file.  */

# define SEEK_CUR	1	/* Seek from current position.  */

# define SEEK_END	2	/* Seek from end of file.  */

#endif