#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

void fibonacciChild(int n);
void fibonacciParent(int n);

int main (int argc, char* argv[]){
	if (argc != 2){
		perror("Not enough input variables\n");
		exit(1);
	}
	int n = atoi(argv[1]);
	if (n < 2){
		perror("Incorrect inputed value for n. n has to be greater than 2 (n>2)\n");
		exit(1);
	}

	pid_t x;
	x = fork();

	// Check if fork is unsuccesfull.
	if (x < 0){
		perror("Fork was unsucessfull, please try again.\n");
		exit(1);
	}
	//Check to see that we are inside of child process. If so, run fibonacci.
	if (x == 0) fibonacciChild(n);
	else {
		wait(NULL);
		printf("Child Complete, Parent Fibonacci running:\n");
		fibonacciParent(n);
	}

	return 0;
}


void fibonacciChild(int n){
	long long fib_2 = 0;
	long long fib_1 = 1;
	long long fib = 1;

	printf("Printing fibonacci sequence of length: %d\n", n);
	// Print fib of n = 0
	printf("%lli\n", fib_2);
	// Print fib of n = 1
	printf("%lli\n", fib_1);
	for (int i = 2; i <= n; i++){
		fib = fib_2 + fib_1;
		fib_2 = fib_1;
		fib_1 = fib;
		printf("%lli\n", fib);
	}
}

void fibonacciParent(int n){
	long long fib_2 = 0;
	long long fib_1 = 1;
	long long fib = 1;

	for (int i = 2; i <= n + 2; i++){
		fib = fib_2 + fib_1;
		fib_2 = fib_1;
		fib_1 = fib;
	}
	printf("%lli\n", fib_2);
	printf("%lli\n", fib_1);

}
