## Chapter 14
# Interlude: Memory API

1. First, write a simple program called `null.c` that creates a pointer to an integer, sets it to `NULL`, and then tries to dereference it. Compile this into an executable called `null`. What happens when you run this program?
    - segmentation fault

2. Next, compile this program with symbol information included(with the `-g` flag). Doing so let’s put more information into the executable, enabling the debugger to access more useful information about variable names and the like. Run the program under the debugger by typing `gdb null` and then, once `gdb` is running, typing `run`. What does `gdb` show you?
    
    note: `gdb` has all sorts of problems on MacOS. I'm using `lldb` instead.
    ```
    Process 38393 stopped
    * thread #1, queue = 'com.apple.main-thread', stop reason = EXC_BAD_ACCESS (code=1, address=0x0)
    frame #0: 0x0000000100000fa8 null`main at null.c:11:6
        8   		x = 5;
        9   		ptr = &x;
        10  		ptr = NULL;
     -> 11  		y = *ptr;
        12  	
        13  	    return 0;
        14  	}
    Target 0: (null) stopped.
    ```

3. Finally, use the `valgrind` tool on this program. We’ll use the `memcheck` tool that is a part of `valgrind` to analyze what happens. Run this by typing in the following: `valgrind --leak-check=yes null`. What happens when you run this? Can you interpret the output from the tool?
    ```
    ==17297== Invalid read of size 4
    ==17297==    at 0x100000FA8: ??? (in ./null)
    ==17297==    by 0x7FFF20335630: ??? (in /dev/ttys000)
    ==17297==  Address 0x0 is not stack'd, malloc'd or (recently) free'd
    ==17297== 
    ==17297== 
    ==17297== Process terminating with default action of signal 11 (SIGSEGV)
    ==17297==  Access not within mapped region at address 0x0
    ==17297==    at 0x100000FA8: ??? (in ./null)
    ==17297==    by 0x7FFF20335630: ??? (in /dev/ttys000)

    ...

    ==17297== HEAP SUMMARY:
    ==17297==     in use at exit: 0 bytes in 0 blocks
    ==17297==   total heap usage: 0 allocs, 0 frees, 0 bytes allocated
    ==17297== 
    ==17297== All heap blocks were freed -- no leaks are possible
    ```
    - This looks like there's an "invalid read of size 4" which probably means an `int`. It's not helpful that there are no line numbers with issues her. `Valgrind` on mac is sub par. 

4. Write a simple program that allocates memory using `malloc()` but forgets to free it before exiting. What happens when this program runs? Can you use `gdb` to find any problems with it? How about `valgrind` (again with the `-`-leak-check=yes` flag)?

    - The program runs just fine. `gdb` and (`lldb` for that matter) does not detect errors. `valgrind` returns this:

    ```
    ==19861== HEAP SUMMARY:
    ==19861==     in use at exit: 0 bytes in 0 blocks
    ==19861==   total heap usage: 0 allocs, 0 frees, 0 bytes allocated
    ==19861== 
    ==19861== All heap blocks were freed -- no leaks are possible
    ==19861== 
    ==19861== For lists of detected and suppressed errors, rerun with: -s
    ==19861== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
    ```
    - `valgrind` is buggy it seems... should have been at least 4 bytes in use at exit and at least one alloc...

5. Write a program that creates an array of integers called `data` of size 100 using `malloc`; then, set `data[100]` to zero. What happens when you run this program? What happens when you run this program using valgrind? Is the program correct?
    - `valgrind`, again, isn't detecting issues here. I assume it would say there's an invalid write if it were not buggy. Big Sur causing me all sorts of development problems...

6. Create a program that allocates an array of integers (as above), frees them, and then tries to print the value of one of the elements of the array. Does the program run? What happens when you use `valgrind` on it?

7. Now pass a funny value to free (e.g., a pointer in the middle of the array you allocated above). What happens? Do you need tools to find this type of problem?

    ```
    $ ./free
    free(21409,0x10c396e00) malloc: *** error for object 0x7f9cb2c05b48: pointer being freed was not allocated  
    free(21409,0x10c396e00) malloc: *** set a breakpoint in malloc_error_break to   debug
    [1]    21409 abort      ./free
    ```

8. Try out some of the other interfaces to memory allocation. For example, create a simple vector-like data structure and related routines that use `realloc()` to manage the vector. Use an array to store the vectors elements; when a user adds an entry to the vector, use `realloc()` to allocate more space for it. How well does such a vector perform? How does it compare to a linked list? Use `valgrind` to help you find bugs.

9. Spend more time and read about using `gdb` and `valgrind`. Knowing your tools is critical; spend the time and learn how to become an expert debugger in the UNIX and C environment.

    - ok.
