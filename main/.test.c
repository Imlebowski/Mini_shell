#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
	int idx = 0;
	for (idx = 0; idx < 15; idx++)
	{
		printf("Proc 3 %d\n", idx);
		sleep(1);
	}

	exit(6);
}
