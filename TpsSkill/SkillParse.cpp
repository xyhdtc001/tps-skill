#include "stdafx.h"
#include "SkillParse.h"
#include "IniLoad.h"



SkillParse::SkillParse():
	m_pHeroSkill(0),m_pIniSkill(0),m_pIniSkiPresent(0),m_pIniLanguage(0),m_pStateIni(0),m_cuHeroInfo(0),
	m_pDamage(0),m_pXmlRole(0),m_pXmlHero1101_C(0),m_pXmlHero1101_P(0),m_pXmlHero1201_C(0),m_pXmlHero1201_P(0),
	m_pXmlHero1301_C(0),m_pXmlHero1301_P(0),m_pXmlHero1401_C(0),m_pXmlHero1401_P(0),m_pXmlHero1501_C(0),m_pXmlHero1501_P(0),
	m_pXmlHero1801_C(0),m_pXmlHero1801_P(0),m_strWorkDir("")
{
}


SkillParse::~SkillParse()
{
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
	// state.ini
	if (m_pStateIni != NULL)
	{
		delete m_pStateIni;
		m_pStateIni = NULL;

	}
	strSkillIniFile = m_strWorkDir + "\\data\\tpconfig\\state.ini";
	m_pStateIni = new CIniLoad(strSkillIniFile);

	//初始化
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
		else if (nVocation == 8)
		{
			//影舞者
			m_Hero1801.insert(make_pair(strName, _st_skillDescrig));
		}
	}

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

		//技能名字识别检查.
		string strSkillPingyin = "";
		string strTemp;
		do 
		{
			string strSkillTemp2;
			strTemp = GetSegmentString(strActionName,"","_",strSkillTemp2);
			if (strSkillTemp2!="sk" || strTemp.length()<=0)
			{
				m_mapErrorInfo[_skillInfo.strID].push_back("crtName_err!");
				break;
			}
			strTemp = GetSegmentString(strTemp,"","_",strSkillPingyin);
			strSkillPingyin = TranToSkillName(strSkillPingyin);
			if (strTemp.length()<=0)
			{
				m_mapErrorInfo[_skillInfo.strID].push_back("crtName_err!");
				break;
			}
			TrimString(strTemp);
			if (strTemp!=m_curHeroID)
			{
				m_mapErrorInfo[_skillInfo.strID].push_back("crtName_err!");
				break;
			}

		} while (0);

		string strSkill = m_curHeroID+"_";
		m_mapSKillIDAndname[_skillInfo.strID] = strSkill +strSkillPingyin; //id--技能名字拼音.--


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
			GetSkillPresentByName(pDoc,_100SetName,mapPInfo, p100Node,_skillInfo.strID);
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
			GetSkillPresentByName(pDoc,_200SetName, mapPInfo, p200Node,_skillInfo.strID);
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

void SkillParse::GetSkillPresentByName(XMLDocument *pdoc,set<string>& setName, map<string, XMLNode*> &mapInfoP, XMLNode* pDesNode,const string &strSkillID)
{
	set<string>::iterator setIter = setName.begin();
	
	while (setIter != setName.end())
	{
		string strKeyName = (*setIter);

		//识别名字.
		string strSkillPingyin = strKeyName;
		string strHero;
		if (!GetSkillName(strSkillPingyin,strHero))
		{
			m_mapErrorInfo[strSkillID].push_back("p_prt name Error");
		}
		string strAllName = strHero+"_"+strSkillPingyin;
		if (strAllName != m_mapSKillIDAndname[strSkillID])
		{
			m_mapErrorInfo[strSkillID].push_back("p_prt name and c_prt No march");
		}
		strSkillPingyin = m_mapSKillIDAndname[strSkillID];

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
							m_vecSkillTme[strSkillID].insert(pChildTmeNode->Attribute("path"));
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
		ProceeStateTme(strSkillID,mapInfoP);




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
					//取第二个参数.
					string strPresentName;
					strTemp = GetSegmentString(strTemp,";",")",strPresentName);
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
		//ondel
		{
			string strValue = pIni->GetValue(strSection,"onDelFunc");
			TrimString(strValue);
			if (strValue.size() > 0)
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
					//取第二个参数.
					string strPresentName;
					strTemp = GetSegmentString(strTemp,";",")",strPresentName);
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


void SkillParse::ProceeStateTme(string strSkllID,map<string, XMLNode*> &mapInfoP)
{
	if (m_mapSKillIDAndname.find(strSkllID) == m_mapSKillIDAndname.end())
	{
		return;
	}
	string skillPingyin = m_mapSKillIDAndname[strSkllID];
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

void SkillParse::GetSkillPresentTmePathByName(string strSkllID,XMLNode* xmlNode)
{
	if (xmlNode == NULL )
	{
		return;
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
					m_vecSkillTme[strSkllID].insert(pChildTmeNode->Attribute("path"));
				}					
				pChildTmeNode = pChildTmeNode->NextSiblingElement("Tme");
			}

		}
		pChild= pChild->NextSiblingElement("FrameEvent");
	}
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
			string strNodename = "tme_"+skillIter->first;
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
			XMLElement *pPathEl = xmlDoc.NewElement("path");
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

}
