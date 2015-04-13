
#include <stdio.h>
#include <stdlib.h> 
#include <memory.h>
#include <string.h>
#include <math.h>
#include <string.h>
#include <iostream>

using namespace std;


struct MyLong
{
	unsigned long long* pointer;
	unsigned long long size;

	void SetMemory(unsigned long long s)
	{
		size = s;
		pointer = (unsigned long long*)malloc(sizeof(unsigned long long)*size);
		memset(pointer, 0, sizeof(unsigned long long)*size);
	}
	
	void Copy(MyLong a)
	{
		for (int i = 0; i < a.size; i++)
		{
			pointer[i] = a.pointer[i];
		}
	}
};

MyLong ReadTextFile(char* file);
MyLong ReadBinFile(char* file);
bool WriteBinFile(char* file, MyLong number);
bool WriteTextFile(char* file, MyLong number); 
MyLong Summ(MyLong a, MyLong b);
MyLong Sub(MyLong a, MyLong b);
MyLong Mul(MyLong a, MyLong b);
int Compare(MyLong a, MyLong b);
int ShiftedCompare(MyLong a, MyLong b);
MyLong Divide(MyLong a, MyLong b, MyLong &ost);
MyLong Mod(MyLong a, MyLong b);
MyLong smallPow(MyLong a,unsigned long long p, MyLong m);
MyLong Pow(MyLong a, MyLong b, MyLong m);
MyLong Pow32(MyLong a, MyLong m);
MyLong Normalize(MyLong a);
void ShiftedSub(MyLong a, MyLong b);


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
		printf("Основные операции: file1 operation(+, -, / , *) file2 resultfile[-b]");
		printf("file1 operation(+,-,/,*) file2 resultfile [-b]");
		return 0;
	}

	if (argv[argc - 1][1] == 'b' && argv[argc - 1][0] == '-')
	{	

		a = ReadBinFile(argv[1]);
		if (a.size == 0)
		{
			free(a.pointer);
			return 0;
		}

		b = ReadBinFile(argv[3]);
		if (b.size == 0)
		{
			free(b.pointer);
			return 0;
		}

		if (argv[2][0] == 'p')
		{
			m = ReadBinFile(argv[4]);
			if (m.size == 0)
			{
				free(m.pointer);
				return 0;
			}
		}
	}
	else
	{
		a = ReadTextFile(argv[1]);
		if (a.size == 0)
		{
			free(a.pointer);
			return 0;
		}
		b = ReadTextFile(argv[3]);
		if (b.size == 0)
		{
			free(b.pointer);
			return 0;
		}
		if (argv[2][0] == 'p')
		{
			m = ReadTextFile(argv[4]);
			if (m.size == 0)
			{
				free(m.pointer);
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
		switch (argv[2][0]) 
		{
		case '+': res = Summ(a, b);
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



int Compare(MyLong a, MyLong b) //-1:a<b 1:a>b 0:a=b 
{
	if (a.size<b.size)
	{
		return -1;
	}
	if ((a.size > b.size))
	{
		return 1;
	}

	for (int i = a.size - 1; i > -1; i--)
	{
		if (a.pointer[i] > b.pointer[i])
		{
			return 1;
		}
		else if (a.pointer[i] < b.pointer[i])
		{
			return -1;
		}
	}
	return 0;

}


bool ShiftLeft(MyLong a)
{
	int k = 0;
	asm("movq  %0, %%rdi\n"
		"movq  %1, %%rcx\n"
		"clc\n"
		"pushf\n"
		"m : movq  (%%rdi), %%rax\n"
		"popf\n"
		"rcl  $1, %%rax\n"
		"pushf\n"
		"movq %%rax,(%%rdi) \n"
		"addq $8,%%rdi \n"
		"loop m\n"
		"popf\n"
		"jnc ex\n"
		"movl $1, %k \n"
		"ex:\n"
		:
		: "r" (a.pointer), "r" (a.size), "r"(k)
		: "rdi", "rcx", "rax");

	a = Normalize(a);
	return k == 1;
}
void ShiftRight(MyLong a)
{
	unsigned long long tmp = (a.size - 1) * sizeof(unsigned long long);
	asm(
		"movq %0,%%rdi\n"
		"addq %2,%%rdi\n"
		"p:	 movq %1, %%rcx \n"
		"clc\n"
		"pushf\n"
		"m : movq  (%%rdi), %%rax\n"
		"popf\n"
		"rcr $1, %%rax\n"
		"pushf\n"
		"movq  %%rax,(%%rdi)\n"
		"subq $8, %%rdi \n"
		"loop m\n"
		"popf\n"
		:
		: "r"(a.pointer), "r"(a.size), "r"(tmp)
		: "rdi", "rcx", "rax"
		);
}
void ShiftedSub(MyLong a, MyLong b)
{
	unsigned long long tmp = (a.size - b.size)*sizeof(unsigned long long);

	asm(
		"clc\n"
		"pushf\n"
		"movq  %0, %%rsi\n"
		"addq  %1, %%rsi\n"
		"movq  %2, %%rdi\n"
		"movq  %3, %%rcx\n"
		"m : movq  (%%rsi), %%rax\n"
		"popf\n"
		"sbbq  (%%rdi), %%rax\n"
		"pushf\n"
		"movq %%rax, (%%rsi)\n"
		"addq $8, %%rsi\n"
		"addq $8, %%rdi\n"
		"loop m\n"
		"popf\n"
		:
		: "r"(a.pointer), "r"(tmp), "r"(b.pointer), "r"(b.size)
		: "rsi", "rdi", "rcx", "rax"
		);
}
int ShiftedCompare(MyLong a, MyLong b)
{
	for (size_t i = 0; i < b.size; i++)
	{
		if (a.pointer[a.size - 1 - i] > b.pointer[b.size - 1 - i])
		{
			return 1;
		}
		else if (a.pointer[a.size - 1 - i] < b.pointer[b.size - 1 - i])
		{
			return -1;
		}
	}
	return 0;
}
MyLong Normalize(MyLong a)
{
	while (a.pointer[a.size-1] == 0)
	{
		a.size--;
	}
	if (a.size == 0) a.size = 1;

	MyLong result;
	result.SetMemory(a.size);
	
	for (size_t i = 0; i < a.size; i++)
	{
		result.pointer[i] = a.pointer[i];
	}

	//free(a.pointer);
	return result;
}
MyLong Divide(MyLong a, MyLong b, MyLong &ost)
{
	a = Normalize(a);
	b = Normalize(b);

	MyLong null;
	null.SetMemory(1); 

	if (Compare(a,b) == -1)
	{
		ost.SetMemory(a.size);	
		for (int i = 0; i < ost.size; i++)
		{
			ost.pointer[i] = a.pointer[i];
		}
		return null;
	}

	MyLong result;
	result.SetMemory(a.size - b.size + 1); 
	MyLong A;
	A.SetMemory(a.size);		
	for (size_t i = 0; i < A.size; i++)
	{
		A.pointer[i] = a.pointer[i];
 	}
	MyLong B;
	B.SetMemory(b.size+1); 
	for (size_t i = 0; i < b.size; i++)
	{
		B.pointer[i+1] = b.pointer[i];
	}

	while ((B.pointer[B.size - 1] <= A.pointer[A.size - 1]) && (B.pointer[B.size - 1] & 0x8000000000000000) != 0x8000000000000000) //А что если вылезет?? Доделать
	{
		ShiftLeft(B);
	}
	if (B.pointer[B.size - 1] > A.pointer[A.size - 1])
	{
		ShiftRight(B);
	}

	while (Compare(A, b) != -1 || ShiftedCompare(B,b)!=-1)
	{
		if (ShiftedCompare(A, B) != -1)
		{
			result.pointer[0]++;
			ShiftedSub(A, B);

		}
		ShiftLeft(result);
		ShiftRight(B);


		if (A.size == 1 && A.pointer[0] == 0 && B.pointer[0]==0)
		{
			for (int i = 0; i < B.size - 1; i++)
			{
				B.pointer[i] = B.pointer[i + 1];
			}
			B.pointer[B.size - 1] = 0;
			B = Normalize(B);
		}

		if (B.pointer[B.size - 1] == 0 && A.pointer[A.size - 1] == 0)
		{
			A.size--;

			if (A.size == B.size - 1)
			{
				B = Normalize(B);
			}
			else
			{
				for (unsigned long long i = B.size - 1; i > 0; i--)
				{
					B.pointer[i] = B.pointer[i - 1];
				}
				B.pointer[0] = 0;
			}		
		}
	}
	ShiftRight(result); 

	ost = Normalize(A);

	free(null.pointer);
	free(A.pointer);
	free(B.pointer);
		
	return Normalize(result);
}
MyLong Pow(MyLong a, MyLong p, MyLong m)
{
	MyLong res;
	res.SetMemory(1);
	res.pointer[0] = 1;

	MyLong ost;
	MyLong tmp;

	if (Compare(a, m) != -1)
	{
		Divide(a, m, a);
	}

	for (int i = p.size - 1; i > -1; i--)
	{	
		tmp = Divide(Mul(Pow32(res, m), smallPow(a, p.pointer[i], m)), m, res);
		free(tmp.pointer);
		tmp = Divide(Normalize(res), m, res);
		free(tmp.pointer);
	}
	return res;
}
MyLong Pow32(MyLong a, MyLong m)
{
	MyLong res;
	res.SetMemory(a.size);
	res.Copy(a);

	MyLong tmp;

	for (int i = 0; i < 64; i++)
	{
		tmp = Divide(Mul(res, res), m, res);
		free(tmp.pointer);
	}
	return res;
}
MyLong smallPow(MyLong a,unsigned long long p, MyLong m)
{
	MyLong res;
	res.SetMemory(1);
	res.pointer[0] = 1;

	char c[64]; //двоичное представление числа p
	int k = 0; //определяет наибольший ненулевой элемент в c

	for (int i = 0; i < 64; i++)
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

		if (c[i] == 1)
		{
			Divide(Mul(res, a), m, res);
		}
	}
	return res;
}

MyLong Mul(MyLong a, MyLong b)
{
	MyLong result;
	result.SetMemory(a.size + b.size);

	MyLong tmp;			
	tmp.SetMemory(2);

	unsigned long long carry = 0;

	for (int i = 0; i < a.size; i++)
	{
		carry = 0;

		for (int j = 0; j < b.size; j++)
		{
			int ik = i*sizeof(unsigned long long);
			int jk = j*sizeof(unsigned long long);
			//tmp = a.pointer[i] * b.pointer[j] + carry + result.pointer[i + j];
			asm(
				"movq %0, %%rsi\n"
				"movq %1r, %%rdi\n"
				"addq %2, %%rsi\n"
				"addq %3, %%rdi\n"
				"movq %4, %%rbx\n"
				"movq(%%rsi), %%rax\n"
				"movq(%%rdi), %%rdx\n"
				"mulq %%rdx\n"
				"movq %%rax, (%%rbx)\n"
				"movq %%rdx, (%%rbx + $8)\n"
				"addq %6, (%%rbx)\n"
				"adcq $0, (%%rbx + $8)\n"
				"movq %5, %%rsi\n"
				"addq %2, %%rsi\n"
				"addq %3, %%rsi\n"
				"movq(%%rsi), %%rax\n"
				"addq %%rax, (%%rbx)\n"
				"adcq $0, (%%rbx + $8)\n"
				:
				: "r"(a.pointer), "r"(b.pointer), "r"(ik), "r"(jk), "r"(tmp.pointer), "r"(result.pointer), "r"(carry)
				: "rsi", "rdi", "rbx", "rax", "rdx", "rbx"
				);

			carry = tmp.pointer[1]; //carry = tmp / base;
			result.pointer[i + j] = tmp.pointer[0];//result.pointer[i + j] = tmp%base;
		}
		result.pointer[i + b.size] = carry;
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

	
	asm("clc\n"
		"pushf\n"
		"movq  %0, %%rsi\n"
		"movq  %1, %%rdi\n"
		"movq  %2, %%rcx\n"
		"m : movq (%%rsi), %%rax\n"
		"popf\n"
		"sbbq(%%rdi), %%rax\n"
		"pushf\n"
		"movq %%rax, (%%rsi)\n"
		"addq $8, %%rsi\n"
		"addq $8, %%rdi\n"
		"loop m\n"
		"movq(%%rsi), %%rax\n"
		"popf\n"
		"sbbq $0, %%rax\n"
		"movq %%rax, (%%rsi)\n"
		:
		: "r"(result.pointer), "r"(minnumber.pointer), "r"(minnumber.size)
		: "rsi", "rdi", "rcx", "rax"
		);
	
	return  Normalize(result);
}

MyLong Summ(MyLong a, MyLong b)
{
	MyLong maxnumber = a;
	MyLong minnumber = b;

	if (a.size < b.size)
	{
		maxnumber = b;
		minnumber = a;
	}

	MyLong result;
	result.SetMemory(maxnumber.size+1);

	for (int i = 0; i < maxnumber.size; i++)
	{
		result.pointer[i] = maxnumber.pointer[i];
	}

	asm("clc\n"
		"pushf\n"
		"movq  %0, %%rsi\n"
		"movq  %1, %%rdi\n"
		"movq  %2, %%rcx\n"
		"m : movq (%%rsi), %%rax\n"
		"popf\n"
		"adcq(%%rdi), %%rax\n"
		"pushf\n"
		"movq %%rax, (%%rsi)\n"
		"addq $8, %%rsi\n"
		"addq $8, %%rdi\n"
		"loop m\n"
		"movq(%%rsi), %%rax\n"
		"popf\n"
		"adcq $0, %%rax\n"
		"movq %%rax, (%%rsi)\n"
		:
		: "r"(result.pointer), "r"(minnumber.pointer), "r"(minnumber.size)
		: "rsi", "rdi", "rcx", "rax"
		);

	return Normalize(result);
}

bool WriteBinFile(char* file, MyLong number)
{
	FILE* out;
	if ((out = fopen(file,"wb")) != NULL)
	{
		for (int i = number.size - 1; i > -1; i--)
		{
			fwrite(&number.pointer[i], sizeof(unsigned long long*), 1, out);
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
		MyLong c;
		c.SetMemory(1);
		c.pointer[0] = 10;
		MyLong k;
		k.SetMemory(1);
		k.pointer[0] = 1;
		
		for (int l = 0; l < size; l++)
		{	
			k = Normalize(Mul(k, c));
		}

		MyLong tmp;
		tmp.SetMemory(1);

		MyLong ost;
		ost.SetMemory(1);
		char ch;
		unsigned long long h = 0;
		while ((ch = fgetc(in))!=EOF)
		{
			k = Divide(k, c, ost);
			free(ost.pointer);
			tmp.pointer[0] = ch - '0';
			res = Summ(res, Normalize(Mul(tmp, k)));	
		}

		
		fclose(in);

		free(k.pointer);
		free(c.pointer);
		free(tmp.pointer);
	}
	else
	{
		printf("Не удалось открыть файл %s!", file);
		res.SetMemory(1);
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

	MyLong dec;
	dec.SetMemory(1);
	dec.pointer[0] = 1000000000;

	MyLong tmp;
	tmp.SetMemory(1);

	MyLong null;
	null.SetMemory(1);

	//считаем количество элементов выходного массива
	MyLong ost;
	unsigned long long k = 1;
	while (Compare(numcopy, null) == 1)
	{
		numcopy = Divide(numcopy, dec, ost);
		k++;
	}
	if (k!=1)
	{
		k--;
	}
	MyLong res;
	res.SetMemory(k);
	
	int i = 0;
	while (Compare(number, null) == 1)
	{
		number = Divide(number, dec, ost);
		res.pointer[i] = ost.pointer[0];
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
		return true;
	}

		printf("Не удалось открыть файл %s!", file);
		return false;

	free(numcopy.pointer);
	free(dec.pointer);
	free(tmp.pointer);
	free(null.pointer);
	free(res.pointer);
	
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

		number.SetMemory((size - 1) / sizeof(unsigned long long)+1);

		fseek(in, 0, 0);

		for (int i = number.size - 1; i >-1; i--)
		{
			fread(&number.pointer[i], sizeof(unsigned long long), 1, in);
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
	


