/*!
* \file ctrl_main_gr2.cc
* \brief Initialization, loop and finilization of the controller written in C (but compiled as C++)
*/

#include "./strategy.h"
#include "./CONTROLLER/path_planning.h"
#include "ctrl_main.h"
#include "./CONTROLLER/odometry.h"
#include "time.h"
#include "./CONTROLLER/sonar.h"
#include <stdlib.h>

extern "C"{
	#include "stdio.h"
	#include "./CONTROLLER/speed_controller.h"
	#include "./CONTROLLER/CtrlStruct.h"
}


#define PI 3.141592654




/*! \brief initialize controller operations (called once)
*
* \param[in] cvs controller main structure
*/
void controller_init(CtrlStruct *cvs)
{

	//set_init_position(cvs->theUserStruct->side, cvs);

}


/*! \brief controller loop (called every timestep)
*
* \param[in] cvs controller main structure
*/
void controller_loop(CtrlStruct *cvs)
{
	int side = cvs->theUserStruct->side;
	// variables declaration
	double t;
	CtrlIn  *inputs;
	CtrlOut *outputs;

	// variables initialization
	inputs = cvs->theCtrlIn;
	outputs = cvs->theCtrlOut;

	//t = clock()/CLOCKS_PER_SEC - cvs->theUserStruct->t_init;
	double time = 0;
	// update the robot odometry
	update_odometry(cvs);
	sonar(cvs);
	int xref, yref, test;
	// finite main_state machine (FSM)
		switch (cvs->main_state)
		{
		case CALIB_STATE:
			calibration(cvs);
			break;
		case WAIT_START: //direction vers le palet du milieu
					cvs->main_state = STRATEGY;
			break;
		case STRATEGY: //marche arriere jusque palet balance
			//printf("flag %d\n", cvs->theUserStruct->flag_opp);
			if(cvs->theUserStruct->flag_opp){
				cvs->main_state = END;
			}
			main_strategy(cvs);
			break;
		case END: //calibration
			//printf("STOP\n");
			run_speed_controller(cvs, 0.0, 0.0);
			if(cvs->theUserStruct->flag_opp==0){
				cvs->main_state = STRATEGY;
			}
			break;
	}
}




/*! \brief last controller operations (called once)
*
* \param[in] cvs controller main structure
*/
void controller_finish(CtrlStruct *cvs){
	run_speed_controller(cvs, 0, 0);
}
