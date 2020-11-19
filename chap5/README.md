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
   

   The higher the the ```fork_percentage``` is, there is more forks being
   created and less ```EXITS``` so the resulting process trees end up looking
   pretty complicated with all sort of forks off of forks, etc. With a lower
   ```fork_percentage```, the trees look simpler since so many of the forks end
   up getting exited.  
---



### Code  
