import os
import re

def process_file(filename):
    # Read the content of the file
    with open(filename, 'r') as file:
        lines = file.readlines()

    # Remove the last underscore from each line
    cleaned_lines = [line.rstrip('_\n') + '\n' for line in lines]

    # Use a set to keep track of prefixes we've already seen
    seen_prefixes = set()
    unique_lines = []

    for line in cleaned_lines:
        prefix = line.split('_')[0]
        if prefix not in seen_prefixes:
            seen_prefixes.add(prefix)
            unique_lines.append(line)

    # Write the cleaned and unique lines back to the file
    with open(filename, 'w') as file:
        file.writelines(unique_lines)

def process_directory(directory):
    # Regex pattern to match the filenames
    pattern = re.compile(r'^\d+_[a-zA-Z]_\d+\.txt$')

    for filename in os.listdir(directory):
        if pattern.match(filename):
            filepath = os.path.join(directory, filename)
            process_file(filepath)

if __name__ == "__main__":
    # Change '.' to the directory you want to scan if different
    process_directory('.')
