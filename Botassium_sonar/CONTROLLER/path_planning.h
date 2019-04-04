/*!
* \author Nicolas Van der Noot
* \file path_planning_ex.h
* \brief path-planning algorithm
*/

#ifndef _PATH_PLANNING_H_
#define _PATH_PLANNING_H_

#include "CtrlStruct.h"



/// path-planning main structure
struct PathPlanning
{
	int dummy_var; ///< dummy variable
};

// function prototype
void path_planning_update(CtrlStruct* structure, double Xgoal, double Ygoal);
bool IsObstacle(CtrlStruct *cvs,double x, double y);
int goTo(double x_ref, double y_ref, CtrlStruct* cvs);

#endif
