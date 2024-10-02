#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main (int argc, char* argv[]){
	pid_t c1 = -1, c2 = -2, c3 =-3, c4 = -4;
	
	// ID of root process:
	printf("Process ID of root: %d\n", getpid());

	// Create initial fork with child c1.
	c1 = fork();	
	// If returned value is negative (i.e. fork failed) exit.
	if (c1 < 0){
		perror("Fork failed\n");
		exit(1);
	} 
	// If c1 == 0 (i.e. in child process of parent) run below code
	if (c1 == 0){
		printf("Parent ID of process C1:%d \t Process ID of Process C1: %d\n", getppid(), getpid());
		// Since we are in child process of parent, create new fork c3 as child 3.
		c3 = fork();
		// If returned value is negative (i.e. fork failed) exit.
		if (c3 < 0){
			perror("Fork failed\n");
			exit(1);
		}
		if (c3 == 0)
			printf("Parent ID of process C3:%d \t Process ID of Process C3: %d\n", getppid(), getpid());
		
		else
			wait(NULL); // c1 waits for child process c3 to finish.

	}
	
	// If process not child of parent (i.e. still in parent process)
	else{

		// Create c2 fork as child of parent process.
		c2 = fork();
		// I returned value is negative (i.e. fork failed) exit.
		if (c2 < 0){
			perror("Fork failed\n");
			exit(1);
		}
		// If c2 == 0 (i.e. we are in child process c2) run below code
		if (c2 == 0){
			printf("Parent ID of process C2:%d \t Process ID of Process C2: %d\n", getppid(), getpid());
			// Since we are in child process c2, create a new fork c4 as child 4.
			c4 = fork();
			// I returned value is negative (i.e. fork failed) exit.
			if (c4 < 0){
				perror("Fork failed\n");
				exit(1);
			}
			if (c4 == 0)
				printf("Parent ID of process C4:%d \t Process ID of Process C4: %d\n", getppid(), getpid());

			else
				wait(NULL); // Tells process c2 to wait for process c4 to finish.
		}

		else {
			// Wait for processes c1 and c2 to finish.
			wait(NULL);
			wait(NULL);
		}
	}
	return 0;
}

// Resulting Structure:
/*
Parent
|
|-------Child 1 (Created initially)
|         |
|         |--------Child 3 (Created after confirming inside c1)
|
|
|-------Child 2 (Created at else block of first if-else)
|         |
|         |--------Child 3 (Created after confirming inside c2)
*/

