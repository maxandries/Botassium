/*!
* \author Nicolas Van der Noot
* \file calibration_ex.h
* \brief calibration of the robot to know its accurate initial position
*/

#ifndef _SONAR_H_
#define _SONAR_H_

extern "C"{
  #include "CtrlStruct.h"

}

void sonar(CtrlStruct *structure);
int IsNotWall(double x, double y);
//void update(CtrlStruct *structure,int sonar,double ditance);
#endif
