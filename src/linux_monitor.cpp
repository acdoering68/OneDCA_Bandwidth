
#include "monitor.h"

#include <sys/time.h> 
#include <vector>
#include <sstream>


//Frequency of the counter
// LONGLONG Frequency;



vector<unsigned long long> vCurrentCount; // , vLastCount;
// bool monitor_initialized;



int start_time()
{
  /*
if (!monitor_initialized)
{
	monitor_initialized = true;
	
if (!QueryPerformanceFrequency((LARGE_INTEGER*)&Frequency))
{throw( "Performance Counter does not exist");}

Frequency = Frequency / 1000; // want ms resolution

}
*/ 
unsigned long long  CurrentCount;
// QueryPerformanceCounter((LARGE_INTEGER*)&CurrentCount);

struct timeval tval;
// I use msec
gettimeofday(&tval,NULL); CurrentCount = tval.tv_sec * 1000 + tval.tv_usec / 1000;
vCurrentCount.push_back(CurrentCount);
// vLastCount.push_back(0);
return vCurrentCount.size()-1;
}



/*

void end_time(int i)
{
	LONGLONG LastCount;
	QueryPerformanceCounter((LARGE_INTEGER*)&LastCount);
	vLastCount[i]=LastCount;
};

*/

void restart_time(int i)
{
  unsigned long long LastCount;
struct timeval tval;
gettimeofday(&tval , NULL); LastCount = tval.tv_sec * 1000 + tval.tv_usec / 1000;
// LONGLONG LastCount;
//	QueryPerformanceCounter((LARGE_INTEGER*)&LastCount);
vCurrentCount[i]=LastCount;
}

string result_time(int i,int j)
{
	ostringstream strm;
	strm << (vCurrentCount[j] - vCurrentCount[i] );
	return strm.str();
}
