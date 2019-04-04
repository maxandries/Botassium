#include "odometry.h"
#include <math.h>
#include <time.h>
#define BILLION 1000000000L

extern "C"{
	#include "CtrlStruct.h"
	#include "stdio.h"
}
/*! \brief update the robot odometry  
*
* \param[in,out] cvs controller main structure
*/
void update_odometry(CtrlStruct* structure)
{
	// variables declaration
	double r_wheel_speed = structure->theUserStruct->speed_odo_right;
	double l_wheel_speed = structure->theUserStruct->speed_odo_left;
	double r_odo = 0.022;
	double D_odo = 0.2827;
	struct timespec t;
    clock_gettime(CLOCK_MONOTONIC_RAW, &t);
	double time = (double)(t.tv_sec) + (double)t.tv_nsec/BILLION ;
	double Delta_t = (time - structure->theUserStruct->t_odo);
	structure->theUserStruct->t_odo = time;
	
	double ThetaPoint = r_odo*(r_wheel_speed - l_wheel_speed)/D_odo;
	double Vxr = 0.5*r_odo*(r_wheel_speed + l_wheel_speed);
	//printf("delta T %f\n", Delta_t);
	double VX = Vxr * cos(structure->theUserStruct->Rob_theta)*1000;
	double VY = Vxr * sin(structure->theUserStruct->Rob_theta)*1000;
	
	// new odometry position
	structure->theUserStruct->Rob_x = (structure->theUserStruct->Rob_x + VX * Delta_t);
	structure->theUserStruct->Rob_y = (structure->theUserStruct->Rob_y + VY * Delta_t);
	double NewTheta = structure->theUserStruct->Rob_theta + ThetaPoint * Delta_t;
	//printf("X %f Y %f theta %f\n", structure->theUserStruct->Rob_x, structure->theUserStruct->Rob_y, structure->theUserStruct->Rob_theta);
	if (NewTheta > 2 * M_PI) {
		NewTheta = NewTheta - 2* M_PI;
	}
	if (NewTheta < 0.0) {
		NewTheta = NewTheta + 2 * M_PI;
	}
	structure->theUserStruct->Rob_theta = NewTheta;
	//printf("tetha %f\n", NewTheta*180/3.1427);
}
