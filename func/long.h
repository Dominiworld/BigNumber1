typedef struct
{
	unsigned long long* pointer;
	unsigned long long size;
}
MyLong;


void SetMemory(unsigned long long s, MyLong *p);
void Copy(MyLong a, MyLong *p);
void FreeMemory(MyLong *p);

///Большие
MyLong Sum(MyLong a, MyLong b);
MyLong Sub(MyLong a, MyLong b);
MyLong Mul(MyLong a, MyLong b);
MyLong Divide(MyLong a, MyLong b, MyLong *mod);
MyLong Pow(MyLong a, MyLong b, MyLong m); //использует smallPow, Pow32
int Compare(MyLong a, MyLong b);

///Маленькие
MyLong ShortSum(MyLong a, unsigned long long b);
MyLong ShortSub(MyLong a, unsigned long long b);
MyLong ShortMul(MyLong a, unsigned long long b);
MyLong ShortDivide(MyLong a, unsigned long long b, unsigned long long *ost);
MyLong smallPow(MyLong a, unsigned long long p, MyLong m); //a^p mod m
int ShortCompare(MyLong a, unsigned long long b);

//Вспомогательные
MyLong Pow64(MyLong a, MyLong m);  //a^(2^32) mod m
MyLong ShiftLeft(MyLong *a, unsigned long long i); //сдвиг на i элементов массива 
MyLong Normalize(MyLong *a);			//уменьшение размера a за счет убирания незначащих нулей

//Работа с файлами
char* toString(MyLong number);
MyLong ReadFromString(char* buffer);
MyLong ReadTextFile(char* file);
MyLong ReadBinFile(char* file);
int WriteBinFile(char* file, MyLong number);
int WriteTextFile(char* file, MyLong number);