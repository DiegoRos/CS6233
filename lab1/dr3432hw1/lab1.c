#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
	// Innitialize seed
	srand(time(NULL));

	// Random number between 0 and 199
	int randNum = rand() % 200;

	// Printing desired result
	printf("Hello World! This is CS3224, Fall 2024\nDiego Rosenberg %d\n", randNum);

	return 0;
}
