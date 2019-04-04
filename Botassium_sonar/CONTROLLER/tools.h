#ifndef _tools_H_
#define _tools_H_



void GetAngle(double *Tick, double *Return);
double mid_angle(double *angle);
double getDistance(double angle);
double saturation(double boundaries, double input);
double CtrlOutCalc(double u_ref);
double speed(double tick, int TT);

#endif //ifndef
