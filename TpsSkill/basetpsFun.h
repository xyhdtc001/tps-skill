#pragma once
#ifndef _BASEFUN___
#define _BASEFUN___
#include <cctype>  

#include <stdio.h>
#include <algorithm>
#include <string>
#include <set>
#include <io.h>
#include <stdlib.h>
#include <direct.h>

using namespace std;


//特殊..

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

static void ReplaceLoopString(string &str,string oldsub,string newSub)
{
	if (oldsub.length()<=0)
	{
		return;
	}
	int nPos = str.find(oldsub.c_str());
	while(nPos != string::npos)
	{
		str.replace(nPos,oldsub.length(),newSub);
		nPos = str.find(oldsub.c_str());
	}
}

//获取param1-param2 之间的字符串. 为空则param1位头，param2为尾.
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
		nParam1Pos = nParam1Pos==string::npos?0:nParam1Pos;
		nParam1Pos = nParam1Pos>=0?nParam1Pos+param1.length():0;
	}
	int nParam2Pos = strOrigin.length();
	if(param2.length() > 0)
	{
		nParam2Pos = strOrigin.find(param2,nParam1Pos);
		nParam2Pos = nParam2Pos==string::npos?strOrigin.length():nParam2Pos;
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

static string GetSegmentStringEx(const string& strOrigin,const  string &param1,const  string &param2,string &strDes,bool bAllExit)
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
		if (nParam1Pos==string::npos)
		{
			//没找到
			nParam1Pos = 0;
			if (bAllExit)
			{
				return strOrigin;
			}
		}
		else
		{
			//nParam1Pos = nParam1Pos==string::npos?0:nParam1Pos;
			nParam1Pos =nParam1Pos+param1.length();
		}

	}
	int nParam2Pos = strOrigin.length();
	if(param2.length() > 0)
	{
		nParam2Pos = strOrigin.find(param2,nParam1Pos);
		if (nParam2Pos==string::npos)
		{
			nParam2Pos = strOrigin.length();
			if (bAllExit)
			{
				return strOrigin;
			}
		}
		else
		{
			nParam2Pos = nParam2Pos>=nParam1Pos?nParam2Pos:strOrigin.length();
		}
		
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

//注意不支持中文..
static void  ToLower(string & strOri)
{
	transform(strOri.begin(), strOri.end(), strOri.begin(), Tolower);  
}

static string TranToSkillName(string strOrigin)
{
	//去除skill。统一小写 。
	 ToLower(strOrigin);

	 int nFindPos = strOrigin.find("skill");
	 if (nFindPos != string::npos)
	 {
		 strOrigin.replace(nFindPos,nFindPos+5,"");
	 }
	 
	 return strOrigin;
}

//取技能没类似1301_skillabc_xxxx格式.
static bool GetSkillName(string &strOrigin,string& strHeroInfo)
{
	bool bres = false;
	if (strOrigin.length() > 0)
	{
		//_分段.第一段英雄ID
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


//组合字符串|||
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

static int getCharNum(string strOrg,char cH)
{
	int nRes = 0;
	int nIndex= 0;
	string::iterator  strIter = strOrg.begin();
	while(strIter != strOrg.end())
	{
		if (*strIter == cH)
		{
			nRes++;
		}
		++strIter;
	}
	return nRes;
}

/*----------------------------  
 * 功能 : 递归遍历文件夹，找到其中包含的所有文件 
 *----------------------------  
 * 函数 : find 
 * 访问 : public   
 *  
 * 参数 : lpPath [in]      需遍历的文件夹目录 
 * 参数 : fileList [in]    以文件名称的形式存储遍历后的文件  
 */  
static void _findFile(string strKey,const char* mainDir,std::set<const std::string> &files)  
{   
	char oldPath[_MAX_PATH];
	_getcwd(oldPath,_MAX_PATH);
	const char *dir = mainDir;  
	_chdir(dir);  
	long hFile;  
	_finddata_t fileinfo;  

	if ((hFile = _findfirst(strKey.c_str(), &fileinfo)) != -1)  
	{  
		do  
		{  
			if ((fileinfo.attrib & _A_SUBDIR))//找到文件夹  
			{  
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)  
				{  
					char subdir[_MAX_PATH]={0};  
					strcpy_s(subdir, dir);  
					strcat_s(subdir, "\\");  
					strcat_s(subdir, fileinfo.name);  
					string temDir = subdir;  
					_findFile(strKey,temDir.c_str(), files);  
				}  
			}  
			else//直接找到文件  
			{  
				char filename[_MAX_PATH]={0};  
				strcpy_s(filename, dir);  
				strcat_s(filename, "\\");  
				strcat_s(filename, fileinfo.name);  
				string temfilename = filename;  
				ReplaceLoopString(temfilename,"\\","/");
				if (temfilename.length()>0)
				{
					files.insert(temfilename);
				}
				  
			}  
		} while (_findnext(hFile, &fileinfo) == 0);  
		_findclose(hFile);  
	} 
	//目录.
	if ((hFile = _findfirst("*.*", &fileinfo)) != -1)  
	{  
		do  
		{  
			if ((fileinfo.attrib & _A_SUBDIR))//找到文件夹  
			{  
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)  
				{  
					char subdir[_MAX_PATH]={0};  
					strcpy_s(subdir, dir);  
					strcat_s(subdir, "\\");  
					strcat_s(subdir, fileinfo.name);  
					string temDir = subdir;  
					_findFile(strKey,temDir.c_str(), files);  
				}  
			}   
		} while (_findnext(hFile, &fileinfo) == 0);  
		_findclose(hFile);  
	}

	_chdir(oldPath); 
}

static string ReplayString(string org,string oldsub,string newsub)
{
	string strDes = org;
	if (oldsub.length()<=0)
	{
		return strDes;
	}
	int nPos = org.find(oldsub);
	if (nPos == string::npos)
	{
		return strDes;
	}
	strDes.replace(nPos,oldsub.length(),newsub.c_str());
	return strDes;
}
//获取当前目录.
static string GetDir(string strPath)
{
	int dwSize =  strPath.length();
	while(dwSize > 0 )
	{
		if (strPath.at(dwSize-1)=='\\'||strPath.at(dwSize-1)=='/')
		{
			break;
		}
		dwSize--;
	}
	return strPath.substr(0,dwSize);
}

#endif