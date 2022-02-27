---
type: useCase
category: ["#lock"]
---



```ad-example
~~~c
#include <pthread.h>

pthread_mutex_t count_mutex;
long long count;

void
increment_count()
{
	    pthread_mutex_lock(&count_mutex);
    count = count + 1;
	    pthread_mutex_unlock(&count_mutex);
}

long long
get_count()
{
    long long c;
    
    pthread_mutex_lock(&count_mutex);
	    c = count;
    pthread_mutex_unlock(&count_mutex);
	    return (c);
}
~~~
```
