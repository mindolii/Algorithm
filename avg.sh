#!/bin/bash

TARGET_DIR="1000000/analysis_1000000"

for file in "$TARGET_DIR"/*.txt; do
    sum=0
    count=0

    while read -r line; do
        if [[ "$line" == *"running time:"* ]]; then
            time=$(echo "$line" | grep -oP 'running time: \K[0-9.]+')
            sum=$(echo "$sum + $time" | bc -l)
            ((count++))
        fi
    done < "$file"

    if (( count > 0 )); then
        avg=$(echo "$sum / $count" | bc -l)
        avg_formatted=$(printf "%.6f" "$avg")

        echo "" >> "$file"
        echo "Average running time: $avg_formatted sec" >> "$file"
        echo "✔ $(basename "$file") 평균 추가됨: $avg_formatted sec"
    else
        echo "⚠ $(basename "$file") 에서 'running time' 데이터를 찾을 수 없습니다."
    fi
done
