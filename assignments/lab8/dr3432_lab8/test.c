#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define DEVICE_PATH "/dev/lab8"

int main(int argc, char *argv[]) {
	if (argc != 2){
		fprintf(stderr, "Usage: %s <number of bytes to read> \n", argv[0]);
		return 1;
	}

	int fd;
	ssize_t bytesRead;
	size_t n; 
	char *buffer;
	
	n = atoi(argv[1]);
	if (n < 1){
		perror("Incorrect inputed number.\n");
		return 1;
	}

	// Allocate buffer to store the data
	buffer = (char *)malloc(n);
	if (!buffer) {
    perror("Failed to allocate memory");
		return 1;
	}


	// Open the misc driver device
	fd = open(DEVICE_PATH, O_RDONLY);
	if (fd < 0) {
		perror("Failed to open device");
		free(buffer);
		return 1;
	}
	
	// Read n bytes from the device
	while (bytesRead = read(fd, buffer, n) > 0){
		printf("%s\t", buffer);
	}
	printf("\n");
	
	return 0;
}


