//pythonÔ´Âë½âÎö±Ê¼Ç

/*
s="""
i=1
s="Python"
d={}
l=[]
"""
co=compile(s,"simple_obj.py","exec")
co.co_consts
(1, 'Python', None)
co.co_names
('i', 's', 'd', 'l')

dis.dis(co)
  1           0 LOAD_CONST               0 (1)
              3 STORE_NAME               0 (i)

  2           6 LOAD_CONST               1 ('Python')
              9 STORE_NAME               1 (s)

  3          12 BUILD_MAP                0
             15 STORE_NAME               2 (d)

  4          18 BUILD_LIST               0
             21 STORE_NAME               3 (l)
             24 LOAD_CONST               2 (None)
             27 RETURN_VALUE        

details									stack_pointer			f_locals		
  1           0 LOAD_CONST              [1]						{}
              3 STORE_NAME              [0]						{i=1}

  2           6 LOAD_CONST              ["Python"]				{i=1}
              9 STORE_NAME              []						{i=1,s="Python"}

  3          12 BUILD_MAP               [{}]					{i=1,s="Python"}
             15 STORE_NAME              []						{i=1,s="Python",d={}}

  4          18 BUILD_LIST              [[]]					{i=1,s="Python",d={}}
             21 STORE_NAME             	[]						{i=1,s="Python",d={},l=[]}
             24 LOAD_CONST              [None]					{i=1,s="Python",d={},l=[]}
             27 RETURN_VALUE       		[]						{i=1,s="Python",d={},l=[]}

RETURN_VALUE:return sth after at the end of a code block


s="""
i=1
s="Python"
d={"1":1,"2":2}
l=[1,2]
"""
co=compile(s,"adv_obj.py","exec")
co.co_consts
(1, 'Python', '1', 2, '2', None)
co.co_names
('i', 's', 'd', 'l')
dis.dis(o)
  2           0 LOAD_CONST               0 (1)
              3 STORE_NAME               0 (i)

  3           6 LOAD_CONST               1 ('Python')
              9 STORE_NAME               1 (s)

  4          12 BUILD_MAP                2
             15 LOAD_CONST               0 (1)
             18 LOAD_CONST               2 ('1')
             21 STORE_MAP           
             22 LOAD_CONST               3 (2)
             25 LOAD_CONST               4 ('2')
             28 STORE_MAP           
             29 STORE_NAME               2 (d)

  5          32 LOAD_CONST               0 (1)
             35 LOAD_CONST               3 (2)
             38 BUILD_LIST               2
             41 STORE_NAME               3 (l)
             44 LOAD_CONST               5 (None)
             47 RETURN_VALUE        


s="""
a=5
b=a
c=a+b
print c
"""
co=compile(s,"normal.py","exec")
co.co_consts
(5, None)
co.co_names
('a', 'b', 'c')
dis.dis(co)
  2           0 LOAD_CONST               0 (5)
              3 STORE_NAME               0 (a)

  3           6 LOAD_NAME                0 (a)
              9 STORE_NAME               1 (b)

  4          12 LOAD_NAME                0 (a)
             15 LOAD_NAME                1 (b)
             18 BINARY_ADD          
             19 STORE_NAME               2 (c)

  5          22 LOAD_NAME                2 (c)
             25 PRINT_ITEM          
             26 PRINT_NEWLINE       
             27 LOAD_CONST               1 (None)
             30 RETURN_VALUE        
>>> 
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

