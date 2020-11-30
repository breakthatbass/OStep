# Interlude: Process API

Skip to [code section](https://github.com/breakthatbass/OStep/tree/main/chap5#code)

#
## Simulation

1. Run ```./fork.py -s 10``` and see which actions are taken. Can you predict
   what the process tree looks like at each step? Use the ```-c``` flag to check
   your answers. Try some different random seeds (```-s```) or add more actions
   (```-a```) to get the hang of it.  
---  
2. One control the simulator gives you is the ```fork_percentage```, controlled
   by the ```-f``` flag. The higher it is, the more likely the next action is a
   fork; The lower it is, the more likely the action is an exit. Run the
   simulator with a large number of actions (e.g., ```-a 100```) and vary the
   ```fork_percentage``` from 0.1 to 0.9. what do you think the resulting final
   process trees will look like as the precentage changes? Check your answers
   with ```-c```.    
   

   The higher the the ```fork_percentage``` is, there are more forks being
   created and less ```EXITS``` so the resulting process trees end up looking
   pretty complicated with all sort of forks off of forks, etc. With a lower
   ```fork_percentage```, the trees look simpler since so many of the forks end
   up getting exited.  
---
3. Now, switch the output by using the ```-t``` flag (e.g., run ```./fork.py -t```). Given a set of process trees, can you tell which actions were taken?  
---  
4. One interesting thing to note is what happens when a child exits; What
   happens to its children in the process tree? To study this, let's create a
   specific example: ```./fork.py -A a+b,b+c,c+d,c+e,c-```. This example has
   process 'a' create 'b', which in turn creates 'c', which tehn creates 'd' and
   'e'. However, then, 'c' exits.What do you think the process tree should look
   like after the exit? What if you use the ```-R``` flag? Learn more about what
   happens to orphaned processes on your own to add more context.  

   I thought the  process would look like this:  

   ```
   a
   |_b
   
   --

   a
   |_b
	 |_c
   
   --

   a
   |_b
     |_c
	   |_d

   --

   a
   |_b
     |_c
	   |_d
	   |_e

   --

   a
   |_b
   ```

   but the last step is this:
   ```
   a
   |_b
   |_d
   |_e
   ```  
   Which means that if a fork exits but has children, the children of the exited
   fork get "adopted" by the root process.  
   Using ```-R``` causes random exits of other forks.  
---  

5. One last flag to explore is the `-F` flag, which skips intermediate steps and only asks to fill in the final process tree. Run `./fork.py -F` and see if you can write down the final tree by looking at the series of actions generated. Use different random seeds to try this a few times.  
---  
6. Finally, use both `-t` and `-F` together. This shows the final process tree, but then asks you to fill in the actions that took place. By looking at the tree, can you determine the exact actions that took place? In which cases can you tell? In which can’t you tell? Try some different random seeds to delve into this question.  

It's hard to tell when a fork is created and is exited. For example, if we have
simply:  

```
a
|_c
```
and we have 3 actions, we wouldn't abele to tell if the actions were ```a+b,a+c,b-``` or ```a+b,b+c,b-```.
  
# 
## Code  

1. Write a program that calls `fork()`. Before calling `fork()`, have the main process access a variable (e.g., `x`) and set its value to something (e.g., 100). What value is the variable in the child process? What happens to the variable when both the child and parent change the value of `x`?  
<br>The value of `x` in the child process is the same in the main process unless redefined in the child. If both the child and parent change the value of `x`, the changes are only reflected with the processes themselves and have no effect on the other processes.

[code: 1.c](https://github.com/breakthatbass/OStep/blob/main/chap5/1.c)

---
2. Write a program that opens a file (with the `open()` system call) and then calls `fork()` to create a new process. Can both the child and parent access the file descriptor returned by `open()`? What happens when they are writing to the file concurrently, i.e., at the same time?  
<br>Yes, both child and parent can access file descriptor. It seems the parent writes to the descriptor before the child does usually.

[code: 2.c](https://github.com/breakthatbass/OStep/blob/main/chap5/2.c)

---
3. Write another program using `fork()`. The child process should print “hello”; the parent process should print “goodbye”. You should try to ensure that the child process always prints first; can you do this *without* calling `wait()` in the parent?  
<br>It is possible. The `kill()` function can be used to send signals to specific processes even from other processes based on PIDs. I used the `kill()` function in the parent process right before the "goodbye" prints. Once paused, the child process continues and prints the "hello" string which then it encounters another `kill()` call which sends a signal to the parent process to continue. 

[code: 3.c](https://github.com/breakthatbass/OStep/blob/main/chap5/3.c)  

---
4. Write a program that calls `fork()` and then calls some form of `exec()` to
run the program `/bin/ls`. See if you can try all of the variants of `exec()`, including(on Linux)`execl()`, `execle()`, `execlp()`, `execv()`, `execvp()`, and `execvpe()`. Why do you think there are so many variants of the same basic call?  
<br>I'm not exactly sure. my guess is that it can be called on files and shell commands with the ability to not only give commands with each, but also the ability to define the environment (which I just have defined as `NULL`).  

[code: 4.c](https://github.com/breakthatbass/OStep/blob/main/chap5/4.c) 

---
5. Now write a program that uses `wait()` to wait for the child process to finish in the parent. What does `wait()` return? What happens if you use `wait()` in the child?  
<br>`wait()` return the pid of the process. Using `wait()` in the child returns `-1` which means it has no child processes. Nothing really happens in that case. It just moves on.  

[code: 5.c](https://github.com/breakthatbass/OStep/blob/main/chap5/5.c) 

---
6. Write a slight modification of the previous program, this time using `waitpid()` instead of `wait()`. When would `waitpid()` be useful?  
<br>This would be helpful to use when there are a lot of processes or if processes are getting paused or exiting since you can add options to dictate how `wait()` acts.  

[code: 6.c](https://github.com/breakthatbass/OStep/blob/main/chap5/6.c)

---
7. Write a program that creates a child process, and then in the child closes standard output (`STDOUT_FILENO`). What happens if the child calls `printf()` to print some output after closing the descriptor?  
<br>It doesn't print anything.

[code: 7.c](https://github.com/breakthatbass/OStep/blob/main/chap5/7.c)  

---
8. Write a program that creates two children, and connects the standard output of one to the standard input of the other, using the `pipe()` system call.  

[code: 8.c](https://github.com/breakthatbass/OStep/blob/main/chap5/8.c) 
