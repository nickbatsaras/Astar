#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define TRUE  1
#define FALSE 0

#define ROWS 10
#define COLS 10


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


/* Calculate using Manhatan Distance */
unsigned int heuristic(Cell *curr, Cell *dest)
{
    return abs(curr->x - dest->x) + abs(curr->y - dest->y);
}


void Astar(Cell graph[ROWS][COLS], Cell *src, Cell *dst)
{
}


int main(int argc, char *argv[])
{
    int i, j;

    int matrix[ROWS][COLS] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    };

    Cell graph[ROWS][COLS];

    for (i=0; i<ROWS; i++) {
        for (j=0; j<COLS; j++) {
            graph[i][j].h = 0;
            graph[i][j].g = 0;
            graph[i][j].f = 0;
            graph[i][j].x = i;
            graph[i][j].y = j;
            graph[i][j].blocked = matrix[i][j];
            graph[i][j].parent = NULL;
            graph[i][j].next = NULL;
        }
    }

    Astar(graph, &graph[0][0], &graph[9][9]);

    return 0;
}
