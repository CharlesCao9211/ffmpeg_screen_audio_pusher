#pragma once
#include "IScreenAudioClass.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <math.h>
extern "C"
{
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libswscale/swscale.h"
#include "libavutil/avutil.h"
#include "libavutil/audio_fifo.h"
#include "libavutil/mathematics.h"
#include "libswresample/swresample.h" 
#include "libavutil/imgutils.h"
#include "libavutil/opt.h"  
#include "libavutil/time.h"  
#include "libavdevice/avdevice.h" 
#include "inttypes.h"
#include "SDL.h"
#include "SDL_thread.h"
};
//DirectShow
#include "dshow.h"  //����ICreateDevEnum
#include "MediaTypeDef.h"

#include "afxcmn.h"
#include "afxwin.h"

#define AUDIO_BUF_SIZE 1024
#define SAMPLE_ARRAY_SIZE (8 * 65536)


#define FF_ALLOC_EVENT   (SDL_USEREVENT)
#define FF_AUDIO_REFRESH_EVENT (SDL_USEREVENT + 1)
#define FF_VIDEO_REFRESH_EVENT (SDL_USEREVENT + 2)
#define FF_BREAK_EVENT (SDL_USEREVENT + 3)
#define FF_QUIT_EVENT    (SDL_USEREVENT + 10)

DEFINE_GUID(CLSID_PushSourceDesktop,
	0x4ea6930a, 0x2c8a, 0x4ae6, 0xa5, 0x61, 0x56, 0xe4, 0xb5, 0x4, 0x44, 0x39);

typedef struct AudioParams {
	int freq;
	int channels;
	int channel_layout;
	enum AVSampleFormat fmt;
} AudioParams;


enum ShowMode {
	SHOW_MODE_NONE = -1,
	SHOW_MODE_VIDEO = 0,
	SHOW_MODE_WAVES = 1,
	SHOW_MODE_RDFT = 2,
	SHOW_MODE_NB = 3
};

typedef struct stream_info{
	AVFormatContext		*m_pFormatCtx;
	SDL_Window			*m_pShowScreen;			//����Ƶ��ʾSDL����
	SDL_Surface			*m_pScreenSurface;		//��screen�󶨵ı���
	int					 m_xLeft;				//��ʾ��������꼰��С
	int					 m_yTop;
	int					 m_width;
	int					 m_height;
	int					 m_iAbortRequest;		//�˳����
	int					 m_iRefresh;				//ˢ�±��
	int					 m_iShowMode;			//��ʾģʽ
	int					 m_iPaused;				//��ͣ���
	SDL_Renderer		*m_pSdlRender;
	SDL_Texture			*m_pSdlTexture;
	/************************��Ƶ��ز���-start*********************/
	SDL_Thread			*m_pAudioThr;		//��Ƶ�����߳�
	SDL_Thread			*m_pAudioRefreshThr;	//��Ƶˢ���߳̾��
	AVStream			*m_pAudioStream;		//��Ƶ��
	AVFrame				*m_pAudioFrame;		//��Ƶ֡
	AVAudioFifo			*m_pAudioFifo;
	SDL_mutex			*m_pAudioMutex;
	AudioParams			 m_AudioPrm;
	uint8_t				*m_pAudioBuf;
	int					 m_iAudioBufSize;
	int					 m_iAudioBufIndex;
	int					 m_iAduioPktSize;
	int					 m_iAudioWriteBufSize;
	int					 m_iAudioLastStart;
	uint8_t				 m_uSilenceBuf[AUDIO_BUF_SIZE];
	int16_t				 m_iSampleArray[SAMPLE_ARRAY_SIZE];
	int					 m_iSampleArrayIndex;
	/************************��Ƶ��ز���-end***********************/

	/************************��Ƶ��ز���-satrt*********************/
	SDL_Thread			*m_pVideoThr;			//��Ƶ�����߳�
	SDL_Thread			*m_pVideoRefreshThr;		//��Ƶˢ���߳̾��
	AVFifoBuffer		*m_pVideoFifo;
	SDL_mutex			*m_pVideoMutex;
	AVStream			*m_pVideoStream;			//��Ƶ��
	uint8_t				*m_pPushPicSize;			//����Pic��С
	SwsContext			*m_pVideoSwsCtx;			//��Ƶ�仯ctx
	/************************��Ƶ��ز���-end***********************/
}struct_stream_info;

using namespace MediaType;
using namespace MediaRtmpPusher;
namespace MediaRtmpPusher
{
	class CScreenAudioRecord : public IScreenAudioClass
	{
	public:
		CScreenAudioRecord();
		~CScreenAudioRecord();

		int SetRecordInfo(const RECORD_INFO& recordInfo) override;
		int StartRecord() override;
		int StopRecord() override;
		DEVICE_INFO GetDevice() override;
	private:
		void RegisterPlugin();
		bool IsOsVersionVistaOrGreater();
		void ShellExecuteExOpen(CString appName, CString appPath);
		void InitData();
		void InitSdl();	
		int CheckRecordInfo();
		int CreateVideoWindow();
		int OpenCamera();
		int InitVideoWindow();
		int OpenAduio();
		int OpenRtmpAddr();
		int OpenRtmpUrl();
		int UpdateSdlInfo();
		int  GetDeviceInfo(int _iDeviceType);
		CString GetWorkDir();

	public:
		struct_stream_info*						m_pStreamInfo;	//����Ƶȫ�ֽṹ��
		RECORD_INFO								m_recordInfo;	//��Ƶ����
		AVFormatContext						   *m_pFmtVideoCtx;	//��Ƶ�ɼ�format
		AVFormatContext						   *m_pFmtAudioCtx;	//��Ƶ�ɼ�format
		AVFormatContext						   *m_pFmtRtmpCtx;	//rtmp����format
		AVCodecContext						   *m_pCodecVideoCtx;//��Ƶ�ɼ���������Ϣ
		AVCodecContext						   *m_pCodecAudioCtx;//��Ƶ�ɼ���������Ϣ
		int										m_iVideoIndex;	//��Ƶ�ɼ�����������
		int										m_iAudioIndex;	//��Ƶ�ɼ�����������
		int										m_iVideoOutIndex;//������Ƶ����������
		int										m_iAudioOutIndex;//������Ƶ����������
		int										m_iSrcVideoHeight;//Դ��Ƶ��
		int										m_iSrcVideoWidth;//Դ��Ƶ��
		int										m_iWindowWidth;	//������Ŀ�
		int										m_iWindowHeight;	//������ĸ�
		BOOL									m_blCreateVideoWin;//�Ƿ񴴽���Ƶ����
		BOOL									m_blPushSuccess;//�Ƿ����ͳɹ�
		BOOL									m_bPushStatus;//����״̬

		SDL_Thread							   *m_pPushStreamThrid;//�����߳�
		SDL_Thread							   *m_pWriteStreamThrid;//�����߳�

		DEVICE_INFO								m_mapDeviceInfo;	//�豸��Ϣ����

	};

	int video_thr(LPVOID lpParam);
	int audio_thr(LPVOID lpParam);
	int push_thr(LPVOID lpParam);
	int write_frame_thr(LPVOID lpParam);
	void set_packet(AVPacket* pkt);
	AVPacket* get_packet();

	list<AVPacket*> m_listPacket;
	CritSec m_CritSec;
}
