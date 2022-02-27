CC=gcc

build: add list 
	

add:
	$(CC) -pthread -g -Wall -Wextra lab2_add.c  -o lab2_add

tests:
	 ./list-generate.sh && ./add-generate.sh
list:
	$(CC) -pthread -g -Wall -Wextra lab2_list.c modules/randomkey.c modules/threadroutines.c sortedlist.c -o lab2_list

graphs: build
	gnuplot lab2_add.gp && gnuplot lab2_list.gp
clean:
	rm -f lab2_list lab2_add lab2_add.csv lab2_list.csv lab2_add-1.png lab2_add-2.png lab2_add-3.png lab2_add-4.png lab2_add-5.png lab2_list-1.png lab2_list-2.png lab2_list-3.png lab2_list-4.png 

dist: clean build tests graphs 
	tar -czvf lab2a-010356794.tar.gz modules lab2_add.c lab2_list.c  lab2_add.csv lab2_list.csv README Makefile lab2_add-1.png lab2_add-2.png lab2_add-3.png lab2_add-4.png lab2_add-5.png lab2_list-1.png lab2_list-2.png lab2_list-3.png lab2_list-4.png sortedlist.c sortedlist.h add-generate.sh list-generate.sh lab2_list.gp lab2_add.gp 

