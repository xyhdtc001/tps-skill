// TpsSkill.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "SkillParse.h"



int _tmain(int argc, _TCHAR* argv[])
{

	SkillParse sk;
	sk.SetWorkDir(argv[1]);
 	sk.ParseHero("1101");
 	sk.ParseHero("1201");
	sk.ParseHero("1301");
	sk.ParseHero("1401");
	sk.ParseHero("1801");

	sk.ProceeCommonTme();

	sk.ProcessLuaTme();

	sk.outPutTmeInfo();
	sk.outPutStateTmeInfo();
	sk.outCommonPath();
	sk.outPutErrorInfo();
	return 0;
}

