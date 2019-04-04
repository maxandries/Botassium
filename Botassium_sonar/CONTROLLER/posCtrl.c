#include "posCtrl.h"
#include "tools.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "./speed_controller.h"

#define KPl 0.05


void run_dist_controller(CtrlStruct *theCtrlStruct, double Ref_distance){
		double left = (double) theCtrlStruct->theCtrlIn->odoLeft;
		double right = (double) theCtrlStruct->theCtrlIn->odoRight;
		double position = (left + right)/2;
		double distanceRef = 9500*Ref_distance/(2*M_PI*3); //3 rayon roue cm 9500 tick/tour
		double error = (distanceRef-position)*REDUCTION;
		printf("error %1.4f\n", distanceRef);
		double error_old = theCtrlStruct->theUserStruct->Error_dist;
		theCtrlStruct->theUserStruct->Error_dist = error;

		double WrefLin = KPl*error;



		if(error < 5000){
			theCtrlStruct->theUserStruct->speedLIN = 0;
			theCtrlStruct->theUserStruct->ok =0;
		}
		else{
			theCtrlStruct->theUserStruct->speedLIN = saturation(WrefLin, 6*M_PI);
		}

}
