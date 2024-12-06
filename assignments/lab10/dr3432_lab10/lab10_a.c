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
	Struct for frames table.
		- Contains index of frames in memory: framesList
		- Contains size of frames list: numFrames
		- Contains current index of last checked frame for removal: currIndex
			- All frames innitialized to null, if the last index is at null pointer, insert.
*/
typedef struct pageTableStruct{
	Frame *frameArr;
	int numFramesAllocated;
	int numFrames;
	int currIndex;
} PageTable;

/*
Method for quick initialization of framesList
*/
void initPageTable(int numFrames, int p, PageTable *pageTable){
	pageTable->frameArr = (Frame *) malloc(p * sizeof(Frame));
	if (pageTable->frameArr == NULL){
		perror("Unable to allocate to frame list.\n");
		exit(1);
	}
	
	// Initialize all frames to index i and both bits as 0
	for (int i = 0; i < p; i++) {
		pageTable->frameArr[i].frameNumber = i;
		pageTable->frameArr[i].validBit = false;
		pageTable->frameArr[i].referenceBit = false;
	}

	// Initialize frameList->numFrames to numFrames
	pageTable->numFrames = numFrames;

	// Initialize number of allocated frames in memory to 0
	pageTable->numFramesAllocated = 0;

	// Initialize currIndex to 0
	pageTable->currIndex = 0;

	return;
}

/* 
Method for quickly removing frameList
*/
void delPageTable(PageTable *pageTable){
	if (pageTable != NULL && pageTable->frameArr != NULL){
		free(pageTable->frameArr);
	}
	return;
}



// Function prototypes
/* 
	Function prototype for pageTraceGenerator
		- Takes an integer n (for size of page trace), (available frames) as an input,
				and the return array for pageTrace.
		- Returns void.
*/
void pageTraceGenerator(int n, int p, int *pageTrace);

/*
	Function prototype for simulate
		- Takes in integer numFrames (number of available frames), an integer n (size of page trace), 
				an integer p for largest frame number, and a pageTrace array
		- Returns an int with the number of page faults found
*/
int simulate(int n, int p, int numFrames, int pageTrace[]);

/*
	Function prototype for findVictim
		- Takes page table and frames list as parameters
		- Returns index of page to replace
*/
int findVictim(PageTable *pageTable, int p);

/*
	Function prototype for replaceVictim
		- Takes page table, index of frame to add (victim) and index of frame to replace (candidate)
		- Returns void
*/
void replaceVictim(PageTable *pageTable, int victim, int candidate);


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
	int pageTrace[n];
	pageTraceGenerator(n, p, pageTrace);

	// Array to store page faults
	int pageFaultsArr[p - 4 + 1];


	// Invoke simulation().
	for(int framesAvailable = 4; framesAvailable <= p; framesAvailable++){
		// Run Simulation for framesAvailable.
		pageFaultsArr[framesAvailable - 4] = simulate(n, p, framesAvailable, pageTrace);
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

/* 
	Function for pageTraceGenerator
		- Takes an integer n (for size of page trace), and p (available frames) as an input.
		- Returns the randomly generated list of pages (of size n).
*/
void pageTraceGenerator(int n, int p, int pageTrace[]){
	for (int i = 0; i < n; i++) pageTrace[i] = rand() % p;
	return;
}

/*
	Function for simulate
		- Takes in integer numFrames (number of available frames), an integer n (size of page trace, 
				and a pageTrace array
		- Returns an int with the number of page faults found
*/
int simulate(int n, int p, int numFrames, int pageTrace[]){
	PageTable pageTable;
	initPageTable(numFrames, p, &pageTable);
	int result = 0;

	for (int i = 0; i < n; i++){
		Frame *currFrame = &pageTable.frameArr[pageTrace[i]];
		if (currFrame->validBit == false){
			int victimIndex = findVictim(&pageTable, p);
			replaceVictim(&pageTable, victimIndex, pageTrace[i]);
			result++;
		}
		currFrame->referenceBit = true;
	}

	delPageTable(&pageTable);
	return result;
}

/*
	Function for findVictim
		- Takes page table and frames list as parameters
		- Returns index of page to replace, or -1 if frames available
*/
int findVictim(PageTable *pageTable, int p){
	int numFramesAllocated = pageTable->numFramesAllocated;
	int numFrames = pageTable->numFrames;
	int currIndex = pageTable->currIndex;
	// if numallocated < numFrames then return -1. Else while loop to find victim.
	if (numFramesAllocated < numFrames) return -1;
	
		bool inMemory = pageTable->frameArr[currIndex].validBit;
		bool referenceBitTrue = pageTable->frameArr[currIndex].referenceBit;

	// We only want to break loop if we find something in memory and that has a 0 reference bit. So we continue as long as it is not in memory or reference bit is 1.
	while (!inMemory || referenceBitTrue){
		if (inMemory){
			pageTable->frameArr[currIndex].referenceBit = false;
		}
		// Move index forward (copy on stack not in pageTable)
		currIndex = (currIndex + 1) % p;
		
		// Set conditionals again
		inMemory = pageTable->frameArr[currIndex].validBit;
		referenceBitTrue = pageTable->frameArr[currIndex].referenceBit;
	}

	//Set page table current index to next one.
	pageTable->currIndex = (currIndex + 1) % numFrames;
	
	//Return local copy of current index with victim.
	return currIndex;
}


/*
	Function for replaceVictim
		- Takes page table, index of frame to add (victim) and index of frame to replace (candidate)
		- Returns void
*/
void replaceVictim(PageTable *pageTable, int victim, int candidate){

	// Set candidate to being in memory
	pageTable->frameArr[candidate].validBit = 1;
	pageTable->frameArr[candidate].referenceBit = 1;
	pageTable->numFramesAllocated++;
	
	// If victim exists (i.e. not -1) set victim to out of memory
	if (victim != -1){
		pageTable->frameArr[victim].validBit = 0;
	}

	return;
}


