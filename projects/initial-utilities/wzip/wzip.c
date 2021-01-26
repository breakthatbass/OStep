#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    if (argc == 1) {
        printf("wzip: file1 [file2 ...]\n");
        exit(1);
    }
    FILE *fp;
    int i;
    char c;
    int count = 1;
    char prev;
    
    for (i = 1; i < argc; i++) {
        fp = fopen(argv[i], "r");
        if (fp == NULL) {
            printf("wzip: cannot open file\n");
	        exit(1);
        }
        while(fread(&c, sizeof(char), 1, fp)) {
            if (c == prev) count++;
            else if (c!= prev && count > 1) {
                fwrite(&count, sizeof(int), 1, stdout);
                fwrite(&prev, sizeof(char), 1, stdout);
                count = 1;
            } else fwrite(&prev, sizeof(char), 1, stdout);
            prev = c;
        }
        fclose(fp);
    }
    //fwrite(&prev, sizeof(char), 1, stdout);
    return 0;
}