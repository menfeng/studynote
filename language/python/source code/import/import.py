1.闭包
	引用外部函数作用域变量的内部函数叫做闭包
	变量叫做自由变量
	co_cellvars：内部函数的自由变量名
	co_freevars：自由变量的值

==1.解释code中的各个作用域|names,stacksize,flags
>>> sText="""
class A(object):
	pass

def F(a,b=1):
	m=1
	def G():
		print m,id(m)
	print G.func_code.co_freevars
	print G.func_closure

F()
"""
>>> code=compile(sText,"test.py","exec")
>>> code.co_consts
('A', <code object A at 00000000028DC030, file "test.py", line 2>, 1, <code object F at 00000000028D1B30, file "test.py", line 5>, None)
>>> code.co_names
('object', 'A', 'F')
>>> fcode=code.co_consts[3]
>>> gcode=fcode.co_consts[2]
>>> gcode.co_freevars
('m',)
>>> gcode.co_cellvars
()

>>> fcode.co_argcount
2
>>> fcode.co_nlocals
3
>>> fcode.co_stacksize
2
>>> fcode.co_flags
3
>>> fcode.co_code
'd\x01\x00\x89\x00\x00\x87\x00\x00f\x01\x00d\x02\x00\x86\x00\x00}\x02\x00|\x02\x00j\x00\x00j\x01\x00GH|\x02\x00j\x02\x00GHd\x00\x00S'
>>> fcode.co_consts
(None, 1, <code object G at 00000000025048B0, file "test.py", line 7>)
>>> fcode.co_names
('func_code', 'co_freevars', 'func_closure')
>>> fcode.co_varnames
('a', 'b', 'G')1
>>> fcode.co_freevars
()
>>> fcode.co_cellvars
('m',)
>>> fcode.co_filename
'test.py'
>>> fcode.co_firstlineno
5
>>> fcode.co_name
'F'
>>> fcode.co_lnotab
'\x00\x01\x06\x01\x0f\x02\x0b\x01'


2.作用
	1.逻辑相关放一起，方便复用
	2.划分命名空间

3.加载分类
	1.主模块的加载 	python main.py
	2.普通模块的加载 import

4.作用域
	1.文本定义的静态作用域 运行是变为名字空间
