#include <stdio.h>

union dum
{
	int a;
	char b[2];
}dum;

int main()
{
	dum.b[0] = 2;
	dum.b[1] = 3;
	printf("%d\n", dum.a);
	printf("%p\n", &dum.a);
	printf("%p\n", dum.b);
}
