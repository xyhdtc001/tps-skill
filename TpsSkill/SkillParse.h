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

	bool SetSelfWorkDir(string strPath);
	
	void ParseHero(string strHeroID);


	void outPutTmeInfo();

	void outPutStateTmeInfo();

	void outPutErrorInfo();

	//查找通用TME
	void ProceeCommonTme();

	void outCommonPath();

	//查找lua中的 tme.
	void ProcessLuaTme();
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

	void GetSkillPresentByName(XMLDocument *pdoc,set<string>& setName, map<string, XMLNode*> &mapInfoP,XMLNode* pDesNode,const string &strSkillID, map<string, XMLNode*> &mapInfoPSelf, map<string, XMLNode*> &mapInfoPLink);
	
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
	void ProceeStateTme(string presentName,string strSkllID,map<string, XMLNode*> &mapInfoP);

	bool GetSkillPresentTmePathByName(string strSkllID,XMLNode* xmlNode);

	void InsetToHeroSkillMap(string skillID,int nVo,bool bCoreSKill=false);

	//处理剩下的与当前英雄有关的光效.
	void ProcessReMainStateTme(map<string, XMLNode*>& ,map<string, XMLNode*>*p=NULL);

	//处理unitdata中的技能相关.
	void ProcessUnitDataSkillPresent(map<string, XMLNode*>&,map<string, XMLNode*>*p=NULL);


	
	//prt文件中的tme直接加入通用数据.
	void AddTmePathInPrt(string strPath);
	void AddTmeFile(string strPath);


	//加载所有通用光效.
	void LoadAllCommonTmeInfo();

	//根据皮肤ID获取英雄类型.
	int  GetHeroTypeByIni(string strRoleID);

	//获取各个职业技能信息.

	//配置文件
	CIniLoad* m_pHeroSkill;//有使用的ID
	CIniLoad* m_pIniSkill;//全部ID
	CIniLoad *m_pIniSkiPresent;//技能对应表现.
	CIniLoad *m_pIniLanguage;//描述语言.
	CIniLoad *m_pDamage; //伤害表现.
	CIniLoad *m_pStateIni; //state.ini

	CIniLoad *m_pRoleInfoIni; //state.ini

	CIniLoad *m_pUnitData;

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

	string m_curHeroID;  //英雄roleID
	string m_curRoleLinkID; //英雄linkID
	string m_curProceeHeroID;//当前正在处理的英雄ID。m_curHeroID m_curRoleLinkID 2 选1 。

	STATETMEMAP m_mapStateTme;//状态光效.
	map<string,string>m_mapSKillIDAndname;//damage.ini表现信息.


	//所有通用光效map
	map<string, XMLNode*>m_mapCommonInfo;
	XMLDocument *m_pDocComonp;

	//格式不规范信息.
	map<string,vector<string>> m_mapErrorInfo;

	//技能对应光效.
	VEC_SKILLTME m_vecSkillTme;

	//处理过的光效.
	map<string,string>m_mapProcessedTmePath;

	//通用光效
	std::set<const string> m_CommonTmeVec;

	//不存在的光效.
	std::set<const string> m_NoexistTmeVec;

	set<const string> m_luaTme;

	//exe 运行路径.
	string m_curSelfRunPath;
};

