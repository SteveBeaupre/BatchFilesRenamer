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

char* ExtractFilePath(char *fullpath)
{
	static char Path[MAX_PATH];
	ZeroMemory(Path, MAX_PATH);

	int Pos = -1;
	int Size = strlen(fullpath);

	for(int i = Size-1; i >= 0; i--){
		if(fullpath[i] == '\\'){
			Pos = i;
			break;
		}
	}

	if(Pos > 0)
		memcpy(Path, fullpath, Pos); 

	return Path;
}

// Set paths to directories
/*void SetPythonPaths(char *AppDir)
{
	LPCSTR DllsDir = "\\Python27\\DLLs";
	LPCSTR LibDir = "\\Python27\\Lib";
	LPCSTR libtkDir = "\\Python27\\Lib\\lib-tk";

	int PathBufSize = (strlen(AppDir) * 3) + strlen(DllsDir) + strlen(LibDir) + strlen(libtkDir) + 3;
		
	char *PythonPaths = new char[PathBufSize];
	ZeroMemory(PythonPaths, PathBufSize);
	sprintf(PythonPaths, "%s%s;%s%s;%s%s", AppDir, DllsDir, AppDir, LibDir, AppDir, libtkDir);
	PySys_SetPath(PythonPaths);
	delete [] PythonPaths;
}*/

// Set paths to .zip file
void SetPythonPaths(char *AppDir)
{
	LPCSTR DllsDir = "/DLLs";
	LPCSTR LibDir = "/Lib";
	LPCSTR libtkDir = "/Lib/lib-tk";
	
	LPCSTR ZipName = "\\Python27.zip";

	int PathBufSize = (strlen(AppDir) * 3) + (strlen(ZipName) * 3) + strlen(DllsDir) + strlen(LibDir) + strlen(libtkDir) + 3;
		
	char *PythonPaths = new char[PathBufSize];
	ZeroMemory(PythonPaths, PathBufSize);
	sprintf(PythonPaths, "%s%s%s;%s%s%s;%s%s%s", AppDir, ZipName, DllsDir, AppDir, ZipName, LibDir, AppDir, ZipName, libtkDir);
	PySys_SetPath(PythonPaths);
	delete [] PythonPaths;
}

void CPythonScript::InitPython()
{
	if(!Initialized){

		Py_NoSiteFlag=1;
		Py_Initialize();
		module = PyImport_AddModule("__main__");
		dictionary = PyModule_GetDict(module);

		SetPythonPaths(ExtractFilePath(Py_GetProgramFullPath()));

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

	bool res = true;

	try {
		FILE *f = fopen(scriptfile.c_str(), "rt");
		if(f){
			PyRun_File(f, scriptfile.c_str(), Py_file_input, dictionary, dictionary);
			fclose(f);
		}
	} catch(...) {
		res = false;
	}

	return res;
}

bool CPythonScript::LoadScriptFromText(string scripttext)
{
	ResetPython();

	bool res = true;

	try {
		PyRun_String(scripttext.c_str(), Py_file_input, dictionary, dictionary);
	} catch(...) {
		res = false;
	}

	return res;
}

void CPythonScript::CallFunction(string name)
{
	//call script function, 0 args, 0 retvals
	PyObject* pFunc = PyObject_GetAttrString(module, name.c_str());
	PyObject_CallObject(pFunc, 0);
	Py_XDECREF(pFunc);
}

bool CPythonScript::SafeCallFunction(string name, char *err)
{
	//call script function, 0 args, 0 retvals
	PyObject* pFunc = PyObject_GetAttrString(module, name.c_str());
	if(pFunc)
		PyObject_CallObject(pFunc, 0);
	Py_XDECREF(pFunc);

	if (PyErr_Occurred()) {

		PyObject *errtype, *errvalue, *traceback;
		PyErr_Fetch(&errtype, &errvalue, &traceback);
		if(errvalue != NULL) {
				
			PyObject *et = PyObject_Str(errtype);
			PyObject *ev = PyObject_Str(errvalue);
			PyObject *tb = PyObject_Str(traceback);

			//PyObject *lineno = PyObject_GetAttrString(tb, "tb_lineno");
			//int plain_lineno = PyInt_AsLong(lineno);
			//Py_DECREF(lineno);

			char *s_et = PyString_AsString(et);
			char *s_ev = PyString_AsString(ev);
			char *s_tb = PyString_AsString(tb);

			snprintf(err, 1024, "Error: %s", s_ev);
			//snprintf(err, 1024, "Error (line %d): %s", plain_lineno, s_ev);

			Py_DECREF(et);
			Py_DECREF(ev);
			Py_DECREF(tb);
		}
		Py_XDECREF(errvalue);
		Py_XDECREF(errtype);
		Py_XDECREF(traceback);
	}

	return true;
}

string CPythonScript::GetGlobalString(string name)
{
	string value = "";

	PyObject* s = PyDict_GetItemString(dictionary, name.c_str());
	if(s){
		char *s_Ansi = PyString_AsString(s);
		if(s_Ansi)
			value = s_Ansi;
	}

	return value;
}

void CPythonScript::SetGlobalString(string name, string value)
{
	PyObject *s = PyString_FromString(value.c_str());
	if(s){
		PyDict_SetItemString(dictionary, name.c_str(), s);
		Py_DECREF(s);
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

