#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <sys/stat.h>
#include <sys/time.h>

#define MAXPATH 200

void dir_search(char *path)
{
    DIR *dp;
    struct dirent *d;
   
    if ((dp = opendir(path)) == NULL) {
        perror("opendir");
        return 2;
    }
    
    while ((d = readdir(dp))) {
        char npath[MAXPATH];
        if (*d->d_name != '.') { // don't print hidden files
            strcpy(npath, path);
            strcat(npath, "/");
            strcat(npath, d->d_name);

            // d_type 4 means we have a directory
            // we then call this function on the new directory
            if (d->d_type == 4)
                dir_search(npath);
        
            printf("%s\n", npath);
        }
    }
    closedir(dp);
}



int main(int argc, char **argv)
{
    char path[MAXPATH];

    if (argc == 2) 
        strcpy(path, *++argv);
    else if (argc == 1) 
        strcpy(path, ".");
    else {
        fprintf(stderr, "usage: %s [path]\n", *argv);
        return 1;
    }

    dir_search(path);

    return 0;
}