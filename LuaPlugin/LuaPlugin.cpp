#include "LuaPlugin.h"

CLuaScript LuaScript;

string LanguageName = "";
string ScriptName = "";
string InitialDirectory = "";
string ScriptFileName = "";

/**************************************************
main procedure.....................................
**************************************************/
BOOL WINAPI DllMain(HINSTANCE hInst, DWORD fdwreason, LPVOID lpReserved)
{
    switch(fdwreason)
	{
    case DLL_PROCESS_ATTACH: break;
    case DLL_PROCESS_DETACH: break; 
    case DLL_THREAD_ATTACH:  break;
    case DLL_THREAD_DETACH:  break;
    }
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

char* EXP_FUNC _GetLanguageName()
{
	LanguageName = "Lua";
	return (char*)LanguageName.c_str();
}

///////////////////////////////////////////////////////////////////////////////////////////////

bool EXP_FUNC _LoadScript(char *txt)
{
	return LuaScript.LoadScriptFromText(txt);
}

///////////////////////////////////////////////////////////////////////////////////////////////

char* EXP_FUNC _GetScriptName()
{
	ScriptName = LuaScript.GetGlobalString("ScriptName");
	return (char*)ScriptName.c_str();
}

///////////////////////////////////////////////////////////////////////////////////////////////

char* EXP_FUNC _GetInitialDirectory()
{
	InitialDirectory = LuaScript.GetGlobalString("InitialDirectory");
	return (char*)InitialDirectory.c_str();
}

///////////////////////////////////////////////////////////////////////////////////////////////

bool EXP_FUNC _Convert(char *in, char *out, char *err)
{
	bool res = false;
	ZeroMemory(out, MAX_PATH);
	ZeroMemory(err, 1024);

	LuaScript.SetGlobalString("Input", in); 
	if(LuaScript.SafeCallFunction("Convert", err)){
		
		string Output = LuaScript.GetGlobalString("Output");

		res = Output.length() > 0;
		if(res){
			memcpy(out, Output.c_str(), Output.length()); 
		} else {
			sprintf(err, "%s", "Error: Convert() returned null output.");
		}
	}

	return res;
}

