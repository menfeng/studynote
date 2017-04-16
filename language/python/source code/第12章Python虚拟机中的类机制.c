//python源码解析笔记
/*
what's the size to malloc?
PyType_GenericAlloc(PyTypeObject *type, Py_ssize_t nitems)
#define _PyObject_VAR_SIZE(typeobj, nitems)     \
    (size_t)                                    \
    ( ( (typeobj)->tp_basicsize +               \
        (nitems)*(typeobj)->tp_itemsize +       \
        (SIZEOF_VOID_P - 1)                     \
      ) & ~(SIZEOF_VOID_P - 1)                  \
    )

1.PyType_Ready
	1.tp_base
		if tp_base==0
			base=tp_base=PyBaseObject_Type
	2.PyType_Ready(base)
	3.ob_type(metaclass)
		if ob_type==0
			ob_type=base.ob_type
	4.bases
		if base==0
			bases=[]
		else
			bases=[base]
	2.tp_dict
	3.mro
	4.inheri method 
	5.subclasses

create class or instance:
	class.__class__.__call__
		instance=class.__new__(class,args,kwds)
		class.__init__(instance,args,kwds)
2.how to create a class
	1.call code get metadata
	2.build_class
		1.use user __metaclass__ or bases[0].__class__		:bases==[object]
		2.metaclass.ob_type.tp_call
			type_call(metaclass,arg,kwds)
				obj=metaclass.tp_new(meta,arg,kwds)
					type_new(meta,arg,kwds)
						metaclass.tp_alloc
							PyBaseObject_Type.PyType_GenericAlloc
								PyObject_GC_Malloc|PyObject_MALLOC(sizeof(PyHeapTypeObject)+sizeof(PyMemberDef))
						tp_basicsize=base.tp_basicsize+8(dict+weaklist)=24
						tp_itemsize=base.tp_itemsize
						PyType_Ready
				metaclass->tp_init

3.how to create a instance?
		2.A.ob_type.tp_call
			type_call(A,arg,kwds)
				obj=A.tp_new(A,arg,kwds)
					object_new(A,arg,kwds)
						A.tp_alloc
							PyBaseObject_Type.PyType_GenericAlloc
								PyObject_GC_Malloc|PyObject_MALLOC(A.tp_basicsize+A.itemsize)
				A->tp_init



in build_class
result = PyObject_CallFunctionObjArgs(metaclass, name, bases, methods, NULL);
 */
/*
tp_dict={"__getitem__":PyWrapperDescrObject}
PyWrapperDescrObject.d_base->slotdef
PyWrapperDescrObject.d_wrapped->func
*/

//slot
typeobject.h
/* The *real* layout of a type object when allocated on the heap */
typedef struct _heaptypeobject {
    /* Note: there's a dependency on the order of these members
       in slotptr() in typeobject.c . */
    PyTypeObject ht_type;
    PyNumberMethods as_number;
    PyMappingMethods as_mapping;
    PySequenceMethods as_sequence; /* as_sequence comes after as_mapping,
                                      so that the mapping wins when both
                                      the mapping and the sequence define
                                      a given operator (e.g. __getitem__).
                                      see add_operators() in typeobject.c . */
    PyBufferProcs as_buffer;
    PyObject *ht_name, *ht_slots;
    /* here are optional user slots, followed by the members. */
} PyHeapTypeObject;

typedef struct wrapperbase slotdef;

//in descrobject.h --> 
struct wrapperbase {
    char *name;
    int offset;
    void *function;
    wrapperfunc wrapper;
    char *doc;
    int flags;
    PyObject *name_strobj;
};

#define PyDescr_COMMON \
    PyObject_HEAD \
    PyTypeObject *d_type; \
    PyObject *d_name

typedef struct {
    PyDescr_COMMON;
    struct wrapperbase *d_base;
    void *d_wrapped; /* This can be any function pointer */
} PyWrapperDescrObject;

static PyDescrObject *
descr_new(PyTypeObject *descrtype, PyTypeObject *type, const char *name)
{
    PyDescrObject *descr;

    descr = (PyDescrObject *)PyType_GenericAlloc(descrtype, 0);
    if (descr != NULL) {
        Py_XINCREF(type);
        descr->d_type = type;
        descr->d_name = PyString_InternFromString(name);
        if (descr->d_name == NULL) {
            Py_DECREF(descr);
            descr = NULL;
        }
    }
    return descr;
}

PyObject *
PyDescr_NewWrapper(PyTypeObject *type, struct wrapperbase *base, void *wrapped)
{
    PyWrapperDescrObject *descr;

    descr = (PyWrapperDescrObject *)descr_new(&PyWrapperDescr_Type,
                                             type, base->name);
    if (descr != NULL) {
        descr->d_base = base;
        descr->d_wrapped = wrapped;
    }
    return (PyObject *)descr;
}

//in descrobject.h <-- 


//in typeobject.c --> 
static int
add_operators(PyTypeObject *type)
{
    PyObject *dict = type->tp_dict;
    slotdef *p;
    PyObject *descr;
    void **ptr;

    init_slotdefs();
    for (p = slotdefs; p->name; p++) {
        if (p->wrapper == NULL)
            continue;
        ptr = slotptr(type, p->offset);
        if (!ptr || !*ptr)
            continue;
        if (PyDict_GetItem(dict, p->name_strobj))
            continue;
        if (*ptr == PyObject_HashNotImplemented) {
            /* Classes may prevent the inheritance of the tp_hash
               slot by storing PyObject_HashNotImplemented in it. Make it
               visible as a None value for the __hash__ attribute. */
            if (PyDict_SetItem(dict, p->name_strobj, Py_None) < 0)
                return -1;
        }
        else {
            descr = PyDescr_NewWrapper(type, p, *ptr);
            if (descr == NULL)
                return -1;
            if (PyDict_SetItem(dict, p->name_strobj, descr) < 0)
                return -1;
            Py_DECREF(descr);
        }
    }
    if (type->tp_new != NULL) {
        if (add_tp_new_wrapper(type) < 0)
            return -1;
    }
    return 0;
}

static void **
slotptr(PyTypeObject *type, int ioffset)
{
    char *ptr;
    long offset = ioffset;

    /* Note: this depends on the order of the members of PyHeapTypeObject! */
    assert(offset >= 0);
    assert((size_t)offset < offsetof(PyHeapTypeObject, as_buffer));
    if ((size_t)offset >= offsetof(PyHeapTypeObject, as_sequence)) {
        ptr = (char *)type->tp_as_sequence;
        offset -= offsetof(PyHeapTypeObject, as_sequence);
    }
    else if ((size_t)offset >= offsetof(PyHeapTypeObject, as_mapping)) {
        ptr = (char *)type->tp_as_mapping;
        offset -= offsetof(PyHeapTypeObject, as_mapping);
    }
    else if ((size_t)offset >= offsetof(PyHeapTypeObject, as_number)) {
        ptr = (char *)type->tp_as_number;
        offset -= offsetof(PyHeapTypeObject, as_number);
    }
    else {
        ptr = (char *)type;
    }
    if (ptr != NULL)
        ptr += offset;
    return (void **)ptr;
}

//PyType_Ready
int
PyType_Ready(PyTypeObject *type)
{
    PyObject *dict, *bases;
    PyTypeObject *base;
    Py_ssize_t i, n;

    if (type->tp_flags & Py_TPFLAGS_READY) {
        assert(type->tp_dict != NULL);
        return 0;
    }
    assert((type->tp_flags & Py_TPFLAGS_READYING) == 0);

    type->tp_flags |= Py_TPFLAGS_READYING;

#ifdef Py_TRACE_REFS
    /* PyType_Ready is the closest thing we have to a choke point
     * for type objects, so is the best place I can think of to try
     * to get type objects into the doubly-linked list of all objects.
     * Still, not all type objects go thru PyType_Ready.
     */
    _Py_AddToAllObjects((PyObject *)type, 0);
#endif

    /* Initialize tp_base (defaults to BaseObject unless that's us) */
    base = type->tp_base;
    if (base == NULL && type != &PyBaseObject_Type) {
        base = type->tp_base = &PyBaseObject_Type;
        Py_INCREF(base);
    }

    /* Now the only way base can still be NULL is if type is
     * &PyBaseObject_Type.
     */

    /* Initialize the base class */
    if (base && base->tp_dict == NULL) {
        if (PyType_Ready(base) < 0)
            goto error;
    }

    /* Initialize ob_type if NULL.      This means extensions that want to be
       compilable separately on Windows can call PyType_Ready() instead of
       initializing the ob_type field of their type objects. */
    /* The test for base != NULL is really unnecessary, since base is only
       NULL when type is &PyBaseObject_Type, and we know its ob_type is
       not NULL (it's initialized to &PyType_Type).      But coverity doesn't
       know that. */
    if (Py_TYPE(type) == NULL && base != NULL)
        Py_TYPE(type) = Py_TYPE(base);

    /* Initialize tp_bases */
    bases = type->tp_bases;
    if (bases == NULL) {
        if (base == NULL)
            bases = PyTuple_New(0);
        else
            bases = PyTuple_Pack(1, base);
        if (bases == NULL)
            goto error;
        type->tp_bases = bases;
    }

    /* Initialize tp_dict */
    dict = type->tp_dict;
    if (dict == NULL) {
        dict = PyDict_New();
        if (dict == NULL)
            goto error;
        type->tp_dict = dict;
    }

    /* Add type-specific descriptors to tp_dict */
    if (add_operators(type) < 0)
        goto error;
    if (type->tp_methods != NULL) {
        if (add_methods(type, type->tp_methods) < 0)
            goto error;
    }
    if (type->tp_members != NULL) {
        if (add_members(type, type->tp_members) < 0)
            goto error;
    }
    if (type->tp_getset != NULL) {
        if (add_getset(type, type->tp_getset) < 0)
            goto error;
    }

    /* Calculate method resolution order */
    if (mro_internal(type) < 0) {
        goto error;
    }

    /* Inherit special flags from dominant base */
    if (type->tp_base != NULL)
        inherit_special(type, type->tp_base);

    /* Initialize tp_dict properly */
    bases = type->tp_mro;
    assert(bases != NULL);
    assert(PyTuple_Check(bases));
    n = PyTuple_GET_SIZE(bases);
    for (i = 1; i < n; i++) {
        PyObject *b = PyTuple_GET_ITEM(bases, i);
        if (PyType_Check(b))
            inherit_slots(type, (PyTypeObject *)b);
    }

    /* Sanity check for tp_free. */
    if (PyType_IS_GC(type) && (type->tp_flags & Py_TPFLAGS_BASETYPE) &&
        (type->tp_free == NULL || type->tp_free == PyObject_Del)) {
        /* This base class needs to call tp_free, but doesn't have
         * one, or its tp_free is for non-gc'ed objects.
         */
        PyErr_Format(PyExc_TypeError, "type '%.100s' participates in "
                     "gc and is a base type but has inappropriate "
                     "tp_free slot",
                     type->tp_name);
        goto error;
    }

    /* if the type dictionary doesn't contain a __doc__, set it from
       the tp_doc slot.
     */
    if (PyDict_GetItemString(type->tp_dict, "__doc__") == NULL) {
        if (type->tp_doc != NULL) {
            PyObject *doc = PyString_FromString(type->tp_doc);
            if (doc == NULL)
                goto error;
            PyDict_SetItemString(type->tp_dict, "__doc__", doc);
            Py_DECREF(doc);
        } else {
            PyDict_SetItemString(type->tp_dict,
                                 "__doc__", Py_None);
        }
    }

    /* Some more special stuff */
    base = type->tp_base;
    if (base != NULL) {
        if (type->tp_as_number == NULL)
            type->tp_as_number = base->tp_as_number;
        if (type->tp_as_sequence == NULL)
            type->tp_as_sequence = base->tp_as_sequence;
        if (type->tp_as_mapping == NULL)
            type->tp_as_mapping = base->tp_as_mapping;
        if (type->tp_as_buffer == NULL)
            type->tp_as_buffer = base->tp_as_buffer;
    }

    /* Link into each base class's list of subclasses */
    bases = type->tp_bases;
    n = PyTuple_GET_SIZE(bases);
    for (i = 0; i < n; i++) {
        PyObject *b = PyTuple_GET_ITEM(bases, i);
        if (PyType_Check(b) &&
            add_subclass((PyTypeObject *)b, type) < 0)
            goto error;
    }

    /* All done -- set the ready flag */
    assert(type->tp_dict != NULL);
    type->tp_flags =
        (type->tp_flags & ~Py_TPFLAGS_READYING) | Py_TPFLAGS_READY;
    return 0;

  error:
    type->tp_flags &= ~Py_TPFLAGS_READYING;
    return -1;
}





/*
>>> s=""" 
class A(object):
	name = 'Python'
	def __init__(self):
		print 'A::__init__'

	def f(self):
		print 'A::f'

	def g(self, aValue):
		self.value = aValue
		print self.value
a=A()
a.f()
a.g(10)
"""
co=compile(s,"class_0.py","exec")
co.co_consts
('A', <code object A at 0000000002901AB0, file "class_0.py", line 2>, 10, None)
co.co_names
('object', 'A', 'a', 'f', 'g')
co_c.co_names
('__name__', '__module__', 'name', '__init__', 'f', 'g')
>>> co_c=co.co_consts[1]
>>> co_c.co_consts
('Python', <code object __init__ at 00000000028DB030, file "class_0.py", line 4>, <code object f at 00000000029016B0, file "class_0.py", line 7>, <code object g at 00000000029017B0, file "class_0.py", line 10>)
>>> co_c.co_flags
66
#define CO_NEWLOCALS	0x0002

>>> co_c.co_names
('__name__', '__module__', 'name', '__init__', 'f', 'g')
>>> co_i=co_c.co_consts[1]
>>> co_i.co_consts
(None, 'A::__init__')
>>> co_i.co_names
()
>>> co_f=co_c.co_consts[2]
>>> co_f.co_consts
(None, 'A::f')
>>> co_f.co_names
()
>>> co_g=co_c.co_consts[3]
>>> co_g.co_consts
(None,)
>>> co_g.co_names
('value',)
>>> import dis
>>> dis.dis(co)
  2           0 LOAD_CONST               0 ('A')
              3 LOAD_NAME                0 (object)
              6 BUILD_TUPLE              1
              9 LOAD_CONST               1 (<code object A at 0000000002901AB0, file "class_0.py", line 2>)
             12 MAKE_FUNCTION            0
             15 CALL_FUNCTION            0
             18 BUILD_CLASS         
             19 STORE_NAME               1 (A)

 13          22 LOAD_NAME                1 (A)
             25 CALL_FUNCTION            0
             28 STORE_NAME               2 (a)

 14          31 LOAD_NAME                2 (a)
             34 LOAD_ATTR                3 (f)
             37 CALL_FUNCTION            0
             40 POP_TOP             

 15          41 LOAD_NAME                2 (a)
             44 LOAD_ATTR                4 (g)
             47 LOAD_CONST               2 (10)
             50 CALL_FUNCTION            1
             53 POP_TOP             
             54 LOAD_CONST               3 (None)
             57 RETURN_VALUE        
>>> dis.dis(co_c)
  2           0 LOAD_NAME                0 (__name__)
              3 STORE_NAME               1 (__module__)

  3           6 LOAD_CONST               0 ('Python')
              9 STORE_NAME               2 (name)

  4          12 LOAD_CONST               1 (<code object __init__ at 00000000028DB030, file "class_0.py", line 4>)
             15 MAKE_FUNCTION            0
             18 STORE_NAME               3 (__init__)

  7          21 LOAD_CONST               2 (<code object f at 00000000029016B0, file "class_0.py", line 7>)
             24 MAKE_FUNCTION            0
             27 STORE_NAME               4 (f)

 10          30 LOAD_CONST               3 (<code object g at 00000000029017B0, file "class_0.py", line 10>)
             33 MAKE_FUNCTION            0
             36 STORE_NAME               5 (g)
             39 LOAD_LOCALS         
             40 RETURN_VALUE        
>>> dis.dis(co_i)
  5           0 LOAD_CONST               1 ('A::__init__')
              3 PRINT_ITEM          
              4 PRINT_NEWLINE       
              5 LOAD_CONST               0 (None)
              8 RETURN_VALUE        
>>> dis.dis(co_f)
  8           0 LOAD_CONST               1 ('A::f')
              3 PRINT_ITEM          
              4 PRINT_NEWLINE       
              5 LOAD_CONST               0 (None)
              8 RETURN_VALUE        
>>> dis.dis(co_g)
 11           0 LOAD_FAST                1 (aValue)
              3 LOAD_FAST                0 (self)
              6 STORE_ATTR               0 (value)

 12           9 LOAD_FAST                0 (self)
             12 LOAD_ATTR                0 (value)
             15 PRINT_ITEM          
             16 PRINT_NEWLINE       
             17 LOAD_CONST               0 (None)
             20 RETURN_VALUE        
>>> 


类中的语句都是拿来执行的
>>> s=""" 
class A(object):
	def f(a):
		print a
	print f
	f=staticmethod(f)
	print f
"""
>>> co=compile(s,"static.py","exec")
>>> co.co_consts
('A', <code object A at 0000000002970330, file "static.py", line 2>, None)
>>> co_a=co.co_consts[1]
>>> dis.dis(co_a)
  2           0 LOAD_NAME                0 (__name__)
              3 STORE_NAME               1 (__module__)

  3           6 LOAD_CONST               0 (<code object f at 0000000002968030, file "static.py", line 3>)
              9 MAKE_FUNCTION            0
             12 STORE_NAME               2 (f)

  5          15 LOAD_NAME                2 (f)
             18 PRINT_ITEM          
             19 PRINT_NEWLINE       

  6          20 LOAD_NAME                3 (staticmethod)
             23 LOAD_NAME                2 (f)
             26 CALL_FUNCTION            1
             29 STORE_NAME               2 (f)

  7          32 LOAD_NAME                2 (f)
             35 PRINT_ITEM          
             36 PRINT_NEWLINE       
             37 LOAD_LOCALS         
             38 RETURN_VALUE        

看懂cell和free
>>> s=""" 
def f1(a):
	def f2(b):
		print a
		def f3(c):
			print b
		return f3
	return f2(1)
"""
>>> co=compile(s,"static.py","exec")
>>> co.co_consts
(<code object f1 at 0000000002CF1730, file "static.py", line 2>, None)
>>> co.co_consts[0].co_consts
(None, <code object f2 at 0000000002CF16B0, file "static.py", line 3>, 1)
>>> co.co_consts[0].co_consts[1]
<code object f2 at 0000000002CF16B0, file "static.py", line 3>
>>> co_f2=co.co_consts[0].co_consts[1]
>>> dis.dis(co_f2)
  4           0 LOAD_DEREF               1 (a)free靠后freevars[1]
              3 PRINT_ITEM          
              4 PRINT_NEWLINE       

  5           5 LOAD_CLOSURE             0 (b)cell靠前freevars[0]
              8 BUILD_TUPLE              1
             11 LOAD_CONST               1 (<code object f3 at 000000000295BAB0, file "static.py", line 5>)
             14 MAKE_CLOSURE             0
             17 STORE_FAST               1 (f3)

  7          20 LOAD_FAST                1 (f3)
             23 RETURN_VALUE        
>>> 
*/

static PyObject *
build_class(PyObject *methods, PyObject *bases, PyObject *name)
{
    PyObject *metaclass = NULL, *result, *base;

    if (PyDict_Check(methods))
        metaclass = PyDict_GetItemString(methods, "__metaclass__");
    if (metaclass != NULL)
        Py_INCREF(metaclass);
    else if (PyTuple_Check(bases) && PyTuple_GET_SIZE(bases) > 0) {
        base = PyTuple_GET_ITEM(bases, 0);
        metaclass = PyObject_GetAttrString(base, "__class__");
        if (metaclass == NULL) {
            PyErr_Clear();
            metaclass = (PyObject *)base->ob_type;
            Py_INCREF(metaclass);
        }
    }
    else {
        PyObject *g = PyEval_GetGlobals();
        if (g != NULL && PyDict_Check(g))
            metaclass = PyDict_GetItemString(g, "__metaclass__");
        if (metaclass == NULL)
            metaclass = (PyObject *) &PyClass_Type;
        Py_INCREF(metaclass);
    }
    result = PyObject_CallFunctionObjArgs(metaclass, name, bases, methods,
                                          NULL);
    Py_DECREF(metaclass);
    if (result == NULL && PyErr_ExceptionMatches(PyExc_TypeError)) {
        /* A type error here likely means that the user passed
           in a base that was not a class (such the random module
           instead of the random.random type).  Help them out with
           by augmenting the error message with more information.*/

        PyObject *ptype, *pvalue, *ptraceback;

        PyErr_Fetch(&ptype, &pvalue, &ptraceback);
        if (PyString_Check(pvalue)) {
            PyObject *newmsg;
            newmsg = PyString_FromFormat(
                "Error when calling the metaclass bases\n"
                "    %s",
                PyString_AS_STRING(pvalue));
            if (newmsg != NULL) {
                Py_DECREF(pvalue);
                pvalue = newmsg;
            }
        }
        PyErr_Restore(ptype, pvalue, ptraceback);
    }
    return result;
}

PyObject *
PyObject_CallFunctionObjArgs(PyObject *callable, ...)
{
    PyObject *args, *tmp;
    va_list vargs;

    if (callable == NULL)
        return null_error();

    /* count the args */
    va_start(vargs, callable);
    args = objargs_mktuple(vargs);
    va_end(vargs);
    if (args == NULL)
        return NULL;
    tmp = PyObject_Call(callable, args, NULL);
    Py_DECREF(args);

    return tmp;
}

typedef PyObject * (*ternaryfunc)(PyObject *, PyObject *, PyObject *);//in object.h
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

static PyObject *
type_call(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    PyObject *obj;

    if (type->tp_new == NULL) {
        PyErr_Format(PyExc_TypeError,
                     "cannot create '%.100s' instances",
                     type->tp_name);
        return NULL;
    }

    obj = type->tp_new(type, args, kwds);
    if (obj != NULL) {
        /* Ugly exception: when the call was type(something),
           don't call tp_init on the result. */
        if (type == &PyType_Type &&
            PyTuple_Check(args) && PyTuple_GET_SIZE(args) == 1 &&
            (kwds == NULL ||
             (PyDict_Check(kwds) && PyDict_Size(kwds) == 0)))
            return obj;
        /* If the returned object is not an instance of type,
           it won't be initialized. */
        if (!PyType_IsSubtype(obj->ob_type, type))
            return obj;
        type = obj->ob_type;
        if (PyType_HasFeature(type, Py_TPFLAGS_HAVE_CLASS) &&
            type->tp_init != NULL &&
            type->tp_init(obj, args, kwds) < 0) {
            Py_DECREF(obj);
            obj = NULL;
        }
    }
    return obj;
}

case LOAD_LOCALS:
	if ((x = f->f_locals) != NULL) {
		Py_INCREF(x);
		PUSH(x);
		continue;
	}
	PyErr_SetString(PyExc_SystemError, "no locals");
	break;
case BUILD_CLASS:
	u = TOP();
	v = SECOND();
	w = THIRD();
	STACKADJ(-2);
	x = build_class(u, v, w);
	SET_TOP(x);
	Py_DECREF(u);
	Py_DECREF(v);
	Py_DECREF(w);
	break;
case LOAD_ATTR:
	w = GETITEM(names, oparg);
	v = TOP();
	x = PyObject_GetAttr(v, w);
	Py_DECREF(v);
	SET_TOP(x);
	if (x != NULL) continue;
	break;

PyObject *
PyObject_GetAttr(PyObject *v, PyObject *name)
{
    PyTypeObject *tp = Py_TYPE(v);

    if (!PyString_Check(name)) {
#ifdef Py_USING_UNICODE
        /* The Unicode to string conversion is done here because the
           existing tp_getattro slots expect a string object as name
           and we wouldn't want to break those. */
        if (PyUnicode_Check(name)) {
            name = _PyUnicode_AsDefaultEncodedString(name, NULL);
            if (name == NULL)
                return NULL;
        }
        else
#endif
        {
            PyErr_Format(PyExc_TypeError,
                         "attribute name must be string, not '%.200s'",
                         Py_TYPE(name)->tp_name);
            return NULL;
        }
    }
    if (tp->tp_getattro != NULL)
        return (*tp->tp_getattro)(v, name);
    if (tp->tp_getattr != NULL)
        return (*tp->tp_getattr)(v, PyString_AS_STRING(name));
    PyErr_Format(PyExc_AttributeError,
                 "'%.50s' object has no attribute '%.400s'",
                 tp->tp_name, PyString_AS_STRING(name));
    return NULL;
}
/*
!!!how to get attr? 
class's data descriptor>instance>class
class's descriptor will call get,not the same in instance's descriptor
*/

/*
 function to method in funcobject.c
a.__class__.__dict__["f"] is a function
a.f is a method
 */
static PyObject *
func_descr_get(PyObject *func, PyObject *obj, PyObject *type)
{
    if (obj == Py_None)
        obj = NULL;
    return PyMethod_New(func, obj, type);
}
PyObject *
PyMethod_New(PyObject *func, PyObject *self, PyObject *klass)
{
    register PyMethodObject *im;
    im = free_list;
    if (im != NULL) {
        free_list = (PyMethodObject *)(im->im_self);
        PyObject_INIT(im, &PyMethod_Type);
        numfree--;
    }
    else {
        im = PyObject_GC_New(PyMethodObject, &PyMethod_Type);
        if (im == NULL)
            return NULL;
    }
    im->im_weakreflist = NULL;
    Py_INCREF(func);
    im->im_func = func;
    Py_XINCREF(self);
    im->im_self = self;
    Py_XINCREF(klass);
    im->im_class = klass;
    _PyObject_GC_TRACK(im);
    return (PyObject *)im;
}
typedef struct {
    PyObject_HEAD
    PyObject *im_func;   /* The callable object implementing the method */
    PyObject *im_self;   /* The instance it is bound to, or NULL */
    PyObject *im_class;  /* The class that asked for the method */
    PyObject *im_weakreflist; /* List of weak references */
} PyMethodObject;
/*
in type_getattro
bound_method: return meta_get(meta_attribute, (PyObject *)type,(PyObject *)metatype);
unbound_method: return meta_get(meta_attribute, (PyObject *)Null,(PyObject *)metatype);
 */

/*
 staticmethod
functype->stacictype->statictype is a descriptor->so just return func
 */
typedef struct {
    PyObject_HEAD
    PyObject *sm_callable;
} staticmethod;

static int
sm_init(PyObject *self, PyObject *args, PyObject *kwds)
{
    staticmethod *sm = (staticmethod *)self;
    PyObject *callable;

    if (!PyArg_UnpackTuple(args, "staticmethod", 1, 1, &callable))
        return -1;
    if (!_PyArg_NoKeywords("staticmethod", kwds))
        return -1;
    Py_INCREF(callable);
    sm->sm_callable = callable;
    return 0;
}
static PyObject *
sm_descr_get(PyObject *self, PyObject *obj, PyObject *type)
{
    staticmethod *sm = (staticmethod *)self;

    if (sm->sm_callable == NULL) {
        PyErr_SetString(PyExc_RuntimeError,
                        "uninitialized staticmethod object");
        return NULL;
    }
    Py_INCREF(sm->sm_callable);
    return sm->sm_callable;
}

