/*********************************************************************\
 Copyright(c) 2010, ***, All Rights Reserved
 Author:	
 Created:	2010-07-23
 Describe:	Ini不允许有重复的section
\*********************************************************************/
#pragma once
#include "SimpleIni.h"
#include "BaseCode.h"

class CIni
{
public:
	explicit CIni(bool bIsUtf8 = false, 
		bool bUseMultiKey = false, 
		bool bUseMultiLine = false);
	~CIni(void);

public:
	// 加载文件
	bool LoadFile(const char* pszFile);
    bool Load( const char *    a_pData,
                    size_t          a_uDataLen
                    );
	bool SaveFile(const char* pszFileName) const;

	// 遍历section
	DWORD GetSectionAmount();
	string GetSectionByIndex(DWORD dwIndex);

	// 遍历key
	DWORD GetKeyAmount(string strSection);
	string GetKeyByIndex(string strSection, DWORD dwKeyIndex);

	// 获取value
	int GetValueInt(string strSection, string strKey, int nDefaultValue = 0);
	string GetValueString(string strSection, string strKey, string strDefaultValue = "");

	void SetValueString(string strSection, string strKey, string strValue);
private:
	CSimpleIni* m_pSimpleIni;
	
	VEC_STR m_vecSection;

	typedef map<string, VEC_STR> MAP_SECTION_KEYS;
	MAP_SECTION_KEYS m_mapSectionKeys;
};
