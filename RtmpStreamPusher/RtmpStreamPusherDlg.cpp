
// RtmpStreamPusherDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RtmpStreamPusher.h"
#include "RtmpStreamPusherDlg.h"
#include "afxdialogex.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CRtmpStreamPusherDlg �Ի���



CRtmpStreamPusherDlg::CRtmpStreamPusherDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRtmpStreamPusherDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRtmpStreamPusherDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_START, m_btStart);
	DDX_Control(pDX, IDC_BUTTON_STOP, m_btStop);
	DDX_Control(pDX, IDC_EDIT_FRAMERATE, m_editFrameRate);
	DDX_Control(pDX, IDC_EDIT_URL, m_editUrl);
	DDX_Control(pDX, IDC_COMBO_VIDEO, m_cboDeviceVideo);
	DDX_Control(pDX, IDC_COMBO_AUDIO, m_cboDeviceAudio);
}

BEGIN_MESSAGE_MAP(CRtmpStreamPusherDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_START, &CRtmpStreamPusherDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CRtmpStreamPusherDlg::OnBnClickedButtonStop)
END_MESSAGE_MAP()


// CRtmpStreamPusherDlg ��Ϣ�������

BOOL CRtmpStreamPusherDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	m_editFrameRate.SetWindowTextW(_T("25"));
	m_editUrl.SetWindowTextW(_T("rtmp://video-center.alivecdn1.com/55667788/LiveStream?vhost=rtmp01.talk915.com"));

	m_pPusher = MR_CreateScreenAudioRecorder();


	m_mapDeviceInfo = MR_GetDevice(m_pPusher);

	m_cboDeviceVideo.ResetContent();
	m_cboDeviceAudio.ResetContent();
	//����ȡ�����豸��Ϣ������������
	MediaType::DEVICE_INFO::iterator iter = m_mapDeviceInfo.begin();
	for (; iter != m_mapDeviceInfo.end(); iter++){
		if (MediaType::n_Video == iter->first){
			//��Ƶ�豸
			for (int i = 0; i < iter->second.size(); i++){
				int iCount = m_cboDeviceVideo.GetCount();
				CharCode szDevice(iter->second[i].c_str());
				m_cboDeviceVideo.InsertString(iCount, szDevice.GetStringW());
			}
			m_cboDeviceVideo.SetCurSel(0);
		}
		else if (MediaType::n_Audio == iter->first){
			//��Ƶ�豸
			for (int i = 0; i < iter->second.size(); i++){
				int iCount = m_cboDeviceAudio.GetCount();
				CharCode szDevice(iter->second[i].c_str());
				m_cboDeviceAudio.InsertString(iCount, szDevice.GetStringW());
			}
			m_cboDeviceAudio.SetCurSel(0);
		}
	}

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CRtmpStreamPusherDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CRtmpStreamPusherDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CRtmpStreamPusherDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CRtmpStreamPusherDlg::OnBnClickedButtonStart()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	int   cx = GetSystemMetrics(SM_CXSCREEN);
	int   cy = GetSystemMetrics(SM_CYSCREEN);
	CString cstrVideoDevName = _T("");
	CString cstrAudioDevName = _T("");
	m_cboDeviceVideo.GetWindowText(cstrVideoDevName);
	m_cboDeviceAudio.GetWindowText(cstrAudioDevName);
	if (cstrVideoDevName.IsEmpty()){
		cstrVideoDevName = _T("screen-capture-recorder");
	}
	if (cstrAudioDevName.IsEmpty()){
		cstrAudioDevName = _T("virtual-audio-capturer");
	}
	cstrVideoDevName = _T("video=") + cstrVideoDevName;
	cstrAudioDevName = _T("audio=") + cstrAudioDevName;

	CString strFrame = _T("");
	CString strUrl = _T("");
	m_editFrameRate.GetWindowTextW(strFrame);
	m_editUrl.GetWindowTextW(strUrl);
	CharCode szUrl(strUrl.GetBuffer(0));
	CharCode szVideoDev(cstrVideoDevName.GetBuffer(0));
	CharCode szAudioDev(cstrAudioDevName.GetBuffer(0));

	MediaType::RECORD_INFO record_info;
	strcpy_s(record_info.rtmp_url, szUrl.GetStringA());
	strcpy_s(record_info.video_device_name, szVideoDev.GetStringA());
	strcpy_s(record_info.audio_device_name, szAudioDev.GetStringA());
	record_info.video_dst_width = cx;
	record_info.video_dst_height = cy;
	record_info.video_frame_rate = _ttoi(strFrame);
	record_info.preview_hwnd = GetDlgItem(IDC_STATIC_PREVIEW)->GetSafeHwnd();

	int ret = MR_SetRecordInfo(m_pPusher, record_info);
	ret = MR_StartRecord(m_pPusher);
}


void CRtmpStreamPusherDlg::OnBnClickedButtonStop()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	MR_StopRecord(m_pPusher);
}
