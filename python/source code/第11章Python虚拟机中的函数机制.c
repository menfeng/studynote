//pythonÔ´Âë½âÎö±Ê¼Ç

//macros
#define GETLOCAL(i)     (fastlocals[i])
#define SETLOCAL(i, value)      do { PyObject *tmp = GETLOCAL(i); \
                                     GETLOCAL(i) = value; \
                                     Py_XDECREF(tmp); } while (0)

//struct
typedef struct {
    PyObject_HEAD
    PyObject *func_code;	/* A code object */
    PyObject *func_globals;	/* A dictionary (other mappings won't do) */
    PyObject *func_defaults;	/* NULL or a tuple */
    PyObject *func_closure;	/* NULL or a tuple of cell objects */
    PyObject *func_doc;		/* The __doc__ attribute, can be anything */
    PyObject *func_name;	/* The __name__ attribute, a string object */
    PyObject *func_dict;	/* The __dict__ attribute, a dict or NULL */
    PyObject *func_weakreflist;	/* List of weak references */
    PyObject *func_module;	/* The __module__ attribute, can be anything */

    /* Invariant:
     *     func_closure contains the bindings for func_code->co_freevars, so
     *     PyTuple_Size(func_closure) == PyCode_GetNumFree(func_code)
     *     (func_closure may be NULL if PyCode_GetNumFree(func_code) == 0).
     */
} PyFunctionObject;

//create
PyObject *
PyFunction_New(PyObject *code, PyObject *globals)
{
    PyFunctionObject *op = PyObject_GC_New(PyFunctionObject,
                                        &PyFunction_Type);
    static PyObject *__name__ = 0;
    if (op != NULL) {
        PyObject *doc;
        PyObject *consts;
        PyObject *module;
        op->func_weakreflist = NULL;
        Py_INCREF(code);
        op->func_code = code;
        Py_INCREF(globals);
        op->func_globals = globals;
        op->func_name = ((PyCodeObject *)code)->co_name;
        Py_INCREF(op->func_name);
        op->func_defaults = NULL; /* No default arguments */
        op->func_closure = NULL;
        consts = ((PyCodeObject *)code)->co_consts;
        if (PyTuple_Size(consts) >= 1) {
            doc = PyTuple_GetItem(consts, 0);
            if (!PyString_Check(doc) && !PyUnicode_Check(doc))
                doc = Py_None;
        }
        else
            doc = Py_None;
        Py_INCREF(doc);
        op->func_doc = doc;
        op->func_dict = NULL;
        op->func_module = NULL;

        /* __module__: If module name is in globals, use it.
           Otherwise, use None.
        */
        if (!__name__) {
            __name__ = PyString_InternFromString("__name__");
            if (!__name__) {
                Py_DECREF(op);
                return NULL;
            }
        }
        module = PyDict_GetItem(globals, __name__);
        if (module) {
            Py_INCREF(module);
            op->func_module = module;
        }
    }
    else
        return NULL;
    _PyObject_GC_TRACK(op);
    return (PyObject *)op;
}

//cellobject struct
typedef struct {
	PyObject_HEAD
	PyObject *ob_ref;	/* Content of the cell or NULL when empty */
} PyCellObject;

//cellobject create
PyObject *
PyCell_New(PyObject *obj)
{
    PyCellObject *op;

    op = (PyCellObject *)PyObject_GC_New(PyCellObject, &PyCell_Type);
    if (op == NULL)
        return NULL;
    op->ob_ref = obj;
    Py_XINCREF(obj);

    _PyObject_GC_TRACK(op);
    return (PyObject *)op;
}

int
PyCell_Set(PyObject *op, PyObject *obj)
{
    PyObject* oldobj;
    if (!PyCell_Check(op)) {
        PyErr_BadInternalCall();
        return -1;
    }
    oldobj = PyCell_GET(op);
    Py_XINCREF(obj);
    PyCell_SET(op, obj);
    Py_XDECREF(oldobj);
    return 0;
}
#define PyCell_SET(op, v) (((PyCellObject *)(op))->ob_ref = v)

PyObject *
PyCell_Get(PyObject *op)
{
    if (!PyCell_Check(op)) {
        PyErr_BadInternalCall();
        return NULL;
    }
    Py_XINCREF(((PyCellObject*)op)->ob_ref);
    return PyCell_GET(op);
}
#define PyCell_GET(op) (((PyCellObject *)(op))->ob_ref)

int
PyFunction_SetClosure(PyObject *op, PyObject *closure)
{
    if (!PyFunction_Check(op)) {
        PyErr_BadInternalCall();
        return -1;
    }
    if (closure == Py_None)
        closure = NULL;
    else if (PyTuple_Check(closure)) {
        Py_INCREF(closure);
    }
    else {
        PyErr_Format(PyExc_SystemError,
                     "expected tuple for closure, got '%.100s'",
                     closure->ob_type->tp_name);
        return -1;
    }
    Py_XDECREF(((PyFunctionObject *) op) -> func_closure);
    ((PyFunctionObject *) op) -> func_closure = closure;
    return 0;
}

/*
s="""
def f():
	print "Function"
f()
"""
co=compile(s,"func_0.py","exec")
co.co_consts
(<code object f at 0000000002981D30, file "func_0.py", line 2>, None)
co.co_names
('f',)
dis.dis(co)
  2           0 LOAD_CONST               0 (<code object f at 0000000002981D30, file "func_0.py", line 2>)
              3 MAKE_FUNCTION            0
              6 STORE_NAME               0 (f)

  4           9 LOAD_NAME                0 (f)
             12 CALL_FUNCTION            0
             15 POP_TOP             
             16 LOAD_CONST               1 (None)
             19 RETURN_VALUE       
co_f=co.co_consts[0]
co_f.co_consts
(None, 'Function')
co_f.co_names
()
dis.dis(co_f)
  3           0 LOAD_CONST               1 ('Function')
              3 PRINT_ITEM          
              4 PRINT_NEWLINE       
              5 LOAD_CONST               0 (None)
              8 RETURN_VALUE        

s="""
def f(name,age):
	age+=5
	print "[",name,", ",age,"]"
age=5
print age
f("Robert",age)
print age
"""
co=compile(s,"func_1.py","exec")
co.co_consts
(<code object f at 000000000258B8B0, file "func_1.py", line 2>, 5, 'Robert', None)
co.co_names
('f', 'age')
dis.dis(co)
  2           0 LOAD_CONST               0 (<code object f at 000000000258B8B0, file "func_1.py", line 2>)
              3 MAKE_FUNCTION            0
              6 STORE_NAME               0 (f)

  5           9 LOAD_CONST               1 (5)
             12 STORE_NAME               1 (age)

  6          15 LOAD_NAME                1 (age)
             18 PRINT_ITEM          
             19 PRINT_NEWLINE       

  7          20 LOAD_NAME                0 (f)
             23 LOAD_CONST               2 ('Robert')
             26 LOAD_NAME                1 (age)
             29 CALL_FUNCTION            2
             32 POP_TOP             

  8          33 LOAD_NAME                1 (age)
             36 PRINT_ITEM          
             37 PRINT_NEWLINE       
             38 LOAD_CONST               3 (None)
             41 RETURN_VALUE  

>>> co_f=co.co_consts[0]
>>> co_f.co_consts
(None, 5, '[', ', ', ']')
>>> co_f.co_names
()
>>> dis.dis(co_f)
  3           0 LOAD_FAST                1 (age)
              3 LOAD_CONST               1 (5)
              6 INPLACE_ADD         
              7 STORE_FAST               1 (age)

  4          10 LOAD_CONST               2 ('[')
             13 PRINT_ITEM          
             14 LOAD_FAST                0 (name)
             17 PRINT_ITEM          
             18 LOAD_CONST               3 (', ')
             21 PRINT_ITEM          
             22 LOAD_FAST                1 (age)
             25 PRINT_ITEM          
             26 LOAD_CONST               4 (']')
             29 PRINT_ITEM          
             30 PRINT_NEWLINE       
             31 LOAD_CONST               0 (None)
             34 RETURN_VALUE        


s="""
def f(a=1,b=2):
	print a+b
f()
f(b=3)
"""
>>> co=compile(s,"func_2.py","exec")
>>> co.co_consts
(1, 2, <code object f at 0000000002981D30, file "func_2.py", line 2>, 'b', 3, None)
>>> co.co_names
('f',)
>>> dis.dis(co)
  2           0 LOAD_CONST               0 (1)
              3 LOAD_CONST               1 (2)
              6 LOAD_CONST               2 (<code object f at 0000000002981D30, file "func_2.py", line 2>)
              9 MAKE_FUNCTION            2
             12 STORE_NAME               0 (f)

  4          15 LOAD_NAME                0 (f)
             18 CALL_FUNCTION            0
             21 POP_TOP             

  5          22 LOAD_NAME                0 (f)
             25 LOAD_CONST               3 ('b')
             28 LOAD_CONST               4 (3)
             31 CALL_FUNCTION          256
             34 POP_TOP             
             35 LOAD_CONST               5 (None)
             38 RETURN_VALUE        
>>> co_f=co.co_consts[2]
>>> co_f.co_consts
(None,)
>>> co_f.co_names
()
>>> dis.dis(co_f)
  3           0 LOAD_FAST                0 (a)
              3 LOAD_FAST                1 (b)
              6 BINARY_ADD          
              7 PRINT_ITEM          
              8 PRINT_NEWLINE       
              9 LOAD_CONST               0 (None)
             12 RETURN_VALUE        
>>> 
>>> s="""
def get_func():
	value="inner"
	def inner_func():
		return value
	return inner_func
show_value=get_func()
show_value()
"""
>>> co=compile(s,"closure.py","exec")
>>> co.co_consts
(<code object get_func at 0000000002A9DD30, file "closure.py", line 2>, None)
>>> co.co_names
('get_func', 'show_value')
>>> co_gf=co.co_consts[0]
>>> co_gf.co_consts
(None, 'inner', <code object inner_func at 0000000002A9DAB0, file "closure.py", line 4>)
>>> co_gf.co_names
()
>>> co_if=co_gf.co_consts[2]
>>> co_if.co_consts
(None,)
>>> co_if.co_names
()
>>> co_gf.co_cellvars
('value',)
>>> co_if.co_freevars
('value',)
>>> dis.dis(co)
  2           0 LOAD_CONST               0 (<code object get_func at 0000000002A9DD30, file "closure.py", line 2>)
              3 MAKE_FUNCTION            0
              6 STORE_NAME               0 (get_func)

  7           9 LOAD_NAME                0 (get_func)
             12 CALL_FUNCTION            0
             15 STORE_NAME               1 (show_value)

  8          18 LOAD_NAME                1 (show_value)
             21 CALL_FUNCTION            0
             24 POP_TOP             
             25 LOAD_CONST               1 (None)
             28 RETURN_VALUE        
>>> dis.dis(co_gf)
  3           0 LOAD_CONST               1 ('inner')
              3 STORE_DEREF              0 (value)

  4           6 LOAD_CLOSURE             0 (value)
              9 BUILD_TUPLE              1
             12 LOAD_CONST               2 (<code object inner_func at 0000000002A9DAB0, file "closure.py", line 4>)
             15 MAKE_CLOSURE             0
             18 STORE_FAST               0 (inner_func)

  6          21 LOAD_FAST                0 (inner_func)
             24 RETURN_VALUE        
>>> dis.dis(co_if)
  5           0 LOAD_DEREF               0 (value)
              3 RETURN_VALUE        

call outfunc:outfunc_co.cellvars[cname1,cname2]->outfunc_frame.cell[cell1,cell2]
exec outfunc:outfunc_fram.cell[cell1->var1,cell2-var2]->infunc_co->closure[(cell1,cell2)]
call infunc :infunc_co.closure->infunc_frame.free[cell1,cell2]->get from free

if the arg of outfunc is cellvars:(eg the Decorator out_func bellow)
call outfunc:outfunc_co.cellvars[cname1,cname2]->outfunc_frame.cell[cell1-->arg1,cell2-->arg2]

Decorator(just func=outfunc(func))
>>> s="""
def out_func(f):
	def in_func():
		print 'in_func'
		return f()
	return in_func
@out_func
def func():
	print 'func'
"""
>>> co=compile(s,"decorator.py","exec")
>>> co.co_consts
(<code object out_func at 0000000002AB1330, file "decorator.py", line 2>, <code object func at 0000000002AB1B30, file "decorator.py", line 7>, None)
>>> co_of=co.co_consts[0]
>>> co_of.co_consts
(None, <code object in_func at 0000000002AA8030, file "decorator.py", line 3>)
>>> co_if=co_of.co_consts[1]
>>> co_f=co.co_consts[1]
>>> dis.dis(co)
  2           0 LOAD_CONST               0 (<code object out_func at 0000000002AB1330, file "decorator.py", line 2>)
              3 MAKE_FUNCTION            0
              6 STORE_NAME               0 (out_func)

  7           9 LOAD_NAME                0 (out_func)
             12 LOAD_CONST               1 (<code object func at 0000000002AB1B30, file "decorator.py", line 7>)
             15 MAKE_FUNCTION            0
             18 CALL_FUNCTION            1
             21 STORE_NAME               1 (func)
             24 LOAD_CONST               2 (None)
             27 RETURN_VALUE        
>>> dis.dis(co_of)
  3           0 LOAD_CLOSURE             0 (f)
              3 BUILD_TUPLE              1
              6 LOAD_CONST               1 (<code object in_func at 0000000002AA8030, file "decorator.py", line 3>)
              9 MAKE_CLOSURE             0
             12 STORE_FAST               1 (in_func)

  6          15 LOAD_FAST                1 (in_func)
             18 RETURN_VALUE        
>>> dis.dis(co_if)
  4           0 LOAD_CONST               1 ('in_func')
              3 PRINT_ITEM          
              4 PRINT_NEWLINE       

  5           5 LOAD_DEREF               0 (f)
              8 CALL_FUNCTION            0
             11 RETURN_VALUE        
>>> dis.dis(co_f)
  9           0 LOAD_CONST               1 ('func')
              3 PRINT_ITEM          
              4 PRINT_NEWLINE       
              5 LOAD_CONST               0 (None)
              8 RETURN_VALUE        
*/

static PyObject *
call_function(PyObject ***pp_stack, int oparg
#ifdef WITH_TSC
                , uint64* pintr0, uint64* pintr1
#endif
                )
{
    int na = oparg & 0xff;
    int nk = (oparg>>8) & 0xff;
    int n = na + 2 * nk;
    PyObject **pfunc = (*pp_stack) - n - 1;
    PyObject *func = *pfunc;
    PyObject *x, *w;

    /* Always dispatch PyCFunction first, because these are
       presumed to be the most frequent callable object.
    */
    if (PyCFunction_Check(func) && nk == 0) {
        int flags = PyCFunction_GET_FLAGS(func);
        PyThreadState *tstate = PyThreadState_GET();

        PCALL(PCALL_CFUNCTION);
        if (flags & (METH_NOARGS | METH_O)) {
            PyCFunction meth = PyCFunction_GET_FUNCTION(func);
            PyObject *self = PyCFunction_GET_SELF(func);
            if (flags & METH_NOARGS && na == 0) {
                C_TRACE(x, (*meth)(self,NULL));
            }
            else if (flags & METH_O && na == 1) {
                PyObject *arg = EXT_POP(*pp_stack);
                C_TRACE(x, (*meth)(self,arg));
                Py_DECREF(arg);
            }
            else {
                err_args(func, flags, na);
                x = NULL;
            }
        }
        else {
            PyObject *callargs;
            callargs = load_args(pp_stack, na);
            READ_TIMESTAMP(*pintr0);
            C_TRACE(x, PyCFunction_Call(func,callargs,NULL));
            READ_TIMESTAMP(*pintr1);
            Py_XDECREF(callargs);
        }
    } else {
        if (PyMethod_Check(func) && PyMethod_GET_SELF(func) != NULL) {
            /* optimize access to bound methods */
            PyObject *self = PyMethod_GET_SELF(func);
            PCALL(PCALL_METHOD);
            PCALL(PCALL_BOUND_METHOD);
            Py_INCREF(self);
            func = PyMethod_GET_FUNCTION(func);
            Py_INCREF(func);
            Py_DECREF(*pfunc);
            *pfunc = self;
            na++;
            n++;
        } else
            Py_INCREF(func);
        READ_TIMESTAMP(*pintr0);
        if (PyFunction_Check(func))
            x = fast_function(func, pp_stack, n, na, nk);
        else
            x = do_call(func, pp_stack, na, nk);
        READ_TIMESTAMP(*pintr1);
        Py_DECREF(func);
    }

    /* Clear the stack of the function object.  Also removes
       the arguments in case they weren't consumed already
       (fast_function() and err_args() leave them on the stack).
     */
    while ((*pp_stack) > pfunc) {
        w = EXT_POP(*pp_stack);
        Py_DECREF(w);
        PCALL(PCALL_POP);
    }
    return x;
}

//args of func are stored at the beginning of f_localsplus
//in PyFrame_New extras = code->co_stacksize + code->co_nlocals + ncells + nfrees;

static PyObject *
fast_function(PyObject *func, PyObject ***pp_stack, int n, int na, int nk)
{
    PyCodeObject *co = (PyCodeObject *)PyFunction_GET_CODE(func);
    PyObject *globals = PyFunction_GET_GLOBALS(func);
    PyObject *argdefs = PyFunction_GET_DEFAULTS(func);
    PyObject **d = NULL;
    int nd = 0;

    PCALL(PCALL_FUNCTION);
    PCALL(PCALL_FAST_FUNCTION);
    if (argdefs == NULL && co->co_argcount == n && nk==0 &&
        co->co_flags == (CO_OPTIMIZED | CO_NEWLOCALS | CO_NOFREE)) {
        PyFrameObject *f;
        PyObject *retval = NULL;
        PyThreadState *tstate = PyThreadState_GET();
        PyObject **fastlocals, **stack;
        int i;

        PCALL(PCALL_FASTER_FUNCTION);
        assert(globals != NULL);
        /* XXX Perhaps we should create a specialized
           PyFrame_New() that doesn't take locals, but does
           take builtins without sanity checking them.
        */
        assert(tstate != NULL);
        f = PyFrame_New(tstate, co, globals, NULL);
        if (f == NULL)
            return NULL;

        fastlocals = f->f_localsplus;
        stack = (*pp_stack) - n;

        for (i = 0; i < n; i++) {
            Py_INCREF(*stack);
            fastlocals[i] = *stack++;
        }
        retval = PyEval_EvalFrameEx(f,0);
        ++tstate->recursion_depth;
        Py_DECREF(f);
        --tstate->recursion_depth;
        return retval;
    }
    if (argdefs != NULL) {
        d = &PyTuple_GET_ITEM(argdefs, 0);
        nd = Py_SIZE(argdefs);
    }
    return PyEval_EvalCodeEx(co, globals,
                             (PyObject *)NULL, (*pp_stack)-n, na,
                             (*pp_stack)-2*nk, nk, d, nd,
                             PyFunction_GET_CLOSURE(func));
}

//PyEval_EvalCodeEx begin
PyObject *
PyEval_EvalCodeEx(PyCodeObject *co, PyObject *globals, PyObject *locals,
           PyObject **args, int argcount, PyObject **kws, int kwcount,
           PyObject **defs, int defcount, PyObject *closure)
{
    register PyFrameObject *f;
    register PyObject *retval = NULL;
    register PyObject **fastlocals, **freevars;
    PyThreadState *tstate = PyThreadState_GET();
    PyObject *x, *u;

    if (globals == NULL) {
        PyErr_SetString(PyExc_SystemError,
                        "PyEval_EvalCodeEx: NULL globals");
        return NULL;
    }

    assert(tstate != NULL);
    assert(globals != NULL);
    f = PyFrame_New(tstate, co, globals, locals);
    if (f == NULL)
        return NULL;

    fastlocals = f->f_localsplus;
    freevars = f->f_localsplus + co->co_nlocals;

    if (co->co_argcount > 0 ||
        co->co_flags & (CO_VARARGS | CO_VARKEYWORDS)) {
        int i;
        int n = argcount;
        PyObject *kwdict = NULL;
        if (co->co_flags & CO_VARKEYWORDS) {
            kwdict = PyDict_New();
            if (kwdict == NULL)
                goto fail;
            i = co->co_argcount;
            if (co->co_flags & CO_VARARGS)
                i++;
            SETLOCAL(i, kwdict);
        }
        if (argcount > co->co_argcount) {
            if (!(co->co_flags & CO_VARARGS)) {
                PyErr_Format(PyExc_TypeError,
                    "%.200s() takes %s %d "
                    "argument%s (%d given)",
                    PyString_AsString(co->co_name),
                    defcount ? "at most" : "exactly",
                    co->co_argcount,
                    co->co_argcount == 1 ? "" : "s",
                    argcount + kwcount);
                goto fail;
            }
            n = co->co_argcount;
        }
        for (i = 0; i < n; i++) {
            x = args[i];
            Py_INCREF(x);
            SETLOCAL(i, x);
        }
        if (co->co_flags & CO_VARARGS) {
            u = PyTuple_New(argcount - n);
            if (u == NULL)
                goto fail;
            SETLOCAL(co->co_argcount, u);
            for (i = n; i < argcount; i++) {
                x = args[i];
                Py_INCREF(x);
                PyTuple_SET_ITEM(u, i-n, x);
            }
        }
        for (i = 0; i < kwcount; i++) {
            PyObject **co_varnames;
            PyObject *keyword = kws[2*i];
            PyObject *value = kws[2*i + 1];
            int j;
            if (keyword == NULL || !(PyString_Check(keyword)
#ifdef Py_USING_UNICODE
                                     || PyUnicode_Check(keyword)
#endif
                        )) {
                PyErr_Format(PyExc_TypeError,
                    "%.200s() keywords must be strings",
                    PyString_AsString(co->co_name));
                goto fail;
            }
            /* Speed hack: do raw pointer compares. As names are
               normally interned this should almost always hit. */
            co_varnames = ((PyTupleObject *)(co->co_varnames))->ob_item;
            for (j = 0; j < co->co_argcount; j++) {
                PyObject *nm = co_varnames[j];
                if (nm == keyword)
                    goto kw_found;
            }
            /* Slow fallback, just in case */
            for (j = 0; j < co->co_argcount; j++) {
                PyObject *nm = co_varnames[j];
                int cmp = PyObject_RichCompareBool(
                    keyword, nm, Py_EQ);
                if (cmp > 0)
                    goto kw_found;
                else if (cmp < 0)
                    goto fail;
            }
            if (kwdict == NULL) {
                PyObject *kwd_str = kwd_as_string(keyword);
                if (kwd_str) {
                    PyErr_Format(PyExc_TypeError,
                                 "%.200s() got an unexpected "
                                 "keyword argument '%.400s'",
                                 PyString_AsString(co->co_name),
                                 PyString_AsString(kwd_str));
                    Py_DECREF(kwd_str);
                }
                goto fail;
            }
            PyDict_SetItem(kwdict, keyword, value);
            continue;
          kw_found:
            if (GETLOCAL(j) != NULL) {
                PyObject *kwd_str = kwd_as_string(keyword);
                if (kwd_str) {
                    PyErr_Format(PyExc_TypeError,
                                 "%.200s() got multiple "
                                 "values for keyword "
                                 "argument '%.400s'",
                                 PyString_AsString(co->co_name),
                                 PyString_AsString(kwd_str));
                    Py_DECREF(kwd_str);
                }
                goto fail;
            }
            Py_INCREF(value);
            SETLOCAL(j, value);
        }
        if (argcount < co->co_argcount) {
            int m = co->co_argcount - defcount;
            for (i = argcount; i < m; i++) {
                if (GETLOCAL(i) == NULL) {
                    int j, given = 0;
                    for (j = 0; j < co->co_argcount; j++)
                        if (GETLOCAL(j))
                            given++;
                    PyErr_Format(PyExc_TypeError,
                        "%.200s() takes %s %d "
                        "argument%s (%d given)",
                        PyString_AsString(co->co_name),
                        ((co->co_flags & CO_VARARGS) ||
                         defcount) ? "at least"
                                   : "exactly",
                        m, m == 1 ? "" : "s", given);
                    goto fail;
                }
            }
            if (n > m)
                i = n - m;
            else
                i = 0;
            for (; i < defcount; i++) {
                if (GETLOCAL(m+i) == NULL) {
                    PyObject *def = defs[i];
                    Py_INCREF(def);
                    SETLOCAL(m+i, def);
                }
            }
        }
    }
    else if (argcount > 0 || kwcount > 0) {
        PyErr_Format(PyExc_TypeError,
                     "%.200s() takes no arguments (%d given)",
                     PyString_AsString(co->co_name),
                     argcount + kwcount);
        goto fail;
    }
    /* Allocate and initialize storage for cell vars, and copy free
       vars into frame.  This isn't too efficient right now. */
    if (PyTuple_GET_SIZE(co->co_cellvars)) {
        int i, j, nargs, found;
        char *cellname, *argname;
        PyObject *c;

        nargs = co->co_argcount;
        if (co->co_flags & CO_VARARGS)
            nargs++;
        if (co->co_flags & CO_VARKEYWORDS)
            nargs++;

        /* Initialize each cell var, taking into account
           cell vars that are initialized from arguments.

           Should arrange for the compiler to put cellvars
           that are arguments at the beginning of the cellvars
           list so that we can march over it more efficiently?
        */
        for (i = 0; i < PyTuple_GET_SIZE(co->co_cellvars); ++i) {
            cellname = PyString_AS_STRING(
                PyTuple_GET_ITEM(co->co_cellvars, i));
            found = 0;
            for (j = 0; j < nargs; j++) {
                argname = PyString_AS_STRING(
                    PyTuple_GET_ITEM(co->co_varnames, j));
                if (strcmp(cellname, argname) == 0) {
                    c = PyCell_New(GETLOCAL(j));
                    if (c == NULL)
                        goto fail;
                    GETLOCAL(co->co_nlocals + i) = c;
                    found = 1;
                    break;
                }
            }
            if (found == 0) {
                c = PyCell_New(NULL);
                if (c == NULL)
                    goto fail;
                SETLOCAL(co->co_nlocals + i, c);
            }
        }
    }
    if (PyTuple_GET_SIZE(co->co_freevars)) {
        int i;
        for (i = 0; i < PyTuple_GET_SIZE(co->co_freevars); ++i) {
            PyObject *o = PyTuple_GET_ITEM(closure, i);
            Py_INCREF(o);
            freevars[PyTuple_GET_SIZE(co->co_cellvars) + i] = o;
        }
    }

    if (co->co_flags & CO_GENERATOR) {
        /* Don't need to keep the reference to f_back, it will be set
         * when the generator is resumed. */
        Py_CLEAR(f->f_back);

        PCALL(PCALL_GENERATOR);

        /* Create a new generator that owns the ready to run frame
         * and return that as the value. */
        return PyGen_New(f);
    }

    retval = PyEval_EvalFrameEx(f,0);

fail: /* Jump here from prelude on failure */

    /* decref'ing the frame can cause __del__ methods to get invoked,
       which can call back into Python.  While we're done with the
       current Python frame (f), the associated C stack is still in use,
       so recursion_depth must be boosted for the duration.
    */
    assert(tstate != NULL);
    ++tstate->recursion_depth;
    Py_DECREF(f);
    --tstate->recursion_depth;
    return retval;
}
//end


int
PyFunction_SetDefaults(PyObject *op, PyObject *defaults)
{
    if (!PyFunction_Check(op)) {
        PyErr_BadInternalCall();
        return -1;
    }
    if (defaults == Py_None)
        defaults = NULL;
    else if (defaults && PyTuple_Check(defaults)) {
        Py_INCREF(defaults);
    }
    else {
        PyErr_SetString(PyExc_SystemError, "non-tuple default args");
        return -1;
    }
    Py_XDECREF(((PyFunctionObject *) op) -> func_defaults);
    ((PyFunctionObject *) op) -> func_defaults = defaults;
    return 0;
}

//case
case MAKE_FUNCTION:
	v = POP(); /* code object */
	x = PyFunction_New(v, f->f_globals);
	Py_DECREF(v);
	/* XXX Maybe this should be a separate opcode? */
	if (x != NULL && oparg > 0) {
		v = PyTuple_New(oparg);
		if (v == NULL) {
			Py_DECREF(x);
			x = NULL;
			break;
		}
		while (--oparg >= 0) {
			w = POP();
			PyTuple_SET_ITEM(v, oparg, w);
		}
		err = PyFunction_SetDefaults(x, v);
		Py_DECREF(v);
	}
	PUSH(x);
	break;
case CALL_FUNCTION:
{
	PyObject **sp;
	PCALL(PCALL_ALL);
	sp = stack_pointer;
#ifdef WITH_TSC
	x = call_function(&sp, oparg, &intr0, &intr1);
#else
	x = call_function(&sp, oparg);
#endif
	stack_pointer = sp;
	PUSH(x);
	if (x != NULL)
		continue;
	break;
}
case LOAD_FAST:
	x = GETLOCAL(oparg);
	if (x != NULL) {
		Py_INCREF(x);
		PUSH(x);
		goto fast_next_opcode;
	}
	format_exc_check_arg(PyExc_UnboundLocalError,
		UNBOUNDLOCAL_ERROR_MSG,
		PyTuple_GetItem(co->co_varnames, oparg));
	break;
PREDICTED_WITH_ARG(STORE_FAST);
case STORE_FAST:
	v = POP();
	SETLOCAL(oparg, v);
	goto fast_next_opcode;
case STORE_DEREF:
	w = POP();
	x = freevars[oparg];
	PyCell_Set(x, w);
	Py_DECREF(w);
	continue;
case LOAD_CLOSURE:
	x = freevars[oparg];
	Py_INCREF(x);
	PUSH(x);
	if (x != NULL) continue;
	break;
case MAKE_CLOSURE:
{
	v = POP(); /* code object */
	x = PyFunction_New(v, f->f_globals);
	Py_DECREF(v);
	if (x != NULL) {
		v = POP();
		if (PyFunction_SetClosure(x, v) != 0) {
			/* Can't happen unless bytecode is corrupt. */
			why = WHY_EXCEPTION;
		}
		Py_DECREF(v);
	}
	if (x != NULL && oparg > 0) {
		v = PyTuple_New(oparg);
		if (v == NULL) {
			Py_DECREF(x);
			x = NULL;
			break;
		}
		while (--oparg >= 0) {
			w = POP();
			PyTuple_SET_ITEM(v, oparg, w);
		}
		if (PyFunction_SetDefaults(x, v) != 0) {
			/* Can't happen unless
			   PyFunction_SetDefaults changes. */
			why = WHY_EXCEPTION;
		}
		Py_DECREF(v);
	}
	PUSH(x);
	break;
}
case LOAD_DEREF:
	x = freevars[oparg];
	w = PyCell_Get(x);
	if (w != NULL) {
		PUSH(w);
		continue;
	}
	err = -1;
	/* Don't stomp existing exception */
	if (PyErr_Occurred())
		break;
	if (oparg < PyTuple_GET_SIZE(co->co_cellvars)) {
		v = PyTuple_GET_ITEM(co->co_cellvars,
							 oparg);
		format_exc_check_arg(
			   PyExc_UnboundLocalError,
			   UNBOUNDLOCAL_ERROR_MSG,
			   v);
	} else {
		v = PyTuple_GET_ITEM(co->co_freevars, oparg -
			PyTuple_GET_SIZE(co->co_cellvars));
		format_exc_check_arg(PyExc_NameError,
							 UNBOUNDFREE_ERROR_MSG, v);
	}
	break;

