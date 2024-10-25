#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>

// Define number of days in year
#define NUM_DAYS_IN_YEAR 365

// Define number of trials per thread
#define NUM_TRIALS 10000

// Define number of threads
#define NUM_THREADS 4

// Use static key word for global shared counter
// This counter will be what we need to protect using mutex or semaphores
int counter;

//Create shared semaphore 
sem_t sem;

// For WorkerThread call trial function and add 1 to counter if found, if not continue.
// Takes single int as parameter 
void* WorkerThread(void * pvar);

bool birthdaySimulation(int num_people, unsigned int *seed);

int main(int argc, char *argv[]){
	if (argc != 2){
		perror("Incorrect number of function args.\n Usage: ./lab7 <positive int>\n");
		exit(1);
	}
	int num_students = atoi(argv[1]);
	if (num_students < 1){
		perror("Non positive integer given as input.\n Usage: ./lab7 <positive int>\n");
		exit(1);
	}
	
	//Initalize semaphore
	sem_init(&sem, 0, 1);

	//Initialize counter to 0
	counter = 0;
	
	//Innitizliaze pthread array to keep track of threads.
	pthread_t threads[NUM_THREADS];
	for (int i = 0; i < NUM_THREADS; i++){
		if (pthread_create(&threads[i], NULL, WorkerThread, &num_students)){
			// If pthread create returned error, mark error.
			perror("Error while creating thread.\n");
			exit(1);
		}
	}
	
	// Wait for all threads to end
	for (int i = 0; i < NUM_THREADS; i++){
		pthread_join(threads[i], NULL);
	}

	// Destroy Semaphore
	sem_destroy(&sem);

	double result = (double)counter / (NUM_THREADS * NUM_TRIALS);
	// Calculate and print result;
	printf("For %d students, Monte Carlo simulation estimated probability of shared birthday is: %lf\n"
				, num_students, result);

	return 0;
}


void* WorkerThread(void *pvar){
	int num_people = *((int *) pvar);	
	// Set seed for thread
	unsigned int seed = (unsigned int) time(NULL) + pthread_self();
	for (int i = 0; i < NUM_TRIALS; i++){
		if (birthdaySimulation(num_people, &seed)){
			// Acquire semaphore
			sem_wait(&sem);
			
			// Critical Section, add to counter
			counter++;

			// Release Semaphore
			sem_post(&sem);
		}
	}

	return NULL;
}

bool birthdaySimulation(int num_people, unsigned int *seed){
		bool foundBirthdays[NUM_DAYS_IN_YEAR] = { false };
		int random_day;
		for (int i = 0; i < num_people; i++){
			random_day = rand_r(seed) % NUM_DAYS_IN_YEAR;
			if (foundBirthdays[random_day]){
				return true;
			}
			else
				foundBirthdays[random_day] = true;
		}

	
	return false;
}

