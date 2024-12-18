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

void fibonacciGenerate(int n, shared_data* shm_ptr);

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

	// Shared Memory Creation
	
	int shm_fd;
	shared_data *shm_ptr;
	
	//Create Shared Memory File
	shm_fd = shm_open(SHM_FILE, O_CREAT | O_RDWR, 0666);
	if (shm_fd == -1){
		perror("Unable to allocate shared memory.\n");
		exit(1);
	}
	
	// Truncate size
	if (ftruncate(shm_fd, sizeof(shared_data)) == -1){
		perror("Unable to truncate shared memory.\n");
		exit(1);
	}
	
	// Map shared memory object into adress space
	shm_ptr = (shared_data *) mmap(NULL, sizeof(shared_data), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	if (shm_ptr == MAP_FAILED){
		perror("Unable to map shared memory.\n");
		exit(1);
	}
	// Innitialize shared data indexes
	shm_ptr->in = 0;
	shm_ptr->out = 0;

	//Shared Buffer Location
	printf("Location of shared buffer producer: %p\n", shm_ptr);
	// Location of n variable
	printf("Location of n producer: %p\n", &n);

	// Set seed
	srand(time(NULL));

	fibonacciGenerate(n, shm_ptr);
	
	// Consumer is in charge of unmapping and deleting since it implies all are done.

	return 0;
}


void fibonacciGenerate(int n, shared_data *shm_ptr){
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

		// Check buffer
		while ( ((shm_ptr->in + 1) % BUF_SZ) == shm_ptr->out ){
			usleep(1000); // Sleep for 1 millisecond
		}

		// Insert fib of n
		shm_ptr->buffer[shm_ptr->in] = fib;
		shm_ptr->in = (shm_ptr->in + 1) % BUF_SZ;
	}

	return;
}
