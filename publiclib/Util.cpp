
#include "stdafx.h"
#include "util.h"
#include "Tlhelp32.h"


#ifdef _WIN32
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")
#endif



unsigned long Ex_GetTickCount(void)
{
#ifdef WIN32
	//return timeGetTime();//GetTickCount();//����  //[bug�����ܻᳬ����int��ʾ��Χ��docIDΪ�������л��ĵ�ʧ��]

	SYSTEMTIME st;
	CStringA strDate;
	GetLocalTime(&st);
	strDate.Format("%02d%02d%02d%03d", (unsigned int)st.wHour, (unsigned int)st.wMinute, (unsigned int)st.wSecond, (unsigned int)st.wMilliseconds);
	return atoi(strDate.GetBuffer(0));
#else
	struct timeval now;
	gettimeofday(&now,NULL);
	return now.tv_sec*1000+now.tv_usec/1000; 
#endif
}

unsigned long GenerateSSRC(void)
{
#ifdef WIN32
	LARGE_INTEGER frequence, privious;
	if(!QueryPerformanceFrequency( &frequence))//ȡ�߾������м�������Ƶ��
	{
		return timeGetTime();//GetTickCount();
	}

	if (!QueryPerformanceCounter( &privious ))
	{
		return timeGetTime();//GetTickCount();
	}

	DWORD dwRet = (DWORD)(1000000 * privious.QuadPart / frequence.QuadPart ); //���㵽΢����

	return dwRet;//΢��
#else
	struct timeval now;
	gettimeofday(&now,NULL);
	return now.tv_sec*1000+now.tv_usec; 
#endif
}

void EX_Sleep(unsigned long ulMS)
{
#ifdef WIN32
	Sleep(ulMS);
#else
	usleep(ulMS*1000);
#endif
}

void EX_Delay(unsigned long ulMS)
{
#ifdef WIN32
	Sleep(ulMS);
	//LARGE_INTEGER frequence, privious, current, interval;
	//if(!QueryPerformanceFrequency( &frequence))
	//{
	//	Sleep(ulMS);
	//	return;
	//}

	//interval.QuadPart = frequence.QuadPart * ulMS / 1000000;
	//QueryPerformanceCounter( &privious );
	//current = privious;
	//while( current.QuadPart - privious.QuadPart < interval.QuadPart )
	//	QueryPerformanceCounter( &current);
	return;
#else
	int fd=-1;
	unsigned long data=0;
	int tmp=1024;
	struct timeval start, now;

	gettimeofday(&start, NULL);

	fd = open ("/dev/rtc", O_RDONLY);
	if (fd==-1)
	{
		usleep(1);
		return;
	}

	/* Read periodic IRQ rate */
	if (-1==ioctl(fd, RTC_IRQP_READ, &tmp))
	{
		usleep(0);
		close(fd);
		return;
	}

	/* The frequencies 128Hz, 256Hz, ... 8192Hz are only allowed for root. */
	tmp=1024;

	if (-1==ioctl(fd, RTC_IRQP_SET, tmp))
	{
		usleep(0);
		close(fd);
		return;
	}

	if (-1 == ioctl(fd, RTC_PIE_ON, 0))
	{
		usleep(0);
		close(fd);
		return;
	}

	while (true)
	{
		/* This blocks */
		read(fd, &data, sizeof(unsigned long));
		
		gettimeofday(&now, NULL);

		if (now.tv_sec*1000+now.tv_usec/1000-(start.tv_sec*1000+start.tv_usec/1000)>=ulMS)
			break;
	}

	/* Disable periodic interrupts */
	ioctl(fd, RTC_PIE_OFF, 0);
	
	close(fd);
#endif
}

/*
AFX_EXT_CLASS char*StringToken(char*szSource,char charKey)
{
	char szTempSource[1024]="";
	strcpy(szTempSource,szSource);
	char*szToken=szTempSource;
	char*pTemp = strchr(szTempSource, charKey);
	if (pTemp)
	{
		strcpy(szSource,pTemp+1);
		*pTemp=0;
		return szTempSource;
	}

	return NULL;
}
*/


BOOL KillProcessFromName(CString strProcessName)
{  
	//�������̿���(TH32CS_SNAPPROCESS��ʾ�������н��̵Ŀ���)
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);

	//PROCESSENTRY32���̿��յĽṹ��   
	PROCESSENTRY32 pe;

	//ʵ������ʹ��Process32First��ȡ��һ�����յĽ���ǰ�����ĳ�ʼ������   
	pe.dwSize = sizeof(PROCESSENTRY32);


	//�����IFЧ��ͬ:   
	//if(hProcessSnap == INVALID_HANDLE_VALUE)   ��Ч�ľ��   
	if(!Process32First(hSnapShot,&pe))
	{
		return FALSE;
	}

	//���ַ���ת��ΪСд   
	strProcessName.MakeLower();

	//��������Ч  ��һֱ��ȡ��һ�����ѭ����ȥ   
	while (Process32Next(hSnapShot,&pe))
	{

		//pe.szExeFile��ȡ��ǰ���̵Ŀ�ִ���ļ�����   
		CString scTmp = pe.szExeFile;


		//����ִ���ļ���������Ӣ����ĸ�޸�ΪСд   
		scTmp.MakeLower();

		//�Ƚϵ�ǰ���̵Ŀ�ִ���ļ����ƺʹ��ݽ������ļ������Ƿ���ͬ   
		//��ͬ�Ļ�Compare����0   
		if(!scTmp.Compare(strProcessName))  
		{

			//�ӿ��ս����л�ȡ�ý��̵�PID(������������е�PID)   
			DWORD dwProcessID = pe.th32ProcessID;  
			HANDLE hProcess = ::OpenProcess(PROCESS_TERMINATE,FALSE,dwProcessID);  
			::TerminateProcess(hProcess,0);  
			CloseHandle(hProcess);  
			return TRUE;  
		}  
		scTmp.ReleaseBuffer();  
	}  
	strProcessName.ReleaseBuffer();  
	return FALSE;  
}  

CString GetOSVersoin(int& nHVersion, int& nLVersion)
{
	//Windows Vista��6.0
	//Windows 7��6.1
	//Windows 8��6.2
	//Windows 8.1��6.3
	//Windows 10 Build 9841/9860/9879��6.4
	//Windows 10 Build 9888��10.0

	OSVERSIONINFO   osver;     
	osver.dwOSVersionInfoSize   =   sizeof(OSVERSIONINFO);     
	GetVersionEx(&osver);
	CString strOSName;
	strOSName.Empty();
	if(osver.dwPlatformId == 2)  
	{
		nHVersion = osver.dwMajorVersion;
		nLVersion = osver.dwMinorVersion;
		if(osver.dwMajorVersion == 5 && osver.dwMinorVersion == 1)
		{ 
			strOSName = _T("XP");
		}  
		else if(osver.dwMajorVersion == 5 && osver.dwMinorVersion == 2)
		{
			strOSName = _T("2003"); 
		}  
		else if(osver.dwMajorVersion ==  6 && osver.dwMinorVersion == 0)
		{ 
			strOSName = _T("Vista&2008");
		}  
		else if(osver.dwMajorVersion ==  6 && osver.dwMinorVersion == 1)
		{ 
			strOSName = _T("2008R2&Win7");
		}  
		else if(osver.dwMajorVersion ==  6 && osver.dwMinorVersion == 2)
		{
			strOSName = _T("Win8");
		}
		else if(osver.dwMajorVersion ==  6 && osver.dwMinorVersion == 3)
		{
			strOSName = _T("Win8.1");
		} 
		else if(osver.dwMajorVersion ==  6 && osver.dwMinorVersion == 4)
		{
			strOSName = _T("Win10");
		}
		else if(osver.dwMajorVersion ==  10 && osver.dwMinorVersion == 0)
		{
			strOSName = _T("Win10");
		} 
	}  
	return strOSName;
} 

BOOL RegisterUrlProtecl(const char* cszPathAll)
{
	/*
	Windows Registry Editor Version 5.00
	[HKEY_CLASSES_ROOT\eClass]
	"URL Protocol"= "C:\\Program Files\\LiveTool\\LiveToolTransfer.exe"
	@="LiveToolProtocol"
	[HKEY_CLASSES_ROOT\eClass\shell]
	[HKEY_CLASSES_ROOT\eClass\shell\open]
	[HKEY_CLASSES_ROOT\eClass\shell\open\command]
	@="\"C:\\Program Files\\LiveTool\\LiveToolTransfer.exe\"\"%1\""

	[HKEY_CLASSES_ROOT\eClass\DefaultIcon]
	@="C:\\Program Files\\LiveTool\\LiveToolTransfer.exe"

	[HKEY_CLASSES_ROOT\Local Settings\Software\Microsoft\Windows\Shell\MuiCache]
	"C:\\Program Files\\LiveTool\\LiveToolTransfer.exe"="eClass"
	*/

	// �������Ӽ����ơ��͵��Ӽ��ľ��
	HKEY hRoot = HKEY_CLASSES_ROOT;
	char *szSubKey = "eClass";
	HKEY hKey;// ��ָ���Ӽ�
	DWORD dwDisposition = REG_OPENED_EXISTING_KEY;
	// ����������򴴽�
	LONG lRet =  RegCreateKeyExA(hRoot, szSubKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &dwDisposition);
	if(lRet != ERROR_SUCCESS)
		return FALSE;
	// ����һ���µļ�ֵ
	lRet = RegSetValueExA(hKey, "URL Protocol",0,REG_SZ,(BYTE*)cszPathAll,strlen(cszPathAll));
	// �ر��Ӽ����
	RegCloseKey(hKey);

	lRet =  RegCreateKeyExA(hRoot, "eClass\\shell", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &dwDisposition);RegCloseKey(hKey);
	lRet =  RegCreateKeyExA(hRoot, "eClass\\shell\\open", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &dwDisposition);RegCloseKey(hKey);
	lRet =  RegCreateKeyExA(hRoot, "eClass\\shell\\open\\command", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &dwDisposition);
	//if(lRet != ERROR_SUCCESS)
	//	return FALSE;
	// ����һ���µļ�ֵ
	char szKeyValue[256]={0};
	sprintf(szKeyValue,"\"%s\"\"%%1\"",cszPathAll);
	lRet = RegSetValueExA(hKey, "",0,REG_SZ,(BYTE*)szKeyValue,strlen(szKeyValue));
	// �ر��Ӽ����
	RegCloseKey(hKey);

	lRet =  RegCreateKeyExA(hRoot, "eClass\\DefaultIcon", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &dwDisposition);
	lRet = RegSetValueExA(hKey, "",0,REG_SZ,(BYTE*)cszPathAll,strlen(cszPathAll));
	// �ر��Ӽ����
	RegCloseKey(hKey);

	lRet =  RegCreateKeyExA(hRoot, "Local Settings\\Software\\Microsoft\\Windows\\Shell\\MuiCache", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &dwDisposition);
	lRet = RegSetValueExA(hKey,cszPathAll,0,REG_SZ,(BYTE*)"eClass",strlen("eClass"));
	// �ر��Ӽ����
	RegCloseKey(hKey);
	return TRUE;
}

CRect EXPORT_FUNCTION AfxGetDesktopRect()
{
	RECT rc;
	SystemParametersInfo(SPI_GETWORKAREA, 0, (PVOID)&rc, 0);//win��xp����������Ȳ�һ��������������

	CRect rcScreen;
	if (rc.right>0 && rc.bottom>0)
	{
		rcScreen.SetRect(rc.left, rc.top, rc.right, rc.bottom);
	}
	else
	{
		int scx = ::GetSystemMetrics(SM_CXSCREEN);
		int scy = ::GetSystemMetrics(SM_CYSCREEN);
		rcScreen.SetRect(0, 0, scx, scy - 28);
	}
	return rcScreen;
}

CRect EXPORT_FUNCTION AfxGetMaxWindowClientRect()
{
	RECT rc;
	SystemParametersInfo(SPI_GETWORKAREA, 0, (PVOID)&rc, 0);

	int nCaptionHeight = ::GetSystemMetrics(SM_CYCAPTION);//������

	CRect rcScreen;
	if (rc.right>0 && rc.bottom>0)
	{
		rcScreen.SetRect(rc.left, rc.top, rc.right, rc.bottom);
		rcScreen.bottom -= nCaptionHeight;
	}
	else
	{
		int scx = ::GetSystemMetrics(SM_CXSCREEN);
		int scy = ::GetSystemMetrics(SM_CYSCREEN);
		rcScreen.SetRect(0, 0, scx, scy - 28);
		rcScreen.bottom -= nCaptionHeight;
	}

	return rcScreen;
}

EXPORT_FUNCTION int StringEncrypt(char* pKey, char* pText, unsigned char* pResult)
{
	if (!pKey || !pText)
		return 0;
	if (strlen(pKey) <= 0 || strlen(pText) <= 0)
		return 0;
	int nTextLen = strlen(pText);
	int nKeyLen = strlen(pKey);
	unsigned char* pPos = pResult;
	srand((int)time(0));
	unsigned char uRand = rand() % 255;

	//��һλΪУ��λ����Ҫͨ���ڶ�λУ��
	for (int j = 0; j < nKeyLen; j++)
	{
		*pPos += (unsigned char)(*(pKey + j));
	}
	*pPos += uRand;
	pPos++;
	//�ڶ�λΪ�����
	*pPos = uRand;
	pPos++;

	for (int i = 0; i < nTextLen; i++)
	{
		*pPos = 0x00;
		*pPos++ = (unsigned char)(*(pText + i)) + (unsigned char)(*(pKey + (i%nKeyLen))) + uRand;
	}
	return nTextLen + 2;
}

EXPORT_FUNCTION bool StringDecrypt(char* pKey, unsigned char* pDate, int nDataLen, char*pResult)
{
	if (!pKey || !pDate)
		return false;
	if (strlen(pKey) <= 0 || nDataLen <= 0)
		return false;
	int nKeyLen = strlen(pKey);
	unsigned char uCheck = *pDate;//ȡ��У��λ
	unsigned char uRand = *(pDate + 1);//ȡ�������
	unsigned char uCauculate = 0;//����ֵ
	for (int j = 0; j < nKeyLen; j++)
	{
		uCauculate += (unsigned char)(*(pKey + j));
	}
	uCauculate += uRand;
	if (uCauculate != uCheck)
		return false;

	for (int i = 2; i < nDataLen; i++)
	{
		*pResult++ = (unsigned char)(*(pDate + i)) - (unsigned char)(*(pKey + ((i - 2) % nKeyLen))) - uRand;
	}
	return true;
}

unsigned __int64 RDTSC()
{
	__asm _emit 0x0F;
	__asm _emit 0x31;
}

EXPORT_FUNCTION double CPUClockSpeed()
{
	HANDLE hThread = GetCurrentThread();
	SetThreadAffinityMask(hThread, 0x1);

	//�����ϸ߾��ȶ�ʱ���ľ���Ƶ��  
	//�����ʱ��Ӧ�þ���һƬ8253����8254  
	//��intel ich7�м�����8254  
	LARGE_INTEGER lFrequency;
	QueryPerformanceFrequency(&lFrequency);
	//printf("�߾��ȶ�ʱ���ľ���Ƶ�ʣ�%1.0fHz.\n",(double)lFrequency.QuadPart);  

	//�����ʱ��ÿ����һ��ʱ�����ڣ����������+1  
	LARGE_INTEGER lPerformanceCount_Start;
	QueryPerformanceCounter(&lPerformanceCount_Start);

	//RDTSCָ��:��ȡCPU������ʱ��������  
	__int64 _i64StartCpuCounter = RDTSC();

	//��ʱ��һ��,����Сһ��  
	//int nTemp=100000;  
	//while (--nTemp);  
	Sleep(200);

	LARGE_INTEGER lPerformanceCount_End;
	QueryPerformanceCounter(&lPerformanceCount_End);

	__int64 _i64EndCpuCounter = RDTSC();

	//f=1/T => f=��������/(��������*T)  
	//����ġ���������*T������ʱ���  
	double fTime = ((double)lPerformanceCount_End.QuadPart - (double)lPerformanceCount_Start.QuadPart)
		/ (double)lFrequency.QuadPart;

	return (_i64EndCpuCounter - _i64StartCpuCounter) / fTime / 1000000.0;
}
