#pragma once

#ifndef _FileLine_scan_H
#define _FileLine_scan_H

#include <set>

using namespace std;

class CFileLineScan
{
public:
	CFileLineScan();
	~CFileLineScan();

	void ProcessFile(string filePath , set<string>&resSet);


protected:
	int ProcessLine(int nType ,string strLine ,set<string>&resSet );
	void ProcessEffectString(string strOrg ,set<string>&resSet);
};



























#endif