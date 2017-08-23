#pragma once
#include <string>


using namespace std;

class CIni;
class CIniLoad
{
public:
	CIniLoad(string path);
	~CIniLoad();

	string GetValue(string strSection,string strKey);
	int GetIntValue(string strSection,string strKey);
	bool GetBoolValue(string strSection,string strKey);
	double GetDoubleValue(string strSection,string strKey);


	//
	string GetSectionName(int nID);
	int GetSectionNum() { return m_nSectionNum; };

	//±éÀúkey
	DWORD GetKeyAmount(string strSection);
	string GetKeyByIndex(string strSection, DWORD dwKeyIndex);

protected:
	CIni  *   ini;
	int m_nSectionNum;
};

