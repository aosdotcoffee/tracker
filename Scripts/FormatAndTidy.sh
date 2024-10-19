#!/bin/bash
set -euo pipefail

DIRECTORIES=(
    "Source/"
)

PATTERN_EXCLUDE=''

PATTERNS=(
    "*.h"
    "*.c"
)

NUM_THREADS=$(( $(nproc) * 2 ))
if [ "$#" = "1" ]; then
    NUM_THREADS="$1"
fi

CHECK() {
    command -v "$*" >/dev/null || (echo "$* not found, please install it."; exit 1)
}

IN_PARALLEL() {
    directory=$1
    pattern=$2
    exclude=$3
    threads=$4
    command=$5
    shift 4

    if [ "$PATTERN_EXCLUDE" != "" ]; then
        find "$directory" \
            -type f \
            -iname "$pattern" |
        grep -Ev "$PATTERN_EXCLUDE" |
        parallel -j$NUM_THREADS -t $*
    else
        find "$directory" \
            -type f \
            -iname "$pattern" |
        parallel -j$NUM_THREADS -t $*
    fi
}

TITLE() {
    cols=$(tput cols)
    text=$1
    length=${#text}
    pad=$(( cols / 2 + length / 2 - 4 ))

    printf "%${cols}s\n" "" | tr " " "*"
    printf "%${pad}s\n" "$text"
    printf "%${cols}s\n" "" | tr " " "*"
}

CHECK clang-tidy
CHECK parallel
CHECK cppcheck

for directory in "${DIRECTORIES[@]}"; do
    for pattern in "${PATTERNS[@]}"; do
        TITLE "clang-format: $directory ($pattern)"
        IN_PARALLEL \
            "$directory" \
            "$pattern" \
            "$PATTERN_EXCLUDE" \
            "$NUM_THREADS" \
            clang-format \
            -i \
            --style=file:.clang-format

        TITLE "clang-tidy: $directory ($pattern)"
        IN_PARALLEL \
            "$directory" \
            "$pattern" \
            "$PATTERN_EXCLUDE" \
            "$NUM_THREADS" \
            clang-tidy \
            --config-file=.clang-tidy \
            -p build/
    done

    TITLE "cppcheck: $directory"
    cppcheck \
        --enable=all \
        --suppress=missingIncludeSystem \
        --check-level=exhaustive \
        "$directory"
done
