### Chapter 39
# Interlude: Files and Directories

1. **Stat:** Write your own version of the command line program `stat`, which simply calls the `stat()` system call on a given file or directory. Print out file size, number of blocks allocated, reference (link) count, and so forth. What is the link count of a directory, as the number of entries changes? Usefule interfaces: `stat()`, naturally.  

    - the link count is simply the number of files and directories connected to whatever file or directory the `stat()` call is used on. An empty directy will have 2 links by default: `.`: itself, `..`: the parent directory. And individual files are just linked to themselves unless it is also linked to another file(s) through a ahrd or sym link.

    ```
    # it can also call multiple files at once
    $ ./stat stat.c
    stat.c
	ID:               16777220
	MODE:             33188
	INODE:            8633697704
	SIZE:             1268
	NO OF BLOCKS:     8
	GROUP ID:         20
	USER ID:          501
	LINKS:            1
	LAST ACCESS:      Fri Feb 19 14:00:21 2021
	LAST MOD:         Fri Feb 19 13:59:47 2021
	LAST STAT CHANGE: Fri Feb 19 13:59:47 2021
    ```
2. **List Files:** Write a program that lists files in the given directory. When called without any arguments, the program should just print the file names. When invoked with the `-l` flag, the program should print out information about each file, such as the owner, group, permissions, and other information obtained from the `stat()` system call. The program should take one additional argument, which is the directory to read, e.g., `myls -l directory`. if no directory is given, the program should just use the current working directory. Useful interfaces: `stat()`, `opendir()`, `readdir()`, `getcwd()`.