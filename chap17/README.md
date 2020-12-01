## Chapter 17
# Free-Space Management

1. First run with the flags `-n 10 -H 0 -p BEST -s 0` to generate a few random allocations and frees. Can you predict what `alloc()`/`free()` will return? Can you guess the state of the free list after each request? What do you notice about the free list over time?

    - it gets increasingly fragmented as more and more calls are made.

2. How are the results different when using a WORST fit policy to search the freelist(`-p WORST`)? What changes?
    - it'll allocate in the largest possible sections in the free list.

3. What about when using `FIRST` fit (`-p FIRST`)? What speeds up when you use first fit?
    - the amount of elements searched is less.

4. For the above questions, how the list is kept ordered can affect the time it takes to find a free location for some of the policies. Use the different free list orderings(`-l ADDRSORT`,`-l SIZESORT+`,`-l SIZESORT-`) to see how the policies and the list orderings interact.

5. Coalescing of a free list can be quite important. Increase the number of random allocations (say to `-n 1000`). What happens to larger allocation requests over time? Run with and without coalescing (i.e., without and with the `-C` flag). What differences in outcome do you see? How big is the free list over time in each case? Does the ordering of the list matter in this case?
    - over time many allocation requests can't be fulfilled as the segments are too fragmented. The free list gets massive.
    - a colaesced list stays relatively small and therefore has to do a lot less searching for segments. 

6. What happens when you change the percent allocated fraction `-P` to higher than 50? What happens to allocations as it nears 100? What about as the percent nears 0?

    - the higher the allocation fraction is, the less likely the allocations are to being freed. So if it's higher, over time it becomes more and more likely that the requested memory can't be allocated since there often isn't space left.

7. What kind of specific requests can you make to generate a highly-fragmented free space? Use the `-A` flag to create fragmented free lists, and see how different policies and options change the organization of the free list.