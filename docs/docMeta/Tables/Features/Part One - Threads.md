---
type: feature
status:
---



```ad-note
title:Description
-   Write a multithreaded application (using pthreads) that performs parallel updates to a shared variable.
-   Demonstrate the race condition in the provided add routine, and address it with different synchronization mechanisms.
-   Do performance instrumentation and measurement.
-   Analyze and explain the observed performance.
```

```ad-abstract
title:Acceptance Critiera
```


## Research and Design

```ad-note
title: Waiting for All Threads

One of the things we must do is wait for a dynamic amount of threads.
Let's say we have n thread handlers, how can run all of those threads but also wait for all of them to yield before continuing?


I think we can create the threads and add their address to an array.
Once that is done, you can  then call a yield function which yields until all of them are done. 

Then you can call a function which gets the time.

```



## Components 
```dataview
list
from "docMeta/Tables/Components" and [[]]
```



```button
name Create Component
type command
action QuickAdd: Create Component
```





