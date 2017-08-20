/**
* Robotics
*
* Globals.h
*
* Students:
* Shir Zerahia - 308417823
* Michal Teverovsky - 313522039
* Yonatan Berg - 311123327
*
* 22.8.17
*/

#ifndef GLOBALS_H_
#define GLOBALS_H_

#define ROBOT_SIZE_IN_CM 20

#define X_START 470
#define Y_START 470
#define YAW_START 0

#define X_GOAL 370
#define Y_GOAL 630

#define numericCharToInt(numChar) (numChar - '0')



// map of directions
const int dirNum = 8; // number of possible directions to go at any position
static int dirX[dirNum] = {1, 1, 0, -1, -1, -1,  0,  1};
static int dirY[dirNum] = {0, 1, 1,  1,  0, -1, -1, -1};

#endif /* GLOBALS_H_ */
