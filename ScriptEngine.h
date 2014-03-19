#pragma once
//----------------------------------------------------------------------//
#include "Windows.h"
#include "stdio.h"
//----------------------------------------------------------------------//

typedef char* (WINAPI* PFNGETLANGUAGENAME)();
typedef char* (WINAPI* PFNGETSCRIPTNAME)();
typedef char* (WINAPI* PFNGETINITIALDIRECTORY)();
typedef void  (WINAPI* PFNLOADSCRIPT)(char *txt);
typedef bool  (WINAPI* PFNCONVERT)(char *in, char *out, char *err);

class CScriptEngine {
public:
	CScriptEngine();
	~CScriptEngine();
private:
	HINSTANCE hDll;
private:
	void PrintErrorMsg(const char *fmt, ...);
public:
	PFNGETLANGUAGENAME      GetLanguageName;
	PFNGETSCRIPTNAME        GetScriptName;
	PFNGETINITIALDIRECTORY  GetInitialDirectory;
	PFNLOADSCRIPT           LoadScript;
	PFNCONVERT              Convert;
public:
	bool IsLoaded();
	bool Load(char *fname);
	void Free();
};

