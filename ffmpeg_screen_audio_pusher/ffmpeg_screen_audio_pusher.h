// ffmpeg_screen_audio_pusher.h : ffmpeg_screen_audio_pusher DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// Cffmpeg_screen_audio_pusherApp
// �йش���ʵ�ֵ���Ϣ������� ffmpeg_screen_audio_pusher.cpp
//

class Cffmpeg_screen_audio_pusherApp : public CWinApp
{
public:
	Cffmpeg_screen_audio_pusherApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
