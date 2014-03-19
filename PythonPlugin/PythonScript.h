#pragma once
//----------------------------------------------------------------------//
#include <Python.h>
//----------------------------------------------------------------------//
#include "Windows.h"
#include <string>
//----------------------------------------------------------------------//
using namespace std;

class CPythonScript {
protected:
	//lua_State *L;
	bool Initialized;
	
	PyObject* module;
	PyObject* dictionary;
public:
	CPythonScript();
	~CPythonScript();

	void InitPython();
	void ClosePython();
	void ResetPython();

	bool LoadScriptFromFile(string scriptfile);
	bool LoadScriptFromText(string scripttext);
	
	void CallFunction(string name);
	bool SafeCallFunction(string name, char *err = NULL);

	string GetGlobalString(string name);
	void   SetGlobalString(string name, string value);
	
	/*double GetGlobalNumber(string name);
	void   SetGlobalNumber(string name, double value);
	
	bool GetGlobalBoolean(string name);
	void SetGlobalBoolean(string name, bool value); */
};
