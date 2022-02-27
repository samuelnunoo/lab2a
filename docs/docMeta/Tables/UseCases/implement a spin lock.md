---
type: useCase
category: 
---



```ad-example
~~~c

volatile int exclusion = 0;

void lock() {
    while (__sync_lock_test_and_set(&exclusion, 1)) {
        // Do nothing. This GCC builtin instruction
        // ensures memory barrier.
    }
}

void unlock() {
    __sync_synchronize(); // Memory barrier.
    exclusion = 0;
}
~~~


```
