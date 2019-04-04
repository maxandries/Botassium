#include "speed_controller.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "CtrlStruct.h"
#include "tools.h"
#include "time.h"
#define KP 0.035
#define KI 0.20
#define KD 0.0001
#define BILLION 1000000000L

void run_speed_controller(CtrlStruct* theCtrlStruct, double refR, double refL){

	struct timespec t;
	clock_gettime(CLOCK_MONOTONIC_RAW, &t);
	double time = (double)(t.tv_sec) + (double)t.tv_nsec/BILLION ;
	double Delta_t = (time - theCtrlStruct->theUserStruct->t_speed);
	theCtrlStruct->theUserStruct->t_speed = time;
	
	double mesure_l = theCtrlStruct->theCtrlIn->l_wheel_speed;
	double mesure_r = theCtrlStruct->theCtrlIn->r_wheel_speed;
	if(refR != theCtrlStruct->theUserStruct->rightOld){
		theCtrlStruct->theUserStruct->Error_omega_r = 0;
	}
	if(refL != theCtrlStruct->theUserStruct->leftOld){
		theCtrlStruct->theUserStruct->Error_omega_l = 0;
	}
	double period = Delta_t;
	//theCtrlStruct->theUserStruct->time = theCtrlStruct->theCtrlIn->t;

	double error_l_old = theCtrlStruct->theUserStruct->Error_omega_l;
	double error_r_old = theCtrlStruct->theUserStruct->Error_omega_r;
	theCtrlStruct->theUserStruct->Error_omega_l = ((refL-mesure_l)*REDUCTION);
	theCtrlStruct->theUserStruct->Error_omega_r = ((refR-mesure_r)*REDUCTION);
	
	
	double derivativeL = (-error_l_old + theCtrlStruct->theUserStruct->Error_omega_l)/period;
	double derivativeR = (-error_r_old + theCtrlStruct->theUserStruct->Error_omega_r)/period;
	
	if(theCtrlStruct->theUserStruct->windup){
		theCtrlStruct->theUserStruct->Error_int_omega_l += theCtrlStruct->theUserStruct->Error_omega_l * period;
		theCtrlStruct->theUserStruct->Error_int_omega_r += theCtrlStruct->theUserStruct->Error_omega_r * period;
	}
	if(refR){
		theCtrlStruct->theUserStruct->Error_omega_r = 0;
	}
	if(refL){
		theCtrlStruct->theUserStruct->Error_omega_l = 0;
	}
	double U_ref_l = KP*(theCtrlStruct->theUserStruct->Error_omega_l) + KI*theCtrlStruct->theUserStruct->Error_int_omega_l + KD*derivativeL;
	double U_ref_r = KP*(theCtrlStruct->theUserStruct->Error_omega_r) + KI*theCtrlStruct->theUserStruct->Error_int_omega_r + KD*derivativeR;

	double Ia_l = U_ref_l/RA;
	double Ia_r = U_ref_r/RA;

	double applied_Il = saturation(CURRENTSAT, Ia_l);
	double applied_Ir = saturation(CURRENTSAT, Ia_r);

	double Ul = applied_Il*RA+KPHI*mesure_l*REDUCTION;
	double Ur = applied_Ir*RA+KPHI*mesure_r*REDUCTION;

	double applied_u_l = saturation(SAT, Ul);
	double applied_u_r = saturation(SAT, Ur);

	//anti-windup condition, if saturation we stop integrating
	if((Ul<(U_ref_l+KPHI*mesure_l*REDUCTION))||(Ul<(U_ref_l+KPHI*mesure_l*REDUCTION))){
		theCtrlStruct->theUserStruct->windup = 0;
	}
	else{
		theCtrlStruct->theUserStruct->windup = 1;
	}

	theCtrlStruct->theCtrlOut->wheel_commands[L_ID] = CtrlOutCalc(applied_u_l);
	theCtrlStruct->theCtrlOut->wheel_commands[R_ID] = CtrlOutCalc(applied_u_r);


}
