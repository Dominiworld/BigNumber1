#include "stdio.h"
#include "2.h"


LONG::LONG()
{
	
}

bool LONG::IsEmpty()
{
	return number.size == 0;
}

LONG LONG::operator=(LONG t)
{
	this->number = t.number;
	return *this;
}

LONG LONG::operator+(LONG t)
{
	LONG res;
	res.number = Sum(this->number, t.number);
	return res;
}

LONG LONG::operator+(unsigned long long t)
{
	LONG res;
	res.number = ShortSum(this->number, t);
	return res;
}


LONG LONG::operator-(LONG t)
{
	LONG res;
	res.number = Sub(this->number, t.number);
	return res;
}
LONG LONG::operator-(unsigned long long t)
{
	LONG res;
	res.number = ShortSub(this->number, t);
	return res;
}

LONG LONG::operator*(LONG t)
{
	LONG res;
	res.number = Mul(this->number, t.number);
	return res;
}
LONG LONG::operator*(unsigned long long t)
{
	LONG res;
	res.number = ShortMul(this->number, t);
	return res;
}
LONG LONG::operator/(LONG t)
{
	MyLong ost;
	LONG res;
	res.number = Divide(this->number, t.number, &ost);
	FreeMemory(&ost);
	return res;
}
LONG LONG::operator/(unsigned long long t)
{
	unsigned long long ost;
	LONG res;
	res.number = ShortDivide(this->number, t, &ost);
	return res;
}

LONG LONG::operator%(LONG t)
{
	MyLong ost;
	MyLong res = Divide(this->number, t.number, &ost);
	FreeMemory(&res);
	LONG result;
	result.number = ost;
	return result;
}
unsigned long long LONG::operator%(unsigned long long t)
{
	unsigned long long ost;
	MyLong res = ShortDivide(this->number, t, &ost);
	FreeMemory(&res);
	return ost;
}



bool LONG::operator>(LONG t)
{
	return Compare(this->number, t.number) == 1;
}
bool LONG::operator>(unsigned long long t)
{
	return ShortCompare(this->number, t) == 1;
}

bool LONG::operator<(LONG t)
{
	return Compare(this->number, t.number) == -1;
}
bool LONG::operator<(unsigned long long t)
{
	return ShortCompare(this->number, t) == -1;
}

bool LONG::operator==(LONG t)
{
	return Compare(this->number, t.number) == 0;
}

bool LONG::operator==(unsigned long long t)
{
	return ShortCompare(this->number, t) == 0;
}

bool LONG::operator>=(LONG t)
{
	return Compare(this->number, t.number) != -1;
}

bool LONG::operator>=(unsigned long long t)
{
	return ShortCompare(this->number, t) != -1;
}

bool LONG::operator<=(LONG t)
{
	return Compare(this->number, t.number) != 1;
}

bool LONG::operator<=(unsigned long long t)
{
	return ShortCompare(this->number, t) != 1;
}

LONG PowMod(LONG a, LONG pow, LONG mod)
{
	LONG res;
	res.number = Pow(a.number, pow.number, mod.number);
	return res;
}


LONG PowMod(LONG a, unsigned long long pow, LONG mod)
{
	LONG res;
	res.number = smallPow(a.number, pow, mod.number);

	return res;
}

bool LONG::ToBinFile(char* file)
{
	return WriteBinFile(file, this->number)==1;
}
bool LONG::ToTextFile(char* file)
{
	return WriteTextFile(file, this->number)==1;
}
void LONG::FromTextFile(char* file)
{
	
	LONG res;
	res.number = ReadTextFile(file);
	*this = res;	
}
void LONG::FromBinFile(char* file)
{
	LONG res;
	res.number = ReadBinFile(file);

	*this = res;
}
void LONG::Kill()
{
	MyLong t = this->number;
	FreeMemory(&t);
}



