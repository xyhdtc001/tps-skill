#pragma once

#ifndef _UNIT_PRESENT_H
#define _UNIT_PRESENT_H

#include "CSKillData.h"



class SkillParse;
class CIniLoad;
class tinyxml2::XMLDocument;
class tinyxml2::XMLNode;
class tinyxml2::XMLElement;

class CSingleUnitProcess 
{
public:
	CSingleUnitProcess();
	~CSingleUnitProcess();

protected:

};



class CUinitPresentParse 
{
	
public:

	CUinitPresentParse(SkillParse* pSkillParse);
	~CUinitPresentParse();



	void InitXmlInfo();


	void ProcessUnitData();
protected:

	void loadRoleXml(string strPath);

	void ProcessSingleUnit(string strUnitID);


	void ProcessPrtFile(string filePath,set<string>& setFilePath,string strPresent,bool bHero = false);


	void GetNodeTmeInfo(tinyxml2::XMLNode* pNode,set<string>& setFilePath);

	std::map<string,ST_ROLEINFO> m_MapRoleInfo;

	string m_strWorkDir;

	//使用skillParser的信息.
	SkillParse* m_pSkillParse;

};

#endif