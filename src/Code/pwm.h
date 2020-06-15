#ifndef PWM_H
#define PWM_H
#include "xc.h"
void drive_motor_forward(char motor, int dutycycle);
void drive_motor_backward(char motor, int dutycycle);
void drive_motor(char motor, int dutycycle);
void fast_stop_motor(char motor);
void init_PWM(void);
void test_PWM(void);
#endif
