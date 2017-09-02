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

	//����key
	int GetKeyAmount(string strSection);
	string GetKeyByIndex(string strSection, int dwKeyIndex);

	//��ȡ��INI�ļ����е�key .(��Ҫ����һ�²Ż��ȡ���������ݡ�)
	const set<string> GetSetKey() {return m_setIniKey;};
protected:
	CIni  *   ini;
	int m_nSectionNum;
	set<string> m_setIniKey;
};

