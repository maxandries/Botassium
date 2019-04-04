#include "tools.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "./CtrlStruct.h"
#include "speed_controller.h"
double mid_angle(double *angle){
	if((angle[1] < angle[0]) && fabs(angle[1])>fabs(angle[0])){
		return (M_PI+angle[0]+angle[1])/2;
	}
	else if((angle[1] < angle[0]) && fabs(angle[1])>fabs(angle[0])){
		return (-M_PI+angle[0]+angle[1])/2;
	}
	else{
		return (angle[0]+angle[1])/2;
	}
}

double getDistance(double angle){
	return 4.0/tan(angle/2.0);
}

void GetAngle(double *Tick, double *Return){

	Return[1] = ((2*M_PI/1790)*(Tick[1]-2685/4));

	if(Tick[0] < 1567){
		Return[0] = ((2*M_PI/1790)*(Tick[0]-2685/4));
	} 
	else{
		Return[0] = (M_PI/895)*Tick[0]-M_PI*2462/895;
	}

	if(Tick[1] < 1567){
		Return[1] = ((2*M_PI/1790)*(Tick[1]-2685/4));
	}
	else{
		Return[1] = (M_PI/895)*Tick[1]-M_PI*2462/895;
	}

	if((Return[1] < Return[0]) && fabs(Return[1])>fabs(Return[0])){
		Return[2] = -(Return[0]+Return[1]);
	}
	else if((Return[1] < Return[0]) && fabs(Return[1])>fabs(Return[0])){
		Return[2] = (Return[0]+Return[1]);
	}
	else{
		Return[2] = (Return[1]-Return[0]);
	}
}

double saturation(double boundaries, double input){
	if(fabs(input) < boundaries){
		return input;
	}
	else if(input < 0){
		return -boundaries;
	}
	else{
		return boundaries;
	}
}

double CtrlOutCalc(double u_ref){
	return 100*u_ref/SAT;
}


double speed(double tick, int TT){
	return (tick*2*M_PI*500)/TT;
}
