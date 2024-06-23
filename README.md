Maze Generation and Solver
Project Overview
This project implements a maze generation and solving algorithm using C++. The maze is generated using a depth-first search (DFS) algorithm with backtracking and is displayed with vertical (|) and horizontal (-) walls. The maze solver also uses backtracking to find a solution path, which is displayed using *.

Features
Random Maze Generation: Generates a random maze starting from a specified position using the DFS algorithm.
Maze Solving: Solves the generated maze using backtracking and displays the solution path.
Visualization: Displays the maze with vertical and horizontal walls, paths, and the solution path.
Getting Started
Prerequisites
A C++ compiler (e.g., g++, clang)
C++11 standard or later

Generating the Maze:

The maze is generated starting from a specified position (startX, startY).
Modify the startX and startY values in the main function to change the starting point.

Solving the Maze:

The solver starts from the top-left corner (0, 0) and finds a path to the bottom-right corner (width-1, height-1).
The solution path is marked with *.

