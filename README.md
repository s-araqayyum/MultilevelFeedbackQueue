# Multilevel Feedback Queue

Your task is to simulate the working of different scheduling algorithms. Your program must
implement a multi-level feedback queue for implementing these algorithms. 
Steps:
1. Your simulation program must ask the user; how many algorithms you want to
implement.
2. After that, creates that number of queues. 
3. Each queue implements a different scheduling algorithm.
4. For consistency, you should follow the following given sequence for implementing
queues arranged in high-to-low priority:
i. Priority Scheduling
ii. Round Robin (Quantum=8)
iii. Round Robin (Quantum=10)
iv. First Come First Serve (FCFS)
5. For Simplicity, you should implement the swapping between queues only from high-to-
low priority queues not from low priority to high priority.
6. Your program must take/read processes from the input file for scheduling.
7. The file format is as; PID | Arrival Time | CPU Burst Time | Priority. All these are
separated by “ ” space. Each process entry is in a new line.
8. The priority value, higher value high priority, is useful for priority-based algorithms and
optional for others.
9. Upon successful scheduling of processes on all queues, your program must print the
following for each implemented queue/scheduling algorithm:

i. All processes with their PID, arrival time, CPU burst time, waiting time, and
turnaround time.

ii. Plus, the average waiting time and average turnaround time of each scheduling
algorithm.
