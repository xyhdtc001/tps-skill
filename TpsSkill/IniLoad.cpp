#include "stdafx.h"
#include "IniLoad.h"
#include "ini\Ini.h"



CIniLoad::CIniLoad(string path)
{
	m_nSectionNum = -1;
	ini = new CIni();
	if (!ini->LoadFile(path.c_str()))
	{
		delete ini;
		ini = NULL;
	}
	else
	{
		m_nSectionNum = ini->GetSectionAmount();
	}
	
}

CIniLoad::~CIniLoad()
{
	if (ini != NULL)
	{
		delete ini;
		ini = NULL;
	}
}

std::string CIniLoad::GetValue(string strSection,string strKey)
{
	if (ini==NULL)
	{
		return "";
	}
	string strRes = ini->GetValueString(strSection,strKey,"");
	return strRes;
}

int CIniLoad::GetIntValue(string strSection,string strKey)
{
	if (ini == NULL)
	{
		return 0;
	}
	int nRes = ini->GetValueInt(strSection,strKey,0);
	return nRes;
}


std::string CIniLoad::GetSectionName(int nID)
{
	if (nID > m_nSectionNum)
	{
		return "";
	}
	string strRes = ini->GetSectionByIndex(nID);
	return strRes;
}

DWORD CIniLoad::GetKeyAmount(string strSection)
{
	if (ini == NULL)
	{
		return 0;
	}
	DWORD nRes = ini->GetKeyAmount(strSection);
	return nRes;

}

std::string CIniLoad::GetKeyByIndex(string strSection, DWORD dwKeyIndex)
{
	if (ini == NULL)
	{
		return 0;
	}
	string nRes = ini->GetKeyByIndex(strSection,dwKeyIndex);
	return nRes;
}