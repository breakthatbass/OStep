### Chapter 39
# Interlude: Files and Directories

1. **Stat:** Write your own version of the command line program `stat`, which simply calls the `stat()` system call on a given file or directory. Print out file size, number of blocks allocated, reference (link) count, and so forth. What is the link count of a directory, as the number of entries changes? Usefule interfaces: `stat()`, naturally.  

    - [`stat.c`](https://github.com/breakthatbass/OStep/blob/main/chap39/stat.c)

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

    - [`myls.c`](https://github.com/breakthatbass/OStep/blob/main/chap39/myls.c)
    - It works exactly the same as the UNIX `ls` command and has the `-l` option.

3. **Tail:** Write a program that prints out the last few lines of a file. The program should be efficient in that it seeks to near the end of the file, reads in a block of data, and then goes backwards until it finds the requested number of lines; at this point, it should print out those lines from beginning to the end of the file. To invoke the program one should type: `mytail -n file` where `n` is the number of lines at the end of the file to print. Useful interefaces: `stat()`, `lseek()`, `open()`, `read()`, `close()`.  
	- [mytail.c](https://github.com/breakthatbass/OStep/blob/main/chap39/mytail.c)

4. **Recursive Search**: Write a program that prints out the names of each file and directory in the file system tree. For example, when run without arguments, the program should start with the current working directory and print its contents, as well as the contents of any sub-directories, etc., until the entire tree, root at the CWD is printed. If given a single argument (of a directory name), use that as the root of the tree instead. Refine your recursive search with more fun options, similar to the powerful `find` command line tool. Useful interfaces: figure it out.

