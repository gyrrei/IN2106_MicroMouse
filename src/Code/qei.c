/* 
 * File:   qei.c
 * Author: Gyri
 *
 * Created on June 11, 2019, 1:22 PM
 */

#include "qei.h"
#include "xc.h"
#include "gpio.h"
#include "uart.h"
#include "stdio.h"

//Motor no. 1 is the LEFT motor
long int longpos1 = 0; // Initialize long position count overflow variable
int current_speed1 = 0; // Initialize variable in which the current speed is stored
long int old_count1 = 0;
long int new_count1 = 0;

//Motor no. 2 is the RIGHT motor
long int longpos2 = 0; // Initialize long position count overflow variable
int current_speed2 = 0; // Initialize variable in which the current speed is stored
long int old_count2 = 0;
long int new_count2 = 0;

long int current_pos1 = 0;
long int current_pos2 = 0;

//distance in 10 micrometer 
// 1cm is 1000 of this (gyrimeters)
long int distance1 = 0;
long int distance2 = 0;

void init_QEI(void)
{
    // Configure QEI module
    // QEIEN - QEI enable/disable  Count Error Status Flag bit
    QEI1CONbits.CNTERR = 0; //CNTERR
    QEI2CONbits.CNTERR = 0;
    // QEISIDL - Idle mode 0=continue, 1=stop
    QEI1CONbits.QEISIDL = 1; //QEICONbits.QEISIDL // Discontinue operation when in idle mode
    QEI2CONbits.QEISIDL = 1;
    //INDEX: Index Pin State Status bit (read-only)
    //QEI1CONbits.INDEX = 0; // 1 = Index pin is high 0 = Index pin is low
    //UPDN: Position Counter Direction Status bit
    QEI1CONbits.UPDN = 0; // 1 positive/0 negative
    QEI2CONbits.UPDN = 0; // 1 positive/0 negative

    /*
     * QEIM<2:0>: Quadrature Encoder Interface Mode Select bits
     * 111 = Quadrature Encoder Interface enabled (x4 mode) with position counter reset by match (MAXxCNT)
     * 110 = Quadrature Encoder Interface enabled (x4 mode) with index pulse reset of position counter
     * 101 = Quadrature Encoder Interface enabled (x2 mode) with position counter reset by match (MAXxCNT)
     * 100 = Quadrature Encoder Interface enabled (x2 mode) with index pulse reset of position counter
     * 011 = Unused (module disabled)
     * 010 = Unused (module disabled)
     * 001 = Starts 16-bit Timer
     * 000 = Quadrature Encoder Interface/Timer off
     * From example QEIM = 0b111; // x4 edge gain and reset POSCNT when == MAXCNT
     */
    QEI1CONbits.QEIM = 0b111;
    QEI2CONbits.QEIM = 0b111;

    //SWPAB: Phase A and Phase B Input Swap Select bit
    QEI1CONbits.SWPAB = 1; // Do not swap the polarity of phase comparison
    QEI2CONbits.SWPAB = 0;

    //PCDOUT: Position Counter Direction State Output Enable bit
    QEI1CONbits.PCDOUT = 0; // Disable counter direction pin (normal I/O operation)
    QEI2CONbits.PCDOUT = 0;

    // TQGATE: Timer Gated Time Accumulation Enable bit
    QEI1CONbits.TQGATE = 0;
    QEI2CONbits.TQGATE = 0;

    //POSRES: Position Counter Reset Enable bit
    QEI1CONbits.POSRES = 0; // index pulse does not reset POSCNT
    QEI2CONbits.POSRES = 0;

    //TQCS: Timer Clock Source Select bitTQCS: Timer Clock Source Select bit
    QEI1CONbits.TQCS = 0; // internal clock source (Tcy)
    QEI2CONbits.TQCS = 0;

    DFLT1CONbits.QEOUT = 0; // disable digital filters bit 7
    DFLT2CONbits.QEOUT = 0;

    // set initial counter value and maximum range
    MAX1CNT = 0xffff; // set the highest possible time out
    POS1CNT = 0; // set POSCNT into middle of range
    MAX2CNT = 0xffff;
    POS2CNT = 0;

    // Configure Interrupt controller for QEI 1 - first motor
    IFS3bits.QEI1IF = 0;  // clear interrupt flag
    IEC3bits.QEI1IE = 1;  // enable QEI interrupt
    IPC14bits.QEI1IP = 6; // set QEI interrupt priority

    // Configure Interrupt controller for QEI 2 - second motor
    IFS4bits.QEI2IF = 0;  // clear interrupt flag
    IEC4bits.QEI2IE = 1;  // enable QEI interrupt
    IPC18bits.QEI2IP = 6; // set QEI interrupt priority
    
    

    //U1TXREG = 'I'; // Transmit one character
}

void calculate_speed(char motor){
    // calculate current speed. This function is supposed to be called in a
    // regular timer. The speed is given in: counts/timer_period
    // where counts is the number of QEI counts since this function was last
    // called and timer_period is the time between two calls of this function
    // This could easily be extended to give a time as well to calculate RPM or
    // something.
    
    if(motor == 'L'){
        GET_ENCODER_VALUE_1(new_count1);
        current_speed1 = new_count1 - old_count1;
        old_count1 = new_count1;      
    } else if (motor == 'R'){
        GET_ENCODER_VALUE_2(new_count2);
        current_speed2 = new_count2 - old_count2;
        old_count2 = new_count2;
    }
}

void calculate_position(char motor){
    if(motor == 'L'){
        GET_ENCODER_VALUE_1(current_pos1);
        distance1 = current_pos1 * DISTANCE_PER_CNT;
    } else if(motor == 'R'){
        GET_ENCODER_VALUE_2(current_pos2);
        distance2 = current_pos2 * DISTANCE_PER_CNT;
        }
}

// interrupt service routine that resets the position counter for the QEI 1
void __attribute__((interrupt, no_auto_psv)) _QEI1Interrupt(void)
{
    IFS3bits.QEI1IF = 0;  // clear interrupt flag
    //less than half of maxcount 32768
    if (POS1CNT < 0x7fff) {
      U1TXREG = 'o'; // Just for debugging purposes
      // Saving count information in long variable in case of over/underflow
      longpos1 += 0x10000; //overflow condition caused interrupt
    } else {
      U1TXREG = 'u'; // Just for debugging purposes
      // Saving count information in long variable in case of over/underflow
      longpos1-= 0x10000; //underflow condition caused interrupt
    }
        
}

// interrupt service routine that resets the position counter for the QEI 2
void __attribute__((interrupt, no_auto_psv)) _QEI2Interrupt(void)
{
    IFS4bits.QEI2IF = 0;  // clear interrupt flag
    //less than half of maxcount 32768
    if (POS2CNT < 0x7fff) {
      U1TXREG = 'O'; // Just for debugging purposes
      // Saving count information in long variable in case of over/underflow
      longpos2 += 0x10000; //overflow condition caused interrupt
    } else {
      U1TXREG = 'U'; // Just for debugging purposes
      // Saving count information in long variable in case of over/underflow
      longpos2 -= 0x10000; //underflow condition caused interrupt
    }
        
}

// another timer interrupt for accessing the position
