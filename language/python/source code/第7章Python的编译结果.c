/*
7.2.3

1.py-->pyc(PyCodeObject)-->virtual-machine
2.CodeBlock:when enter a new namespace,we enter a new CodeBlock|namespace<-->CodeBlock<-->PyCodeObject
3.PyCodeObject
	�����е�һ��Code Block(�����ռ��ࡢ������module)���ᴴ��һ��PyCodeObject����
	co_argcount 	Code Block ��λ�ò����ĸ���������˵һ��������λ�ò���������λ�ò��� ��μ��� 11 �¶Ժ������Ƶ�������
	co_nlocals 		Code Block �оֲ������ĸ�����������λ�ò����ĸ���
	co_stacksize 	ִ�иö� Code Block ��Ҫ��ջ�ռ�
	co_flags		N/A
	co_code			Code Block �������õ��ֽ���ָ�����С��� PyStringObject ����ʽ����
	co_consts		PyTupleObject ���󣬱��� Code Block �е����г���(Ƕ�׵�PyCodeObject��������)
	co_names		PyTupleObject ���󣬱��� Code Block �е����з���
	co_varnames		Code Block �еľֲ�����������
	co_freevars		Python ʵ�ֱհ���Ҫ�õ��Ķ�����������½��л��漰
	co_cellvars		Code Block ���ڲ�Ƕ�׺��������õľֲ�����������
	co_filename		Code Block ����Ӧ��.py �ļ�������·��
	co_name			Code Block �����֣�ͨ���Ǻ�����������
	co_firstlineno 	Code Block �ڶ�Ӧ��.py �ļ��е���ʼ��
	co_lnotab 		�ֽ���ָ����.py �ļ��� source code �кŵĶ�Ӧ��ϵ����PyStringObject����ʽ����
					(�ֽ���ָ���� co_code ��λ�ã�source code �к�)
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
 1.pyc������������ python a.py�Ͳ���
 2.import a ������a.dll ���� a.pyc û�оͻ��ȱ���a.py->a.pyc
 */

//pyc����
/*
�����ظ��ַ�����д�� TYPE_INTERNED TYPE_STRINGREF
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

