
// RtmpStreamPusherDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "../publiclib/publicfun.h"
#include "../ffmpeg_screen_audio_pusher/IScreenAudioPusher.h"
#pragma comment(lib, "../Debug/publiclib.lib")
#pragma comment(lib, "../Debug/ffmpeg_screen_audio_pusher.lib")


// CRtmpStreamPusherDlg �Ի���
class CRtmpStreamPusherDlg : public CDialog
{
// ����
public:
	CRtmpStreamPusherDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_RTMPSTREAMPUSHER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonStop();
	CButton m_btStart;
	CButton m_btStop;
	CEdit m_editFrameRate;
	CEdit m_editUrl;
	void* m_pPusher;
	CComboBox m_cboDeviceVideo;
	CComboBox m_cboDeviceAudio;
	MediaType::DEVICE_INFO m_mapDeviceInfo;
};
