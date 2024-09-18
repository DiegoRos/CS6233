#include <stdio.h> // General C functions
#include <unistd.h> // Linux syscalls
#include <fcntl.h> // File control
#include <sys/stat.h> // Chmod function
#include <sys/types.h> // For non-standard types
#include <string.h> // String functions
#include <stdbool.h> // Booleans

#define BUFF_SIZE  255

bool isTextFile(char* filename, int size);
void copyContentsFileToFIle(int fdInput, int fdOutput);

int main(int argc, char *argv[]){
	if (argc != 3){
		printf("Error, did not input the correct number of inputs (2)\n");
		return -1;
	}
	
	if (!isTextFile( argv[1], strlen(argv[1])) ){
		printf("Input file is not txt.\n");
		return -1;
	}
	
	if (!isTextFile( argv[1], strlen(argv[1])) ){
		printf("Output file is not txt.\n");
		return -1;
	}
	
	// Open file with O_RDONLY flag to specify this file can only be read.
	int fdInput = open(argv[1], O_RDONLY);
	if (fdInput == -1){
		printf("Unable to open input file. Please try again.\n");
	}

	// Open file with the following flags:
	//	- O_WROLNY: Open with read and write privileges.
	//	- O_CREAT: File is to be created with filename argv[2] if it does not exist.
	//	- O_TRUNC: If file already exists allows writing and will be truncated to length 0.
	int fdOutput = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC); 
	if (fdOutput == -1){
		printf("Unable to open output file. Please try again.\n");
	}

	char copyString[BUFF_SIZE];
	ssize_t c;
	while( c = read(fdInput, copyString, BUFF_SIZE), c > 0 ){
		write(fdOutput, copyString, c);
	}

	write(fdOutput, "\n\n", 2);
	
	char userName[] = "Diego Rosenberg\n";
	int userNameLength = strlen(userName);
	write(fdOutput, userName, userNameLength);
	
	// Returns userID as seen by the computer
	uid_t userID = getuid();
	
	// To make it human readable we have to convert it into a string with 32 bit space (due to uid_t of 32 bits).
	char tmp[12] = {0x0};
	sprintf(tmp, "%11d", userID);
	write(fdOutput, tmp, sizeof(tmp));
	write(fdOutput, "\n", 1);

		

	// Before closing file we can change the permissions of the created file to allow viewing and future modification.
	//	(0#####) indicates octal number in C.
	// Permissions set:
	// 	- 6: 110, Permission to read write but not excecute
	//	- 4: 100, Permission to read.
	int mode = 0644; 	
	fchmod(fdOutput, mode);

	close(fdInput);
	close(fdOutput);
	return 0;
}

bool isTextFile(char* filename, int size){
	if (filename == NULL) return false;

	if (strcmp(&filename[size - 4], ".txt") == 0)
		return true;

	return false;	
}
