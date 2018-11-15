#include <iostream>
#include <sstream>
#include <vector>
#include <cstring>
#include <ctime>

#include <stdio.h>

using namespace std;

class A
{
  public:
	static void test1 ()
	{
		printf ("test\n");
	};

	typedef void (*PFunc) ();
};

A::PFunc f = &(A::test1);

int main (int argc, char const *argv[])
{
	/** 测试char*转string */
	const char *abc = "abc";
	string buf (abc, abc + strlen (abc));

	std::cout << "abc: " << abc << ", " << buf << ", " << buf.size () << endl;

	/** 测试std::move() */

	std::string str = "Hello";
	std::vector < std::string > v;
	//调用常规的拷贝构造函数，新建字符数组，拷贝数据
	v.push_back (str);
	std::cout << "After copy, str is \"" << str << "\"\n";
	//调用移动构造函数，掏空str，掏空后，最好不要使用str
	v.push_back (std::move (str));
	std::cout << "After move, str is \"" << str << "\"\n";
	std::cout << "The contents of the vector are \"" << v[0] << "\", \"" << v[1] << "\"\n";

	/** 测试结构体赋值 */

	typedef struct Struct_T
	{
		int a;
		int b;
	} Struct_T;

	Struct_T d;

	d.a = 123;

	d.b = 456;

	Struct_T e;
	e = d;

	cout << e.a << "," << e.b << endl;

	/** 测试正数与小数的比较 */
	int a = 1;
	printf ("1equal1.0: %d\n", 1 == 1.0);

	/** test memcpy size */

	const char *str1 = "abc";
	char *str2 = new char[3];
	memcpy (str2, str1, 3);

	std::cout << str1 << ", strlen(str1): " << strlen (str1) << std::endl;
	std::cout << str2 << ", strlen(str2): " << strlen (str2) << std::endl;

	(*f) ();

	/** ctime时间测试 */
	std::time_t t = std::time (0);
	printf ("%ld seconds since 01-Jan-1970\n", t);

	/** unsigned char *指针 输出 */
	unsigned char buf1[16] = { 0 };
	memcpy (buf1, "abc", 3);

	ostringstream o;
	o << buf1;

	cout << o.str () << endl;

	int buf2[3];
	printf("%p, %p, %p\n", buf2, buf2+1, buf2+2);

	return 0;
}
