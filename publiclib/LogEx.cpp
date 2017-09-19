#include "stdafx.h"
#include "CharCode.h"
#include "LogEx.h"
#include "DirFileEx.h"
#include "DateTimeEx.h"

char g_szLogPath[512] = { 0};//

class LogInit
{
public:
	LogInit();
	~LogInit();
};

LogInit::LogInit()
{
	CharCode codePath(GetRunDirW());
	sprintf(g_szLogPath, "%s", codePath.GetStringA());
	Ex_CreateDiretory(g_szLogPath);
}

LogInit::~LogInit()
{
}
static LogInit g_LogInit;

/*********************************************************************
* ��������:void Write_Log(int nLogType, char *pstrFmt, ...)
* ˵��:��־���������֧�ֱ䳤����
* �����ߣ��κ���Ҫд��־�ĵط�
* �������:
* int nLogTypee --  ��־���, LOG_INFO ��ʾ������Ϣ,LOG_ERROR ��ʾ������Ϣ
* char *pstrFmt  --  ��־����
* ...            --  �䳤����
* ���������
* ��
* ����ֵ:
* void  --
* use example:
* Write_Log(0,"%s %d %s", __FILE__, __LINE__, "LogContent");  //only need to call this function to print log
* ע��Ҫ��ӡ����־���Ȳ��ܳ���LOG_MAXTEXT���ֽ�
*********************************************************************/
bool EXPORT_FUNCTION Write_Log(int nLogType, const char *pstrFmt, ...)
{
#ifdef _LOG_WRITE_STATE_ 
	if (nLogType != LOG_INFO && nLogType != LOG_ERROR)//��ջ���ƻ��������쳣��nLogType��Ϊ���ֵ
		return false;

	if (pstrFmt == NULL)
		return false;
	//ȡ����ʽ��������ַ���
	char szLogBuf[LOG_MAXTEXT] = { 0 };
	va_list vArgList;
	va_start(vArgList, pstrFmt);
	_vsnprintf_s(szLogBuf, LOG_MAXTEXT, LOG_MAXTEXT-1, pstrFmt, vArgList);
	va_end(vArgList);

	//��־�ļ�·��+�ļ���
	char szLogPathFile[512] = { 0 };
	//char szData[DATE_TIME_BUF_LEN] = { 0 };
	Ex_DateTime dateTime;
	sprintf_s(szLogPathFile, 512, "%s%s.log", g_szLogPath, dateTime.Ex_GetDate());

	//��־һ�е�����
	char szLogText[LOG_MAXTEXT + 50] = { 0 };

	//׷�ӷ�ʽ���ļ�
	FILE *pFile = NULL;
	pFile = fopen(szLogPathFile, "a+");
	if (NULL == pFile)
	{
		return false;
	}

	//��ӡ�߳�ID
	unsigned long lThreadID = GetCurrentThreadId();

	//char szDataTime[32]={0};
	//strcpy(szDataTime, dateTime.Ex_GetDateTime());
	if (nLogType == LOG_INFO)
	{
		sprintf_s(szLogText, LOG_MAXTEXT + 50, "%s INFO[%08x] %s\r\n", dateTime.Ex_GetDateTime(), lThreadID, szLogBuf);
	}
	else if (nLogType == LOG_ERROR)
	{
		sprintf_s(szLogText, LOG_MAXTEXT + 50, "%s ERROR[%08x] %s\r\n", dateTime.Ex_GetDateTime(), lThreadID, szLogBuf);
	}

	//printf(szLogText);

	fwrite(szLogText, strlen(szLogText), 1, pFile);
	fclose(pFile);

	return true;
#endif _LOG_WRITE_STATE_
}


bool EXPORT_FUNCTION Write_Data(int nLogType, const char *pstrFmt, ...)
{
#ifdef _LOG_WRITE_STATE_ 

	//ȡ����ʽ��������ַ���
	char szLogBuf[LOG_MAXTEXT] = { 0 };
	va_list vArgList;
	va_start(vArgList, pstrFmt);
	_vsnprintf_s(szLogBuf, LOG_MAXTEXT, LOG_MAXTEXT-1, pstrFmt, vArgList);
	va_end(vArgList);

	//��־�ļ�·��+�ļ���
	char szLogPathFile[512] = { 0 };
	Ex_DateTime dateTime;
	sprintf_s(szLogPathFile, 512, "%s%s.data", g_szLogPath, dateTime.Ex_GetDate());

	//��־һ�е�����
	char szLogText[LOG_MAXTEXT + 50] = { 0 };

	//׷�ӷ�ʽ���ļ�
	FILE *pFile = NULL;
	pFile = fopen(szLogPathFile, "a+");
	if (NULL == pFile)
	{
		return false;
	}

	if (nLogType == LOG_INFO)
	{
		sprintf_s(szLogText, LOG_MAXTEXT + 50, "%s INFO %s\r\n", dateTime.Ex_GetDateTime(), szLogBuf);
	}
	else if (nLogType == LOG_ERROR)
	{
		sprintf_s(szLogText, LOG_MAXTEXT + 50, "%s ERROR %s\r\n", dateTime.Ex_GetDateTime(), szLogBuf);
	}

	printf(szLogText);

	fwrite(szLogText, strlen(szLogText), 1, pFile);
	fclose(pFile);

	return true;
#endif _LOG_WRITE_STATE_
}

/*
Write_LogW(LOG_INFO, _T("%s---%s"), CString.GetBuffer(0), CString.GetBuffer(0));//��ȷ�÷�
Write_LogW(LOG_INFO, _T("%s---%s"), CA2W(...), CA2W(...); //�����÷�1��������ʱ�����Ī���Ĵ���
Write_LogW(LOG_INFO, _T("%s---%s"), CString, CString);//�����÷�2
Write_Log(LOG_INFO, "%s---%s", string, string);//�����÷�3
*/
bool EXPORT_FUNCTION Write_LogW(int nLogType, TCHAR *pstrFmt, ...)
{
	if (nLogType != LOG_INFO && nLogType != LOG_ERROR)//��ջ���ƻ��������쳣��nLogType��Ϊ���ֵ
		return false;

	if (pstrFmt == NULL)
		return false;

	int nSize = 0;
	TCHAR wcharLogBuf[LOG_MAXTEXT] = { 0 };


	va_list args;
	va_start(args, pstrFmt);	
	_vsnwprintf(wcharLogBuf, LOG_MAXTEXT, pstrFmt, args);

	va_end(args);

	//wcharLogBuf�д�%s,%d�Ȼᵼ��Write_Log����,���滻
	CString cstrLogBuf = wcharLogBuf;
	cstrLogBuf.Replace(_T("%"), _T("%%"));

	CharCode charLog(cstrLogBuf.GetBuffer(0));
	Write_Log(nLogType, charLog.GetStringA());

	return true;
}