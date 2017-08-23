#pragma once


#include <string>
#include <vector>
#include <map>
#include <set>


#include "tinyxml2.h"

using namespace std;


typedef map<string,string> INIKEY;


typedef struct  _stSkillDescribleData
{
	unsigned int  nID;
	string strID;
	string strName;
	string strSkillPresent;
	string strSkillHitedPresent;
	string strType;
	string strTime;
	INIKEY vec_key;
}SKILLDESCRIBLEDATA,*LPSKILLDESCRIBLEDATA;


typedef struct  _stPresentInfo
{
	string str24000Event;
	string str25000Event;
	string str26000Event;
	vector<string> vecTme;
}PRESENTINFO,*LPPRESENTINFO;


typedef map<string, SKILLDESCRIBLEDATA > MAPSKILLINFO;