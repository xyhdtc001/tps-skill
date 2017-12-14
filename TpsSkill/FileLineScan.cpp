#include "stdafx.h"
#include "FileLineScan.h"
#include "IniLoad.h"
#include <fstream>
#include <set>

CFileLineScan::CFileLineScan()
{

}

CFileLineScan::~CFileLineScan()
{

}

void CFileLineScan::ProcessFile(string filePath , set<const string>&resSet)
{
	ifstream in(filePath.c_str());  
	string filename;  
	string line;  
	int nType = 0;
	int nLineIndex = 0;
	if(in) // �и��ļ�  
	{  
		while (getline (in, line)) // line�в�����ÿ�еĻ��з�  
		{   
			nLineIndex++;
			if (nLineIndex==4336)
			{
				int iiii =0;
			}
			if (line.size() == 0 )
			{
				continue;
			}

			nType = ProcessLine(nType,line,resSet);
		}  
	}  
	in.close();
}

void CFileLineScan::ProcessFile(string filePath ,CIniLoad*pInit,CIniLoad *pLang)
{
	ifstream in(filePath.c_str());  
	string filename;  
	string line;  
	int nType = 0;
	int nLineIndex = 0;
	std::set<string> setTemp;
	setTemp.clear();

	string str2 = "e:\\22.txt";
	ofstream o_file(str2.c_str()); //����ļ�����������������ļ�   
	if(!in || !o_file)
	{
		return;
	}
	if(in) // �и��ļ�  
	{  
		while (getline (in, line)) // line�в�����ÿ�еĻ��з�  
		{   
			nLineIndex++;
			if (line.size() == 0 )
			{
				continue;
			}
			string strDes;
			string strLine2 = GetSegmentStringEx(line,"skillID(",")",strDes,true);
			if (strDes.length() <= 0)
			{
				continue;
			}
			string strDes2;
			GetSegmentStringEx(strLine2," "," ",strDes2,true);
			string str3 = strDes+strDes2;
			if(setTemp.find(str3) != setTemp.end())
				continue;
			setTemp.insert(str3);
			//
			string strKey= "skillname";
			string strNameID =  pInit->GetValue(strDes,strKey.c_str());

			string strSkillName = pLang->GetValue("tps-skillconfig",strNameID.c_str());
			o_file<<line.c_str()<<" name:"<<strSkillName.c_str()<<endl;
		}  
	}
	in.close();
	o_file.close();
}

int CFileLineScan::ProcessLine(int nType ,string strLine ,set<const string>&resSet)
{
	int nRes = nType;
	if (nType == 0)
	{
		//������
		int nPos = strLine.find("--[[");
		if (nPos != string::npos)
		{
			//���뷶Χע������.
			nRes = 1;
			strLine =  strLine.substr(nPos+4,strLine.length()-nPos-4);
			nRes = ProcessLine(0,strLine,resSet);
			return nRes;
		}
		else
			nPos = strLine.length();

		int nPos2 = strLine.find("--");
		if (nPos2 != string::npos && nPos2 < nPos)
		{
			nRes = 0;
			nPos = nPos2;
		}

		if (nPos > 0 )
		{
			string strEffect = strLine.substr(0,nPos);
			ProcessEffectString(strEffect,resSet);
		}

	}
	else if(nType == 1 )
	{
		//��Χע������.

		int nPos = strLine.find("]]");
		if (nPos != string::npos)
		{
			//ȡ�����뷶Χע������.
			nRes = 0;
			strLine =  strLine.substr(nPos+2,strLine.length()-nPos-2);
			nRes = ProcessLine(0,strLine,resSet);
		}

	}
	return nRes;
}

void CFileLineScan::ProcessEffectString(string strOrg ,set<const string>&resSet)
{
	int nPos = strOrg.find(".tme\"");
	if (nPos != string::npos)
	{
		bool bFind = false;
		int nPos2 = nPos;
		while(nPos2 >= 0)
		{
			if (strOrg.at(nPos2)=='\"')
			{
				bFind = true;
				break;
			}
			nPos2--;
		}

		if (bFind)
		{
			string strTemp = strOrg.substr(nPos2+1,nPos-nPos2+3);//��Ҫ����.
			ReplaceLoopString(strTemp,"\\","/");
			if (strTemp.length()>0)
			{
				resSet.insert(strTemp);
			}
			
		}


	}
}
