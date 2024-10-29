#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <semaphore.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

// Define number of days in year
#define NUM_DAYS_IN_YEAR 365

// Define number of trials per process
#define NUM_TRIALS 1000000

// Define number of created processes
#define NUM_PROCS 4

// This counter will be what we need to protect using mutex or semaphores
int counter;

//Create shared semaphore 
#define SEM_NAME "/mysem"

// For WorkerThread call trial function and add 1 to counter if found, if not continue.
// Takes single int as parameter 
void WorkerProcess(int num_people);

// Single simulation function
bool birthdaySimulation(int num_people);

int main(int argc, char *argv[]){
	if (argc != 2){
		perror("Incorrect number of function args.\n Usage: ./lab7 <positive int>\n");
		exit(1);
	}
	int num_people = atoi(argv[1]);
	if (num_people < 1){
		perror("Non positive integer given as input.\n Usage: ./lab7 <positive int>\n");
		exit(1);
	}
	
	// Create shared semaphore
	sem_t *sem = sem_open(SEM_NAME, O_CREAT, 0644, 1);

	//Initialize counter to 0
	counter = 0;
	
	//Create processes.
	pid_t c1 = -1, c2 = -2, c3 = -3, c4 = -4;
	c1 = fork();

	if (c1 < 0){ // Thread creation failed
		perror("Fork failed\n");
		exit(1);
	}
	if (c1 == 0) { // In c1
		// Run c1 simulation
		WorkerProcess(num_people);
	}
	else {// In parent
		c2 = fork();
		if (c2 < 0 ){
			perror("Fork failed\n");
			exit(1);
		}
		if (c2 == 0){//In c2
			c3 = fork();
			if (c3 < 0){
				perror("Fork failed\n");
				exit(1);
			}
			if (c3 == 0){//In c3
				c4 = fork();
				if (c4 < 0){
					perror("Fork failed\n");
					exit(1);
				}
				if (c4 == 0){//In c4
					// Run c4 simulation
					WorkerProcess(num_people);
				}
				else{
					// Run c3 simulation.
					WorkerProcess(num_people);
					wait(NULL); // Wait for process 4 to finish
				}
			}
			else {// In c2
				// Run c2 simulation.
				WorkerProcess(num_people);
				wait(NULL); // Wait for process 3 to finish.
			}
		}
		else { //Still in parent

			// Wait for all children to end (c1 and c2)
			while (wait(NULL) > 0);

			// Calculate and print result;
			printf("Unable to properly communicate semaphore state between processes.\n");
			double result = (double)counter / (NUM_PROCS * NUM_TRIALS);
			printf("For %d students, Monte Carlo simulation estimated probability of shared birthday is: %lf\n"
				, num_people, result);

			// Destroy Semaphore
			sem_destroy(sem);
		}
	}

	
	return 0;
}


void WorkerProcess(int num_people){
	sem_t *sem = sem_open(SEM_NAME, 1);
	// Set seed for process
	srand(time(NULL) + getpid());
	for (int i = 0; i < NUM_TRIALS; i++){
		if (birthdaySimulation(num_people)){
			// Acquire semaphore
			sem_wait(sem);
				
			// Critical Section, add to counter
			counter++;
			printf("%d\n", counter);

			// Release Semaphore
			sem_post(sem);
		}
	}
	return;
}

bool birthdaySimulation(int num_people){
		bool foundBirthdays[NUM_DAYS_IN_YEAR] = { false };
		int random_day;
		for (int i = 0; i < num_people; i++){
			random_day = rand() % NUM_DAYS_IN_YEAR;
			if (foundBirthdays[random_day]){
				return true;
			}
			else
				foundBirthdays[random_day] = true;
		}

	
	return false;
}

