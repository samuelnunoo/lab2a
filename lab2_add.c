#include <getopt.h>
#include <unistd.h>
#include<stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <string.h>

/*
Sources:

https://stackoverflow.com/questions/232693/including-one-c-source-file-in-another
https://stackoverflow.com/questions/1383363/is-my-spin-lock-implementation-correct-and-optimal
*/

#define BILLION 1000000000
#define NONE 0
#define SPIN_LOCK 1
#define MUT_EX 2 
#define COMPARE 3


static struct option long_options[] =
{
    {"threads",optional_argument, NULL, 't'},
    {"iterations",optional_argument, NULL, 'i'},
    {"yield",no_argument,NULL,'y'},
    {"sync",required_argument,NULL,'s'},
    {0,0,0,0}
};

int opt_yield = 0;
int opt_sync = NONE;
int lock = 0;
volatile int block = 0;
pthread_mutex_t count_mutex;

struct thread_args {
    long long *counter;
    int iterations;
};

void spin_lock() {
 while (__sync_lock_test_and_set(&block, 1)) {
 }
}

void spin_unlock() {
    __sync_lock_release(&block);
}

void add_compare_and_swap(long long *pointer, long long value) {
    while( __sync_val_compare_and_swap(&lock,0,1) == 1);
    if (opt_yield) sched_yield();
    long long sum = *pointer + value;
    *pointer = sum;
    __sync_val_compare_and_swap(&lock,1,0);
}

void add_spin_lock(long long *pointer, long long value) {
    spin_lock();
    long long sum = *pointer + value;
    if (opt_yield) sched_yield();
    *pointer = sum;
    spin_unlock();
}

void add_mutex(long long *pointer, long long value) {
    pthread_mutex_lock(&count_mutex);
    long long sum = *pointer + value;
    if (opt_yield) sched_yield();
    *pointer = sum;
    pthread_mutex_unlock(&count_mutex);
}

void add(long long *pointer, long long value) {
    long long sum = *pointer + value;
    if (opt_yield) sched_yield();
    *pointer = sum;
}

long get_execution_time(struct timespec start, struct timespec end) {
    long start_nanoseconds = (start.tv_sec * BILLION) + start.tv_nsec;
    long end_nanoseconds = (end.tv_sec * BILLION) + end.tv_nsec;
    return end_nanoseconds - start_nanoseconds;
}

void *thread_routine(void * arguments) {
    int index;
    struct thread_args *args = (struct thread_args *) arguments;
    
    for (index = 0; index < args -> iterations; index++) {
        switch(opt_sync) {
            case NONE:
                add(args -> counter,1);
                add(args -> counter,-1);
                break;
            case SPIN_LOCK:
                add_spin_lock(args -> counter,1);
                add_spin_lock(args -> counter,-1);
                break;
            case MUT_EX:
                add_mutex(args -> counter,1);
                add_mutex(args -> counter,-1);
                break;
            case COMPARE:
                add_compare_and_swap(args -> counter,1);
                add_compare_and_swap(args -> counter,-1);
                break;
            default:
                fprintf(stderr,"In orderr opt_sync argument");
                exit(1);
                break;
        }
    }
    return thread_routine;
}

pthread_t *create_threads(int iterations,int threads,long long * counter) {
    int index;
    pthread_t *thread_array = malloc(sizeof(pthread_t) * threads);
    struct thread_args args;
    args.counter = counter;
    args.iterations = iterations;

    for (index = 0; index < threads; index++) {
        pthread_t thread_id;
        pthread_create(&thread_id,NULL,thread_routine,(void *) &args);
        *(thread_array + index) = thread_id;
     
    }

    return thread_array;
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
                break;
            case 's':
                if (strcmp(optarg,"s") == 0) opt_sync = SPIN_LOCK;
                else if (strcmp(optarg,"m") == 0) opt_sync = MUT_EX;
                else if (strcmp(optarg,"c") == 0) opt_sync = COMPARE;
                else {
                    fprintf(stderr,"Invalid argument provided: valid options for sync include s m or c");
                    exit(1); 
                }
                break;

            default:
                fprintf(stderr,"Invalid argument provided: valid options include --yield --iterations --threads and --sync");
                exit(1);
                break;
        }
    }
}
  
void yield_threads(pthread_t * thread_values, int thread_count) {
    int index;
    
    for (index = 0; index < thread_count; index++) {
        void *retval;;
        pthread_join(*(thread_values + index),&retval);
    }
}

void log_results(char * test,int threads, int iterations, long execution_time, long long counter) {
    int operations = threads * iterations * 2;
    long time_per_operation = execution_time / operations;
    printf("%s,%d,%d,%d,%ld,%ld,%lld\n",test,threads,iterations,operations,execution_time,time_per_operation,counter);
}

char * get_test_name() {
    switch(opt_sync) {
        case NONE:
            if (opt_yield) return "add-yield-none";
            return "add-none";
        case MUT_EX:
            if (opt_yield) return "add-yield-m";
            return "add-m";
        case SPIN_LOCK:
            if (opt_yield) return "add-yield-s";
            return "add-s";
        case COMPARE:
            if (opt_yield) return "add-yield-c";
            return "add-c";
        default:
            break;
    }

    return 0;
}

int main(int argc, char *argv[]) {
    long long counter = 0;
    int iterations = 1;
    int threads = 1;
    struct timespec start, end;

    clock_gettime(CLOCK_MONOTONIC,&start);
    set_variable_values(argc,argv,&iterations,&threads);
    pthread_t *thread_values = create_threads(iterations,threads,&counter);
    yield_threads(thread_values,threads);
    clock_gettime(CLOCK_MONOTONIC,&end);

    long execution_time = get_execution_time(start,end);
    char * test_name = get_test_name();
    log_results(test_name,threads,iterations,execution_time,counter);
    return 0;
}

