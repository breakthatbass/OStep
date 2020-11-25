#include <stdlib.h>
#include <stdio.h>

int main()
{

	int *ptr = (int*) malloc(sizeof(int)*500);
	*ptr = 5;

	return 0;
}
