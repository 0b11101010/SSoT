# README #

Scheduling Simulation over Threads

Objective : To design a scheduler and a process control block (pcb), pcb-table and queue structures of the sample OS using threads, to demonstrate the correct operation of scheduler on at least three processes simulated by threads.

### What is this repository for? ###

http://www.baskent.edu.tr/~hogul/project_assign.pdf

### How do I get set up? ###

erhan@erhan-GE72-7RD:~/src/ssot$ make
gcc -g -Wall -c SSoT.c -o SSoT.o
gcc -g -Wall -c SSoT_Scheduler.c -o SSoT_Scheduler.o
gcc -g -Wall -c SSoT_ControlBlock.c -o SSoT_ControlBlock.o
gcc -g -Wall -c SSoT_Queue.c -o SSoT_Queue.o
gcc  SSoT.o  SSoT_Scheduler.o  SSoT_ControlBlock.o  SSoT_Queue.o -Wall -lm -lpthread -o SSoT

erhan@erhan-GE72-7RD:~/src/ssot$ ./SSoT

Thread_H1 is WORKING - 0

Thread_H1 will wait IO - 1

Thread_l1 is WORKING

			0,1,2,3,4,5,6,7,8,9
			

### Contribution guidelines ###
### Who do I talk to? ###

* Repo owner or admin
* Other community or team contact