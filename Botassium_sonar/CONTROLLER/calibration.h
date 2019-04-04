/*!
* \author Nicolas Van der Noot
* \file calibration_ex.h
* \brief calibration of the robot to know its accurate initial position
*/

#ifndef _CALIBRATION_H_
#define _CALIBRATION_H_

extern "C"{
  #include "CtrlStruct.h"

}

void calibration(CtrlStruct *structure);
double getX(CtrlStruct *structure);
double meanAngle(double *array, int team);

#endif
