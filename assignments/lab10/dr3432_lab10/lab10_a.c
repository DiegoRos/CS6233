#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

// Constant Definition
#define OUT_FILE_NAME "lab10results.csv"

// Struct Definitions
/*
	Frame structure to contain all usefull info of a given frame
		-- A list of frames can be a page table.
*/
typedef struct frameStruct {
	int frameNumber;
	bool validBit;
	bool referenceBit;
} Frame;

/*
	Struct for frames list.
		- Contains index of frames in memory: framesList
		- Contains size of frames list: p
		- Contains current index of last checked frame for removal: last
			- All frames innitialized to null, if the last index is at null pointer, insert.
*/
typedef struct framesListStruct{
	int *frameIndexList;
	int p;
	int last;
} FrameList;

/*
Method for quick initialization of framesList
*/
void initFrames(int p, FrameList *frameList){
	frameList->frameIndexList = (int *) malloc(p * sizeof(int));
	if (frameList->frameIndexList == NULL){
		perror("Unable to allocate to frame list.\n");
		exit(1);
	}
	
	// Initialize all frameList->flist indexes to -1
	for (int i = 0; i < p; i++) frameList->frameIndexList[i] = -1;

	// Initialize frameList->p to p
	frameList->p = p;

	// Initialize last to 0
	frameList->last = 0;

	return;
}

/* 
Method for quickly removing frameList
*/
void delFrames(FrameList *frameList){
	if (frameList != NULL){
		free(frameList->frameIndexList);
	}
	return;
}



// Function prototypes
/* 
	Function prototype for pageTraceGenerator
		- Takes an integer n (for size of page trace), and p (available frames) as an input.
		- Returns the randomly generated list of pages (of size n).
*/
int* pageTraceGenerator(int n, int p);

/*
	Function prototype for simulate
		- Takes in integer numFrames (number of available frames), an integer n (size of page trace, 
				and a pageTrace array
		- Returns an int with the number of page faults found
*/
int simulate(int numFrames, int n, int pageTrace[]);

/*
	Function prototype for findVictim
		- Takes page table and frames list as parameters
		- Returns index of page to replace
*/
int findVictim(Frame* pageTable, FrameList *frameList);


/*
	Main routine
*/
int main(int argc, char *argv[]){
	//Check for valid number of entries.
	if (argc != 3){
		printf("\nIncorrect number of arguments. Usage: ./lab10_a <n> <p>\n Where n >= 16 and p >= 8.\n\n");
		exit(1);
	}

	int n = atoi(argv[1]);
	int p = atoi(argv[2]);

	// Check for valid n and p
	if (n < 16 || p < 8){
		printf("\nIncorrect argument values. Usage: ./lab10_a <n> <p>\n Where n >= 16 and p >= 8.\n\n");
		exit(1);
	}
	
	// Set random seed
	srand((unsigned)time(NULL));

	// Invoke pageTraceGenerator()
	int* pageTrace = pageTraceGenerator(n, p);

	// Array to store page faults
	int pageFaultsArr[p - 4 + 1];


	// Invoke simulation().
	for(int framesAvailable = 4; framesAvailable <= p; framesAvailable++){
		// Run Simulation for framesAvailable.
		pageFaultsArr[framesAvailable - 4] = simulate(framesAvailable, n, pageTrace);
	}

	// Write results to file
	FILE *fp = fopen(OUT_FILE_NAME, "w");
	fprintf(fp, "framesAvailable,pageFault\n");
	for (int framesAvailable = 4; framesAvailable <= p; framesAvailable++){
		fprintf(fp, "%d,%d\n", framesAvailable, pageFaultsArr[framesAvailable - 4]);
	}
	fclose(fp);

	return 0;
}
