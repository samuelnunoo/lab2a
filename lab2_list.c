#include <getopt.h>
#include <unistd.h>
#include<stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <string.h>
#include <stddef.h>
#include "sortedlist.h"
#include "modules/randomkey.h"
#include "modules/threadroutines.h"
#include <signal.h>

static struct option long_options[] =
{
    {"threads",required_argument, NULL, 't'},
    {"iterations",required_argument, NULL, 'i'},
    {"yield",required_argument,NULL,'y'},
    {"sync",required_argument,NULL,'s'},
    {0, 0, 0, 0}
};

#define SPIN_LOCK  1;
#define MUT_EXCLUSION 2;
#define BILLION 1000000000

int opt_yield = 0;
int yield_insert = 0;
int yield_delete = 0;
int yield_lookup = 0;
int sync_value = 0;

void set_yield_opts(char *options) {
    int index = 0;
    char value = *(options + index);

    while (value != 0) {
        switch(value) {
            case 'i':
                yield_insert = 1;
                break;
            case 'd':
                yield_delete = 1;
                break;
            case 'l':
                yield_lookup = 1;
                break;
            default:
                fprintf(stderr,"Invalid option provided: valid options for --yield include i l or d");
                exit(1);  
        }
        index++;
        value = *(options + index);
    }
  
}

void set_variable_values(int argc, char *argv[],int *iterations, int *threads) {
    int opt;
    while ((opt = getopt_long(argc,argv,"",long_options,NULL)) != -1) {
        switch(opt) {
            case 'i':
                *iterations = atoi(optarg);
                break;
            case 't':
                *threads = atoi(optarg);
                break;
            case 'y':
                opt_yield = 1;
                set_yield_opts(optarg);
                break;
            case 's':
                if (strcmp(optarg,"m") == 0) {
                    sync_value = MUT_EXCLUSION;
                }
                else if (strcmp(optarg,"s") == 0) {
                    sync_value = SPIN_LOCK;
                }
                else {
                    exit(1); //@todo;
                }
                break;
            default:
                fprintf(stderr,"Invalid argument provided: valid options include --yield --iterations --sync and --threads");
                exit(1);
                break;
        }
    }
}

struct SortedListElement create_list_item() {
    char *key = (char*) malloc((20)*sizeof(char));
    random_key(key,20);

    struct SortedListElement elem;
    elem.key = key;
    return elem;

}

void init_list_items(struct SortedListElement *listItems, int size) {
    int index;

    for (index = 0; index < size; index++) {
        *(listItems + index) = create_list_item();
    }
}

void handle_segfault() {
    fprintf(stderr,"Segfault has occurred\n");
    exit(2); 
}

long get_execution_time(struct timespec start, struct timespec end) {
    long start_nanoseconds = (start.tv_sec * BILLION) + start.tv_nsec;
    long end_nanoseconds = (end.tv_sec * BILLION) + end.tv_nsec;
    return end_nanoseconds - start_nanoseconds;
}

char * get_name() {
    char *name = malloc(sizeof(char) * 20);
    strcpy(name,"list-");
    char yield[6] = "";
    char sync_v[6] = "";

    if (yield_insert) strcat(yield,"i");
    if (yield_delete) strcat(yield,"d");
    if (yield_lookup) strcat(yield,"l");

    if (!yield_insert && !yield_delete & !yield_lookup) {
        strcpy(yield,"none");
    }
    strcat(yield,"-");

    if (sync_value == 0) strcpy(sync_v,"none");
    else if (sync_value == 1) strcpy(sync_v,"s");
    else strcpy(sync_v,"m");

    strcat(name,yield);
    strcat(name,sync_v);
    return name;

}

void log_results(int threads, int iterations,long runtime) {
    int total_ops = threads * iterations * 3;
    long average_runtime = runtime / total_ops;
    printf("%s,%d,%d,%d,%d,%ld,%ld\n",get_name(),threads,iterations,1,total_ops,runtime,average_runtime);
}

int main(int argc, char *argv[]) {
    int iterations = 1;
    int threads = 1;
    struct timespec start, end;
    signal(SIGSEGV,handle_segfault);
    set_variable_values(argc,argv,&iterations,&threads);

    SortedList_t list;
    list.key = 0;
    list.next = &list;
    list.prev = &list;

    int size = threads * iterations;
    struct SortedListElement *listItems = malloc(sizeof(struct SortedListElement) * size);
    init_list_items(listItems,size);
    
    clock_gettime(CLOCK_MONOTONIC,&start);
    pthread_t *thread_array = create_threads(iterations,threads,listItems,&list);
    wait_for_threads(thread_array,threads);
    clock_gettime(CLOCK_MONOTONIC,&end);
    long execution_time = get_execution_time(start,end);
    log_results(threads,iterations,execution_time);
    free(listItems);
}