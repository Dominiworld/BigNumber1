#include <stdio.h>
#include <locale.h>
#include <string.h>

#include "2.h"



int main(int argc, char * argv[])
{

	setlocale(LC_ALL, "Rus");


	LONG a;
	LONG b;
	LONG res;
	LONG m;
	LONG ost;


	if (argc < 5 || argc > 7)
	{
		printf("�������� ������ ������!");
		printf("�������� ��������: file1 operation(+, -, / , m, %) file2 resultfile [-b]");
		printf("���������� � ������� �� ������: file1 file2 p file3 resultfile [-b]");
		getchar();
		return 0;
	}

	if (argv[argc - 1][1] == 'b' && argv[argc - 1][0] == '-')
	{
		a.FromBinFile(argv[1]);
		if (a.IsEmpty())
		{
			a.Kill();
			return 0;
		}

		b.FromBinFile(argv[3]);

		if (b.IsEmpty())
		{
			b.Kill();
			return 0;
		}


		if (argv[2][0] == 'p')
		{
			m.FromBinFile(argv[4]);
			if (m.IsEmpty())
			{
				m.Kill();
				return 0;
			}
		}
	}
	else
	{
		a.FromTextFile(argv[1]);



		if (a.IsEmpty())
		{
			a.Kill();
			return 0;
		}

		b.FromTextFile(argv[3]);



		if (b.IsEmpty())
		{
			b.Kill();
			return 0;
		}


		if (argv[2][0] == 'p')
		{
			m.FromTextFile(argv[4]);
			if (m.IsEmpty())
			{
				m.Kill();
				return 0;
			}

		}
	}

	if (strlen(argv[2]) != 1)
	{
		printf("������ �����. �������� - ���� ������!");
		return 0;
	}
	else
	{
		switch (argv[2][0]) //��������
		{
		case '+':
			res = a + b;			
			break;

		case '-': res = a - b;
			break;

		case 'm': res = a*b;
			break;

		case '/': res = a / b;
			break;

		case '%':
			res = a%b;
			break;

		case 'p':
			
			res = PowMod(a, b, m);
			break;

		default:

			printf("�������� ��������.\n '+' - �������� \n '-' - ���������\n 'm' - ���������\n '/' - �������\n 'p' - ���������� � ������� �� ������");
			return 0;
			break;
		}
	}

	if (argv[argc - 1][1] == 'b' && argv[argc - 1][0] == '-')
	{
		int k = 4;
		if (argv[2][0] == 'p')
		{
			k = 5;
		}
		if (k < argc)
		{

			if (!res.ToBinFile(argv[k]))
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
		if (argv[2][0] == 'p')
		{

			k = 5;
		}
		if (k < argc)
		{
			if (!res.ToTextFile(argv[k]))
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
