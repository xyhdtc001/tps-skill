#pragma once

#ifndef _FileLine_scan_H
#define _FileLine_scan_H

#include <set>

using namespace std;

class CIniLoad;
class CFileLineScan
{
public:
	CFileLineScan();
	~CFileLineScan();

	void ProcessFile(string filePath , set<const string>&resSet);


	void ProcessFile(string filePath ,CIniLoad*pInit,CIniLoad *pLang);


protected:
	int ProcessLine(int nType ,string strLine ,set<const string>&resSet );
	void ProcessEffectString(string strOrg ,set<const string>&resSet);
};



























#endif