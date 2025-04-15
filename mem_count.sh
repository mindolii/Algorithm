#!/bin/bash

BUILD_DIR="./build"
INPUT_DIR="./input_data"
OUTPUT_DIR="./heaptrack_output"

mkdir -p "$OUTPUT_DIR"

input_files=("sorted_inc")

selected_algorithms=("selection_sort")

for algo_name in "${selected_algorithms[@]}"; do
  algo_path="${BUILD_DIR}/${algo_name}"


  if [[ ! -f "$algo_path" ]]; then
    echo "⚠️  Executable not found: $algo_path"
    continue
  fi

  for input_name in "${input_files[@]}"; do
    echo "▶ Running heaptrack for ${algo_name} with input ${input_name}..."

    heaptrack_output=$(heaptrack "$algo_path" "$input_name" 2>&1)

    gz_file=$(echo "$heaptrack_output" | grep -oP 'heaptrack\..*?\.gz')

    if [[ -f "$gz_file" ]]; then
      new_gz_name="${algo_name}_${input_name}.gz"
      mv "$gz_file" "${OUTPUT_DIR}/${new_gz_name}"
      echo "📦 Saved: ${OUTPUT_DIR}/${new_gz_name}"
    else
      echo "⚠️  Failed to find .gz output for ${algo_name} with ${input_name}"
      echo "$heaptrack_output" > "${OUTPUT_DIR}/${algo_name}_${input_name}_log.txt"
    fi

    echo "✅ Done for ${algo_name} with ${input_name}"
    echo
  done
done

echo "🎉 Heaptrack runs complete for selected algorithms. Results in: $OUTPUT_DIR"
