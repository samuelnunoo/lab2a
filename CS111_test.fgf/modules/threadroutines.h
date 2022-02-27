#include "../sortedlist.h"
#include <pthread.h>

void *thread_routine(void *arguments);

pthread_t *create_threads(int iterations,int threads,SortedListElement_t *listItems, SortedList_t *list);

void wait_for_threads(pthread_t *threads,int count);