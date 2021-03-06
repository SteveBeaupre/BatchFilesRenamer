//----------------------------------------------------------------------//
#define EXP_FUNC __stdcall
//----------------------------------------------------------------------//
#include "Windows.h"
#include "stdio.h"
//----------------------------------------------------------------------//
#include "PythonScript.h"
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
void InitializePython();
void ShutdownPython();
bool CallPythonFunction(char *funcname, char *in, char *out);
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
char* EXP_FUNC _GetLanguageName();
char* EXP_FUNC _GetScriptName();
char* EXP_FUNC _GetInitialDirectory();
bool  EXP_FUNC _LoadScript(char *txt);
bool  EXP_FUNC _Convert(char *in, char *out, char *err);
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//

