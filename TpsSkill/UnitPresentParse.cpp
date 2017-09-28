#include "stdafx.h"
#include "IniLoad.h"
#include "CXmlOprate.h"
#include "UnitPresentParse.h"
#include "SkillParse.h"





CUinitPresentParse::CUinitPresentParse(SkillParse* pSkillParse)
{
	m_pSkillParse = pSkillParse;
}

CUinitPresentParse::~CUinitPresentParse()
{

}

void CUinitPresentParse::InitXmlInfo()
{
	if (m_pSkillParse == NULL)
	{
		return;
	}
	m_strWorkDir = m_pSkillParse->m_strWorkDir;
	
	string strFilePath  = m_strWorkDir+"\\"+"presentations\\roles.xml";
	loadRoleXml(strFilePath);

	// 读取part 的 rolexml . 
	 strFilePath  = m_strWorkDir+"\\"+"presentations\\partsconfig.xml";
	 CXmlOprate xmlOprate;
	 xmlOprate.InitXmlFile(strFilePath);
	 XMLNode *pRoot = xmlOprate.GetNodePtr();
	 if (pRoot)
	 {
		 XMLElement *pEl = pRoot->FirstChildElement("Part");
		 while(pEl != NULL)
		 {
			 string strPath  = pEl->Attribute("path")?pEl->Attribute("path"):"";
			 if (strPath.length() > 0)
			 {
				 strFilePath = m_strWorkDir+"\\";
				 strFilePath+=strPath;
				 ReplaceLoopString(strFilePath,"/","\\");
				 ReplaceLoopString(strFilePath,"\\\\","\\");
				 loadRoleXml(strFilePath);
			 }
			 pEl = pEl->NextSiblingElement("Part");
		 }
	 }

}

void CUinitPresentParse::ProcessUnitData()
{

	if (m_pSkillParse== NULL)
	{
		return;
	}
	//

	int nUnitNum = m_pSkillParse->m_pUnitData->GetSectionNum();
	for (int nIndex= 0;nIndex < nUnitNum;++nIndex)
	{
		//取role ID.
		string strSectionName = m_pSkillParse->m_pUnitData->GetSectionName(nIndex);
		ProcessSingleUnit(strSectionName);
	}
}

void CUinitPresentParse::loadRoleXml(string strPath)
{
	CXmlOprate xmlInfo ;
	xmlInfo.InitXmlFile(strPath);

	XMLNode* pRoot =  xmlInfo.GetNodePtr();
	//取root
	XMLElement *pChildNode = pRoot->FirstChildElement("Role");
	while (pChildNode != NULL)
	{
		string strID = pChildNode->Attribute("id")?pChildNode->Attribute("id") :"";
		if (strID.length() > 0)
		{
			_st_roleInfo stRoleInfo;
			XMLNode *pRoleNode = pChildNode->FirstChild();

			while(pRoleNode!=NULL)
			{
				string strNodeValue = pRoleNode->Value();
				if(strNodeValue == "WWiseBnkList")
				{
					//wwisebnkList
					XMLElement *WiseList = pRoleNode->FirstChildElement("WWiseBnk");
					while(WiseList != NULL)
					{
						if (WiseList->Attribute("path"))
						{
							stRoleInfo.setWWisePath.insert(WiseList->Attribute("path"));
						}
						WiseList = WiseList->NextSiblingElement("WWiseBnk");
					}
				}
				else if (strNodeValue == "Skin" && pRoleNode->ToElement())
				{
					if (pRoleNode->ToElement()->Attribute("path"))
					{
						stRoleInfo.strSkinPath = pRoleNode->ToElement()->Attribute("path");
					}

					if (pRoleNode->ToElement()->Attribute("ske"))
					{
						stRoleInfo.strSkePath = pRoleNode->ToElement()->Attribute("ske");
					}

				}
				else if (strNodeValue == "Skas"&& pRoleNode->ToElement())
				{
					if (pRoleNode->ToElement()->Attribute("path"))
					{
						stRoleInfo.strSkasPath = pRoleNode->ToElement()->Attribute("path");
					}
				}
				pRoleNode = pRoleNode->NextSibling();
			}
			m_MapRoleInfo.insert(make_pair(strID,stRoleInfo));
		}
		
		pChildNode = pChildNode->NextSiblingElement("Role");
	}
}

void CUinitPresentParse::ProcessSingleUnit(string strUnitID)
{
	if (m_pSkillParse->m_pUnitData->GetIntValue(strUnitID,"roleid") > 0)
	{
		bool bHero =  m_pSkillParse->m_pUnitData->GetIntValue(strUnitID,"ishero")==1; //hero 不处理present 。
		string strRoleID =  m_pSkillParse->m_pUnitData->GetValue(strUnitID,"roleid");
		string strPresent =  m_pSkillParse->m_pUnitData->GetValue(strUnitID,"presentation");
		string strLinkID =   m_pSkillParse->m_pUnitData->GetValue(strUnitID,"linkroleid");;
		string strUseskaID =/* m_pSkillParse->m_pUnitData->GetValue(strUnitID,"useskas");*/strLinkID;


		CXmlOprate xmlWrite;
		string strXmlFilePath = m_strWorkDir+"\\data\\tpconfig\\preloadroleres\\";
		strXmlFilePath+=strRoleID;
		strXmlFilePath+=".xml";
		xmlWrite.InitXmlFile(strXmlFilePath);
		XMLNode *pRoot = xmlWrite.GetNodePtr();

		set<string> setUnitTme;
		set<string> setUnitTmeOld;
		set<string> setUnitIntroTmeOld;
		set<string> setUnitWiseOld;
		setUnitWiseOld.clear();
		setUnitIntroTmeOld.clear();
		setUnitTme.clear();
		if (pRoot )
		{
			XMLNode *pResNode = pRoot->FirstChild();
			XMLNode *pIntroNode = NULL;
			XMLNode *pResourceNode = NULL;
			XMLElement *pElement = pRoot->FirstChildElement("ResourcePaths");
			//ResourcePaths 节点添加tme光效.
			while(pResNode != NULL)
			{
				string strNodeValue = pResNode->Value();
				if (strNodeValue == "ResourcePaths")
				{
					pResourceNode = pResNode;
				}
				else if (strNodeValue == "IntroResourcePaths")
				{
					pIntroNode = pResNode;
				}
				pResNode = pResNode->NextSibling();
			}

			if (pResourceNode == NULL)
			{
				// ??????
				return;
			}
			//读取原有节点..

			XMLElement *pOldEl = pElement->FirstChildElement("Tme");
			while(pOldEl!=NULL)
			{
				if (pOldEl->Attribute("path"))
				{
					string strPath = pOldEl->Attribute("path");
					ReplaceLoopString(strPath,"/","\\");
					ReplaceLoopString(strPath,"\\\\","\\");

					setUnitTmeOld.insert(strPath);
				}
				pOldEl = pOldEl->NextSiblingElement("Tme");
			}

			pOldEl = pElement->FirstChildElement("WwiseSound");
			while(pOldEl!=NULL)
			{
				if (pOldEl->Attribute("path"))
				{
					string strPath = pOldEl->Attribute("path");
					ReplaceLoopString(strPath,"/","\\");
					ReplaceLoopString(strPath,"\\\\","\\");
					setUnitWiseOld.insert(strPath);
				}
				pOldEl = pOldEl->NextSiblingElement("WwiseSound");
			}

			pOldEl = pIntroNode->FirstChildElement("Tme");
			while(pOldEl!=NULL)
			{
				if (pOldEl->Attribute("path"))
				{
					string strPath = pOldEl->Attribute("path");
					ReplaceLoopString(strPath,"/","\\");
					ReplaceLoopString(strPath,"\\\\","\\");
					setUnitIntroTmeOld.insert(strPath);
				}
				pOldEl = pOldEl->NextSiblingElement("Tme");
			}


			if (m_MapRoleInfo.find(strRoleID)== m_MapRoleInfo.end()&&!(strLinkID.length()>0&&m_MapRoleInfo.find(strLinkID)!=m_MapRoleInfo.end()))
			{
				return;
			}

			_st_roleInfo roleInfo;
			string strLinkPreocessPath;
			if (m_MapRoleInfo.find(strRoleID)== m_MapRoleInfo.end())
			{
				roleInfo = m_MapRoleInfo[strLinkID];
				strLinkPreocessPath = roleInfo.strSkasPath;
				ReplaceLoopString(strLinkPreocessPath,strLinkID,strRoleID);
				//ID 替换。此处应该不需要处理.
				//ReplaceLoopString(roleInfo.strSkinPath);
			}
			else
				roleInfo = m_MapRoleInfo[strRoleID];


			string useSkaPath = "";
			if (strUseskaID.length()>0&&m_MapRoleInfo.find(strUseskaID)!=m_MapRoleInfo.end())
			{
				useSkaPath = m_MapRoleInfo[strUseskaID].strSkasPath;
			}
			// 读取prt 文件.
			int nRoleType = m_pSkillParse->GetHeroTypeByIni(strRoleID);
			bHero = bHero||nRoleType!=0;
// 			if(!bHero && nRoleType == 0)
// 			{
				//英雄role不处理光效.修正。。要扫present ..不全部添加.
				ProcessPrtFile(roleInfo.strSkasPath,setUnitTme,strPresent,bHero);
				ProcessPrtFile(useSkaPath,setUnitTme,strPresent,bHero);
				ProcessPrtFile(strLinkPreocessPath,setUnitTme,strPresent,bHero);



				//主要是处理部件光效.. presentation
				while (strPresent.length()> 0)
				{
					//多个 光效以,分隔
					string strRealPresent;
					strPresent= GetSegmentString(strPresent,"",",",strRealPresent);
					//在 common 里找.
					if (m_pSkillParse->m_mapCommonInfo.find(strRealPresent)!=m_pSkillParse->m_mapCommonInfo.end())
					{
						// 添加进入tme光效...
						GetNodeTmeInfo(m_pSkillParse->m_mapCommonInfo[strRealPresent],setUnitTme);
					}
				}

			//}

			//写入文件.
			set<string>::iterator setIter = setUnitTme.begin();
			while(setIter != setUnitTme.end())
			{
				//去除绝对路径，判断文件是否存在.
				string strPath = (*setIter);
				ReplaceLoopString(m_strWorkDir,"/","\\");
				ReplaceLoopString(m_strWorkDir,"\\\\","\\");

				ReplaceLoopString(strPath,"/","\\");
				ReplaceLoopString(strPath,"\\\\","\\");

				ReplaceLoopString(strPath,m_strWorkDir,"");

				string fileFullPath = m_strWorkDir+"\\";
				fileFullPath+=strPath;
				if (_access(fileFullPath.c_str(), 0)==-1)
				{
					++setIter;
					continue;
				}


				if (setUnitTmeOld.find(strPath)==setUnitTmeOld.end())
				{
					XMLElement* pNewNode = xmlWrite.CreateEle("Tme");
					if (pNewNode != NULL)
					{
						pNewNode->SetAttribute("path",strPath.c_str());
						pResourceNode->InsertEndChild(pNewNode);
					}
				}

				if (setUnitIntroTmeOld.find(strPath)==setUnitIntroTmeOld.end())
				{
					//intro 节点也要添加光效.
					XMLElement* pNewIntroNode = xmlWrite.CreateEle("Tme");
					if (pNewIntroNode != NULL && pIntroNode != NULL)
					{
						pNewIntroNode->SetAttribute("path",strPath.c_str());
						pIntroNode->InsertEndChild(pNewIntroNode);
					}
				}

				++setIter;
			}

			//音频文件预加载.
			set<string>::iterator setIter2 =  roleInfo.setWWisePath.begin();
			while(setIter2 !=  roleInfo.setWWisePath.end())
			{
				string strPath = *setIter2;
				ReplaceLoopString(m_strWorkDir,"/","\\");
				ReplaceLoopString(m_strWorkDir,"\\\\","\\");

				ReplaceLoopString(strPath,"/","\\");
				ReplaceLoopString(strPath,"\\\\","\\");

				ReplaceLoopString(strPath,m_strWorkDir,"");

				if (setUnitWiseOld.find(strPath)!=setUnitWiseOld.end())
				{
					++setIter2;
					continue;
				}

				XMLElement* pNewNode = xmlWrite.CreateEle("WwiseSound");
				if (pNewNode == NULL)
				{
					continue;
				}
				pNewNode->SetAttribute("path",strPath.c_str());
				pResourceNode->InsertEndChild(pNewNode);
				++setIter2;
			}



			
		}
		xmlWrite.SaveFile(strXmlFilePath);

	}




}


void CUinitPresentParse::ProcessPrtFile(string filePath,set<string>& setFilePath,string strPresent,bool bHero )
{
	if (filePath.length()==0)
	{
		return;
	}
	//去除绝对路径.
	ReplaceLoopString(m_strWorkDir,"/","\\");
	ReplaceLoopString(m_strWorkDir,"\\\\","\\");

	ReplaceLoopString(filePath,"/","\\");
	ReplaceLoopString(filePath,"\\\\","\\");

	ReplaceLoopString(filePath,m_strWorkDir,"");


	map<string, XMLNode*>mapInfo;
	mapInfo.clear();

	CXmlOprate xmlPrt;
	string strFilePath = m_strWorkDir+"\\";
	strFilePath+=filePath;
	ReplaceLoopString(strFilePath,"\\\\","\\");

	xmlPrt.InitXmlFile(strFilePath);

	m_pSkillParse->LoopXMlForInfo(xmlPrt.GetNodePtr(),mapInfo);

	if (bHero)
	{
		while (strPresent.length()> 0)
		{
			//多个 光效以,分隔
			string strRealPresent;
			strPresent= GetSegmentString(strPresent,"",",",strRealPresent);
			if (mapInfo.find(strRealPresent)!=mapInfo.end())
			{
				// 添加进入tme光效...
				GetNodeTmeInfo(mapInfo[strRealPresent],setFilePath);
			}
		}
	}
	else
	{

		//遍历节点.
		map<string, XMLNode*>::iterator mapIter = mapInfo.begin();
		while(mapIter != mapInfo.end())
		{
			GetNodeTmeInfo(mapIter->second,setFilePath);
			++mapIter;
		}
	}


}

void CUinitPresentParse::GetNodeTmeInfo(XMLNode* pNode,set<string>& setFilePath)
{
	if (pNode == NULL )
	{
		return ;
	}
	XMLElement *pChild = pNode->FirstChildElement("FrameEvent");
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
					setFilePath.insert(pChildTmeNode->Attribute("path"));
				}					
				pChildTmeNode = pChildTmeNode->NextSiblingElement("Tme");
			}

		}
		pChild= pChild->NextSiblingElement("FrameEvent");
	}
}
