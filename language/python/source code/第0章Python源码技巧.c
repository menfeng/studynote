//python源码解析笔记

/*
C基础

宏定义:
#后的变量替换后会加上引号
##后的变量替换后与前面字符连接
do-while(0) #define XXXX(i)      do {a(xxx); b(xxx)} while (0)

可变参数:
#ifdef _M_ALPHA
typedef struct {
char *a0;
int offset;
} va_list;
#else
typedef char * va_list;
#endif
_M_ALPHA是指DEC ALPHA（Alpha AXP）架构。所以一般情况下va_list所定义变量为字符指针。
INTSIZEOF 宏,获取类型占用的空间长度，最小占用长度为int的整数倍：
#define _INTSIZEOF(n) ( (sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1) )
VA_START宏，获取可变参数列表的第一个参数的地址（ap是类型为va_list的指针，v是可变参数最左边的参数）：
#define va_start(ap,v) ( ap = (va_list)&v + _INTSIZEOF(v) )
VA_ARG宏，获取可变参数的当前参数，返回指定类型并将指针指向下一参数（t参数描述了当前参数的类型）：
#define va_arg(ap,t) ( *(t *)((ap += _INTSIZEOF(t)) - _INTSIZEOF(t)) )
VA_END宏，清空va_list可变参数列表：
#define va_end(ap) ( ap = (va_list)0 )

*/
//加法溢出检测
x = (long)((unsigned long)a + b);
((x^a) >= 0 || (x^b) >= 0)则没有溢出(符号位与两个都不同则溢出了)

//获取type结构体中member的偏移量
#define offsetof(type, member) ((size_t)(&((type *)0)->member))

