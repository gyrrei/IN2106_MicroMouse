/*
 * File:   motor_control.c
 * Author: Steff
 *
 * Created on 19. Juni 2019, 16:39
 * Changed by Adrian on 5-7-2019
 */


#include "xc.h"
#include "pwm.h"
#include "gpio.h"
#include "qei.h"
#include "control.h"
//#include "uart.h"
#include "sensor.h"
#include "timer.h"

<<<<<<< HEAD
int motor1_wanted_speed = 1;
=======
int motor1_wanted_speed = 1.2;
>>>>>>> master
int motor2_wanted_speed = 1;

int flag = 0;

float correction = 0.7;
float correction2 = 0.2;

PID_Controller pos_control_left = {.kp = 1, .ki = 0, .kd = 0,
                                    .top_lim = 30, .bot_lim = -30};
PID_Controller pos_control_right = {.kp = 1, .ki = 0, .kd = 0,
                                    .top_lim = 30, .bot_lim = -30};
<<<<<<< HEAD
PID_Controller vel_control_left = {.kp =50, .ki = 0, .kd = 1,
                                    .top_lim = 2000, .bot_lim = -2000};
PID_Controller vel_control_right = {.kp = 50, .ki = 0, .kd = 1,
                                    .top_lim = 2000, .bot_lim = -2000};
=======
PID_Controller vel_control_left = {.kp =50, .ki = 5, .kd = 1,
                                    .top_lim = 2000, .bot_lim = -2000,
                                    .int_lim = 100};
PID_Controller vel_control_right = {.kp = 50, .ki = 5, .kd = 1,
                                    .top_lim = 2000, .bot_lim = -2000,
                                    .int_lim = 100};
>>>>>>> master

void test_go_straight(int speed) {
    vel_control_left.target = (long)speed;
    vel_control_right.target = (long)speed;
    evaluate_controller(&vel_control_left, current_speed1);
    evaluate_controller(&vel_control_right, current_speed2);
    //sendNameValue("val", vel_control_left.value);
    drive_motor('L', vel_control_left.value);
    drive_motor('R', vel_control_right.value);
}


/*Here is another try to a more general and generic version of the PID that
 will probably work nicer in the controller cascade*/

void evaluate_controller(PID_Controller *controller, long int current_control_value) {
    
    (*controller).error = (*controller).target - current_control_value ;
    
    (*controller).integral = (*controller).integral + (*controller).error;
    
    if ((*controller).integral > (*controller).int_lim) {
        (*controller).integral = (*controller).int_lim;
    }
    
    (*controller).derivative = (*controller).error - (*controller).last_error;
    
    (*controller).last_error = (*controller).error;
    
    (*controller).value = (int)((*controller).kp * (*controller).error) + 
            ((*controller).ki * (*controller).integral) + 
            ((*controller).kd * (*controller).derivative);
    
    if ((*controller).value > (*controller).top_lim) {
        (*controller).value = (*controller).top_lim;
    } else if ((*controller).value < (*controller).bot_lim) {
        (*controller).value = (*controller).bot_lim;
    }
 
}

void motor_control() {
    
    evaluate_controller(&pos_control_left, (long)(longpos1 + POS1CNT));
    evaluate_controller(&pos_control_right, (long)(longpos2 + POS2CNT));
    
    vel_control_left.target = pos_control_left.value;
    vel_control_right.target = pos_control_right.value;
    
    /*if (flag == 1 && frontWall() && distance('f')<= 20) {
        if (distance('f') != -1) {
            
            fast_stop_motor('l');
            fast_stop_motor('r');
            flag = 0;
        }
    } else */
    
    
    
    if (flag == 1 || flag == 7) {
        int dist_front = distance('f');
        if (frontWall() && dist_front != -1) {
            approach_wall();
            flag = 10;
        }
     else if (flag == 1 && (rightWall() || leftWall())) {
        int dist_left = distance('l');
        if (dist_left == -1) {
            dist_left = 0;
        }
        int dist_right = distance('r');
        if (dist_right == -1) {
            dist_right = 0;
        }
        int dist_diff = dist_left - dist_right;
        if (rightWall() && leftWall()) {
            vel_control_left.target -= (int)(correction*dist_diff);
            vel_control_right.target += (int)(correction*dist_diff);
        } else if (rightWall()) {
<<<<<<< HEAD
            int dist_miss = dist_right - 25;
            //vel_control_left.target += (int)(correction2*dist_miss);
            vel_control_right.target -= (int)(correction2*dist_miss);
        } else {
            int dist_miss = dist_left - 25;
            vel_control_left.target -= (int)(correction2*dist_miss);
            //vel_control_right.target += (int)(correction2*dist_miss);
=======
            int dist_miss = dist_right - 30;
            vel_control_left.target += (int)(correction2*dist_miss);
            vel_control_right.target -= (int)(correction2*dist_miss);
        } else {
            int dist_miss = dist_left - 30;
            vel_control_left.target -= (int)(correction2*dist_miss);
            vel_control_right.target += (int)(correction2*dist_miss);
>>>>>>> master
        }
    }
    }
    
    evaluate_controller(&vel_control_left, (long)current_speed1);
    evaluate_controller(&vel_control_right, (long)current_speed2);
    
    drive_motor('L', vel_control_left.value);
    drive_motor('R', vel_control_right.value);     
}

void go_one_cell() {
    
    /*Given we take the big wheels, the number to set dist roughly calculates as:
    d*112 ; where d is the distance to be travelled in cm
     
    To go more than one cell ahead, you have to set the flag to 0 manually and
    then call go_one_cell() again, otherwise it will only go one cell ahead*/
    
    if (flag != 1 && flag != 10) {
        
        int d = 2000;
    
        GET_ENCODER_VALUE_1(current_pos1);
        GET_ENCODER_VALUE_2(current_pos2);
    
        pos_control_left.target = current_pos1 + d;
        pos_control_right.target = current_pos2 + d;
    
        flag = 1;
    }
}

void turn_right() {
    
    if (flag != 2) {
        
        int t = 924;
    
        GET_ENCODER_VALUE_1(current_pos1);
        GET_ENCODER_VALUE_2(current_pos2);

<<<<<<< HEAD
        pos_control_left.target = current_pos1 + 924;
        pos_control_right.target = current_pos2 - 924;
=======
        pos_control_left.target = current_pos1 + t;
        pos_control_right.target = current_pos2 - t;
>>>>>>> master

        flag = 2;
    }
}

void turn_left() {
    
    if (flag != 3) {
        
        int t = 924;

        GET_ENCODER_VALUE_1(current_pos1);
        GET_ENCODER_VALUE_2(current_pos2);

<<<<<<< HEAD
        pos_control_left.target = current_pos1 - 924;
        pos_control_right.target = current_pos2 + 924;
=======
        pos_control_left.target = current_pos1 - t;
        pos_control_right.target = current_pos2 + t;
>>>>>>> master

        flag = 3;
    }
}

void turn_180() {
    
    if (flag != 4) {
        
        int t = 1848;

        GET_ENCODER_VALUE_1(current_pos1);
        GET_ENCODER_VALUE_2(current_pos2);

        pos_control_left.target = current_pos1 - t;
        pos_control_right.target = current_pos2 + t;

        flag = 4;
    }
}

void stop() {
    
    if (flag != 5) {
        
        GET_ENCODER_VALUE_1(current_pos1);
        GET_ENCODER_VALUE_2(current_pos2);
       
        pos_control_left.target = current_pos1;
        pos_control_right.target = current_pos2;
<<<<<<< HEAD

        flag = 5;
    }
}

void approach_wall() {
    
    static int desired_dist = 15;
    
    if (flag != 6 && frontWall()) {
        
        GET_ENCODER_VALUE_1(current_pos1);
        GET_ENCODER_VALUE_2(current_pos2);
        
        int miss_value = distance('f') - desired_dist;
        
        int correction_value = (int)(miss_value * 11.2);
                
        pos_control_left.target = current_pos1 + correction_value;
        pos_control_right.target = current_pos2 + correction_value;

        flag = 6;
    }    
=======

        flag = 5;
    }
}

void approach_wall() {
    
    static int desired_dist = 10;
    
    if (flag != 6 && frontWall()) {
        
        GET_ENCODER_VALUE_1(current_pos1);
        GET_ENCODER_VALUE_2(current_pos2);
        
        int miss_value = distance('f') - desired_dist;
        
        int correction_value = (int)(miss_value * 11.2);
                
        pos_control_left.target = current_pos1 + correction_value;
        pos_control_right.target = current_pos2 + correction_value;

        flag = 6;
    }    
}

void go_x_cells(int x_cells) {
    
    if (flag != 7 && flag != 10) {
        
        int d = x_cells*2000;
    
        GET_ENCODER_VALUE_1(current_pos1);
        GET_ENCODER_VALUE_2(current_pos2);
    
        pos_control_left.target = current_pos1 + d;
        pos_control_right.target = current_pos2 + d;
    
        flag = 7;
    }
>>>>>>> master
}



