// ffmpeg_screen_audio_pusher.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "ffmpeg_screen_audio_pusher.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO:  ����� DLL ����� MFC DLL �Ƕ�̬���ӵģ�
//		��Ӵ� DLL �������κε���
//		MFC �ĺ������뽫 AFX_MANAGE_STATE ����ӵ�
//		�ú�������ǰ�档
//
//		����: 
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ��  ����ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

// Cffmpeg_screen_audio_pusherApp

BEGIN_MESSAGE_MAP(Cffmpeg_screen_audio_pusherApp, CWinApp)
END_MESSAGE_MAP()


// Cffmpeg_screen_audio_pusherApp ����

Cffmpeg_screen_audio_pusherApp::Cffmpeg_screen_audio_pusherApp()
{
	// TODO:  �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� Cffmpeg_screen_audio_pusherApp ����

Cffmpeg_screen_audio_pusherApp theApp;


// Cffmpeg_screen_audio_pusherApp ��ʼ��

BOOL Cffmpeg_screen_audio_pusherApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
