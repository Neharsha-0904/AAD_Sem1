# Algorithm Design - Class Labs

This repository contains Python and C++ implementations of the recorded class-lab problems.

## Structure
- `Python Implementation/`: standalone Python programs grouped by experiment.
- `C++ Implementation/`: standalone C++ programs grouped by experiment.
- `Documentation/`: consolidated practical lab manual in DOCX format.

## Running programs
Python: `python filename.py`
C++: `g++ -std=c++17 filename.cpp -o program && ./program`

Each file includes a small demonstration in its `main` block / `main()` function. Replace the demonstration data with the values from your lab question.

## Notes on assumptions
- Experiment 4 LCS is interpreted as Longest Common Subsequence.
- Experiment 4 LIS is interpreted as Longest Increasing Subsequence.
- Experiment 4 path matching is intentionally left as a placeholder until the exact problem statement is supplied.
- Experiment 6 linked-list deletion is interpreted as deleting the 1-based kth node from the head. The auxiliary jump-pointer detail needs confirmation.
- Experiment 6 postman tour is implemented as a Hamiltonian cycle because the stated requirement says visit every vertex exactly once and return to the source. The general Chinese Postman Problem is different.
- The Experiment 3 multiple-stack solution uses a shared array and free-list, allowing stacks to share capacity.

All programs are educational reference implementations. Check input constraints and adapt demonstrations to the exact test cases used by your instructor.
