#include <stdio.h>
#include <stdlib.h> 
#include <memory.h>
#include <string.h>
#include <locale.h>


struct MyLong
{
	unsigned int* pointer;
	unsigned int size;

	void SetMemory(unsigned int s)
	{
		size = s;
		pointer = (unsigned int*)malloc(sizeof(unsigned int)*size);
		memset(pointer, 0, sizeof(unsigned int)*size);
	}
	
	void Copy(MyLong a)
	{
		for (int i = 0; i < a.size; i++)
		{
			pointer[i] = a.pointer[i];
		}
	}

	void FreeMemory()
	{
		free(pointer);
	}
};

///Большие
MyLong Sum(MyLong a, MyLong b);
MyLong Sub(MyLong a, MyLong b);
MyLong Mul(MyLong a, MyLong b);
MyLong Divide(MyLong a, MyLong b, MyLong &mod);
MyLong Pow(MyLong a, MyLong b, MyLong m); //использует smallPow, Pow32
int Compare(MyLong a, MyLong b);

///Маленькие
MyLong ShortSum(MyLong a, unsigned int b);
MyLong ShortSub(MyLong a, unsigned int b);
MyLong ShortMul(MyLong a, unsigned int b);		
MyLong ShortDivide(MyLong a, unsigned int b, unsigned int &ost);
MyLong smallPow(MyLong a, unsigned int p, MyLong m); //a^p mod m
int ShortCompare(MyLong a, unsigned int b);

//Вспомогательные
MyLong Pow32(MyLong a, MyLong m);  //a^(2^32) mod m
void ShiftLeft(MyLong &a, unsigned int i); //сдвиг на i элементов массива 
MyLong Normalize(MyLong a);			//уменьшение размера a за счет убирания незначащих нулей

//Работа с файлами
MyLong ReadTextFile(char* file);
MyLong ReadBinFile(char* file);
bool WriteBinFile(char* file, MyLong number);
bool WriteTextFile(char* file, MyLong number);


int main(int argc, char * argv[])
{
	setlocale(LC_ALL,"Rus");

	MyLong a;
	MyLong b;
	MyLong res;
	MyLong m;


	
	if ( argc < 5 || argc > 7)
	{
		printf("Неверный формат строки!");
		printf("Основные операции: file1 operation(+, -, / , *) file2 resultfile [-b]");
		printf("Возведение в степень по модулю: file1 file2 p file3 resultfile [-b]");
		return 0;
	}

	if (argv[argc - 1][1] == 'b' && argv[argc - 1][0] == '-')
	{	

		a = ReadBinFile(argv[1]);
		if (a.size == 0)
		{
			a.FreeMemory();
			
			return 0;
		}

		b = ReadBinFile(argv[3]);
		if (b.size == 0)
		{
			b.FreeMemory();
			return 0;
		}

		if (argv[2][0] == 'p')
		{
			m = ReadBinFile(argv[4]);
			if (m.size == 0)
			{
				m.FreeMemory();
				return 0;
			}
		}
	}
	else 
	{
		a = ReadTextFile(argv[1]);
		if (a.size == 0)
		{
			a.FreeMemory();
			return 0;
		}
		b = ReadTextFile(argv[3]);
		if (b.size == 0)
		{
			b.FreeMemory();
			return 0;
		}
		if (argv[2][0] == 'p')
		{
			m = ReadTextFile(argv[4]);
			if (m.size == 0)
			{
				m.FreeMemory();
				return 0;
			}
		}
	}
	
	if (strlen(argv[2])!=1)
	{
		printf("Ошибка ввода. Операция - один символ!");
		return 0;
	}
	else
	{
		switch (argv[2][0]) //операции
		{
		case '+': res = Sum(a, b);
			break;

		case '-': res = Sub(a, b);
			break;

		case '*': res = Mul(a,b);
			break;

		case '/': res = Divide(a, b, m);
			break;

		case '%': Divide(a, b, res);
			break;

		case 'p': res = Pow(a, b, m);
			break;

		default:

			printf("Неверная операция.\n '+' - сложение \n '-' - вычитание\n '*' - умножение\n '/' - деление\n 'p' - возведение в степень по модулю");
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
			if (!WriteBinFile(argv[k], res))
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
			if (!WriteTextFile(argv[k], res))
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

MyLong Sum(MyLong a, MyLong b)
{
	MyLong maxnumber = a;
	MyLong minnumber = b;

	if (a.size < b.size)
	{
		maxnumber = b;
		minnumber = a;
	}

	MyLong result;
	result.SetMemory(maxnumber.size + 1);

	for (int i = 0; i < maxnumber.size; i++)
	{
		result.pointer[i] = maxnumber.pointer[i];
	}

	_asm
	{
		clc
			pushf
			mov esi, result.pointer
			mov edi, minnumber.pointer
			mov ecx, minnumber.size
		m : mov eax, [esi]
			popf
			adc eax, [edi]
			pushf
			mov[esi], eax
			add esi, 4
			add edi, 4
			loop m
			mov eax, [esi]
			popf
			adc eax, 0
			mov[esi], eax
			jnc ex
			dec result.size
		ex :
	}

	return Normalize(result);
}
MyLong Sub(MyLong a, MyLong b)
{
	MyLong maxnumber = a;
	MyLong minnumber = b;

	if (b.size>a.size)
	{
		maxnumber = b;
		minnumber = a;
	}
	MyLong result;
	result.SetMemory(maxnumber.size);
	result.Copy(maxnumber);

	_asm
	{
		clc
			pushf
			mov esi, result.pointer
			mov edi, minnumber.pointer
			mov ecx, minnumber.size
		m : mov eax, [esi]
			popf
			sbb eax, [edi]
			pushf
			mov[esi], eax
			add esi, 4
			add edi, 4
			loop m
			mov eax, [esi]
			popf
			sbb eax, 0
			mov[esi], eax
	}

	return  Normalize(result);
}
MyLong Mul(MyLong a, MyLong b)
{
	MyLong res;
	res.SetMemory(a.size + b.size);
	int cf = 0;
	int bs = b.size * 4;
	_asm
	{
			mov esi, a.pointer
			mov ebx, res.pointer
			mov ecx, a.size

		m1 : push ecx
			 mov edi, b.pointer
			 mov ecx, b.size
		 m2 : mov eax, [esi]
			  mov edx, [edi]
			  mul edx

			 add[ebx], eax
			 adc edx, cf
			 jnc m3
			 mov cf, 1
			  jmp m4
		  m3 : mov cf, 0
		   m4 : add[ebx + 4], edx
				jnc m5
				mov cf, 1

			m5 : add ebx, 4
				 add edi, 4

				 loop m2

				 pop ecx
				 sub ebx, bs

				 add esi, 4
				 add ebx, 4
				 loop m1
	}

	return Normalize(res);
}
MyLong Divide(MyLong a, MyLong b, MyLong &mod)
{
	
	MyLong res;

	if (b.size == 1)
	{
		mod.SetMemory(1);
		res = ShortDivide(a, b.pointer[0], mod.pointer[0]);
		return res;
	}

	if (ShortCompare(a,0) == 0)
	{
		printf("Деление на 0!");
		res.SetMemory(1);
		mod.SetMemory(1);
		mod.size = 0;
		res.size = 0;
		return res;
	}
	if (Compare(a,b) == -1)
	{
		res.SetMemory(1);
		mod.SetMemory(a.size);
		mod.Copy(a);
	}
	
	res.SetMemory(a.size - b.size + 1);
	MyLong A;
	A.SetMemory(a.size);
	A.Copy(a);

	MyLong tmp;

	int i = res.size - 1;
	for(i = res.size - 1; i >-1; i--)
	{
			unsigned int x = 0;
			unsigned long long low = 0;
			unsigned long long up = 4294967295;
			while (up - low >1)
			{
				unsigned long long d = (low + up) >> 1;
				tmp = ShortMul(b, d);
				ShiftLeft(tmp, i);
				if (Compare(A,tmp)!= -1)
				{
					x = d;
					low = d;
				}
				else
				{
					up = d;
				}
				tmp.FreeMemory();
			}
			res.pointer[i] = x;
			tmp = ShortMul(b, x);
			ShiftLeft(tmp, i);
			A = Sub(A, tmp);
			tmp.FreeMemory();
	}

	mod = Normalize(A);
	return Normalize(res);
}
MyLong Pow(MyLong a, MyLong p, MyLong m)
{
	MyLong res;
	res.SetMemory(1);
	res.pointer[0] = 1;

	MyLong ost;
	MyLong tmp;

	if (Compare(a, m) != -1) Divide(a, m, a);

	for (int i = p.size - 1; i > -1; i--)
	{
		tmp = Divide(Mul(Pow32(res, m), smallPow(a, p.pointer[i], m)), m, res);
		tmp.FreeMemory();
		tmp = Divide(Normalize(res), m, res);
		tmp.FreeMemory();
	}
	return res;
}
int Compare(MyLong a, MyLong b) //-1:a<b 1:a>b 0:a=b 
{
	if (a.size<b.size) return -1;

	if (a.size > b.size) return 1;

	for (int i = a.size - 1; i > -1; i--)
	{
		if (a.pointer[i] > b.pointer[i]) return 1;
		if (a.pointer[i] < b.pointer[i]) return -1;
	}
	return 0;
}

MyLong ShortSum(MyLong a, unsigned int b)
{
	MyLong res;
	res.SetMemory(a.size+1);
	res.Copy(a);

	_asm
	{
		clc
		mov edi, res.pointer
		mov ebx,b
		add[edi], ebx
		pushf
		add edi, 4
		
		mov ecx, res.pointer
m:		popf
		adc[edi], 0
		pushf
		jnc ex
		add edi, 4
		loop m		
ex:		popf
	}
	return Normalize(res);
}
MyLong ShortSub(MyLong a, unsigned int b)
{
	MyLong res;
	res.SetMemory(a.size);
	res.Copy(a);
	_asm
	{		clc
			mov edi, res.pointer
			mov ebx, b
			sub[edi], ebx
			pushf
			add edi, 4

			mov ecx, res.size
		m : popf
			sbb[edi], 0
			pushf
			jnc ex
			add edi, 4
			loop m
		ex :popf 
	}
	WriteBinFile("res11.bin", res);
	return Normalize(res);

}
MyLong ShortDivide(MyLong a, unsigned int b, unsigned int &ost)
{
	MyLong res;
	if (!b)
	{
		printf("Деление на 0!");
		res.SetMemory(1);
		ost = 0;
		res.size = 0;
		return res;
	}

	if (ShortCompare(a, b) == -1)
	{
		ost = a.pointer[0];
		res.SetMemory(1);
		return res;
	}

	res.SetMemory(a.size);

	unsigned int s = (a.size - 1)*sizeof(int);
	unsigned int k = 0;
	_asm
	{
			mov esi, a.pointer
			add esi, s
			mov edi, res.pointer
			add edi, s
			mov ecx, a.size
			xor edx, edx

		m : mov eax, [esi]
			div b
			mov[edi], eax
			sub esi, 4
			sub edi, 4
			loop m
			mov k, edx
	}
	ost = k;
	return Normalize(res);
}
MyLong ShortMul(MyLong a, unsigned int b)
{
	MyLong res;
	res.SetMemory(a.size + 1);
		_asm
		{
			mov esi, a.pointer
			mov ecx, a.size
			mov ebx, res.pointer

m:			mov eax, [esi]
			mul b
			add[ebx], eax
			adc[ebx+4], edx
			adc[ebx+8], 0
			add esi,4
			add ebx,4
			loop m
		}
	return Normalize(res);
}
MyLong smallPow(MyLong a, unsigned int p, MyLong m)
{
	MyLong res;
	res.SetMemory(1);
	res.pointer[0] = 1;

	char c[32]; //двоичное представление числа p
	memset(c, 0, 32);
	int k = 0; //определяет наибольший ненулевой элемент в c

	for (int i = 0; i < 32; i++)
	{
		c[i] = p % 2;
		p /= 2;
		if (p == 0)
		{
			k = i;
			break;
		}
	}

	for (int i = k; i >= 0; i--)
	{
		Divide(Mul(res, res), m, res);
		if (c[i] == 1) Divide(Mul(res, a), m, res);
	}
	return res;
}
int ShortCompare(MyLong a, unsigned int b)
{
	if (a.size > 1) return 1;
	if (a.size == 0) return -1;
	if (a.pointer[0] > b) return 1;
	if (a.pointer[0] < b) return -1;
	return 0;
}

void ShiftLeft(MyLong &a, unsigned int i)
{
	MyLong a1;

	a1.SetMemory(a.size + i);

	for (int j = i; j < a.size + i; j++)
		a1.pointer[j] = a.pointer[j - i];

	a.FreeMemory();
	a = a1;
	a.size = a1.size;

}
MyLong Normalize(MyLong a)
{
	while (a.pointer[a.size - 1] == 0) a.size--;

	if (a.size == 0) a.size = 1;

	MyLong result;
	result.SetMemory(a.size);
	
	for (size_t i = 0; i < a.size; i++)
		result.pointer[i] = a.pointer[i];
	

	a.FreeMemory();
	return result;
}
MyLong Pow32(MyLong a, MyLong m)
{
	MyLong res;
	res.SetMemory(a.size);
	res.Copy(a);

	MyLong tmp;

	for (int i = 0; i < 32; i++)
	{
		tmp = Divide(Mul(res, res), m, res);
		tmp.FreeMemory();
	}
	return res;
}

bool WriteBinFile(char* file, MyLong number)
{
	FILE* out;
	if ((out = fopen(file,"wb")) != NULL)
	{
		for (int i = number.size - 1; i > -1; i--)
		{
			fwrite(&number.pointer[i], sizeof(unsigned int*), 1, out);
		}
		fclose(out);
		return true;
	}

	printf("Не удалось открыть файл %s!", file);
	return false;
	
}
MyLong ReadTextFile(char* file)
{
	FILE* in;
	
	int size = 0;

	MyLong res;
	res.SetMemory(1);

	if ((in = fopen(file, "r")) != NULL)
	{
		while (fgetc(in) != EOF)
		{
			size++;
		}

		if (size == 0)
		{
			res.SetMemory(1);
			res.size = 0;
			return res;
		}

		fseek(in, 0, 0);

		MyLong k;
		k.SetMemory(1);
		k.pointer[0] = 1;

		MyLong tmp1;
		
		for (int l = 0; l < size; l++)
		{				
			tmp1 = ShortMul(k, 10);
			k.FreeMemory();
			k = tmp1;
		}

		MyLong tmp;
		tmp.SetMemory(1);

		unsigned int ost = 0;
		char ch;
		unsigned int h = 0;
		while ((ch = fgetc(in))!=EOF)
		{
			if ((ch - '0' > 9) || (ch - '0' < 0))
			{
				printf("Файл %s содержит не только цифры!");
				res.size = 0;
				return res;
			}

			k = ShortDivide(k, 10, ost);

			tmp.pointer[0] = ch - '0';
			res = Sum(res, Normalize(Mul(tmp, k)));	
		}
		
		fclose(in);

		k.FreeMemory();
		tmp.FreeMemory();

	}
	else
	{
		printf("Не удалось открыть файл %s!", file);
		res.size = 0;		
	}

	return res;
}
bool WriteTextFile(char* file, MyLong number)
{
	FILE* out;

	MyLong numcopy;
	numcopy.SetMemory(number.size);
	numcopy.Copy(number);

	MyLong tmp;
	tmp.SetMemory(1);

	//считаем количество элементов выходного массива
	unsigned int ost;
	unsigned int k = 1;
	while (ShortCompare(numcopy, 0) == 1)
	{
		numcopy = ShortDivide(numcopy, 1000000000, ost);
		k++;
	}
	if (k != 1) k--;
	MyLong res;
	res.SetMemory(k);
	
	int i = 0;
	while (ShortCompare(number, 0) == 1)
	{
		
		number = ShortDivide(number, 1000000000, ost);
		res.pointer[i] = ost;
		i++;
	}

	res = Normalize(res);
	if ((out = fopen(file, "w")) != NULL)
	{
		fprintf(out, "%u", res.pointer[res.size - 1]);

		for (int i = res.size - 2; i > -1; i--)
		{
			//нужно предусмотреть тот случай, когда количество цифр < 9	
			char buffer[10] = "000000000";
			int k = 0;
			while (res.pointer[i] > 0)
			{
				buffer[k++] = res.pointer[i] % 10 + '0';
				res.pointer[i] /= 10;
			}
			for (int i = 8; i > -1; i--)
			{
				fprintf(out, "%c", buffer[i]);
			}
		}
		fclose(out);
		numcopy.FreeMemory();
		tmp.FreeMemory();
		res.FreeMemory();
		
		return true;
	}
	numcopy.FreeMemory();
	tmp.FreeMemory();
	res.FreeMemory();
		printf("Не удалось открыть файл %s!", file);
		return false;

} 
MyLong ReadBinFile(char* file)
{
	FILE* in;
	MyLong number;
	int size = 0;
	
	if ((in = fopen(file, "rb")) != NULL)
	{
		while (fgetc(in) != EOF)
		{
			size++;
		}

		if (size == 0)
		{
			printf("Файл %s пустой!", file);
			number.SetMemory(1);
			number.size = 0;
			return number;
		}

		number.SetMemory((size - 1) / sizeof(unsigned int)+1);

		fseek(in, 0, 0);

		for (int i = number.size - 1; i >-1; i--)
		{
			fread(&number.pointer[i], sizeof(unsigned int), 1, in);
		}

		fclose(in);
	}
	else
	{
		printf("Не удалось открыть файл %s!", file);
		number.SetMemory(1);
		number.size = 0;
	}
	return number;
}
	


