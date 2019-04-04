#ifndef _SPEED_CONTROLLER_H_
#define _SPEED_CONTROLLER_H_

#include "CtrlStruct.h"

#define REDUCTION 19
#define KPHI 0.03783
#define GAINW 0.011
#define SAT 22.8
#define CURRENTSAT 0.74
#define RA 5.84


void run_speed_controller(CtrlStruct *theCtrlStruct, double omegaR, double omegaL);


#endif //ifndef
