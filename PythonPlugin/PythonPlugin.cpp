#include "PythonPlugin.h"
#include <string>

CPythonScript PythonScript;

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
    case DLL_PROCESS_ATTACH: InitializePython(); break;
    case DLL_PROCESS_DETACH: ShutdownPython();   break; 
    case DLL_THREAD_ATTACH:  break;
    case DLL_THREAD_DETACH:  break;
    }
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void InitializePython()
{
}

void ShutdownPython()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

char* EXP_FUNC _GetLanguageName()
{
	LanguageName = "Python";
	return (char*)LanguageName.c_str();
}

///////////////////////////////////////////////////////////////////////////////////////////////

bool EXP_FUNC _LoadScript(char *txt)
{
	return PythonScript.LoadScriptFromText(txt);
}

///////////////////////////////////////////////////////////////////////////////////////////////

char* EXP_FUNC _GetScriptName()
{
	ScriptName = PythonScript.GetGlobalString("ScriptName");
	return (char*)ScriptName.c_str();
}

///////////////////////////////////////////////////////////////////////////////////////////////

char* EXP_FUNC _GetInitialDirectory()
{
	InitialDirectory = PythonScript.GetGlobalString("InitialDirectory");
	return (char*)InitialDirectory.c_str();
}

///////////////////////////////////////////////////////////////////////////////////////////////

bool EXP_FUNC _Convert(char *in, char *out, char *err)
{
	ZeroMemory(out, MAX_PATH);
	ZeroMemory(err, 1024);

	PythonScript.SetGlobalString("Input", in); 
	if(PythonScript.SafeCallFunction("Convert", err)){
		//PythonScript.CallFunction("Convert");
		
		string Output = PythonScript.GetGlobalString("Output");

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

