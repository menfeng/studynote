//python源码解析笔记
/*
PyFrameObject is created in PyFrame_New
	PyFrame_New(PyThreadState *tstate, PyCodeObject *code, PyObject *globals, PyObject *locals)
	ncells = PyTuple_GET_SIZE(code->co_cellvars);
	nfrees = PyTuple_GET_SIZE(code->co_freevars);
	extras = code->co_stacksize + code->co_nlocals + ncells + nfrees;		//动态内存区四部分组成
	f = PyObject_GC_Resize(PyFrameObject, f, extras);
	extras = code->co_nlocals + ncells + nfrees;
	f->f_valuestack = f->f_localsplus + extras;								//栈底
	f->f_stacktop = f->f_valuestack;										//栈顶

get frame in python
	sys._get_frame()

虚拟机框架
	python start-->运行时环境-->PyEval_EvalFrameEx(PyFrameObject *f, int throwflag)
	PyEval_EvalFrameEx会执行f中的 co_code

	PyEval_EvalFrameEx设置线程的frame
	PyThreadState *tstate = PyThreadState_GET();
	tstate->frame = f;

	PyFrame_New中会连接frame
	PyFrameObject *back = tstate->frame;
	f = PyObject_GC_Resize(PyFrameObject, f, extras);
	f->f_back = back;
	f->f_tstate = tstate;

	i_state->i_state->
	|
	t_state->t_state->
	|
	frame->f_back-f_back
 */
//PyFrameObject
typedef struct _frame {
    PyObject_VAR_HEAD
    struct _frame *f_back;	/* previous frame, or NULL */
    PyCodeObject *f_code;	/* code segment */
    PyObject *f_builtins;	/* builtin symbol table (PyDictObject) */
    PyObject *f_globals;	/* global symbol table (PyDictObject) */
    PyObject *f_locals;		/* local symbol table (any mapping) */
    PyObject **f_valuestack;	/* points after the last local */
    /* Next free slot in f_valuestack.  Frame creation sets to f_valuestack.
       Frame evaluation usually NULLs it, but a frame that yields sets it
       to the current stack top. */
    PyObject **f_stacktop;
    PyObject *f_trace;		/* Trace function */

    /* If an exception is raised in this frame, the next three are used to
     * record the exception info (if any) originally in the thread state.  See
     * comments before set_exc_info() -- it's not obvious.
     * Invariant:  if _type is NULL, then so are _value and _traceback.
     * Desired invariant:  all three are NULL, or all three are non-NULL.  That
     * one isn't currently true, but "should be".
     */
    PyObject *f_exc_type, *f_exc_value, *f_exc_traceback;

    PyThreadState *f_tstate;
    int f_lasti;		/* Last instruction if called */
    /* Call PyFrame_GetLineNumber() instead of reading this field
       directly.  As of 2.3 f_lineno is only valid when tracing is
       active (i.e. when f_trace is set).  At other times we use
       PyCode_Addr2Line to calculate the line from the current
       bytecode index. */
    int f_lineno;		/* Current line number */
    int f_iblock;		/* index in f_blockstack */
    PyTryBlock f_blockstack[CO_MAXBLOCKS]; /* for try and loop blocks */
    PyObject *f_localsplus[1];	/* locals+stack, dynamically sized */
} PyFrameObject;


//进程PyInterpreterState
typedef struct _is {

    struct _is *next;
    struct _ts *tstate_head; //模拟进程中线程**********

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

//线程PyThreadState
typedef struct _ts {
    /* See Python/ceval.c for comments explaining most fields */

    struct _ts *next;
    PyInterpreterState *interp;

    struct _frame *frame;//模拟进程函数调用堆栈**********
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


