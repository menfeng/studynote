//pythonÔ´Âë½âÎö±Ê¼Ç
/*
pool
(1)small int
#define NSMALLPOSINTS           257
#define NSMALLNEGINTS           5
//small_ints [-5,257)
static PyIntObject *small_ints[NSMALLNEGINTS + NSMALLPOSINTS];

(2)large int
#define BLOCK_SIZE      1000     1K less typical malloc overhead 
#define BHEAD_SIZE      8        Enough for a 64-bit pointer 
#define N_INTOBJECTS    ((BLOCK_SIZE - BHEAD_SIZE) / sizeof(PyIntObject))=(1000-8)/12=82
struct _intblock {
    struct _intblock *next;
    PyIntObject objects[N_INTOBJECTS];
};
typedef struct _intblock PyIntBlock;
static PyIntBlock *block_list = NULL;
static PyIntObject *free_list = NULL;(link by PY_TYPE(p))

(3)_PyInt_Init
	init small_inits when python init
	small int also be managed by block_list

(4)PyInt_ClearFreeList:
	python never give the memory of block_list back to system untill the 
	clear_freelists  be called during the collection of the highest generation.
*/

//struct
typedef struct {
    PyObject_HEAD
    long ob_ival;
} PyIntObject;

//type
PyTypeObject PyInt_Type = {
    PyVarObject_HEAD_INIT(&PyType_Type, 0)
    "int",
    sizeof(PyIntObject),
    0,
    (destructor)int_dealloc,                    /* tp_dealloc */
    (printfunc)int_print,                       /* tp_print */
    0,                                          /* tp_getattr */
    0,                                          /* tp_setattr */
    (cmpfunc)int_compare,                       /* tp_compare */
    (reprfunc)int_to_decimal_string,            /* tp_repr */
    &int_as_number,                             /* tp_as_number */
    0,                                          /* tp_as_sequence */
    0,                                          /* tp_as_mapping */
    (hashfunc)int_hash,                         /* tp_hash */
    0,                                          /* tp_call */
    (reprfunc)int_to_decimal_string,            /* tp_str */
    PyObject_GenericGetAttr,                    /* tp_getattro */
    0,                                          /* tp_setattro */
    0,                                          /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_CHECKTYPES |
        Py_TPFLAGS_BASETYPE | Py_TPFLAGS_INT_SUBCLASS,          /* tp_flags */
    int_doc,                                    /* tp_doc */
    0,                                          /* tp_traverse */
    0,                                          /* tp_clear */
    0,                                          /* tp_richcompare */
    0,                                          /* tp_weaklistoffset */
    0,                                          /* tp_iter */
    0,                                          /* tp_iternext */
    int_methods,                                /* tp_methods */
    0,                                          /* tp_members */
    int_getset,                                 /* tp_getset */
    0,                                          /* tp_base */
    0,                                          /* tp_dict */
    0,                                          /* tp_descr_get */
    0,                                          /* tp_descr_set */
    0,                                          /* tp_dictoffset */
    0,                                          /* tp_init */
    0,                                          /* tp_alloc */
    int_new,                                    /* tp_new */
    (freefunc)int_free,                         /* tp_free */
};

//doc
//in python.h
/* Define macroh for inline documentation. */
#define PyDoc_VAR(name) static char name[]
#define PyDoc_STRVAR(name,str) PyDoc_VAR(name) = PyDoc_STR(str)
#ifdef WITH_DOC_STRINGS
#define PyDoc_STR(str) str
#else
#define PyDoc_STR(str) ""
#endif
//intobject.c
PyDoc_STRVAR(int_doc,
"int(x=0) -> int or long\n\
int(x, base=10) -> int or long\n\
\n\
Convert a number or string to an integer, or return 0 if no arguments\n\
are given.  If x is floating point, the conversion truncates towards zero.\n\
If x is outside the integer range, the function returns a long instead.\n\
\n\
If x is not a number or if base is given, then x must be a string or\n\
Unicode object representing an integer literal in the given base.  The\n\
literal can be preceded by '+' or '-' and be surrounded by whitespace.\n\
The base defaults to 10.  Valid bases are 0 and 2-36.  Base 0 means to\n\
interpret the base from the string as an integer literal.\n\
>>> int('0b100', base=0)\n\
4");


//add
#define PyInt_AS_LONG(op) (((PyIntObject *)(op))->ob_ival)
#define CONVERT_TO_LONG(obj, lng)               \
    if (PyInt_Check(obj)) {                     \
        lng = PyInt_AS_LONG(obj);               \
    }                                           \
    else {                                      \
        Py_INCREF(Py_NotImplemented);           \
        return Py_NotImplemented;               \
    }

static PyObject *
int_add(PyIntObject *v, PyIntObject *w)
{
    register long a, b, x;
    CONVERT_TO_LONG(v, a);
    CONVERT_TO_LONG(w, b);
    /* casts in the line below avoid undefined behaviour on overflow */
    x = (long)((unsigned long)a + b);
    if ((x^a) >= 0 || (x^b) >= 0)
        return PyInt_FromLong(x);
    return PyLong_Type.tp_as_number->nb_add((PyObject *)v, (PyObject *)w);
}

//small_ints [-5,257)
#ifndef NSMALLPOSINTS
#define NSMALLPOSINTS           257
#endif
#ifndef NSMALLNEGINTS
#define NSMALLNEGINTS           5
#endif
#if NSMALLNEGINTS + NSMALLPOSINTS > 0
/* References to small integers are saved in this array so that they
   can be shared.
   The integers that are saved are those in the range
   -NSMALLNEGINTS (inclusive) to NSMALLPOSINTS (not inclusive).
*/
static PyIntObject *small_ints[NSMALLNEGINTS + NSMALLPOSINTS];
#endif

//int block
#define BLOCK_SIZE      1000    /* 1K less typical malloc overhead */
#define BHEAD_SIZE      8       /* Enough for a 64-bit pointer */
#define N_INTOBJECTS    ((BLOCK_SIZE - BHEAD_SIZE) / sizeof(PyIntObject))

struct _intblock {
    struct _intblock *next;
    PyIntObject objects[N_INTOBJECTS];
};

typedef struct _intblock PyIntBlock;

static PyIntBlock *block_list = NULL;
static PyIntObject *free_list = NULL;

//use type as next
static PyIntObject *
fill_free_list(void)
{
    PyIntObject *p, *q;
    /* Python's object allocator isn't appropriate for large blocks. */
    p = (PyIntObject *) PyMem_MALLOC(sizeof(PyIntBlock));
    if (p == NULL)
        return (PyIntObject *) PyErr_NoMemory();
    ((PyIntBlock *)p)->next = block_list;
    block_list = (PyIntBlock *)p;
    /* Link the int objects together, from rear to front, then return
       the address of the last int object in the block. */
    p = &((PyIntBlock *)p)->objects[0];
    q = p + N_INTOBJECTS;
    while (--q > p)
        Py_TYPE(q) = (struct _typeobject *)(q-1);
    Py_TYPE(q) = NULL;
    return p + N_INTOBJECTS - 1;

//create PyInt_FromLong
PyObject *
PyInt_FromLong(long ival)
{
    register PyIntObject *v;
#if NSMALLNEGINTS + NSMALLPOSINTS > 0
    if (-NSMALLNEGINTS <= ival && ival < NSMALLPOSINTS) {
        v = small_ints[ival + NSMALLNEGINTS];
        Py_INCREF(v);
#ifdef COUNT_ALLOCS
        if (ival >= 0)
            quick_int_allocs++;
        else
            quick_neg_int_allocs++;
#endif
        return (PyObject *) v;
    }
#endif
    if (free_list == NULL) {
        if ((free_list = fill_free_list()) == NULL)
            return NULL;
    }
    /* Inline PyObject_New */
    v = free_list;
    free_list = (PyIntObject *)Py_TYPE(v);
    PyObject_INIT(v, &PyInt_Type);
    v->ob_ival = ival;
    return (PyObject *) v;
}

//dealloc
static void
int_dealloc(PyIntObject *v)
{
    if (PyInt_CheckExact(v)) {
        Py_TYPE(v) = (struct _typeobject *)free_list;
        free_list = v;
    }
    else
        Py_TYPE(v)->tp_free((PyObject *)v);
}

int
_PyInt_Init(void)
{
    PyIntObject *v;
    int ival;
#if NSMALLNEGINTS + NSMALLPOSINTS > 0
    for (ival = -NSMALLNEGINTS; ival < NSMALLPOSINTS; ival++) {
          if (!free_list && (free_list = fill_free_list()) == NULL)
                    return 0;
        /* PyObject_New is inlined */
        v = free_list;
        free_list = (PyIntObject *)Py_TYPE(v);
        PyObject_INIT(v, &PyInt_Type);
        v->ob_ival = ival;
        small_ints[ival + NSMALLNEGINTS] = v;
    }
#endif
    return 1;
}

