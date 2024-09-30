#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>


int main (int argc, char* argv[]){
	pid_t c1 = -1, c2 = -2, c3 =-3, c4 = -4;
	
	// Create initial fork with child c1.
	c1 = fork();
	
	// If returned value is negative (i.e. fork failed) exit.
	if (c1 < 0){
		perror("Fork failed\n");
		exit(1);
	} 
	// If c1 == 0 (i.e. in child process of parent) run below code
	if (c1 == 0){
		// Since we are in child process of parent, create new fork c3 as child 3.
		c3 = fork();
		// If returned value is negative (i.e. fork failed) exit.
		if (c3 < 0){
			perror("Fork failed\n");
			exit(1);
		}
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
			// Since we are in child process c2, create a new fork c4 as child 4.
			c4 = fork();
			// I returned value is negative (i.e. fork failed) exit.
			if (c4 < 0){
				perror("Fork failed\n");
				exit(1);
			}
		}
	}

	return 0;
}

// Resulting Structure:
/*
Parent
|
|-------Child 1 (Created initially)
|					|
|					|--------Child 3 (Created after confirming inside c1)
|
|
|-------Child 2 (Created at else block of first if-else)
|					|
|					|--------Child 3 (Created after confirming inside c2)
*/

