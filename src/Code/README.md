# Code

## Sensors

### ADC

takes the sensor input from AN0, 1 and 4 

### DMA

transfers these values to adcData[0], adcData[1] and adcData[2]

### Sensor

can return if there's a wall detected and how far away the walls are


## Motor

### PWM

Motors driving forward, backward, stop

### QEI

get the encoder signal -> calculating speed and distance

### Control

PID controler to keep the angular speed as it should
Speed of Motor 1 (negativ), Motor 2 (positiv) --> mounted the other way round


## Communication

## UART

can send String messages via bluetooth module
