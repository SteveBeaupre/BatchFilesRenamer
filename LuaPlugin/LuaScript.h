#pragma once
//----------------------------------------------------------------------//
#pragma comment (lib, "lua51.lib")
#pragma comment (lib, "lua5.1.lib")
extern "C" {
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
}
//----------------------------------------------------------------------//
#include "Windows.h"
#include <string>
//----------------------------------------------------------------------//
using namespace std;

class CLuaScript {
protected:
	lua_State *L;
public:
	CLuaScript();
	~CLuaScript();

	void InitLua();
	void CloseLua();
	void ResetLua();

	bool LoadScriptFromFile(string scriptfile);
	bool LoadScriptFromText(string scripttext);
	
	void CallFunction(string name);
	bool SafeCallFunction(string name, char *err = NULL);

	string GetGlobalString(string name);
	void   SetGlobalString(string name, string value);
	
	double GetGlobalNumber(string name);
	void   SetGlobalNumber(string name, double value);
	
	bool GetGlobalBoolean(string name);
	void SetGlobalBoolean(string name, bool value);
};
