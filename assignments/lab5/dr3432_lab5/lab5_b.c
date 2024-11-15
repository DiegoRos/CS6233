#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>

#define READ_END 0
#define WRITE_END 1

void fibonacciGenerate(int n, int *pipe_fd);
void fibonacciPrint(int n, int *pipe_fd);

int main (int argc, char* argv[]){
	if (argc != 2){
		perror("Not enough input variables\n");
		exit(1);
	}
	int n = atoi(argv[1]);
	if (n < 1){
		perror("Incorrect inputed value for n. n has to be greater than 1 (n>1).\n");
		exit(1);
	}

	// Pipe Creation
	int pipe_fd[2];
	if (pipe(pipe_fd) == -1){
		perror("Pipe creation failed.\n");
		exit(1);
	}

	// Set seed
	srand(time(NULL));

	pid_t x;
	x = fork();

	// Check if fork is unsuccesfull.
	if (x < 0){
		perror("Fork was unsucessfull, please try again.\n");
		exit(1);
	}
	//Check to see that we are inside of child process. If so, run fibonacci.
	if (x == 0) fibonacciGenerate(n, pipe_fd);
	else fibonacciPrint(n, pipe_fd);


	return 0;
}


void fibonacciGenerate(int n, int *pipe_fd){
	// Close read end of pipe
	close(pipe_fd[READ_END]);

	long long fib_2 = 0, fib_1 = 1;
	long long fib;

	for (int i = 0; i <= n; i++){
		if (i == 0){
			fib = fib_2;
		}
		else if (i == 1){
			fib = fib_1;
		}
		else{
			fib = fib_2 + fib_1;
			fib_2 = fib_1;
			fib_1 = fib;
		}
		// Wait random time between 0 and 3 seconds
		usleep((rand() % 3000) * 1000);

		// Insert fib of n into pipe
		write(pipe_fd[WRITE_END], &fib, sizeof(fib));
		
	}
	close(pipe_fd[WRITE_END]);

	return;
}

void fibonacciPrint(int n, int *pipe_fd){
	// Close write end of pipe
	close(pipe_fd[WRITE_END]);

	int numbers_printed = 0;
	long long fib;
	
	printf("Parent printing fibonnacci sequence of %d:\n", n);

	while (numbers_printed <= n){
		// Read from pipe
		read(pipe_fd[READ_END], &fib, sizeof(fib));

		printf("%llu\n", fib);
		fflush(stdout);
		numbers_printed++;
	}

	// Close read end of pipe
	close(pipe_fd[READ_END]);
	return;
}
