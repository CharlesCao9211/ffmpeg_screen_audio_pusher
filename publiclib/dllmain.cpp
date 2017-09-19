// dllmain.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include <afxwin.h>
#include <afxdllx.h>
#include "publicfun.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern CString		g_strIniFile;
extern CString	g_languageFile;
extern TCHAR szPath[];// = {0};

static AFX_EXTENSION_MODULE publiclibDLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// ���ʹ�� lpReserved���뽫���Ƴ�
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("publiclib.DLL ���ڳ�ʼ��!\n");
		
		// ��չ DLL һ���Գ�ʼ��
		if (!AfxInitExtensionModule(publiclibDLL, hInstance))
			return 0;

		// ���� DLL ���뵽��Դ����
		// ע��:  �������չ DLL ��
		//  MFC ���� DLL (�� ActiveX �ؼ�)��ʽ���ӵ���
		//  �������� MFC Ӧ�ó������ӵ�������Ҫ
		//  �����д� DllMain ���Ƴ������������һ��
		//  �Ӵ���չ DLL �����ĵ����ĺ����С�  ʹ�ô���չ DLL ��
		//  ���� DLL Ȼ��Ӧ��ʽ
		//  ���øú����Գ�ʼ������չ DLL��  ����
		//  CDynLinkLibrary ���󲻻ḽ�ӵ�
		//  ���� DLL ����Դ���������������ص�
		//  ���⡣

		new CDynLinkLibrary(publiclibDLL);

		//��ȡ��������·��
		//GetModuleFileName(NULL, szPath, 1024);
		//TCHAR *pos = strrchr(szPath, '\\');
		//*(pos + 1) = '\0';

		//CString strPath = szPath;
		//if (strPath.IsEmpty())
		//	return 0;

		//CString strLan_CS = strPath + "str-sc.ini";
		//CString strLan_EN = strPath + "str-en.ini";
		//CString strConfig = strPath + "setting.ini";
		//g_strIniFile = strConfig;

		////Ŀǰֻ֧����Ӣ�����汾
		//LANGID lid = GetSystemDefaultLangID();
		//if (lid == 0x0804)//��������
		//{
		//	g_languageFile = strLan_CS;
		//}
		//else
		//{
		//	g_languageFile = strLan_EN;
		//}
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("publiclib.DLL ������ֹ!\n");

		// �ڵ�����������֮ǰ��ֹ�ÿ�
		AfxTermExtensionModule(publiclibDLL);
	}
	return 1;   // ȷ��
}
