#ifndef UART_H
#define UART_H
#include "xc.h"
#include "qei.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FP 40000000 //40MHZ
#define BAUDRATE 57600
#define BRGVAL ((FP / BAUDRATE) / 16) - 1
//unsigned int i;
#define DELAY_105uS                \
    asm volatile("REPEAT, #4201"); \
    Nop(); // 105uS delay

void initUART();
void send(char msg[]);
void sendNameValue(char name[], int value);
#endif /* GPIO_H */