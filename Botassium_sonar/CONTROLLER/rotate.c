#include "rotate.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "CtrlStruct.h"
#include "tools.h"
#include "time.h"
#include "speed_controller.h"
#define KR 1

void rotate(CtrlStruct* theCtrlStruct, double direction){
	double alpha;
  double pi = 3.14159265359;

	if (direction> pi) {
		alpha = direction - 2 * pi;
	}
	else{
		alpha = direction;
  }

	double Vf = 0.0;
	double wr = KR * alpha;
	double omeRight = (1 / 0.03)*(wr *0.225 / 2 + Vf);
	double omeLeft = (1 / 0.03)*(-wr * 0.225 / 2 + Vf);
  run_speed_controller(theCtrlStruct,omeRight,omeLeft);
  double error = (direction - theCtrlStruct->theUserStruct->Rob_theta)/(2*pi);
  if (error < 0.01){
    run_speed_controller(theCtrlStruct,0,0);
    theCtrlStruct->strat->state = main_state3;
  }
}
