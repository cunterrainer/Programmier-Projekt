import os
import re

def process_file(filename):
    with open(filename, 'r') as file:
        lines = file.readlines()

    cleaned_lines = [line.rstrip('_\n') + '\n' for line in lines] # Remove the last underscore from each line

    seen_prefixes = set()
    unique_lines = []

    for line in cleaned_lines:
        prefix = line.split('_')[0]
        if prefix not in seen_prefixes:
            seen_prefixes.add(prefix)
            unique_lines.append(line)

    with open(filename, 'w') as file:
        file.writelines(unique_lines)

def process_directory(directory):
    pattern = re.compile(r'^\d+_[a-zA-Z]_\d+\.txt$') # Regex pattern to match the filenames

    for filename in os.listdir(directory):
        if pattern.match(filename):
            filepath = os.path.join(directory, filename)
            process_file(filepath)

if __name__ == "__main__":
    process_directory('.')