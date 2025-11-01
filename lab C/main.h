#ifndef MAZE_H
#define MAZE_H

#include <stddef.h>


typedef struct 
{
    int x;
    int y;
} Point;


typedef struct 
{
    char **grid;      
    int rows;
    int cols;
} Maze;

Maze *maze_create(const char *lines[], int rows);

void maze_destroy(Maze *maze);

int find_path(const Maze *maze, Point start, Point **path, size_t *path_len);

#endif // MAZE_H