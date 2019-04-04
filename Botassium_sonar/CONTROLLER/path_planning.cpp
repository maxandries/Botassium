#include "path_planning.h"
#include "CtrlStruct.h"
#include "math.h"
extern "C"{
	#include "stdio.h"
	#include "tools.h"
}
/*! \brief update the path-planning algorithm
*
* \param[in,out] structure controller main structure
*/
int sign(double a) {
	if (a > 0.0)
		return 1;
	else
		return -1;
}
void path_planning_update(CtrlStruct* structure, double Xgoal, double Ygoal)
{
	double Kf = 0.002; // gradient constant
	double Kr =0.1 ; // gradient constant
	double Vf = 0.0;
	double wr = 0.0;
	double omeRight = 0.0;
	double omeLeft = 0.0;
	double alpha = 0.0;
	double alphaR = 0.0;
	double pi = 3.14159265359;

	double L = 3000;
	double l = 2000;

	double Katt = 1;

	double x = structure->theUserStruct->Rob_x + l / 2;
	double y = structure->theUserStruct->Rob_y + L / 2;

	Xgoal = Xgoal + l / 2;
	Ygoal = Ygoal + L / 2;

	double F_attX;
	double F_attY;

	double sat = 4000;
	// attractive part :
	double distance = sqrt(pow(x - Xgoal, 2) + pow(y - Ygoal, 2));
	if (fabs(Katt*(-x + Xgoal)) < sat) {
		F_attX = Katt * (-x + Xgoal);
	}
	else {
		F_attX = sat * sign(Katt * (-x + Xgoal));
	}
	if (fabs(Katt*(-y + Ygoal)) < sat) {
		F_attY = Katt * (-y + Ygoal);
	}
	else {
		F_attY = sat * sign(Katt * (-y + Ygoal));
	}

	// repulsive part :

	double Krep = 10000000000;
	double DistInflu = 200;
	double ditanceMin = 5000;
	double N = 50; // r�solution
	double dist = 0.0;
	double Xlow = 0.0; //nearst distance from an obsracle in x direction
	double Ylow = 0.0; //nearst distance from an obsracle in y direction
	double F_repX = 0.0;
	double F_repY = 0.0;


	for (int i = 0; i < l; i = int(i + l / N)) {
		for (int j = 0; j < L; j = int(j + L / N)) {
			if (IsObstacle(structure,i, j)) {
				dist = sqrt(pow(i - x, 2) + pow(j - y, 2));
				if (dist < ditanceMin) {
					ditanceMin = dist;
					Xlow = i;
					Ylow = j;
				}
			}
		}
	}
	if (ditanceMin < DistInflu) {
		F_repX = Krep * (1 / ditanceMin - (1 / DistInflu))*(1 / pow(ditanceMin, 2))*((x - Xlow) / ditanceMin);
		F_repY = Krep * (1 / ditanceMin - (1 / DistInflu))*(1 / pow(ditanceMin, 2))*((y - Ylow) / ditanceMin);
	}
	//printf("F_attX = %f et F_atty=%f \n", F_attX, F_attY);
	//printf("F_repX = %f et Frep_y=%f \n", F_repX, F_repY);
	//printf("GradX = %f et GradYf = %f \n", F_repX + +F_attX, F_repY + F_attY);
	structure->theUserStruct->GradX = F_repX + F_attX;
	structure->theUserStruct->GradY = F_repY + F_attY;
	printf("theta %f\n", structure->theUserStruct->Rob_theta);
	alpha = atan2(structure->theUserStruct->GradY, structure->theUserStruct->GradX) - structure->theUserStruct->Rob_theta;
	//alpha = alpha - alpha%2*M_PI;
	
	if (alpha < 0.0) {
		alpha = alpha + 2 * pi;
	}
	else if (alpha > 2 * pi) {
		alpha = alpha - 2 * pi;
	}
	if(alpha < 0){
		alpha = alpha + 2*M_PI;
	}
	else if(alpha > 2*M_PI){
		alpha -= 2*M_PI; 
	}
	//printf(" alph = %f \n", alpha * 180 / 3.1427);
	//printf(" alpha = %f \n ",alpha * 180 / 3.1427);
	if (alpha < pi / 8 || alpha > 15 * pi / 8) {
		//Vf = Kf*(structure->GradX * cos(structure->Rob_theta) + structure->GradY * sin(structure->Rob_theta))*cos(alpha);
		Vf = Kf* sqrt(pow(structure->theUserStruct->GradX, 2) + pow(structure->theUserStruct->GradY, 2))*cos(alpha);
		Kr = 1;
	}
	else if( alpha < M_PI/4 || alpha > 7*M_PI/4){
		Vf = Kf/2 * sqrt(pow(structure->theUserStruct->GradX, 2) + pow(structure->theUserStruct->GradY, 2))*cos(alpha);
		Kr = 1;
	}
 
	else {
		Vf = 0.0;
		Kr =  10*Kr;
	}
	if (alpha < pi) {
		alphaR = alpha;
	}
	else {
		alphaR = alpha - 2 * pi;
	}
	
	//printf(" alpha après = %f \n ", alpha * 180 / 3.1427);
	wr = Kr * alphaR;
	/*if (Vf>0){
		Vf = fmax(Vf, 1);
	}*/
	/*if (wr >0){
		wr = fmax(wr,0.5);
	}
	if(wr <0){
		wr = fmin(wr, -0.5);
	}*/
	Vf = fmin(Vf, 0.25);
	//if(Vf<0){
	//	Vf = 0;
	//}
	printf("wr = %f \n", wr);
	printf("Vf = %f \n", Vf);
	omeRight = (1 / 0.03)*(wr *0.225 / 2 + Vf);
	omeLeft = (1 / 0.03)*(-wr * 0.225 / 2 + Vf);
	structure->theUserStruct->omegaR = omeRight;
	structure->theUserStruct->omegaL = omeLeft;

}

bool IsObstacle(CtrlStruct *structure,double x, double y) {
	double Is = 0;
	double mur = 30;

	/*if (pow((x - 1050), 2) + pow((y - 1000), 2) < pow(150, 2)) { // zone de chao
		Is = 1;
	}
	else if (pow((x - 1050), 2) + pow((y - 2000), 2) < pow(150, 2)) { // zone de chao
		Is = 1;
	}*/

	if (x > 1543 && y > 450 && y < 2550) { // rampe + balance
		Is = 1;
	}
	else if (x > 1400 && x < 1600 && y>1480 && y < 1520) { // la pointe devant balance
		Is = 1;
	}
	else if (x < 66 && y>500 && y < 2500) { // acc�l�rateur de particules
		Is = 1;
	}
	else if (x<mur || x>(2000 - mur) || y<mur || y>(3000 - mur)) {// murs
		Is = 1;
	}
	else if (pow(x - structure->theUserStruct->opp1_x, 2) + pow(y - structure->theUserStruct->opp1_y, 2) < pow(300, 2)) { // adverssaire 1
		Is = 1;
	}
	else if (pow(x - structure->theUserStruct->opp2_x, 2) + pow(y - structure->theUserStruct->opp2_y, 2) < pow(300, 2)) { // adverssaire 2
		Is = 1;
	}

	return Is;
}

int BoundOk(double a, double b, double tolerance) {
	if (fabs(a) < fabs(b)+(tolerance) && fabs(a) > fabs(b) - tolerance)
	{
		return 1;
	}
	return 0;
}

/*! Is used in the FSM. Apply the potential field algorithm by default until it is close to the target (0.1m).
 *  If to_end is 1, continues towards the target on a straight line.
 *  If dir = -1, goes backward. If dir = 1, goes frontward.
 *  Returns 1 if the target is reached
 */

int goTo(double x_ref, double y_ref, CtrlStruct* structure)
{
	if (!BoundOk(structure->theUserStruct->Rob_x, x_ref, 100) || !BoundOk(structure->theUserStruct->Rob_y, y_ref, 100))
	{
		path_planning_update(structure,x_ref, y_ref);
		//printf("error X %f error Y %f\n", x_ref - structure->Rob_x, y_ref - structure->Rob_y);
		return 0;
	}
	return 1;
}
