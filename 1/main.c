#include <stdio.h>
#include <locale.h>
#include <string.h>
#include "long.h"

int main(int argc, char * argv[])
{
	setlocale(LC_ALL, "Rus");

	MyLong a;
	MyLong b;
	MyLong res;
	MyLong m;
	MyLong tmp;

	if (argc < 5 || argc > 7)
	{
		printf("�������� ������ ������!");
		printf("�������� ��������: file1 operation(+, -, / , m) file2 resultfile [-b]");
		printf("���������� � ������� �� ������: file1 file2 p file3 resultfile [-b]");
		getchar();
		return 0;
	}

	char* name_a = argv[1];
	char znak = argv[2][0];
	char* name_b = argv[3];
	char* name_m = "";

	if (znak == 'p')
		name_m = argv[4];

	if (argv[argc - 1][1] == 'b' && argv[argc - 1][0] == '-')
	{
		a = ReadBinFile(name_a);
		if (a.size==0)
			return 0;

		b = ReadBinFile(name_b);
		if (b.size==0)
			return 0;

		if (znak == 'p') 
		{
			m = ReadBinFile(name_m);
			if (m.size==0)
				return 0;
		}
	}
	else
	{
		a = ReadTextFile(name_a);
		if (a.size==0)
			return 0;

		b = ReadTextFile(name_b);
		if (b.size==0)
			return 0;

		if (znak == 'p') 
		{
			m = ReadTextFile(name_m);
			if (m.size==0)
				return 0;
		}
	}

		switch (znak) //��������
		{
		case '+':
			res = Sum(a,b);
			break;

		case '-': res = Sub(a,b);
			break;

		case 'm': res = Mul(a,b);
			break;

		case '/': 
			res = Divide(a,b, &tmp);
			FreeMemory(&tmp);
			break;

		case '%':
			tmp = Divide(a,b, &res);
			FreeMemory(&tmp);
			break;

		case 'p': res = Pow(a,b,m);
			break;

		default:

			printf("�������� ��������.\n '+' - �������� \n '-' - ���������\n 'm' - ���������\n '/' - �������\n 'p' - ���������� � ������� �� ������");
			return 0;
			break;
		}
	

	if (argv[argc - 1]=="-b")
	{
		int k = 4;
		if (znak == 'p')
		{
			k = 5;
		}
		if (k < argc)
		{

			if (WriteBinFile(argv[k],res)==0)
			{
				return 0;
			}
		}
		else
		{
			printf("�������� ������ ������!");
			return 0;
		}
	}
	else
	{
		int k = 4;
		if (znak == 'p')
		{

			k = 5;
		}
		if (k < argc)
		{
			if (WriteTextFile(argv[k],res)==0)
			{
				return 0;
			}
		}
		else
		{
			printf("�������� ������ ������!");
			return 0;
		}
	}

	printf("�������� ��������� �������");

	return 0;
}
