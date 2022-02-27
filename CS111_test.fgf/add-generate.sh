#!/bin/bash

test_0() {
    iterations=(100 1000 10000 100000)
    threads=(2 4 8 10)
    for t in "${threads[@]}"
    do
        for i in "${iterations[@]}"
        do
            ./lab2_add --threads=$t --iterations=$i  >> lab2_add.csv
        done 
    done 
}

test_1() {
    threads2=(2 4 8 12)
    iterations2=(10 20 40 80 100 1000 10000 100000)
    for t in "${threads2[@]}"
    do
        for i in "${iterations2[@]}"
        do
            ./lab2_add --threads=$t --iterations=$i --yield >> lab2_add.csv
        done
    done 
}

test_2() {
    threads=(1 2 3 4 5 6 7 8)
    iterations=(100 1000 10000 100000)
    for t in "${threads[@]}" 
    do 
        for i in "${iterations[@]}"
        do
            ./lab2_add --threads=$t --iterations=$i --yield >> lab2_add.csv 
            ./lab2_add --threads=$t --iterations=$i >> lab2_add.csv 
        done
    done
}

test_3() {
    threads2=(2 4 8 12)
    for t in "${threads2[@]}" 
    do
        ./lab2_add --threads=$t --iterations=10000 --yield --sync=m >> lab2_add.csv
        ./lab2_add --threads=$t --iterations=10000 --yield --sync=c >> lab2_add.csv
        ./lab2_add --threads=$t --iterations=1000 --yield --sync=s >> lab2_add.csv
    done 
}

test_4() {
    threads2=(2 4 8 12)
    for t in "${threads2[@]}" 
    do
        ./lab2_add --threads=$t --iterations=10000  --sync=m >> lab2_add.csv
        ./lab2_add --threads=$t --iterations=10000  --sync=c >> lab2_add.csv
        ./lab2_add --threads=$t --iterations=10000 --sync=s >> lab2_add.csv
    done 
}

test_0
test_1
test_2
test_3
test_4