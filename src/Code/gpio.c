
#include "gpio.h"

void initIO()
{
    int i;

    // set up analogue pins
    AD1CON1bits.ADON = 0; // disable ADC1 module
    AD1PCFGL = 0xFFFF;    //all pins are digital    
    
    TRISBbits.TRISB14 = 1; //BUTTON
    
    //outputs
    TRISBbits.TRISB12 = 0; //LED_Back output 
    TRISAbits.TRISA8 = 0; //LED_Front output
    TRISBbits.TRISB3 = 0; //LED_Left output
    TRISCbits.TRISC0 = 0; //LED_Right output

    // ******* Configure two bits as output bits for the first hbridge
    TRISAbits.TRISA7 = 0;
    TRISAbits.TRISA10 = 0; 
    // ******* Configure two bits as output bits for the second hbridge
    TRISBbits.TRISB11 = 0; 
    TRISBbits.TRISB10 = 0; 
    // ******* H-Bridge Part over

    // set up remappable pins
    __builtin_write_OSCCONL(OSCCON & 0xbf); // clear bit 6 (unlock, they are usually write protected)

    RPINR14bits.QEA1R = 17; 
    RPINR14bits.QEB1R = 18; 

    RPINR16bits.QEA2R = 24; 
    RPINR16bits.QEB2R = 25;
        
    RPINR18bits.U1RXR = 8;
    
    RPOR4bits.RP9R = 3; //output serial
    
    __builtin_write_OSCCONL(OSCCON | 0x40); // Lock PPS registers (lock again!)

    for (i = 0; i < 30000; i++)
        ; // short delay
}
