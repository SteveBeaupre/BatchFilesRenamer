//----------------------------------------------------------------------//
#define EXP_FUNC __stdcall
//----------------------------------------------------------------------//
#define WIN32_LEAN_AND_MEAN // Trim libraies.
#define VC_LEANMEAN         // Trim even further.
//----------------------------------------------------------------------//
#include "Windows.h"
#include <stdio.h>
//----------------------------------------------------------------------//
#include "LuaScript.h"
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
void InitializeLua();
void ShutdownLua();
bool CallLuaFunction(char *funcname, char *in, char *out);
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
char* EXP_FUNC _GetLanguageName();
bool  EXP_FUNC _LoadScript(char *txt);
char* EXP_FUNC _GetScriptName();
char* EXP_FUNC _GetInitialDirectory();
bool  EXP_FUNC _Convert(char *in, char *out, char *err);
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//

