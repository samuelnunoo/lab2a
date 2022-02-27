#include "../sortedlist.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

struct thread_args {
    SortedList_t *list;
    int iterations;
    int thread_number;
    SortedListElement_t *listItems;
};

extern int sync_value;

void insert_to_list(struct thread_args args) {
    int start_index = args.iterations * args.thread_number;
    int count = 0;

    while (count < args.iterations) {
        SortedListElement_t *element = &(args.listItems[start_index + count]);
        if (sync_value == 0) SortedList_insert(args.list,element); 
        else if (sync_value == 1) SortedList_insert_s(args.list,element); 
        else SortedList_insert_m(args.list,element); 
        count++;
    }
}

void remove_inserted_values_from_list(struct thread_args args) {
    int start_index = args.iterations * args.thread_number;
    int count = 0;
    int result = 0;
    int result2 = 0;
    
    while (count < args.iterations) {
        SortedListElement_t *element = &(args.listItems[start_index + count]);
        if (sync_value == 0) {
            if (SortedList_lookup(args.list,element->key) == 0) result2 = 1;
            result = SortedList_delete(element);
        }
        else if (sync_value == 1) {
            if (SortedList_lookup_s(args.list,element->key) == 0) result2 = 1;
            result = SortedList_delete_s(element);
        }
        else {
            if (SortedList_lookup_m(args.list,element->key) == 0) result2 = 1;
            result = SortedList_delete_m(element);
        }
        count++;

        if (result == 1|| result2 == 1) {
            fprintf(stderr,"Corrupted List\n");
            exit(2);
        }
    }
}

void *thread_routine_list(void *arguments) {
    struct thread_args *args = (struct thread_args*) arguments;
    insert_to_list(*args);

    if (sync_value == 0) SortedList_length(args -> list);
    else if (sync_value == 1) SortedList_length_s(args -> list);
    else SortedList_length_m(args -> list);
    remove_inserted_values_from_list(*args);

    return arguments;
}

void wait_for_threads(pthread_t *threads,int count) {
    int index;
    for (index = 0; index < count; index++) {
        void *retval;
        pthread_join(*(threads + index),&retval);
    }
}

pthread_t *create_threads(int iterations,int threads,SortedListElement_t *listItems, SortedList_t *list) {
    int index;
    pthread_t *thread_array = malloc(sizeof(pthread_t) * threads);
    struct thread_args *args = malloc(sizeof(struct thread_args) * threads);

    for (index = 0; index < threads; index++) {
        pthread_t thread_id;
        
        (args + index)->iterations = iterations;
        (args + index)->thread_number = index;
        (args + index)->listItems = listItems;
        (args + index)->list = list;
        
        pthread_create(&thread_id,NULL,thread_routine_list,(void *) (args + index));
        *(thread_array + index) = thread_id;
    }

    return thread_array;
}
