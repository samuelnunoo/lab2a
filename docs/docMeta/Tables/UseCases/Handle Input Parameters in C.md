---
type: useCase
category: 
---
```ad-note
1. include <getopt.h>
2. write long_options[]
3. perform while loop with getopt_long()
```





```ad-example

~~~c
#include <getopt.h>

static struct option long_options[] =

{

 {"input",required_argument, NULL, 'i'},

 {"output", required_argument, NULL, 'o'},

 {"segfault",no_argument, NULL,'s'},

 {"catch", no_argument,NULL,'c'},

};

// in-main routine

 while ((opt = getopt_long(argc,argv,"",long_options,NULL)) != -1) {

 switch(opt) {

 case 'i':

 input = optarg;

 break;

  

 case 'o':

 output = optarg;

 break;

  

 case 's':

 segfault = 1;

 break;

  

 case 'c':

 catch = 1;

 break;

  

 default:

 printf("%s: Valid options are --input=VALUE --output=VALUE --segfault --catch\n",strerror(1));

 exit(1);

 break;

 }

 }



~~~

```
