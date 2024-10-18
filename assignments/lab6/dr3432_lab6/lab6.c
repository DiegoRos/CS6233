#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

#define SERVER_PORT 8000
#define MAX_CONNECT_ATTEMPTS 1000

void fibonacciGenerate(int n);
void fibonacciPrint(int n);

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

	// Set random seed
	srand(time(NULL));

	pid_t x;
	x = fork();

	// Check if fork is unsuccesfull.
	if (x < 0){
		perror("Fork was unsucessfull, please try again.\n");
		exit(1);
	}
	//Check to see that we are inside of child process. If so, run fibonacci.
	if (x == 0) fibonacciGenerate(n);
	else fibonacciPrint(n);


	return 0;
}


void fibonacciGenerate(int n){
	// Socket Creation
	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd == -1){
		perror("Socket creation failed.\n");
		exit(1);
	}

	// Client socket fd
	int client_socket_fd;

	// Bind socket to server
	struct sockaddr_in server_addr, client_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVER_PORT);

	socklen_t addr_size = sizeof(struct sockaddr_in);
	if (bind(socket_fd, (struct sockaddr *) &server_addr, addr_size) == -1){
		perror("Socket binding failed.\n");
		exit(1);
	}

	// Wait for 1 to 3 seconds as per assignment instructions.
		// Select random num between 0 and 2,000, then multiply by 1,000
		// To the result (a random value between 0 and 2,000,000) add 1,000,000.
		// This will essentially result in a random number between 1,000,000 and 3,000,000.
	usleep( ((rand() % 2000) * 1000) + 1000000 );

	// Listen to incoming connection
	if (listen(socket_fd, 10) == -1){
		perror("Listen returned error.\n");
		exit(1);
	}
	else{
		// If listen is successful then accept the connection.
		client_socket_fd = accept(socket_fd, (struct sockaddr *) &client_addr, &addr_size);
	}

	// Begin generating fibonacci sequence numbers.
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

		// Insert fib of n into socket
		write(client_socket_fd, &fib, sizeof(fib));
		
	}
	close(client_socket_fd);
	close(socket_fd);
	return;
}

void fibonacciPrint(int n){
	// Socket Creation
	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd == -1){
		perror("Socket creation failed.\n");
		exit(1);
	}

	// Set up server structure
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVER_PORT);
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	socklen_t server_addr_size = sizeof(struct sockaddr_in);

	// Connect to server
	int attempts = 0;
	while (	(attempts < MAX_CONNECT_ATTEMPTS) && 
					connect(socket_fd, (struct sockaddr *) &server_addr, server_addr_size) == -1){
		attempts++;
		//Wait 100 ms
		usleep(100*1000);
	}
	// If loop exits due to attempts raise error
	if (attempts == MAX_CONNECT_ATTEMPTS){
		perror("Socket connect failed, too many attempts.\n");
		exit(1);
	}

	// Begin fibonacci sequence reading
	int numbers_printed = 0;
	long long fib;
	
	printf("Parent printing fibonnacci sequence of %d:\n", n);

	while (numbers_printed <= n){
		// Read from socket
		read(socket_fd, &fib, sizeof(fib));

		printf("%llu\n", fib);
		fflush(stdout);
		numbers_printed++;
	}

	// Close socket
	close(socket_fd);
	return;
}
