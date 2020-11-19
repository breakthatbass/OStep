# The Abstraction: The Process  

### Simulation  

#### Questions  
1. Run ```process-run.py``` with the following flags: ```-l 5:100, 5:100```. What should the CPU utilization be (e.g., the percent of time the CPU is in use?) Why do you know this? Use the ```-c``` and ```-p``` flags to see if you were right.  
   <br>
   The CPU utilization is 100%. There are no IO processes. The ```-c``` and ```-p``` flags return a little more info on the processes:  
   ```
   Stats: Total Time 10
   Stats: CPU Busy 10 (100.00%)
   Stats: IO Busy  0 (0.00%)
   ```  

2. Now run with these flags: ```./process-run.py -l 4:100,1:0```. These flags
   specify one process with 4 instructions (all to use the CPU), and one that
   simply issues an I/O and waits for it to be done. How long does it take to
   complete both processes? Use ```-c``` and ```-p``` to see if you were right.  

   It takes the same amount of time as the previous question, 10. After the I/O
   call, it waits to finish which gives that process a total time of 5 even
   though it was inserted as ```1:0```. And it ends with the ```DONE``` signal
   from the IO call adding a sixth.  

3. Switch the order of the process: ```.process-run.py -l 1:0,4:100```. What
   happens now? Does switching the order matter? Why? (As always use ```-c```
   and ```-p``` to see if you were right).  

   Running with just the ```-l``` flags shows that it works the same as the
   previous invocation but flipped. However, a good assumption with this would
   be that since the I/O would be made before the CPU call, while the I/O is
   waiting, it would most likely then run the CPU call, probably resulting in
   less time. Let's see.

   Yes. It had a total time of 6 and does indeed run the CPU calls while waiting
   for the I/O to do its thing. Here's what it returns:  

   ```
   Time    PID: 0    PID: 1       CPU       IOs
     1     RUN:io     READY         1          
	 2    WAITING   RUN:cpu         1         1
	 3    WAITING   RUN:cpu         1         1
	 4    WAITING   RUN:cpu         1         1
	 5    WAITING   RUN:cpu         1         1
	 6*      DONE      DONE         1

	 Stats: Total Time 6
	 Stats: CPU Busy 6 (100.00%)
	 Stats: IO Busy  4 (66.67%)
   ```  

4. We'll now explore some of the other flags. One important flag is ```-S```,
   which determines how the system reacts when a process issues and I/O. With
   the flad set to ```SWITCH_ON_END```, the system will NOT switch to another
   process while one is doing I/O, instead waiting until the process is
   completely finished. What happens when you run the following two processes
   (```-l 1:0,4:100 -c -S SWITCH_ON_END```), one doing I/O and the other doing
   CPU work?  

   When using ```SWITCH_ON_END``` it only affects processes that make I/O calls.
   It simply prevents other processes from running while waiting for the I/O to
   finish. It makes no difference with CPU work since that already only runs
   once process at a time.  

5. Now, run the same processes, but with the switching behavior set to switch to
   another process whenever one is ```WAITING``` for I/O (```-l 1:0,4:100 -c -S
   SWITCH_ON_IO```). What happens now? Use ```-c``` and ```-p``` to confirm that
   you are right.  

   This seems to work the same as default where it will switch processes while
   waiting on I/O.   

6. One other important behavior is what to do when an I/O completes. With ```-I
   IO_RUN_LATER```, when an I/O completes, the process that issued it is not
   necessarily run right away; rather, whatever was running at the time keeps
   running. What happens when you run this combination of processes? (Run
   ```./process-run.py -l 3:0,5:100,5:100,5:100 -S SWITCH_ON_IO -I IO_RUN_LATER
   -c -p```) Are system resources being effectively utilized?  

	```
	   Stats: Total Time 27
	   Stats: CPU Busy 19 (70.37%)
	   Stats: IO Busy  12 (44.44%)
	```
	
	System resources are not effectively utilized.There are 3 I/O calls in the
	first process and then 3 CPU processes. It first calls the first I/O, while
	it waits it runs the first CPU call and then continues to run the rest of
	the CPU calls before going back to the I/O calls in the first process. It
	then calls those I/Os and has a lot of waiting.  

7. Now run the same processes, but with ```-I IO_RUN_IMMEDIATE``` set, which
   immediately runs the process that issued the I/O. How does this behavior
   differ? Why might running a process that just completed an I/O again be a
   good idea?

   The behavior differs in that it prioritizes running the I/O process over the
   others. This is a good idea because once the I/O is done waiting, it
   immediately calls the next I/O and while waiting for each one, run thes CPU
   processes. Ultimately having a total time of 18 compared to 27 in the last
   question.  

8. Now run with some randomly generated processes: ```-s 1 -l 3:50,3:50``` or
   ```-s 2 -l 3:50,3:50``` or ```-s 3 -l 3:50,3:50```. See if you can predict
   how the trace will turn out. What happens when you use the flag ```-I
   IO_RUN_IMMEDIATE``` vs ```-I IO_RUN_LATER```? What happens when you use
   ```-S SWITCH_ON_IO``` vs ```-S SWITCH_ON_END```  

   There's not much of a difference besides the fact that ```SWITCH_ON_IO```
   results in less time since it prioritizes running I/O and then runs other
   processes while waiting.  

