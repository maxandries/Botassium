#include "rotCtrl.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "tools.h"
#include "speed_controller.h"
#include "CtrlStruct.h"
#define KPr 0.001

void run_angle_controller(CtrlStruct *theCtrlStruct, double Ref_angle){
	double position = ((double) theCtrlStruct->theCtrlIn->odoLeft - (double)theCtrlStruct->theCtrlIn->odoRight)/2;
	double angle_ref = Ref_angle*9500*10.5/(360*3);
	double error = (angle_ref - position)*REDUCTION;
	printf("error %1.2f", error);
	double WrefRot = KPr*error;


	if(error < 250){
			theCtrlStruct->theUserStruct->speedROT = 0;
			theCtrlStruct->theUserStruct->ok =0;
	}
	else{
		theCtrlStruct->theUserStruct->speedROT = saturation(WrefRot, 6*M_PI);
	}

}
