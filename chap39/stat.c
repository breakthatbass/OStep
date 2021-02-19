#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/time.h>


int main(int argc, char **argv)
{
    struct stat s;
    if (argc == 1) {
        fprintf(stderr, "usage: %s path1 [path2...]\n", *argv);
        return 1;
    }

    while (--argc > 0 && (*argv)) {        
        if (stat(*++argv, &s) < 0) {
            fprintf(stderr, "%s: ", *argv);
            perror("stat");
            return 1;
        }

        printf("%s\n", *argv);
        printf("\tID:               %lu\n", (unsigned long) s.st_dev);
        printf("\tMODE:             %lu\n", (unsigned long) s.st_mode);
        printf("\tINODE:            %lu\n", (unsigned long) s.st_ino);
        printf("\tSIZE:             %lu\n", (unsigned long) s.st_size);
        printf("\tNO OF BLOCKS:     %lu\n", (unsigned long) s.st_blocks);
        printf("\tGROUP ID:         %lu\n", (unsigned long) s.st_gid);
        printf("\tUSER ID:          %lu\n", (unsigned long) s.st_uid);
        printf("\tLINKS:            %lu\n", (unsigned long) s.st_nlink);
        printf("\tLAST ACCESS:      %s", ctime(&s.st_atime));
        printf("\tLAST MOD:         %s", ctime(&s.st_mtime));
        printf("\tLAST STAT CHANGE: %s", ctime(&s.st_ctime));
    }
    return 0;
}
