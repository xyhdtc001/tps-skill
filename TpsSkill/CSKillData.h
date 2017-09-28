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
	bool bCoreSkill;//使用的是雕文ID
}SKILLDESCRIBLEDATA,*LPSKILLDESCRIBLEDATA;


typedef struct  _stPresentInfo
{
	string str24000Event;
	string str25000Event;
	string str26000Event;
	vector<string> vecTme;
}PRESENTINFO,*LPPRESENTINFO;


typedef map<string, SKILLDESCRIBLEDATA > MAPSKILLINFO;

typedef struct _tme_st
{
	_tme_st(){
		bNoExit= false;
	}
	set<string> setPath;
	set<string> bufName;
	set<string> usedskill;
	bool  bNoExit;
}ST_TME;
	
typedef map<string,ST_TME>  STATETMEMAP;

typedef map<string,set<string>>  VEC_SKILLTME;



enum EM_HEROTYPE {
	_em_Error = 0,
	_em_Zhanshi = 1,
	_em_Fashi,
	_em_Mushi,
	_em_Qishi,
	_em_YingWuZhe
};

 static EM_HEROTYPE GetHeroType(string strHeroID2str)
{
	EM_HEROTYPE resType = _em_Error;
	if(strHeroID2str=="11")
	{
		resType = _em_Zhanshi;
	}
	else if (strHeroID2str=="12")
	{
		resType = _em_Fashi;
	}
	else if (strHeroID2str=="13")
	{
		resType = _em_Mushi;
	}
	else if (strHeroID2str=="14")
	{
		resType = _em_Qishi;
	}
	else if (strHeroID2str=="18")
	{
		resType = _em_YingWuZhe;
	}
	return resType;
}	

 typedef struct _st_roleInfo
 {
	 string strSkinPath;
	 string strSkePath;
	 string strSkasPath;
	 set<string> setWWisePath ;
 }ST_ROLEINFO;


