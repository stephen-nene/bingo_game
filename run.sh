#!/bin/bash

# Function to compile and run C++ program
compile_and_run() {
    local file="$1"
    local base_name="${file%.*}"
    
    # Check if file exists
    if [ ! -e "$file" ]; then
        echo "Error: File '$file' not found."
        exit 1
    fi

    # Compile if file is .cpp
    if [[ "$file" == *.cpp ]]; then
        g++ -o "$base_name.o" "$file"
        if [ $? -ne 0 ]; then
            echo "Compilation failed."
            exit 1
        fi
    fi

    # Run the program
    "./$base_name.o"

    # Delete .o file if -n option is not provided
    if [ "$NO_DELETE" != "true" ]; then
        rm -f "$base_name.o"
    fi
}

# Check if -n option is provided
if [[ "$1" == "-n" ]]; then
    NO_DELETE="true"
    shift
fi

# Check if argument is provided
if [ $# -eq 0 ]; then
    echo "Usage: $0 [-n] <file>"
    exit 1
fi

# Loop through each argument
for file in "$@"; do
    compile_and_run "$file"
done
