/************************************
*comment:��־������ļ�
*auth:huangyubin
*date:2015.12.30
*************************************/

#ifndef __LOG_EX_H__
#define __LOG_EX_H__

#include <stdarg.h>
#include "def.h"

#define LOG_INFO  1 //������Ϣ
#define LOG_ERROR 2 //������Ϣ

#define _LOG_WRITE_STATE_  //��־���뿪��
#define LOG_LEVEL 1 //��־�������>=LOG_LEVEL



#define LOG_MAXTEXT 2048//һ����־����󳤶�

bool EXPORT_FUNCTION Write_Log(int nLogType, const char *pstrFmt, ...);
bool EXPORT_FUNCTION Write_LogW(int nLogType, TCHAR *pstrFmt, ...);

bool EXPORT_FUNCTION Write_Data(int nLogType, const char *pstrFmt, ...);

#endif
