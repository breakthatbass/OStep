# Scheduling: Introduction

1. Compute the response time and turnaround time when running three jobs of length 200 with the SJF and FIFO schedulers.

    ```
    FIFO & SJF:  
    response time: 0, 200, 400  
    turnaround time: 200, 400, 600
    ```
    <br>
2. Now do the same but with jobs of different lengths: 100, 200, & 300.
    ```
    FIFO:
    response time: 0, 100, 300
    turnaround time: 100, 300, 600

    SJF:
    response time: 0, 100, 300
    turnaround time: 100, 300, 600
    ```  
    <br>
3. Now do the same, but also with the RR scheduler and a time-slice of 1.
    ```
    RR:
    response time: 0, 1, 2
    turnaround time: 598, 599, 600
    ```
    <br>
4. For what types of workloads does SJF deliver the same turnaround times as FIFO?
    - When the jobs are all of the same length or if the FIFO jobs come in shortest to largest.
    <br>
5. For what types of workloads and quantum lengths does SJF deliver the same response times as RR?
    - jobs that all have the same length and where the time-silce is the same as the job length.
    <br>
6. What happens to repsonse time with SJF as job lengths increase? Can you use the simulator to demonstrate the trend?
    - Response time increases as job lengths increase.
    <br>
7. What happens to response time with RR as quantum lengths increase? Can you write an equation that gives the worst-case response time, given *N* jobs?
    - Response time increases.

