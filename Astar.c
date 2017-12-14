/*
 * A C implementation of the A* algorithm
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

#define FAIL(msg) {\
    fprintf(stderr, "Error [%s:%d]: "msg"\n",\
            __FILE__, __LINE__);\
    exit(EXIT_FAILURE);\
}

/*
 * This structure represents a node in the graph
 */
typedef struct Cell {
    double h;              /* Heuristic value                  */
    double g;              /* Movement cost                    */
    double f;              /* The sum: g+h                     */
    unsigned int x;        /* The row of the cell              */
    unsigned int y;        /* The column of the cell           */
    unsigned int blocked;  /* True or false                    */
    unsigned int path;     /* True or false                    */
    struct Cell *parent;   /* Node to reach this node          */
    struct Cell *next;     /* Pointer to next node on the list */
} Cell;


/*
 * The list containing all the neighbors of the
 * nodes that we've visited
 */
Cell *openedList = NULL;

/*
 * The list containing all the nodes we've visited
 */
Cell *closedList = NULL;

/*
 * The graph is nothing more than a 2D array of Cells
 */
Cell **graph = NULL;


Cell *source;      /* The source cell      */
Cell *destination; /* The destination cell */

unsigned int rows; /* The number of rows in the graph    */
unsigned int cols; /* The number of columns in the graph */

/*
 * A flag to delete the source from the opened list
 * right after we enter the loop
 */
unsigned int checkSrc = 1;


/*
 * Inserts cells in the above lists
 */
void insert(Cell **list, Cell *cell)
{
    if (*list)
        cell->next = *list;

    *list = cell;
}

/*
 * Deletes cells from the above lists
 */
void delete(Cell **list, Cell *cell)
{
    Cell *prev = NULL;
    Cell *curr = *list;

    while (curr) {
        if (curr->x == cell->x && curr->y == cell->y) {
            if (!prev) {
                *list = curr->next;
            }
            else {
                prev->next = curr->next;
                curr->next = NULL;
            }
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

/*
 * Checks if a cell exists in a list
 */
unsigned int isInside(Cell **list, Cell *cell)
{
    Cell *curr = *list;

    while (curr) {
        if (curr->x == cell->x && curr->y == cell->y)
            return 1;
        curr = curr->next;
    }

    return 0;
}

/*
 * Returns the cell with the minimum f value
 * in the openedList
 */
Cell *getNext(Cell **list)
{
    Cell *min = *list;
    Cell *curr = (*list)->next;

    while (curr) {
        if (curr->f < min->f)
            min = curr;
        curr = curr->next;
    }

    return min;
}

/*
 * Checks if the coords of a cell are valid
 */
unsigned int isValid(int row, int col)
{
    if ((row >= 0 && row < rows) && (col >= 0 && col < cols))
        return 1;
    return 0;
}

/*
 * Calculates the heuristic value of a cell
 */
unsigned int heuristic(Cell *cell, Cell *dst)
{
    return abs(cell->x - dst->x) + abs(cell->y - dst->y);
    /* return sqrt (pow(cell->x - dst->x, 2) + pow(cell->y - dst->y, 2)); */
}

/*
 * Prints the path from destination to source
 */
void trace(Cell *dst)
{
    int i, j;

    while (dst) {
        dst->path = 1;
        dst = dst->parent;
    }

    printf("\t\t");
    for (i=0; i<rows; i++) {
        for (j=0; j<cols; j++) {
            if (graph[i][j].blocked)
                printf("X ");
            else if (graph[i][j].path)
                printf("* ");
            else
                printf("- ");
        }
        printf("\n\t\t");
    }
    printf("\n");
}

/*
 * The A* algorithm. Calculates the path from destination
 * to source. We do not allow diagonal movement.
 */
void Astar(Cell *src, Cell *dst)
{
    double g, h, f;

    while (openedList) {
        Cell *curr = getNext(&openedList);

        if (checkSrc) {
            delete(&openedList, src);
            checkSrc = 0;
        }

        /* North */
        if (isValid(curr->x-1, curr->y)) {
            if (curr == dst) {
                printf(
                        "\t\t--------------------\n"
                        "\t\t|  Path found !!!  |\n"
                        "\t\t--------------------\n\n");
                trace(dst);
                return;
            }

            if (!isInside(&closedList, &graph[curr->x-1][curr->y])
                    && !graph[curr->x-1][curr->y].blocked) {

                g = curr->g + 1.0;
                h = heuristic(&graph[curr->x-1][curr->y], dst);
                f = g + h;

                if (graph[curr->x-1][curr->y].f > f) {
                    if (graph[curr->x-1][curr->y].f == DBL_MAX)
                        insert(&openedList, &graph[curr->x-1][curr->y]);

                    graph[curr->x-1][curr->y].g = g;
                    graph[curr->x-1][curr->y].h = h;
                    graph[curr->x-1][curr->y].f = f;
                    graph[curr->x-1][curr->y].parent = curr;
                }
            }
        }

        /* South */
        if (isValid(curr->x+1, curr->y)) {
            if (curr == dst) {
                printf("Path found!!!\n");
                trace(dst);
                return;
            }

            if (!isInside(&closedList, &graph[curr->x+1][curr->y])
                    && !graph[curr->x+1][curr->y].blocked) {

                g = curr->g + 1.0;
                h = heuristic(&graph[curr->x+1][curr->y], dst);
                f = g + h;

                if (graph[curr->x+1][curr->y].f > f) {
                    if (graph[curr->x+1][curr->y].f == DBL_MAX)
                        insert(&openedList, &graph[curr->x+1][curr->y]);

                    graph[curr->x+1][curr->y].g = g;
                    graph[curr->x+1][curr->y].h = h;
                    graph[curr->x+1][curr->y].f = f;
                    graph[curr->x+1][curr->y].parent = curr;
                }
            }
        }

        /* East */
        if (isValid(curr->x, curr->y+1)) {
            if (curr == dst) {
                printf("Path found!!!\n");
                trace(dst);
                return;
            }

            if (!isInside(&closedList, &graph[curr->x][curr->y+1])
                    && !graph[curr->x][curr->y+1].blocked) {

                g = curr->g + 1.0;
                h = heuristic(&graph[curr->x][curr->y+1], dst);
                f = g + h;

                if (graph[curr->x][curr->y+1].f > f) {
                    if (graph[curr->x][curr->y+1].f == DBL_MAX)
                        insert(&openedList, &graph[curr->x][curr->y+1]);

                    graph[curr->x][curr->y+1].g = g;
                    graph[curr->x][curr->y+1].h = h;
                    graph[curr->x][curr->y+1].f = f;
                    graph[curr->x][curr->y+1].parent = curr;
                }
            }
        }

        /* West */
        if (isValid(curr->x, curr->y-1)) {
            if (curr == dst) {
                printf("Path found!!!\n");
                trace(dst);
                return;
            }

            if (!isInside(&closedList, &graph[curr->x][curr->y-1])
                    && !graph[curr->x][curr->y-1].blocked) {

                g = curr->g + 1.0;
                h = heuristic(&graph[curr->x][curr->y-1], dst);
                f = g + h;

                if (graph[curr->x][curr->y-1].f > f) {
                    if (graph[curr->x][curr->y-1].f == DBL_MAX)
                        insert(&openedList, &graph[curr->x][curr->y-1]);

                    graph[curr->x][curr->y-1].g = g;
                    graph[curr->x][curr->y-1].h = h;
                    graph[curr->x][curr->y-1].f = f;
                    graph[curr->x][curr->y-1].parent = curr;
                }
            }
        }

        delete(&openedList, curr);
        insert(&closedList, curr);
    }
}

/*
 * Parses the graph from a file. The file contains only
 * ones and zeros + the dimensions of the array.
 * - 0: Non-blocking cell
 * - 1: Blocking cell
 */
void parseGraph(int argc, char *argv[])
{
    int i, j;
    FILE *map = NULL;

    if (argc != 6)
        FAIL("\n\nUsage: \n"
                "\t./Astar <map.txt> <srcx> <srcy> <dstx> <dsty>\n");

    if (!(map = fopen(argv[1], "r")))
        FAIL("Can't open map file");

    fscanf(map, "%d", &rows);
    fscanf(map, "%d", &cols);

    if (atoi(argv[2]) < 0 || atoi(argv[2]) >= rows
            || atoi(argv[3]) < 0 || atoi(argv[3]) >= cols
            || atoi(argv[4]) < 0 || atoi(argv[4]) >= rows
            || atoi(argv[5]) < 0 || atoi(argv[5]) >= cols)
        FAIL("Invalid coordinates");

    if (!(graph = (Cell **)malloc(rows*sizeof(Cell *))))
        FAIL("Can't allocate graph");

    for (i=0; i<rows; i++)
        if (!(graph[i] = (Cell *)malloc(cols*sizeof(Cell))))
            FAIL("Can't allocate graph");

    for (i=0; i<rows; i++) {
        for (j=0; j<cols; j++) {
            graph[i][j].h = 0;
            graph[i][j].g = 0;
            graph[i][j].f = DBL_MAX;
            graph[i][j].x = i;
            graph[i][j].y = j;

            fscanf(map, "%d", &graph[i][j].blocked);

            graph[i][j].path = 0;
            graph[i][j].parent = NULL;
            graph[i][j].next = NULL;
        }
    }

    fclose(map);

    source = &graph[atoi(argv[2])][atoi(argv[3])];
    destination = &graph[atoi(argv[4])][atoi(argv[5])];

    insert(&openedList, source);
}

/*
 * Frees resources
 */
void free_all()
{
    int i, j;

    for (i=0; i<rows; i++)
            free(graph[i]);
    free(graph);
}

/*
 * Main function
 */
int main(int argc, char *argv[])
{
    parseGraph(argc, argv);

    Astar(source, destination);

    free_all();
    return 0;
}
