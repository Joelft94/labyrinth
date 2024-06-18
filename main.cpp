#include <iostream>
#include <vector>
#include <stack>
#include <cstdlib>
#include <ctime>

const int WIDTH = 81;  // must be odd
const int HEIGHT = 31; // must be odd

enum Cell {
    WALL,
    PATH,
    VISITED_PATH
};

struct Position {
    int x, y;
    Position(int x, int y) : x(x), y(y) {}
};

std::vector<std::vector<Cell>> maze(HEIGHT, std::vector<Cell>(WIDTH, WALL));

void initializeMaze() {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            maze[y][x] = WALL;
        }
    }
}

void printMaze() {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (maze[y][x] == WALL) {
                if (y % 2 == 0) {
                    std::cout << '-';
                } else {
                    std::cout << '|';
                }
            } else if (maze[y][x] == PATH) {
                std::cout << ' ';
            } else if (maze[y][x] == VISITED_PATH) {
                std::cout << '*';
            }
        }
        std::cout << std::endl;
    }
}

bool isInsideMaze(int x, int y) {
    return x > 0 && y > 0 && x < WIDTH - 1 && y < HEIGHT - 1;
}

std::vector<Position> getNeighbours(Position& pos) {
    std::vector<Position> neighbours;
    if (isInsideMaze(pos.x + 2, pos.y)) neighbours.push_back(Position(pos.x + 2, pos.y));
    if (isInsideMaze(pos.x - 2, pos.y)) neighbours.push_back(Position(pos.x - 2, pos.y));
    if (isInsideMaze(pos.x, pos.y + 2)) neighbours.push_back(Position(pos.x, pos.y + 2));
    if (isInsideMaze(pos.x, pos.y - 2)) neighbours.push_back(Position(pos.x, pos.y - 2));
    return neighbours;
}

void generateMaze(int startX, int startY) {
    std::stack<Position> stack;
    Position current(startX, startY);
    maze[startY][startX] = PATH;
    stack.push(current);

    while (!stack.empty()) {
        current = stack.top();
        std::vector<Position> neighbours = getNeighbours(current);
        std::vector<Position> unvisited;

        for (Position& neighbour : neighbours) {
            if (maze[neighbour.y][neighbour.x] == WALL) {
                unvisited.push_back(neighbour);
            }
        }

        if (!unvisited.empty()) {
            Position chosen = unvisited[rand() % unvisited.size()];
            maze[(current.y + chosen.y) / 2][(current.x + chosen.x) / 2] = PATH;
            maze[chosen.y][chosen.x] = PATH;
            stack.push(chosen);
        } else {
            stack.pop();
        }
    }
}

bool solveMaze(int x, int y) {
    if (x == WIDTH - 2 && y == HEIGHT - 2) {
        maze[y][x] = VISITED_PATH;
        return true;
    }

    if (maze[y][x] == PATH) {
        maze[y][x] = VISITED_PATH;

        if (x > 0 && solveMaze(x - 1, y)) return true;
        if (x < WIDTH - 1 && solveMaze(x + 1, y)) return true;
        if (y > 0 && solveMaze(x, y - 1)) return true;
        if (y < HEIGHT - 1 && solveMaze(x, y + 1)) return true;

        maze[y][x] = PATH;
    }

    return false;
}

int main() {
    srand(time(0));
    initializeMaze();
    generateMaze(1, 1);
    printMaze();
    std::cout << "\nSolving maze:\n\n";
    solveMaze(1, 1);
    printMaze();
    return 0;
}
