#ifndef __MY_DEF__
#define __MY_DEF__

#define EXPORT_FUNCTION AFX_EXT_CLASS

#include "stdafx.h"
#include <list>
#include <map>
using namespace std;


//����
#define ORGANIZATION_TALK915
//#define ORGANIZATION_CHUNXI
//#define ORGANIZATION_TALKILLA

//------------------------------------ģ���ṹ����------------------------------------

#define GETROOMSTATUS	(WM_USER + 2000)  //��ȡ����������״̬������֪ͨ��ģ�鴰�ڴ���

//�û����Ͷ���(��ʱ����)
typedef enum _tagUserTypeEnum
{
	USER_TYPE_TEACHER = 1,
	USER_TYPE_STUDENT,
	USER_TYPE_ASSISTANT, //����
	USER_TYPE_WATCHER, //Ѳ��Ա
	USER_TYPE_GUEST = 5  //�ο�
}UserTypeEnum;

//��ɫ���Ͷ���
typedef enum _tagRoleTypeEnum
{
	ROLE_TYPE_HOST = 1,  //����
	ROLE_TYPE_ATTENDER,  //������
	ROLE_TYPE_ASSISTANT, //����
	ROLE_TYPE_WATCHER,   //Ѳ��Ա
	ROLE_TYPE_GUEST = 5  //�ο�
}RoleTypeEnum;

//�������Ͷ���
typedef enum _tagRoomTypeEnum
{
	ROOM_TYPE_1VS1 = 1, //1��1
	ROOM_TYPE_SMALL,     //С���
	ROOM_TYPE_BIG,     //����
	ROOM_TYPE_MAX
}RoomTypeEnum;

//statusBar��ʾ������Ϣ
typedef struct _tagXmlScrollMessage
{
	string messageType;
	string messageTextSC; //��ʾ����(����)
	string messageTextEN; //��ʾ����(Ӣ��)
	string hyperlink;  //web����
}XmlScrollMessage;

typedef list<XmlScrollMessage *> P_LIST_XMLSCROLLMESSAGE;

//����״̬
typedef struct _tagRoomStatus
{
	string strTimerStartTime; //�γ̼�ʱ����ʼ��ʱ��ʱ��            
	bool bLockWhiteBoard;   //�Ƿ������װ壬Ϊ0ʱ����ȫ��ѧ���װ�
	list<int> listLimitVideo;  //�ر���Ƶѧ��id
	list<int> listAllowMic;    //�ر���˷�ѧ��id
	list<int> listLimitTalk;   //����ѧ��id
	list<int> listLimitEnter;  //�������ѧ��id   <�Ȳ�ʵ��>
	int nTempHost;          //��ʱ����id
	map<int, int> mapLike;  //����(userID, count)
}RoomStatus;

//����״̬����
typedef enum _tagRoomStatusEnum
{
	ROOM_STATUS_STRAT_TIMER_TIME, 
	ROOM_STATUS_LOCK_WHITEBOARD,
	ROOM_STATUS_LIMIT_VIDEO,
	ROOM_STATUS_LIMIT_MIC,
	ROOM_STATUS_LIMIT_TALK,
	ROOM_STATUS_LIMIT_ENTER,
	ROOM_STATUS_TEMP_HOST, //��ʱ����
	ROOM_STATUS_LIKE //����
}RoomStatusEnum;

enum LOGIN_MODE
{
	MODE_CLIENT_USER = 0,//�ͻ��˵�¼���û�
	MODE_CLIENT_GUEST,//�ͻ��˵�¼���ο�
	MODE_WEB_USER,//web��¼���û�
	MODE_MAX
};


#ifdef PUBLICLIB_EXPORT
#define PUBLICLIB_API extern "C" __declspec(dllexport)
#else
#define PUBLICLIB_API extern "C" __declspec(dllimport)
#endif

PUBLICLIB_API TCHAR g_szUploadDocServer[256];//doc server��ip
PUBLICLIB_API TCHAR g_szUploadServerIP[256];
PUBLICLIB_API TCHAR g_szUploadServerIPCDN[256];//ͨ������ת�����IP
PUBLICLIB_API TCHAR g_szUploadServerDomain[256]; //doc��������
PUBLICLIB_API USHORT g_nUploadServerPort;//doc����server�˿ں�
PUBLICLIB_API LOGIN_MODE g_enumCurLoginMode;
PUBLICLIB_API TCHAR g_szCurGroupName[128];
PUBLICLIB_API TCHAR g_szeClassGroupName[128];
PUBLICLIB_API TCHAR g_szTGroupName[128];
#endif