#ifndef _CTRLSTRUCT_H_
#define _CTRLSTRUCT_H_

#include "../IO/ctrl_io.h"


enum { CALIB_STATE, WAIT_START,STRATEGY, END};

enum {main_state1, main_state2, main_state3, main_state4, main_state5, main_state6, main_state7, main_state8, main_state9, main_state10, main_state11, main_state12, STOP };

/// strategy
typedef struct Strategy
{
	int state; ///< main state of the strategy

} Strategy;

/// robot calibration
typedef struct RobotCalibration
{
	int flag; ///< flag for calibration

} RobotCalibration;

typedef struct UserStruct{
	double speedROT;
	double speedLIN;
	double Error_omega_l;
	double Error_omega_r;
	double Error_int_omega_l;
	double Error_int_omega_r;
	double previous_theta;
	double Error_theta;
	double Error_int_theta;
	double Error_dist;
	double Error_int_dist;
	double *angle;
	long double period;
	int windup;
	int integration;
	int int_posRot;
	int int_posLin;
	int ok;

	double speed_odo_left;
	double speed_odo_right;

	double rightOld;
	double leftOld;

	double Rob_x;
	double Rob_y;
	double Rob_theta;

	double GradX;
	double GradY;

	double omegaR;
	double omegaL;

	double opp1_x;
	double opp1_y;
	double opp2_x;
	double opp2_y;

	double t_init;
	double t_odo;
	double t_speed;
	int side;

	double opp_x;
	double opp_y;

	int distance1;
	int distance2;
	int distance3;
	int distance4;

	int flag_opp;


} UserStruct;


typedef struct CtrlStruct{
	UserStruct *theUserStruct;  ///< user defined CtrlStruct
	CtrlIn *theCtrlIn;   ///< controller inputs
	CtrlOut *theCtrlOut; ///< controller outputs
	RobotCalibration *calib; ///< calibration
	int main_state;
	Strategy *strat;

} CtrlStruct;



int size_UserStruct();
CtrlStruct* initalize_struct();


#endif //ifndef
