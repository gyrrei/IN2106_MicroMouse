#include "maze.h"
#include "string.h"


// HOW TO USE:
// 
// 1. explore();
// 2. shortestPath(X, Y, DISTANCE(last[X][Y]));
// 3. driveSP();


// 2D array to store data for each cell
// Important!!! JUST access data via the DEFINEd commands
//
// Bits:
//
// 1-11: 	DISTANCE to start
// 12:		Visited flag
// 13:		(1) wall in the north (0) no wall in the north
// 14:		(1) wall in the east (0) no wall in the east
// 15:		(1) wall in the south (0) no wall in the south
// 16:		(1) wall in the west (0) no wall in the west


int node[6][6] = {
			{0,255,255,255,255,255},
			{255,255,255,255,255,255},
			{255,255,255,255,255,255},
			{255,255,255,255,255,255},
			{255,255,255,255,255,255},
			{255,255,255,255,255,255}
			};
			
// 2D array to store the X and Y index for bevorhand visited cell for each cell
// Important!!! JUST access data via the DEFINEd commands
//
// Bits:
//
// 1-8: 	Y
// 9-16:	X

int last[6][6] = {
			{-1,-1,-1,-1,-1,-1},
			{-1,-1,-1,-1,-1,-1},
			{-1,-1,-1,-1,-1,-1},
			{-1,-1,-1,-1,-1,-1},
			{-1,-1,-1,-1,-1,-1},
			{-1,-1,-1,-1,-1,-1}
			};

char path[36] = "";	

// Time needed for 1 actual movement (turn,forward, etc.)
//    int i, j;
//    for (j = 0; j < time; j++)
//        for (i = 0; i < 20000; i++)
//            ; // short delay
int time = 400;


// executes the path computed in shortestPath

void driveSP(){
	int i, n = strlen(path);
    int t = 280;
	for(i = n-1 ; i >= 0 ; i--){
		char com = path[i];
		if(com == 'f')
			maze_forward(t);
		else if(com == 'r')
			maze_turn_right(t);
		else if(com == 'l')
			maze_turn_left(t);
		else if(com == 'b')
			maze_turn_180(t);
	}
    stop();
}

// computes the path from (0,0) to (X,Y) 
// distance is the shortest distance between those 2 nodes (DISTANCE(last[X][Y]))
// actually computes the way back from (X,Y) to (0,0) and is reversed by driveSP()

void shortestPath(int X, int Y, int distance){
	//path = (char*) malloc(distance*sizeof(char));
	
	char direction, next;
	int i;
	int x = X, y = Y;
	int last_x = LAST_X(last[x][y]), last_y = LAST_Y(last[x][y]);
	
	// direction of arrival at the goal
	if(x-last_x > 0){
		direction = 'E';
	} else if (x-last_x < 0){
		direction = 'W';
	} else if(y-last_y > 0){
		direction = 'N';
	} else if(y-last_y < 0){
		direction = 'S';
	}
	
	// compare last and current to compute the command to go from last to current
	// adjust direction for each step!!!
	
	for(i = distance; i > 1 ; i--){
	
		x = last_x;
		y = last_y;
		last_x = LAST_X(last[x][y]);
		last_y = LAST_Y(last[x][y]);
		
		if(x-last_x > 0){
			if(direction == 'N')
				next = 'l';
			else if(direction == 'E')
				next = 'f';
			else if(direction == 'S')
				next = 'r';
			else if(direction == 'W')
				next = 'b';
			direction = 'E';
		} else if (x-last_x < 0){
			if(direction == 'N')
				next = 'r';
			else if(direction == 'E')
				next = 'b';
			else if(direction == 'S')
				next = 'l';
			else if(direction == 'W')
				next = 'f';
			direction = 'W';
		} else if(y-last_y > 0){
			if(direction == 'N')
				next = 'f';
			else if(direction == 'E')
				next = 'r';
			else if(direction == 'S')
				next = 'b';
			else if(direction == 'W')
				next = 'l';
			direction = 'N';
		} else if(y-last_y < 0){
			if(direction == 'N')
				next = 'b';
			else if(direction == 'E')
				next = 'l';
			else if(direction == 'S')
				next = 'f';
			else if(direction == 'W')
				next = 'r';
			direction = 'S';
		}
		path[distance-i] = next;
	}
	
	// compare (0,0) and current to compute the command to go from (0,0) to current
	// direction has to be 'N' as we start in north direction
	if(x-last_x > 0){
		if(direction == 'N')
			path[distance-1] = 'f';
		else if(direction == 'E')
			path[distance-1] = 'r';
		else if(direction == 'S')
			path[distance-1] = 'b';
		else if(direction == 'W')
			path[distance-1] = 'l';
	} else if (x-last_x < 0){
		if(direction == 'N')
			path[distance-1] = 'f';
		else if(direction == 'E')
			path[distance-1] = 'r';
		else if(direction == 'S')
			path[distance-1] = 'b';
		else if(direction == 'W')
			path[distance-1] = 'l';
	} else if(y-last_y > 0){
		if(direction == 'N')
			path[distance-1] = 'f';
		else if(direction == 'E')
			path[distance-1] = 'r';
		else if(direction == 'S')
			path[distance-1] = 'b';
		else if(direction == 'W')
			path[distance-1] = 'l';
	} else if(y-last_y < 0){
		if(direction == 'N')
			path[distance-1] = 'b';
		else if(direction == 'E')
			path[distance-1] = 'l';
		else if(direction == 'S')
			path[distance-1] = 'f';
		else if(direction == 'W')
			path[distance-1] = 'r';
	}
}
			
// explores the maze starting at (0,0) 
// after exploration the mouse is at (0,0) again
// storing distance from start to cell for each cell 
// distances can be accessed by DEFINEd command
// 	
//	as example:
//
//	int i, j;	
//	for(i = 0 ; i < 6 ; i++){
//		for(j = 0 ; j < 6 ; j++){
//			printf("%2d ", DISTANCE(node[i][j]));
//		}
//		printf("\n");
//	} 

void explore(){
	
	//initial setup
	int current_X = 0;
	int current_Y = 0;
	char direction = 'N';
	
	int run = 1;
	while(run){
        
        sendNameValue("X", current_X);
        sendNameValue("Y", current_Y);
        sendNameValue("D", direction);
		
		//update Walls from sensor data
		setWalls(current_X, current_Y, direction, getRight(), getLeft(), getFront());
		
		SET_VISITED(node[current_X][current_Y]);
		
		//check wether there are unvisited neighbors or not 
		//either go to unvisited cell or go back to last visited one
		
		if(neighborNorthUnvisited(current_X,current_Y)){
			current_Y++;
			SET_LAST_X(last[current_X][current_Y], current_X);
			SET_LAST_Y(last[current_X][current_Y], (current_Y-1));
			SET_DISTANCE(node[current_X][current_Y], (DISTANCE(node[current_X][current_Y-1])+1));
			
            if(direction == 'N'){
                maze_forward(time);
            } else if(direction == 'S'){
                maze_turn_180(time);
            } else if(direction == 'E'){
                maze_turn_left(time);
            } else if(direction == 'W'){
                maze_turn_right(time);
            }
			direction = 'N';
		} else if(neighborEastUnvisited(current_X,current_Y)){
			current_X++;
			SET_LAST_X(last[current_X][current_Y], (current_X-1));
			SET_LAST_Y(last[current_X][current_Y], current_Y);
			SET_DISTANCE(node[current_X][current_Y], (DISTANCE(node[current_X-1][current_Y])+1));
			
			if(direction == 'N'){
                maze_turn_right(time);
            } else if(direction == 'S'){
                maze_turn_left(time);
            } else if(direction == 'E'){
                maze_forward(time);
            } else if(direction == 'W'){
                maze_turn_180(time);
            }
            direction = 'E';		
		} else if(neighborSouthUnvisited(current_X,current_Y)){
			current_Y--;
			SET_LAST_X(last[current_X][current_Y], current_X);
			SET_LAST_Y(last[current_X][current_Y], (current_Y+1));
			SET_DISTANCE(node[current_X][current_Y], (DISTANCE(node[current_X][current_Y+1])+1));
			
            if(direction == 'N'){
                maze_turn_180(time);
            } else if(direction == 'S'){
                maze_forward(time);
            } else if(direction == 'E'){
                maze_turn_right(time);
            } else if(direction == 'W'){
                maze_turn_left(time);
            }
			direction = 'S';
		} else if(neighborWestUnvisited(current_X,current_Y)){
			current_X--;
			SET_LAST_X(last[current_X][current_Y], (current_X+1));
			SET_LAST_Y(last[current_X][current_Y], current_Y);
			SET_DISTANCE(node[current_X][current_Y], (DISTANCE(node[current_X+1][current_Y])+1));
			
			if(direction == 'N'){
                maze_turn_left(time);
            } else if(direction == 'S'){
                maze_turn_right(time);
            } else if(direction == 'E'){
                maze_turn_180(time);
            } else if(direction == 'W'){
                maze_forward(time);
            }
			direction = 'W';
		} else if(current_X == 0 && current_Y == 0)
			run = 0;
		else {
			int new_X = LAST_X(last[current_X][current_Y]);
			int new_Y = LAST_Y(last[current_X][current_Y]);
			if (new_X < 6 && new_Y < 6){
                direction = driveToLast(current_X, current_Y, new_X, new_Y, direction, time);
				current_X = new_X;
				current_Y = new_Y;
			} else
				run = 0;
		}
	}
	//again update all distances
	int i, j, k;
	for(k = 0 ; k < 6 ; k++)
		for(i = 0 ; i < 6 ; i++){
			for(j = 0 ; j < 6 ; j++){
				updateDistanceNorth(i, j);
				updateDistanceEast(i, j);
				updateDistanceSouth(i, j);
				updateDistanceWest(i, j);
			}
		}
    if (direction == 'S')
        turn_180();
    else 
        turn_right();
    wait(time);
    flag = 0;
    shortestPath(2, 1, DISTANCE(node[2][1]));
    driveSP();
    while(1){
        turn_180();
        wait(300);
        flag = 0;
    }
}

// drives the robot to the last recent cell and returns the new direction

char driveToLast(int current_X, int current_Y, int new_X, int new_Y, char direction, int time){
    char newDirection;
    if(new_X > current_X){
        if(direction == 'N'){
            maze_turn_right(time);
        } else if(direction == 'S'){
            maze_turn_left(time);
        } else if(direction == 'E'){
            maze_forward(time);
        } else if(direction == 'W'){
            maze_turn_180(time);
        }
        newDirection = 'E';
    } else if(new_X < current_X){
        if(direction == 'N'){
            maze_turn_left(time);
        } else if(direction == 'S'){
            maze_turn_right(time);
        } else if(direction == 'E'){
            maze_turn_180(time);
        } else if(direction == 'W'){
            maze_forward(time);
        }
        newDirection = 'W';
    } else if(new_Y > current_Y){
        if(direction == 'N'){
            maze_forward(time);
        } else if(direction == 'S'){
            maze_turn_180(time);
        } else if(direction == 'E'){
            maze_turn_left(time);
        } else if(direction == 'W'){
            maze_turn_right(time);
        }
        newDirection = 'N';
    } else if(new_Y < current_Y){
        if(direction == 'N'){
            maze_turn_180(time);
        } else if(direction == 'S'){
            maze_forward(time);
        } else if(direction == 'E'){
            maze_turn_right(time);
        } else if(direction == 'W'){
            maze_turn_left(time);
        }
        newDirection = 'S';        
    }
    return newDirection;
}

//turns the robot 180 degrees and goes one cell forward
void maze_turn_180(int lim){
    LED_Left = ON;
    LED_Right = ON;
    turn_180();
    wait(lim);
    flag = 0;
    go_one_cell();
    LED_Left = OFF;
    LED_Right = OFF;
    wait(lim);
    flag = 0;

}

//turns the robot right degrees and goes one cell forward

void maze_turn_right(int lim){
    LED_Right = ON;
    turn_right();
    wait(lim);
    flag = 0;
    go_one_cell();
    LED_Right = OFF;
    wait(lim);
    flag = 0;
    
}

//turns the robot left degrees and goes one cell forward

void maze_turn_left(int lim){
    LED_Left = ON;
    turn_left();
    wait(lim);
    flag = 0;
    go_one_cell();
    LED_Left = OFF;
    wait(lim);
    flag = 0;
    
}

//robot goes one cell forward

void maze_forward(int lim){
    go_one_cell();
    wait(lim);
    flag = 0;
}

void updateDistanceNorth(int X, int Y){
	if(!NORTH_Wall(node[X][Y])){
		int dist = DISTANCE(node[X][Y]);
		int minimum = min(dist, (DISTANCE(node[X][Y+1])+1));
		if(minimum < dist){
			SET_DISTANCE(node[X][Y], minimum);
			SET_LAST_X(last[X][Y], X);
			SET_LAST_Y(last[X][Y], (Y+1));
		}
	}
}

void updateDistanceEast(int X, int Y){
	if(!EAST_Wall(node[X][Y])){
		int dist = DISTANCE(node[X][Y]);
		int minimum = min(dist, (DISTANCE(node[X+1][Y])+1));
		if(minimum < dist){
			SET_DISTANCE(node[X][Y], minimum);
			SET_LAST_X(last[X][Y], (X+1));
			SET_LAST_Y(last[X][Y], Y);
		}
	}
}

void updateDistanceSouth(int X, int Y){
	if(!SOUTH_Wall(node[X][Y])){
		int dist = DISTANCE(node[X][Y]);
		int minimum = min(dist, (DISTANCE(node[X][Y-1])+1));
		if(minimum < dist){
			SET_DISTANCE(node[X][Y], minimum);
			SET_LAST_X(last[X][Y], X);
			SET_LAST_Y(last[X][Y], (Y-1));
		}
	}
}

void updateDistanceWest(int X, int Y){
	if(!WEST_Wall(node[X][Y])){
		int dist = DISTANCE(node[X][Y]);
		int minimum = min(dist, (DISTANCE(node[X-1][Y])+1));
		if(minimum < dist){
			SET_DISTANCE(node[X][Y], minimum);
			SET_LAST_X(last[X][Y], (X-1));
			SET_LAST_Y(last[X][Y], Y);
		}
	}
}

//checks if there is an unvisited neighbor in the north

int neighborNorthUnvisited(int X, int Y){
	int check = 0;
	if(!NORTH_Wall(node[X][Y]))
		if(!VISITED(node[X][Y+1]))
			check = 1;
	return check;
}

//checks if there is an unvisited neighbor in the east

int neighborEastUnvisited(int X, int Y){
	int check = 0;
	if(!EAST_Wall(node[X][Y]))
		if(!VISITED(node[X+1][Y]))
			check = 1;
	return check;
}

//checks if there is an unvisited neighbor in the south

int neighborSouthUnvisited(int X, int Y){
	int check = 0;
	if(!SOUTH_Wall(node[X][Y]))
		if(!VISITED(node[X][Y-1]))
			check = 1;
	return check;
}

//checks if there is an unvisited neighbor in the west
//else updates distance

int neighborWestUnvisited(int X, int Y){
	int check = 0;
	if(!WEST_Wall(node[X][Y]))
		if(!VISITED(node[X-1][Y]))
			check = 1;
	return check;
}

//sets wall at positon X, Y regarding the current direction we're looking at
//right, left, front have to be pulled from sensors.c
//all walls except for borders have to be set for both adjacent cells

void setWalls(int X, int Y, char dir, int right, int left, int front){
	if(dir == 'N'){
		if(right){
			SET_EAST_Wall(node[X][Y]);
			if(X < 5)
				SET_WEST_Wall(node[X+1][Y]);
		}
		if(left){
			SET_WEST_Wall(node[X][Y]);
			if(X > 0)
				SET_EAST_Wall(node[X-1][Y]);
		}
		if(front){
			SET_NORTH_Wall(node[X][Y]);
			if(Y < 5)
				SET_SOUTH_Wall(node[X][Y+1]);
		}
	} else if(dir == 'E'){
		if(right){
			SET_SOUTH_Wall(node[X][Y]);
			if(Y > 0)
				SET_NORTH_Wall(node[X][Y-1]);
		}
		if(left){
			SET_NORTH_Wall(node[X][Y]);
			if(Y < 5)
				SET_SOUTH_Wall(node[X][Y+1]);
		}
		if(front){
			SET_EAST_Wall(node[X][Y]);
			if(X < 5)
				SET_WEST_Wall(node[X+1][Y]);
		}
	} else if(dir == 'S'){
		if(right){
			SET_WEST_Wall(node[X][Y]);
			if(X > 0)
				SET_EAST_Wall(node[X-1][Y]);
		}
		if(left){
			SET_EAST_Wall(node[X][Y]);
			if(X < 5)
				SET_WEST_Wall(node[X+1][Y]);
		}
		if(front){
			SET_SOUTH_Wall(node[X][Y]);
			if(Y > 0)
				SET_NORTH_Wall(node[X][Y-1]);
		}
	} else if(dir == 'W'){
		if(right){
			SET_NORTH_Wall(node[X][Y]);
			if(Y < 5)
				SET_SOUTH_Wall(node[X][Y+1]);
		}
		if(left){
			SET_SOUTH_Wall(node[X][Y]);
			if(Y > 0)
				SET_NORTH_Wall(node[X][Y-1]);
		}
		if(front){
			SET_WEST_Wall(node[X][Y]);
			if(X > 0)
				SET_EAST_Wall(node[X-1][Y]);
		}
	}  
}

int getRight(){
	return rightWall();
}

int getLeft(){
	return leftWall();
}

int getFront(){
	return frontWall();
}

//JUST for test purpose!!!!!!!!!

void setBorder(){
	int i;
	for(i = 0 ; i < 6 ; i++)
		setWalls(i, 5, 'N', 0, 0, 1);	
	
	for(i = 0 ; i < 6 ; i++)
		setWalls(i, 0, 'S', 0, 0, 1);
	
	for(i = 0 ; i < 6 ; i++)
		setWalls(5, i, 'E', 0, 0, 1);
	
	for(i = 0 ; i < 6 ; i++)
		setWalls(0, i, 'W', 0, 0, 1);
	
	for(i = 0 ; i < 4 ; i++)
		setWalls(i, 4, 'N', 0, 0, 1);
	
	for(i = 3 ; i < 5 ; i++)
		setWalls(i, 2, 'N', 0, 0, 1);
	
	for(i = 1 ; i < 5 ; i++)
		setWalls(i, 0, 'N', 0, 0, 1);
	
	for(i = 1 ; i < 2 ; i++)
		setWalls(4, i, 'E', 0, 0, 1);
	
	for(i = 1 ; i < 2 ; i++)
		setWalls(3, i, 'E', 0, 0, 1);
	
	for(i = 2 ; i < 4 ; i++)
		setWalls(1, i, 'E', 0, 0, 1);
	
	for(i = 1 ; i < 5 ; i++)
		setWalls(0, i, 'E', 0, 0, 1);
	
	//printmaze();
}

//tries to print the maze
//ugly yet

void printmaze(){
	int i, j;
	for(i = 0 ; i < 6 ; i++){
		for(j = 0 ; j < 6 ; j++){
			if(SOUTH_Wall(node[i][j]))
				printf("[  ");
			else
				printf("   ");
			if(WEST_Wall(node[i][j]))
				printf(" - ");
			else
				printf("   ");
			if(EAST_Wall(node[i][j]))
				printf("_  ");
			else
				printf("   ");
			if(NORTH_Wall(node[i][j]))
				printf("]  ");
			else
				printf("   ");
		}
		printf("\n");
		printf("\n");
	}
}

void printLast(){
    int i, j;
	for(i = 0 ; i < 6 ; i++){
		for(j = 0 ; j < 6 ; j++){
            printf("X:%3d Y:%3d  ", LAST_X(last[i][j]), LAST_Y(last[i][j]));
        }
        printf("\n");
    }
    printf("end\n");
}

//just an easy minimum function

int min(int a, int b){
	return a < b ? a : b;
}

void wait(int lim){
    int i, j;
    for (j = 0; j < lim; j++)
        for (i = 0; i < 20000; i++)
            ; // short delay
}