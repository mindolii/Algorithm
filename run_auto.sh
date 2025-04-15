#!/bin/bash

ulimit -s 65532

algorithms=("tournament_sort" "tim_sort" "selection_sort" "quick_sort" "merge_sort" "intro_sort" "insertion_sort" "heap_sort" "comb_sort" "cocktailshaker_sort" "bubble_sort" "library_sort")
input_types=("stability")
repeat=1

for algo in "${algorithms[@]}"
do
    for input_type in "${input_types[@]}"
    do
        echo ""
        echo "|=====Algorithm=====|: $algo"
        echo "|=====Input Type====|: $input_type"
        echo "|======Repeat=======|: $repeat"
        echo ""

        for ((i=1; i<=repeat; i++))
        do
            echo "======================"
            echo "Starting at $(date +%T)"
            echo "  ...execute $i ...   "
            ./build/$algo $input_type
            echo "======================"
        done
    done
done
