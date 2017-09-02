// TpsSkill.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "SkillParse.h"



int _tmain(int argc, _TCHAR* argv[])
{

// 	string str1 = "1101_skillcf";
//	GetSkillName(str1);
	SkillParse sk;
	sk.SetWorkDir(argv[1]);
 	sk.ParseHero("1101");
 	sk.ParseHero("1201");
	sk.ParseHero("1301");
	sk.ParseHero("1401");
	sk.ParseHero("1801");

	sk.outPutTmeInfo();
	sk.outPutStateTmeInfo();
	return 0;
}

