# Interlude: Process API

### Simulation

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

   The process will look like this:  

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






### Code  
