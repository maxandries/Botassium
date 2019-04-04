#include "strategy.h"

#include <math.h>
#include "strategy.h"
#include "./CONTROLLER/path_planning.h"

#include "./CONTROLLER/odometry.h"
#include "./CONTROLLER/calibration.h"

extern "C"{
	#include "unistd.h"
	#include "./CONTROLLER/CtrlStruct.h"
	#include "./CONTROLLER/speed_controller.h"
	#include "./CONTROLLER/rotate.h"
	#include "stdio.h"
}
/*! \brief startegy during the game
*
* \param[in,out] cvs controller main cvs
*/
void main_strategy(CtrlStruct *cvs)
{
	// variables declaration
	Strategy *strat;
	CtrlIn  *inputs;
	CtrlOut *outputs;

	// variables initialization
	inputs = cvs->theCtrlIn;
	outputs = cvs->theCtrlOut;

	// variables initialization
	strat = cvs->strat;
	int xref, yref, test;

	int side = cvs->theUserStruct->side;


	//printf(" position en x = %f, position en Y = %f , position en theta = %f \n ", cvs->theUserStruct->Rob_x, cvs->theUserStruct->Rob_y, cvs->theUserStruct->Rob_theta * 180 / 3.1427);
	switch (cvs->strat->state)
	{
	case main_state1: //direction vers le palet du milieu

		xref = 0;
		yref = 0;
		test = goTo(xref, yref, cvs);
		if (test) {
			printf("ok\n");
			run_speed_controller(cvs, 0, 0);
			test = 0;
			cvs->strat->state = main_state2;
		}
		else {
			run_speed_controller(cvs, cvs->theUserStruct->omegaR, cvs->theUserStruct->omegaL);
		}
		break;
	case main_state2:
		xref = 0;
		yref = 1300;
		test = goTo(xref, yref, cvs);
		if (test){
			run_speed_controller(cvs, 0, 0);
			cvs->strat->state = STOP;
		}
		else{
			run_speed_controller(cvs, cvs->theUserStruct->omegaR, cvs->theUserStruct->omegaL);
		}	
	break;
	case main_state4: // dynamixel
	run_speed_controller(cvs, 0, 0);
	// dynamixel
	break;
	case STOP:
		run_speed_controller(cvs, 0, 0);
		if (cvs->theCtrlIn->t > 10) {
			cvs->strat->state = STOP;
		}
		else if (cvs->theCtrlIn->t > 0) {
			cvs->strat->state = STOP;

		}

		break;
	}


}
