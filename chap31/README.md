1. The first problem is just to implement and test a solution to the **fork/join problem**, as described in the text. Even though this solution is described in the text, the act of typing it on your own is worthwhile; even Bach would rewrite Vivaldi, allowing one soon-to-be master to learn from an existing one. See `fork-join.c` for details. Add the call `sleep(1)` to the child to ensure it's working.

2. Now let's generalize this a bit by investigating the **rendezvous** problem. The problem is as follows: you have two threads, each of which are about to enter the rendezvous part of your code. Neither should exit this part of the code before the other enters it. Consider using two semaphores for this task., see `rendezvous.c` for details.

