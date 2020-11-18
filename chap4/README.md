# The Abstraction: The Process  

### Simulation  

#### Questions  
1. Run ```process-run.py``` with the following flags: ```-l 5:100, 5:100```.  
   
   What should the CPU utilization be (e.g., the percent of time the CPU is in
   use?) 
   
   It looks like the CPU utilization is 100% in both processes. Here's what it
   returns:  
   ```
   Produce a trace of what would happen when you run these processes:
   Process 0
     cpu
	 cpu
	 cpu
	 cpu
	 cpu

	Process 1
	 cpu
	 cpu
	 cpu
	 cpu
	 cpu

	Important behaviors:
	 System will switch when the current process is FINISHED or ISSUES AN IO
	 After IOs, the process issuing the IO will run LATER (when it is its turn)
   ```  

   Why do you know this? Use the ```-c``` and ```-p``` flags to see if you
   were right.  

   I know this because of the usage of the program that prints out. It says:  

   ```Must be number <x:y>...Y is the percent change that an instruction is CPU not
   IO.```  

   Using the ```-c``` and ```-p``` flags returns more info:  

   ```
   Time    PID: 0    PID: 1       CPU       IOs
     1    RUN:cpu     READY         1          
	 2    RUN:cpu     READY         1          
     3    RUN:cpu     READY         1          
	 4    RUN:cpu     READY         1          
	 5    RUN:cpu     READY         1          
     6       DONE   RUN:cpu         1          
	 7       DONE   RUN:cpu         1          
	 8       DONE   RUN:cpu         1          
	 9       DONE   RUN:cpu         1          
	10       DONE   RUN:cpu         1          

   Stats: Total Time 10
   Stats: CPU Busy 10 (100.00%)
   Stats: IO Busy  0 (0.00%)
   ```  

2. Now run with these flags: ```.process-run.py -l 4:100,1:0```. These flags
   specify one process with 4 instructions (all to use the CPU), and one that
   simply issues an I/O and waits for it to be done.  

   How long does it take to complete both processes? Use ```-c``` and ```-p```
   to find out if you were right.  





