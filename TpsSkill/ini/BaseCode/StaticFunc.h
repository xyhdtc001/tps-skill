/*********************************************************************\
 Copyright(c) 2010, ***, All Rights Reserved
 Author:	
 Created:	2010-07-23
 Describe:	��̬������
\*********************************************************************/

#pragma once
#include "BaseCode.h"

class CStaticFunc
{
public:
	CStaticFunc(void);
	virtual ~CStaticFunc(void);


// ---------------------------------------------------------------------
// ��־
// ---------------------------------------------------------------------
public:
	// ��־��ӡ���ļ���
	static void LogMsg(const char* pszFormat, ...);
	static void LogMsgEx(const char* pszFile, const char* pszFormat, ...);

	// ��־��ӡ�����������
	static void DebugMsg(const char* pszFormat, ...);
// ---------------------------------------------------------------------
// ʱ��
// ---------------------------------------------------------------------
public:
	// ��ȡʱ��δ𣨺��룩
	static unsigned long GetTime(void);

	static void GetTime(OUT int& nYear, OUT int& nMonth, OUT int& nDay, OUT int& nHour, OUT int& nMin, OUT int& nSec);

// ---------------------------------------------------------------------
// �����
// ---------------------------------------------------------------------
public:
	static int RandGet(int nMax, bool bRealRand = false);

// ---------------------------------------------------------------------
// exe
// ---------------------------------------------------------------------
public:
	// ��ȡexe����·������/������
	static string GetCurrentExeDir();

	// ��ȡ��ǰ·��
	static string GetCurrentDir();
	static void SetCurrentDir(const string& strDir);

	// ��ȡ�ļ�·����(/)
	static string GetFilePath(const string& strFile);

    // ��ȡ�ļ�Ŀ¼
    static string GetFileDir(const string& strFile);

	// ���ز���·��Ҳ������׺���ļ���
	static string GetBaseName(const string& strFile);

    // ��ȡ����·�����ļ���
    static string GetFileName(const string& strFile);

	// ��ȡ�ļ���չ��(��.)
	static string GetFileExt(const string& strFile);

	// ·��תСд����'\\'���'/'
	static string ToLowerCase(const string& strFile);

	static string ReplaceFileExt(const string& strFile,const string& strNewExt);

	// �����ļ���
	static void MakeDir(const char* pszPath);

    static bool IsFileExist( const char* fullFilePath );

    static int FileLength(FILE* pFile);
    static int FileRead(FILE* pFile, OUT char* buf, int len);
    static string ToString(int n);

    static void StrRemoveSpaces(string& str);
    static void StrSplit(const string& str, const string& delimiter, OUT std::vector<string>& outElements);

    // ��֤���һ���ַ���/
    static void StrMakePath(IN OUT string& str);

    // ��urlΪtrue����'\'�滻Ϊ/������/�滻Ϊ'\'
    static void StrFormatPath(IN OUT string& str, bool url);
    static int StrHexToInt(const string& str);
    static bool StrIsEndWith(const string& str, const string& strEndWith, bool bCase);
// ---------------------------------------------------------------------
// �������ת��
// ---------------------------------------------------------------------
public:
	// Unicode�ַ���ת��Ansi�ַ���
	static string UnicodeToAnsi(const wchar_t* pszUnicode);

	// Ansi�ַ���ת��Unicode�ַ���
	static wstring AnsiToUnicode(const char* pszAnisi);

// ---------------------------------------------------------------------
// ini��ȡ
// ---------------------------------------------------------------------
public:
	static int GetIniInt(const char* pszFileName, const char* pszTitle, const char* pszKey);
	static string GetIniString(const char* pszFileName, const char* pszTitle, const char* pszKey);

// ---------------------------------------------------------------------
// ����
// ---------------------------------------------------------------------
public:
	static string GetMd5(const string& strBuffer);
	static string GetMd5ByFile(const char* pszFileName);
    
};

