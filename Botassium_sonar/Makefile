CC = g++
CFLAGS =
LIBS = -lwiringPi
OBJECTS = SPI.o SPI_DE0.o SPI_CAN.o CAN.o CtrlStruct.o tools.o speed_controller.o sonar.o rotate.o posCtrl.o rotCtrl.o calibration.o odometry.o path_planning.o strategy.o ctrl_main.o main.o

all: clean $(OBJECTS)
	$(CC) $(CFLAGS) -o main $(OBJECTS) $(LIBS)
	rm -f *.o

main.o: main.cc
	$(CC) -o $@ -c $<

CAN.o: ./IO/COM/CAN/CAN.cc
	$(CC) $(CFLAGS) -o $@ -c $<

SPI_CAN.o: ./IO/COM/SPI/Specific/SPI_CAN.cc
	$(CC) $(CFLAGS) -o $@ -c $<

SPI_DE0.o: ./IO/COM/SPI/Specific/SPI_DE0.cc
	$(CC) $(CFLAGS) -o $@ -c $<

SPI.o: ./IO/COM/SPI/SPI.cc
	$(CC) $(CFLAGS) -o $@ -c $<

speed_controller.o: ./CONTROLLER/speed_controller.c
	gcc $(CFLAGS) -o $@ -c $<

CtrlStruct.o: ./CONTROLLER/CtrlStruct.c
	gcc $(CFLAGS) -o $@ -c $<

posCtrl.o: ./CONTROLLER/posCtrl.c
	gcc $(CFLAGS) -o $@ -c $<
 
sonar.o: ./CONTROLLER/sonar.cpp
	gcc $(CFLAGS) -o $@ -c $<
 
rotate.o: ./CONTROLLER/rotate.c
	gcc $(CFLAGS) -o $@ -c $<
 
rotCtrl.o: ./CONTROLLER/rotCtrl.c
	gcc $(CFLAGS) -o $@ -c $<

tools.o: ./CONTROLLER/tools.c
	gcc $(CFLAGS) -o $@ -c $<

calibration.o: ./CONTROLLER/calibration.cpp
	$(CC) $(CFLAGS) -o $@ -c $<

odometry.o: ./CONTROLLER/odometry.cpp
	$(CC) $(CFLAGS) -o $@ -c $<

path_planning.o: ./CONTROLLER/path_planning.cpp
	$(CC) $(CFLAGS) -o $@ -c $<

strategy.o: ./strategy.cpp
	$(CC) $(CFLAGS) -o $@ -c $<

ctrl_main.o: ./ctrl_main.cc ./CONTROLLER/calibration.cpp
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -f *.o
	rm -f ./main
