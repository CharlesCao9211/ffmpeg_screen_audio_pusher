
// RtmpStreamPusher.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CRtmpStreamPusherApp: 
// �йش����ʵ�֣������ RtmpStreamPusher.cpp
//

class CRtmpStreamPusherApp : public CWinApp
{
public:
	CRtmpStreamPusherApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CRtmpStreamPusherApp theApp;