#pragma once
#ifndef _LUAOPRATE___
#define _LUAOPRATE___

class CLuaOprate 
{
public:
	CLuaOprate();
	~CLuaOprate();

	void runLua(string s,unsigned int nID,set<string> &);

};




#endif