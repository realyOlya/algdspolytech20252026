#include "main.h"
#include <gtest/gtest.h>
#include <vector>


std::vector<Point> to_vector(Point *path, size_t len) {
    std::vector<Point> v;
    for (size_t i = 0; i < len; ++i) {
        v.push_back(path[i]);
    }
    return v;
}


TEST(MazeTest, SimplePath_no1) {
    const char *grid[] = {
        "S E"
    };
    Maze *maze = maze_create(grid, 1);
    ASSERT_NE(maze, nullptr);

    Point start = {0, 0};
    Point *path = nullptr;
    size_t len = 0;

    ASSERT_TRUE(find_path(maze, start, &path, &len));
    ASSERT_EQ(len, 3);
    auto v = to_vector(path, len);
    EXPECT_EQ(v[0].x, 0); EXPECT_EQ(v[0].y, 0);
    EXPECT_EQ(v[1].x, 0); EXPECT_EQ(v[1].y, 1);
    EXPECT_EQ(v[2].x, 0); EXPECT_EQ(v[2].y, 2);

    free(path);
    maze_destroy(maze);
}


TEST(MazeTest, NoExit_no2) {
    const char *grid[] = {
        "S X"
    };
    Maze *maze = maze_create(grid, 1);
    ASSERT_NE(maze, nullptr);

    Point start = {0, 0};
    Point *path = nullptr;
    size_t len = 0;

    EXPECT_FALSE(find_path(maze, start, &path, &len));
    maze_destroy(maze);
}


TEST(MazeTest, ExitInCorner_no3) {
    const char *grid[] = {
        "SXX",
        "  X",
        "  E"
    };
    Maze *maze = maze_create(grid, 3);
    ASSERT_NE(maze, nullptr);

    Point start = {0, 0};
    Point *path = nullptr;
    size_t len = 0;

    ASSERT_TRUE(find_path(maze, start, &path, &len));
    ASSERT_EQ(len, 5);
    auto v = to_vector(path, len);
    EXPECT_EQ(v.back().x, 2); EXPECT_EQ(v.back().y, 2);

    free(path);
    maze_destroy(maze);
}


TEST(MazeTest, StartIsExit_no4) {
    const char *grid[] = {"E"};
    Maze *maze = maze_create(grid, 1);
    ASSERT_NE(maze, nullptr);

    Point start = {0, 0};
    Point *path = nullptr;
    size_t len = 0;

    ASSERT_TRUE(find_path(maze, start, &path, &len));
    ASSERT_EQ(len, 1);
    EXPECT_EQ(path[0].x, 0); EXPECT_EQ(path[0].y, 0);

    free(path);
    maze_destroy(maze);
}


TEST(MazeTest, SingleCellNoExit_no5) {
    const char *grid[] = {"S"};
    Maze *maze = maze_create(grid, 1);
    ASSERT_NE(maze, nullptr);

    Point start = {0, 0};
    Point *path = nullptr;
    size_t len = 0;

    EXPECT_FALSE(find_path(maze, start, &path, &len));
    maze_destroy(maze);
}


TEST(MazeTest, BlockedPath_no6) {
    const char *grid[] = {
        "SXXE"
    };
    Maze *maze = maze_create(grid, 1);
    ASSERT_NE(maze, nullptr);

    Point start = {0, 0};
    Point *path = nullptr;
    size_t len = 0;

    EXPECT_FALSE(find_path(maze, start, &path, &len));
    maze_destroy(maze);
}


TEST(MazeTest, ComplexMaze_no7) {
    const char *grid[] = {
        "S   X   ",
        "XX  X  X",
        "    X  X",
        "XXXXX  X",
        "       E"
    };
    Maze *maze = maze_create(grid, 5);
    ASSERT_NE(maze, nullptr);

    Point start = {0, 0};
    Point *path = nullptr;
    size_t len = 0;

    ASSERT_TRUE(find_path(maze, start, &path, &len));
    EXPECT_GT(len, 10);
    EXPECT_EQ(path[len - 1].x, 4);
    EXPECT_EQ(path[len - 1].y, 7);

    free(path);
    maze_destroy(maze);
}


TEST(MazeTest, MultipleExits_no8) {
    const char *grid[] = {
        "S E X E"
    };
    Maze *maze = maze_create(grid, 1);
    ASSERT_NE(maze, nullptr);

    Point start = {0, 0};
    Point *path = nullptr;
    size_t len = 0;

    ASSERT_TRUE(find_path(maze, start, &path, &len));
    ASSERT_EQ(len, 3); // до первого 'E'

    free(path);
    maze_destroy(maze);
}


TEST(MazeTest, NullMaze_no9) {
    Point *path = nullptr;
    size_t len = 0;
    EXPECT_FALSE(find_path(NULL, (Point){0,0}, &path, &len));
}


TEST(MazeTest, StartOnWall_no10) {
    const char *grid[] = {"X"};
    Maze *maze = maze_create(grid, 1);
    ASSERT_NE(maze, nullptr);

    Point start = {0, 0};
    Point *path = nullptr;
    size_t len = 0;

    EXPECT_FALSE(find_path(maze, start, &path, &len));
    maze_destroy(maze);
}