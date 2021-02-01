**NOTE:** Using the `semaphore.h` lib is a bit different on MacOS than on Linux.
MacOS requires named semaphores as opposed to the unnamed on Linux. the OStep book shows semaphores in the Linux way.
Here is usage in a MacOs envrionment:
```c
#include <semaphore.h>

sem_t *s;   // must be a pointer

// init is done with sem_open(). 
// param 1 is a string, semaphore name, and param 2 can be used
// with oflags in the same fashion as the open() system call
s = sem_open("/sem", 0);

// wait and post are the same as Linux
sem_wait(s);
sem_post(s);

// must be closed and unlinked when done
sem_close(s);
sem_unlink("/sem");
```
For brevity there is no error checking above. Normally error checking is essential.
more info: http://blog.cs4u.us/2014/04/using-semaphores-on-mac-os-x.html

1. The first problem is just to implement and test a solution to the **fork/join problem**, as described in the text. Even though this solution is described in the text, the act of typing it on your own is worthwhile; even Bach would rewrite Vivaldi, allowing one soon-to-be master to learn from an existing one. See `fork-join.c` for details. Add the call `sleep(1)` to the child to ensure it's working.

2. Now let's generalize this a bit by investigating the **rendezvous** problem. The problem is as follows: you have two threads, each of which are about to enter the rendezvous part of your code. Neither should exit this part of the code before the other enters it. Consider using two semaphores for this task., see `rendezvous.c` for details.

3. Now go one step further and by implementing a general solution to **barrier synchronization**. Assume there are two points in an sequential piece of code, called `P1` and `P2`. Putting a **barrier** between `P1` and `P2` guaruntees that all threads will execute `P1` before anyone executes `P2`. Your task: write the code to implement a `barrier()` function that cna be used in this manner. It is safe to assume you know `N` (the total number of threads in the running program) and that all `N` threads will try to enter the barrier. Again, you should likely use two semaphores to achieve the solution, and some other integers to count things. See `barrier.c` for details.
