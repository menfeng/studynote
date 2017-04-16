//pythonÔ´Âë½âÎö±Ê¼Ç
/*
============summary
1.import a.b
2.IMPORT_NAME
//formlist as xxx1,xxx2
//level may -1
3.PyEval_CallObject(builtin___import__,(name,f_globals,g_locals,fromlist,level)
4.PyImport_ImportModuleLevel(char *name, PyObject *globals, PyObject *locals,PyObject *fromlist, int level)
5.import_module_level(char *name, PyObject *globals, PyObject *locals,PyObject *fromlist, int level)
	1.get_parent
	2.load_next
	3.return head or tail
//altmod can be None[import sys in a.b]
6.load_next(PyObject *mod, PyObject *altmod, char **p_name, char *buf, Py_ssize_t *p_buflen)
7.import_submodule(PyObject *mod, char *subname, char *fullname)
        fdp = find_module(fullname, subname, path, buf, MAXPATHLEN+1,&fp, &loader);
		load_module(char *name, FILE *fp, char *pathname, int type, PyObject *loader)
		load_source_module(char *name, char *pathname, FILE *fp)
        	1.co = parse_source_module(pathname, fp);
    		2.PyImport_ExecCodeModuleEx(name, (PyObject *)co, pathname);
    			1.m = PyImport_AddModule(name);
    			2.d = PyModule_GetDict(m);
    			3.PyEval_EvalCode((PyCodeObject *)co, d, d);
        add_submodule(mod, m, fullname, subname, modules))[a.b=b]

============about namespace
in a.py
a=1
import b 
b.F()

in b.py
a=2
def F():
	print 2
1.import b will give a Null as b'global
2.give b's gloabal to F when F is created
3.give F's global to new frame when b.F()


==============sys.meta_path
class CFinder:
	def find_module(self,fullname,path=None):
		if fullname=="xxx":
			return self
	def load_module(self,fullname):
		import sys
		return sys
	
sys.meta_path.append(CFinder())
import xxx
xxx
<module 'sys' (built-in)>
>>> 
 */


s="""
import sys
"""
co=compile(s,"import.py","exec")
co.co_consts
(-1, None)
co.co_names
('sys',)
dis.dis(co)
import dis
dis.dis(co)
  2           0 LOAD_CONST               0 (-1)
              3 LOAD_CONST               1 (None)
              6 IMPORT_NAME              0 (sys)
              9 STORE_NAME               0 (sys)
             12 LOAD_CONST               1 (None)
             15 RETURN_VALUE        

s="""
import a.b.c
"""
  2           0 LOAD_CONST               0 (-1)
              3 LOAD_CONST               1 (None)
              6 IMPORT_NAME              0 (a.b.c)
              9 STORE_NAME               1 (a)			//return a!!!
             12 LOAD_CONST               1 (None)
             15 RETURN_VALUE    
s="""
from a.b import c
"""
  2           0 LOAD_CONST               0 (-1)
              3 LOAD_CONST               1 (('c',))
              6 IMPORT_NAME              0 (a.b)
              9 IMPORT_FROM              1 (c)
             12 STORE_NAME               1 (c)
             15 POP_TOP             
             16 LOAD_CONST               2 (None)
             19 RETURN_VALUE      

s="""
from a.b import c as x
"""
  2           0 LOAD_CONST               0 (-1)
              3 LOAD_CONST               1 (('c',))
              6 IMPORT_NAME              0 (a.b)
              9 IMPORT_FROM              1 (c)
             12 STORE_NAME               2 (x)
             15 POP_TOP             
             16 LOAD_CONST               2 (None)
             19 RETURN_VALUE    
s="""
from ..a import b
"""
  2           0 LOAD_CONST               0 (2)
              3 LOAD_CONST               1 (('b',))
              6 IMPORT_NAME              0 (a)
              9 IMPORT_FROM              1 (b)
             12 STORE_NAME               1 (b)
             15 POP_TOP             
             16 LOAD_CONST               2 (None)
             19 RETURN_VALUE       
s="""
from a import *
"""
  2           0 LOAD_CONST               0 (-1)
              3 LOAD_CONST               1 (('*',))
              6 IMPORT_NAME              0 (a)
              9 IMPORT_STAR         
             10 LOAD_CONST               2 (None)
             13 RETURN_VALUE        

case IMPORT_FROM:
	w = GETITEM(names, oparg);
	v = TOP();
	READ_TIMESTAMP(intr0);
	x = import_from(v, w);
	READ_TIMESTAMP(intr1);
	PUSH(x);
	if (x != NULL) continue;
	break;
static PyObject *
import_from(PyObject *v, PyObject *name)
{
    PyObject *x;

    x = PyObject_GetAttr(v, name);
    if (x == NULL && PyErr_ExceptionMatches(PyExc_AttributeError)) {
        PyErr_Format(PyExc_ImportError,
                     "cannot import name %.230s",
                     PyString_AsString(name));
    }
    return x;
}

case IMPORT_NAME:
	w = GETITEM(names, oparg);
	x = PyDict_GetItemString(f->f_builtins, "__import__");
	if (x == NULL) {
		PyErr_SetString(PyExc_ImportError,
						"__import__ not found");
		break;
	}
	Py_INCREF(x);
	v = POP();
	u = TOP();
	if (PyInt_AsLong(u) != -1 || PyErr_Occurred())
		w = PyTuple_Pack(5,
					w,
					f->f_globals,
					f->f_locals == NULL ?
						  Py_None : f->f_locals,
					v,
					u);
	else
		w = PyTuple_Pack(4,
					w,
					f->f_globals,
					f->f_locals == NULL ?
						  Py_None : f->f_locals,
					v);
	Py_DECREF(v);
	Py_DECREF(u);
	if (w == NULL) {
		u = POP();
		Py_DECREF(x);
		x = NULL;
		break;
	}
	READ_TIMESTAMP(intr0);
	v = x;
	x = PyEval_CallObject(v, w);
	Py_DECREF(v);
	READ_TIMESTAMP(intr1);
	Py_DECREF(w);
	SET_TOP(x);
	if (x != NULL) continue;
	break;
/* Inline this */
#define PyEval_CallObject(func,arg) \
    PyEval_CallObjectWithKeywords(func, arg, (PyObject *)NULL)
PyObject *
PyEval_CallObjectWithKeywords(PyObject *func, PyObject *arg, PyObject *kw)
{
    PyObject *result;

    if (arg == NULL) {
        arg = PyTuple_New(0);
        if (arg == NULL)
            return NULL;
    }
    else if (!PyTuple_Check(arg)) {
        PyErr_SetString(PyExc_TypeError,
                        "argument list must be a tuple");
        return NULL;
    }
    else
        Py_INCREF(arg);

    if (kw != NULL && !PyDict_Check(kw)) {
        PyErr_SetString(PyExc_TypeError,
                        "keyword list must be a dictionary");
        Py_DECREF(arg);
        return NULL;
    }

    result = PyObject_Call(func, arg, kw);
    Py_DECREF(arg);
    return result;
}

PyObject *
PyObject_Call(PyObject *func, PyObject *arg, PyObject *kw)
{
    ternaryfunc call;

    if ((call = func->ob_type->tp_call) != NULL) {
        PyObject *result;
        if (Py_EnterRecursiveCall(" while calling a Python object"))
            return NULL;
        result = (*call)(func, arg, kw);
        Py_LeaveRecursiveCall();
        if (result == NULL && !PyErr_Occurred())
            PyErr_SetString(
                PyExc_SystemError,
                "NULL result without error in PyObject_Call");
        return result;
    }
    PyErr_Format(PyExc_TypeError, "'%.200s' object is not callable",
                 func->ob_type->tp_name);
    return NULL;
}

typedef struct {
    PyObject_HEAD
    PyMethodDef *m_ml; /* Description of the C function to call */
    PyObject    *m_self; /* Passed as 'self' arg to the C func, can be NULL */
    PyObject    *m_module; /* The __module__ attribute, can be anything */
} PyCFunctionObject;

PyTypeObject PyCFunction_Type = {
    PyVarObject_HEAD_INIT(&PyType_Type, 0)
    "builtin_function_or_method",
    sizeof(PyCFunctionObject),
    0,
    (destructor)meth_dealloc,                   /* tp_dealloc */
    0,                                          /* tp_print */
    0,                                          /* tp_getattr */
    0,                                          /* tp_setattr */
    (cmpfunc)meth_compare,                      /* tp_compare */
    (reprfunc)meth_repr,                        /* tp_repr */
    0,                                          /* tp_as_number */
    0,                                          /* tp_as_sequence */
    0,                                          /* tp_as_mapping */
    (hashfunc)meth_hash,                        /* tp_hash */
    PyCFunction_Call,                           /* tp_call */
    0,                                          /* tp_str */
    PyObject_GenericGetAttr,                    /* tp_getattro */
    0,                                          /* tp_setattro */
    0,                                          /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_GC,/* tp_flags */
    0,                                          /* tp_doc */
    (traverseproc)meth_traverse,                /* tp_traverse */
    0,                                          /* tp_clear */
    meth_richcompare,                                           /* tp_richcompare */
    0,                                          /* tp_weaklistoffset */
    0,                                          /* tp_iter */
    0,                                          /* tp_iternext */
    0,                                          /* tp_methods */
    meth_members,                               /* tp_members */
    meth_getsets,                               /* tp_getset */
    0,                                          /* tp_base */
    0,                                          /* tp_dict */
};
PyObject *
PyCFunction_Call(PyObject *func, PyObject *arg, PyObject *kw)
{
    PyCFunctionObject* f = (PyCFunctionObject*)func;
    PyCFunction meth = PyCFunction_GET_FUNCTION(func);
    PyObject *self = PyCFunction_GET_SELF(func);
    Py_ssize_t size;

    switch (PyCFunction_GET_FLAGS(func) & ~(METH_CLASS | METH_STATIC | METH_COEXIST)) {
    case METH_VARARGS:
        if (kw == NULL || PyDict_Size(kw) == 0)
            return (*meth)(self, arg);
        break;
    case METH_VARARGS | METH_KEYWORDS:
    case METH_OLDARGS | METH_KEYWORDS:
		//***********************call here
        return (*(PyCFunctionWithKeywords)meth)(self, arg, kw);
    case METH_NOARGS:
        if (kw == NULL || PyDict_Size(kw) == 0) {
            size = PyTuple_GET_SIZE(arg);
            if (size == 0)
                return (*meth)(self, NULL);
            PyErr_Format(PyExc_TypeError,
                "%.200s() takes no arguments (%zd given)",
                f->m_ml->ml_name, size);
            return NULL;
        }
        break;
    case METH_O:
        if (kw == NULL || PyDict_Size(kw) == 0) {
            size = PyTuple_GET_SIZE(arg);
            if (size == 1)
                return (*meth)(self, PyTuple_GET_ITEM(arg, 0));
            PyErr_Format(PyExc_TypeError,
                "%.200s() takes exactly one argument (%zd given)",
                f->m_ml->ml_name, size);
            return NULL;
        }
        break;
    case METH_OLDARGS:
        /* the really old style */
        if (kw == NULL || PyDict_Size(kw) == 0) {
            size = PyTuple_GET_SIZE(arg);
            if (size == 1)
                arg = PyTuple_GET_ITEM(arg, 0);
            else if (size == 0)
                arg = NULL;
            return (*meth)(self, arg);
        }
        break;
    default:
        PyErr_BadInternalCall();
        return NULL;
    }
    PyErr_Format(PyExc_TypeError, "%.200s() takes no keyword arguments",
                 f->m_ml->ml_name);
    return NULL;
}

//bltinmodule.c
static PyObject *
builtin___import__(PyObject *self, PyObject *args, PyObject *kwds)
{
    static char *kwlist[] = {"name", "globals", "locals", "fromlist",
                             "level", 0};
    char *name;
    PyObject *globals = NULL;
    PyObject *locals = NULL;
    PyObject *fromlist = NULL;
    int level = -1;

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "s|OOOi:__import__",
                    kwlist, &name, &globals, &locals, &fromlist, &level))
        return NULL;
    return PyImport_ImportModuleLevel(name, globals, locals,
                                      fromlist, level);
}

//import.c
PyObject *
PyImport_ImportModuleLevel(char *name, PyObject *globals, PyObject *locals,
                         PyObject *fromlist, int level)
{
    PyObject *result;
    _PyImport_AcquireLock();
    result = import_module_level(name, globals, locals, fromlist, level);
    if (_PyImport_ReleaseLock() < 0) {
        Py_XDECREF(result);
        PyErr_SetString(PyExc_RuntimeError,
                        "not holding the import lock");
        return NULL;
    }
    return result;
}
static PyObject *
import_module_level(char *name, PyObject *globals, PyObject *locals,
                    PyObject *fromlist, int level)
{
    char *buf;
    Py_ssize_t buflen = 0;
    PyObject *parent, *head, *next, *tail;

    if (strchr(name, '/') != NULL
#ifdef MS_WINDOWS
        || strchr(name, '\\') != NULL
#endif
        ) {
        PyErr_SetString(PyExc_ImportError,
                        "Import by filename is not supported.");
        return NULL;
    }

    buf = PyMem_MALLOC(MAXPATHLEN+1);
    if (buf == NULL) {
        return PyErr_NoMemory();
    }
    parent = get_parent(globals, buf, &buflen, level);
    if (parent == NULL)
        goto error_exit;

    head = load_next(parent, level < 0 ? Py_None : parent, &name, buf,
                        &buflen);
    if (head == NULL)
        goto error_exit;

    tail = head;
    Py_INCREF(tail);
    while (name) {
        next = load_next(tail, tail, &name, buf, &buflen);
        Py_DECREF(tail);
        if (next == NULL) {
            Py_DECREF(head);
            goto error_exit;
        }
        tail = next;
    }
    if (tail == Py_None) {
        /* If tail is Py_None, both get_parent and load_next found
           an empty module name: someone called __import__("") or
           doctored faulty bytecode */
        Py_DECREF(tail);
        Py_DECREF(head);
        PyErr_SetString(PyExc_ValueError,
                        "Empty module name");
        goto error_exit;
    }

    if (fromlist != NULL) {
        int b = (fromlist == Py_None) ? 0 : PyObject_IsTrue(fromlist);
        if (b < 0) {
            Py_DECREF(tail);
            Py_DECREF(head);
            goto error_exit;
        }
        if (!b)
            fromlist = NULL;
    }

    if (fromlist == NULL) {
        Py_DECREF(tail);
        PyMem_FREE(buf);
        return head;
    }

    Py_DECREF(head);
    if (!ensure_fromlist(tail, fromlist, buf, buflen, 0)) {
        Py_DECREF(tail);
        goto error_exit;
    }

    PyMem_FREE(buf);
    return tail;

error_exit:
    PyMem_FREE(buf);
    return NULL;
}
/* altmod is either None or same as mod */
static PyObject *
load_next(PyObject *mod, PyObject *altmod, char **p_name, char *buf,
          Py_ssize_t *p_buflen)
{
    char *name = *p_name;
    char *dot = strchr(name, '.');
    size_t len;
    char *p;
    PyObject *result;

    if (strlen(name) == 0) {
        /* completely empty module name should only happen in
           'from . import' (or '__import__("")')*/
        Py_INCREF(mod);
        *p_name = NULL;
        return mod;
    }

    if (dot == NULL) {
        *p_name = NULL;
        len = strlen(name);
    }
    else {
        *p_name = dot+1;
        len = dot-name;
    }
    if (len == 0) {
        PyErr_SetString(PyExc_ValueError,
                        "Empty module name");
        return NULL;
    }

    p = buf + *p_buflen;
    if (p != buf)
        *p++ = '.';
    if (p+len-buf >= MAXPATHLEN) {
        PyErr_SetString(PyExc_ValueError,
                        "Module name too long");
        return NULL;
    }
    strncpy(p, name, len);
    p[len] = '\0';
    *p_buflen = p+len-buf;

    result = import_submodule(mod, p, buf);
    if (result == Py_None && altmod != mod) {
        Py_DECREF(result);
        /* Here, altmod must be None and mod must not be None */
        result = import_submodule(altmod, p, p);
        if (result != NULL && result != Py_None) {
            if (mark_miss(buf) != 0) {
                Py_DECREF(result);
                return NULL;
            }
            strncpy(buf, name, len);
            buf[len] = '\0';
            *p_buflen = len;
        }
    }
    if (result == NULL)
        return NULL;

    if (result == Py_None) {
        Py_DECREF(result);
        PyErr_Format(PyExc_ImportError,
                     "No module named %.200s", name);
        return NULL;
    }

    return result;
}
static PyObject *
import_submodule(PyObject *mod, char *subname, char *fullname)
{
    PyObject *modules = PyImport_GetModuleDict();
    PyObject *m = NULL;

    /* Require:
       if mod == None: subname == fullname
       else: mod.__name__ + "." + subname == fullname
    */

    if ((m = PyDict_GetItemString(modules, fullname)) != NULL) {
        Py_INCREF(m);
    }
    else {
        PyObject *path, *loader = NULL;
        char *buf;
        struct filedescr *fdp;
        FILE *fp = NULL;

        if (mod == Py_None)
            path = NULL;
        else {
            path = PyObject_GetAttrString(mod, "__path__");
            if (path == NULL) {
                PyErr_Clear();
                Py_INCREF(Py_None);
                return Py_None;
            }
        }

        buf = PyMem_MALLOC(MAXPATHLEN+1);
        if (buf == NULL) {
            return PyErr_NoMemory();
        }
        buf[0] = '\0';
        fdp = find_module(fullname, subname, path, buf, MAXPATHLEN+1,
                          &fp, &loader);
        Py_XDECREF(path);
        if (fdp == NULL) {
            PyMem_FREE(buf);
            if (!PyErr_ExceptionMatches(PyExc_ImportError))
                return NULL;
            PyErr_Clear();
            Py_INCREF(Py_None);
            return Py_None;
        }
        m = load_module(fullname, fp, buf, fdp->type, loader);
        Py_XDECREF(loader);
        if (fp)
            fclose(fp);
        if (!add_submodule(mod, m, fullname, subname, modules)) {
            Py_XDECREF(m);
            m = NULL;
        }
        PyMem_FREE(buf);
    }

    return m;
}

//>>>>>>>>>
//the type of fdp is filedescr in importdl.h
//_PyImport_Filetab is inited in _PyImport_Init use
//_PyImport_DynLoadFiletab and _PyImport_StandardFiletab

//_PyImport_StandardFiletab in import.c
#ifdef RISCOS
static const struct filedescr _PyImport_StandardFiletab[] = {
    {"/py", "U", PY_SOURCE},
    {"/pyc", "rb", PY_COMPILED},
    {0, 0}
};
#else
static const struct filedescr _PyImport_StandardFiletab[] = {
    {".py", "U", PY_SOURCE},
#ifdef MS_WINDOWS
    {".pyw", "U", PY_SOURCE},
#endif
    {".pyc", "rb", PY_COMPILED},
    {0, 0}
};
#endif

//_PyImport_DynLoadFiletab in dynload_win.c
const struct filedescr _PyImport_DynLoadFiletab[] = {
#ifdef _DEBUG
    {"_d.pyd", "rb", C_EXTENSION},
#else
    {".pyd", "rb", C_EXTENSION},
#endif
    {0, 0}
};

enum filetype {
    SEARCH_ERROR,
    PY_SOURCE,
    PY_COMPILED,
    C_EXTENSION,
    PY_RESOURCE, /* Mac only */
    PKG_DIRECTORY,
    C_BUILTIN,
    PY_FROZEN,
    PY_CODERESOURCE, /* Mac only */
    IMP_HOOK
};

struct filedescr {
    char *suffix;
    char *mode;
    enum filetype type;
};

extern struct filedescr * _PyImport_Filetab;
//>>>>>>>>>>>>>>

//find_module: 
//fp opening file
//fdp filedescr
//buf the full name of find file 

//load_module
static PyObject *
load_module(char *name, FILE *fp, char *pathname, int type, PyObject *loader)
{
    PyObject *modules;
    PyObject *m;
    int err;

    /* First check that there's an open file (if we need one)  */
    switch (type) {
    case PY_SOURCE:
    case PY_COMPILED:
        if (fp == NULL) {
            PyErr_Format(PyExc_ValueError,
               "file object required for import (type code %d)",
                         type);
            return NULL;
        }
    }

    switch (type) {

    case PY_SOURCE:
        m = load_source_module(name, pathname, fp);
        break;

    case PY_COMPILED:
        m = load_compiled_module(name, pathname, fp);
        break;

#ifdef HAVE_DYNAMIC_LOADING
    case C_EXTENSION:
        m = _PyImport_LoadDynamicModule(name, pathname, fp);
        break;
#endif

    case PKG_DIRECTORY:
        m = load_package(name, pathname);
        break;

    case C_BUILTIN:
    case PY_FROZEN:
        if (pathname != NULL && pathname[0] != '\0')
            name = pathname;
        if (type == C_BUILTIN)
            err = init_builtin(name);
        else
            err = PyImport_ImportFrozenModule(name);
        if (err < 0)
            return NULL;
        if (err == 0) {
            PyErr_Format(PyExc_ImportError,
                         "Purported %s module %.200s not found",
                         type == C_BUILTIN ?
                                    "builtin" : "frozen",
                         name);
            return NULL;
        }
        modules = PyImport_GetModuleDict();
        m = PyDict_GetItemString(modules, name);
        if (m == NULL) {
            PyErr_Format(
                PyExc_ImportError,
                "%s module %.200s not properly initialized",
                type == C_BUILTIN ?
                    "builtin" : "frozen",
                name);
            return NULL;
        }
        Py_INCREF(m);
        break;

    case IMP_HOOK: {
        if (loader == NULL) {
            PyErr_SetString(PyExc_ImportError,
                            "import hook without loader");
            return NULL;
        }
        m = PyObject_CallMethod(loader, "load_module", "s", name);
        break;
    }

    default:
        PyErr_Format(PyExc_ImportError,
                     "Don't know how to import %.200s (type code %d)",
                      name, type);
        m = NULL;

    }

    return m;
}
PyObject *
_PyImport_LoadDynamicModule(char *name, char *pathname, FILE *fp)
{
	...
    p = _PyImport_GetDynLoadFunc(name, shortname, pathname, fp);
    (*p)();
	...
}
#define Py_InitModule(name, methods) \
	Py_InitModule4(name, methods, (char *)NULL, (PyObject *)NULL, \
		       PYTHON_API_VERSION)
/*
abc.dll
static PyMethDef abc_methods={
{"hello",Hello,METH_VARCHARS,"say hello"}
{None,None}
}
EXPORT init initabc(void)
{
	Py_InitModule("abc",abc_methods)
	return 0
}
*/


//reload
static PyObject *
builtin_reload(PyObject *self, PyObject *v)
{
    if (PyErr_WarnPy3k("In 3.x, reload() is renamed to imp.reload()",
                       1) < 0)
        return NULL;

    return PyImport_ReloadModule(v);
}
PyObject *
PyImport_ReloadModule(PyObject *m)
{
	...
    newm = load_module(name, fp, buf, fdp->type, loader);
	...
}
//load_module-->load_source_module(if py mode)
static PyObject *
load_source_module(char *name, char *pathname, FILE *fp)
{
	...
    m = PyImport_ExecCodeModuleEx(name, (PyObject *)co, pathname);
	...
}

//if the module is exist,just update dict by PyEval_EvalCode
PyObject *
PyImport_ExecCodeModuleEx(char *name, PyObject *co, char *pathname)
{
    PyObject *modules = PyImport_GetModuleDict();
    PyObject *m, *d, *v;

    m = PyImport_AddModule(name);
    if (m == NULL)
        return NULL;
    /* If the module is being reloaded, we get the old module back
       and re-use its dict to exec the new code. */
    d = PyModule_GetDict(m);
    if (PyDict_GetItemString(d, "__builtins__") == NULL) {
        if (PyDict_SetItemString(d, "__builtins__",
                                 PyEval_GetBuiltins()) != 0)
            goto error;
    }
    /* Remember the filename as the __file__ attribute */
    v = NULL;
    if (pathname != NULL) {
        v = PyString_FromString(pathname);
        if (v == NULL)
            PyErr_Clear();
    }
    if (v == NULL) {
        v = ((PyCodeObject *)co)->co_filename;
        Py_INCREF(v);
    }
    if (PyDict_SetItemString(d, "__file__", v) != 0)
        PyErr_Clear(); /* Not important enough to report */
    Py_DECREF(v);

    v = PyEval_EvalCode((PyCodeObject *)co, d, d);
    if (v == NULL)
        goto error;
    Py_DECREF(v);

    if ((m = PyDict_GetItemString(modules, name)) == NULL) {
        PyErr_Format(PyExc_ImportError,
                     "Loaded module %.200s not found in sys.modules",
                     name);
        return NULL;
    }

    Py_INCREF(m);

    return m;

  error:
    remove_module(name);
    return NULL;
}


/*
for ppt 
s="""
INT_SIZE=4
def Send():
    print "Send"
class CNet(object):
    m_IntSize=4
    def Send(self):
        print "Send"
"""
>>> co=compile(s,"net.py","exec")
>>> dis.dis(co)

Traceback (most recent call last):
  File "<pyshell#24>", line 1, in <module>
    dis.dis(co)
NameError: name 'dis' is not defined
>>> import dis
>>> dis.dis(co)
  2           0 LOAD_CONST               0 (4)
              3 STORE_NAME               0 (INT_SIZE)

  3           6 LOAD_CONST               1 (<code object Send at 00000000025FEB30, file "net.py", line 3>)
              9 MAKE_FUNCTION            0
             12 STORE_NAME               1 (Send)

  5          15 LOAD_CONST               2 ('CNet')
             18 LOAD_NAME                2 (object)
             21 BUILD_TUPLE              1
             24 LOAD_CONST               3 (<code object CNet at 00000000029D1030, file "net.py", line 5>)
             27 MAKE_FUNCTION            0
             30 CALL_FUNCTION            0
             33 BUILD_CLASS         
             34 STORE_NAME               3 (CNet)
             37 LOAD_CONST               4 (None)
             40 RETURN_VALUE        
>>> co.co_consts
(4, <code object Send at 00000000025FEB30, file "net.py", line 3>, 'CNet', <code object CNet at 00000000029D1030, file "net.py", line 5>, None)
>>> co.co_consts[1]
<code object Send at 00000000025FEB30, file "net.py", line 3>
>>> co.co_consts[3]
<code object CNet at 00000000029D1030, file "net.py", line 5>
>>> co_f=co.co_consts[1]
>>> c_co=co.co_consts[3]
>>> dis.dis(co_f)
  4           0 LOAD_CONST               1 ('Send')
              3 PRINT_ITEM          
              4 PRINT_NEWLINE       
              5 LOAD_CONST               0 (None)
              8 RETURN_VALUE        
>>> dis.dis(c_co)
  5           0 LOAD_NAME                0 (__name__)
              3 STORE_NAME               1 (__module__)

  6           6 LOAD_CONST               0 (4)
              9 STORE_NAME               2 (m_IntSize)

  7          12 LOAD_CONST               1 (<code object Send at 0000000002A105B0, file "net.py", line 7>)
             15 MAKE_FUNCTION            0
             18 STORE_NAME               3 (Send)
             21 LOAD_LOCALS         
             22 RETURN_VALUE        
>>> 
>>> c_co.co_consts
(<code object Send at 000000000261C0B0, file "net.py", line 6>,)
>>> s_co=c_co.co_consts[0]
>>> dis.dis(s_co)
  7           0 LOAD_CONST               1 ('Send')
              3 PRINT_ITEM          
              4 PRINT_NEWLINE       
              5 LOAD_CONST               0 (None)
              8 RETURN_VALUE        

*/

