#include <iostream>
#include "calculation.h"
#include "python2.7/Python.h"

using namespace std;

double calculation(string cal)
{
   Py_Initialize();
   
   string path = "/home/cxy/rapidnet_w/src/rapidnet";
   string chdir_cmd = string("sys.path.append(\"") + path + "\")";
    const char* cstr_cmd = chdir_cmd.c_str();
    PyRun_SimpleString("import sys");
    PyRun_SimpleString(cstr_cmd);

    PyObject* moduleName = PyString_FromString("calculate"); //module name
    PyObject* pModule = PyImport_Import(moduleName);
    if (!pModule) // fail
    {
        cout << "[ERROR] Python get module failed." << endl;
        cout << cstr_cmd<<endl;
        return 0;
    }
    //cout << "[INFO] Python get module succeed." << endl;
    
    
    PyObject* pv = PyObject_GetAttrString(pModule, "calculate");
    if (!pv || !PyCallable_Check(pv))  
    {
        cout << "[ERROR] Can't find funftion (calculate)" << endl;
        return 0;
    }
    //cout << "[INFO] Get function (calculate) succeed." << endl;
    
    
    PyObject* args = PyTuple_New(1);   
    
    char *input = const_cast<char *>(cal.c_str());
    PyObject* arg1 = PyString_FromString(input);    
    
    PyTuple_SetItem(args, 0, arg1);
    
    PyObject* pRet = PyObject_CallObject(pv, args);
    
    double result;
    
    if (pRet)  
    {
        result = PyFloat_AsDouble(pRet);
    }
    else
    {result = -1.0;}
    Py_Finalize();      
    return result;



};
