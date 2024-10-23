#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

// Use static key word for global shared counter
// This counter will be what we need to protect using mutex or semaphores
