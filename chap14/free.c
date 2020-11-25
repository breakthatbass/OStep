#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	int *data = (int *) malloc(sizeof(int)*5);
	free(&data[2]);	
	printf("%d\n", data[2]);
	return 0;
}
