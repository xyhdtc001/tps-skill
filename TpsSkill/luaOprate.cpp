
#include "stdafx.h"
#include "luaOprate.h"

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include <iostream>
#include <string>
using namespace std;


CLuaOprate::CLuaOprate()
{

}

CLuaOprate::~CLuaOprate()
{

}

void CLuaOprate::runLua(string s,unsigned int nID,set<string> &setTmePath)
{
	//数据交换.
	char *szLua_Code =
		"package.path=package.path..c.szDir\n require \"innatecoreskill\" \nlocal effect = GetCoreSkillEffect(tonumber(c.SkillID))\n nNum=0\n strRes ={}\n if effect then\n for i,v in pairs(effect) do\n table.insert(strRes,v[\"tme\"])\n nNum=nNum+1 \n end\n end\n ";

	//lua字符串模式
	//char *szMode = ";E:/hero/tpsgame-runpath_1.6/client1.5/data/config/?.lua";



	lua_State *L = luaL_newstate();    //初始化lua

	if (L == NULL) {
		cout << "cannot create state: not enough memory" << endl;
		return;
	}
	luaL_openlibs(L);    //载入所有lua标准库


	lua_newtable(L);

	lua_pushstring(L, "szDir");
	lua_pushstring(L, s.c_str());
	lua_settable(L, -3);

	lua_pushstring(L, "SkillID");
	lua_pushnumber(L, nID);
	lua_settable(L, -3);


	lua_setglobal(L, "c");

	bool err = luaL_loadbuffer(L, szLua_Code, strlen(szLua_Code), "demo") || lua_pcall(L, 0, 0, 0);
	if (err)
	{
		cerr << lua_tostring(L, -1);
		lua_pop(L, 1);
	}
	else
	{
		int nNum = 0;
		lua_getglobal(L, "nNum");
		if (lua_isnumber(L,-1))
		{
			nNum =(int) lua_tonumber(L, -1);
			lua_pop(L, 1);
		}

		if (nNum > 0 )
		{
			lua_getglobal(L, "strRes");

			if (lua_istable(L, -1))
			{
				//取值
				int nIndex= 1;
				while(nIndex<=nNum)
				{
					lua_pushnumber(L, nIndex);
					lua_gettable(L, -2);
					cout << "x[1] = " << lua_tostring(L, -1) << endl;
					setTmePath.insert(lua_tostring(L, -1));
					lua_pop(L, 1);
					nIndex++;
				}
			}

		}
	}

	lua_close(L);//关闭
}

