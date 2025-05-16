#!/bin/bash

# Ensure the output directory exists
rm -rf output
mkdir -p output

# ANSI color codes
GREEN="\e[32m"
YELLOW="\e[33m"
RED="\e[31m"
RESET="\e[0m"

# Associative array to hold PIDs and corresponding output files
declare -A pid_to_output

# Function to check the last line of the output file and detect leaks
check_output() {
    local output="$1"
    if [[ -s "$output" ]]; then
        last_line=$(tail -n 1 "$output")
        if grep -qi "leak" "$output"; then
            echo -e "${YELLOW}$output (potential memory leak detected) ❓${RESET}"
        elif [[ "$last_line" == *"test passed."* ]]; then
            echo -e "${GREEN}$output ✅${RESET}"
        else
            echo -e "${RED}$output ❌${RESET}"
        fi
    else
        echo -e "${RED}$output (empty file) ❌${RESET}"
    fi
}

# Start all tests and store their PIDs
for file in tests/*.in; do
    filename=$(basename "$file" .in)
    output_file="output/$filename.out"

    # Run the command in the background; redirect stdout and stderr
    ./grader ./engine < "$file" &> "$output_file" &
    pid_to_output[$!]="$output_file"
done

# Wait for all processes to finish by checking if the PID array is empty
while [ ${#pid_to_output[@]} -gt 0 ]; do
    for pid in "${!pid_to_output[@]}"; do
        if ! ps -p "$pid" > /dev/null 2>&1; then
            # Process finished; wait to collect its exit status and check output
            wait "$pid"
            output="${pid_to_output[$pid]}"
            check_output "$output"
            # Remove the finished PID from the array
            unset pid_to_output[$pid]
        fi
    done
    sleep 0.1  # Prevent busy-waiting
done

echo -e "\nAll tests completed."
