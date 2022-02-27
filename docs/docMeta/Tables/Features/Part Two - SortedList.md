---
type: feature
status:
---



```ad-note
title:Description
```



```ad-abstract
title:Acceptance Critiera
```


## Research and Design


```ad-note
title: Traverse 
(thread_num,iterations,listItems,list)
~~~go
start_index = thread_num * iterataions
count = 0;

while (count < iterations):
	currentItem = listItems[start_index + count]
	count++;
~~~
```


```ad-note
title: Insert to List

to insert into the list we just need to get an element and insert it

~~~typescript
	list.insert(currentItem);
```

```ad-note
title: Get List Length 

This is just getting the length which can be done just by calling a specific method

~~~typescript
list.length()
~~~~
```

```ad-note
title: Delete List Item

for this we need to look up and delete the items we are inserting. This means that we need to iterate over all the items as done so in the #Traverse section. Then we need to call the delete method with that element.

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





