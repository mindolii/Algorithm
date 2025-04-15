#!/bin/bash

if [ "$#" -lt 1 ]; then
    echo "사용법: $0 <algorithm_name> [repeat] [input_type]"
    exit 1
fi

algo=$1
repeat=${2:-10}         
input_type=${3:-random} 

echo "|=====Algorithm=====|: $algo"
echo "|======Repeat=======|: $repeat"
echo "|=====Input Type====|: $input_type"

for ((i=1; i<=repeat; i++))
do
    echo "======================"
    echo "  ...execute $i ...   "
    ./build/$algo $input_type
    echo "======================"
done
