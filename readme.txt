Versioning:
-----------------------------
1.0: Initial launch version.
		21.10.2016 - Gaal Alexandru
		Round robin scheduler
		Blocked and sleeping main threads
		Periodic threads / events
		FIFO
		Main thread syncronization

Copyright & License:
-----------------------------
1) Gaal Alexandru:
This RTOS was developed during the course UTAustinX: UT.RTBN.12.01x Realtime Bluetooth Networks.
Some of the code was copied from the examples mentioned in 2)
You may use, edit, run or distribute this file as long as the copyright notice remains.

2) Reference & Source:
This example accompanies the book
   "Embedded Systems: Real Time Interfacing to ARM Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2016

   "Embedded Systems: Real-Time Operating Systems for ARM Cortex-M Microcontrollers",
   ISBN: 978-1466468863, , Jonathan Valvano, copyright (c) 2016
   Programs 4.4 through 4.12, section 4.2

Copyright 2016 by Jonathan W. Valvano, valvano@mail.utexas.edu
You may use, edit, run or distribute this file
as long as the above copyright notice remains
THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
For more information about my classes, my research, and my books, see
http://users.ece.utexas.edu/~valvano/

Configuration instructions:
-----------------------------
1) Include os.h in your project
2) Include BSP.h in your project
3) In os.c edit number of main end event threads: NUMTHREADS & NUMPERIODIC
4) Maximum frequency for event threads is 1000Hz.
5) Configure OS_AddThreads so it will take NUMTHREADS main threads. Update function definition and circular list, last member points to first member.

main.c usage example:
-----------------------------
1) The OS implementation in main.c has 6 main threads and 2 event threads.
2) OS_AddPeriodicEventThread registers the event threads with the desired period in this case 10ms and 100ms.
3) More event threads are possible, just modify NUMPERIODIC in os.c and make sure to register all.
4) OS_AddThreads registers the main threads, always make sure to have one idle thread (not blocked and not sleeping)
5) More main threads are possible, just modify NUMTHREADS in os.c and make sure to register all and reconfigure the linked list in OS_AddThreads.
6) In this example Event0 is a producer and puts data to Fifo A. Event1 is a producer and puts data to Fifo B.
7) Task0 and Task1 are consumers and get data from Fifo A. Task2 is a consumer and gets data from Fifo B.
8) Task3 and Task4 are syncronized by Task34Sync and Task43Sync
10) OS_Launch will launch the OS, use the arguments as in the example.

Debugging:
-----------------------------
1) By logic analyser / osciloscope:
	The Profile.c and profile.h 3rd party files define debugging pins to measure with scope / logic analyser
	The following pins were selected to be Profile pins:
		J3.27/PE1 Profile 0
		J3.28/PE2 Profile 1
		J3.29/PE3 Profile 2
		J3.30/PF1 Profile 3
		J2.18/PE0 Profile 4
		J4.36/PC5 Profile 5
		J4.34/PC7 Profile 6

2) With the debugger:
	Count0;   // number of times Task0 loops
	Count1;   // number of times Task1 loops
	Count2;   // number of times Task2 loops
	Count3;   // number of times Task2 loops
	Count4;   // number of times Task2 loops
	CountIdle;   // number of times Task2 loops
	CountEv0 = 0;   // number of times Task2 loops
	CountEv1 = 0;   // number of times Task2 loops

3) In the main.c example you can check correct functioning with:
	Count0 ~ Count1 ~ CountEv0
	Count2 ~ CountEv1
	Count3 ~ Count4