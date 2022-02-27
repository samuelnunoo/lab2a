---
type: useCase
category: 
---



  
```ad-example

~~~c
  

void wait_for_threads(pthread_t *threads,int count) {

 int index;

 for (index = 0; index < count; index++) {

 void *retval;

 pthread_join(*(threads + index),&retval);

 }

}
~~~
```
