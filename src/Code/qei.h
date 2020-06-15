/* 
 * File:   qei.h
 * Author: Gyri
 *
 * Created on June 11, 2019, 1:22 PM
 */

#ifndef QEI_H
#define QEI_H
#include "xc.h"

//Motor no. 1 is the LEFT motor
extern long int longpos1; // Declare long position count overflow variable
extern int current_speed1; // Declare variable in which the current speed is stored
extern long int old_count1;
extern long int new_count1;

//Motor no. 2 is the RIGHT motor
extern long int longpos2; // Declare long position count overflow variable
extern int current_speed2; // Declare variable in which the current speed is stored
extern long int old_count2;
extern long int new_count2; 

extern long int current_pos1;
extern long int current_pos2;
extern long int distance1;
extern long int distance2;

#define MAX_COUNT_PER_WHEEL_REV 2112 //16*4*33 
//PULSES_PER_REV x COUNT_INC_PER_REV x MOTOR_REV_PER_WHEEL_REV = 16 x 4 x 33
#define WHEEL_CIRCONFERENCE 18900 //60mm * pi in 10micrometer
#define DISTANCE_PER_CNT 9

//Left motor
#define GET_ENCODER_VALUE_1(POSITION_1) (POSITION_1 = longpos1 + POS1CNT)
//Right motor
#define GET_ENCODER_VALUE_2(POSITION_2) (POSITION_2 = longpos2 + POS2CNT)

void init_QEI();
void calculate_speed(char motor);
void calculate_position(char motor);
long int calculate_distance();
#endif 
