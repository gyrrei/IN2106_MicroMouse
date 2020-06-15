/* 
 * File:   sensor.h
 * Author: schul
 *
 * Created on 25. Juni 2019, 09:47
 */

#ifndef SENSOR_H
#define	SENSOR_H

#ifdef	__cplusplus
#endif

int distance(char sensor);
int rightWall();
int leftWall();
int frontWall();
int middle();
#ifdef	__cplusplus
}
#endif

#endif

