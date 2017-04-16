//python源码解析笔记
/*
 summarize============================

static PyMethodDef thread_methods[] = {
{"start_new_thread",(PyCFunction)thread_PyThread_start_new_thread, METH_VARARGS, start_new_doc},
...
thread_PyThread_start_new_thread(PyObject *self, PyObject *fargs)
1.PyEval_InitThreads
//func is t_bootstrap
//arg is a bootstate with func&args&new_tstate
2.PyThread_start_new_thread(void (*func)(void *), void *arg)
	//obj is a callobj with func&arg 
	CreateThread(NULL,Py_SAFE_DOWNCAST(_pythread_stacksize, Py_ssize_t, SIZE_T),bootstrap, obj, 0, &threadID);
in t_bootstrap
	//all tstate in link with pid and key(autoTLSkey) in keyhead
	//autoTLSkey:Thread Local Store,all the thread's key is same(autoTLSkey)
    _PyThreadState_Init(tstate);-->PyThread_set_key_value
    PyEval_AcquireThread(tstate);
    nb_threads++;
    PyEval_CallObjectWithKeywords(boot->func, boot->args, boot->keyw);

schedule
1.standard 
in PyEval_EvalFrameEx(PyFrameObject *f, int throwflag)
_Py_Ticker = _Py_CheckInterval;//(100)
2.block
eg:time.sleep-->time_sleep->floatsleep in timemodule.c
Py_BEGIN_ALLOW_THREADS-----------Py_END_ALLOW_THREADS
3.lock for user(GIL is just for python's env)
typedef struct {
    PyObject_HEAD
    PyThread_type_lock lock_lock;
    PyObject *in_weakreflist;
} lockobject;
4.threading.py
*/


//>>>>>>>>>>>>>> thread state list
//in thread.c
//
/* A singly-linked list of struct key objects remembers all the key->value
 * associations.  File static keyhead heads the list.  keymutex is used
 * to enforce exclusion internally.
 */
struct key {
    /* Next record in the list, or NULL if this is the last record. */
    struct key *next;

    /* The thread id, according to PyThread_get_thread_ident(). */
    long id;

    /* The key and its associated value. */
    int key;
    void *value;
};
static struct key *keyhead = NULL;
static PyThread_type_lock keymutex = NULL;
static int nkeys = 0;  /* PyThread_create_key() hands out nkeys+1 next */

void *
PyThread_get_key_value(int key)
{
    struct key *p = find_key(key, NULL);

    if (p == NULL)
        return NULL;
    else
        return p->value;
}

int
PyThread_set_key_value(int key, void *value)
{
    struct key *p;

    assert(value != NULL);
    p = find_key(key, value);
    if (p == NULL)
        return -1;
    else
        return 0;
}

static struct key *
find_key(int key, void *value)
{
    struct key *p, *prev_p;
    long id = PyThread_get_thread_ident();

    if (!keymutex)
        return NULL;
    PyThread_acquire_lock(keymutex, 1);
    prev_p = NULL;
    for (p = keyhead; p != NULL; p = p->next) {
        if (p->id == id && p->key == key)
            goto Done;
        /* Sanity check.  These states should never happen but if
         * they do we must abort.  Otherwise we'll end up spinning in
         * in a tight loop with the lock held.  A similar check is done
         * in pystate.c tstate_delete_common().  */
        if (p == prev_p)
            Py_FatalError("tls find_key: small circular list(!)");
        prev_p = p;
        if (p->next == keyhead)
            Py_FatalError("tls find_key: circular list(!)");
    }
    if (value == NULL) {
        assert(p == NULL);
        goto Done;
    }
    p = (struct key *)malloc(sizeof(struct key));
    if (p != NULL) {
        p->id = id;
        p->key = key;
        p->value = value;
        p->next = keyhead;
        keyhead = p;
    }
 Done:
    PyThread_release_lock(keymutex);
    return p;
}

//in pystate.c
/* The single PyInterpreterState used by this process'
   GILState implementation
*/
static PyInterpreterState *autoInterpreterState = NULL;
static int autoTLSkey = 0;
/* Internal initialization/finalization functions called by
   Py_Initialize/Py_Finalize
*/
void
_PyGILState_Init(PyInterpreterState *i, PyThreadState *t)
{
    assert(i && t); /* must init with valid states */
    autoTLSkey = PyThread_create_key();
    autoInterpreterState = i;
    assert(PyThread_get_key_value(autoTLSkey) == NULL);
    assert(t->gilstate_counter == 0);

    _PyGILState_NoteThreadState(t);
}
static void
_PyGILState_NoteThreadState(PyThreadState* tstate)
{
    /* If autoTLSkey isn't initialized, this must be the very first
       threadstate created in Py_Initialize().  Don't do anything for now
       (we'll be back here when _PyGILState_Init is called). */
    if (!autoInterpreterState)
        return;

    /* Stick the thread state for this thread in thread local storage.

       The only situation where you can legitimately have more than one
       thread state for an OS level thread is when there are multiple
       interpreters, when:

           a) You shouldn't really be using the PyGILState_ APIs anyway,
          and:

           b) The slightly odd way PyThread_set_key_value works (see
          comments by its implementation) means that the first thread
          state created for that given OS level thread will "win",
          which seems reasonable behaviour.
    */
    if (PyThread_set_key_value(autoTLSkey, (void *)tstate) < 0)
        Py_FatalError("Couldn't create autoTLSkey mapping");

    /* PyGILState_Release must not try to delete this thread state. */
    tstate->gilstate_counter = 1;
}

/* Return a new key.  This must be called before any other functions in
 * this family, and callers must arrange to serialize calls to this
 * function.  No violations are detected.
 */
int
PyThread_create_key(void)
{
    /* All parts of this function are wrong if it's called by multiple
     * threads simultaneously.
     */
    if (keymutex == NULL)
        keymutex = PyThread_allocate_lock();
    return ++nkeys;
}

//>>>>>>>>>>>>>>>> thread state list

//in pystate.h
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

//in threadmodule.c
static PyMethodDef thread_methods[] = {
    {"start_new_thread",        (PyCFunction)thread_PyThread_start_new_thread,
                            METH_VARARGS,
                            start_new_doc},
    {"start_new",               (PyCFunction)thread_PyThread_start_new_thread,
                            METH_VARARGS,
                            start_new_doc},
    {"allocate_lock",           (PyCFunction)thread_PyThread_allocate_lock,
     METH_NOARGS, allocate_doc},
    {"allocate",                (PyCFunction)thread_PyThread_allocate_lock,
     METH_NOARGS, allocate_doc},
    {"exit_thread",             (PyCFunction)thread_PyThread_exit_thread,
     METH_NOARGS, exit_doc},
    {"exit",                    (PyCFunction)thread_PyThread_exit_thread,
     METH_NOARGS, exit_doc},
    {"interrupt_main",          (PyCFunction)thread_PyThread_interrupt_main,
     METH_NOARGS, interrupt_doc},
    {"get_ident",               (PyCFunction)thread_get_ident,
     METH_NOARGS, get_ident_doc},
    {"_count",                  (PyCFunction)thread__count,
     METH_NOARGS, _count_doc},
    {"stack_size",              (PyCFunction)thread_stack_size,
                            METH_VARARGS,
                            stack_size_doc},
    {NULL,                      NULL}           /* sentinel */
};

static PyObject *
thread_PyThread_start_new_thread(PyObject *self, PyObject *fargs)
{
    PyObject *func, *args, *keyw = NULL;
    struct bootstate *boot;
    long ident;

    if (!PyArg_UnpackTuple(fargs, "start_new_thread", 2, 3,
                           &func, &args, &keyw))
        return NULL;
    if (!PyCallable_Check(func)) {
        PyErr_SetString(PyExc_TypeError,
                        "first arg must be callable");
        return NULL;
    }
    if (!PyTuple_Check(args)) {
        PyErr_SetString(PyExc_TypeError,
                        "2nd arg must be a tuple");
        return NULL;
    }
    if (keyw != NULL && !PyDict_Check(keyw)) {
        PyErr_SetString(PyExc_TypeError,
                        "optional 3rd arg must be a dictionary");
        return NULL;
    }
    boot = PyMem_NEW(struct bootstate, 1);
    if (boot == NULL)
        return PyErr_NoMemory();
    boot->interp = PyThreadState_GET()->interp;
    boot->func = func;
    boot->args = args;
    boot->keyw = keyw;
    boot->tstate = _PyThreadState_Prealloc(boot->interp);
    if (boot->tstate == NULL) {
        PyMem_DEL(boot);
        return PyErr_NoMemory();
    }
    Py_INCREF(func);
    Py_INCREF(args);
    Py_XINCREF(keyw);
    PyEval_InitThreads(); /* Start the interpreter's thread-awareness */
    ident = PyThread_start_new_thread(t_bootstrap, (void*) boot);
    if (ident == -1) {
        PyErr_SetString(ThreadError, "can't start new thread");
        Py_DECREF(func);
        Py_DECREF(args);
        Py_XDECREF(keyw);
        PyThreadState_Clear(boot->tstate);
        PyMem_DEL(boot);
        return NULL;
    }
    return PyInt_FromLong(ident);
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

//>>>>>>>>>>>>>>>>PyEval_InitThreads
typedef void *PyThread_type_lock;
static PyThread_type_lock interpreter_lock = 0; /* This is the GIL */
void
PyEval_InitThreads(void)
{
    if (interpreter_lock)
        return;
    interpreter_lock = PyThread_allocate_lock();
    PyThread_acquire_lock(interpreter_lock, 1);
    main_thread = PyThread_get_thread_ident();
}
//in thread_nt.h
PyThread_type_lock
PyThread_allocate_lock(void)
{
    PNRMUTEX aLock;

    dprintf(("PyThread_allocate_lock called\n"));
    if (!initialized)
        PyThread_init_thread();

    aLock = AllocNonRecursiveMutex() ;

    dprintf(("%ld: PyThread_allocate_lock() -> %p\n", PyThread_get_thread_ident(), aLock));

    return (PyThread_type_lock) aLock;
}
void
PyThread_init_thread(void)
{
#ifdef Py_DEBUG
    char *p = Py_GETENV("PYTHONTHREADDEBUG");

    if (p) {
        if (*p)
            thread_debug = atoi(p);
        else
            thread_debug = 1;
    }
#endif /* Py_DEBUG */
    if (initialized)
        return;
    initialized = 1;
    dprintf(("PyThread_init_thread called\n"));
    PyThread__init_thread();
}
/*
 * Initialization of the C package, should not be needed.
 */
static void
PyThread__init_thread(void)
{
}
typedef struct NRMUTEX {
    LONG   owned ;
    DWORD  thread_id ;
    HANDLE hevent ;
} NRMUTEX, *PNRMUTEX ;

PNRMUTEX
AllocNonRecursiveMutex(void)
{
    PNRMUTEX mutex = (PNRMUTEX)malloc(sizeof(NRMUTEX)) ;
    if (mutex && !InitializeNonRecursiveMutex(mutex))
    {
        free(mutex) ;
        mutex = NULL ;
    }
    return mutex ;
}
int
PyThread_acquire_lock(PyThread_type_lock aLock, int waitflag)
{
    int success ;

    dprintf(("%ld: PyThread_acquire_lock(%p, %d) called\n", PyThread_get_thread_ident(),aLock, waitflag));

    success = aLock && EnterNonRecursiveMutex((PNRMUTEX) aLock, (waitflag ? INFINITE : 0)) == WAIT_OBJECT_0 ;

    dprintf(("%ld: PyThread_acquire_lock(%p, %d) -> %d\n", PyThread_get_thread_ident(),aLock, waitflag, success));

    return success;
}
/*
InterlockedCompareExchange是把目标操作数（第1参数所指向的内存中的数）
与一个值（第3参数）比较，如果相等，则用另一个值（第2参数）与目标操作数
（第1参数所指向的内存中的数）交换；InterlockedExchange是不比较直接交换。
整个操作过程是锁定内存的，其它处理器不会同时访问内存，从而实现多处理器环境下的线程互斥。
InterlockedIncrement实现数的原子性加减 返回值是输入参数增加后的值
 */ DWORD
EnterNonRecursiveMutex(PNRMUTEX mutex, BOOL wait)
{
    /* Assume that the thread waits successfully */
    DWORD ret ;

    /* InterlockedIncrement(&mutex->owned) == 0 means that no thread currently owns the mutex */
    if (!wait)
    {
        if (InterlockedCompareExchange(&mutex->owned, 0, -1) != -1)
            return WAIT_TIMEOUT ;
        ret = WAIT_OBJECT_0 ;
    }
    else
        ret = InterlockedIncrement(&mutex->owned) ?
            /* Some thread owns the mutex, let's wait... */
            WaitForSingleObject(mutex->hevent, INFINITE) : WAIT_OBJECT_0 ;

    mutex->thread_id = GetCurrentThreadId() ; /* We own it */
    return ret ;
}
void
PyThread_release_lock(PyThread_type_lock aLock)
{
    dprintf(("%ld: PyThread_release_lock(%p) called\n", PyThread_get_thread_ident(),aLock));

    if (!(aLock && LeaveNonRecursiveMutex((PNRMUTEX) aLock)))
        dprintf(("%ld: Could not PyThread_release_lock(%p) error: %ld\n", PyThread_get_thread_ident(), aLock, GetLastError()));
}
BOOL
LeaveNonRecursiveMutex(PNRMUTEX mutex)
{
    /* We don't own the mutex */
    mutex->thread_id = 0 ;
    return
        InterlockedDecrement(&mutex->owned) < 0 ||
        SetEvent(mutex->hevent) ; /* Other threads are waiting, wake one on them up */
}

//<<<<<<<<<<<<<<PyEval_InitThreads
//>>>>>>>>>>>>>>PyThread_start_new_thread
long
PyThread_start_new_thread(void (*func)(void *), void *arg)
{
    HANDLE hThread;
    unsigned threadID;
    callobj *obj;

    dprintf(("%ld: PyThread_start_new_thread called\n",
             PyThread_get_thread_ident()));
    if (!initialized)
        PyThread_init_thread();

    obj = (callobj*)HeapAlloc(GetProcessHeap(), 0, sizeof(*obj));
    if (!obj)
        return -1;
    obj->func = func;
    obj->arg = arg;
#if defined(MS_WINCE)
    hThread = CreateThread(NULL,
                           Py_SAFE_DOWNCAST(_pythread_stacksize, Py_ssize_t, SIZE_T),
                           bootstrap, obj, 0, &threadID);
#else
    hThread = (HANDLE)_beginthreadex(0,
                      Py_SAFE_DOWNCAST(_pythread_stacksize,
                                       Py_ssize_t, unsigned int),
                      bootstrap, obj,
                      0, &threadID);
#endif
    if (hThread == 0) {
#if defined(MS_WINCE)
        /* Save error in variable, to prevent PyThread_get_thread_ident
           from clobbering it. */
        unsigned e = GetLastError();
        dprintf(("%ld: PyThread_start_new_thread failed, win32 error code %u\n",
                 PyThread_get_thread_ident(), e));
#else
        /* I've seen errno == EAGAIN here, which means "there are
         * too many threads".
         */
        int e = errno;
        dprintf(("%ld: PyThread_start_new_thread failed, errno %d\n",
                 PyThread_get_thread_ident(), e));
#endif
        threadID = (unsigned)-1;
        HeapFree(GetProcessHeap(), 0, obj);
    }
    else {
        dprintf(("%ld: PyThread_start_new_thread succeeded: %p\n",
                 PyThread_get_thread_ident(), (void*)hThread));
        CloseHandle(hThread);
    }
    return (long) threadID;
}

/*
 * Thread support.
 */

typedef struct {
    void (*func)(void*);
    void *arg;
} callobj;
bootstrap(void *call)
{
    callobj *obj = (callobj*)call;
    void (*func)(void*) = obj->func;
    void *arg = obj->arg;
    HeapFree(GetProcessHeap(), 0, obj);
    func(arg);
    return 0;
}
static void
t_bootstrap(void *boot_raw)
{
    struct bootstate *boot = (struct bootstate *) boot_raw;
    PyThreadState *tstate;
    PyObject *res;

    tstate = boot->tstate;
    tstate->thread_id = PyThread_get_thread_ident();
    _PyThreadState_Init(tstate);
    PyEval_AcquireThread(tstate);
    nb_threads++;
    res = PyEval_CallObjectWithKeywords(
        boot->func, boot->args, boot->keyw);
    if (res == NULL) {
        if (PyErr_ExceptionMatches(PyExc_SystemExit))
            PyErr_Clear();
        else {
            PyObject *file;
            PyObject *exc, *value, *tb;
            PyErr_Fetch(&exc, &value, &tb);
            PySys_WriteStderr(
                "Unhandled exception in thread started by ");
            file = PySys_GetObject("stderr");
            if (file)
                PyFile_WriteObject(boot->func, file, 0);
            else
                PyObject_Print(boot->func, stderr, 0);
            PySys_WriteStderr("\n");
            PyErr_Restore(exc, value, tb);
            PyErr_PrintEx(0);
        }
    }
    else
        Py_DECREF(res);
    Py_DECREF(boot->func);
    Py_DECREF(boot->args);
    Py_XDECREF(boot->keyw);
    PyMem_DEL(boot_raw);
    nb_threads--;
    PyThreadState_Clear(tstate);
    PyThreadState_DeleteCurrent();
    PyThread_exit_thread();
}
void
PyEval_AcquireThread(PyThreadState *tstate)
{
    if (tstate == NULL)
        Py_FatalError("PyEval_AcquireThread: NULL new thread state");
    /* Check someone has called PyEval_InitThreads() to create the lock */
    assert(interpreter_lock);
    PyThread_acquire_lock(interpreter_lock, 1);
    if (PyThreadState_Swap(tstate) != NULL)
        Py_FatalError(
            "PyEval_AcquireThread: non-NULL old thread state");
}

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


//schedule

//1.standard
PyObject *
PyEval_EvalFrameEx(PyFrameObject *f, int throwflag)
{
...
        if (--_Py_Ticker < 0) {
            if (*next_instr == SETUP_FINALLY) {
                /* Make the last opcode before
                   a try: finally: block uninterruptible. */
                goto fast_next_opcode;
            }
            _Py_Ticker = _Py_CheckInterval;
            tstate->tick_counter++;
#ifdef WITH_TSC
            ticked = 1;
#endif
            if (pendingcalls_to_do) {
                if (Py_MakePendingCalls() < 0) {
                    why = WHY_EXCEPTION;
                    goto on_error;
                }
                if (pendingcalls_to_do)
                    /* MakePendingCalls() didn't succeed.
                       Force early re-execution of this
                       "periodic" code, possibly after
                       a thread switch */
                    _Py_Ticker = 0;
            }
#ifdef WITH_THREAD
            if (interpreter_lock) {
                /* Give another thread a chance */

                if (PyThreadState_Swap(NULL) != tstate)
                    Py_FatalError("ceval: tstate mix-up");
                PyThread_release_lock(interpreter_lock);

                /* Other threads may run now */

                PyThread_acquire_lock(interpreter_lock, 1);

                if (PyThreadState_Swap(tstate) != NULL)
                    Py_FatalError("ceval: orphan tstate");

                /* Check for thread interrupts */

                if (tstate->async_exc != NULL) {
                    x = tstate->async_exc;
                    tstate->async_exc = NULL;
                    PyErr_SetNone(x);
                    Py_DECREF(x);
                    why = WHY_EXCEPTION;
                    goto on_error;
                }
            }
...
//2block
//time.sleep-->time_sleep->floatsleep in timemodule.c
static int
floatsleep(double secs)
{
	...
    Py_BEGIN_ALLOW_THREADS
	...
    Py_END_ALLOW_THREADS
	...
}
//in ceval.h
#define Py_BEGIN_ALLOW_THREADS { \
                        PyThreadState *_save; \
                        _save = PyEval_SaveThread();

#define Py_END_ALLOW_THREADS    PyEval_RestoreThread(_save); \

PyThreadState *
PyEval_SaveThread(void)
{
    PyThreadState *tstate = PyThreadState_Swap(NULL);
    if (tstate == NULL)
        Py_FatalError("PyEval_SaveThread: NULL tstate");
#ifdef WITH_THREAD
    if (interpreter_lock)
        PyThread_release_lock(interpreter_lock);
#endif
    return tstate;
}

void
PyEval_RestoreThread(PyThreadState *tstate)
{
    if (tstate == NULL)
        Py_FatalError("PyEval_RestoreThread: NULL tstate");
#ifdef WITH_THREAD
    if (interpreter_lock) {
        int err = errno;
        PyThread_acquire_lock(interpreter_lock, 1);
        errno = err;
    }
#endif
    PyThreadState_Swap(tstate);
}

//lock for user
static PyObject *
thread_PyThread_allocate_lock(PyObject *self)
{
    return (PyObject *) newlockobject();
}
static lockobject *
newlockobject(void)
{
    lockobject *self;
    self = PyObject_New(lockobject, &Locktype);
    if (self == NULL)
        return NULL;
    self->lock_lock = PyThread_allocate_lock();
    self->in_weakreflist = NULL;
    if (self->lock_lock == NULL) {
        Py_DECREF(self);
        PyErr_SetString(ThreadError, "can't allocate lock");
        return NULL;
    }
    return self;
}
typedef struct {
    PyObject_HEAD
    PyThread_type_lock lock_lock;
    PyObject *in_weakreflist;
} lockobject;

typedef void *PyThread_type_lock;
//methods of lock type
static PyMethodDef lock_methods[] = {
    {"acquire_lock", (PyCFunction)lock_PyThread_acquire_lock,
     METH_VARARGS, acquire_doc},
    {"acquire",      (PyCFunction)lock_PyThread_acquire_lock,
     METH_VARARGS, acquire_doc},
    {"release_lock", (PyCFunction)lock_PyThread_release_lock,
     METH_NOARGS, release_doc},
    {"release",      (PyCFunction)lock_PyThread_release_lock,
     METH_NOARGS, release_doc},
    {"locked_lock",  (PyCFunction)lock_locked_lock,
     METH_NOARGS, locked_doc},
    {"locked",       (PyCFunction)lock_locked_lock,
     METH_NOARGS, locked_doc},
    {"__enter__",    (PyCFunction)lock_PyThread_acquire_lock,
     METH_VARARGS, acquire_doc},
    {"__exit__",    (PyCFunction)lock_PyThread_release_lock,
     METH_VARARGS, release_doc},
    {NULL}              /* sentinel */
};
static PyObject *
lock_PyThread_acquire_lock(lockobject *self, PyObject *args)
{
    int i = 1;

    if (!PyArg_ParseTuple(args, "|i:acquire", &i))
        return NULL;

    Py_BEGIN_ALLOW_THREADS
    i = PyThread_acquire_lock(self->lock_lock, i);
    Py_END_ALLOW_THREADS

    return PyBool_FromLong((long)i);
}
static PyObject *
lock_PyThread_release_lock(lockobject *self)
{
    /* Sanity check: the lock must be locked */
    if (PyThread_acquire_lock(self->lock_lock, 0)) {
        PyThread_release_lock(self->lock_lock);
        PyErr_SetString(ThreadError, "release unlocked lock");
        return NULL;
    }

    PyThread_release_lock(self->lock_lock);
    Py_INCREF(Py_None);
    return Py_None;
}

//threading in threading.py
_start_new_thread = thread.start_new_thread
_allocate_lock = thread.allocate_lock
_get_ident = thread.get_ident
ThreadError = thread.error
del thread

# Active thread administration
_active_limbo_lock = _allocate_lock()
_active = {}    # maps thread id to Thread object
_limbo = {}

def enumerate():
    """Return a list of all Thread objects currently alive.

    The list includes daemonic threads, dummy thread objects created by
    current_thread(), and the main thread. It excludes terminated threads and
    threads that have not yet been started.

    """
    with _active_limbo_lock:
        return _active.values() + _limbo.values()

# Synchronization classes
Lock = _allocate_lock
Lock,RLock,Conditon,Semaphore,Event,Thread

