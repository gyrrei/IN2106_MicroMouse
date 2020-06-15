#ifndef CONTROL_H
#define	CONTROL_H
#include "xc.h"

typedef struct
{
    int kp;
    int ki;
    int kd;
    
    int error;
    int integral;
    int derivative;
    int last_error;
    int value;
    
    int top_lim;
    int bot_lim;
    
    int int_lim;
    
    long int target;
} PID_Controller;

//extern PID_Controller controller_array[4];
extern PID_Controller pos_control_left;
extern PID_Controller pos_control_right;
extern PID_Controller vel_control_left;
extern PID_Controller vel_control_right;
extern int flag;

//void motor_control(char motor, int current_angular_speed);
void motor_control();
void test_go_straight(int speed);
void stop();
void evaluate_controller(PID_Controller *controller, long int current_control_value);
void go_one_cell();
void turn_right();
void turn_left();
void turn_180();
void approach_wall();
<<<<<<< HEAD
=======
void go_x_cells(int x_cells);
>>>>>>> master

#endif	

