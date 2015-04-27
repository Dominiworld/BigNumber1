extern "C"
{
#include "1.h"
}

class  LONG {
public:

	LONG();
	LONG operator=(LONG t);
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

	void FromTextFile(char* file);
	void FromBinFile(char* file);
	bool ToBinFile(char* file);
	bool ToTextFile(char* file);
	void Kill();
	bool IsEmpty();
	friend LONG PowMod(LONG a,LONG pow, LONG mod);
	friend LONG PowMod(LONG a, nsigned long long pow, LONG mod);
private:
	MyLong number;

};
