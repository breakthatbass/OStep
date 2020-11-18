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
