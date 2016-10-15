//python源码解析笔记
//in main.c
int
Py_Main(int argc, char **argv)
{
	...
    Py_Initialize();
	...
	PyRun_AnyFileExFlags(fp,filename==NULL?"<stdin>":filename,filename != NULL, &cf)
	...
}

int
PyRun_AnyFileExFlags(FILE *fp, const char *filename, int closeit,
                     PyCompilerFlags *flags)
{
    if (filename == NULL)
        filename = "???";
    if (Py_FdIsInteractive(fp, filename)) {
        int err = PyRun_InteractiveLoopFlags(fp, filename, flags);
        if (closeit)
            fclose(fp);
        return err;
    }
    else
        return PyRun_SimpleFileExFlags(fp, filename, closeit, flags);
}

//Interactive
int
PyRun_InteractiveLoopFlags(FILE *fp, const char *filename, PyCompilerFlags *flags)
{
    PyObject *v;
    int ret;
    PyCompilerFlags local_flags;

    if (flags == NULL) {
        flags = &local_flags;
        local_flags.cf_flags = 0;
    }
    v = PySys_GetObject("ps1");
    if (v == NULL) {
        PySys_SetObject("ps1", v = PyString_FromString(">>> "));
        Py_XDECREF(v);
    }
    v = PySys_GetObject("ps2");
    if (v == NULL) {
        PySys_SetObject("ps2", v = PyString_FromString("... "));
        Py_XDECREF(v);
    }
    for (;;) {
        ret = PyRun_InteractiveOneFlags(fp, filename, flags);
        PRINT_TOTAL_REFS();
        if (ret == E_EOF)
            return 0;
        /*
        if (ret == E_NOMEM)
            return -1;
        */
    }
}

int
PyRun_InteractiveOneFlags(FILE *fp, const char *filename, PyCompilerFlags *flags)
{
    PyObject *m, *d, *v, *w;
    mod_ty mod;
    PyArena *arena;
    char *ps1 = "", *ps2 = "";
    int errcode = 0;

    v = PySys_GetObject("ps1");
    if (v != NULL) {
        v = PyObject_Str(v);
        if (v == NULL)
            PyErr_Clear();
        else if (PyString_Check(v))
            ps1 = PyString_AsString(v);
    }
    w = PySys_GetObject("ps2");
    if (w != NULL) {
        w = PyObject_Str(w);
        if (w == NULL)
            PyErr_Clear();
        else if (PyString_Check(w))
            ps2 = PyString_AsString(w);
    }
    arena = PyArena_New();
    if (arena == NULL) {
        Py_XDECREF(v);
        Py_XDECREF(w);
        return -1;
    }
    mod = PyParser_ASTFromFile(fp, filename,
                               Py_single_input, ps1, ps2,
                               flags, &errcode, arena);
    Py_XDECREF(v);
    Py_XDECREF(w);
    if (mod == NULL) {
        PyArena_Free(arena);
        if (errcode == E_EOF) {
            PyErr_Clear();
            return E_EOF;
        }
        PyErr_Print();
        return -1;
    }
    m = PyImport_AddModule("__main__");
    if (m == NULL) {
        PyArena_Free(arena);
        return -1;
    }
    d = PyModule_GetDict(m);
    v = run_mod(mod, filename, d, d, flags, arena);
    PyArena_Free(arena);
    if (v == NULL) {
        PyErr_Print();
        return -1;
    }
    Py_DECREF(v);
    if (Py_FlushLine())
        PyErr_Clear();
    return 0;
}
//SimpleFile
int
PyRun_SimpleFileExFlags(FILE *fp, const char *filename, int closeit,
                        PyCompilerFlags *flags)
{
    PyObject *m, *d, *v;
    const char *ext;
    int set_file_name = 0, len, ret = -1;

    m = PyImport_AddModule("__main__");
    if (m == NULL)
        return -1;
    Py_INCREF(m);
    d = PyModule_GetDict(m);
    if (PyDict_GetItemString(d, "__file__") == NULL) {
        PyObject *f = PyString_FromString(filename);
        if (f == NULL)
            goto done;
        if (PyDict_SetItemString(d, "__file__", f) < 0) {
            Py_DECREF(f);
            goto done;
        }
        set_file_name = 1;
        Py_DECREF(f);
    }
    len = strlen(filename);
    ext = filename + len - (len > 4 ? 4 : 0);
    if (maybe_pyc_file(fp, filename, ext, closeit)) {
        /* Try to run a pyc file. First, re-open in binary */
        if (closeit)
            fclose(fp);
        if ((fp = fopen(filename, "rb")) == NULL) {
            fprintf(stderr, "python: Can't reopen .pyc file\n");
            goto done;
        }
        /* Turn on optimization if a .pyo file is given */
        if (strcmp(ext, ".pyo") == 0)
            Py_OptimizeFlag = 1;
        v = run_pyc_file(fp, filename, d, d, flags);
    } else {
        v = PyRun_FileExFlags(fp, filename, Py_file_input, d, d,
                              closeit, flags);
    }
    if (v == NULL) {
        PyErr_Print();
        goto done;
    }
    Py_DECREF(v);
    if (Py_FlushLine())
        PyErr_Clear();
    ret = 0;
  done:
    if (set_file_name && PyDict_DelItemString(d, "__file__"))
        PyErr_Clear();
    Py_DECREF(m);
    return ret;
}
PyObject *
PyRun_FileExFlags(FILE *fp, const char *filename, int start, PyObject *globals,
                  PyObject *locals, int closeit, PyCompilerFlags *flags)
{
    PyObject *ret;
    mod_ty mod;
    PyArena *arena = PyArena_New();
    if (arena == NULL)
        return NULL;

    mod = PyParser_ASTFromFile(fp, filename, start, 0, 0,
                               flags, NULL, arena);
    if (closeit)
        fclose(fp);
    if (mod == NULL) {
        PyArena_Free(arena);
        return NULL;
    }
    ret = run_mod(mod, filename, globals, locals, flags, arena);
    PyArena_Free(arena);
    return ret;
}
//together
static PyObject *
run_mod(mod_ty mod, const char *filename, PyObject *globals, PyObject *locals,
         PyCompilerFlags *flags, PyArena *arena)
{
    PyCodeObject *co;
    PyObject *v;
    co = PyAST_Compile(mod, filename, flags, arena);
    if (co == NULL)
        return NULL;
    v = PyEval_EvalCode(co, globals, locals);
    Py_DECREF(co);
    return v;
}

PyObject *
PyEval_EvalCode(PyCodeObject *co, PyObject *globals, PyObject *locals)
{
    return PyEval_EvalCodeEx(co,
                      globals, locals,
                      (PyObject **)NULL, 0,
                      (PyObject **)NULL, 0,
                      (PyObject **)NULL, 0,
                      NULL);
}

PyObject *
PyEval_EvalCodeEx(PyCodeObject *co, PyObject *globals, PyObject *locals,
           PyObject **args, int argcount, PyObject **kws, int kwcount,
           PyObject **defs, int defcount, PyObject *closure)

//name zone
//builtins = PyDict_GetItem(globals, builtin_object);
//f->f_globals = globals;  dict of __main__
//f->f_locals = locals;
PyFrameObject *
PyFrame_New(PyThreadState *tstate, PyCodeObject *code, PyObject *globals,
            PyObject *locals)
{
    PyFrameObject *back = tstate->frame;
    PyFrameObject *f;
    PyObject *builtins;
    Py_ssize_t i;

#ifdef Py_DEBUG
    if (code == NULL || globals == NULL || !PyDict_Check(globals) ||
        (locals != NULL && !PyMapping_Check(locals))) {
        PyErr_BadInternalCall();
        return NULL;
    }
#endif
    if (back == NULL || back->f_globals != globals) {
        builtins = PyDict_GetItem(globals, builtin_object);
        if (builtins) {
            if (PyModule_Check(builtins)) {
                builtins = PyModule_GetDict(builtins);
                assert(!builtins || PyDict_Check(builtins));
            }
            else if (!PyDict_Check(builtins))
                builtins = NULL;
        }
        if (builtins == NULL) {
            /* No builtins!              Make up a minimal one
               Give them 'None', at least. */
            builtins = PyDict_New();
            if (builtins == NULL ||
                PyDict_SetItemString(
                    builtins, "None", Py_None) < 0)
                return NULL;
        }
        else
            Py_INCREF(builtins);

    }
    else {
        /* If we share the globals, we share the builtins.
           Save a lookup and a call. */
        builtins = back->f_builtins;
        assert(builtins != NULL && PyDict_Check(builtins));
        Py_INCREF(builtins);
    }
    if (code->co_zombieframe != NULL) {
        f = code->co_zombieframe;
        code->co_zombieframe = NULL;
        _Py_NewReference((PyObject *)f);
        assert(f->f_code == code);
    }
    else {
        Py_ssize_t extras, ncells, nfrees;
        ncells = PyTuple_GET_SIZE(code->co_cellvars);
        nfrees = PyTuple_GET_SIZE(code->co_freevars);
        extras = code->co_stacksize + code->co_nlocals + ncells +
            nfrees;
        if (free_list == NULL) {
            f = PyObject_GC_NewVar(PyFrameObject, &PyFrame_Type,
            extras);
            if (f == NULL) {
                Py_DECREF(builtins);
                return NULL;
            }
        }
        else {
            assert(numfree > 0);
            --numfree;
            f = free_list;
            free_list = free_list->f_back;
            if (Py_SIZE(f) < extras) {
                f = PyObject_GC_Resize(PyFrameObject, f, extras);
                if (f == NULL) {
                    Py_DECREF(builtins);
                    return NULL;
                }
            }
            _Py_NewReference((PyObject *)f);
        }

        f->f_code = code;
        extras = code->co_nlocals + ncells + nfrees;
        f->f_valuestack = f->f_localsplus + extras;
        for (i=0; i<extras; i++)
            f->f_localsplus[i] = NULL;
        f->f_locals = NULL;
        f->f_trace = NULL;
        f->f_exc_type = f->f_exc_value = f->f_exc_traceback = NULL;
    }
    f->f_stacktop = f->f_valuestack;
    f->f_builtins = builtins;
    Py_XINCREF(back);
    f->f_back = back;
    Py_INCREF(code);
    Py_INCREF(globals);
    f->f_globals = globals;
    /* Most functions have CO_NEWLOCALS and CO_OPTIMIZED set. */
    if ((code->co_flags & (CO_NEWLOCALS | CO_OPTIMIZED)) ==
        (CO_NEWLOCALS | CO_OPTIMIZED))
        ; /* f_locals = NULL; will be set by PyFrame_FastToLocals() */
    else if (code->co_flags & CO_NEWLOCALS) {
        locals = PyDict_New();
        if (locals == NULL) {
            Py_DECREF(f);
            return NULL;
        }
        f->f_locals = locals;
    }
    else {
        if (locals == NULL)
            locals = globals;
        Py_INCREF(locals);
        f->f_locals = locals;
    }
    f->f_tstate = tstate;

    f->f_lasti = -1;
    f->f_lineno = code->co_firstlineno;
    f->f_iblock = 0;

    _PyObject_GC_TRACK(f);
    return f;
}

//in pythonrun.c
void
Py_Initialize(void)
{
    Py_InitializeEx(1);
}

void
Py_InitializeEx(int install_sigs)
{
    interp = PyInterpreterState_New();
    tstate = PyThreadState_New(interp);
    _Py_ReadyTypes();
	//设置全局变量builtin_object：__builtins__
    if (!_PyFrame_Init())
        Py_FatalError("Py_Initialize: can't init frames");
    if (!_PyLong_Init())
        Py_FatalError("Py_Initialize: can't init longs");
    if (!PyByteArray_Init())
        Py_FatalError("Py_Initialize: can't init bytearray");
    _PyFloat_Init();
	//模块字典
    interp->modules = PyDict_New();
    interp->modules_reloading = PyDict_New();
	//__builtins__模块
    bimod = _PyBuiltin_Init();
    interp->builtins = PyModule_GetDict(bimod);
	//sys模块
    sysmod = _PySys_Init();
    interp->sysdict = PyModule_GetDict(sysmod);
	//设置搜索路径
    PySys_SetPath(Py_GetPath());
	//设置sys.modules
    PyDict_SetItemString(interp->sysdict, "modules", interp->modules);
	//初始化import机制
    _PyImport_Init();
	//备份
    _PyImport_FixupExtension("exceptions", "exceptions");
    _PyImport_FixupExtension("__builtin__", "__builtin__");
	//用于import机制
    _PyImportHooks_Init();
	//__main__
    initmain(); /* Module __main__ */
	//lib\\site-package第三方库位置
    initsite(); /* Module site */
}

//process
typedef struct _is {

    struct _is *next;
    struct _ts *tstate_head;

    PyObject *modules;
    PyObject *sysdict;
    PyObject *builtins;
    PyObject *modules_reloading;

    PyObject *codec_search_path;
    PyObject *codec_search_cache;
    PyObject *codec_error_registry;

#ifdef HAVE_DLOPEN
    int dlopenflags;
#endif
#ifdef WITH_TSC
    int tscdump;
#endif

} PyInterpreterState;

//thread
typedef struct _ts {
    /* See Python/ceval.c for comments explaining most fields */

    struct _ts *next;
    PyInterpreterState *interp;

    struct _frame *frame;
    int recursion_depth;
    /* 'tracing' keeps track of the execution depth when tracing/profiling.
       This is to prevent the actual trace/profile code from being recorded in
       the trace/profile. */
    int tracing;
    int use_tracing;

    Py_tracefunc c_profilefunc;
    Py_tracefunc c_tracefunc;
    PyObject *c_profileobj;
    PyObject *c_traceobj;

    PyObject *curexc_type;
    PyObject *curexc_value;
    PyObject *curexc_traceback;

    PyObject *exc_type;
    PyObject *exc_value;
    PyObject *exc_traceback;

    PyObject *dict;  /* Stores per-thread state */

    /* tick_counter is incremented whenever the check_interval ticker
     * reaches zero. The purpose is to give a useful measure of the number
     * of interpreted bytecode instructions in a given thread.  This
     * extremely lightweight statistic collector may be of interest to
     * profilers (like psyco.jit()), although nothing in the core uses it.
     */
    int tick_counter;

    int gilstate_counter;

    PyObject *async_exc; /* Asynchronous exception to raise */
    long thread_id; /* Thread id where this tstate was created */

    int trash_delete_nesting;
    PyObject *trash_delete_later;

    /* XXX signal handlers should also be here */

} PyThreadState;

//create process 
PyInterpreterState *
PyInterpreterState_New(void)
{
    PyInterpreterState *interp = (PyInterpreterState *)
                                 malloc(sizeof(PyInterpreterState));

    if (interp != NULL) {
        HEAD_INIT();
#ifdef WITH_THREAD
        if (head_mutex == NULL)
            Py_FatalError("Can't initialize threads for interpreter");
#endif
        interp->modules = NULL;
        interp->modules_reloading = NULL;
        interp->sysdict = NULL;
        interp->builtins = NULL;
        interp->tstate_head = NULL;
        interp->codec_search_path = NULL;
        interp->codec_search_cache = NULL;
        interp->codec_error_registry = NULL;
#ifdef HAVE_DLOPEN
#ifdef RTLD_NOW
        interp->dlopenflags = RTLD_NOW;
#else
        interp->dlopenflags = RTLD_LAZY;
#endif
#endif
#ifdef WITH_TSC
        interp->tscdump = 0;
#endif

        HEAD_LOCK();
        interp->next = interp_head;
        interp_head = interp;
        HEAD_UNLOCK();
    }

    return interp;
}
//new thread
PyThreadState *
PyThreadState_New(PyInterpreterState *interp)
{
    return new_threadstate(interp, 1);
}

PyThreadState *
_PyThreadState_Prealloc(PyInterpreterState *interp)
{
    return new_threadstate(interp, 0);
}

static PyThreadState *
new_threadstate(PyInterpreterState *interp, int init)
{
    PyThreadState *tstate = (PyThreadState *)malloc(sizeof(PyThreadState));

    if (_PyThreadState_GetFrame == NULL)
        _PyThreadState_GetFrame = threadstate_getframe;

    if (tstate != NULL) {
        tstate->interp = interp;

        tstate->frame = NULL;
        tstate->recursion_depth = 0;
        tstate->tracing = 0;
        tstate->use_tracing = 0;
        tstate->tick_counter = 0;
        tstate->gilstate_counter = 0;
        tstate->async_exc = NULL;
#ifdef WITH_THREAD
        tstate->thread_id = PyThread_get_thread_ident();
#else
        tstate->thread_id = 0;
#endif

        tstate->dict = NULL;

        tstate->curexc_type = NULL;
        tstate->curexc_value = NULL;
        tstate->curexc_traceback = NULL;

        tstate->exc_type = NULL;
        tstate->exc_value = NULL;
        tstate->exc_traceback = NULL;

        tstate->c_profilefunc = NULL;
        tstate->c_tracefunc = NULL;
        tstate->c_profileobj = NULL;
        tstate->c_traceobj = NULL;

        tstate->trash_delete_nesting = 0;
        tstate->trash_delete_later = NULL;

        if (init)
            _PyThreadState_Init(tstate);

        HEAD_LOCK();
        tstate->next = interp->tstate_head;
        interp->tstate_head = tstate;
        HEAD_UNLOCK();
    }

    return tstate;
}
//swap
PyThreadState *
PyThreadState_Swap(PyThreadState *newts)
{
    PyThreadState *oldts = _PyThreadState_Current;

    _PyThreadState_Current = newts;
    /* It should not be possible for more than one thread state
       to be used for a thread.  Check this the best we can in debug
       builds.
    */
#if defined(Py_DEBUG) && defined(WITH_THREAD)
    if (newts) {
        /* This can be called from PyEval_RestoreThread(). Similar
           to it, we need to ensure errno doesn't change.
        */
        int err = errno;
        PyThreadState *check = PyGILState_GetThisThreadState();
        if (check && check->interp == newts->interp && check != newts)
            Py_FatalError("Invalid thread state for this thread");
        errno = err;
    }
#endif
    return oldts;
}


//mod
//in module.c
//if not find,add a new one into _PyThreadState_Current->interp->modules
PyObject *
PyImport_AddModule(const char *name)
{
    PyObject *modules = PyImport_GetModuleDict();
    PyObject *m;

    if ((m = PyDict_GetItemString(modules, name)) != NULL &&
        PyModule_Check(m))
        return m;
    m = PyModule_New(name);
    if (m == NULL)
        return NULL;
    if (PyDict_SetItemString(modules, name, m) != 0) {
        Py_DECREF(m);
        return NULL;
    }
    Py_DECREF(m); /* Yes, it still exists, in modules! */

    return m;
}
PyObject *
PyImport_GetModuleDict(void)
{
    PyInterpreterState *interp = PyThreadState_GET()->interp;
    if (interp->modules == NULL)
        Py_FatalError("PyImport_GetModuleDict: no module dictionary!");
    return interp->modules;
}
#define PyThreadState_GET() (_PyThreadState_Current)
typedef struct {
    PyObject_HEAD
    PyObject *md_dict;
} PyModuleObject;
PyObject *
PyModule_New(const char *name)
{
    PyModuleObject *m;
    PyObject *nameobj;
    m = PyObject_GC_New(PyModuleObject, &PyModule_Type);
    if (m == NULL)
        return NULL;
    nameobj = PyString_FromString(name);
    m->md_dict = PyDict_New();
    if (m->md_dict == NULL || nameobj == NULL)
        goto fail;
    if (PyDict_SetItemString(m->md_dict, "__name__", nameobj) != 0)
        goto fail;
    if (PyDict_SetItemString(m->md_dict, "__doc__", Py_None) != 0)
        goto fail;
    if (PyDict_SetItemString(m->md_dict, "__package__", Py_None) != 0)
        goto fail;
    Py_DECREF(nameobj);
    PyObject_GC_Track(m);
    return (PyObject *)m;

 fail:
    Py_XDECREF(nameobj);
    Py_DECREF(m);
    return NULL;
}

