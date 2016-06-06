#include "TimeUtil.h" 
#include <sys/time.h> 
#include <time.h> 
	
	
double GetSystemTime()
{
	struct timeval t_start;
	gettimeofday(&t_start, NULL); 
	double start = ((double)t_start.tv_sec) * 1000.0+((double)t_start.tv_usec) / 1000.0;
	
	return start;
}
