Configuration instructions:
-----------------------------
1) Include os.h in your project
2) Include BSP.h in your project
3) In os.c edit number of main end event threads: NUMTHREADS & NUMPERIODIC
4) If smallest sleep / event thread period is smaller than 1ms (1000 Hz), update in function OS_Init the calls for function BSP_PeriodicTask_Init
5) Configure OS_AddThreads so it will take NUMTHREADS main threads. Update function definition and circular list, last member points to first member.
6) 
