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

	void ProcessFile(string filePath , set<const string>&resSet);


protected:
	int ProcessLine(int nType ,string strLine ,set<const string>&resSet );
	void ProcessEffectString(string strOrg ,set<const string>&resSet);
};



























#endif