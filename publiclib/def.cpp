
#include "def.h"


/* ---------------------- ������ ---------------------- */
//[UploadServer]
TCHAR g_szUploadDocServer[256] = {0};//GetDocServer�ӿڷ��أ� http://106.75.129.178:8080/
TCHAR g_szUploadServerIP[256] = { 0 }; //������IP 106.75.129.178������post����
TCHAR g_szUploadServerIPCDN[256] = {0};//106.75.129.178����CDNת����IP�������ļ�����
TCHAR g_szUploadServerDomain[256] = { 0 }; //����eclassdoc1.talk915.com
USHORT g_nUploadServerPort = 8080; //�ļ����ض˿�

LOGIN_MODE g_enumCurLoginMode = MODE_MAX;//��ǰ��¼����
TCHAR g_szCurGroupName[128] = {0}; //��ǰ��¼����������rQXBUcWduZIQZCd��
TCHAR g_szeClassGroupName[128] = { 0 }; //eclass�˻��Ļ�����
TCHAR g_szTGroupName[128] = { 0 };	//T�˻��Ļ�����
