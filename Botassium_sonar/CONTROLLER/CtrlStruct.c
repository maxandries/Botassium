#include "CtrlStruct.h"
#include <stdlib.h>
#include <time.h>
#define BILLION 1000000000L




CtrlStruct* initalize_struct(){
	CtrlStruct* structure = (CtrlStruct*) malloc(sizeof(CtrlStruct));

	structure->theUserStruct = (UserStruct *) malloc(sizeof(UserStruct));
	structure->theCtrlIn = (CtrlIn *) malloc(sizeof(CtrlIn));

	structure->theCtrlOut = (CtrlOut *) malloc(sizeof(CtrlOut));
	structure->theUserStruct->angle = (double *) malloc(3*sizeof(double));
	structure->strat = (Strategy *) malloc(sizeof(Strategy));
	structure->calib = (RobotCalibration*)malloc(sizeof(RobotCalibration));

	structure->calib->flag = 0;
	structure->strat->state = 0;

	structure->theUserStruct->Error_omega_l = 0;
	structure->theUserStruct->Error_omega_r = 0;
	structure->theUserStruct->speedLIN = 0;
	structure->theUserStruct->speedROT = 0;
	structure->theUserStruct->period = 0.02;
	structure->theUserStruct->windup = 1;
	structure->theUserStruct->Error_int_omega_l=0;
	structure->theUserStruct->Error_int_omega_r=0;
	structure->theUserStruct->Error_theta=0;
	structure->theUserStruct->Error_int_theta=0;
	structure->theUserStruct->int_posRot=1;
	structure->theUserStruct->int_posLin=1;
	structure->theUserStruct->ok = 1;

	structure->theUserStruct->Rob_x = 0;
	structure->theUserStruct->Rob_y = 0;
	structure->theUserStruct->Rob_theta = 0;

	structure->theUserStruct->GradX = 0;
	structure->theUserStruct->GradY = 0;

	structure->theUserStruct->omegaR = 0;
	structure->theUserStruct->omegaL = 0;

	structure->theUserStruct->opp1_x = 2000;
	structure->theUserStruct->opp1_y = 2000;
	structure->theUserStruct->opp2_x = 2000;
	structure->theUserStruct->opp2_y = 2000;

	structure->theUserStruct->rightOld = 0;
	structure->theUserStruct->leftOld = 0;
	structure->theUserStruct->side = 1;

	struct timespec t;
    clock_gettime(CLOCK_MONOTONIC_RAW, &t);
	structure->theUserStruct->t_init = (double)(t.tv_sec) + (double) t.tv_nsec/BILLION;
	structure->theUserStruct->t_odo = (double)(t.tv_sec) + (double) t.tv_nsec/BILLION;
	structure->theUserStruct->t_speed = (double)(t.tv_sec) + (double) t.tv_nsec/BILLION;
	structure->main_state = CALIB_STATE;

	structure->theUserStruct->speed_odo_left = 0;
	structure->theUserStruct->speed_odo_right = 0;

	structure->theUserStruct->distance1 = 0;
	structure->theUserStruct->distance2 = 0;
	structure->theUserStruct->distance3 = 0;
	structure->theUserStruct->distance4 = 0;

	structure->theUserStruct->opp_x = 2000;
	structure->theUserStruct->opp_y = 2000;

	structure->theUserStruct->flag_opp = 0;
	return structure;
}

void freeStruct(CtrlStruct *structure){
	free(structure->theUserStruct);
	free(structure);
}
