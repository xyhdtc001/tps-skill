#pragma once
#ifndef _BASEFUN___
#define _BASEFUN___
#include <cctype>  

#include <algorithm>
#include <string>
#include <set>
using namespace std;


//����..

//1201_skillbszh --- 1201_frozenicebuf_state1  ,1201_frozenicebuf_state2



static void TrimString(string &str)
{
	int nPos = str.find(" ");
	while(nPos != string::npos)
	{
		str.replace(nPos,1,"");
		nPos = str.find(" ");
	}
}

//��ȡparam1-param2 ֮����ַ���. Ϊ����param1λͷ��param2Ϊβ.
static string GetSegmentString(const string& strOrigin,const  string &param1,const  string &param2,string &strDes)
{
	strDes="";
	if (strOrigin.length() <= 0)
	{
		return "";
	}
	int nParam1Pos = 0;
	if (param1.length()>0)
	{
		nParam1Pos = strOrigin.find(param1.c_str());
		nParam1Pos = nParam1Pos>=0?nParam1Pos+param1.length():0;
	}
	int nParam2Pos = strOrigin.length();
	if(param2.length() > 0)
	{
		nParam2Pos = strOrigin.find(param2,nParam1Pos);
		nParam2Pos = nParam2Pos>=nParam1Pos?nParam2Pos:strOrigin.length();
	}

	if (nParam2Pos-nParam1Pos > 0)
	{
		strDes = strOrigin.substr(nParam1Pos,nParam2Pos-nParam1Pos);
	}
	string strRes = "";
	if (strOrigin.length() > (nParam2Pos+param2.length()) )
	{
		strRes = strOrigin.substr(nParam2Pos+param2.length(),strOrigin.length() - (nParam2Pos+param2.length()));
	}

	return strRes;
}


static int Tolower(int c)  
{  
	return tolower(c);  
}  

//ע�ⲻ֧������..
static void  ToLower(string & strOri)
{
	transform(strOri.begin(), strOri.end(), strOri.begin(), Tolower);  
}

static string TranToSkillName(string strOrigin)
{
	//ȥ��skill��ͳһСд ��
	 ToLower(strOrigin);

	 int nFindPos = strOrigin.find("skill");
	 if (nFindPos != string::npos)
	 {
		 strOrigin.replace(nFindPos,nFindPos+5,"");
	 }
	 
	 return strOrigin;
}

//ȡ����û����1301_skillabc_xxxx��ʽ.
static bool GetSkillName(string &strOrigin,string& strHeroInfo)
{
	bool bres = false;
	if (strOrigin.length() > 0)
	{
		//_�ֶ�.��һ��Ӣ��ID
		string strHero;
		string strNext = GetSegmentString(strOrigin,"","_",strHero);
		if (strHero=="1101"||strHero=="1102"||strHero=="1201"||strHero=="1301"||strHero=="1401"||strHero=="1801")
		{
			bres = true;
			string strSkill;
			GetSegmentString(strNext,"","_",strSkill);
			
			strSkill = TranToSkillName(strSkill);
			strOrigin =strSkill;
			strHeroInfo = strHero;
		}
	}
	return bres;
}


//����ַ���|||
static void ComBinString(string& strDes,string split,set<string>setOrg)
{
	strDes = "";
	set<string>::iterator setIter = setOrg.begin();
	while(setIter != setOrg.end())
	{
		strDes+=*setIter+split;
		++setIter;
	}
	if (strDes.length()>=split.length())
	{
		strDes = strDes.substr(0,strDes.length()-split.length());
	}
	
}





#endif