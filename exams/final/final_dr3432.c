#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdbool.h>
#include <time.h>

#define READ_END 0
#define WRITE_END 1

// Define number of days in year
#define NUM_DAYS_IN_YEAR 365

// Define number of trials per thread
#define NUM_TRIALS 1000000

// Define number of threads
#define NUM_PROCS 4


void workerProcess(int num_people, int *pipe_fd_prev, int *pipe_fd_next);
bool birthdaySimulation(int num_people);


int main (int argc, char* argv[]){
	if (argc != 2){
		perror("Not enough input variables\n");
		exit(1);
	}
	int num_people = atoi(argv[1]);
	if (num_people < 1){
		perror("Incorrect inputed value for n. n has to be greater than 1 (n>1).\n");
		exit(1);
	}

	// Set seed
	srand(time(NULL));
    pid_t x;

    int pipes_fd[NUM_PROCS][2]; // List of created pipes.
    for (int i = 0; i < NUM_PROCS + 1; i++){
        x = fork();
        if (x == 0){ // In child process.
            if (pipe(pipes_fd[i]) == -1){
                perror("Pipe creation failed.\n");
                exit(1);
            }
            // if i = 0 it is first child so no previous pipe found.
            if (i == 0){
                workerProcess(num_people, NULL, pipes_fd[i]);
            }
            else{
                workerProcess(num_people, pipes_fd[i - 1], pipes_fd[i]);
            }
        }
        
    }
	
    if (x != 0){
        int counter;
        read(pipes_fd[NUM_PROCS - 1][READ_END], &counter, sizeof(counter));
        close(pipe_fd_prev[READ_END]);
        double result = (double)counter / (NUM_THREADS * NUM_TRIALS);
        // Calculate and print result;
        printf("For %d students, Monte Carlo simulation estimated probability of shared birthday is: %lf\n"
                    , num_students, result);
    }

	return 0;
}


void workerProcess(int num_people, int *pipe_fd_prev, int *pipe_fd_next){
    int counter = 0;
    
    // Close read end of pipe if first entry
    if (pipe_fd_prev == NULL){
        close(pipe_fd_next[READ_END]);
    }
    else(
        read(pipe_fd_prev[READ_END], &counter, sizeof(counter));
        close(pipe_fd_prev[READ_END]);
    )

    for (int i = 0; i < NUM_TRIALS; i++){
		if (birthdaySimulation(num_people)){
			counter++;
		}
	}

    // Write counter to pipe
    write(pipe_fd_next[WRITE_END], &counter, sizeof(counter));

    // Close write end of pipe
    close(pipe_fd_next[WRITE_END]);


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