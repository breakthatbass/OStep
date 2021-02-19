#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <limits.h>
#include <getopt.h>
#include <stdlib.h>
#include <pwd.h> // getpwuid()
#include <grp.h> // getgrgid()

// convert mode_t number into readable mode characters
// credit for this function: 
// https://stackoverflow.com/questions/10323060/printing-file-permissions-like-ls-l-using-stat2-in-c
void print_mode(struct stat fileStat)
{
    printf( (S_ISDIR(fileStat.st_mode)) ? "d" : "-");
    printf( (fileStat.st_mode & S_IRUSR) ? "r" : "-");
    printf( (fileStat.st_mode & S_IWUSR) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXUSR) ? "x" : "-");
    printf( (fileStat.st_mode & S_IRGRP) ? "r" : "-");
    printf( (fileStat.st_mode & S_IWGRP) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXGRP) ? "x" : "-");
    printf( (fileStat.st_mode & S_IROTH) ? "r" : "-");
    printf( (fileStat.st_mode & S_IWOTH) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXOTH) ? "x" : "-");
    printf(" ");
}

#define MAXDATE 25
// parse parse out usable parts of the ctime(s.st_mtime) data
char *parse_time(struct stat s)
{   
    static char t[MAXDATE] = "";
    strftime(t, MAXDATE, "%b %d %H:%M", localtime(&s.st_mtime));
    return t;
}

// file_l: list files/dirs with info
// print file mode, num of links, owner name, group name, 
// num of bytes, last mod, path
void file_l(char *path)
{
    // file mode, num of links, owner name, group name, num of bytes, last mod, path
    DIR *dp;
    struct dirent *d;
    struct stat s;
    struct passwd *u;
    struct group *g;
    char *t;

    // no -l, just normal ls command
    if ((dp = opendir(path)) == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }
    
    while ((d = readdir(dp))) {
        if (*d->d_name != '.') {
            if (stat(d->d_name, &s) < 0) {
                fprintf(stderr, "%s: ", d->d_name);
                perror("stat");
                exit(EXIT_FAILURE);
            }
            // get user name
            u = getpwuid(s.st_uid);
            // get group name
            g = getgrgid(s.st_gid);
            // get time string
            t = parse_time(s);

            int n = 8;

            print_mode(s);
            printf("%lu ", (unsigned long) s.st_nlink);
            printf("%s ", u->pw_name);
            printf(" %s ", g->gr_name);
            printf("%*d ", n-((int)(sizeof(s.st_size)/4)), (int)s.st_size);
            printf("%s ", t);
            printf("%s\n", d->d_name);
        }
    }
    closedir(dp);
}


// file_nol: list files/dirs with no info. basic ls command
void file_nol(char *path)
{
    DIR *dp;
    struct dirent *d;

    // no -l, just normal ls command
    if ((dp = opendir(path)) == NULL) {
        printf("path: %s\n", path);
        perror("opendir");
        exit(EXIT_FAILURE);
    }
    
    while ((d = readdir(dp))) {
    if (*d->d_name != '.') // don't print hidden files
        printf("%s\n", d->d_name);
    }
    closedir(dp);
}


int main(int argc, char **argv)
{
    DIR *dp;
    char path[PATH_MAX];
    int opt;
    int l = 0;  // default no -l
    opterr = 0;

    while ((opt = getopt(argc, argv, "l:")) != -1) {
        switch(opt) {
        case 'l':
            // case for -l with a path name
            l = 1;
            strcpy(path, optarg);
            break;
        case '?':
            // case for -l but no path name
            if (optopt == 'l') {
                l = 1;
                getcwd(path, sizeof(path));
            } else {
                fprintf(stderr, "usage: %s [-l] [pathname]\n", *argv);
                exit(EXIT_FAILURE);
            }
            break;
        default:
            // case for no -l
            break;
        }
    }
    
    if (l == 0 && argc == 2)
        strcpy(path, *++argv);
    else if (l == 0)
        getcwd(path, sizeof(path));

    if (l == 0) file_nol(path);
    else file_l(path);

    exit(EXIT_SUCCESS);
}
