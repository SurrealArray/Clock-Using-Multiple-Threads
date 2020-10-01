#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <signal.h>
#include <stdint.h>
#include <unistd.h>

void *mainThread(int signalInterrupt);
void *workerThread1();
void *workerThread2();
void *workerThread3();
void alarmFunc(int signalAlarm); //alarm signal handler

int arg1 = 25; //default parameter 1
int arg2 = 1; //default parameter 2
int arg3 = 17; //default parameter 3
	
pthread_t threads[4]; //4 threads that will run