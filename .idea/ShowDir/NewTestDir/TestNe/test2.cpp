#include "iostream"
#include <fstream.h>


class foox
{
	int *a, **b;
	int *c;
	int **d;

	struct tm gmtime (); 
	struct tm *gmtime2 (); 
	const struct tm gmtime3 (); 
	const struct tm *gmtime4 (); 
	struct stat {};
	struct stat archive_stat;
	static struct stat archive_stat2;
	union block{};
	union block record_start;
	union block *record_start2;
	//	union block *record_start3 {1,2,3};
	enum access_mode access_mode;

};






class cls1
{

	struct Shape1
	{
		int x;
	};
	Shape1 s1;

	struct Shape2
	{
		int x;
	} s,ss,sss;

	Shape1 s2;
	struct Shape1 s3;



	union Uni1
	{
		int x;
	};
	Uni1 u1;

	union Uni2
	{
		int x;
	} u,uu,uuu;

	Uni1 u2;
	union Uni1 u3;



	enum En1
	{
		x = 3
	};
	En1 en1;

	enum En2
	{
		y = 4
	} en,enn,ennn;

	En1 en2;
	enum En1 en3;




	class Cls1
	{
		int x;
	};
	Cls1 cl0;

	class Cls3
	{
	};

	const class Cls2 : Cls1, Cls3
	{
		double y;
	} cl1, cl2, cl3;

	Cls2 cl4;
	class Cls2 cls5;


	class cls12
	{
	} cl12;
	void vfunc()
	{

		struct Shape1
		{
			int x;
		};

		struct Shape2
		{
			int x;
		} s;

		struct Shape1 s1;

	}

	bool ISALPHA(int)
	{
		return 0;
	}

	void fff()
	{


		int buff[7],c,yyInput;
		int** b = new (int*);
	}
	void foo3(int());
	void foo(int a, int b)
	{
		if (a=3, a=(6==7))
		{
			int b;
			int y = a, z = 4;
		}
	};
	int* a (int *a, int b)
	{
		if (*a=3, a=(6==7))
		{
			int b;
			int y = *a, z = 4;
		}
	} ;

	void fooo(int);
	void foo(int(int, int));
	void foo0(void a(int* i, int));
	void foo1(void (*a)(int* i, int));


};
class cls
{
public:
	cls(int y);

	int aa (int *a, int b)
	{
		if (*a=3, a=(6==7))
		{
			int b;
			int y = *a, z = 4;
		}
	} ;
	void foo1(int*(int, int));
	void foo0(void (*a)(int* i, int));
	void foo2(int a);
	void foo2(cls(int* i, int));
	void foo0(int (*a)(int* i, int));

};
class cls10 : cls
{
public:
	cls10() : cls(5){}
	;
	static int a,b,c,d;
	static short foo(int...)
	{
		bool b2;
		__int16 y = a, z = 4;
	}
};

