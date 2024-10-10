// ncabw.cpp : Defines the entry point for the console application.
//

#include "Enum_exs.h"
#include "monitor.h"

int test_main(int argc, char* argv[]);

// for now, performance counter  here



int argedmain(int argc, char* argv[])
{
	Config * MyCfg = new Config; 
    string toptag("CellAutBW");
MyCfg->Init(argv[1]);

    if (argc > 2) 
	{
		toptag = argv[2];
	}
	int thandle1 = start_time();	
	int thandle2;
	try{

	
	
	
	
	ca_analysis  * MyAlgorithm = CreateNamedAlgorithm(MyCfg,toptag); 
    
	cout << " Algorithm created " << endl;
    MyAlgorithm ->configure(MyCfg,toptag);
    cout << "Algorithm configured " << endl;
	MyAlgorithm->prepare();
	cout << "Algorithm prepared " << endl;
	
	MyAlgorithm -> calc();
	MyAlgorithm -> report(toptag);

	}catch(...)
	{
		cout << "Unhandled exception occured!";
	}
   
    thandle2 = start_time();
    cout << "total time " << result_time(thandle1,thandle2) << endl;
	return 0;

}


int main(int argc, char* argv[])
{

    int res;
	char ch;




/*
if (!QueryPerformanceFrequency((LARGE_INTEGER*)&Frequency))
{cout << "Performance Counter does not exist";}

Frequency = Frequency / 1000; // want ms resolution
QueryPerformanceCounter((LARGE_INTEGER*)&CurrentCount);

*/




	char * DefaultFilename = "Problem1.xml";
	char * FakeArgv[2];
	FakeArgv[0] = argv[1];
	FakeArgv[1] = DefaultFilename;
	  // was test_main from top_test
        if ((argc>=2) && (argv[1][0]=='-') && (argv[1][1]=='t'))
          {// test modes
	    FakeArgv[0]=argv[1];
            FakeArgv[1]=argv[2];
	    switch(argv[2][2])
	      {
	      case '2': return test_main2(); break;
              case '3': return test_main3(); break;
              case '4': return test_main4(); break;
              case '5': return test_main5(); break;
              case '6': return test_main6(); break;
              case '7': return test_main7(); break;
			  case 'A': return test_main10();break;
		cout << "Unknown test required!\n";
		return -1;  
	      }      
	  } 
	if (argc==1) {argedmain(2,(char **) FakeArgv);}
	 else {return argedmain(argc,argv);}

	 /* this was all for testing
    // lets do a simple example:
// test_main2();
// return test_main6();

int thandle = start_time();	
	try{

	res= test_main8();
	}catch(...)
	{
		cout << "Unhandled exception occured!";
	}
end_time(thandle);


// QueryPerformanceCounter((LARGE_INTEGER*)&LastCount);
// iTimeDiff = ((LastCount - CurrentCount) / Frequency); 

    cout << "Elapsed time " << result_time(thandle) << "\n";

	cin >> ch;



	return res;
	*/
}

