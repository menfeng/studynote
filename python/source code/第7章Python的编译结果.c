/*
7.2.3

1.py-->pyc(PyCodeObject)-->virtual-machine
2.CodeBlock:when enter a new namespace,we enter a new CodeBlock|namespace<-->CodeBlock<-->PyCodeObject
3.PyCodeObject
	代码中的一个Code Block(命名空间类、函数、module)，会创建一个PyCodeObject对象
	co_argcount 	Code Block 的位置参数的个数，比如说一个函数的位置参数个数（位置参数 请参见第 11 章对函数机制的剖析）
	co_nlocals 		Code Block 中局部变量的个数，包括其位置参数的个数
	co_stacksize 	执行该段 Code Block 需要的栈空间
	co_flags		N/A
	co_code			Code Block 编译所得的字节码指令序列。以 PyStringObject 的形式存在
	co_consts		PyTupleObject 对象，保存 Code Block 中的所有常量(嵌套的PyCodeObject存在这里)
	co_names		PyTupleObject 对象，保存 Code Block 中的所有符号
	co_varnames		Code Block 中的局部变量名集合
	co_freevars		Python 实现闭包需要用到的东西，后面的章节中会涉及
	co_cellvars		Code Block 中内部嵌套函数所引用的局部变量名集合
	co_filename		Code Block 所对应的.py 文件的完整路径
	co_name			Code Block 的名字，通常是函数名或类名
	co_firstlineno 	Code Block 在对应的.py 文件中的起始行
	co_lnotab 		字节码指令与.py 文件中 source code 行号的对应关系，以PyStringObject的形式存在
					(字节码指令在 co_code 中位置，source code 行号)
*/
typedef struct {
    PyObject_HEAD
    int co_argcount;		/* #arguments, except *args */
    int co_nlocals;		/* #local variables */
    int co_stacksize;		/* #entries needed for evaluation stack */
    int co_flags;		/* CO_..., see below */
    PyObject *co_code;		/* instruction opcodes */
    PyObject *co_consts;	/* list (constants used) */
    PyObject *co_names;		/* list of strings (names used) */
    PyObject *co_varnames;	/* tuple of strings (local variable names) */
    PyObject *co_freevars;	/* tuple of strings (free variable names) */
    PyObject *co_cellvars;      /* tuple of strings (cell variable names) */
    /* The rest doesn't count for hash/cmp */
    PyObject *co_filename;	/* string (where it was loaded from) */
    PyObject *co_name;		/* string (name, for reference) */
    int co_firstlineno;		/* first source line number */
    PyObject *co_lnotab;	/* string (encoding addr<->lineno mapping) See
				   Objects/lnotab_notes.txt for details. */
    void *co_zombieframe;     /* for optimization only (see frameobject.c) */
    PyObject *co_weakreflist;   /* to support weakrefs to code objects */
} PyCodeObject;

//pyc
/*
 1.pyc并不总是生成 python a.py就不会
 2.import a 会先找a.dll 或者 a.pyc 没有就会先编译a.py->a.pyc
 */

//pyc生成
/*
避免重复字符串的写入 TYPE_INTERNED TYPE_STRINGREF
 */
static void
write_compiled_module(PyCodeObject *co, char *cpathname, struct stat *srcstat, time_t mtime)
{
    FILE *fp;
#ifdef MS_WINDOWS   /* since Windows uses different permissions  */
    mode_t mode = srcstat->st_mode & ~S_IEXEC;
    /* Issue #6074: We ensure user write access, so we can delete it later
     * when the source file changes. (On POSIX, this only requires write
     * access to the directory, on Windows, we need write access to the file
     * as well)
     */
    mode |= _S_IWRITE;
#else
    mode_t mode = srcstat->st_mode & ~S_IXUSR & ~S_IXGRP & ~S_IXOTH;
#endif

    fp = open_exclusive(cpathname, mode);
    if (fp == NULL) {
        if (Py_VerboseFlag)
            PySys_WriteStderr(
                "# can't create %s\n", cpathname);
        return;
    }
    PyMarshal_WriteLongToFile(pyc_magic, fp, Py_MARSHAL_VERSION);
    /* First write a 0 for mtime */
    PyMarshal_WriteLongToFile(0L, fp, Py_MARSHAL_VERSION);
    PyMarshal_WriteObjectToFile((PyObject *)co, fp, Py_MARSHAL_VERSION);
    if (fflush(fp) != 0 || ferror(fp)) {
        if (Py_VerboseFlag)
            PySys_WriteStderr("# can't write %s\n", cpathname);
        /* Don't keep partial file */
        fclose(fp);
        (void) unlink(cpathname);
        return;
    }
    /* Now write the true mtime (as a 32-bit field) */
    fseek(fp, 4L, 0);
    assert(mtime <= 0xFFFFFFFF);
    PyMarshal_WriteLongToFile((long)mtime, fp, Py_MARSHAL_VERSION);
    fflush(fp);
    fclose(fp);
    if (Py_VerboseFlag)
        PySys_WriteStderr("# wrote %s\n", cpathname);
}

void
PyMarshal_WriteObjectToFile(PyObject *x, FILE *fp, int version)
{
    WFILE wf;
    wf.fp = fp;
    wf.error = WFERR_OK;
    wf.depth = 0;
    wf.strings = (version > 0) ? PyDict_New() : NULL;
    wf.version = version;
    w_object(x, &wf);
    Py_XDECREF(wf.strings);
}

PyObject *
PyMarshal_ReadObjectFromFile(FILE *fp)
{
    RFILE rf;
    PyObject *result;
    rf.fp = fp;
    rf.strings = PyList_New(0);
    rf.depth = 0;
    rf.ptr = rf.end = NULL;
    result = r_object(&rf);
    Py_DECREF(rf.strings);
    return result;
}

//python bytecode (opcode.h)
#define HAS_ARG(op) ((op) >= HAVE_ARGUMENT)

