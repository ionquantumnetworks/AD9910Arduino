#pragma once
#include <SPI.h>
#include <stdint.h>
//this should be moved to the header file eventually or we will have errors
struct stepFlag {
	double step;
	bool flag;
};
//

stepFlag calcStepTime(double stepSize, double stepRate, double maxStepTime = 0.000524288);

stepFlag calcStepSize(double stepTime, double stepRate, double maxStepSize = 100000);