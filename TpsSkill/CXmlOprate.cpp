#include "stdafx.h"
#include "CXmlOprate.h"
#include "tinyxml2.h"

CXmlOprate::CXmlOprate()
{
	m_pDoc = NULL;
}

CXmlOprate::~CXmlOprate()
{
	bool bRes = true;
	if (m_pDoc != NULL)
	{
		m_pDoc->Clear();
		delete m_pDoc;;
		m_pDoc= NULL;
	}
}

bool CXmlOprate::InitXmlFile(string strFile)
{
	bool bRes = true;
	if (m_pDoc != NULL)
	{
		m_pDoc->Clear();
		delete m_pDoc;;
		m_pDoc= NULL;
	}
	m_pDoc = new XMLDocument;
	if (m_pDoc->LoadFile(strFile.c_str())!=XML_SUCCESS)
	{
		delete m_pDoc;
		m_pDoc=NULL;
		return false;
	}

	return bRes;
}

bool CXmlOprate::CreateXmlDoc()
{
	return true;
}

bool CXmlOprate::SaveFile(string strFilePath)
{
	if (m_pDoc != NULL)
	{
		m_pDoc->Clear();
		delete m_pDoc;;
		m_pDoc= NULL;
	}
	m_pDoc = new XMLDocument;
	return true;
}

tinyxml2::XMLNode* CXmlOprate::GetNodePtr(string strNodeName/*=""*/)
{
	if (m_pDoc == NULL)
	{
		return NULL;
	}
	if (strNodeName.length() ==  0)
	{
		return m_pDoc->RootElement();
	}
	return m_pDoc->RootElement()->FirstChildElement(strNodeName.c_str());
}
