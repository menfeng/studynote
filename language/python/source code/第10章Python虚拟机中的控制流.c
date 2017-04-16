//pythonÔ´Âë½âÎö±Ê¼Ç

//bool
#define Py_False ((PyObject *) &_Py_ZeroStruct)
#define Py_True ((PyObject *) &_Py_TrueStruct)
PyIntObject _Py_ZeroStruct = {
    PyObject_HEAD_INIT(&PyBool_Type)
    0
};

PyIntObject _Py_TrueStruct = {
    PyObject_HEAD_INIT(&PyBool_Type)
    1
};

/*
s="""
a=1
if a > 10:
	print "a > 10" 
elif a <= -2:
	print "a <= -2"
elif a != 1:
	print "a != 1"
elif a == 1:
	print "a == 1"
else:
	print "Unknown"
"""
co=compile(s,"if_control.py","exec")
co.co_consts
(1, 10, 'a > 10', -2, 'a <= -2', 'a != 1', 'a == 1', 'Unknown', None)
co.co_names
('a',)
dis.dis(co)
  2           0 LOAD_CONST               0 (1)
              3 STORE_NAME               0 (a)

  3           6 LOAD_NAME                0 (a)
              9 LOAD_CONST               1 (10)
             12 COMPARE_OP               4 (>)
             15 POP_JUMP_IF_FALSE       26

  4          18 LOAD_CONST               2 ('a > 10')
             21 PRINT_ITEM          
             22 PRINT_NEWLINE       
             23 JUMP_FORWARD            65 (to 91)************26+65=91

  5     >>   26 LOAD_NAME                0 (a)
             29 LOAD_CONST               3 (-2)
             32 COMPARE_OP               1 (<=)
             35 POP_JUMP_IF_FALSE       46

  6          38 LOAD_CONST               4 ('a <= -2')
             41 PRINT_ITEM          
             42 PRINT_NEWLINE       
             43 JUMP_FORWARD            45 (to 91)

  7     >>   46 LOAD_NAME                0 (a)
             49 LOAD_CONST               0 (1)
             52 COMPARE_OP               3 (!=)
             55 POP_JUMP_IF_FALSE       66

  8          58 LOAD_CONST               5 ('a != 1')
             61 PRINT_ITEM          
             62 PRINT_NEWLINE       
             63 JUMP_FORWARD            25 (to 91)

  9     >>   66 LOAD_NAME                0 (a)
             69 LOAD_CONST               0 (1)
             72 COMPARE_OP               2 (==)
             75 POP_JUMP_IF_FALSE       86

 10          78 LOAD_CONST               6 ('a == 1')
             81 PRINT_ITEM          
             82 PRINT_NEWLINE       
             83 JUMP_FORWARD             5 (to 91)

 12     >>   86 LOAD_CONST               7 ('Unknown')
             89 PRINT_ITEM          
             90 PRINT_NEWLINE       
        >>   91 LOAD_CONST               8 (None)
             94 RETURN_VALUE        

s="""
lst=[1,2]
for i in lst:
	print i
"""
co=compile(s,"for_control.py","exec")
co.co_consts
(1, 2, None)
co.co_names
('lst', 'i')
dis.dis(co)
  2           0 LOAD_CONST               0 (1)
              3 LOAD_CONST               1 (2)
              6 BUILD_LIST               2
              9 STORE_NAME               0 (lst)

  3          12 SETUP_LOOP              19 (to 34)
             15 LOAD_NAME                0 (lst)
             18 GET_ITER            
        >>   19 FOR_ITER                11 (to 33)
             22 STORE_NAME               1 (i)

  4          25 LOAD_NAME                1 (i)
             28 PRINT_ITEM          
             29 PRINT_NEWLINE       
             30 JUMP_ABSOLUTE           19
        >>   33 POP_BLOCK           
        >>   34 LOAD_CONST               2 (None)
             37 RETURN_VALUE        

s="""
i=0
while i<10:
	i+=1
	if i>5:
		continue
	if i==20:
		break
	print i
"""
>>> co=compile(s,"whiler_control.py","exec")
>>> co.co_consts
(0, 10, 1, 5, 20, None)
>>> co.co_names
('i',)
>>> dis.dis(co)
  2           0 LOAD_CONST               0 (0)
              3 STORE_NAME               0 (i)

  3           6 SETUP_LOOP              65 (to 74)
        >>    9 LOAD_NAME                0 (i)
             12 LOAD_CONST               1 (10)
             15 COMPARE_OP               0 (<)
             18 POP_JUMP_IF_FALSE       73

  4          21 LOAD_NAME                0 (i)
             24 LOAD_CONST               2 (1)
             27 INPLACE_ADD         
             28 STORE_NAME               0 (i)

  5          31 LOAD_NAME                0 (i)
             34 LOAD_CONST               3 (5)
             37 COMPARE_OP               4 (>)
             40 POP_JUMP_IF_FALSE       49

  6          43 JUMP_ABSOLUTE            9
             46 JUMP_FORWARD             0 (to 49)

  7     >>   49 LOAD_NAME                0 (i)
             52 LOAD_CONST               4 (20)
             55 COMPARE_OP               2 (==)
             58 POP_JUMP_IF_FALSE       65

  8          61 BREAK_LOOP          
             62 JUMP_FORWARD             0 (to 65)

  9     >>   65 LOAD_NAME                0 (i)
             68 PRINT_ITEM          
             69 PRINT_NEWLINE       
             70 JUMP_ABSOLUTE            9
        >>   73 POP_BLOCK           
        >>   74 LOAD_CONST               5 (None)
             77 RETURN_VALUE        



>>> s="""
try:
	raise Exception("i am an exception")	
except Exception,e:
	print e
finally:
	print "the finally code"
"""
>>> 
>>> co=compile(s,"exception_control.py","exec")
>>> co.co_consts
('i am an exception', None, 'the finally code')
>>> co.co_names
('Exception', 'e')
>>> dis.dis(co)
  2           0 SETUP_FINALLY           47 (to 50)
              3 SETUP_EXCEPT            16 (to 22)

  3           6 LOAD_NAME                0 (Exception)
              9 LOAD_CONST               0 ('i am an exception')
             12 CALL_FUNCTION            1
             15 RAISE_VARARGS            1			//push tb,val,exc|SETUP_EXCEPT|-->22
             18 POP_BLOCK           
             19 JUMP_FORWARD            24 (to 46)

  4     >>   22 DUP_TOP             
             23 LOAD_NAME                0 (Exception)
             26 COMPARE_OP              10 (exception match)
             29 POP_JUMP_IF_FALSE       45			//|not match|-->45
             32 POP_TOP             
             33 STORE_NAME               1 (e)
             36 POP_TOP             

  5          37 LOAD_NAME                1 (e)
             40 PRINT_ITEM          
             41 PRINT_NEWLINE       
             42 JUMP_FORWARD             1 (to 46)	//-->46
        >>   45 END_FINALLY        					//pop exc,val,tb|restore|SETUP_FINALLY|-->50
        >>   46 POP_BLOCK          					//POP(not exe SETUP_FINALLY,so we need to pop manually) 
             47 LOAD_CONST               1 (None)

  7     >>   50 LOAD_CONST               2 ('the finally code')
             53 PRINT_ITEM          
             54 PRINT_NEWLINE       
             55 END_FINALLY         
             56 LOAD_CONST               1 (None)
             59 RETURN_VALUE        
*/

//macro
#define GETITEM(v, i) PyTuple_GET_ITEM((PyTupleObject *)(v), (i))
//adjust stack_top
#define BASIC_STACKADJ(n) (stack_pointer += n)
#define STACKADJ(n)            BASIC_STACKADJ(n)
//push stack
#define BASIC_PUSH(v)     (*stack_pointer++ = (v))
#define PUSH(v)                BASIC_PUSH(v)
//pop stack
#define BASIC_POP()       (*--stack_pointer)
#define POP()                  BASIC_POP()

#define TOP()             (stack_pointer[-1])
#define SET_TOP(v)        (stack_pointer[-1] = (v))
#define SECOND()          (stack_pointer[-2])
#define THIRD()           (stack_pointer[-3])

#define PREDICT(op)             if (*next_instr == op) goto PRED_##op
#define PREDICTED(op)           PRED_##op: next_instr++
#define PREDICTED_WITH_ARG(op)  PRED_##op: oparg = PEEKARG(); next_instr += 3
#define PEEKARG()       ((next_instr[2]<<8) + next_instr[1])
#define JUMPTO(x)       (next_instr = first_instr + (x))
#define JUMPBY(x)       (next_instr += (x))

#define STACK_LEVEL()     ((int)(stack_pointer - f->f_valuestack))
#define INSTR_OFFSET()  ((int)(next_instr - first_instr))

case LOAD_CONST:
	x = GETITEM(consts, oparg);
	Py_INCREF(x);
	PUSH(x);
case STORE_NAME:
	w = GETITEM(names, oparg);
	v = POP();
	if ((x = f->f_locals) != NULL) {
		if (PyDict_CheckExact(x))
			err = PyDict_SetItem(x, w, v);
		else
			err = PyObject_SetItem(x, w, v);
		Py_DECREF(v);
		if (err == 0) continue;
		break;
	}
	t = PyObject_Repr(w);
	if (t == NULL)
		break;
	PyErr_Format(PyExc_SystemError,
				 "no locals found when storing %s",
				 PyString_AS_STRING(t));
	Py_DECREF(t);
	break;
case BUILD_MAP:
	x = _PyDict_NewPresized((Py_ssize_t)oparg);
	PUSH(x);
	if (x != NULL) continue;
	break;
case BUILD_LIST:
	x =  PyList_New(oparg);
	if (x != NULL) {
		for (; --oparg >= 0;) {
			w = POP();
			PyList_SET_ITEM(x, oparg, w);
		}
		PUSH(x);
		continue;
	}
	break;
case RETURN_VALUE:
	retval = POP();
	why = WHY_RETURN;
	goto fast_block_end;
case STORE_MAP:
	w = TOP();     /* key */
	u = SECOND();  /* value */
	v = THIRD();   /* dict */
	STACKADJ(-2);
	assert (PyDict_CheckExact(v));
	err = PyDict_SetItem(v, w, u);  /* v[w] = u */
	Py_DECREF(u);
	Py_DECREF(w);
	if (err == 0) continue;
	break;
case LOAD_NAME:
	w = GETITEM(names, oparg);
	if ((v = f->f_locals) == NULL) {
		why = WHY_EXCEPTION;
		t = PyObject_Repr(w);
		if (t == NULL)
			break;
		PyErr_Format(PyExc_SystemError,
					 "no locals when loading %s",
					 PyString_AS_STRING(w));
		Py_DECREF(t);
		break;
	}
	if (PyDict_CheckExact(v)) {
		x = PyDict_GetItem(v, w);
		Py_XINCREF(x);
	}
	else {
		x = PyObject_GetItem(v, w);
		if (x == NULL && PyErr_Occurred()) {
			if (!PyErr_ExceptionMatches(
							PyExc_KeyError))
				break;
			PyErr_Clear();
		}
	}
	if (x == NULL) {
		x = PyDict_GetItem(f->f_globals, w);
		if (x == NULL) {
			x = PyDict_GetItem(f->f_builtins, w);
			if (x == NULL) {
				format_exc_check_arg(
							PyExc_NameError,
							NAME_ERROR_MSG, w);
				break;
			}
		}
		Py_INCREF(x);
	}
	PUSH(x);
	continue;
case BINARY_ADD:
	w = POP();
	v = TOP();
	if (PyInt_CheckExact(v) && PyInt_CheckExact(w)) {
		/* INLINE: int + int */
		register long a, b, i;
		a = PyInt_AS_LONG(v);
		b = PyInt_AS_LONG(w);
		/* cast to avoid undefined behaviour
		   on overflow */
		i = (long)((unsigned long)a + b);
		if ((i^a) < 0 && (i^b) < 0)
			goto slow_add;
		x = PyInt_FromLong(i);
	}
	else if (PyString_CheckExact(v) &&
			 PyString_CheckExact(w)) {
		x = string_concatenate(v, w, f, next_instr);
		/* string_concatenate consumed the ref to v */
		goto skip_decref_vx;
	}
	else {
	  slow_add:
		x = PyNumber_Add(v, w);
	}
	Py_DECREF(v);
  skip_decref_vx:
	Py_DECREF(w);
	SET_TOP(x);
	if (x != NULL) continue;
	break;

case PRINT_ITEM:
	v = POP();
	if (stream == NULL || stream == Py_None) {
		w = PySys_GetObject("stdout");
		if (w == NULL) {
			PyErr_SetString(PyExc_RuntimeError,
							"lost sys.stdout");
			err = -1;
		}
	}
	/* PyFile_SoftSpace() can exececute arbitrary code
	   if sys.stdout is an instance with a __getattr__.
	   If __getattr__ raises an exception, w will
	   be freed, so we need to prevent that temporarily. */
	Py_XINCREF(w);
	if (w != NULL && PyFile_SoftSpace(w, 0))
		err = PyFile_WriteString(" ", w);
	if (err == 0)
		err = PyFile_WriteObject(v, w, Py_PRINT_RAW);
	if (err == 0) {
		/* XXX move into writeobject() ? */
		if (PyString_Check(v)) {
			char *s = PyString_AS_STRING(v);
			Py_ssize_t len = PyString_GET_SIZE(v);
			if (len == 0 ||
				!isspace(Py_CHARMASK(s[len-1])) ||
				s[len-1] == ' ')
				PyFile_SoftSpace(w, 1);
		}
#ifdef Py_USING_UNICODE
		else if (PyUnicode_Check(v)) {
			Py_UNICODE *s = PyUnicode_AS_UNICODE(v);
			Py_ssize_t len = PyUnicode_GET_SIZE(v);
			if (len == 0 ||
				!Py_UNICODE_ISSPACE(s[len-1]) ||
				s[len-1] == ' ')
				PyFile_SoftSpace(w, 1);
		}
#endif
		else
			PyFile_SoftSpace(w, 1);
	}
	Py_XDECREF(w);
	Py_DECREF(v);
	Py_XDECREF(stream);
	stream = NULL;
	if (err == 0)
		continue;
	break;

case PRINT_NEWLINE:
	if (stream == NULL || stream == Py_None) {
		w = PySys_GetObject("stdout");
		if (w == NULL) {
			PyErr_SetString(PyExc_RuntimeError,
							"lost sys.stdout");
			why = WHY_EXCEPTION;
		}
	}
	if (w != NULL) {
		/* w.write() may replace sys.stdout, so we
		 * have to keep our reference to it */
		Py_INCREF(w);
		err = PyFile_WriteString("\n", w);
		if (err == 0)
			PyFile_SoftSpace(w, 0);
		Py_DECREF(w);
	}
	Py_XDECREF(stream);
	stream = NULL;
	break;
case COMPARE_OP:
	w = POP();
	v = TOP();
	if (PyInt_CheckExact(w) && PyInt_CheckExact(v)) {
		/* INLINE: cmp(int, int) */
		register long a, b;
		register int res;
		a = PyInt_AS_LONG(v);
		b = PyInt_AS_LONG(w);
		switch (oparg) {
		case PyCmp_LT: res = a <  b; break;
		case PyCmp_LE: res = a <= b; break;
		case PyCmp_EQ: res = a == b; break;
		case PyCmp_NE: res = a != b; break;
		case PyCmp_GT: res = a >  b; break;
		case PyCmp_GE: res = a >= b; break;
		case PyCmp_IS: res = v == w; break;
		case PyCmp_IS_NOT: res = v != w; break;
		default: goto slow_compare;
		}
		x = res ? Py_True : Py_False;
		Py_INCREF(x);
	}
	else {
	  slow_compare:
		x = cmp_outcome(oparg, v, w);
	}
	Py_DECREF(v);
	Py_DECREF(w);
	SET_TOP(x);
	if (x == NULL) break;
	PREDICT(POP_JUMP_IF_FALSE);
	PREDICT(POP_JUMP_IF_TRUE);
	continue;
PREDICTED_WITH_ARG(POP_JUMP_IF_FALSE);
case POP_JUMP_IF_FALSE:
	w = POP();
	if (w == Py_True) {
		Py_DECREF(w);
		goto fast_next_opcode;
	}
	if (w == Py_False) {
		Py_DECREF(w);
		JUMPTO(oparg);
		goto fast_next_opcode;
	}
	err = PyObject_IsTrue(w);
	Py_DECREF(w);
	if (err > 0)
		err = 0;
	else if (err == 0)
		JUMPTO(oparg);
	else
		break;
	continue;
case JUMP_FORWARD:
	JUMPBY(oparg);
	goto fast_next_opcode;
case SETUP_LOOP:
case SETUP_EXCEPT:
case SETUP_FINALLY:
	/* NOTE: If you add any new block-setup opcodes that
	   are not try/except/finally handlers, you may need
	   to update the PyGen_NeedsFinalizing() function.
	   */

	PyFrame_BlockSetup(f, opcode, INSTR_OFFSET() + oparg,
					   STACK_LEVEL());
	continue;

case POP_BLOCK:
	{
		PyTryBlock *b = PyFrame_BlockPop(f);
		while (STACK_LEVEL() > b->b_level) {
			v = POP();
			Py_DECREF(v);
		}
	}
	continue;
//***comment begin


//in PyFrameObject 
int f_iblock;		/* index in f_blockstack */
PyTryBlock f_blockstack[CO_MAXBLOCKS]; /* for try and loop blocks */

//type:op,handler hander:codebyte pos level:stacklevel
 void
PyFrame_BlockSetup(PyFrameObject *f, int type, int handler, int level)
{
    PyTryBlock *b;
    if (f->f_iblock >= CO_MAXBLOCKS)
        Py_FatalError("XXX block stack overflow");
    b = &f->f_blockstack[f->f_iblock++];
    b->b_type = type;
    b->b_level = level;
    b->b_handler = handler;
}
typedef struct {
    int b_type;			/* what kind of block this is */
    int b_handler;		/* where to jump to find handler */
    int b_level;		/* value stack level to pop to */
} PyTryBlock;

PyTryBlock *
PyFrame_BlockPop(PyFrameObject *f)
{
    PyTryBlock *b;
    if (f->f_iblock <= 0)
        Py_FatalError("XXX block stack underflow");
    b = &f->f_blockstack[--f->f_iblock];
    return b;
}
//**comment end
case GET_ITER:
	/* before: [obj]; after [getiter(obj)] */
	v = TOP();
	x = PyObject_GetIter(v);
	Py_DECREF(v);
	if (x != NULL) {
		SET_TOP(x);
		PREDICT(FOR_ITER);
		continue;
	}
	STACKADJ(-1);
	break;

PREDICTED_WITH_ARG(FOR_ITER);
case FOR_ITER:
	/* before: [iter]; after: [iter, iter()] *or* [] */
	v = TOP();
	x = (*v->ob_type->tp_iternext)(v);
	if (x != NULL) {
		PUSH(x);
		PREDICT(STORE_FAST);
		PREDICT(UNPACK_SEQUENCE);
		continue;
	}
	if (PyErr_Occurred()) {
		if (!PyErr_ExceptionMatches(
						PyExc_StopIteration))
			break;
		PyErr_Clear();
	}
	/* iterator ended normally */
	x = v = POP();
	Py_DECREF(v);
	JUMPBY(oparg);
	continue;
//**iter begin
//getiter
PyObject *
PyObject_GetIter(PyObject *o)
{
    PyTypeObject *t = o->ob_type;
    getiterfunc f = NULL;
    if (PyType_HasFeature(t, Py_TPFLAGS_HAVE_ITER))
        f = t->tp_iter;
    if (f == NULL) {
        if (PySequence_Check(o))
            return PySeqIter_New(o);
        return type_error("'%.200s' object is not iterable", o);
    }
    else {
        PyObject *res = (*f)(o);
        if (res != NULL && !PyIter_Check(res)) {
            PyErr_Format(PyExc_TypeError,
                         "iter() returned non-iterator "
                         "of type '%.100s'",
                         res->ob_type->tp_name);
            Py_DECREF(res);
            res = NULL;
        }
        return res;
    }
}
//tp_iter
static PyObject *
list_iter(PyObject *seq)
{
    listiterobject *it;

    if (!PyList_Check(seq)) {
        PyErr_BadInternalCall();
        return NULL;
    }
    it = PyObject_GC_New(listiterobject, &PyListIter_Type);
    if (it == NULL)
        return NULL;
    it->it_index = 0;
    Py_INCREF(seq);
    it->it_seq = (PyListObject *)seq;
    _PyObject_GC_TRACK(it);
    return (PyObject *)it;
}
//lstiterobj
typedef struct {
    PyObject_HEAD
    long it_index;
    PyListObject *it_seq; /* Set to NULL when iterator is exhausted */
} listiterobject;

//tp_iternext
static PyObject *
listiter_next(listiterobject *it)
{
    PyListObject *seq;
    PyObject *item;

    assert(it != NULL);
    seq = it->it_seq;
    if (seq == NULL)
        return NULL;
    assert(PyList_Check(seq));

    if (it->it_index < PyList_GET_SIZE(seq)) {
        item = PyList_GET_ITEM(seq, it->it_index);
        ++it->it_index;
        Py_INCREF(item);
        return item;
    }

    Py_DECREF(seq);
    it->it_seq = NULL;
    return NULL;
}
//**iter end
PREDICTED_WITH_ARG(JUMP_ABSOLUTE);
case JUMP_ABSOLUTE:
	JUMPTO(oparg);
#if FAST_LOOPS
	/* Enabling this path speeds-up all while and for-loops by bypassing
	   the per-loop checks for signals.  By default, this should be turned-off
	   because it prevents detection of a control-break in tight loops like
	   "while 1: pass".  Compile with this option turned-on when you need
	   the speed-up and do not need break checking inside tight loops (ones
	   that contain only instructions ending with goto fast_next_opcode).
	*/
	goto fast_next_opcode;
#else
	continue;
#endif
case BREAK_LOOP:
	why = WHY_BREAK;
	goto fast_block_end;
//comment begin
while (STACK_LEVEL() > b->b_level) {
	v = POP();
	Py_XDECREF(v);
}
if (b->b_type == SETUP_LOOP && why == WHY_BREAK) {
	why = WHY_NOT;
	JUMPTO(b->b_handler);
	break;
}
//comment end
case BINARY_DIVIDE:
	if (!_Py_QnewFlag) {
		w = POP();
		v = TOP();
		x = PyNumber_Divide(v, w);
		Py_DECREF(v);
		Py_DECREF(w);
		SET_TOP(x);
		if (x != NULL) continue;
		break;
	}
	/* -Qnew is in effect:  fall through to
	   BINARY_TRUE_DIVIDE */
//comment begin
//in intobject.c
static PyObject *
int_classic_div(PyIntObject *x, PyIntObject *y)
{
    long xi, yi;
    long d, m;
    CONVERT_TO_LONG(x, xi);
    CONVERT_TO_LONG(y, yi);
    if (Py_DivisionWarningFlag &&
        PyErr_Warn(PyExc_DeprecationWarning, "classic int division") < 0)
        return NULL;
    switch (i_divmod(xi, yi, &d, &m)) {
    case DIVMOD_OK:
        return PyInt_FromLong(d);
    case DIVMOD_OVERFLOW:
        return PyLong_Type.tp_as_number->nb_divide((PyObject *)x,
                                                   (PyObject *)y);
    default:
        return NULL;
    }
}
/* Return type of i_divmod */
enum divmod_result {
    DIVMOD_OK,                  /* Correct result */
    DIVMOD_OVERFLOW,            /* Overflow, try again using longs */
    DIVMOD_ERROR                /* Exception raised */
};

static enum divmod_result
i_divmod(register long x, register long y,
         long *p_xdivy, long *p_xmody)
{
    long xdivy, xmody;

    if (y == 0) {
        PyErr_SetString(PyExc_ZeroDivisionError,
                        "integer division or modulo by zero");
        return DIVMOD_ERROR;
    }
    /* (-sys.maxint-1)/-1 is the only overflow case. */
    if (y == -1 && UNARY_NEG_WOULD_OVERFLOW(x))
        return DIVMOD_OVERFLOW;
    xdivy = x / y;
    /* xdiv*y can overflow on platforms where x/y gives floor(x/y)
     * for x and y with differing signs. (This is unusual
     * behaviour, and C99 prohibits it, but it's allowed by C89;
     * for an example of overflow, take x = LONG_MIN, y = 5 or x =
     * LONG_MAX, y = -5.)  However, x - xdivy*y is always
     * representable as a long, since it lies strictly between
     * -abs(y) and abs(y).  We add casts to avoid intermediate
     * overflow.
     */
    xmody = (long)(x - (unsigned long)xdivy * y);
    /* If the signs of x and y differ, and the remainder is non-0,
     * C89 doesn't define whether xdivy is now the floor or the
     * ceiling of the infinitely precise quotient.  We want the floor,
     * and we have it iff the remainder's sign matches y's.
     */
    if (xmody && ((y ^ xmody) < 0) /* i.e. and signs differ */) {
        xmody += y;
        --xdivy;
        assert(xmody && ((y ^ xmody) >= 0));
    }
    *p_xdivy = xdivy;
    *p_xmody = xmody;
    return DIVMOD_OK;
}
void
PyErr_SetString(PyObject *exception, const char *string)
{
    PyObject *value = PyString_FromString(string);
    PyErr_SetObject(exception, value);
    Py_XDECREF(value);
}
void
PyErr_SetObject(PyObject *exception, PyObject *value)
{
    Py_XINCREF(exception);
    Py_XINCREF(value);
    PyErr_Restore(exception, value, (PyObject *)NULL);
}
void
PyErr_Restore(PyObject *type, PyObject *value, PyObject *traceback)
{
    PyThreadState *tstate = PyThreadState_GET();
    PyObject *oldtype, *oldvalue, *oldtraceback;

    if (traceback != NULL && !PyTraceBack_Check(traceback)) {
        /* XXX Should never happen -- fatal error instead? */
        /* Well, it could be None. */
        Py_DECREF(traceback);
        traceback = NULL;
    }

    /* Save these in locals to safeguard against recursive
       invocation through Py_XDECREF */
    oldtype = tstate->curexc_type;
    oldvalue = tstate->curexc_value;
    oldtraceback = tstate->curexc_traceback;

    tstate->curexc_type = type;
    tstate->curexc_value = value;
    tstate->curexc_traceback = traceback;

    Py_XDECREF(oldtype);
    Py_XDECREF(oldvalue);
    Py_XDECREF(oldtraceback);
}

//in ceval.c PyEval_EvalFrameEx begin
        if (why == WHY_EXCEPTION) {
            PyTraceBack_Here(f);

            if (tstate->c_tracefunc != NULL)
                call_exc_trace(tstate->c_tracefunc,
                               tstate->c_traceobj, f);
        }

        /* For the rest, treat WHY_RERAISE as WHY_EXCEPTION */

        if (why == WHY_RERAISE)
            why = WHY_EXCEPTION;

        /* Unwind stacks if a (pseudo) exception occurred */

fast_block_end:
        while (why != WHY_NOT && f->f_iblock > 0) {
            /* Peek at the current block. */
            PyTryBlock *b = &f->f_blockstack[f->f_iblock - 1];

            assert(why != WHY_YIELD);
            if (b->b_type == SETUP_LOOP && why == WHY_CONTINUE) {
                why = WHY_NOT;
                JUMPTO(PyInt_AS_LONG(retval));
                Py_DECREF(retval);
                break;
            }

            /* Now we have to pop the block. */
            f->f_iblock--;

            while (STACK_LEVEL() > b->b_level) {
                v = POP();
                Py_XDECREF(v);
            }
            if (b->b_type == SETUP_LOOP && why == WHY_BREAK) {
                why = WHY_NOT;
                JUMPTO(b->b_handler);
                break;
            }
            if (b->b_type == SETUP_FINALLY ||
                (b->b_type == SETUP_EXCEPT &&
                 why == WHY_EXCEPTION) ||
                b->b_type == SETUP_WITH) {
                if (why == WHY_EXCEPTION) {
                    PyObject *exc, *val, *tb;
                    PyErr_Fetch(&exc, &val, &tb);
                    if (val == NULL) {
                        val = Py_None;
                        Py_INCREF(val);
                    }
                    /* Make the raw exception data
                       available to the handler,
                       so a program can emulate the
                       Python main loop.  Don't do
                       this for 'finally'. */
                    if (b->b_type == SETUP_EXCEPT ||
                        b->b_type == SETUP_WITH) {
                        PyErr_NormalizeException(
                            &exc, &val, &tb);
                        set_exc_info(tstate,
                                     exc, val, tb);
                    }
                    if (tb == NULL) {
                        Py_INCREF(Py_None);
                        PUSH(Py_None);
                    } else
                        PUSH(tb);
                    PUSH(val);
                    PUSH(exc);
                }
                else {
                    if (why & (WHY_RETURN | WHY_CONTINUE))
                        PUSH(retval);
                    v = PyInt_FromLong((long)why);
                    PUSH(v);
                }
                why = WHY_NOT;
                JUMPTO(b->b_handler);
                break;
            }
        } /* unwind stack */

        /* End the loop if we still have an error (or return) */

        if (why != WHY_NOT)
            break;
        READ_TIMESTAMP(loop1);

    } /* main loop */

//in ceval.c PyEval_EvalFrameEx end 
int
PyTraceBack_Here(PyFrameObject *frame)
{
    PyThreadState *tstate = PyThreadState_GET();
    PyTracebackObject *oldtb = (PyTracebackObject *) tstate->curexc_traceback;
    PyTracebackObject *tb = newtracebackobject(oldtb, frame);
    if (tb == NULL)
        return -1;
    tstate->curexc_traceback = (PyObject *)tb;
    Py_XDECREF(oldtb);
    return 0;
}
static PyTracebackObject *
newtracebackobject(PyTracebackObject *next, PyFrameObject *frame)
{
    PyTracebackObject *tb;
    if ((next != NULL && !PyTraceBack_Check(next)) ||
                    frame == NULL || !PyFrame_Check(frame)) {
        PyErr_BadInternalCall();
        return NULL;
    }
    tb = PyObject_GC_New(PyTracebackObject, &PyTraceBack_Type);
    if (tb != NULL) {
        Py_XINCREF(next);
        tb->tb_next = next;
        Py_XINCREF(frame);
        tb->tb_frame = frame;
        tb->tb_lasti = frame->f_lasti;
        tb->tb_lineno = PyFrame_GetLineNumber(frame);
        PyObject_GC_Track(tb);
    }
    return tb;
}
typedef struct _traceback {
	PyObject_HEAD
	struct _traceback *tb_next;
	struct _frame *tb_frame;
	int tb_lasti;
	int tb_lineno;
} PyTracebackObject;

//in pythonrun.c
int
PyRun_SimpleFileExFlags(FILE *fp, const char *filename, int closeit,
                        PyCompilerFlags *flags)
{
......
    } else {
        v = PyRun_FileExFlags(fp, filename, Py_file_input, d, d,
                              closeit, flags);
    }
    if (v == NULL) {
        PyErr_Print();
        goto done;
    }
PyRun_FileExFlags-->...->PyEval_EvalFrameEx
PyErr_Print-->PyErr_PrintEx-->PyErr_Fetch
void
PyErr_Fetch(PyObject **p_type, PyObject **p_value, PyObject **p_traceback)
{
    PyThreadState *tstate = PyThreadState_GET();

    *p_type = tstate->curexc_type;
    *p_value = tstate->curexc_value;
    *p_traceback = tstate->curexc_traceback;

    tstate->curexc_type = NULL;
    tstate->curexc_value = NULL;
    tstate->curexc_traceback = NULL;
}
//comment end
case RAISE_VARARGS:
	u = v = w = NULL;
	switch (oparg) {
	case 3:
		u = POP(); /* traceback */
		/* Fallthrough */
	case 2:
		v = POP(); /* value */
		/* Fallthrough */
	case 1:
		w = POP(); /* exc */
	case 0: /* Fallthrough */
		why = do_raise(w, v, u);
		break;
	default:
		PyErr_SetString(PyExc_SystemError,
				   "bad RAISE_VARARGS oparg");
		why = WHY_EXCEPTION;
		break;
	}
	break;
case DUP_TOP:
	v = TOP();
	Py_INCREF(v);
	PUSH(v);
	goto fast_next_opcode;
PREDICTED(END_FINALLY);
case END_FINALLY:
	v = POP();
	if (PyInt_Check(v)) {
		why = (enum why_code) PyInt_AS_LONG(v);
		assert(why != WHY_YIELD);
		if (why == WHY_RETURN ||
			why == WHY_CONTINUE)
			retval = POP();
	}
	else if (PyExceptionClass_Check(v) ||
			 PyString_Check(v)) {
		w = POP();
		u = POP();
		PyErr_Restore(v, w, u);
		why = WHY_RERAISE;
		break;
	}
	else if (v != Py_None) {
		PyErr_SetString(PyExc_SystemError,
			"'finally' pops bad exception");
		why = WHY_EXCEPTION;
	}
	Py_DECREF(v);
	break;

