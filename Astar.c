#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

#define FAIL(msg) {\
    fprintf(stderr, "Error [%s:%d]: "msg"\n",\
            __FILE__, __LINE__);\
    exit(EXIT_FAILURE);\
}

typedef struct Cell {
    double h;              /* Heuristic value                  */
    double g;              /* Movement cost                    */
    double f;              /* The sum: g+h                     */
    unsigned int x;        /* The row of the cell              */
    unsigned int y;        /* The column of the cell           */
    unsigned int blocked;  /* True or false                    */
    struct Cell *parent;   /* Node to reach this node          */
    struct Cell *next;     /* Pointer to next node on the list */
} Cell;


Cell *openedList = NULL;
Cell *closedList = NULL;

Cell **graph = NULL;
unsigned int rows;
unsigned int cols;

unsigned int checkSrc = 1;


void insert(Cell **list, Cell *cell)
{
    if (*list)
        cell->next = *list;

    *list = cell;
}

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

unsigned int isValid(int row, int col)
{
    if ((row >= 0 && row < rows) && (col >= 0 && col < cols))
        return 1;
    return 0;
}

unsigned int heuristic(Cell *src, Cell *dst)
{
    return abs(src->x - dst->x) + abs(src->y - dst->y);
}

void trace(Cell **graph, Cell *dst)
{
    while (dst) {
        printf("(%d, %d)\n", dst->x, dst->y);
        dst = dst->parent;
    }
}


void Astar(Cell **graph, Cell *src, Cell *dst)
{
    while (openedList) {
        Cell *curr = getNext(&openedList);

        if (checkSrc) {
            delete(&openedList, src);
            checkSrc = 0;
        }

        double g, h, f;

        /* North */
        if (isValid(curr->x-1, curr->y)) {
            if (curr == dst) {
                printf("Path found!!!\n");
                trace(graph, dst);
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
                trace(graph, dst);
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
                trace(graph, dst);
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
                trace(graph, dst);
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

void parseGraph(const char *file)
{
    int i, j;
    FILE *map = NULL;

    if (!(map = fopen(file, "r")))
        FAIL("Can't open map file");

    fscanf(map, "%d", &rows);
    fscanf(map, "%d", &cols);

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

            graph[i][j].parent = NULL;
            graph[i][j].next = NULL;
        }
    }

    fclose(map);
}

int main(int argc, char *argv[])
{
    Cell *src, *dst;

    parseGraph("map.txt");

    src = &graph[0][0];
    dst = &graph[9][9];

    insert(&openedList, src);

    Astar(graph, src, dst);

    return 0;
}
