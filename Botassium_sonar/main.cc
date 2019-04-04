#include <cstdio>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <wiringPiSPI.h>
#include <ctime>
#include "IO/COM/CAN/CAN.hh"
#include "IO/COM/SPI/Specific/SPI_DE0.hh"
#include "IO/COM/SPI/SPI.hh"
#include <iostream>
#include <signal.h>
#include "./ctrl_main.h"
extern "C"{
#include "./CONTROLLER/CtrlStruct.h"
#include "./CONTROLLER/speed_controller.h"
#include "./CONTROLLER/posCtrl.h"
#include "./CONTROLLER/rotCtrl.h"
#include "./CONTROLLER/tools.h"
#include "stdio.h"
}

#define CAN_BR 500e3

int quit;

void my_handler(int s){
           printf("Caught signal %d\n",s);
           quit = 1;

}

int main(){

	printf("hello max!\n");
	printf("##############################################################################################################\n");
  printf("######\t\t\t  Welcome to the Minibot project of the ELEME2002 class :)\t\t\t######\n");
  printf("##############################################################################################################\n");
  printf("\t\t I'm Miss Sunshine, please take care of me !\n");
  printf("\t\t Please do not interchange the chips on my tower/motor PWM boards !\n");
  printf("\t\t Try to respect the C-file interface when programming me because\n \t\t it will be the same in the robotic project (Q2) !\n");

  //Signal handler to stop the programme and the motors when CTRL+C is hit
  struct sigaction sigIntHandler;
  sigIntHandler.sa_handler = my_handler;
  sigemptyset(&sigIntHandler.sa_mask);
  sigIntHandler.sa_flags = 0;
  sigaction(SIGINT, &sigIntHandler, NULL);
  quit = 0;

	//Configuration of the CAN bus
	CAN *can;
	can = new CAN(CAN_BR);
	can->configure();
	can->RST_cycle();//Reset the period so with 0 mean duty cycle applied, the motors don't spin
	can->RST_lsb();
	can->ctrl_motor(1);
	can->push_PropDC(0,0);


	//Configuration of the SPI bus
	SPI_DE0 *spi;
	spi = new SPI_DE0(0,500000);

	//Create and initialize the controle structure
	CtrlStruct* structure;
    printf("ok\n");
  structure =  initalize_struct();
      printf("ok2\n");
	int tick[2];
	double tick2[2];
	double *angleInfo= (double *) malloc(3*sizeof(double));
	double speedRight;
	double speedLeft;
	int tickRight;
	int tickLeft;
	double tickR;
	double tickL;
	double speedRef[2];
	int leftOffset;
	int rightOffset;
	int tickL1;
	int tickR1;
	double tL1;
	double tR1;
	int switchs;
  int odoLeft, odoRight;
  FILE *fp;
	fp = fopen("result.txt","w");
  while(1){
		//usleep(2000);

		tickL1 = spi->read_speed(0x3);
		tickR1 = spi->read_speed(0x4);
		tL1 = (double) tickL1;
		tR1 = (double) tickR1;
		tL1 = speed(tL1, 2048);
		tR1 = speed(tR1, 2048);
		structure->theUserStruct->speed_odo_left = tL1;
		structure->theUserStruct->speed_odo_right = tR1;

		//ENCODEUR
		tickRight = (spi->read_speed(0x0));
		tickLeft = (spi->read_speed(0x1));
		tickR = (double) tickRight;
		tickL = (double) tickLeft;
		speedRight = speed(tickR, 9500);
		speedLeft = speed(tickL, 9500);
		structure->theCtrlIn->l_wheel_speed = speedLeft;
		structure->theCtrlIn->r_wheel_speed = speedRight;


	switchs = spi->read(0x5);
	structure->theCtrlIn->u_switch[R_ID] = switchs & 1;
	structure->theCtrlIn->u_switch[L_ID] = switchs >> 1;
    
	// sonars
    if(spi->read(0x6) ==0){
      structure->theUserStruct->distance1 = 2000;
    }
    else{
      structure->theUserStruct->distance1 =spi->read(0x6);
    }
    if(spi->read(0x7) ==0){
      structure->theUserStruct->distance2 = 2000;
    }
    else{
      structure->theUserStruct->distance2 = spi->read(0x7);
    }
    if(spi->read(0x8) ==0){
      structure->theUserStruct->distance3 = 2000;
    }
    else{
      structure->theUserStruct->distance3 =2000;//spi->read(0x8);
    }
    if(spi->read(0x9) ==0){
      structure->theUserStruct->distance4 = 2000;
    }
    else{
      structure->theUserStruct->distance4 = 2000;//spi->read(0x9);
    }
	//int d = spi->read(0x6);
	//printf("d = %d \n",d);
	//printf("distance1 %d distance2 %d distance3 %d distance4 %d\n", structure->theUserStruct->distance1,structure->theUserStruct->distance2,structure->theUserStruct->distance3,structure->theUserStruct->distance4);

		//printf("left %f, right %f\n", tL1, tR1);
		fprintf(fp,"%f,%f\n", speedLeft, speedRight);
		controller_loop(structure);
		//run_speed_controller(structure, 10, 10);
		can->push_PropDC(structure->theCtrlOut->wheel_commands[R_ID], structure->theCtrlOut->wheel_commands[L_ID]);
		//printf("X %f, Y %f, theta %f\n",structure->theUserStruct->Rob_x, structure->theUserStruct->Rob_y, structure->theUserStruct->Rob_theta);
		if(quit ==1){
			break;
		}
  }
  quit = 0;
  fclose(fp);
  can->push_PropDC(0,0);
  can->ctrl_motor(0);

}
