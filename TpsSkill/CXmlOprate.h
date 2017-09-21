#pragma once
#ifndef _CXMOPRATE_H
#define _CXMOPRATE_H
#include <string>
#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;

class tinyxml2::XMLDocument;
class tinyxml2::XMLNode;
class tinyxml2::XMLElement;

class CXmlOprate 
{
public:
	CXmlOprate();
	~CXmlOprate();

	bool InitXmlFile(string strFile);

	bool CreateXmlDoc();

	bool SaveFile(string strFilePath);

	XMLNode* GetNodePtr(string strNodeName="");

protected:
	XMLDocument * m_pDoc ;

};

#endif