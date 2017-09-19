#include "stdafx.h"
#include "DateTimeEx.h"

Ex_DateTime::Ex_DateTime()
{
	m_PDateTime = new char[DATE_TIME_BUF_LEN];
	memset(m_PDateTime, 0, DATE_TIME_BUF_LEN);

}

Ex_DateTime::~Ex_DateTime()
{
	if (m_PDateTime)
	{
		delete []m_PDateTime;
		m_PDateTime = NULL;
	}
}

//��ȡ��ǰʱ�䣺1999-05-09 16:54:55
char* Ex_DateTime::Ex_GetDateTime()
{
	//SYSTEMTIME st;
	//::GetLocalTime(&st);
	//m_strChatInput.Format(_T("%s %d:%d %0.2d:\r\n%s\r\n\r\n"), m_strUserName, st.wHour, st.wMinute, st.wSecond, m_strChatInput);
	if (m_PDateTime)
		memset(m_PDateTime, 0, DATE_TIME_BUF_LEN);

	struct tm *nowtime;
	time_t lt1;
	time(&lt1);
	nowtime = localtime(&lt1);
	if (nowtime)
	{
		strftime(m_PDateTime, DATE_TIME_BUF_LEN, "%Y-%m-%d %H:%M:%S", nowtime);
		return m_PDateTime;
	}
	else
	{
		return "";
	}
}

int Ex_DateTime::Ex_GetHour()
{
	struct tm *nowtime;
	time_t lt1;
	time(&lt1);

	nowtime = localtime(&lt1);
	if (nowtime)
	{
		return nowtime->tm_hour;
	}
	else
	{
		return 0;
	}
}

//��õ�ǰ���ڣ���ʽ��long,��1970-01-01���������
long Ex_DateTime::Ex_GetDateTimeSecond()
{
	time_t lt1;
	time(&lt1);
	return (long)lt1;
}

//��ȡ��ǰʱ�䣺1999-05-09
char* Ex_DateTime::Ex_GetDate()
{
	if (m_PDateTime)
		memset(m_PDateTime, 0, DATE_TIME_BUF_LEN);

	struct tm *nowtime;
	time_t lt1;
	time(&lt1);
	nowtime = localtime(&lt1);

	if (nowtime)
	{
		strftime(m_PDateTime, DATE_TIME_BUF_LEN, "%Y-%m-%d", nowtime);
		return m_PDateTime;
	}
	else
	{
		return "";
	}

}

//��ȡ��ǰʱ�䣺16:54:55
char* Ex_DateTime::Ex_GetTime()
{
	if (m_PDateTime)
		memset(m_PDateTime, 0, DATE_TIME_BUF_LEN);

	struct tm *nowtime;
	time_t lt1;
	time(&lt1);
	nowtime = localtime(&lt1);

	if (nowtime)
	{
		strftime(m_PDateTime, DATE_TIME_BUF_LEN, "%H:%M:%S", nowtime);
		return m_PDateTime;
	}
	else
	{
		return "";
	}
}

//����ת������ʽ��2015-01-29 15:09:05 ת��Ϊ long
long Ex_DateTime::TimeSwitthToSecond(const char* cszDateTime)
{
	if (cszDateTime == NULL)
	{
		return 0;
	}
	//�ж��ַ����Ƿ�Ϊ��
	if (strlen(cszDateTime) == 0)
	{
		return 0;
	}

	//ʱ���ַ���ת��tm�ṹ��
	tm t;
	memset(&t, 0, sizeof(tm));
	sscanf(cszDateTime, "%d-%d-%d %d:%d:%d", &t.tm_year, &t.tm_mon, &t.tm_mday, &t.tm_hour, &t.tm_min, &t.tm_sec);
	t.tm_year -= 1900;
	t.tm_mon -= 1;//ע�⣬tm_mom��ȡֵ��ΧΪ0-11����Ҫ��1

	//tmת��������(long)
	time_t t1 = 0;
	t1 = mktime(&t);
	return (long)t1;
}

//����ת����long ת��Ϊ ��ʽ��2015-01-29 15:09:05
const char* Ex_DateTime::TimeSwitchToString(time_t lDateTime)
{
	if (lDateTime <= 0)
	{
		return "";
	}

	if (m_PDateTime)
		memset(m_PDateTime, 0, DATE_TIME_BUF_LEN);

	struct tm *tm_time;
	time_t* t1 = (time_t*)&lDateTime;
	tm_time = localtime(t1);
	if (tm_time)
	{
		strftime(m_PDateTime, DATE_TIME_BUF_LEN, "%Y-%m-%d %H:%M:%S\n", tm_time);
		return m_PDateTime;
	}
	else
	{
		return "";
	}
}