
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
	lua_State *L = luaL_newstate();    //��ʼ��lua

	if (L == NULL) {
		cout << "cannot create state: not enough memory" << endl;
		//return EXIT_FAILURE;
	}

	luaL_openlibs(L);    //��������lua��׼��
	lua_newtable(L);

	 while (getline(cin, s))    //��cin�ж���һ�е�s
	 {
	 	//����s���lua�����ִ��
	 	bool err = luaL_loadbuffer(L, s.c_str(), s.length(),"line") || lua_pcall(L, 0, 0, 0);
	 	if (err)
	 	{
	 		//���������ʾ
	 		cerr << lua_tostring(L, -1);
	 		//����������Ϣ���ڵ����ϲ�ջ
	 		lua_pop(L, 1);
	 	}
	 }

	 

	 lua_close(L);//�ر�
}

