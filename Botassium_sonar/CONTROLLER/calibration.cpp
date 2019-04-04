#include <cmath>
#include "calibration.h"
extern "C"{
	#include "speed_controller.h"
	#include <unistd.h>
	#include "stdio.h"
}

/**

Algortihm for the calibration of the
robot during the time [-20 ; 0].

Implementation of a FSM using switchs,
odometers and beacons.

*/

/*

Ideas :

1) switches against the wall to align robot axis with map axis
2) beacons to know its position on the starting area

*/

void calibration(CtrlStruct* structure) {

	enum { CALIB_START, CALIB_STATE_1, CALIB_STATE_2, CALIB_STATE_3, CALIB_STATE_4, CALIB_STATE_5, CALIB_STATE_6, CALIB_FINISH }; // replace 'STATE_X' by a more explicit name

		/*double x_ref, y_ref, z_ref, x_init; */ // Necessaire ?
	int signe;

	CtrlIn *inputs; ///< controller inputs
	RobotCalibration *calib; ///< calibration structure

	// variables initialization
	inputs = structure->theCtrlIn;
	calib = structure->calib;
	
	if (structure->theUserStruct->side == 1){ // y en positif
		signe = 1;
	}
	else{
		signe = -1;
	}

	switch (calib->flag) // structure->state is a state stored in the controller main structure
	{

	case CALIB_START: // start calibration
		run_speed_controller(structure, 0, 0); // not moving
		//Sleep(2000);
		calib->flag = CALIB_STATE_1; // directly go to state CALIB_STATE_1
		break;

	case CALIB_STATE_1:
		//printf("im here\n");
		run_speed_controller(structure, -5.0, -5.0); //faire reculez le robot en arriere selon y
		if (structure->theCtrlIn->u_switch[L_ID] == 1 && structure->theCtrlIn->u_switch[R_ID] == 1)
		{
			run_speed_controller(structure, 0.0, 0.0); 	// not moving
			usleep(1000);
			//printf("switch 1 %d  switch 2  %d", structure->inputs->u_switch[L_ID], structure->inputs->u_switch[R_ID]);

			structure->theUserStruct->Rob_y = signe * 1350;  			// position de y mesure = 1.44 ou -1.44 en fonction du cot� de d�part
			structure->theUserStruct->Rob_theta = ((1+signe)/2)*M_PI + M_PI / 2;		// orientation mesure = PI/2 ou -PI/2 en fonction du cot� de d�part
			calib->flag = CALIB_STATE_2;
		}
		else {
			calib->flag = CALIB_STATE_1;
		}

		break;

	case CALIB_STATE_2:

		run_speed_controller(structure, 10, 10);
		if (fabs(structure->theUserStruct->Rob_y )> 1240) {
			calib->flag = CALIB_STATE_2;
		}
		else
		{
			calib->flag = CALIB_STATE_3;
		}

		/*checker la position en x via laser et beacon*/

		//structure->Rob_x = getX(structure);/*position trouvee grace au laser et beacon*/

		//structure->main_state = WAIT_INIT_STATE;  // tell to the main state that the calibration is done

		break;
	case CALIB_STATE_3:
		run_speed_controller(structure, signe*5,signe* -5);
		if (structure->theUserStruct->Rob_theta > 2 * M_PI - 0.1) {
			calib->flag = CALIB_STATE_4;
		}
		else {
			calib->flag = CALIB_STATE_3;
		}
		//printf("Y %f\n", structure->Rob_theta);
		break;
	case CALIB_STATE_4:
		run_speed_controller(structure, -5.0, -5.0); //faire reculez le robot en arriere selon y
		if (structure->theCtrlIn->u_switch[L_ID] == 1 && structure->theCtrlIn->u_switch[R_ID] == 1)
		{
			structure->theUserStruct->Rob_theta = 0;
			run_speed_controller(structure, 0.0, 0.0); 	// not moving
			usleep(1000);
			//printf("switch 1 %d  switch 2  %d", structure->inputs->u_switch[L_ID], structure->inputs->u_switch[R_ID]);

			structure->theUserStruct->Rob_x = -850;  			// position de y mesure = 1.44 ou -1.44 en fonction du cot� de d�part 	// orientation mesure = PI/2 ou -PI/2 en fonction du cot� de d�part
			calib->flag = CALIB_STATE_5;

		}
		else {
			calib->flag = CALIB_STATE_4;
		}

		break;
	case CALIB_STATE_5:
		run_speed_controller(structure, 10, 10);
		if (structure->theUserStruct->Rob_x < -550) {
			calib->flag = CALIB_STATE_5;
		}
		else {
			calib->flag = CALIB_FINISH;
		}
		break;
	case CALIB_STATE_6:
		run_speed_controller(structure, 5, -5);
		if (structure->theUserStruct->Rob_theta < M_PI) {
			calib->flag = CALIB_STATE_6;
		}
		else {
			calib->flag = CALIB_FINISH;
		}
		//printf("Y %f\n", structure->Rob_theta);
		break;
	case CALIB_FINISH: // wait before the match is starting
		structure->main_state = WAIT_START;
		printf("X %f Y %f, theta %f", structure->theUserStruct->Rob_x, structure->theUserStruct->Rob_y, structure->theUserStruct->Rob_theta);
		run_speed_controller(structure, 0.0, 0.0);
		break;

	default:
		//printf("Error: unknown state : %d !\n", calib->flag);
	break;
	}

}

double getX(CtrlStruct *structure) {
	CtrlIn *CtrlIn;
	CtrlIn = structure->theCtrlIn;
	double meanRising = meanAngle(CtrlIn->last_rising, structure->theUserStruct->side);
	double meanFalling = meanAngle(CtrlIn->last_falling, structure->theUserStruct->side);
	double angle = (meanFalling + meanRising) / 2;
	//printf("angle: %f  %f\n", angle, structure->Rob_theta);
	//printf("X: %f", (1000 - (237 / tan(-(M_PI / 2) + angle))));
	return (1000 - (237 / tan(-(M_PI / 2) + angle) - 50));
}

double meanAngle(double *array, int team) {
	double n = 0;
	double mean = 0;
	double value;
	if (team == 0) {
		value = M_PI / 2;
		for (int i = 0; i < 10; i++) {
			if (array[i] > value) {
				n++;
				mean += array[i];
			}
		}
	}
	else if (team == 1) {
		value = -M_PI / 2;
		for (int i = 0; i < 10; i++) {
			if (array[i] < value) {
				n++;
				mean += array[i];
			}
		}
	}

	return fabs(mean / n);
}
