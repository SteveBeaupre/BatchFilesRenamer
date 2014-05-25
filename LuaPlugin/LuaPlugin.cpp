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
	ZeroMemory(out, MAX_PATH);
	ZeroMemory(err, 1024);

	LuaScript.SetGlobalString("Input", in); 
	if(LuaScript.SafeCallFunction("Convert", err)){
		
		string Output = LuaScript.GetGlobalString("Output");

		if(strlen(err) > 0)
			return false;

		if(Output.length() == 0){
			_snprintf(err, 1024, "%s", "Error: Convert() returned null output.");
			return false;
		}

		memcpy(out, Output.c_str(), Output.length()); 
		return true;
	}

	return false;
}

