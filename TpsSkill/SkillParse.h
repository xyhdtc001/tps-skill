#pragma once
#include <string>
#include "CSKillData.h"

using namespace std;
using namespace tinyxml2;

class CIniLoad;
class tinyxml2::XMLDocument;
class tinyxml2::XMLNode;
class tinyxml2::XMLElement;

class SkillParse
{
public:
	SkillParse();
	~SkillParse();

	
	bool SetWorkDir(string strPath);

	
	void ParseHero(string strHeroID);


	void outPutTmeInfo();

	void outPutStateTmeInfo();

	void outPutErrorInfo();

protected:

	void InitConfigFile();

	void InitXmlFile();

	void InitHeroSkillMap();

	void CreateHeroSkillInfoByXml(MAPSKILLINFO&,string strHeroID);



	//xml处理函数.
	void LoopXMlForInfo(XMLNode* pSourceNode, map<string, XMLNode*>&mapInfo);

	//递归寻找TreeNodePresentation
	void LoopForTreeNodePresentation(XMLNode *pRoot,set<string>& setName);

	//遍历setName获取技能实际表现.

	void GetSkillPresentByName(XMLDocument *pdoc,set<string>& setName, map<string, XMLNode*> &mapInfoP,XMLNode* pDesNode,const string &strSkillID);
	
	string m_strWorkDir; 

	void GetEventInfo(XMLNode *pParent, const char* pszKeyname,map<string, XMLNode*>&mapInfo,string &str24000,string &str25000,string &str26000,string &str30000,bool blower,bool bheroID=true);


	//获取所有INI键值
	void GetIniKeyAndValue(INIKEY &iniKeyValue,CIniLoad* pIniFile,string strSection,string xmlNodeName="");

	//保存键值
	void GetIniKeyAndValueAndRecoardToXml(INIKEY &iniKeyValue,CIniLoad* pIniFile,string strSection,const string& strIniName);

	void InsertKeyNameToXMl(const string &  strIniName,const string & Keyname);


	//处理state.ini,damage.ini.获取数据,(目前取技能光效.)
	void ProcessStateAndDamageInfo();
	void ProcessStateAndDamageInfoByIni(CIniLoad* pIni,STATETMEMAP& mapData);

	//状态光效处理添加.
	void ProceeStateTme(string strSkllID,map<string, XMLNode*> &mapInfoP);

	void GetSkillPresentTmePathByName(string strSkllID,XMLNode* xmlNode);


	//获取各个职业技能信息.

	//配置文件
	CIniLoad* m_pHeroSkill;//有使用的ID
	CIniLoad* m_pIniSkill;//全部ID
	CIniLoad *m_pIniSkiPresent;//技能对应表现.
	CIniLoad *m_pIniLanguage;//描述语言.
	CIniLoad *m_pDamage; //伤害表现.
	CIniLoad *m_pStateIni; //state.ini

	XMLDocument *m_pXmlRole;

	XMLDocument *m_pXmlHero1101_C;
	XMLDocument *m_pXmlHero1101_P;

	XMLDocument *m_pXmlHero1201_C;
	XMLDocument *m_pXmlHero1201_P;

	XMLDocument *m_pXmlHero1301_C;
	XMLDocument *m_pXmlHero1301_P;

	XMLDocument *m_pXmlHero1401_C;
	XMLDocument *m_pXmlHero1401_P;

	XMLDocument *m_pXmlHero1501_C;
	XMLDocument *m_pXmlHero1501_P;

	XMLDocument *m_pXmlHero1801_C;
	XMLDocument *m_pXmlHero1801_P;


	//iniKey XMl
	XMLDocument *m_pDocXmlKeyName;


	//各个职业技能表.
	MAPSKILLINFO m_Hero1101;
	MAPSKILLINFO m_Hero1201;
	MAPSKILLINFO m_Hero1301;
	MAPSKILLINFO m_Hero1401;
	MAPSKILLINFO m_Hero1501;
	MAPSKILLINFO m_Hero1801;

	MAPSKILLINFO* m_cuHeroInfo;

	//buf..

	//所有技能表现表.
	string m_curHeroID;

	STATETMEMAP m_mapStateTme;//状态光效.
	map<string,string>m_mapSKillIDAndname;

	//格式不规范信息.
	map<string,vector<string>> m_mapErrorInfo;

	//技能对应光效.
	VEC_SKILLTME m_vecSkillTme;

	//处理过的光效.
	map<string,string>m_mapProcessedTmePath;
};

