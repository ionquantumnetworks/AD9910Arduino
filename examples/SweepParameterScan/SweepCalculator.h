//Header file
//Functions needed to calculate things for frequency sweep program.


#pragma once
#include <SPI.h>
#include <stdint.h>

struct stepFlag 
{
	double step;
	bool flag;
};

struct start_stop
{
	double start;
	double stop;
};


stepFlag calcStepTime(double stepSize, double stepRate, double maxStepTime = 0.000524288);

stepFlag calcStepSize(double stepTime, double stepRate, double maxStepSize = 100000);