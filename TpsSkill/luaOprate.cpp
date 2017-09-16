
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

void CLuaOprate::runLua(string s)
{
	lua_State *L = luaL_newstate();    //初始化lua

	if (L == NULL) {
		cout << "cannot create state: not enough memory" << endl;
		//return EXIT_FAILURE;
	}

	luaL_openlibs(L);    //载入所有lua标准库
	lua_newtable(L);

	 while (getline(cin, s))    //从cin中读入一行到s
	 {
	 	//载入s里的lua代码后执行
	 	bool err = luaL_loadbuffer(L, s.c_str(), s.length(),"line") || lua_pcall(L, 0, 0, 0);
	 	if (err)
	 	{
	 		//如果错误，显示
	 		cerr << lua_tostring(L, -1);
	 		//弹出错误信息所在的最上层栈
	 		lua_pop(L, 1);
	 	}
	 }

	 

	 lua_close(L);//关闭
}

