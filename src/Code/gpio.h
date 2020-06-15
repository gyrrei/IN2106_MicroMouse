/* 
 * File:   gpio.h
 * Author: a2-lenz
 *
 * Created on April 5, 2019, 11:31 AM
 */

#ifndef GPIO_H
#define	GPIO_H
#include "xc.h"

#define BUTTON PORTBbits.RB14 //BUTTON

#define LED_Back  LATBbits.LATB12 
#define LED_Front LATAbits.LATA8 
#define LED_Left LATBbits.LATB3 //Left LED
#define LED_Right LATCbits.LATC0 
 
//******* Define two bits as output bits for the first H-Bridge
#define HBRIDGE1 LATAbits.LATA7 
#define HBRIDGE2 LATAbits.LATA10 

//******* Define two bits as output bits for the second H-Bridge
#define HBRIDGE4 LATBbits.LATB11 
#define HBRIDGE3 LATBbits.LATB10


void initIO();
#endif	/* GPIO_H */

