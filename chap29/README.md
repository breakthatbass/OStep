For all the questions below on timing and plotting the times I used a **2.7 GHz Dual-Core Intel Core i5** processor with **macOS 11.1** (Big Sur).

1. `gettimeofday()` works for me.
2. Now, build a simple concurrent counter and measure how long it takes to increment the counter many times as the number of threads increases. How many CPUs are available on the system you are using? Does this number impact your measurements at all?

    `$ sysctl hw.physicalcpu` returns 2 CPUs, however, `sysconf(_SC_NPROCESSORS_ONLN)` returns 4. I imagine I have two physical cores and 4 virtualized cores. I am sure this number impacts my measurements but I don't have another system to test it on.

    ```
    threads: 1
    count: 1000000
    time: 0.028296

    threads: 2
    count: 2000000
    time: 0.122237

    threads: 3
    count: 3000000
    time: 0.193724

    threads: 4
    count: 4000000
    time: 0.283017
    ```

3. Next, build a version of the sloppy counter. Once again, measure its performance as the number of threads varies, as well as the threshold. Do the numbers match what you see in the chapter?

    In the book a threshold of 1 took about 12 seconds to complete. A threshold of 2 took about 6 seconds. My program (copied from the book) took 0.6 seconds with a threshold of 1 and just under a half of a second for a threshold of 2. And starts to match the book around a threshold of 32. I have a similar processor archtiecture to the one used in the book, however, I believe the book's OS is Linux. So I imagine that might have something to do with the difference.

    ![approximation](plot.png)