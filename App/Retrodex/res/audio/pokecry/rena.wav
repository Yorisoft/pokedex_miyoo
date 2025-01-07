#!/bin/bash

# Check if there are any files in the directory
if [ "$(ls -1 | wc -l)" -eq 0 ]; then
    echo "No files found in the current directory."
    exit 1
fi

# Loop through all files in the current directory
for file in *; do
    # Skip if it's not a regular file
    if [ ! -f "$file" ]; then
        continue
    fi

    # Get the first 4 characters of the filename
    new_name="${file:0:4}.wav"

    # Rename the file
    mv -v "$file" "$new_name"
done

echo "Renaming completed!"
