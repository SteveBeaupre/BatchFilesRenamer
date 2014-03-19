#include "ScriptEngine.h"

CScriptEngine::CScriptEngine()
{
	hDll = NULL;
	Free();
}

CScriptEngine::~CScriptEngine()
{
	Free();
}

bool CScriptEngine::IsLoaded()
{
	return hDll != NULL;
}

void CScriptEngine::PrintErrorMsg(const char *fmt, ...)
{
	if(fmt == NULL)
		return;

	const int BufSize = 1024;

	char TextBuf[BufSize];
	char *pTxt = &TextBuf[0];
	ZeroMemory(pTxt, BufSize);

	va_list ap;
	va_start(ap, fmt);
	#ifdef RAD_STUDIO_XE
	vsnprintf(pTxt, BufSize, fmt, ap);
	#else
	vsprintf(pTxt, fmt, ap);
	#endif
	va_end(ap);

	MessageBoxA(NULL, pTxt, "Error!", 0);
}

bool CScriptEngine::Load(char *fname)
{
	LPCSTR szLanguageName = "_GetLanguageName";
	LPCSTR szScriptName = "_GetScriptName";
	LPCSTR szInitialDirectory = "_GetInitialDirectory";
	LPCSTR szLoadScript = "_LoadScript";
	LPCSTR szConvert  = "_Convert";

	if(IsLoaded())
		Free();

	hDll = LoadLibraryA(fname);

	if(hDll){
		GetLanguageName = (PFNGETLANGUAGENAME)GetProcAddress(hDll, szLanguageName);
		GetScriptName = (PFNGETSCRIPTNAME)GetProcAddress(hDll, szScriptName);
		GetInitialDirectory = (PFNGETINITIALDIRECTORY)GetProcAddress(hDll, szInitialDirectory);
		LoadScript = (PFNLOADSCRIPT)GetProcAddress(hDll, szLoadScript);
		Convert = (PFNCONVERT)GetProcAddress(hDll, szConvert);

		LPCSTR FuncErrMsg = "\"%s\" dll: \"%s\" function not found.";

		if(!GetLanguageName){
			PrintErrorMsg(FuncErrMsg, fname, szLanguageName);
			return false;
		}
		if(!GetScriptName){
			PrintErrorMsg(FuncErrMsg, fname, szScriptName);
			return false;
		}
		if(!GetInitialDirectory){
			PrintErrorMsg(FuncErrMsg, fname, szInitialDirectory);
			return false;
		}
		if(!LoadScript){
			PrintErrorMsg(FuncErrMsg, fname, szLoadScript);
			return false;
		}
		if(!Convert){
			PrintErrorMsg(FuncErrMsg, fname, szConvert);
			return false;
		}

		return true;
	} else {
		PrintErrorMsg("Unable to load \"%s\".", fname);
	}

	return false;
}

void CScriptEngine::Free()
{
	if(IsLoaded())
		FreeLibrary(hDll);

	hDll = NULL;

	GetLanguageName = NULL;
	GetScriptName = NULL;
	GetInitialDirectory = NULL;
	LoadScript = NULL;
	Convert = NULL;
}




