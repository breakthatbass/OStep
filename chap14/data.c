#include <stdlib.h>
#include <stdio.h>

int main()
{

	int *data = (int*) malloc(100);
	data[100] = 0;

	return 0;
}
