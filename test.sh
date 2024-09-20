#!/bin/bash

# Compile the program with cc
echo "Compiling the program..."
cc -std=c11 -Wall -Werror -g -o runml runml.c

# Check if the compilation was successful
if [ $? -ne 0 ]; then
    echo "Compilation failed!"
    exit 1
fi

echo "Compilation successful."

# Loop through sample01.ml to sample08.ml and run the compiled program
for i in {1..8}
do
    # Format the sample file name
    sample_file=$(printf "sample0%d.ml" $i)

    # Check if the sample file exists
    if [ -f "$sample_file" ]; then
        echo "Running ./runml $sample_file"
        ./runml "$sample_file"
        echo ""
    else
        echo "File $sample_file not found, skipping..."
    fi
done
