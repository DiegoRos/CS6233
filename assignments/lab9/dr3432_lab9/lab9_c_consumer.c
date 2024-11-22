#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <time.h>

#define SHM_FILE "/lab9_shm"

#define BUF_SZ 4
typedef struct {
	long long buffer[BUF_SZ];
	int in;
	int out;
} shared_data;

static int n;

void fibonacciPrint(int n, shared_data* shm_ptr);

int main (int argc, char* argv[]){
	if (argc != 2){
		perror("Not enough input variables\n");
		exit(1);
	}
	n = atoi(argv[1]);
	if (n < 1){
		perror("Incorrect inputed value for n. n has to be greater than 1 (n>1).\n");
		exit(1);
	}

	// Shared Memory Open
	
	int shm_fd;
	shared_data *shm_ptr;
	
	//Open Shared Memory File
	shm_fd = shm_open(SHM_FILE, O_RDWR, 0666);
	if (shm_fd == -1){
		perror("Unable to find shared memory.\n");
		exit(1);
	}
	
	// Map shared memory object into adress space
	shm_ptr = (shared_data *) mmap(NULL, sizeof(shared_data), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	if (shm_ptr == MAP_FAILED){
		perror("Unable to map shared memory.\n");
		exit(1);
	}
	
	//Shared Buffer Location
	printf("Location of shared buffer consumer: %p\n", shm_ptr);
	// Location of n variable
	printf("Location of n consumer: %p\n", &n);

	// Run consumer side of fibonacci
	fibonacciPrint(n, shm_ptr);

	// After we are done consuming, close shared memory
	munmap(shm_ptr, sizeof(shared_data));
	shm_unlink(SHM_FILE);

	return 0;
}


void fibonacciPrint(int n, shared_data *shm_ptr){
	int numbers_printed = 0;
	long long fib;
	
	printf("Printing fibonnacci sequence of %d:\n", n);

	while (numbers_printed <= n){
		// Wait until buffer is utilized.
		while (shm_ptr->in == shm_ptr->out){
			usleep(1000); // Sleep for 1 millisecond
		}

		fib = shm_ptr->buffer[shm_ptr->out];
		shm_ptr->out = (shm_ptr->out + 1) % BUF_SZ;

		printf("%llu\n", fib);
		fflush(stdout);
		numbers_printed++;
	}

	return;
}
