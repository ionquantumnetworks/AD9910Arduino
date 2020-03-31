//Functions for determining step size or step rate of AD9910 frequency ramp scans
//These are intended for use in an adiabatic rapid passage program


#include "SweepCalculator.h"

//this should be moved to the header file eventually or we will have errors
//struct stepFlag {
	//double step;
	//bool flag;
//};
//


stepFlag calcStepTime(double stepSize, double stepRate, double maxStepTime = 0.000524288) 
{
	double st; //st = step time
	bool alertFlag;
	st = stepSize / stepRate; //Units should be Hz and Hz.. I think >.<
	if (st > maxStepTime || st < 0) 
	{
		alertFlag = true;
	}
	else
	{
		alertFlag = false;
	}
	return { st, alertFlag };
}

stepFlag calcStepSize(double stepTime, double stepRate, double maxStepSize = 100000)
{
	double ss; //ss = step size
	bool alertFlag;
	ss = stepTime * stepRate;
	if (ss > maxStepSize || ss < 0)
	{
		alertFlag = true;
	}
	else
	{
		alertFlag = false;
	}
	return { st, alertFlag };
}
