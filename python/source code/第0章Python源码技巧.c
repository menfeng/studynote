//pythonԴ������ʼ�

/*
C����

�궨��:
#��ı����滻����������
##��ı����滻����ǰ���ַ�����
do-while(0) #define XXXX(i)      do {a(xxx); b(xxx)} while (0)

�ɱ����:
#ifdef _M_ALPHA
typedef struct {
char *a0;
int offset;
} va_list;
#else
typedef char * va_list;
#endif
_M_ALPHA��ָDEC ALPHA��Alpha AXP���ܹ�������һ�������va_list���������Ϊ�ַ�ָ�롣
INTSIZEOF ��,��ȡ����ռ�õĿռ䳤�ȣ���Сռ�ó���Ϊint����������
#define _INTSIZEOF(n) ( (sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1) )
VA_START�꣬��ȡ�ɱ�����б�ĵ�һ�������ĵ�ַ��ap������Ϊva_list��ָ�룬v�ǿɱ��������ߵĲ�������
#define va_start(ap,v) ( ap = (va_list)&v + _INTSIZEOF(v) )
VA_ARG�꣬��ȡ�ɱ�����ĵ�ǰ����������ָ�����Ͳ���ָ��ָ����һ������t���������˵�ǰ���������ͣ���
#define va_arg(ap,t) ( *(t *)((ap += _INTSIZEOF(t)) - _INTSIZEOF(t)) )
VA_END�꣬���va_list�ɱ�����б�
#define va_end(ap) ( ap = (va_list)0 )

*/
//�ӷ�������
x = (long)((unsigned long)a + b);
((x^a) >= 0 || (x^b) >= 0)��û�����(����λ����������ͬ�������)

//��ȡtype�ṹ����member��ƫ����
#define offsetof(type, member) ((size_t)(&((type *)0)->member))

