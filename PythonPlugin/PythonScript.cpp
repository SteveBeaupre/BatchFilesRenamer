#include "PythonScript.h"

CPythonScript::CPythonScript()
{
	Initialized = false;
	InitPython();
}

CPythonScript::~CPythonScript()
{
	ClosePython();	
}

void CPythonScript::InitPython()
{
	if(!Initialized){
		Py_Initialize();
		module = PyImport_AddModule("__main__");
		dictionary = PyModule_GetDict(module); 
		Initialized = true;
	}
}

void CPythonScript::ClosePython()
{
	if(Initialized){
		Py_Finalize();
		module = dictionary = NULL;
		Initialized = false;
	}
}

void CPythonScript::ResetPython()
{
	ClosePython();
	InitPython();
}

bool CPythonScript::LoadScriptFromFile(string scriptfile)
{
	ResetPython();

	try {
		FILE *f = fopen(scriptfile.c_str(), "rt");
		if(f){
			PyRun_File(f, scriptfile.c_str(), Py_file_input, dictionary, dictionary);
			fclose(f);
		}
	} catch(...) {
		return false;
	}

	return true;
}

bool CPythonScript::LoadScriptFromText(string scripttext)
{
	ResetPython();

	try {
		PyRun_String(scripttext.c_str(), Py_file_input, dictionary, dictionary);
	} catch(...) {
		return false;
	}

	return true;
}

void CPythonScript::CallFunction(string name)
{
	//call script function, 0 args, 0 retvals
	PyObject* pFunc = PyObject_GetAttrString(module, name.c_str());
	PyObject_CallObject(pFunc, 0);
	//Py_DECREF(pFunc);
}

bool CPythonScript::SafeCallFunction(string name, char *err)
{
	//call script function, 0 args, 0 retvals
	PyObject* pFunc = PyObject_GetAttrString(module, name.c_str());
	if(pFunc){
		PyObject_CallObject(pFunc, 0);
		Py_DECREF(pFunc);
	}

	int have_error = PyErr_Occurred() ? 1 : 0;
	
	if(have_error){

		//const char *pErrMsg = getPythonTraceback();

		if(!err){
			string Caption;
			Caption = "Python: Error in function " + name + "().";

			//MessageBox(0, pErrMsg, Caption.c_str(), MB_OK);
		} else {
			sprintf(err, "Python: Error in function %s().", name.c_str());
		}

		return false;
	}

	return true;
}

string CPythonScript::GetGlobalString(string name)
{
	string value = "";

	PyObject* s = PyDict_GetItemString(dictionary, name.c_str());
	if(s){
		PyObject *s_Ansi = PyUnicode_AsEncodedString(s, "utf-8", "");
		const char *pValue = PyBytes_AS_STRING(s_Ansi);
		Py_DECREF(s);
		if(pValue)
			value = pValue;
	}

	return value;
}

void CPythonScript::SetGlobalString(string name, string value)
{
	PyObject *s = PyUnicode_FromString(value.c_str());
	if(s){
		PyDict_SetItemString(dictionary, name.c_str(), s);
		Py_XDECREF(s);
	}
}

/*double CPythonScript::GetGlobalNumber(string name)
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

void CPythonScript::SetGlobalNumber(string name, double value)
{
	lua_pushnumber(L, (int)value);
	lua_setglobal(L, name.c_str());
}

bool CPythonScript::GetGlobalBoolean(string name)
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

void CPythonScript::SetGlobalBoolean(string name, bool value)
{
	lua_pushboolean(L, (int)value);
	lua_setglobal(L, name.c_str());
}*/

/*char* getPythonTraceback()
{
    PyObject *type, *value, *traceback;
    PyObject *tracebackModule;
    char *chrRetval;

    PyErr_Fetch(&type, &value, &traceback);

    tracebackModule = PyImport_ImportModule("traceback");
    if (tracebackModule != NULL)
    {
        PyObject *tbList, *emptyString, *strRetval;

        tbList = PyObject_CallMethod(tracebackModule, "format_exception", "OOO", type, value == NULL ? Py_None : value, traceback == NULL ? Py_None : traceback);

        emptyString = PyUnicode_FromString("");
        strRetval = PyObject_CallMethod(emptyString, "join", "O", tbList);

		PyObject *strRetval_Ansi = PyUnicode_AsEncodedString(strRetval, "utf-8", "");
		chrRetval = strdup(PyBytes_AS_STRING(strRetval));
        //chrRetval = strdup(PyString_AsString(strRetval));

        //Py_DECREF(tbList);
        //Py_DECREF(emptyString);
        //Py_DECREF(strRetval);
        //Py_DECREF(tracebackModule);
    } else {
        chrRetval = strdup("Unable to import traceback module.");
    }

    //Py_DECREF(type);
    //Py_XDECREF(value);
    //Py_XDECREF(traceback);

    return chrRetval;
}*/

