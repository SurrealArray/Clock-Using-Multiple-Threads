#include "clock.h"

void main(int argc, char *argv[]){		
	
	//converts string arguments to integers if arguments are valid 
	//parameters that aren't inputted will revert to default
	switch(argc){ //switch case based on the number of arguments
	
		case 2: //converts only the first parameter
			if(atoi(argv[1]) > 0) arg1 = atoi(argv[1]);	
			break;
			
		case 3: //converts the first and second parameter
			if(atoi(argv[1]) > 0) arg1 = atoi(argv[1]);	
			if(atoi(argv[2]) == 1 || atoi(argv[2]) == 60) arg2 = atoi(argv[2]);
			else{
				printf("Input error: must enter a 1 or 60 for parameter 2\n");
				exit(0);
			}
			break;
			
		case 4: //converts the first, second, and third parameter
			if(atoi(argv[1]) > 0) arg1 = atoi(argv[1]);	
			if(atoi(argv[2]) == 1 || atoi(argv[2]) == 60) arg2 = atoi(argv[2]);
			else{
				printf("Input error: must enter a 1 or 60 for parameter 2\n");
				exit(0);
			}
			if(atoi(argv[3]) >= 0) arg3 = atoi(argv[3]);
			break;
			
	}
	//when too many parameters are entered program will exit gracefully
	if(argc > 4){
		printf("Input error: too many arguments\n");
		printf("Sample acceptable input: ./mot 3 1 2\n");
		exit(0);
	}
	
	//creats main thread and waits for it to finish before exiting program
	pthread_create(&threads[0], NULL, (void *)mainThread, NULL);	
	pthread_join(threads[0],NULL);
}

//Main Thread
//signal handler and worker threads creator
void *mainThread(int signalInterrupt){
	
	//terminates all when signaled by workerThread2 (countdown timer)
	if(signalInterrupt > 0){
		
		pthread_cancel(threads[3]);	
		pthread_cancel(threads[1]);	
		pthread_cancel(threads[2]);
		
	}
	//runs this initially to create and wait for threads
	else{
		
		//creates worker threads
		pthread_create(&threads[1], NULL, workerThread1, NULL);
		pthread_create(&threads[2], NULL, workerThread2, NULL);
		pthread_create(&threads[3], NULL, workerThread3, NULL);
		
		//waits for threads to finish or get terminated
		pthread_join(threads[1],NULL);
		pthread_join(threads[2],NULL);
		pthread_join(threads[3],NULL);
		
		printf("All threads completed! Good bye! :)\n");
		exit(0); //main thread closes the program/process as specified in instructions
	}
}

//Alarm
//monitor timer print alarm
void *workerThread1(){

	//fixes bug that allows alarm to display when arg3 is one greater than arg1
	if(arg3 > arg1) pause(); //pauses thread until terminated by the main thread
	
	//signal alarm that is sent to alarm signal handler function to print alarm 
	signal(SIGALRM, alarmFunc);
	alarm(arg3);
}

//Countdown Timer
//signals to the main thread to terminate other threads when time is done
void *workerThread2(){

	int i; //variable to keep track of how many seconds have passed
	
	signal(SIGINT, (void *)mainThread); //registers signal handler

	//countdown timer 
	for(i = 0; i <= arg1; i++){
		sleep(1);
	}

	raise(SIGINT); //initiates signal to signal handler/main thread to terminate threads

}

//Wall Clock
//print time of day based on user given interval
void *workerThread3(){
	
	int i = 0;
	
	//should go forever until the thread is terminated by the main thread
	while(1){
		
		sleep(arg2); //only works with 1 or 60 second time intervals
		i++; //counts the number of seconds that have past
		
		time_t t = time(NULL);
		struct tm tm = *localtime(&t); 
		
		//fixes bug that allows wall clock to display one too many times
		if(i > arg1) pause(); //pauses thread until terminated by the main thread

		//displays "pm" time 
		if(tm.tm_hour > 12){
			tm.tm_hour -= 12;
			char period[] = "pm";
			printf("%02d:%02d:%02d %s\n", tm.tm_hour, tm.tm_min, tm.tm_sec, period);
		}
		//displays "am" time
		else{
			if(tm.tm_hour == 0) tm.tm_hour = 12;
			char period[] = "am";
			printf("%02d:%02d:%02d %s\n", tm.tm_hour, tm.tm_min, tm.tm_sec, period);
		}
	}
}

//Alarm signal handler 
//prints alarm
void alarmFunc(int signalAlarm){
	printf("ALARM! %d seconds have passed\n", arg3);
}
