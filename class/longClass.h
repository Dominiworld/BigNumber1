extern "C"
{
#include "long.h"
}

class  LONG {
public:
	LONG();
	LONG(unsigned long long t);
	LONG(char* str);
	LONG operator=(LONG);
	LONG operator=(unsigned long long);
	LONG operator+(LONG);
	LONG operator-(LONG);
	LONG operator*(LONG);
	LONG operator/(LONG);
	LONG operator%(LONG);
	bool operator>(LONG);
	bool operator<(LONG);
	bool operator==(LONG);
	bool operator<=(LONG);
	bool operator>=(LONG);

	LONG operator+(unsigned long long);
	LONG operator-(unsigned long long);
	LONG operator*(unsigned long long);
	LONG operator/(unsigned long long);
	unsigned long long operator%(unsigned long long);
	bool operator>(unsigned long long);
	bool operator<(unsigned long long);
	bool operator==(unsigned long long);
	bool operator<=(unsigned long long);
	bool operator>=(unsigned long long);


	LONG PowMod(LONG pow, LONG mod);
	LONG PowMod(unsigned long long pow, LONG mod);
	void FromTextFile(char* file);
	void FromBinFile(char* file);
	bool ToBinFile(char* file);
	bool ToTextFile(char* file);
	bool IsEmpty();
private:
	MyLong number;
	char* Print();
	void Kill();
};
