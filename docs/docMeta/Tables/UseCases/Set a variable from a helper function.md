---
type: useCase
category: ["#c"]
---



```ad-note
1. Get address of variable as input
2. dereference the variable and set it
```


```ad-example

~~~c

void main() {
 int iterations = 1;
 int threads = 1;
 set_variable_values(argc,argv,&iterations,&threads);

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

 }

 }

}
~~~
```
