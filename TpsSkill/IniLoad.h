#pragma once
#include <string>
#include <set>


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
	int GetSectionNum() { return m_nSectionNum; }

	//遍历key
	int GetKeyAmount(string strSection);
	string GetKeyByIndex(string strSection, int dwKeyIndex);

	//获取该INI文件所有的key .(需要遍历一下才会获取到完整数据。)
	const set<string> GetSetKey() {return m_setIniKey;};
protected:
	CIni  *   ini;
	int m_nSectionNum;
	set<string> m_setIniKey;
};

