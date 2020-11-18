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

2. Now run with these flags: ```.process-run.py -l 4:100,1:0```. These flags
   specify one process with 4 instructions (all to use the CPU), and one that
   simply issues an I/O and waits for it to be done.  

   How long does it take to complete both processes? Use ```-c``` and ```-p```
   to find out if you were right.  





