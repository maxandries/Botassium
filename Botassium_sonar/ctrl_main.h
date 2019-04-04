/*!
 * \file ctrl_main_gr2.h
 * \brief main header of the controller
 */

#ifndef _CTRL_MAIN_H_
#define _CTRL_MAIN_H_

#include "./CONTROLLER/calibration.h"
extern "C"{
  #include "./CONTROLLER/CtrlStruct.h"

}


void controller_init(CtrlStruct *cvs);
void controller_loop(CtrlStruct *cvs);
void controller_finish(CtrlStruct *cvs);

#endif
