#ifndef MEDIATYPEDEF_H
#define MEDIATYPEDEF_H
#include <CString>
#include <string>
#include <map>
#include <vector>
#include <comdef.h>

namespace MediaType
{
	struct RECORD_INFO
	{
		char rtmp_url[1024];//������ַ
		char video_device_name[1024];//��Ƶ�豸��
		char audio_device_name[1024];//��Ƶ�豸��
		int  video_dst_width;//��Ƶ��
		int  video_dst_height;//��Ƶ��
		int  video_frame_rate;//֡��
		HWND preview_hwnd;//��Ƶ��ʾ���
		RECORD_INFO()
		{
			memset(rtmp_url, 0, 1024);
			memset(video_device_name, 0, 1024);
			memset(audio_device_name, 0, 1024);
			video_dst_width = 0;
			video_dst_height = 0;
			video_frame_rate = 0;
			preview_hwnd = NULL;
		}
	};

	enum DeviceType{
		n_Video = 0,	//��Ƶ
		n_Audio = 1		//��Ƶ
	};

	typedef map<int, std::vector<std::string>> DEVICE_INFO;
}
#endif