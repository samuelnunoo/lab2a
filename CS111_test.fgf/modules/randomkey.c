#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stddef.h>
#include <getopt.h>
#include <unistd.h>

/*
Source: 
    https://stackoverflow.com/questions/15767691/whats-the-c-library-function-to-generate-random-string
    https://stackoverflow.com/questions/1108780/why-do-i-always-get-the-same-sequence-of-random-numbers-with-rand
*/

char charset[] = "0123456789"
                 "abcdefghijklmnopqrstuvwxyz"
                 "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

long long  get_seed() {
    struct timespec start;
    clock_gettime(CLOCK_MONOTONIC,&start);
    return start.tv_nsec;
}

void random_key(char *dest, size_t length) {
    srand (get_seed());
    size_t current = 0;

     for (current = 0; current < length;current++) {
        size_t index = (double) rand() / RAND_MAX * (sizeof charset - 1);
        *dest++ = charset[index];
     }


}