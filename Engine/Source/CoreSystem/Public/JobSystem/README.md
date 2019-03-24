# Job System

Advanced thread management with followed memory logic for runnable tasks in pool.
Presents new type of IRunnable object called IJob with associated with that heap
memory allocation.

## Tagged heap allocator

Pool allocator with blocks of fixed size (2 MiB) and synchronization for allocation.
Marks explicitly each allocated block - introduces categories of allocation. Allows to
clear ALL BLOCKS in the chosen category at once.

## Job

Runnable object with stack and heap allocations. Stack - thread stack, heap - allocations
achieved via passing to each job pointer (reference) to executor thread linear allocator.

## Thread pool

Each thread has its linear memory allocator. No synchronization in allocation in thread. 
If linear block is exhausted new on could be allocated in tagged heap. Note: to clear call
in time of execution of current job. Moreover, all the allocated memory will be freed only 
when current state (frame, system) are processed.