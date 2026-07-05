#!/bin/bash

set +e

cd "$(dirname "$0")"

dir="shaders"
ext=".spv"

echo "-- Shader Compilation"

start_time=$(date +%s.%N)

# Remove previous .spv files
find "$dir" -type f -name "*$ext" -delete

# Collect shader files (excluding .spv)
mapfile -t files < <(find "$dir" -maxdepth 1 -type f ! -name "*$ext" | sort)

len=${#files[@]}
success=0
fail=0

# Compile shaders
for ((count=0; count<len; count++)); do
    target="${files[$count]}"
    compiled="${target}${ext}"

    printf "\033[36m# [%d/%d] Compiling %s...\033[0m\n" "$((count+1))" "$len" "$(basename "$target")"

    if glslc "$target" -o "$compiled"; then
        ((success++))
    else
        printf "\033[31m-- Failed to compile %s!\033[0m\n" "$(basename "$target")"
        ((fail++))
    fi
done

end_time=$(date +%s.%N)
elapsed=$(awk "BEGIN {printf \"%.2f\", $end_time - $start_time}")

printf -- "-- Shader Compilation Done (%ss), " "$elapsed"
printf "\033[36mSuccess %d\033[0m" "$success"
printf " | "
printf "\033[31mFail %d\033[0m" "$fail"
printf " | "
printf "Total %d\n" "$len"