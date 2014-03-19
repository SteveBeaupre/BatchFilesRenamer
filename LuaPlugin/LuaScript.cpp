#include "LuaScript.h"

CLuaScript::CLuaScript()
{
	L = NULL;
	InitLua();
}

CLuaScript::~CLuaScript()
{
	CloseLua();
}

void CLuaScript::InitLua()
{
	if(!L){
		L = lua_open();
		luaL_openlibs(L);
	}
}

void CLuaScript::CloseLua()
{
	if(L){
		lua_close(L);
		L = NULL;
	}
}

void CLuaScript::ResetLua()
{
	CloseLua();
	InitLua();
}

bool CLuaScript::LoadScriptFromFile(string scriptfile)
{
	ResetLua();

	bool value = true;

	try {
		luaL_dofile(L, scriptfile.c_str());
	} catch(...) {
		value = false;
	}

	return value;
}

bool CLuaScript::LoadScriptFromText(string scripttext)
{
	ResetLua();

	bool value = true;

	try {
		luaL_dostring(L, scripttext.c_str());
	} catch(...) {
		value = false;
	}

	return value;
}

void CLuaScript::CallFunction(string name)
{
	//call script function, 0 args, 0 retvals
	lua_getglobal(L, name.c_str());
	lua_call(L, 0, 0);
}

bool CLuaScript::SafeCallFunction(string name, char *err)
{
	//call script function, 0 args, 0 retvals
	lua_getglobal(L, name.c_str());
	int res = lua_pcall(L, 0, 0, 0);

	if(res > 0){
		const char *pErrMsg = lua_tostring(L, 1);

		if(!err){
			string Caption;
			Caption = "Lua: Error in function " + name + "().";

			MessageBox(0, pErrMsg, Caption.c_str(), MB_OK);
		} else {
			sprintf(err, "%s", pErrMsg);
		}

		return false;
	}

	return true;
}

string CLuaScript::GetGlobalString(string name)
{
	string value = "";
	try {
		lua_getglobal(L, name.c_str());
		const char *pValue = lua_tostring(L, -1);
		if(pValue)
			value = pValue;
		lua_pop(L, 1);
	} catch(...) {
	
	}

	return value;
}

void CLuaScript::SetGlobalString(string name, string value)
{
	lua_pushstring(L, value.c_str());
	lua_setglobal(L, name.c_str());
}

double CLuaScript::GetGlobalNumber(string name)
{
	double value = 0.0;
	
	try {
		lua_getglobal(L, name.c_str());
		value = lua_tonumber(L, -1);
		lua_pop(L, 1);
	} catch(...) {
	
	}

	return value;
}

void CLuaScript::SetGlobalNumber(string name, double value)
{
	lua_pushnumber(L, (int)value);
	lua_setglobal(L, name.c_str());
}

bool CLuaScript::GetGlobalBoolean(string name)
{
	bool value = false;
	
	try {
		lua_getglobal(L, name.c_str());
		value = lua_toboolean(L, -1) != 0;
		lua_pop(L, 1);
	} catch(...) {
	
	}
	
	return value;
}

void CLuaScript::SetGlobalBoolean(string name, bool value)
{
	lua_pushboolean(L, (int)value);
	lua_setglobal(L, name.c_str());
}

