#include "stdafx.h"
#include "DirFileEx.h"
#include "LogEx.h"

static char g_szPathMulti[512] = { 0 };//��ǰģ�鹤��·��
static TCHAR g_szPath[512] = { 0 }; //��ǰģ�鹤��·��
static TCHAR g_szeClassLogPath[512] = { 0 }; //eclass log·��
static TCHAR g_szeClassDocumentPath[512] = { 0 }; //eclass document·��
static TCHAR g_szeClassScreenshotPath[512] = { 0 }; //eclass ��Ļ��ͼ����·��
static TCHAR g_szeClassAppdataPath[MAX_PATH] = { 0 }; //eclass ¼�����ñ���·��

//���ܣ���ȡ����ǰ����Ŀ¼
//��������
//����ֵ����ǰ����Ŀ¼
const char* GetRunDir()
{
	if ('\0' == g_szPathMulti[0])
	{
		memset(g_szPathMulti, 0, sizeof(g_szPathMulti));
		GetModuleFileNameA(NULL, g_szPathMulti, sizeof(g_szPathMulti));
		char *pos = strrchr(g_szPathMulti, '\\');
		if (!pos)
		{
			return "";//������return NULL���ⲿ����strcpy�����
		}
		*(pos + 1) = '\0';
	}

	return g_szPathMulti;
}

const TCHAR* GetRunDirW()
{
	//TCHAR szPath[512] = { 0 };
	if (_T('\0') == g_szPath[0])
	{
		memset(g_szPath, 0, sizeof(g_szPath));
		GetModuleFileName(NULL, g_szPath, sizeof(g_szPath)); //sizeof(g_szPath) == sizeof(TCHAR)* 512

		TCHAR *pos = wcsrchr(g_szPath, _T('\\')); //wcsrchr:��һ���ַ�����Ѱ��ĳ���ַ������ֵ�λ��
		if (!pos)
		{
			return _T("");//������return NULL���ⲿ����strcpy�����
		}
		*(pos + 1) = _T('\0');
	}

	return g_szPath;
}

/*********************************************************************
* ��������:bool Ex_CreateDiretory(const char* pszDir)
* ˵��:����Ŀ¼
* �����ߣ�anyone
* �������:
* const char *pszDir --�û�ָ����·��
* ���������
* ��
* ����ֵ:
* true ��ʾ�ɹ���flase ��ʾʧ��
* ע�⣺
*********************************************************************/
bool Ex_CreateDiretory(const char* pszDir)
{
	if (!pszDir)
	{
		return false;
	}

	//���������\�����ڵ�ǰĿ¼�´���
	if (strstr(pszDir, "\\") == NULL)
	{
		//��ȡ��ǰ·��+��Ҫ�������ļ�������
		char szPathAll[512] = { 0 };
		sprintf(szPathAll, "%s%s", GetRunDir(), pszDir);

		int iRet = CreateDirectoryA(szPathAll, NULL);
		if (0 == iRet)
		{
			iRet = GetLastError();
			if (ERROR_ALREADY_EXISTS == iRet)
			{
				return true;
			}
			else
				return false;
		}
	}
	else//ȫ·�������ҿ����ж༶Ŀ¼�������Ŀ¼����û�д���
	{
		int iRet = 0;
		char szSub[512] = { 0 };
		const char *pSub = NULL;
		int iIndex = 0;
		int iLen = 0;
		int bFind = 0;

		//��㴴��Ŀ¼
		while (true)
		{
			pSub = strchr(pszDir + iLen, '\\');
			if (NULL == pSub)//����Ҳ���'\'
			{
				//����Ǹ�Ŀ¼,����'C:\\'
				if (iLen == 0)
				{
					return false;
				}
				//����Ѿ����������һ��Ŀ¼
				iRet = CreateDirectoryA(pszDir, NULL);
				if (0 == iRet)
				{
					iRet = GetLastError();
					if (ERROR_ALREADY_EXISTS == iRet)
					{
						return true;
					}
					else
						return false;
				}
			}//if (NULL == pSub)//����Ҳ���'\'
			else//�ҵ�'\'
			{
				if (!bFind)//������һ��'\'
				{
					bFind = 1;
				}
				else
				{
					memset(szSub, 0, sizeof(szSub));
					strncpy(szSub, pszDir, pSub - pszDir);
					CreateDirectoryA(szSub, NULL);
				}
				iLen = pSub - pszDir + 1;
			}//else//�ҵ�'\'
		}//while (true)
	}//else//ȫ·�������ҿ����ж༶Ŀ¼�������Ŀ¼����û�д���
	return true;
}

bool  Ex_CreateDiretory(const TCHAR* pszDir)
{
	CharCode charPath(pszDir);
	return Ex_CreateDiretory(charPath.GetStringA());
}

//����ļ��Ƿ����
bool IsFileExist(const TCHAR* pszFileName)
{
	if (!pszFileName)
	{
		return false;
	}
#ifdef _UNICODE
	if (GetFileAttributesW(pszFileName) == -1)
#else
	if (GetFileAttributesA(pszFileName) == -1)
#endif
	{
		return false;
	}

	return true;
}

//���Ŀ¼�Ƿ����
bool IsDirectoryExist(const char* psDirName)
{
	return false;
}

//���·���Ƿ���ڣ���β������򴴽�
bool CheckPathCreate(const char* pszPath)
{
	//char*t = file_name;
	//while (t = strchr(++t, '\\'))
	//{		
	//	*t = 0;		
	//	if (access(file_name, 0) != -1) 
	//	{
	//		*t = '\\';
	//		continue;
	//	}		
	//	mkdir(file_name);		
	//	*t = '\\';
	//}
	return false;
}

int GetFileLen(TCHAR* szFileName)
{
	CFile hFile;
	int nLength = 0;
	
	if (!(hFile.Open(szFileName, CFile::modeRead)))
	{
		return 0;
	}
	nLength = hFile.GetLength();
	hFile.Close();
	return nLength;
}

//���ܣ������û�и��ļ��У�û���򴴽�
//������sFolder:�ļ��е�ȫ·��
//����ֵ����
//void CheckFolder(CString sFolder)
//{
//	sFolder = sPath + "\\" + sFolder;
//	CFileFind finder;
//	if(!(finder.FindFile(sFolder)))		
//		CreateDirectory(sFolder,NULL);
//	finder.Close();
//}


//AFX_EXT_CLASS void GetAllFileName(CString&		strAllFileName,
//								  LPCTSTR	cszDirectory)
//{
//	WIN32_FIND_DATA FindFileData;
//	HANDLE hFind;
//
//	hFind = FindFirstFile(cszDirectory, &FindFileData);
//	strAllFileName = _T("");
//
//	if (hFind == INVALID_HANDLE_VALUE)
//	{
//		if(hFind)
//			FindClose(hFind);
//		strAllFileName = _T("");
//	} 
//	else 
//	{
//		CString strTempComp = _T("");
//		do 
//		{
//			strTempComp = FindFileData.cFileName;
//			if (strAllFileName.Compare(_T("")))
//			{
//				strAllFileName = strAllFileName + "," + strTempComp;
//			}
//			else
//			{
//				strAllFileName = strTempComp;
//			}
//		}while (FindNextFile(hFind,&FindFileData));
//	}
//	if(hFind)
//		FindClose(hFind);
//}

//AFX_EXT_CLASS void RemoveOldDirectory(LPCTSTR strOldDirectory)
//{
//	RemoveDirectory(strOldDirectory);
//}


//��ȡ��ǰĿ¼�Ƿ��д
BOOL GetRunDirWriten()
{
	BOOL bWriteble = FALSE;
	CFile   file;
	CString cFilePath = _T("tmp.txt");
	Write_LogW(LOG_INFO, _T("DirFileEx: Start GetRunDirWriten..."));
	if (file.Open(cFilePath.GetBuffer(0), CFile::modeCreate | CFile::modeWrite))
	{
		bWriteble = TRUE;
		file.Close();
		file.Remove(cFilePath.GetBuffer(0));
	}
	Write_LogW(LOG_INFO, _T("DirFileEx: GetRunDirWriten:%s."), bWriteble ? _T("true") : _T("false"));

	return bWriteble;
}

//Log��document������tmpĿ¼��
const TCHAR* GetLogDir()
{
	if (!g_szeClassLogPath[0])
	{
		TCHAR szTempPath[256] = { 0 };
		DWORD dwSize = 128;
		GetTempPath(dwSize, szTempPath);

		CString cstrTmpPath = szTempPath;
		cstrTmpPath += _T("eclass\\log_eClass\\");

		CharCode szDir(cstrTmpPath.GetBuffer(0));
		if (!Ex_CreateDiretory(szDir.GetStringA()))
		{
			CString cstrLogPath = GetRunDirW();
			cstrLogPath += _T("\\log_eClass\\");
			wcscpy(g_szeClassLogPath, cstrLogPath.GetBuffer(0));
		}
		else
		{
			wcscpy(g_szeClassLogPath, cstrTmpPath.GetBuffer(0));
		}
	}

	return g_szeClassLogPath;
}

//Log��document������tmpĿ¼��
const TCHAR* GetDocumentDir()
{
	if (!g_szeClassDocumentPath[0])
	{
		TCHAR szTempPath[256] = { 0 };
		DWORD dwSize = 128;
		GetTempPath(dwSize, szTempPath);

		CString cstrTmpPath = szTempPath;
		cstrTmpPath += _T("eclass\\document\\");

		CharCode szDir(cstrTmpPath.GetBuffer(0));
		if (!Ex_CreateDiretory(szDir.GetStringA()))
		{
			Write_LogW(LOG_ERROR, _T("DirFileEx: Ex_CreateDiretory error:%s."), cstrTmpPath.GetBuffer(0));
			CString cstrDocPath = GetRunDirW();
			cstrDocPath += _T("\\document\\");
			wcscpy(g_szeClassDocumentPath, cstrDocPath.GetBuffer(0));
		}
		else
		{
			wcscpy(g_szeClassDocumentPath, cstrTmpPath.GetBuffer(0));
		}
	}

	return g_szeClassDocumentPath;
}

const TCHAR* GetScreenshotDir()
{
	if (!g_szeClassScreenshotPath[0])
	{
		TCHAR szTempPath[256] = { 0 };
		DWORD dwSize = 128;
		GetTempPath(dwSize, szTempPath);

		CString cstrTmpPath = szTempPath;
		cstrTmpPath += _T("eclass\\screenshot\\");

		CharCode szDir(cstrTmpPath.GetBuffer(0));
		if (!Ex_CreateDiretory(szDir.GetStringA()))
		{
			Write_LogW(LOG_ERROR, _T("DirFileEx: Ex_CreateDiretory error:%s."), cstrTmpPath.GetBuffer(0));
			CString cstrDocPath = GetRunDirW();
			cstrDocPath += _T("\\screenshot\\");
			wcscpy(g_szeClassScreenshotPath, cstrDocPath.GetBuffer(0));
		}
		else
		{
			wcscpy(g_szeClassScreenshotPath, cstrTmpPath.GetBuffer(0));
		}
	}

	return g_szeClassScreenshotPath;
}

const TCHAR* GetAppdataDir()
{
	if (!g_szeClassAppdataPath[0])
	{
		SHGetSpecialFolderPath(NULL, g_szeClassAppdataPath, CSIDL_APPDATA, TRUE);
	}
	return g_szeClassAppdataPath;
}