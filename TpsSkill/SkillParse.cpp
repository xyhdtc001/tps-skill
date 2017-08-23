#include "stdafx.h"
#include "SkillParse.h"
#include "IniLoad.h"



SkillParse::SkillParse():
	m_pHeroSkill(0),m_pIniSkill(0),m_pIniSkiPresent(0),m_pIniLanguage(0),
	m_pDamage(0),m_pXmlRole(0),m_pXmlHero1101_C(0),m_pXmlHero1101_P(0),m_pXmlHero1201_C(0),m_pXmlHero1201_P(0),
	m_pXmlHero1301_C(0),m_pXmlHero1301_P(0),m_pXmlHero1401_C(0),m_pXmlHero1401_P(0),m_pXmlHero1501_C(0),m_pXmlHero1501_P(0),
	m_pXmlHero1801_C(0),m_pXmlHero1801_P(0),m_strWorkDir("")
{
	XMLDocument *m_pDocXmlKeyName = new XMLDocument;
}


SkillParse::~SkillParse()
{
	if (m_pDocXmlKeyName != NULL)
	{
		delete m_pDocXmlKeyName;
		m_pDocXmlKeyName = NULL;
	}
}

bool SkillParse::SetWorkDir(string strPath)
{
	m_strWorkDir = strPath;
	InitConfigFile();
	//分类英雄技能.
	InitHeroSkillMap();
	return true;
}

void SkillParse::ParseHero(string strHeroID)
{
	if (strHeroID=="1101")
	{
		CreateHeroSkillInfoByXml(m_Hero1101, strHeroID);
	}
	else if (strHeroID=="1201")
	{
		CreateHeroSkillInfoByXml(m_Hero1201, strHeroID);
	}
	else if (strHeroID=="1301")
	{
		CreateHeroSkillInfoByXml(m_Hero1301, strHeroID);
	}
	else if (strHeroID=="1401")
	{
		CreateHeroSkillInfoByXml(m_Hero1401, strHeroID);
	}
	else if (strHeroID=="1501")
	{
		CreateHeroSkillInfoByXml(m_Hero1501, strHeroID);
	}
	else if (strHeroID=="1801")
	{
		CreateHeroSkillInfoByXml(m_Hero1801, strHeroID);
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
		if (nVocation <=0 || nVocation >8)
		{
			continue;
		}

		string strName = m_pHeroSkill->GetValue(strSecName,"talent");
		SKILLDESCRIBLEDATA _st_skillDescrig = {0};
		_st_skillDescrig.strID = strName;
		//获取INI值
		GetIniKeyAndValue(_st_skillDescrig.vec_key,m_pHeroSkill,strName);
		if (strSecName.length() != 8)
		{
			continue;
		}
		if (nVocation == 1 )
		{
			//战士
			m_Hero1101.insert(make_pair(strName, _st_skillDescrig));
		}
		else if(nVocation == 2)
		{
			//法师
			m_Hero1201.insert(make_pair(strName, _st_skillDescrig));
		}
		else if (nVocation == 3)
		{
			//木事
			m_Hero1301.insert(make_pair(strName, _st_skillDescrig));
		}
		else if (nVocation == 4)
		{
			//骑士
			m_Hero1401.insert(make_pair(strName, _st_skillDescrig));
		}
		else if (strSecName.find("15", 0) == 0)
		{
			//亡灵..

		}
		else if (nVocation == 8)
		{
			//影舞者
			m_Hero1801.insert(make_pair(strName, _st_skillDescrig));
		}
	}

}

void SkillParse::CreateHeroSkillInfoByXml(MAPSKILLINFO mapInfo, string strHeroID)
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
	XMLDocument *pDoc_c = new XMLDocument;
	if (pDoc_c->LoadFile(strC.c_str())!=XML_SUCCESS)
	{
		return;
	}
	XMLDocument *pDoc_p = new XMLDocument;
	if (pDoc_p->LoadFile(strP.c_str()) != XML_SUCCESS)
	{
		return;
	}
	XMLNode *pNode_p = pDoc_p->FirstChild();
	XMLNode *pNode_c = pDoc_c->FirstChild();
	if (pNode_c==NULL||pNode_p==NULL)
	{
		return;
	}

	//遍历载入信息.
	map<string, XMLNode*>mapCInfo;
	map<string, XMLNode*>mapPInfo;

	LoopXMlForInfo(pNode_p,mapPInfo);
	LoopXMlForInfo(pNode_c,mapCInfo);

	//读取技能信息。写入XML文件..
	XMLDocument *pDoc = new XMLDocument();

	XMLElement *pEl = pDoc->NewElement("HeroSkill");
	XMLNode *pRootNode = pDoc->InsertFirstChild(pEl);

	MAPSKILLINFO::iterator mapIter = mapInfo.begin();
	while (mapIter!=mapInfo.end())
	{
		SKILLDESCRIBLEDATA& _skillInfo = mapIter->second;

		//技能名字.
		string strSkillName;
		{
			string strKey= "skillname";
			string strNameID =  m_pIniSkill->GetValue(_skillInfo.strID,strKey.c_str());

			strSkillName = m_pIniLanguage->GetValue("tps-skillconfig",strNameID.c_str());
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
		{
			string strKey ="default";
			strActionName = m_pIniSkiPresent->GetValue(_skillInfo.strID,strKey.c_str());
		}
		pSKillNewEl->SetAttribute("actionname",strActionName.c_str());
		if (strActionName.length() == 0)
		{
			++mapIter;
			continue;
		}
		
		
		if (mapCInfo.find(strActionName)==mapCInfo.end())
		{
			++mapIter;
			continue;
		}



		XMLNode *pCurCNode = mapCInfo[strActionName];

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
			GetSkillPresentByName(pDoc,_100SetName,mapPInfo, p100Node);
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
			GetSkillPresentByName(pDoc,_200SetName, mapPInfo, p200Node);
		}

		++mapIter;
	}

	//保存
	string strNewFile = m_strWorkDir;
	strNewFile += "\\processSkillRes\\hero_";
	strNewFile += strHeroID;
	strNewFile += ".xml";
	pDoc->SaveFile(strNewFile.c_str());

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

void SkillParse::GetSkillPresentByName(XMLDocument *pdoc,set<string>& setName, map<string, XMLNode*> &mapInfoP, XMLNode* pDesNode)
{
	set<string>::iterator setIter = setName.begin();
	
	while (setIter != setName.end())
	{
		string strKeyName = (*setIter);
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
		if (!szUsdcast&&!szUsdchant&&!szUsdseq)
		{
			//遍历下child
			XMLElement* pChildNode = pSkillEl->FirstChildElement("FrameEvent");
			while (pChildNode!=NULL)
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
				pChildNode = pChildNode->NextSiblingElement("FrameEvent");
			}

			//
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
		//tme

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
		if (strIniname.size()>0&&m_pDocXmlKeyName!=NULL)
		{
			//存入XML节点.
			InsertKeyNameToXMl(strIniname,strKey);
		}
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
