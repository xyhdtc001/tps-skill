#include "stdafx.h"

#define DLLEXPORT_API extern "C" _declspec(dllexport)
#include "SkillParse.h"
#include "UnitPresentParse.h"

DLLEXPORT_API void TPSSkillTmePreloadScen(char* szWorkDir)
{
	SkillParse sk;
	//sk.SetSelfWorkDir(argv[0]);
	sk.SetWorkDir(szWorkDir);

	sk.ParseHero("1100");
	sk.ParseHero("1101");
	sk.ParseHero("1102");
	sk.ParseHero("1103");
	sk.ParseHero("1104");
	sk.ParseHero("1105");

	sk.ParseHero("1201");
	sk.ParseHero("1202");
	sk.ParseHero("1203");
	sk.ParseHero("1204");
	sk.ParseHero("1200");

	sk.ParseHero("1301");
	sk.ParseHero("1302");
	sk.ParseHero("1303");
	sk.ParseHero("1304");
	sk.ParseHero("1300");

	sk.ParseHero("1400");
	sk.ParseHero("1401");
	sk.ParseHero("1402");
	sk.ParseHero("1403");
	sk.ParseHero("1404");

	sk.ParseHero("1800");
	sk.ParseHero("1801");
	sk.ParseHero("1802");
	sk.ParseHero("1803");
	sk.ParseHero("1804");

	sk.ParseHero("1900");
	sk.ParseHero("1901");
	sk.ParseHero("1902");
	sk.ParseHero("1903");
	sk.ParseHero("1904");

	sk.ProceeCommonTme();

	sk.ProcessLuaTme();

	sk.outPutTmeInfo();
	sk.outPutStateTmeInfo();
	sk.outCommonPath();
	sk.outPutErrorInfo();

	//role 需要加载的光效修改.
	CUinitPresentParse unitPresen(&sk);
	unitPresen.InitXmlInfo();
	unitPresen.ProcessUnitData();
}