/*
 * File:   uart.c
 * Author: Natalia
 *
 * Created on June 10, 2019, 10:26 PM
 */

#include "uart.h"

void initUART()
{
    IPC3bits.U1TXIP = 5;

    U1MODEbits.STSEL = 0;   // 1-Stop bit
    U1MODEbits.PDSEL = 0;   // No Parity, 8-Data bits
    U1MODEbits.ABAUD = 0;   // Auto-Baud disabled
    U1MODEbits.BRGH = 0;    // Standard-Speed mode
    U1BRG = BRGVAL;         // Baud Rate setting for BAUDRATE
    U1STAbits.UTXISEL0 = 0; // Interrupt after one TX character is transmitted
    U1STAbits.UTXISEL1 = 0;
    
    U1STAbits.URXISEL = 0; // Interrupt after one RX character is received;
    IFS0bits.U1RXIF = 0;
    IEC0bits.U1RXIE = 1;
    IPC2bits.U1RXIP = 5;
    
    IEC0bits.U1TXIE = 1;   // Enable UART TX interrupt
    U1MODEbits.UARTEN = 1; // Enable UART
    U1STAbits.UTXEN = 1;   // Enable UART TX
    /* Wait at least 105 microseconds (1/9600) before sending first char */
    DELAY_105uS
}

void __attribute__((__interrupt__, no_auto_psv)) _U1TXInterrupt(void)
{
    IFS0bits.U1TXIF = 0; 
}

/**
 * 
 * @param msg sends this message up to length 4
 */
void send(char msg[]){
    int i = 0, n = strlen(msg);
    while(i < n){
        if(!U1STAbits.UTXBF){
            U1TXREG = msg[i];
            i++;
        }
    }
    int send = 1;
    while(send){
        if(!U1STAbits.UTXBF){
            U1TXREG = '\n';
            send = 0;
        }
    }
}

void sendNameValue(char name[], int value){
    int i = 0, n = strlen(name);
    while(i < n){
        if(!U1STAbits.UTXBF){
            U1TXREG = name[i];
            i++;
        }
    }
    while(U1STAbits.UTXBF)
        ;
    U1TXREG = ' ';
    char buffer[5];
    sprintf(buffer, "%d", value);
    i = 0, n = strlen(buffer);
    while(i < n){
        if(!U1STAbits.UTXBF){
            U1TXREG = buffer[i];
            i++;
        }
    }
    while(U1STAbits.UTXBF)
        ;
    U1TXREG = '\n';
}


//=================================
/* UART1 Receive Interrupt Service Routine */

void __attribute__((__interrupt__,auto_psv)) _U1RXInterrupt(void)
{
    IFS0bits.U1RXIF = 0; 
} 

