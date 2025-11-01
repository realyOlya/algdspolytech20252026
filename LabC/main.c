#include "main.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define QUEUE_CAPACITY 10000

typedef struct 
{
    Point *data;
    size_t head;
    size_t tail;
    size_t capacity;
} Queue;

Queue *queue_create(size_t capacity) 
{
    Queue *q = malloc(sizeof(Queue));
    if (!q) return NULL;
    q->data = malloc(sizeof(Point) * capacity);
    if (!q->data) 
    {
        free(q);
        return NULL;
    }
    q->head = 0;
    q->tail = 0;
    q->capacity = capacity;
    return q;
}

void queue_destroy(Queue *q) 
{
    if (q) 
    {
        free(q->data);
        free(q);
    }
}

bool queue_empty(const Queue *q) 
{
    return q->head == q->tail;
}

bool queue_full(const Queue *q) 
{
    return (q->tail + 1) % q->capacity == q->head;
}

void queue_push(Queue *q, Point p) 
{
    if (queue_full(q)) return;
    q->data[q->tail] = p;
    q->tail = (q->tail + 1) % q->capacity;
}

Point queue_pop(Queue *q) 
{
    Point p = q->data[q->head];
    q->head = (q->head + 1) % q->capacity;
    return p;
}

Maze *maze_create(const char *lines[], int rows) 
{
    if (!lines || rows <= 0) return NULL;

    Maze *maze = malloc(sizeof(Maze));
    if (!maze) return NULL;

    maze->rows = rows;
    maze->cols = (int)strlen(lines[0]);

    maze->grid = malloc(sizeof(char *) * rows);
    if (!maze->grid) 
    {
        free(maze);
        return NULL;
    }

    for (int i = 0; i < rows; ++i) 
    {
        if ((int)strlen(lines[i]) != maze->cols)
        {
            for (int j = 0; j < i; ++j) free(maze->grid[j]);
            free(maze->grid);
            free(maze);
            return NULL;
        }
        maze->grid[i] = strdup(lines[i]);
        if (!maze->grid[i]) 
        {
            for (int j = 0; j <= i; ++j) free(maze->grid[j]);
            free(maze->grid);
            free(maze);
            return NULL;
        }
    }
    return maze;
}

void maze_destroy(Maze *maze) 
{
    if (!maze) return;
    for (int i = 0; i < maze->rows; ++i) 
    {
        free(maze->grid[i]);
    }
    free(maze->grid);
    free(maze);
}

bool is_valid(const Maze *maze, int x, int y) 
{
    return x >= 0 && x < maze->rows && y >= 0 && y < maze->cols &&
           (maze->grid[x][y] == ' ' || maze->grid[x][y] == 'S' || maze->grid[x][y] == 'E');
}

int find_path(const Maze *maze, Point start, Point **path, size_t *path_len) {
    if (!maze || !path || !path_len) return 0;
    if (!is_valid(maze, start.x, start.y)) return 0;


    Point **prev = malloc(sizeof(Point *) * maze->rows);
    for (int i = 0; i < maze->rows; ++i) 
    {
        prev[i] = malloc(sizeof(Point) * maze->cols);
        for (int j = 0; j < maze->cols; ++j) 
        {
            prev[i][j].x = -1;
            prev[i][j].y = -1;
        }
    }

    Queue *q = queue_create(QUEUE_CAPACITY);
    queue_push(q, start);
    prev[start.x][start.y].x = start.x;
    prev[start.x][start.y].y = start.y;

    int dx[4] = {-1, 1, 0, 0};
    int dy[4] = {0, 0, -1, 1};

    Point exit = {-1, -1};

    while (!queue_empty(q)) {
        Point cur = queue_pop(q);

        if (maze->grid[cur.x][cur.y] == 'E') 
        {
            exit = cur;
            break;
        }

        for (int i = 0; i < 4; ++i) 
        {
            int nx = cur.x + dx[i];
            int ny = cur.y + dy[i];
            if (is_valid(maze, nx, ny) && prev[nx][ny].x == -1) 
            {
                prev[nx][ny].x = cur.x;
                prev[nx][ny].y = cur.y;
                queue_push(q, (Point){nx, ny});
            }
        }
    }

    queue_destroy(q);

    if (exit.x == -1) 
    {
        for (int i = 0; i < maze->rows; ++i) free(prev[i]);
        free(prev);
        return 0;
    }

    size_t len = 0;
    Point cur = exit;
    while (!(cur.x == start.x && cur.y == start.y))
    {
        ++len;
        cur = prev[cur.x][cur.y];
    }
    ++len; 

    *path = malloc(sizeof(Point) * len);
    if (!(*path)) 
    {
        for (int i = 0; i < maze->rows; ++i) free(prev[i]);
        free(prev);
        return 0;
    }

    cur = exit;
    size_t idx = len - 1;
    (*path)[idx--] = exit;
    while (!(cur.x == start.x && cur.y == start.y)) 
    {
        cur = prev[cur.x][cur.y];
        (*path)[idx--] = cur;
    }

    *path_len = len;

    for (int i = 0; i < maze->rows; ++i) free(prev[i]);
    free(prev);
    return 1;
}