

#include "monitor.h"

#include <windows.h>
#include <vector>
#include <sstream>


//Frequency of the counter
LONGLONG Frequency;



vector<LONGLONG> vCurrentCount; // , vLastCount;
bool monitor_initialized;

//Zeitdifferenz in Sekunden


int start_time()
{
if (!monitor_initialized)
{
	monitor_initialized = true;
if (!QueryPerformanceFrequency((LARGE_INTEGER*)&Frequency))
{throw( "Performance Counter does not exist");}
 vCurrentCount.assign(20,0);
Frequency = Frequency / 1000; // want ms resolution
}
LONGLONG CurrentCount;
QueryPerformanceCounter((LARGE_INTEGER*)&CurrentCount);
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
LONGLONG LastCount;
	QueryPerformanceCounter((LARGE_INTEGER*)&LastCount);
vCurrentCount[i]=LastCount;
}

string result_time(int i,int j)
{
	ostringstream strm;
	strm << ((vCurrentCount[j] - vCurrentCount[i] ) / Frequency);
	return strm.str();
}