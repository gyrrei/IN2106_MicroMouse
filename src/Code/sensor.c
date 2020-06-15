#include "dma.h"
#include "uart.h"
#include "stdio.h"
#include <stdlib.h>



/** 
 *  returns approximated distance in mm for FRONT sensor
 *  distance 10-60mm in 5mm steps!!! Lower than 10mm gives weird values and collision gives -1
 *  returns -1 for to far
*/
int distance(char sensor){
    int ret, val;
    if (sensor == 'f')
        val = adcData[1];
    else if(sensor == 'r')
        val = adcData[0];
    else
        val = adcData[2];
    
            if (val > 3000) {
                ret = 10;
            } else {
                if (val > 2700) {
                    ret = 15;
                } else {
                    if (val > 2300) {
                        ret = 20;
                    } else {
                        if (val > 2000) {
                            ret = 25;
                        } else {
                            if (val > 1800) {
                                ret = 30;
                            } else {
                                if (val > 1600) {
                                    ret = 35;
                                } else {
                                    if (val > 1400) {
                                        ret = 40;
                                    } else {
                                        if (val > 1300) {
                                            return 45;
                                        } else {
                                            if (val > 1200) {
                                                ret = 50;
                                            } else {
                                                if (val > 1100) {
                                                    ret = 55;
                                                } else {
                                                    if (val > 1000) {
                                                        ret = 60;
                                                    } 
                                                    else {
                                                        ret = -1;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
    return ret;
}

/** 
 *  returns 0 for no wall 1 for wall on the right side
*/
int rightWall(){
    if (adcData[0] < 1000 || adcData[0] > 3300)
        return 0;
    return 1;
}

/** 
 *  returns 0 for no wall 1 for wall on the left side
*/
int leftWall(){
    if (adcData[2] < 1000 || adcData[2] > 3300)
        return 0;
    return 1;
}

/** 
 *  returns 0 for no wall 1 for wall on the left side
*/
int frontWall(){
    if (adcData[1] < 1000 || adcData[1] > 3300)
        return 0;
    return 1;
}
/** 
 *  returns 0 for yes, -1 for too much on the left, 1 for too much on the right
*/
int middle(){
    int difference = adcData[0] - adcData[2];
    if (difference < 200 && difference > -200)
        return 0;
    if (difference <= -200)
        return -1;
    return 1;
}


