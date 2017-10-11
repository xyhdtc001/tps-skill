#include "stdafx.h"
#include "SkillParse.h"
#include "IniLoad.h"
#include <direct.h> 
#include "FileLineScan.h"
#include "luaOprate.h"
#include "CXmlOprate.h"



SkillParse::SkillParse():
	m_pHeroSkill(0),m_pIniSkill(0),m_pIniSkiPresent(0),m_pIniLanguage(0),m_pStateIni(0),m_cuHeroInfo(0),
	m_pDamage(0),m_pXmlRole(0),m_pXmlHero1101_C(0),m_pXmlHero1101_P(0),m_pXmlHero1201_C(0),m_pXmlHero1201_P(0),
	m_pXmlHero1301_C(0),m_pXmlHero1301_P(0),m_pXmlHero1401_C(0),m_pXmlHero1401_P(0),m_pXmlHero1501_C(0),m_pXmlHero1501_P(0),
	m_pXmlHero1801_C(0),m_pXmlHero1801_P(0),m_strWorkDir(""),m_pUnitData(0),m_pRoleInfoIni(0)
{
	m_CommonTmeVec.clear();
	m_NoexistTmeVec.clear();
	m_mapProcessedTmePath.clear();
	m_vecSkillTme.clear();
	m_mapErrorInfo.clear();
	m_mapSKillIDAndname.clear();
	m_pDocComonp = NULL;
	m_curSelfRunPath.clear();
}


SkillParse::~SkillParse()
{
	if (m_pDocComonp != NULL)
	{
		m_pDocComonp->Clear();
		delete m_pDocComonp;
		m_pDocComonp  = NULL;
	}
}

bool SkillParse::SetWorkDir(string strPath)
{
	m_strWorkDir = strPath;
	InitConfigFile();
	//分类英雄技能.
	InitHeroSkillMap();

	string strDir = strPath+"\\processSkillRes\\";
	_mkdir(strDir.c_str())  ;
	return true;
}

bool SkillParse::SetSelfWorkDir(string strPath)
{
	m_curSelfRunPath = GetDir(strPath);
	return true;
}

void SkillParse::ParseHero(string strHeroID)
{
	int nVoa = GetHeroTypeByIni(strHeroID);
	if (nVoa == 0)
	{
		nVoa = GetHeroTypeByIni(m_pUnitData->GetValue(strHeroID,"linkroleid"));//useskas 动作相关.,linkroleid 皮肤相关，主程序处理会将linkid转为role id.
	}

	if (nVoa == 1)
	{
		CreateHeroSkillInfoByXml(m_Hero1101, strHeroID);
	}
	else if (nVoa == 2)
	{
		CreateHeroSkillInfoByXml(m_Hero1201, strHeroID);
	}
	else if (nVoa == 3)
	{
		CreateHeroSkillInfoByXml(m_Hero1301, strHeroID);
	}
	else if (nVoa == 4)
	{
		CreateHeroSkillInfoByXml(m_Hero1401, strHeroID);
	}
	else if (nVoa == 8)
	{
		CreateHeroSkillInfoByXml(m_Hero1801, strHeroID);
	}
	else if (nVoa == 9)
	{
		CreateHeroSkillInfoByXml(m_Hero1901, strHeroID);
	}
	
}

void SkillParse::InitConfigFile()
{
	//skill Init;
	if (m_pIniSkill != NULL)
	{
		delete m_pIniSkill;
		m_pIniSkill = NULL;
	}
	string strSkillIniFile = m_strWorkDir+"\\data\\tpconfig\\skill.ini";
		m_pIniSkill = new CIniLoad(strSkillIniFile);
	//hero skill
	if (m_pHeroSkill != NULL)
	{
		delete m_pHeroSkill;
		m_pHeroSkill = NULL;
	}
	 strSkillIniFile = m_strWorkDir + "\\data\\config\\glyphsconfig.ini";
	m_pHeroSkill = new CIniLoad(strSkillIniFile);
	//skill present
	if (m_pIniSkiPresent != NULL)
	{
		delete m_pIniSkiPresent;
		m_pIniSkiPresent = NULL;
		
	}
	 strSkillIniFile = m_strWorkDir + "\\data\\tpconfig\\skillpresentation.ini";
	m_pIniSkiPresent = new CIniLoad(strSkillIniFile);
	//语言描述.
	if (m_pIniLanguage != NULL)
	{
		delete m_pIniLanguage;
		m_pIniLanguage = NULL;
		
	}
	 strSkillIniFile = m_strWorkDir + "\\data\\config\\language.ini";
	m_pIniLanguage = new CIniLoad(strSkillIniFile);
	//伤害表现 。
	if (m_pDamage != NULL)
	{
		delete m_pDamage;
		m_pDamage = NULL;
		
	}
	strSkillIniFile = m_strWorkDir + "\\data\\tpconfig\\damage.ini";
	m_pDamage = new CIniLoad(strSkillIniFile);
	// state.ini
	if (m_pStateIni != NULL)
	{
		delete m_pStateIni;
		m_pStateIni = NULL;

	}
	strSkillIniFile = m_strWorkDir + "\\data\\tpconfig\\state.ini";
	m_pStateIni = new CIniLoad(strSkillIniFile);

	//unitdata.ini
	if (m_pUnitData != NULL)
	{
		delete m_pUnitData;
		m_pUnitData = NULL;

	}
	strSkillIniFile = m_strWorkDir + "\\data\\tpconfig\\unitdata.ini";
	m_pUnitData = new CIniLoad(strSkillIniFile);

	//本地ROLEID ini
	if (m_pRoleInfoIni != NULL)
	{
		delete m_pRoleInfoIni;
		m_pRoleInfoIni = NULL;
	}
	
	strSkillIniFile = m_curSelfRunPath+"tpsSkill.ini";
	m_pRoleInfoIni = new CIniLoad(strSkillIniFile);

	//初始化
	LoadAllCommonTmeInfo();
	ProcessStateAndDamageInfo();
}

void SkillParse::InitXmlFile()
{

}

void SkillParse::InitHeroSkillMap()
{
	if (m_pHeroSkill == NULL)
	{
		return;
	}
	int nNum = m_pHeroSkill->GetSectionNum();
	for (int nIndex = 0; nIndex < nNum;++nIndex)
	{
		string strSecName = m_pHeroSkill->GetSectionName(nIndex);


		//区别职业
		int nVocation = m_pHeroSkill->GetIntValue(strSecName,"vocation");
		if (nVocation <=0 || nVocation >9)
		{
			continue;
		}

		int nGrade = m_pHeroSkill->GetIntValue(strSecName,"grade");
		if (nGrade==4)
		{
			//核心被动.
			InsetToHeroSkillMap(strSecName,nVocation,true);
		}

		string strName = m_pHeroSkill->GetValue(strSecName,"talent");

		
		InsetToHeroSkillMap(strName,nVocation);
	}

}
void SkillParse::InsetToHeroSkillMap(string skillID,int nVocation,bool bCoreSKill)
{

	SKILLDESCRIBLEDATA _st_skillDescrig = {0};
	_st_skillDescrig.strID = skillID;
	_st_skillDescrig.bCoreSkill = bCoreSKill;
	//获取INI值
	GetIniKeyAndValue(_st_skillDescrig.vec_key,m_pHeroSkill,skillID);
	if (nVocation == 1 )
	{
		//战士
		m_Hero1101.insert(make_pair(skillID, _st_skillDescrig));
	}
	else if(nVocation == 2)
	{
		//法师
		m_Hero1201.insert(make_pair(skillID, _st_skillDescrig));
	}
	else if (nVocation == 3)
	{
		//木事
		m_Hero1301.insert(make_pair(skillID, _st_skillDescrig));
	}
	else if (nVocation == 4)
	{
		//骑士
		m_Hero1401.insert(make_pair(skillID, _st_skillDescrig));
	}
	else if (nVocation == 8)
	{
		//影舞者
		m_Hero1801.insert(make_pair(skillID, _st_skillDescrig));
	}
	else if (nVocation == 9)
	{
		//影舞者
		m_Hero1901.insert(make_pair(skillID, _st_skillDescrig));
	}
	//nextskill_1 nextskill_2
	string strValue =  m_pIniSkill->GetValue(skillID,"nextskill_1");
	if (strValue.size()>0)
	{
		InsetToHeroSkillMap(strValue,nVocation);
	}
	strValue =  m_pIniSkill->GetValue(skillID,"nextskill_2");
	if (strValue.size()>0)
	{
		InsetToHeroSkillMap(strValue,nVocation);
	}
}

/*
 *	查找通用tme
 */
void SkillParse::ProceeCommonTme()
{
	//获取state中的comon present
	STATETMEMAP::iterator skillIter = m_mapStateTme.begin();
	int nIndex= 0;
	char szNameID[100];
	while(skillIter != m_mapStateTme.end())
	{
		nIndex++;
		sprintf(szNameID,"Common%d",nIndex);
		string strCommonID = szNameID;

		string strvalue;
		strvalue = "";
		if (skillIter->second.usedskill.size() > 0)
		{
			++skillIter;
			continue;
		}
		string strPresentName = skillIter->first;
		string strRealName;
		GetSegmentString(strPresentName,"",";",strRealName);
		if (strRealName.length()<=0)
		{
			m_mapErrorInfo[strCommonID].push_back("start|");
			m_mapErrorInfo[strCommonID].push_back("commond name error");
			m_mapErrorInfo[strCommonID].push_back(skillIter->first);
			m_mapErrorInfo[strCommonID].push_back("|End")	;	
			++skillIter;
			continue;
		}

		if (m_mapCommonInfo.find(strRealName) == m_mapCommonInfo.end())
		{
			m_mapErrorInfo[strCommonID].push_back("start|");
			m_mapErrorInfo[strCommonID].push_back("commond name No Find");
			m_mapErrorInfo[strCommonID].push_back(skillIter->first);
			m_mapErrorInfo[strCommonID].push_back("|End")	;	
			++skillIter;
			continue;
		}
		XMLNode* pNode= m_mapCommonInfo[strRealName];
		bool bRes = GetSkillPresentTmePathByName("common",pNode);
		if (bRes)
		{
			skillIter->second.usedskill.insert("common");
		}
		++skillIter;
	}

// 	//贴花。
	string strDirPath = m_strWorkDir+"\\effect\\decal\\";
	AddTmeFile(strDirPath);

	strDirPath = m_strWorkDir+"\\effect\\ui\\battle_field\\";
	AddTmeFile(strDirPath);
// 
	strDirPath = m_strWorkDir+"\\effect\\maps\\victory_petal\\";
	AddTmeFile(strDirPath);


	//扫描lua pvpzdjmctrl.lua E:\hero\tpsgame-runpath_1.6\client1.5\data\ui\addons\pvpzdjm\pvpzdjmctrl.lua
	string strFilePath= m_strWorkDir+"\\data\\ui\\addons\\pvpzdjm\\pvpzdjmctrl.lua";
	CFileLineScan lineScan;
	lineScan.ProcessFile(strFilePath,m_CommonTmeVec);

	//E:\hero\tpsgame-runpath_1.6\client1.5\data\tpconfig\effectdata.lua
	strFilePath= m_strWorkDir+"\\data\\tpconfig\\effectdata.lua";
	lineScan.ProcessFile(strFilePath,m_CommonTmeVec);

// 	//按钮
// 	strDirPath = m_strWorkDir+"\\effect\\ui\\";
// 	AddTmeFile(strDirPath);
	string strCommonRidePrt = m_strWorkDir+"\\presentations\\common\\commonride.prt";
	AddTmePathInPrt(strCommonRidePrt);
}

/*
 *	prt文件中的tme直接加入通用数据.
 */
void SkillParse::AddTmePathInPrt(string strPath)
{
	XMLDocument *pDocPrt = new XMLDocument;
	if (pDocPrt->LoadFile(strPath.c_str())!=XML_SUCCESS)
	{
		delete pDocPrt;
		return;
	}
	XMLNode *pNode_p = pDocPrt->FirstChild();
	if (pNode_p==NULL)
	{
		delete pDocPrt;
		return;
	}
	//遍历载入信息.
	map<string, XMLNode*>mapPInfo;
	LoopXMlForInfo(pNode_p,mapPInfo);

	//遍历加入通用数据.
	XMLNode* pNodePresent = pNode_p->FirstChildElement("Presentation") ;
	while(pNodePresent)
	{
		 GetSkillPresentTmePathByName("common",pNodePresent);
		 pNodePresent = pNodePresent->NextSiblingElement("Presentation");
	}

	pDocPrt->Clear();
	delete pDocPrt;
	pDocPrt = NULL;
}

/*
 *	目录下的tme全部加入common
 */
void SkillParse::AddTmeFile(string strPath)
{
	//扫描目录下的 tme文件.
	_findFile("*.tme",strPath.c_str(),m_CommonTmeVec);
}

void SkillParse::LoadAllCommonTmeInfo()
{
	//加载通用Prt
	string strCommonPrt = m_strWorkDir+"\\presentations\\common\\common.prt";

	// 	string strCommonRidePrt = m_strWorkDir+"\\presentations\\common\\commonride.prt";
	// 	AddTmePathInPrt(strCommonRidePrt);
	if (m_pDocComonp != NULL)
	{
		m_pDocComonp->Clear();
		delete m_pDocComonp;
		m_pDocComonp = NULL;
	}
	XMLDocument *m_pDocComonp = new XMLDocument;
	if (m_pDocComonp->LoadFile(strCommonPrt.c_str()) != XML_SUCCESS)
	{
		m_pDocComonp->Clear();
		m_pDocComonp = NULL;
		return;
	}
	XMLNode *pNode_p = m_pDocComonp->FirstChild();
	LoopXMlForInfo(pNode_p,m_mapCommonInfo);
}

int SkillParse::GetHeroTypeByIni(string strRoleID)
{
	return m_pRoleInfoIni->GetIntValue(strRoleID,"vocation");
}

void SkillParse::CreateHeroSkillInfoByXml(MAPSKILLINFO &mapInfo, string strHeroID)
{
	//加载xml文件.
	string strP = m_strWorkDir + "\\presentations\\player\\hero_";
	strP += strHeroID;
	strP += "\\hero_";
	strP += strHeroID;
	strP += "_p.prt";
	string strC = m_strWorkDir + "\\presentations\\player\\hero_";
	strC += strHeroID;
	strC += "\\hero_";
	strC += strHeroID;
	strC += "_c.prt";
	m_curHeroID = strHeroID;
	m_cuHeroInfo = &mapInfo;

	//遍历载入信息.
	CXmlOprate SelfXml_P;
	CXmlOprate SelfXml_C;
	map<string, XMLNode*>mapCInfo;
	map<string, XMLNode*>mapPInfo;

	SelfXml_C.InitXmlFile(strC);
	SelfXml_P.InitXmlFile(strP);
	LoopXMlForInfo(SelfXml_C.GetNodePtr(),mapCInfo);
	LoopXMlForInfo(SelfXml_P.GetNodePtr(),mapPInfo);


	//载入linkID信息.
	CXmlOprate LinkXml_P;
	CXmlOprate LinkXml_C;
	map<string, XMLNode*>mapCInfo_Link;
	mapCInfo_Link.clear();
	map<string, XMLNode*>mapPInfo_Link;
	mapPInfo_Link.clear();
	m_curRoleLinkID = m_pUnitData->GetValue(m_curHeroID,"linkroleid");
	if (m_curRoleLinkID.length()>0 && m_pUnitData->IsSectionName(m_curRoleLinkID))
	{
		//存在linkID..需要处理.
		//加载xml文件.
		string strP_Link = m_strWorkDir + "\\presentations\\player\\hero_";
		strP_Link += m_curRoleLinkID;
		strP_Link += "\\hero_";
		strP_Link += m_curRoleLinkID;
		strP_Link += "_p.prt";
		string strC_Link = m_strWorkDir + "\\presentations\\player\\hero_";
		strC_Link += m_curRoleLinkID;
		strC_Link += "\\hero_";
		strC_Link += m_curRoleLinkID;
		strC_Link += "_c.prt";
		LinkXml_C.InitXmlFile(strC_Link);
		LinkXml_P.InitXmlFile(strP_Link);
		LoopXMlForInfo(LinkXml_C.GetNodePtr(),mapCInfo_Link);
		LoopXMlForInfo(LinkXml_P.GetNodePtr(),mapPInfo_Link);
	}

	if (mapCInfo.size()==0&&mapCInfo_Link.size()==0)
	{
		return;
	}

	//读取技能信息。写入XML文件..
	XMLDocument *pDoc = new XMLDocument();

	XMLElement *pEl = pDoc->NewElement("HeroSkill");
	XMLNode *pRootNode = pDoc->InsertFirstChild(pEl);

	MAPSKILLINFO::iterator mapIter = mapInfo.begin();
	while (mapIter!=mapInfo.end())
	{
		SKILLDESCRIBLEDATA& _skillInfo = mapIter->second;

		if (_skillInfo.bCoreSkill == true)
		{
			//核心被动光效..处理.
			CLuaOprate op;
			string strPath = ";"; 
			strPath+= m_strWorkDir;
			strPath+="/data/config/?.lua";
			ReplaceLoopString(strPath,"\\","/");
			unsigned int nID = _ttoi(_skillInfo.strID.c_str());
			string strIDKey = m_curHeroID+"_";
			strIDKey+=_skillInfo.strID;
			op.runLua(strPath,nID,m_vecSkillTme[strIDKey]);
			string strName = m_pHeroSkill->GetValue(_skillInfo.strID,"talent");
			if (strName.length() > 0 )
			{
				string strIDKeyTalent = m_curHeroID+"_";
				strIDKeyTalent+=strName;
				set<string>::iterator iterCoreTme = m_vecSkillTme[strIDKey].begin();
				while(iterCoreTme != m_vecSkillTme[strIDKey].end())
				{
					//获取talent
					m_vecSkillTme[strIDKeyTalent].insert(*iterCoreTme);
					++iterCoreTme;
				}
			}

			++mapIter;
			continue;;
		}

		//技能名字.
		string strSkillName;
		{
			string strKey= "skillname";
			string strNameID =  m_pIniSkill->GetValue(_skillInfo.strID,strKey.c_str());

			strSkillName = m_pIniLanguage->GetValue("tps-skillconfig",strNameID.c_str());
			_skillInfo.strName = strSkillName;
		}
		
		//技能表现名称.


		//*** 新XML操作
		XMLElement *pSKillNewEl = pDoc->NewElement("skillnode");
		if (pSKillNewEl == NULL)
		{
			++mapIter;
			continue;
		}
		XMLNode *pSKillNewNode = pRootNode->InsertEndChild(pSKillNewEl);
		pSKillNewEl->SetAttribute("skillname",strSkillName.c_str());
		pSKillNewEl->SetAttribute("skillid",_skillInfo.strID.c_str());
		//类型，时间.
		pSKillNewEl->SetAttribute("skilltype",m_pIniSkill->GetValue(_skillInfo.strID,"skilltype").c_str());
		pSKillNewEl->SetAttribute("skilltime",m_pIniSkill->GetValue(_skillInfo.strID,"skillchanttime").c_str());
		//**

		string strActionName;
		do 
		{
			strActionName = m_pIniSkiPresent->GetValue(_skillInfo.strID,m_curHeroID);
			if (strActionName.length() > 0)
			{
				break;
			}
			strActionName = m_pIniSkiPresent->GetValue(_skillInfo.strID,m_curRoleLinkID);
			if (strActionName.length() > 0)
			{
				break;
			}
			string strKey ="default";
			strActionName = m_pIniSkiPresent->GetValue(_skillInfo.strID,strKey.c_str());
		} while (0);
			
		pSKillNewEl->SetAttribute("actionname",strActionName.c_str());
		if (strActionName.length() == 0)
		{
			++mapIter;
			continue;
		}
		
		map<string, XMLNode*>* ProcessMap_P = &mapPInfo;
		map<string, XMLNode*>* ProcessMap_C = &mapCInfo;
		m_curProceeHeroID = m_curHeroID;
		
		if (mapCInfo.find(strActionName)==mapCInfo.end())
		{
			//c_prt缺少配置.尝试寻找linkID.
			if(mapCInfo_Link.find(strActionName)==mapCInfo_Link.end())
			{
				++mapIter;
				continue;
			}
			//linkID  存在.
			ProcessMap_P = &mapPInfo_Link;
			ProcessMap_C = &mapCInfo_Link;
			m_curProceeHeroID = m_curRoleLinkID;
		}


		//技能名字识别检查.
		string strSkillPingyin = "";
		string strTemp;
		do 
		{
			string strSkillTemp2;
			strTemp = GetSegmentString(strActionName,"","_",strSkillTemp2);
			if (strSkillTemp2!="sk" || strTemp.length()<=0)
			{
				m_mapErrorInfo[_skillInfo.strID].push_back("start|");
				m_mapErrorInfo[_skillInfo.strID].push_back("crtName_err!");
				m_mapErrorInfo[_skillInfo.strID].push_back(strActionName);
				m_mapErrorInfo[_skillInfo.strID].push_back("|End")	;	
				break;
			}
			strTemp = GetSegmentString(strTemp,"","_",strSkillPingyin);
			strSkillPingyin = TranToSkillName(strSkillPingyin);
			if (strTemp.length()<=0)
			{
				m_mapErrorInfo[_skillInfo.strID].push_back("start|");
				m_mapErrorInfo[_skillInfo.strID].push_back("crtName_err!");
				m_mapErrorInfo[_skillInfo.strID].push_back(strActionName);
				m_mapErrorInfo[_skillInfo.strID].push_back("|End");
				break;
			}
			TrimString(strTemp);
			if (strTemp!=m_curProceeHeroID)
			{
				m_mapErrorInfo[_skillInfo.strID].push_back("start|");
				m_mapErrorInfo[_skillInfo.strID].push_back("crtName_err!");
				m_mapErrorInfo[_skillInfo.strID].push_back(strActionName);
				m_mapErrorInfo[_skillInfo.strID].push_back("|End");
				break;
			}

		} while (0);

		string strSkill = m_curHeroID+"_";
		m_mapSKillIDAndname[_skillInfo.strID] = strSkillPingyin; //id--技能名字拼音.--


		XMLNode *pCurCNode = (*ProcessMap_C)[strActionName];

		set < string> _100SetName;
		//100动作.
		{
			XMLElement *pChildEl = pCurCNode->FirstChildElement("SkillPhase");
			while (pChildEl != NULL)
			{
				if (pChildEl&&pChildEl->Attribute("id", "100"))
				{
					//递归寻找 TreeNodePresentation.
					LoopForTreeNodePresentation(pChildEl, _100SetName);
					break;
				}
				pChildEl =pChildEl->NextSiblingElement("SkillPhase");
			}
			
			//遍历setName获取技能实际表现.
			XMLElement *p100ActionEl = pDoc->NewElement("sk_100action");
			XMLNode *p100Node =pSKillNewNode->InsertEndChild(p100ActionEl);
			GetSkillPresentByName(pDoc,_100SetName,*ProcessMap_P, p100Node,_skillInfo.strID,mapPInfo,mapPInfo_Link);
		}
		//技能200动作.
		set < string> _200SetName;
		{
			XMLElement *pChildEl = pCurCNode->FirstChildElement("SkillPhase");
			while (pChildEl != NULL)
			{
				if (pChildEl&&pChildEl->Attribute("id", "200"))
				{
					//递归寻找 TreeNodePresentation.
					LoopForTreeNodePresentation(pChildEl, _200SetName);
					break;
				}
				pChildEl = pChildEl->NextSiblingElement("SkillPhase");
			}
			XMLElement *p200ActionEl = pDoc->NewElement("sk_200action");
			XMLNode *p200Node = pSKillNewNode->InsertEndChild(p200ActionEl);
			GetSkillPresentByName(pDoc,_200SetName, *ProcessMap_P, p200Node,_skillInfo.strID,mapPInfo,mapPInfo_Link);
		}

		++mapIter;
	}

	//处理state中名字不规范的字段.
	ProcessReMainStateTme(mapPInfo,&mapPInfo_Link);

	//处理unitdata中的职业相关.()
	ProcessUnitDataSkillPresent(mapPInfo,&mapPInfo_Link);

	//保存
	string strNewFile = m_strWorkDir;
	strNewFile += "\\processSkillRes\\hero_";
	strNewFile += strHeroID;
	strNewFile += ".xml";
	pDoc->SaveFile(strNewFile.c_str());

	pDoc->Clear();
	delete pDoc;
	pDoc=NULL;

}



void SkillParse::LoopXMlForInfo(XMLNode* pRoot, map<string, XMLNode*>&mapInfo)
{
	if (pRoot == NULL)
	{
		return;
	}
	XMLNode *pCurNode = pRoot->FirstChild();
	char szValueTemp[200] = { 0 };

	while (pCurNode != NULL)
	{
		XMLElement *pCurElement = pCurNode->ToElement();
		if (pCurElement == NULL)
		{
			pCurNode = pCurNode->NextSibling();
			continue;
		}
		const char *szNameValue = pCurElement->Attribute("name");
		if (szNameValue == NULL)
		{
			pCurNode = pCurNode->NextSibling();
			continue;
		}
		string strName = szNameValue;
		mapInfo.insert(make_pair(strName, pCurNode));
		pCurNode = pCurNode->NextSibling();
	}

}

void SkillParse::LoopForTreeNodePresentation(XMLNode *pRoot, set<string>& setName)
{
	XMLNode *pChild = pRoot->FirstChild();
	while (pChild != NULL)
	{
		if (strcmp(pChild->Value(), "TreeNodePresentation") == 0)
		{
			if (pChild->ToElement() &&
				pChild->ToElement()->Attribute("presentationName"))
			{
				setName.insert(pChild->ToElement()->Attribute("presentationName"));
			}
			
		}
		else
		{
			LoopForTreeNodePresentation(pChild, setName);
		}
		pChild = pChild->NextSibling();
	}
}

void SkillParse::GetSkillPresentByName(XMLDocument *pdoc,set<string>& setName, map<string, XMLNode*> &mapInfoP, XMLNode* pDesNode,const string &strSkillID, map<string, XMLNode*> &mapInfoPSelf, map<string, XMLNode*> &mapInfoPLink)
{
	set<string>::iterator setIter = setName.begin();
	
	while (setIter != setName.end())
	{
		string strKeyName = (*setIter);

		//识别名字.
		string strSkillPingyin = strKeyName;
		string strHero;
		bool bError = false;
		do 
		{
			int nCharNum = getCharNum(strSkillPingyin,'_');
			if ( nCharNum != 2 && nCharNum != 1 )
			{
				if (!bError)
				{
					m_mapErrorInfo[strSkillID].push_back("start|");
					m_mapErrorInfo[strSkillID].push_back("p_prt _Number Error");
					m_mapErrorInfo[strSkillID].push_back(strKeyName);
					m_mapErrorInfo[strSkillID].push_back("|End");
					bError = true;
				}
			}
			string strTemp = m_curProceeHeroID+"_";
			if (strSkillPingyin.find(strTemp)!=0)
			{
				if (!bError)
				{
					m_mapErrorInfo[strSkillID].push_back("start|");
					m_mapErrorInfo[strSkillID].push_back("p_prt no skill Error");
					m_mapErrorInfo[strSkillID].push_back(strKeyName);
					m_mapErrorInfo[strSkillID].push_back("|End");
					bError = true;
				}	
			}
			if (!GetSkillName(strSkillPingyin,strHero))
			{
				if (!bError)
				{
					m_mapErrorInfo[strSkillID].push_back("start|");
					m_mapErrorInfo[strSkillID].push_back("p_prt name Error");
					m_mapErrorInfo[strSkillID].push_back(strKeyName);
					m_mapErrorInfo[strSkillID].push_back("|End");
					bError = true;
				}
			
			}
			string strAllName = strSkillPingyin;
			if (strAllName != m_mapSKillIDAndname[strSkillID])
			{
				if (!bError)
				{
					m_mapErrorInfo[strSkillID].push_back("start|");
					m_mapErrorInfo[strSkillID].push_back("p_prt name and c_prt No march");
					m_mapErrorInfo[strSkillID].push_back(strKeyName);
					m_mapErrorInfo[strSkillID].push_back(m_mapSKillIDAndname[strSkillID]);
					m_mapErrorInfo[strSkillID].push_back("|End");
					bError = true;
				}
				
			}
		} while (0);

		//m_mapSKillIDAndname[strSkillID] = strSkillPingyin ;

		if (mapInfoP.find(strKeyName)==mapInfoP.end())
		{
			++setIter;
			continue;
		}
		XMLNode * pSkillNode = mapInfoP[strKeyName];
		
		XMLElement *pSkillEl = pSkillNode->ToElement();
		if (pSkillEl == NULL)
		{
			++setIter;
			continue;
		}
		XMLElement*pCurEl = pdoc->NewElement("action");
		XMLNode *pCurNode = pDesNode->InsertEndChild(pCurEl);
		pCurNode->ToElement()->SetAttribute("actionName", strKeyName.c_str());
		bool bUsd = false;

		

		//usdcast
		const char * szUsdcast = pSkillEl->Attribute("usdcast");
		if (szUsdcast)
		{
			pCurNode->ToElement()->SetAttribute("usdcast", szUsdcast);
			{

				string str24000Fps;
				string str25000Fps;
				string str26000Fps;
				string str30000Fps;
				GetEventInfo(pSkillNode,szUsdcast,mapInfoP,str24000Fps,str25000Fps,str26000Fps,str30000Fps,true);
				if (str24000Fps.length()>0||str25000Fps.length()>0||str26000Fps.length()>0||str30000Fps.length()>0)
				{
					XMLElement*pCurElNew = pdoc->NewElement("casteventLower");
					XMLNode *pCurNodeNew = pCurNode->InsertEndChild(pCurElNew);
					pCurNodeNew->ToElement()->SetAttribute("event24000", str24000Fps.c_str());
					pCurNodeNew->ToElement()->SetAttribute("event25000", str25000Fps.c_str());
					pCurNodeNew->ToElement()->SetAttribute("event26000", str26000Fps.c_str());
					pCurNodeNew->ToElement()->SetAttribute("event30000", str30000Fps.c_str());
				}
			}
			{
				string str24000Fps;
				string str25000Fps;
				string str26000Fps;
				string str30000Fps;
				GetEventInfo(pSkillNode,szUsdcast,mapInfoP,str24000Fps,str25000Fps,str26000Fps,str30000Fps,false);
				if (str24000Fps.length()>0||str25000Fps.length()>0||str26000Fps.length()>0||str30000Fps.length()>0)
				{
					XMLElement*pCurElNew = pdoc->NewElement("casteventUp");
					XMLNode *pCurNodeNew = pCurNode->InsertEndChild(pCurElNew);
					pCurNodeNew->ToElement()->SetAttribute("event24000", str24000Fps.c_str());
					pCurNodeNew->ToElement()->SetAttribute("event25000", str25000Fps.c_str());
					pCurNodeNew->ToElement()->SetAttribute("event26000", str26000Fps.c_str());
					pCurNodeNew->ToElement()->SetAttribute("event30000", str30000Fps.c_str());
				}
			}


		}
		//usdchant
		const char * szUsdchant = pSkillEl->Attribute("usdchant");
		if (szUsdchant)
		{
			pCurNode->ToElement()->SetAttribute("usdchant", szUsdchant);
			{
				string str24000Fps;
				string str25000Fps;
				string str26000Fps;
				string str30000Fps;
				GetEventInfo(pSkillNode,szUsdchant,mapInfoP,str24000Fps,str25000Fps,str26000Fps,str30000Fps,true);
				if (str24000Fps.length()>0||str25000Fps.length()>0||str26000Fps.length()>0||str30000Fps.length()>0)
				{
					XMLElement*pCurElNew = pdoc->NewElement("chanteventLower");
					XMLNode *pCurNodeNew = pCurNode->InsertEndChild(pCurElNew);
					pCurNodeNew->ToElement()->SetAttribute("event24000", str24000Fps.c_str());
					pCurNodeNew->ToElement()->SetAttribute("event25000", str25000Fps.c_str());
					pCurNodeNew->ToElement()->SetAttribute("event26000", str26000Fps.c_str());
					pCurNodeNew->ToElement()->SetAttribute("event30000", str30000Fps.c_str());
				}
			}
			{
				string str24000Fps;
				string str25000Fps;
				string str26000Fps;
				string str30000Fps;
				GetEventInfo(pSkillNode,szUsdchant,mapInfoP,str24000Fps,str25000Fps,str26000Fps,str30000Fps,false);
				if (str24000Fps.length()>0||str25000Fps.length()>0||str26000Fps.length()>0||str30000Fps.length()>0)
				{
					XMLElement*pCurElNew = pdoc->NewElement("chanteventUp");
					XMLNode *pCurNodeNew = pCurNode->InsertEndChild(pCurElNew);
					pCurNodeNew->ToElement()->SetAttribute("event24000", str24000Fps.c_str());
					pCurNodeNew->ToElement()->SetAttribute("event25000", str25000Fps.c_str());
					pCurNodeNew->ToElement()->SetAttribute("event26000", str26000Fps.c_str());
					pCurNodeNew->ToElement()->SetAttribute("event30000", str30000Fps.c_str());
				}
			}
		}
		//usdcastseq
		const char * szUsdseq = pSkillEl->Attribute("usdcastseq");
		if (szUsdseq)
		{
			pCurNode->ToElement()->SetAttribute("usdcastseq", szUsdseq);
		}
		//usdtgapresent
		const char * szUsdtga = pSkillEl->Attribute("usdtgapresent");
		if (szUsdtga)
		{
			pCurNode->ToElement()->SetAttribute("usdtgapresent", szUsdtga);
		}
		//event

			//遍历下child
			XMLElement* pChildNode = pSkillEl->FirstChildElement("FrameEvent");
			while (pChildNode!=NULL)
			{
				if (!szUsdcast&&!szUsdchant&&!szUsdseq)
				{
					if (pChildNode->Attribute("type","EmptyFrameEvent")&&!pChildNode->Attribute("triggerByProgram","true"))
					{
						if (pChildNode->Attribute("EventID","24000")&&pChildNode->Attribute("Frame"))
						{
							pCurNode->ToElement()->SetAttribute("event24000",pChildNode->Attribute("Frame"));
						}
						if (pChildNode->Attribute("EventID","25000")&&pChildNode->Attribute("Frame"))
						{
							pCurNode->ToElement()->SetAttribute("event25000",pChildNode->Attribute("Frame"));
						}
						if (pChildNode->Attribute("EventID","26000")&&pChildNode->Attribute("Frame"))
						{
							pCurNode->ToElement()->SetAttribute("event26000",pChildNode->Attribute("Frame"));
						}
						if (pChildNode->Attribute("EventID","30000")&&pChildNode->Attribute("Frame"))
						{
							pCurNode->ToElement()->SetAttribute("event30000",pChildNode->Attribute("Frame"));
						}
					}
				}
				//TME光效处理.
				if (pChildNode->Attribute("type","TmeEvent"))
				{
					//
					XMLElement* pChildTmeNode = pChildNode->FirstChildElement("Tme");
					while(pChildTmeNode != NULL)
					{
						if (pChildTmeNode->Attribute("path"))
						{
							string strIDTemp = m_curHeroID+"_";
							strIDTemp+=strSkillID;
							m_vecSkillTme[strIDTemp].insert(pChildTmeNode->Attribute("path"));
						}					
						pChildTmeNode = pChildTmeNode->NextSiblingElement("Tme");
					}
					
				}

				
				pChildNode = pChildNode->NextSiblingElement("FrameEvent");
			}

		if (!szUsdcast&&!szUsdchant&&!szUsdseq)
		{
			pCurNode->ToElement()->SetAttribute("usdself", strKeyName.c_str());
			{
				string str24000Fps;
				string str25000Fps;
				string str26000Fps;
				string str30000Fps;
				GetEventInfo(pSkillNode,strKeyName.c_str(),mapInfoP,str24000Fps,str25000Fps,str26000Fps,str30000Fps,true,false);
				if (str24000Fps.length()>0||str25000Fps.length()>0||str26000Fps.length()>0||str30000Fps.length()>0)
				{
					XMLElement*pCurElNew = pdoc->NewElement("selfeventLower");
					XMLNode *pCurNodeNew = pCurNode->InsertEndChild(pCurElNew);
					pCurNodeNew->ToElement()->SetAttribute("event24000", str24000Fps.c_str());
					pCurNodeNew->ToElement()->SetAttribute("event25000", str25000Fps.c_str());
					pCurNodeNew->ToElement()->SetAttribute("event26000", str26000Fps.c_str());
					pCurNodeNew->ToElement()->SetAttribute("event30000", str30000Fps.c_str());
				}
			}
			{
				string str24000Fps;
				string str25000Fps;
				string str26000Fps;
				string str30000Fps;
				GetEventInfo(pSkillNode,strKeyName.c_str(),mapInfoP,str24000Fps,str25000Fps,str26000Fps,str30000Fps,false,false);
				if (str24000Fps.length()>0||str25000Fps.length()>0||str26000Fps.length()>0||str30000Fps.length()>0)
				{
					XMLElement*pCurElNew = pdoc->NewElement("selfeventUp");
					XMLNode *pCurNodeNew = pCurNode->InsertEndChild(pCurElNew);
					pCurNodeNew->ToElement()->SetAttribute("event24000", str24000Fps.c_str());
					pCurNodeNew->ToElement()->SetAttribute("event25000", str25000Fps.c_str());
					pCurNodeNew->ToElement()->SetAttribute("event26000", str26000Fps.c_str());
					pCurNodeNew->ToElement()->SetAttribute("event30000", str30000Fps.c_str());
				}
			}
		}

		//状态光效处理.
		string  strSkillPingyinSelf =m_curHeroID+"_"+strSkillPingyin;
		ProceeStateTme(strSkillPingyinSelf,strSkillID,mapInfoPSelf);

		string  strSkillPingyinLink =m_curRoleLinkID+"_"+strSkillPingyin;
		ProceeStateTme(strSkillPingyinLink,strSkillID,mapInfoPLink);


		//child..
		++setIter;
	}
}



void SkillParse::GetEventInfo(XMLNode *pParent, const char* pszKeyname, map<string, XMLNode*>&mapInfo,string &str24000,string &str25000,string &str26000,string &str30000,bool blower,bool bHeroID)
{
	string strKeyName;
	if (bHeroID)
	{
		strKeyName = m_curHeroID+"_";
		strKeyName+=pszKeyname;
	}
	else
		strKeyName = pszKeyname;
	

	// lower
	if (blower)
	{
		strKeyName = strKeyName+="_lower";
	}
	else
	{
		strKeyName = strKeyName+="_up";
	}

	if (mapInfo.find(strKeyName)==mapInfo.end())
	{
		return;
	}

	XMLNode *pNode = mapInfo[strKeyName]; 
	{
		XMLElement* pChildNode = pNode->FirstChildElement("FrameEvent");
		while (pChildNode!=NULL)
		{
			if (pChildNode->Attribute("type","EmptyFrameEvent")&&!pChildNode->Attribute("triggerByProgram","true"))
			{
				if (pChildNode->Attribute("EventID","24000")&&pChildNode->Attribute("Frame"))
				{
					str24000= pChildNode->Attribute("Frame");
				}
				if (pChildNode->Attribute("EventID","25000")&&pChildNode->Attribute("Frame"))
				{
					str25000= pChildNode->Attribute("Frame");
				}
				if (pChildNode->Attribute("EventID","26000")&&pChildNode->Attribute("Frame"))
				{
					str26000= pChildNode->Attribute("Frame");
				}
				if (pChildNode->Attribute("EventID","30000")&&pChildNode->Attribute("Frame"))
				{
					str30000= pChildNode->Attribute("Frame");
				}
			}
			pChildNode = pChildNode->NextSiblingElement("FrameEvent");
		}
	}

}

void SkillParse::GetIniKeyAndValue(INIKEY &iniKeyValue,CIniLoad* pIniFile,string strSection,string strIniname)
{
	if (pIniFile == NULL || strSection.size()==0)
	{
		return;
	}
	int dwCount = (int)pIniFile->GetKeyAmount(strSection);
	for (int nIndex= 0;nIndex<dwCount;++nIndex)
	{
		string strKey = pIniFile->GetKeyByIndex(strSection,nIndex);
		string strKeyValue = pIniFile->GetValue(strSection,strKey);
		iniKeyValue.insert(make_pair(strKey,strKeyValue));
	}
}

void SkillParse::GetIniKeyAndValueAndRecoardToXml(INIKEY &iniKeyValue,CIniLoad* pIniFile,string strSection,const string& strIniName)
{
	GetIniKeyAndValue(iniKeyValue,pIniFile,strSection,strIniName);
}

void SkillParse::InsertKeyNameToXMl(const string & strIniName,const string & Keyname)
{
	//避免重复...

}


//处理state.ini,damage.ini.获取数据,(目前取技能光效.)
void SkillParse::ProcessStateAndDamageInfo()
{
	ProcessStateAndDamageInfoByIni(m_pStateIni,m_mapStateTme);
	ProcessStateAndDamageInfoByIni(m_pDamage,m_mapStateTme);
}


void SkillParse::ProcessStateAndDamageInfoByIni(CIniLoad* pIni ,STATETMEMAP &mapData)
{
	if (pIni == NULL)
	{
		return;
	}
	//遍历INI.
	int nSectionNum = pIni->GetSectionNum();
	for (int i = 0 ;i < nSectionNum ;++i)
	{
		string strSection = pIni->GetSectionName(i);
		string strbufname =pIni->GetValue(strSection,"describe");
		//目前处理. onAddFunc onDelFunc  skilleffect skill
		
		{
			string strValue = pIni->GetValue(strSection,"onAddFunc");
			TrimString(strValue);
			if (strValue.size() > 0)
			{
				{
					string strTemp = strValue;
					string keyString = "AddStatePresentation";
					int nPos = strTemp.find(keyString.c_str());
					while(nPos != string::npos)
					{
						if (strTemp.length()<= nPos+keyString.length())
						{
							break;
						}
						strTemp = strTemp.substr(nPos+keyString.length(),strTemp.length()-nPos-keyString.length());
						//取参数.
						string strFunParam;
						strTemp = GetSegmentString(strTemp,"(",")",strFunParam);

						string strPresentName;
						int nParamIndex= 0;
						while(strFunParam.length()>0)
						{
							strFunParam = GetSegmentString(strFunParam,"",";",strPresentName);
							if (nParamIndex==0)
							{
								//第二个参数开始.
								nParamIndex++;
								continue;
							}
							if (nParamIndex>1)
							{
								break;
							}
							nParamIndex++;

							strValue = strPresentName;
							string strHero ;
							if (strPresentName.length()>0 &&GetSkillName(strPresentName,strHero))
							{
								//加入技能数据.
								strPresentName=strHero+"_"+strPresentName;
								mapData[strPresentName].setPath.insert(strValue);
								mapData[strPresentName].bufName.insert(strbufname);
							}
							else if (strValue.length()>0)
							{
								mapData[strValue].setPath.insert(strValue);
							}
						}
						nPos = strTemp.find(keyString);
					}
				}
				{
					//查找AddStageStatePresentation
					string strTemp = strValue;
					string keyString = "AddStageStatePresentation";
					int nPos = strTemp.find(keyString.c_str());
					while(nPos != string::npos)
					{
						if (strTemp.length()<= nPos+keyString.length())
						{
							break;
						}
						strTemp = strTemp.substr(nPos,strTemp.length()-nPos);
						//取参数.
						string strFunParam;
						strTemp = GetSegmentString(strTemp,"(",")",strFunParam);

						string strPresentName;
						int nParamIndex= 0;
						while(strFunParam.length()>0)
						{
							strFunParam = GetSegmentString(strFunParam,"",";",strPresentName);
							if (nParamIndex==0)
							{
								//第二个参数开始.
								nParamIndex++;
								continue;
							}
							nParamIndex++;

							strValue = strPresentName;
							string strHero ;
							if (strPresentName.length()>0 &&GetSkillName(strPresentName,strHero))
							{
								//加入技能数据.
								strPresentName=strHero+"_"+strPresentName;
								mapData[strPresentName].setPath.insert(strValue);
								mapData[strPresentName].bufName.insert(strbufname);
							}
							else if (strValue.length()>0)
							{
								mapData[strValue].setPath.insert(strValue);
							}
						}
						nPos = strTemp.find(keyString);
					}
				}
			}
		}
		//ondel
		{
			string strValue = pIni->GetValue(strSection,"onDelFunc");
			TrimString(strValue);
			if (strValue.size() > 0)
			{
				{
					string strTemp = strValue;
					string keyString = "AddStatePresentation";
					int nPos = strTemp.find(keyString.c_str());
					while(nPos != string::npos)
					{
						if (strTemp.length()<= nPos+keyString.length())
						{
							break;
						}
						strTemp = strTemp.substr(nPos+keyString.length(),strTemp.length()-nPos-keyString.length());
						//取参数.
						string strFunParam;
						strTemp = GetSegmentString(strTemp,"(",")",strFunParam);

						string strPresentName;
						int nParamIndex= 0;
						while(strFunParam.length()>0)
						{
							strFunParam = GetSegmentString(strFunParam,"",";",strPresentName);
							if (nParamIndex==0)
							{
								//第二个参数开始.
								nParamIndex++;
								continue;
							}
							if (nParamIndex>1)
							{
								break;
							}
							nParamIndex++;

							strValue = strPresentName;
							string strHero ;
							if (strPresentName.length()>0 &&GetSkillName(strPresentName,strHero))
							{
								//加入技能数据.
								strPresentName=strHero+"_"+strPresentName;
								mapData[strPresentName].setPath.insert(strValue);
								mapData[strPresentName].bufName.insert(strbufname);
							}
							else if (strValue.length()>0)
							{
								mapData[strValue].setPath.insert(strValue);
							}
						}
						nPos = strTemp.find(keyString);
					}
				}
				{
					//查找AddStageStatePresentation
					string strTemp = strValue;
					string keyString = "AddStageStatePresentation";
					int nPos = strTemp.find(keyString.c_str());
					while(nPos != string::npos)
					{
						if (strTemp.length()<= nPos+keyString.length())
						{
							break;
						}
						strTemp = strTemp.substr(nPos,strTemp.length()-nPos);
						//取参数.
						string strFunParam;
						strTemp = GetSegmentString(strTemp,"(",")",strFunParam);

						string strPresentName;
						int nParamIndex= 0;
						while(strFunParam.length()>0)
						{
							strFunParam = GetSegmentString(strFunParam,"",";",strPresentName);
							if (nParamIndex==0)
							{
								//第二个参数开始.
								nParamIndex++;
								continue;
							}
							nParamIndex++;

							strValue = strPresentName;
							string strHero ;
							if (strPresentName.length()>0 &&GetSkillName(strPresentName,strHero))
							{
								//加入技能数据.
								strPresentName=strHero+"_"+strPresentName;
								mapData[strPresentName].setPath.insert(strValue);
								mapData[strPresentName].bufName.insert(strbufname);
							}
							else if (strValue.length()>0)
							{
								mapData[strValue].setPath.insert(strValue);
							}
						}
						nPos = strTemp.find(keyString);
					}
				}
			}
		}
		//skill
		{
			string strValue = pIni->GetValue(strSection,"skill");
			TrimString(strValue);
			if (strValue.size() > 0)
			{
				string strTemp = strValue;
				string keyString = "DoSkillEffect";
				int nPos = strTemp.find(keyString.c_str());
				while(nPos != string::npos)
				{
					if (strTemp.length()<= nPos+keyString.length())
					{
						break;
					}
					strTemp = strTemp.substr(nPos+keyString.length(),strTemp.length()-nPos-keyString.length());
					//取第二个参数.
					string strPresentName;
					strTemp = GetSegmentString(strTemp,",",")",strPresentName);
					strValue = strPresentName;
					string strHero ;
					if (strPresentName.length()>0 &&GetSkillName(strPresentName,strHero))
					{
						//加入技能数据.
						strPresentName=strHero+"_"+strPresentName;
						mapData[strPresentName].setPath.insert(strValue);
						mapData[strPresentName].bufName.insert(strbufname);
					}
					else if (strValue.length()>0)
					{
						mapData[strValue].setPath.insert(strValue);
					}
					nPos = strTemp.find(keyString);
				}
			}
		}
		//skilleffect
		{
			string strValue = pIni->GetValue(strSection,"skilleffect");
			TrimString(strValue);
			if (strValue.size() > 0)
			{
				string strTemp = strValue;
				string keyString = "DoSkillEffect";
				int nPos = strTemp.find(keyString.c_str());
				while(nPos != string::npos)
				{
					if (strTemp.length()<= nPos+keyString.length())
					{
						break;
					}
					strTemp = strTemp.substr(nPos+keyString.length(),strTemp.length()-nPos-keyString.length());
					//取第二个参数.
					string strPresentName;
					strTemp = GetSegmentString(strTemp,",",")",strPresentName);
					strValue = strPresentName;
					string strHero ;
					if (strPresentName.length()>0 &&GetSkillName(strPresentName,strHero))
					{
						//加入技能数据.
						strPresentName=strHero+"_"+strPresentName;
						mapData[strPresentName].setPath.insert(strValue);
						mapData[strPresentName].bufName.insert(strbufname);
					}
					else if (strValue.length()>0)
					{
						mapData[strValue].setPath.insert(strValue);
					}
					nPos = strTemp.find(keyString);
				}
			}
		}




	}
}


void SkillParse::ProceeStateTme(string presentName,string strSkllID,map<string, XMLNode*> &mapInfoP)
{
	if (m_mapSKillIDAndname.find(strSkllID) == m_mapSKillIDAndname.end())
	{
		return;
	}
	string skillPingyin =presentName;
	if (m_mapStateTme.find(skillPingyin)==m_mapStateTme.end())
	{
		return;
	}
	set<string>::iterator stateIter = m_mapStateTme[skillPingyin].setPath.begin();
	while(stateIter != m_mapStateTme[skillPingyin].setPath.end())
	{
		if (mapInfoP.find(*stateIter)==mapInfoP.end())
		{
			++stateIter;
			m_mapStateTme[skillPingyin].bNoExit = true;
			continue;
		}
		GetSkillPresentTmePathByName(strSkllID,mapInfoP[(*stateIter)]);
		m_mapStateTme[skillPingyin].usedskill.insert(strSkllID);
		++stateIter;
	}
}

void SkillParse::ProcessReMainStateTme(map<string, XMLNode*>& mapInfoP,map<string, XMLNode*>*pLinkMap)
{
	STATETMEMAP::iterator mapIter = m_mapStateTme.begin();
	while(mapIter != m_mapStateTme.end())
	{
		if (mapIter->second.usedskill.size()>0&&mapIter->second.usedskill.find("heroCommon")==mapIter->second.usedskill.end())
		{
			++mapIter;
			continue;
		}
		string strName = mapIter->first;
		string strName1 ;
		GetSegmentString(strName,"","_",strName1);
		if (strName1 != m_curHeroID && strName1!=m_curRoleLinkID)
		{
			++mapIter;
			continue;
		}
		//加入职业通用.
		set<string>::iterator stateIter = mapIter->second.setPath.begin();
		while(stateIter != mapIter->second.setPath.end())
		{
			map<string, XMLNode*>* ProcessMap = &mapInfoP;
			if (mapInfoP.find(*stateIter)==mapInfoP.end())
			{
				if (pLinkMap&&pLinkMap->find(*stateIter)!=pLinkMap->end())
				{
					ProcessMap = pLinkMap;
				}
				else
				{
					++stateIter;
					mapIter->second.bNoExit = true;
					continue;
				}
			}
			GetSkillPresentTmePathByName("heroCommon",(*ProcessMap)[(*stateIter)]);
			mapIter->second.usedskill.insert("heroCommon");
			++stateIter;
		}

		++mapIter;
	}
}



void SkillParse::ProcessUnitDataSkillPresent(map<string, XMLNode*>&pHeroCommonP,map<string, XMLNode*>*pLinkMap)
{
	/************************************************************************/
	/* 根据字段特殊处理.如有修改配置规则需要作出修改.                                                                     */
	/************************************************************************/
	if (m_pUnitData == NULL)
	{
		return;
	}
	int nSectionNum = m_pUnitData->GetSectionNum();
	int nSectionIndex = 0;
	while(nSectionIndex < nSectionNum)
	{
		string  strName = m_pUnitData->GetSectionName(nSectionIndex);
		//字段长度为5 根据前面2个字段确定职业。
		if (strName.length() == 5)
		{
			string strHeroType = strName.substr(0,2);
			EM_HEROTYPE emType = GetHeroType(strHeroType);
			EM_HEROTYPE emCurType = GetHeroType(m_curHeroID.substr(0,2));
			if (emType ==  emCurType)
			{
				string strPresent = m_pUnitData->GetValue(strName,"presentation");
				if (strPresent.length() > 0)
				{
					//获取光效.
					if (pHeroCommonP.find(strPresent)!=pHeroCommonP.end())
					{
						GetSkillPresentTmePathByName("heroCommon",pHeroCommonP[strPresent]);
					}
					else if (pLinkMap&&pLinkMap->find(strPresent)!=pLinkMap->end())
					{
						map<string, XMLNode*>& mapTemp = *pLinkMap;
						GetSkillPresentTmePathByName("heroCommon",mapTemp[strPresent]);
					}
					else if (m_mapCommonInfo.find(strPresent)!=m_mapCommonInfo.end())
					{
						GetSkillPresentTmePathByName("heroCommon",m_mapCommonInfo[strPresent]);
					}
				}
			}
		}

		// role id link id = 英雄ID。加入通用.
// 		string strRoleId = m_pUnitData->GetValue(strName,"roleid");
// 		string useSkaID =  m_pUnitData->GetValue(strName,"useskas");
		else if (/*strName ==  m_curHeroID || strName == m_curRoleLinkID*/0) //  暂不处理.
		{
			string strPresent = m_pUnitData->GetValue(strName,"presentation");
			while(strPresent.length() > 0 )
			{
				string strRealPresenName ;
				strPresent = GetSegmentString(strPresent,"",",",strRealPresenName);

				//获取光效.
				if (pHeroCommonP.find(strRealPresenName)!=pHeroCommonP.end())
				{
					GetSkillPresentTmePathByName("heroCommon",pHeroCommonP[strRealPresenName]);
				}
				else if (pLinkMap&&pLinkMap->find(strRealPresenName)!=pLinkMap->end())
				{
					map<string, XMLNode*>& mapTemp = *pLinkMap;
					GetSkillPresentTmePathByName("heroCommon",mapTemp[strRealPresenName]);
				}
				else if (m_mapCommonInfo.find(strRealPresenName)!=m_mapCommonInfo.end())
				{
					GetSkillPresentTmePathByName("heroCommon",m_mapCommonInfo[strRealPresenName]);
				}
			}
		}
		nSectionIndex++;
	}



}

bool SkillParse::GetSkillPresentTmePathByName(string strSkllID,XMLNode* xmlNode)
{
	bool bRes= false;
	if (xmlNode == NULL )
	{
		return bRes;
	}
	XMLElement *pChild = xmlNode->FirstChildElement("FrameEvent");
	while(pChild != NULL)
	{
		//TME光效处理.
		if (pChild->Attribute("type","TmeEvent"))
		{
			//
			XMLElement* pChildTmeNode = pChild->FirstChildElement("Tme");
			while(pChildTmeNode != NULL)
			{
				if (pChildTmeNode->Attribute("path"))
				{
					bRes = true;
					string strPath = pChildTmeNode->Attribute("path");
					if (strSkllID=="common")
					{
						//通用的.
						m_CommonTmeVec.insert(strPath);
					}
					else if (strSkllID == "heroCommon")
					{
						string iDName=m_curHeroID+"_";
						iDName+="heroCommon";
						m_vecSkillTme[iDName].insert(strPath);
					}
					else
					{
						string iDName=m_curHeroID+"_";
						iDName+=strSkllID;
						m_vecSkillTme[iDName].insert(strPath);
					}
						
				}					
				pChildTmeNode = pChildTmeNode->NextSiblingElement("Tme");
			}

		}
		pChild= pChild->NextSiblingElement("FrameEvent");
	}
	return bRes;
}

void SkillParse::outPutTmeInfo()
{
	XMLDocument xmlDoc;

	XMLElement *pEl = xmlDoc.NewElement("SkillTme");
	XMLNode *pRootNode = xmlDoc.InsertFirstChild(pEl);
	// 输出TME信息.
	VEC_SKILLTME::iterator skillIter = m_vecSkillTme.begin();
	while(skillIter != m_vecSkillTme.end())
	{
		XMLElement *pTmeNewNode  = NULL;
		XMLNode *pSKillNewNode= NULL;
		if (skillIter->second.size()>0)
		{
			string strName = skillIter->first;
			strName = ReplayString(strName,"_heroCommon","");
			string strNodename = "tme_"+strName;
			pTmeNewNode = xmlDoc.NewElement(strNodename.c_str());
			if (pTmeNewNode == NULL)
			{
				
				++skillIter;
				//deeeeeorre
				continue;
			}
			pSKillNewNode = pRootNode->InsertEndChild(pTmeNewNode);
			string strKey= "skillname";
			string strNameID =  m_pIniSkill->GetValue(skillIter->first.c_str(),strKey.c_str());

			string strSkillName = m_pIniLanguage->GetValue("tps-skillconfig",strNameID.c_str());

			pSKillNewNode->ToElement()->SetAttribute("name",strSkillName.c_str());
		}

		set<string>::iterator pathIter = skillIter->second.begin();
		while(pathIter!=skillIter->second.end()&&pSKillNewNode)
		{
			//判断文件是否存在.
			string fileFullPath = m_strWorkDir+"\\";
			fileFullPath+=(*pathIter).c_str();
			if (_access(fileFullPath.c_str(), 0)==-1)
			{
				m_NoexistTmeVec.insert(*pathIter);
				++pathIter;
				continue;
			}
			XMLElement *pPathEl = xmlDoc.NewElement("Tme");
			if (pPathEl == NULL)
			{
				++pathIter;
				continue;
			}
			XMLNode *pPathNode =  pSKillNewNode->InsertEndChild(pPathEl);
			if (pPathNode->ToElement())
			{
				pPathNode->ToElement()->SetAttribute("path",(*pathIter).c_str());
			}
			++pathIter;
		}
		++skillIter;

	}
	string strNewFile = m_strWorkDir;
	strNewFile += "\\processSkillRes\\skill_tme.xml";
	xmlDoc.SaveFile(strNewFile.c_str());
}

void SkillParse::outPutStateTmeInfo()
{
	XMLDocument xmlDoc;

	XMLElement *pEl = xmlDoc.NewElement("StateTme");
	XMLNode *pRootNode = xmlDoc.InsertFirstChild(pEl);
	// 输出TME信息.
	STATETMEMAP::iterator skillIter = m_mapStateTme.begin();
	while(skillIter != m_mapStateTme.end())
	{
		XMLElement *pTmeNewNode  = NULL;

		string strNodename = "tmelist";
		pTmeNewNode = xmlDoc.NewElement(strNodename.c_str());
		if (pTmeNewNode != NULL)
		{
			pRootNode->InsertEndChild(pTmeNewNode);
			
			pTmeNewNode->SetAttribute("name",skillIter->first.c_str());

			string strvalue;
			ComBinString(strvalue,"|",skillIter->second.bufName);
			pTmeNewNode->SetAttribute("buffDescrible",strvalue.c_str());

			ComBinString(strvalue,"|",skillIter->second.setPath);
			pTmeNewNode->SetAttribute("statePresentname",strvalue.c_str());

			strvalue = "";
			set<string>::iterator setIter = skillIter->second.usedskill.begin();
			while(setIter != skillIter->second.usedskill.end())
			{
				string strNameID = m_pIniSkill->GetValue(*setIter,"skillname");
				if (strNameID.length()>0)
				{
					strNameID = m_pIniLanguage->GetValue("tps-skillconfig",strNameID.c_str());
					strvalue+=strNameID+"|";
				}
				if (*setIter=="common")
				{
					strvalue+="common|";
				}
				else if(*setIter=="heroCommon")
				{
					strvalue+="heroCommon|";
				}
				++setIter;
			}
			if (strvalue.length()>1)
			{
				strvalue = strvalue.substr(0,strvalue.length()-1);
			}
			pTmeNewNode->SetAttribute("usedskillname",strvalue.c_str());


			if (skillIter->second.bNoExit)
			{
				pTmeNewNode->SetAttribute("error_noExist","1");
			}
		}
		++skillIter;

	}
	string strNewFile = m_strWorkDir;
	strNewFile += "\\processSkillRes\\State_tme.xml";
	xmlDoc.SaveFile(strNewFile.c_str());
}

void SkillParse::outPutErrorInfo()
{
	XMLDocument xmlDoc;

	XMLElement *pEl = xmlDoc.NewElement("ErrorInfomation");
	XMLNode *pRootNode = xmlDoc.InsertFirstChild(pEl);
	// 输出TME信息.
	map<string,vector<string>>::iterator errorIter = m_mapErrorInfo.begin();
	while(errorIter != m_mapErrorInfo.end())
	{
		do 
		{
			//新建节点 
			XMLElement *pNewNode  = NULL;

			string strNodename = "errorlist";
			pNewNode = xmlDoc.NewElement(strNodename.c_str());
			if (pNewNode == NULL)
			{
				break;
			}
			pRootNode->InsertEndChild(pNewNode);
			//获取技能信息.
			string strKey= "skillname";
			string strNameID =  m_pIniSkill->GetValue(errorIter->first,strKey.c_str());

			string strSkillName = m_pIniLanguage->GetValue("tps-skillconfig",strNameID.c_str());

			pNewNode->SetAttribute("ID" ,errorIter->first.c_str());
			pNewNode->SetAttribute("name",strSkillName.c_str());

			int nErrorSize = errorIter->second.size();
			string info = "";
			for (int nIndex = 0; nIndex < nErrorSize;++nIndex)
			{
				if (errorIter->second[nIndex] == "start|")
				{
					info.clear();
					continue;
				}
				if (errorIter->second[nIndex] == "|End")
				{
					//添加信息.
					XMLElement *pNewInfoNode  = NULL;
					pNewInfoNode = xmlDoc.NewElement("ErrorInfo");
					if (pNewInfoNode != NULL)
					{
						pNewNode->InsertEndChild(pNewInfoNode);
						pNewInfoNode->SetAttribute("COTX",info.c_str());
					}
					info.c_str();
					continue;
				}
				info+=errorIter->second[nIndex];
				info+="|";
			}
		} while (0);
		++errorIter;
	}

	//不存在的tme文件.
	std::set<const string>::iterator setIter = m_NoexistTmeVec.begin();
	while(setIter != m_NoexistTmeVec.end())
	{
		//新建节点 
		XMLElement *pNewNode  = NULL;

		string strNodename = "errorlist";
		pNewNode = xmlDoc.NewElement(strNodename.c_str());
		if (pNewNode == NULL)
		{
			break;
		}
		pRootNode->InsertEndChild(pNewNode);

		pNewNode->SetAttribute("name","no Tme File");
		pNewNode->SetAttribute("path",(*setIter).c_str());

		++setIter;
	}


	string strNewFile = m_strWorkDir;
	strNewFile += "\\processSkillRes\\error_info.xml";
	xmlDoc.SaveFile(strNewFile.c_str());
}

void SkillParse::outCommonPath()
{
	XMLDocument xmlDoc;

	XMLElement *pEl = xmlDoc.NewElement("Pack");
	XMLNode *pRootNode = xmlDoc.InsertFirstChild(pEl);
	pEl->SetAttribute("name","common");
	// 输出TME信息.

	XMLElement *pResEl = xmlDoc.NewElement("ResourcePaths");
	pEl->InsertFirstChild(pResEl);

	std::set<const string>::iterator commonIter = m_CommonTmeVec.begin();
	while(commonIter != m_CommonTmeVec.end())
	{
		
		//除去绝对路径.
		string strTemp = (*commonIter);
		string strWorkPath = m_strWorkDir+"\\";
		ReplaceLoopString(strTemp,"\\\\","\\");
		ReplaceLoopString(strTemp,"\\","/");
		ReplaceLoopString(strTemp,"//","/");

		
		ReplaceLoopString(strWorkPath,"\\\\","\\");
		ReplaceLoopString(strWorkPath,"\\","/");
		ReplaceLoopString(strWorkPath,"//","/");

		strTemp=ReplayString(strTemp,strWorkPath,"");

		//判断文件是否存在.
		string fileFullPath = m_strWorkDir+"\\";
		fileFullPath+=strTemp.c_str();

		ReplaceLoopString(fileFullPath,"/","\\");
		ReplaceLoopString(fileFullPath,"\\\\","\\");
		ReplaceLoopString(fileFullPath,"\\","/");
		
		if (_access(fileFullPath.c_str(), 0)==-1)
		{
			m_NoexistTmeVec.insert(strTemp);
			++commonIter;
			continue;
		}
		XMLElement *pResChildEl = xmlDoc.NewElement("Tme");
		pResEl->InsertFirstChild(pResChildEl);

		pResChildEl->SetAttribute("path",strTemp.c_str());
		++commonIter;
	}
	string strNewFile = m_strWorkDir;
	strNewFile += "\\processSkillRes\\common.xml";
	xmlDoc.SaveFile(strNewFile.c_str());
}

void SkillParse::ProcessLuaTme()
{
}

