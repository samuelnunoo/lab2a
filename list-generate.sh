#!/bin/bash


test_1() {
    iterations=(100 1000 10000)
    for i in "${iterations[@]}"
    do 
        ./lab2_list --threads=1 --iterations=$i >> lab2_list.csv 
    done 
}


test_2() {
    threads=(2 4 8 12)
    iterations=(1 10 100 1000)
    for t in "${threads[@]}"
    do 
        for i in "${iterations[@]}" 
        do 
            ./lab2_list --threads=$t --iterations=$i >> lab2_list.csv 
        done
    done
}


test_3() {
    threads=(2 4 8 12)
    iterations=(1 2 4 8 16 32)
    comb=(i d il dl)
    
    for t in "${threads[@]}"
    do
        for i in "${iterations[@]}"
        do 
            for c in "${comb[@]}"
            do 
                ./lab2_list --threads=$t --iterations=$i --yield=$c >> lab2_list.csv 
            done
        done
    done 
}


test_4() {
    iterations=1000
    threads=(1 2 4 8 12 16 24)

    for t in "${threads[@]}"
    do 
         ./lab2_list --threads=$t --iterations=$iterations >> lab2_list.csv
    done 

}

test_5() {
    threads=(1 2 4 8 12)
    iterations=(1 2 4 8 16 32)
    yield=(i l il d id dl ild)
    opts=(s m)

    for t in "${threads[@]}"
    do
        for i in "${iterations[@]}"
        do
            for o in "${opts[@]}"
            do
                ./lab2_list --threads=$t --iterations=$i --sync=$o >> lab2_list.csv
                for y in "${yield[@]}"
                do
                    ./lab2_list --threads=$t --iterations=$i --sync=$o --yield=$y >> lab2_list.csv
                done 
            done 
        done 
    done 
    
    for t in "${threads[@]}"
    do 
        ./lab2_list --threads=$t --iterations=1000 --sync=s>> lab2_list.csv

        ./lab2_list --threads=$t --iterations=1000 --sync=m>> lab2_list.csv
    done 
}


test_1
test_2
test_3
test_4
test_5