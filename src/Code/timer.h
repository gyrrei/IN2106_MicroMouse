/* 
 * File:   timer.h
 * Author: a2-lenz
 *
 * Created on April 5, 2019, 11:34 AM
 */

#ifndef TIMER_H
#define TIMER_H
#include "xc.h"
#define ON 1
#define OFF 0
void initTimer1(unsigned int period);
void startTimer1(void);
void stopTimer1(void);
void initTimer2(unsigned int period);
void startTimer2(void);
#endif /* TIMER_H */
