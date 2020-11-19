# Interlude: Process API

Skip to [code section](https://github.com/breakthatbass/OStep/tree/main/chap5#code)

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
  

---  

### Code  
