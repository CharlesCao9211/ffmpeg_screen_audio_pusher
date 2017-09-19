
#include "stdafx.h"
#include "NetAdapter.h"
#include "LogEx.h"
#include <iphlpapi.h>
#pragma comment(lib,"Iphlpapi.lib")

int GetNetAdapterType(void)
{
	int nType = 1;//1���߻���ʶ��ģ�2����

	PIP_ADAPTER_INFO pAdapterInfo;
	PIP_ADAPTER_INFO pAdapter = NULL;
	ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
	pAdapterInfo = (PIP_ADAPTER_INFO)malloc(ulOutBufLen);
	DWORD dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen);
	// ��һ�ε���GetAdapterInfo��ȡulOutBufLen��С
	if (dwRetVal == ERROR_BUFFER_OVERFLOW)
	{
		free(pAdapterInfo);
		pAdapterInfo = (IP_ADAPTER_INFO *)malloc(ulOutBufLen);
		dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen);
	}
	if (dwRetVal == NO_ERROR)
	{
		char szIP[32] = { 0 };
		pAdapter = pAdapterInfo;
		while (pAdapter)
		{
			//printf("Adapter Name: \t%s\n", pAdapter->AdapterName);
			//printf("Adapter Desc: \t%s\n", pAdapter->Description);
			//printf("MAC Addr: \t%02x-%02x-%02x-%02x-%02x-%02x\n",
			//	pAdapter->Address[0],
			//	pAdapter->Address[1],
			//	pAdapter->Address[2],
			//	pAdapter->Address[3],
			//	pAdapter->Address[4],
			//	pAdapter->Address[5]);
			//printf("IP Address: \t%s\n", pAdapter->IpAddressList.IpAddress.String);
			//printf("IP Mask: \t%s\n", pAdapter->IpAddressList.IpMask.String);
			//printf("Gateway: \t%s\n", pAdapter->GatewayList.IpAddress.String);
			//printf("Type: \t%d\n", pAdapter->Type);
			if (pAdapter->Type == 6)//���ߣ�ֻ������vista����ϵͳ��xp�����ã�
			{
				strcpy(szIP, pAdapter->IpAddressList.IpAddress.String);
				if (strcmp(szIP, "0.0.0.0") == 0);
				else if(strcmp(szIP, "127.0.0.1") == 0);
				else
				{
					nType = 1;
					break;
				}
			}
			else if (pAdapter->Type == 71)//���ߣ�ֻ������vista����ϵͳ��xp�����ã�
			{
				strcpy(szIP, pAdapter->IpAddressList.IpAddress.String);
				if (strcmp(szIP, "0.0.0.0") == 0);
				else if (strcmp(szIP, "127.0.0.1") == 0);
				else
				{
					nType = 2;
					break;
				}
			}

			pAdapter = pAdapter->Next;
		}// end while
	}
	else
	{
		//printf("Call to GetAdaptersInfo failed.\n");
	}
	return nType;
}


int GetHostIpByDomain(CString cstrDomain, CString &cstrIp)
{
	//����ת��Ϊip��ַ
	char szIP[256] = { 0 };
	struct hostent *pHostent = NULL;
	string strDomain = CW2A(cstrDomain.GetBuffer(0), CP_UTF8);
	if ("" == strDomain || NULL == strstr(strDomain.c_str(), ".com"))
	{
		Write_Log(LOG_ERROR, "CHTTPDownload: GetHostIpByDomain domain error(domain:%s).", strDomain.c_str());
		return -1;
	}

	CStringA strADomain = strDomain.c_str();
	strADomain.Replace("http://", "");
	strADomain.Replace("/", "");
	strDomain = strADomain.GetBuffer(0);

	if (NULL == (pHostent = gethostbyname(strDomain.c_str())))
	{
		Write_Log(LOG_ERROR, "CHTTPDownload: GetHostIpByDomain gethostbyname error(domain:%s).", strDomain.c_str());
		return -1;
	}

	sockaddr_in   addr;
	addr.sin_addr.S_un.S_addr = *(u_long *)pHostent->h_addr_list[0];
	//if (NULL == inet_ntop(pHostent->h_addrtype, &addr.sin_addr, szIP, sizeof(szIP)))
	char *inetIP = NULL;
	if (NULL == (inetIP = inet_ntoa(addr.sin_addr)))
	{
		Write_Log(LOG_ERROR, "CHTTPDownload: get ip address error(domain:%s).", strDomain.c_str());
		return -1;
	}

	strcpy(szIP, inetIP);
	cstrIp = CA2W(szIP, CP_UTF8);

	return 0;
}