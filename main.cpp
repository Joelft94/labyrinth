#include <iostream>
#include <vector>
#include <stack>
#include <cstdlib>
#include <ctime>


/* Se declaran las constantes WIDTH y HEIGHT que tienen que ser impares para que terminen en pared y no en espacio
por ej: pared en la fila 0 luego espacio luego pared luego espacio y asi sucesivamente entonces debe terminar en impar para que termine en pared de nuevo */
const int WIDTH = 81;  // must be odd
const int HEIGHT = 21; // must be odd

// Se declara un enum Cell que tiene 3 valores WALL, PATH, VISITED_PATH
enum Cell {
    WALL,
    PATH,
    VISITED_PATH
};

// Se declara una estructura Position que tiene dos enteros x e y con un constructor que inicializa x e y
struct Position {
    int x, y;
    Position(int x, int y) : x(x), y(y) {}
};

// Se declara un vector de vectores para que sea 2D llamado maze que tiene HEIGHT filas y WIDTH columnas y se inicializa todas las cells como WALL
std::vector<std::vector<Cell>> maze(HEIGHT, std::vector<Cell>(WIDTH, WALL));

/* Se declara una funcion initializeMaze que recorre todas las cells del maze y las inicializa como WALL
Aunque sea redundante es para asegurar que todas las cells sean WALL */
void initializeMaze() {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            maze[y][x] = WALL;
        }
    }
}

/* Con esta funcion se imprime el laberinto en la terminal
Si la cell es WALL se imprime un '-' si la fila es par y un '|' si la fila es impar
Si la cell es PATH se imprime un espacio
Si la cell es VISITED_PATH se imprime un '*' */

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

// Se declara una funcion isInsideMaze que recibe dos enteros x e y y retorna un booleano para saber si esta dentro de los limites del laberinto
bool isInsideMaze(int x, int y) {
    return x > 0 && y > 0 && x < WIDTH - 1 && y < HEIGHT - 1;
}


/* Se declara una funcion getNeighbours que recibe una posicion y retorna un vector de posiciones que son los vecinos de la posicion pos
Se verifica si la posicion pos.x + 2 y pos.y esta dentro del laberinto y si lo esta se agrega al vector de vecinos y asi para cada direccion
Se verifican 2 posiciones en cada sentido para que al recorrer el algoritmo se mueva de a 2 posiciones para asegurar que se mueve de un camino a otro con una pared de por medio */
std::vector<Position> getNeighbours(Position& pos) {
    std::vector<Position> neighbours;
    if (isInsideMaze(pos.x + 2, pos.y)) neighbours.push_back(Position(pos.x + 2, pos.y)); //derecha
    if (isInsideMaze(pos.x - 2, pos.y)) neighbours.push_back(Position(pos.x - 2, pos.y)); //izquierda
    if (isInsideMaze(pos.x, pos.y + 2)) neighbours.push_back(Position(pos.x, pos.y + 2)); //abajo
    if (isInsideMaze(pos.x, pos.y - 2)) neighbours.push_back(Position(pos.x, pos.y - 2)); //arriba
    return neighbours; //retorna un vector de posiciones que son los vecinos de la posicion pos
}

/* Se declara una funcion generateMaze que recibe dos enteros startX y startY que son las coordenadas iniciales del laberinto
Se utiliza el metodo DFS Depth First Search para generar el laberinto
Se declara una pila de posiciones llamada stack y se crea una posicion current con las coordenadas iniciales
Se marca la posicion current como PATH y se agrega al stack
Continua explorando los vecinos no visitados
Para cada celda encuentra todos los vecinos no visitados
Si hay vecinos sin visitar, elige de manera aleatoria uno de ellos y marca el camino entre la celda actual y la celda escogida como PATH y agrega al stack(pila)
Si no hay vecinos sin visitar hace backtrack haciendo pop en el stack */
void generateMaze(int startX, int startY) {
    std::stack<Position> stack;
    Position current(startX, startY);
    maze[startY][startX] = PATH;
    stack.push(current);

/* El loop continua hasta que el stack este vacio
la posicion actual se toma del top(cima) del stack
con getNeighbours obtenemos los posibles vecinos para visitar
se crea un vector unvisited para guardar los vecinos que aun son WALL esto se va a usar para decidir donde ir luego */
    while (!stack.empty()) {
        current = stack.top();
        std::vector<Position> neighbours = getNeighbours(current);
        std::vector<Position> unvisited;

/* Se itera por cada vecino en el vector neighbours 
Para cada vecino verifica si sigue siendo WALL
Si es WALL se agrega al vector unvisited que son posiciones potenciales para recorrer luego */
        for (Position& neighbour : neighbours) {
            if (maze[neighbour.y][neighbour.x] == WALL) {
                unvisited.push_back(neighbour);
            }
        }
/* Si no hay vecinos sin visitar el algoritmo procede a crear el camino
Se elige un vecino de manera aleatoria y se marca como PATH
Se marca el camino entre la celda actual y la celda escogida como PATH
Se agrega la celda escogida al stack(pila)
Si no hay vecinos sin visitar se hace pop en el stack
y vuelve a la celda anterior para continuar generando el laberinto
 */
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

/* Esta funcion resuelve el laberinto utilizando DFS Depth First Search
Inicia en la posicion dada x e y
Si la posicion actual es la salida retorna true
Si la posicion actual es un PATH se marca como VISITED_PATH
Se mueve a la izquierda, derecha, arriba y abajo tratando de resolver el laberinto
Si no puede moverse a ninguna direccion hace backtrack y marca la celda actual como PATH
Si no puede resolver el laberinto retorna false y marca las celdas como PATH
Al final solo las celdas que forman el camino de la salida estaran marcadas como VISITED_PATH que son las que se van a visualizar en el laberinto con un "*"
 */
bool solveMaze(int x, int y) {
    if (x == WIDTH - 2 && y == HEIGHT - 2) { //Si llega a la salida (esquina inferior derecha) retorna true
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
/* La funcion main inicializa el laberinto, genera el laberinto, imprime el laberinto, resuelve el laberinto e imprime el laberinto resuelto
Se inicializa el seed (srand) para que los numeros aleatorios sean diferentes cada vez que se ejecute el programa
Se inicializa el laberinto
Se genera el laberinto iniciando en la posicion 1, 1
Se imprime el laberinto
Se resuelve el laberinto iniciando en la posicion 1, 1
Se imprime el laberinto resuelto 
 */
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
