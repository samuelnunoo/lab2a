#include "sortedlist.h"
#include <string.h>
#include <unistd.h>
#include<stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>

extern int yield_delete;
extern int yield_insert;
extern int yield_lookup;
extern int opt_yield;
pthread_mutex_t mutex;
volatile int block = 0;

void spin_lock() {
 while (__sync_lock_test_and_set(&block, 1)) {
 }
}

void spin_unlock() {
    __sync_lock_release(&block);
}

void SortedList_insert(SortedList_t *list, SortedListElement_t *element) {
	SortedListElement_t *current = (list -> next);

	while ((current -> key) != 0 && strcmp(current -> key,element -> key) > 0) {
		if (opt_yield & yield_insert) sched_yield();
		current = current -> next; 
	};

	SortedListElement_t *prev = current->prev;
	prev -> next = element;
	element -> prev = prev; 
	element -> next = current;
	current -> prev = element;
}

int SortedList_delete(SortedListElement_t *element) {
	SortedListElement_t *prev =(element -> prev);
	SortedListElement_t *next = (element->next);

	if (prev -> next == 0 || next -> prev == 0) return 1;
	if (strcmp(prev -> next->key,element->key) != 0) return 1;
	if (strcmp(next->prev->key,element->key) != 0) return 1;

	prev -> next = next;
	if (opt_yield & yield_delete) sched_yield();
	next -> prev = prev;
	return 0;
}

SortedListElement_t *SortedList_lookup(SortedList_t *list, const char *key) {
	SortedListElement_t *current = (list->next);

	while (current -> key != 0) {
		if (strcmp(current -> key,key) == 0) return current;
		if (opt_yield & yield_lookup) sched_yield();
		current = current -> next;
	}

	return 0;
}

int SortedList_length(SortedList_t *list) {
	if (list == 0) return -1;
	SortedListElement_t *current = (list -> next);
	int length = 0;

	while ((current -> key)!= 0) {
		length++;
		current = (current -> next);
	}

	return length;
}


// Spin Lock Methods 
void SortedList_insert_s(SortedList_t *list, SortedListElement_t *element) {
	spin_lock();
	SortedListElement_t *current = (list -> next);

	while ((current -> key) != 0 && strcmp(current -> key,element -> key) > 0) {
		if (opt_yield & yield_insert) sched_yield();
		current = current -> next; 
	};

	SortedListElement_t *prev = current->prev;
	prev -> next = element;
	element -> prev = prev; 
	element -> next = current;
	current -> prev = element;
	spin_unlock();
}

int SortedList_delete_s(SortedListElement_t *element) {
	spin_lock();
	SortedListElement_t *prev =(element -> prev);
	SortedListElement_t *next = (element->next);


	if (prev -> next == 0 || next -> prev == 0) {
		spin_unlock();
		return 1;
	}
	if (strcmp(prev -> next->key,element->key) != 0) {
		spin_unlock();
		return 1;
	}
	if (strcmp(next->prev->key,element->key) != 0) {
		spin_unlock();
		return 1;
	}

	prev -> next = next;
	if (opt_yield & yield_delete) sched_yield();
	next -> prev = prev;
	spin_unlock();
	return 0;
}

SortedListElement_t *SortedList_lookup_s(SortedList_t *list, const char *key) {
	spin_lock();
	SortedListElement_t *current = (list->next);

	while (current -> key != 0) {
		if (strcmp(current -> key,key) == 0) {
			spin_unlock();
			return current;
		}
		if (opt_yield & yield_lookup) sched_yield();
		current = current -> next;
	}

	spin_unlock();
	return 0;
}

int SortedList_length_s(SortedList_t *list) {
	spin_lock();
	if (list == 0) {
		spin_unlock();
		return -1;
	} 
	SortedListElement_t *current = (list -> next);
	int length = 0;

	while ((current -> key)!= 0) {
		length++;
		current = (current -> next);
	}

	spin_unlock();
	return length;
}


// Mutual Exclusion Methods
int SortedList_length_m(SortedList_t *list) {
	pthread_mutex_lock(&mutex);
	if (list == 0) return -1;
	SortedListElement_t *current = (list -> next);
	int length = 0;

	while ((current -> key)!= 0) {
		length++;
		current = (current -> next);
	}
	pthread_mutex_unlock(&mutex);
	return length;
}

int SortedList_delete_m(SortedListElement_t *element) {
	pthread_mutex_lock(&mutex);
	SortedListElement_t *prev =(element -> prev);
	SortedListElement_t *next = (element->next);

	if (prev -> next == 0 || next -> prev == 0) {
		pthread_mutex_unlock(&mutex);
		return 1;
	}
	if (strcmp(prev -> next->key,element->key) != 0){
		pthread_mutex_unlock(&mutex);
		return 1;
	} 
	if (strcmp(next->prev->key,element->key) != 0) {
		pthread_mutex_unlock(&mutex);
		return 1;
	}

	prev -> next = next;
	if (opt_yield & yield_delete) sched_yield();
	next -> prev = prev;
	pthread_mutex_unlock(&mutex);
	return 0;
}

SortedListElement_t *SortedList_lookup_m(SortedList_t *list, const char *key) {
	pthread_mutex_lock(&mutex);
	SortedListElement_t *current = (list->next);

	while (current -> key != 0) {
		if (strcmp(current -> key,key) == 0) {
			pthread_mutex_unlock(&mutex);
			return current;
		}
		if (opt_yield & yield_lookup) sched_yield();
		current = current -> next;
	}
	pthread_mutex_unlock(&mutex);

	return 0;
}

void SortedList_insert_m(SortedList_t *list, SortedListElement_t *element) {
	pthread_mutex_lock(&mutex);
	SortedListElement_t *current = (list -> next);

	while ((current -> key) != 0 && strcmp(current -> key,element -> key) > 0) {
		if (opt_yield & yield_insert) sched_yield();
		current = current -> next; 
	};

	SortedListElement_t *prev = current->prev;
	prev -> next = element;
	element -> prev = prev; 
	element -> next = current;
	current -> prev = element;
	pthread_mutex_unlock(&mutex);
}