#include <stdio.h>
#include <locale.h>
#include <string.h>

#include "longClass.h"



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
		printf("Неверный формат строки!");
		printf("Основные операции: file1 operation(+, -, / , m, %) file2 resultfile [-b]");
		printf("Возведение в степень по модулю: file1 file2 p file3 resultfile [-b]");
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
		printf("Ошибка ввода. Операция - один символ!");
		return 0;
	}
	else
	{
		switch (argv[2][0]) //операции
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

			printf("Неверная операция.\n '+' - сложение \n '-' - вычитание\n 'm' - умножение\n '/' - деление\n 'p' - возведение в степень по модулю");
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
			printf("Неверный формат строки!");
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
			printf("Неверный формат строки!");
			return 0;
		}
	}

	printf("Операция выполнена успешно");

	return 0;
}
