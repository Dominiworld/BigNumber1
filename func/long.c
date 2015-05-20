#include <stdio.h>
#include <stdlib.h> 
#include <memory.h>
#include <string.h>
#include "long.h"

void SetMemory(unsigned long long s, MyLong *p)
{
	p->size = s;
	p->pointer = (unsigned long long*)malloc(sizeof(unsigned long long)*p->size);
	memset(p->pointer, 0, sizeof(unsigned long long)*(p->size));
}
void Copy(MyLong a, MyLong *p) //p = copy(a)
{
	for (int i = 0; i < a.size; i++)
	{
		p->pointer[i] = a.pointer[i];
	}
}
void FreeMemory(MyLong *p)
{
	free(p->pointer);
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
	SetMemory(maxnumber.size + 1, &result);
	Copy(maxnumber, &result);
	asm("clc\n"
		"pushf\n"
		"movq  %0, %%rsi\n"
		"movq  %1, %%rdi\n"
		"movq  %2, %%rcx\n"
		"m12 : movq (%%rsi), %%rax\n"
		"popf\n"
		"adcq (%%rdi), %%rax\n"
		"pushf\n"
		"movq %%rax, (%%rsi)\n"
		"addq $8, %%rsi\n"
		"addq $8, %%rdi\n"
		"loop m12\n"
		"movq (%%rsi), %%rax\n"
		"popf\n"
		"adcq $0, %%rax\n"
		"movq %%rax, (%%rsi)\n"
		:
		: "r"(result.pointer), "r"(minnumber.pointer), "r"(minnumber.size)
		: "rsi", "rdi", "rcx", "rax"
		);


MyLong res = Normalize(&result);

	return res;
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
	SetMemory(maxnumber.size, &result);
	Copy(maxnumber, &result);

	asm("clc\n"
		"pushf\n"
		"movq  %0, %%rsi\n"
		"movq  %1, %%rdi\n"
		"movq  %2, %%rcx\n"
		"m11 : movq (%%rsi), %%rax\n"
		"popf\n"
		"sbbq (%%rdi), %%rax\n"
		"pushf\n"
		"movq %%rax, (%%rsi)\n"
		"addq $8, %%rsi\n"
		"addq $8, %%rdi\n"
		"loop m11\n"
		"movq (%%rsi), %%rax\n"
		"popf\n"
		"sbbq $0, %%rax\n"
		"movq %%rax, (%%rsi)\n"
		:
		: "r"(result.pointer), "r"(minnumber.pointer), "r"(minnumber.size)
		: "rsi", "rdi", "rcx", "rax"
		);

	return  Normalize(&result);
}
MyLong Mul(MyLong a, MyLong b)
{
	MyLong res;
	SetMemory(a.size + b.size, &res);
	unsigned long long cf = 0;
	unsigned long long bs = b.size * sizeof(unsigned long long);

	asm(	"movq %0, %%rsi\n"
		"movq %1, %%rbx\n"
		"movq %2, %%rcx\n"
		"m1 : push %%rcx\n"
		"movq %3, %%rdi\n"
		"movq %4, %%rcx\n"
		"m2 : movq (%%rsi), %%rax\n"
		"movq (%%rdi), %%rdx\n"
		"mulq %%rdx\n"
		"addq %%rax, (%%rbx)\n"
		"adcq  %5,%%rdx\n"
		"jnc m3\n"
		"movq $1,%5\n"
		"jmp m4\n"
		"m3 : movq $0,%5\n"
		"m4 : addq %%rdx, 8(%%rbx)\n"
		"jnc m5\n"
		"movq  $1,%5\n"
		"m5 : addq  $8, %%rbx\n"
		"addq  $8, %%rdi\n"
		"loop m2\n"
		"pop %%rcx\n"
		"subq %6,%%rbx\n"
		"addq $8, %%rsi\n"
		"addq  $8, %%rbx\n"
		"loop m1\n"
		:
		: "r"(a.pointer), "r"(res.pointer), "r"(a.size), "r"(b.pointer), "r"(b.size), "r"(cf), "r"(bs)
		: "rsi", "rbx", "rcx", "rdi", "rax", "rdx"
		);

	return Normalize(&res);
}
MyLong Divide(MyLong a, MyLong b, MyLong *mod)
{
	MyLong res;

	if (ShortCompare(a, 0) == 0)
	{
		printf ("Division by zero!");
		SetMemory(1, &res);
		SetMemory(1, mod);
		mod->size = 0;
		res.size = 0;
		return res;
	}

	if (b.size == 1)
	{
		SetMemory(1, mod);
		unsigned long long t = 0;
		res = ShortDivide(a, b.pointer[0], &t);
		
		mod->pointer[0] = t;
		return res;
	}

	
	if (Compare(a, b) == -1)
	{
		
		SetMemory(1, &res);
		SetMemory(a.size, mod);
		Copy(a, mod);
		return res;
	}

	SetMemory(a.size - b.size + 1, &res);

	MyLong tmp;
	MyLong A;
	SetMemory(a.size, &A);
	Copy(a, &A);

	MyLong tmp1;

	int i = res.size - 1;


	for (i = res.size - 1; i >-1; i--)
	{
		unsigned long long x = 0;
		unsigned long long low = 0;
		unsigned long long up = 18446744073709551615ULL;

		
		while (up - low > 1)
		{
			unsigned long long d = low/2+ up/2;

			tmp = ShortMul(b, d);		

			SetMemory(tmp.size + i, &tmp1);
			for (int j = i; j < tmp.size + i; j++)
			{
				tmp1.pointer[j] = tmp.pointer[j - i];
			}
				
			FreeMemory(&tmp);
		
			if (Compare(A, tmp1) != -1)
			{
				x = d;
				low = d;
			}
			else
			{
				up = d;
			}		
			FreeMemory(&tmp1);
		}

		res.pointer[i] = x;

		tmp = ShortMul(b, x);
		SetMemory(tmp.size + i, &tmp1);
		for (int j = i; j < tmp.size + i; j++)
			tmp1.pointer[j] = tmp.pointer[j - i];
		FreeMemory(&tmp);
		A = Sub(A, tmp1);
		FreeMemory(&tmp1);
	}
		
		tmp = Normalize(&A);
		mod->pointer = tmp.pointer;
		mod->size = tmp.size;

	return Normalize(&res);
}
MyLong Pow(MyLong a, MyLong p, MyLong m)
{
	MyLong res;
	SetMemory(1, &res);
	res.pointer[0] = 1;

	MyLong ost;
	MyLong tmp;
	MyLong tmp1;
	MyLong tmp2;
	MyLong tmp3;

	if (Compare(a, m) != -1) Divide(a, m, &a);

	for (int i = p.size - 1; i > -1; i--)
	{
		tmp1 = Pow64(res, m);
		tmp2 = smallPow(a, p.pointer[i], m);
		tmp3 = Mul(tmp1, tmp2);
		FreeMemory(&tmp1);
		FreeMemory(&tmp2);
		tmp = Divide(tmp3, m, &res);
		FreeMemory(&tmp3);
		FreeMemory(&tmp);
		tmp = Divide(Normalize(&res), m, &res);
		FreeMemory(&tmp);
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

MyLong ShortSum(MyLong a, unsigned long long b)
{
	MyLong res;
	SetMemory(a.size + 1, &res);
	Copy(a, &res);

	asm(
		"clc\n"
		"movq  %0, %%rdi\n"
		"movq %1, %%rbx\n"
		"addq %%rbx, (%%rdi)\n"
		"pushf\n"
		"addq  $8, %%rdi\n"
		"movq  %2, %%rcx\n"
		"m7:	popf\n"
		"adcq $0, (%%rdi)\n"
		"pushf\n"
		"jnc ex1\n"
		"addq $8, %%rdi\n"
		"loop m7\n"
		"ex1: popf\n"
		:
		: "r"(res.pointer), "r"(b), "r"(res.size)
		: "rdi", "rbx", "rcx"
		);

	return Normalize(&res);
}
MyLong ShortSub(MyLong a, unsigned long long b)
{
	MyLong res;
	SetMemory(a.size, &res);
	Copy(a, &res);

	asm(
		"clc\n"
		"movq  %0, %%rdi\n"
		"movq %1, %%rbx\n"
		"subq %%rbx, (%%rdi)\n"
		"pushf\n"
		"addq  $8, %%rdi\n"
		"movq  %2, %%rcx\n"
		"m:	popf\n"
		"sbbq $0, (%%rdi)\n"
		"pushf\n"
		"jnc ex2\n"
		"addq $8, %%rdi\n"
		"loop m\n"
		"ex2: popf\n"
		:
		: "r"(res.pointer), "r"(b), "r"(res.size)
		: "rdi", "rbx", "rcx"
		);

	return Normalize(&res);
}
MyLong ShortDivide(MyLong a, unsigned long long b, unsigned long long *ost)
{
	MyLong res;
	if (!b)
	{
		printf("Division by zero!");
		SetMemory(1, &res);
		ost = 0;
		res.size = 0;
		return res;
	}

	if (ShortCompare(a, b) == -1)
	{
		*ost = a.pointer[0];
		SetMemory(1, &res);
		return res;
	}

	SetMemory(a.size, &res);

	unsigned long long s = (a.size - 1)*sizeof(unsigned long long);
	unsigned long long k = 0;
	MyLong A;
	SetMemory(a.size, &A);
	Copy(a, &A);
	

	asm(
		"movq  %1, %%rsi\n"
		"addq  %2, %%rsi\n"
		"movq  %3, %%rdi\n"
		"addq  %2, %%rdi\n"
		"movq  %4, %%rcx\n"
		"xor %%rdx, %%rdx\n"
		"m6 : movq (%%rsi), %%rax\n"
		"divq %5\n"
		"movq  %%rax, (%%rdi)\n"
		"subq $8, %%rsi\n"
		"subq  $8, %%rdi\n"
		"loop m6\n"
		"mov  %%rdx, %0\n"
		:"=r"(k)
		: "r"(A.pointer), "r"(s), "r"(res.pointer), "r"(A.size), "r"(b)
		: "rsi", "rsi", "rcx", "rdx", "rax"
		);

	*ost = k;
	
	return Normalize(&res);
}

MyLong ShortMul(MyLong a, unsigned long long b)
{
	MyLong res;
	SetMemory(a.size + 1, &res);

	asm(
		"movq  %0, %%rsi\n"
		"movq  %1, %%rcx\n"
		"movq  %2, %%rbx\n"
		"m9 : movq (%%rsi), %%rax\n"
		"mulq %3\n"
		"addq %%rax, (%%rbx)\n"
		"adcq %%rdx, 8(%%rbx)\n"
		"adcq $0, 16(%%rbx)\n"
		"addq $8, %%rsi\n"
		"addq $8, %%rbx\n"
		"loop m9\n"
		:
		: "r"(a.pointer), "r"(a.size), "r"(res.pointer), "r"(b)
		: "rsi", "rcx", "rbx", "rax", "rdx"
		);

	return Normalize(&res);
}
MyLong smallPow(MyLong a, unsigned long long p, MyLong m)
{
	MyLong res;
	SetMemory(1, &res);
	res.pointer[0] = 1;
	MyLong tmp;


	char c[64]; 
	memset(c, 0, 64);
	int k = 0; 


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

	
	MyLong tmp1;

	for (int i = k; i >= 0; i--)
	{
		tmp1 = Mul(res, res);

		tmp = Divide(tmp1, m, &res);


		FreeMemory(&tmp1);

		FreeMemory(&tmp);

		if (c[i] == 1)
		{
			tmp1 = Mul(res, a);
			tmp = Divide(tmp1, m, &res);
			
			FreeMemory(&tmp1);
			FreeMemory(&tmp);
		}
		
	}

	return res;
}
int ShortCompare(MyLong a, unsigned long long b)
{
	if (a.size > 1) return 1;
	if (a.size == 0) return -1;
	if (a.pointer[0] > b) return 1;
	if (a.pointer[0] < b) return -1;
	return 0;
}

MyLong ShiftLeft(MyLong *a, unsigned long long i)
{
	MyLong a1;

	SetMemory(a->size + i, &a1);

	for (int j = i; j < a->size + i; j++)
		a1.pointer[j] = a->pointer[j - i];

	FreeMemory(a);
	return a1;
}
MyLong Normalize(MyLong *a)
{
	
	while (a->pointer[a->size - 1] == 0) a->size--;

	if (a->size == 0) a->size = 1;

	MyLong result;
	SetMemory(a->size, &result);
	
	for (size_t i = 0; i < a->size; i++)
	{
		result.pointer[i] = a->pointer[i];
	}
	FreeMemory(a);

	return result;
}
MyLong Pow64(MyLong a, MyLong m)
{
	MyLong res;
	SetMemory(a.size, &res);
	Copy(a, &res);

	MyLong tmp;

	for (int i = 0; i < 64; i++)
	{
		tmp = Divide(Mul(res, res), m, &res);
		FreeMemory(&tmp);
	}
	return res;
}

int WriteBinFile(char* file, MyLong number)
{
	FILE* out;

	if ((out = fopen(file,"wb")) != NULL)
	{
		for (int i = number.size - 1; i > -1; i--)
		{
			fwrite(&number.pointer[i], sizeof(unsigned long long*), 1, out);
		}
		fclose(out);
		return 1;
	}
	
		printf("Error by opening file %s!", file);
		return 0;
}

MyLong ReadFromString(char* buffer)
{
	int size = 0;
	
	MyLong res;
	SetMemory(1, &res);

	if (buffer == NULL)
	{
		printf("Error by reading string!");
		res.size = 0;
		return res;
	}
	
	if (strlen(buffer) == 0)
	{
		res.size = 0;
		return res;
	}
	
	MyLong k;
	SetMemory(1, &k);
	k.pointer[0] = 1;
		
	MyLong tmp1;
	MyLong tmp2;

	unsigned long long tmp = 0;
	char ch;

	for (int i = strlen(buffer)-1; i>-1; i--)
	{
	        ch = buffer[i];
		if ((ch > '9') || (ch < '0'))
		{
			printf("Incorrect input!");
			res.size = 0;
			return res;
		}
		tmp = ch - '0';			
		tmp1 = ShortMul(k, tmp);
		tmp2 = Normalize(&tmp1);
		res = Sum(res, tmp2);
		FreeMemory(&tmp2);
		k = ShortMul(k,10);
	}
	
	FreeMemory(&k);
	return res;
}

MyLong ReadTextFile(char* file)
{
	FILE* in;
	
	int size = 0;
	
	MyLong res;
	SetMemory(1, &res);

	if ((in = fopen(file, "r")) != NULL)
	{
		while (fgetc(in) != EOF)
		{
			size++;
		}

		if (size == 0)
		{
			SetMemory(1, &res);
			res.size = 0;
			return res;
		}
		MyLong k;
		SetMemory(1, &k);
		k.pointer[0] = 1;
		
		MyLong tmp1;

		unsigned long long tmp = 0;
		char ch;
		int offset = -1;

		MyLong tmp2;

		fseek(in, offset, SEEK_END);

		while (offset!=-size-1)
		{
			ch = fgetc(in);
			if ((ch > '9') || (ch < '0'))
			{
				printf("File %s contains not only numbers!", file);
				res.size = 0;
				return res;
			}

			tmp = ch - '0';			
			tmp1 = ShortMul(k, tmp);
			tmp2 = Normalize(&tmp1);
			res = Sum(res, tmp2);
			FreeMemory(&tmp2);
			k = ShortMul(k,10);
			offset--;
			fseek(in, offset, SEEK_END);
		}	
		fclose(in);
		FreeMemory(&k);
	}
	else
	{
		printf("Error by opening file %s!", file);
		res.size = 0;		
	}

	return res;
}

char* toString(MyLong number)
{
	MyLong numcopy;
	SetMemory(number.size, &numcopy);
	Copy(number, &numcopy);

	MyLong tmp;

	unsigned long long ost;
	unsigned long long k = 1;

	if(ShortCompare(number,0)==0)
		return "0";	

	FILE* stream;

	if ((stream = tmpfile())==NULL)
	
	{
		printf("Temporary file wasn't created!");
		return "";
	}

	unsigned long long size = 0;

	while (ShortCompare(numcopy, 0) == 1)
	{
		tmp = numcopy;
		numcopy = ShortDivide(numcopy, 10000000000000000000LLU, &ost);
		FreeMemory(&tmp);

		char buffer[] = "0000000000000000000";
		int k = 18;
		while (ost > 0)
		{
			buffer[k--] = ost % 10 + '0';
			ost /= 10;	
		}
		fprintf(stream, "%s", buffer);
		size++;        
	}


	FreeMemory(&numcopy);

	long long offset = -19;

	fseek(stream, offset, SEEK_END);

	char* str = (char*)malloc(sizeof(char)*(size+1)*19);

	memset(str, '\0', (size+1)*19);

	char tmps [19];	
	fgets(tmps, 20, stream);
	int count = 0;
	while(tmps[count]=='0')
		count++;
	int s = 19-count;
	char sss[19];
	memset(sss, '\0', 19);
	int i = 0;
	while(i!=s)
		sss[i++]=tmps[count++];
	strcat(str,sss);
	offset-=19;
	fseek(stream, offset, SEEK_END);

	while(offset>-(size+1)*19)
	{
		fgets(tmps, 20, stream);
		strcat(str, tmps);
		offset-=19;
		fseek(stream, offset, SEEK_END);
	}

	fclose(stream);
	

	return str;
}


int WriteTextFile(char* file, MyLong number)
{
	FILE* out;

	if ((out = fopen(file, "w")) != NULL)
	{
		char* str = toString(number);	
		
		if(fputs(str, out)==EOF)
		{
			printf("Error by writing file %s", file);
			return 0;
		}

		fclose(out);
		return 1;
	}

	printf("Error by opening file %s!", file);
	return 0;

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
			printf("File %s is empty!", file);
			SetMemory(1, &number);
			number.size = 0;
			return number;
		}

		SetMemory((size - 1) / sizeof(unsigned long long) + 1, &number);

		fseek(in, 0, 0);

		for (int i = number.size - 1; i >-1; i--)
		{
			fread(&number.pointer[i], sizeof(unsigned long long), 1, in);
		}

		fclose(in);
	}
	else
	{
		printf("Error by opening file %s!", file);
		SetMemory(1, &number);
		number.size = 0;
	}
	return number;
}
	




