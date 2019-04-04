#include "SPI.hh"
#include <stdio.h>

SPI::SPI(unsigned int cs, unsigned int baud)
{
	this->cs = cs;
	this->baud = baud;
	wiringPiSPISetup(cs, baud);	
}

SPI::~SPI()
{
}


// ------------- SPI functions -------------

void SPI::tobytes(int len, int val, unsigned char *bytes)
{
    	for(int i=0; i<len; ++i)
        	bytes[i] = ( val >> 8*(len-(i+1)) ) % (256);
}

int SPI::frombytes(int len, unsigned char *bytes)
{
	int res = 0;
	for(int i=0; i<len; ++i){
        	res = res + bytes[len-i-1]*((2 << (8*i))/2);
     }
    return res;
}

int SPI::frombytes_speed(int len, unsigned char *bytes)
{
    unsigned int res = 0;
	for(int i=0; i<len-1; ++i){
        	res = res + bytes[len-i-1]*((2 << (8*i))/2);
     }
	 unsigned int test = bytes[0];
	 unsigned int mask = 1<<7;
    int reverse = (test&mask);
    //printf("reverse: %d speed: %d\n", reverse, res);
    if(reverse)
		return -res;
	else
		return res;
}
