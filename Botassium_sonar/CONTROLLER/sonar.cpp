#include <cmath>
#include "sonar.h"
extern "C"{
	#include "speed_controller.h"
	#include <unistd.h>
	#include "stdio.h"
}



void sonar(CtrlStruct* structure) {
	double d3 = structure->theUserStruct->distance1;
	double d4 = structure->theUserStruct->distance2;
	/*double d3 = structure->theUserStruct->distance3;
	double d4 = structure->theUserStruct->distance4;*/

	double theta = structure->theUserStruct->Rob_theta;
	double Rob_x = structure->theUserStruct->Rob_x;
	double Rob_y = structure->theUserStruct->Rob_y;

	double pi = 3.14159265359;
	/*double sonar1x = 120;
	double sonar1y = -120;
	double eta1 = -pi/4;

	double sonar2x = 120;
	double sonar2y = 120;
	double eta2 = pi/4;
*/
	double sonar3x = 145;
	double sonar3y = -75;
	double eta3 = 0;

	double sonar4x = 145;
	double sonar4y = 75;
	double eta4 = 0;

	/*double pos1xR = sonar1x + d1*cos(eta1);
	double pos1yR = sonar1y + d1*sin(eta1);
	double pos2xR = sonar2x + d2*cos(eta2);
	double pos2yR = sonar2y + d2*sin(eta2);*/
	double pos3xR = sonar3x + d3*cos(eta3);
	double pos3yR = sonar3y + d3*sin(eta3);
	double pos4xR = sonar4x + d4*cos(eta4);
	double pos4yR = sonar4y + d4*sin(eta4);

	/*double pos1xI = pos1xR*cos(theta) + pos1yR*sin(theta) + Rob_x;
	double pos1yI = pos1yR*cos(theta) - pos1xR*sin(theta) + Rob_y;
	double pos2xI = pos2xR*cos(theta) + pos2yR*sin(theta) + Rob_x;
	double pos2yI = pos2yR*cos(theta) - pos2xR*sin(theta) + Rob_y;*/
	double pos3xI = pos3xR*cos(theta) + pos3yR*sin(theta) + Rob_x;
	double pos3yI = pos3yR*cos(theta) - pos3xR*sin(theta) + Rob_y;
	double pos4xI = pos4xR*cos(theta) + pos4yR*sin(theta) + Rob_x;
	double pos4yI = pos4yR*cos(theta) - pos4xR*sin(theta) + Rob_y;
	
	bool robot = false;
	if(/*IsNotWall(pos1xI,pos1yI) ||IsNotWall(pos2xI,pos2yI) ||*/ IsNotWall(pos3xI,pos3yI) ||IsNotWall(pos4xI,pos4yI)){
		robot = true;
	}

	double distmin = 200;
	int sonar =0;
	/*if(IsNotWall(pos1xI,pos1yI) && d1 < distmin){
		int sonar = 1;
		distmin = d1;
		structure->theUserStruct->flag_opp = 1;
		structure->theUserStruct->opp_x = pos1xI;
		structure->theUserStruct->opp_y = pos1yI;
	}
	else if(IsNotWall(pos2xI,pos2yI) && d2 < distmin){
		int sonar = 2;
		distmin = d2;
		structure->theUserStruct->flag_opp = 1;
		structure->theUserStruct->opp_x = pos2xI;
		structure->theUserStruct->opp_y = pos2yI;
	}*/
	if(IsNotWall(pos3xI,pos3yI) && d3 < distmin){
		int sonar = 3;
		distmin = d3;
		structure->theUserStruct->flag_opp = 1;
		structure->theUserStruct->opp_x = pos3xI;
		structure->theUserStruct->opp_y = pos3yI;
	}
	else if(IsNotWall(pos4xI,pos4yI) && d4 < distmin){
		int sonar = 4;
		distmin = d4;
		structure->theUserStruct->flag_opp = 1;
		structure->theUserStruct->opp_x = pos4xI;
		structure->theUserStruct->opp_y = pos4yI;

	}
	else{
		//printf("else\n");
		structure->theUserStruct->flag_opp = 0;
		structure->theUserStruct->opp_x = 2000;
		structure->theUserStruct->opp_y = 2000;
	}
	//printf("int sonar %d\n", sonar);
}

int IsNotWall(double x, double y){
	double Is = 1;
	double mur = 30;
	double tol = 50;

	if (x > 543-tol && y >-950-tol && y < 1050+tol) { // rampe + balance
		Is = 0;
	}
	else if (x > 400-tol && x < 600+tol && y>-20-tol && y < 20+tol) { // la pointe devant balance
		Is = 0;
	}
	else if (x < -934+tol && y>-1000-tol && y < 1000+tol) { // acc�l�rateur de particules
		Is = 0;
	}
	else if (x<-1000+mur+tol || x>(1000 - mur)-tol || y<-1500+mur+tol || y>(1500 - mur)-tol) {// murs
		Is = 0;
	}
	//printf("X %f, Y %f, is %d\n", x, y, Is);
	return Is;

}
