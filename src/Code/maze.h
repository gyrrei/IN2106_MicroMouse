#ifndef Maze
#define Maze

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "timer.h"
#include "control.h"
#include "uart.h"
#include "sensor.h"
#include "gpio.h"

#define VISITED(node)	(node & 0b0000100000000000)
#define NORTH_Wall(node)(node & 0b0001000000000000)
#define EAST_Wall(node) (node & 0b0010000000000000)
#define SOUTH_Wall(node)(node & 0b0100000000000000)
#define WEST_Wall(node) (node & 0b1000000000000000)
#define DISTANCE(node)	(node & 0b0000011111111111)

#define SET_VISITED(node)			(node = node | 0b0000100000000000)
#define SET_NORTH_Wall(node)		(node = node | 0b0001000000000000)
#define SET_EAST_Wall(node)			(node = node | 0b0010000000000000)
#define SET_SOUTH_Wall(node)		(node = node | 0b0100000000000000)
#define SET_WEST_Wall(node)			(node = node | 0b1000000000000000)
#define SET_DISTANCE(node, value)	(node = (node & 0b1111100000000000) | value)

#define LAST_X(last) ((last & 0xFF00) >> 8)
#define LAST_Y(last) (last & 0x00FF)

#define SET_LAST_X(last, value) (last = (last & 0x00FF) | (value << 8))
#define SET_LAST_Y(last, value) (last = (last & 0xFF00) | value)

extern int last[6][6];

void driveSP();
void shortestPath(int X, int Y, int distance);
void explore();
void printmaze();
int neighborNorthUnvisited(int X, int Y);
int neighborEastUnvisited(int X, int Y);
int neighborSouthUnvisited(int X, int Y);
int neighborWestUnvisited(int X, int Y);
void updateDistanceNorth(int X, int Y);
void updateDistanceEast(int X, int Y);
void updateDistanceSouth(int X, int Y);
void updateDistanceWest(int X, int Y);
void setWalls(int X, int Y, char dir, int right, int left, int front);
int getRight();
int getLeft();
int getFront();
void setBorder();
int min(int a, int b);
void wait(int lim);
char driveToLast(int current_X, int current_Y, int new_X, int new_Y, char direction, int time);
void maze_forward(int lim);
void maze_turn_left(int lim);
void maze_turn_right(int lim);
void maze_turn_180(int lim);

#endif