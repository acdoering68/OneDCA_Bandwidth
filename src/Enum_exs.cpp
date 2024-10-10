
#include "Enum_exs.h"


int test_main2()
{
int i;
char ch;
TopoIndexer myTI(12);
cout << "Max Index in TI is " << myTI.max_ix << "\n";

cout << "Receive Baseline: ";
for (i=0;i<30;i++) {cout << myTI.baseline_r(i) << " ";}
cout << "Send Baseline: ";
for (i=0;i<15;i++) {cout << myTI.baseline_s(i) << " ";}

cin >> ch;

// for (i=0;i<myTI->max_ix;i++)
return 0;

}


int test_main3()
{

char ch;

TopoIndexer myTI(80);

CellAut myCA;
OneD_direct myCAL(0);
BinaryFSSP myCAS;
// MooreFSSP myCAS;
myCA.CALocation=  &myCAL;
myCA.CState = &myCAS;

intcellstate Aconf[40];
intcellstate Cconf[20];
intcellstate cone_stor[1200]; // be a bit generous

int i; int l;
int j; int k;

for (i=0;i<1200;i++) {cone_stor[i]=0;}

for(i=0;i<40;i++) {Aconf[i]=0;}
Aconf[10]=3;
for(i=0;i<20;i++) {Cconf[i]=0;}
Cconf[8]=1;

cout << "starting ... \n";

calc_cone(&myTI, &myCA,Aconf,Cconf,cone_stor,20,20);
l=20;
k=20;
for (i=0;i<10;i++)
{
	for (j=0;j<l;j++)
	{
		cout << myCA.CState->display_table[cone_stor[k++]];
		// cout << cone_stor[k++] << " ";
	}
	cout << "\n";
	l = l-3;
}

cout << "result:\n";
for (i=myTI.generation_start(20,19) ;i<myTI.generation_end(20,19);i++)
{
	cout << cone_stor[myTI.offsets_per_generation(20,19)+i] << ",";
}


cin >> ch;

return 0;
}


int test_main4()
{

char ch;

TopoIndexer myTI(80);

CellAut myCA;
OneD_direct myCAL(0);
MazoyerFSSP myCAS;
// MooreFSSP myCAS;
myCA.CALocation=  &myCAL;
myCA.CState = &myCAS;

intcellstate Aconf[40];
intcellstate Cconf[20];
intcellstate cone_stor[1200]; // be a bit generous

int i; int l;
int j; int k;

int w=20; 
int d=20;

for (i=0;i<1200;i++) {cone_stor[i]=0;}

for(i=0;i<40;i++) {Aconf[i]=1;}
Aconf[10]=0; Aconf[9]=5; // g
for(i=0;i<20;i++) {Cconf[i]=1;}
Cconf[8]=0; // Cconf[7]=5;


for (i=0;i<20;i++) cout << myTI.offsets_per_generation(d,i) << " ";

cout << "starting ... \n";

calc_cone(&myTI, &myCA,Aconf,Cconf,cone_stor,d,w);

cout << "Cone ... \n";

string cone_line; // (20*d,' ');

for (i=0;i<d;i++)
{
	cone_line.assign(10*d,' ');
	// for (j=0;j<i;j++) {cout <<" ";}

for (j=myTI.generation_start(d,i);j<myTI.generation_end(d,i);j++)
{
	ch = myCA.CState->display_table[cone_stor[myTI.offsets_per_generation(d,i)+j ]];
	cone_line[myTI.index2location(j)->position+2*d]=ch;
	// cout <<  // got to rearrange chars back to topology 
	
   

}
cout << cone_line.substr(0,4*d);
cout << "\n";


}

/* Believe our cone-ix and use its indexes 
l=20;
k=20;
for (i=0;i<10;i++)
{
	for (j=0;j<l;j++)
	{
		cout << myCA.CState->display_table[cone_stor[k++]];
		// cout << cone_stor[k++] << " ";
	}
	cout << "\n";
	l = l-3;
}
*/
cout << "result:\n";
for (i=myTI.generation_start(20,19) ;i<myTI.generation_end(20,19);i++)
// for (i=0;i<myTI.cells_per_generation(20,19);i++)
{
	cout << cone_stor[myTI.offsets_per_generation(20,19)+i] << ",";
}


cin >> ch;

return 0;
}
// just show results of TopoIndexer
int test_main5()
{

char ch;
int i;
int j;
int n;
TopoIndexer myTI(80);


int w=20; 
int d=5;


for (i=0;i<=d;i++)
{

	for (j=0;j<i;j++) {cout <<"    ";}
for (j=myTI.generation_start(d,i);j<myTI.generation_end(d,i);j++)
{

cout.width(3);	
cout << myTI.offsets_per_generation(d,i+1)+j << ":";

for (n=0;n<3;n++)
{
cout.width(4);	cout<<  myTI.offsets_per_generation(d,i)+ myTI.nb_ix(d,i,j)[n];
}
cout << ";";

}
cout << "\n";
}

cin >> ch;
return 0;
}

int test_main6()
{



CellAut myCA;
OneD_direct myCAL(0);
MazoyerFSSP myCAS;
// MooreFSSP myCAS;
myCA.CALocation=  &myCAL;
myCA.CState = &myCAS;

int i;
char ch;

Enum_BWInc * myEBW = new Enum_BWInc(myCA);

myEBW->td=3;
myEBW->sd=1;
myEBW->prepare();

cout << "Starting Enumeration \n";

ofstream Reports;
Reports.open("EnumBW_report.txt");
myEBW->report_decent=true;
myEBW->report_sizes=true;
myEBW->report_stream= &Reports;

myEBW->calc();

cout << "Images for best Aconf " << myEBW->bestAbsets << "\n";

for (i=0; i<2* myEBW->td;i++) 
{ch = myCA.CState->display_table[myEBW->Abestconf[i]];
cout << ch;}


ofstream Aconfs;
 Aconfs.open ("aconfs_3_mazoyer.txt");
 

 for (i=0;i< myEBW->Bcountvals.size();i++)
 {Aconfs << myEBW->Bcountvals[i] << "\n";}
 Aconfs.close();
 Reports.close(); 

cin >> ch;

	return 0;

}

int test_main7()
{



CellAut myCA;
OneD_direct myCAL(0);
MazoyerFSSP myCAS;
// MooreFSSP myCAS;
// Yunes1FSSP myCAS;
myCA.CALocation=  &myCAL;
myCA.CState = &myCAS;



int i;
char ch;

Enum_BWInc * myEBW = new Enum_BWInc(myCA);

cout << "EnumBW created created";

myEBW->td=3;
myEBW->sd=1;
myEBW->prepare();

cout << "Starting Enumeration \n";

ofstream Reports;
Reports.open("EnumBW_report.txt");
myEBW->report_decent=true;
myEBW->report_sizes=true;
myEBW->report_stream= &Reports;



myEBW->max_state=7; // one less than Mazoyer is defined, ignore the illegal state

myEBW->calc();

cout << "Images for best Aconf " << myEBW->bestAbsets << "\n";

for (i=0; i<2* myEBW->td;i++) 
{ch = myCA.CState->display_table[myEBW->Abestconf[i]];
cout << ch;}

ofstream Aconfs;
 Aconfs.open ("aconfs_3_mazoyer.txt");

 for (i=0;i< myEBW->Bcountvals.size();i++)
 {Aconfs << myEBW->Bcountvals[i] << "\n";}
 Aconfs.close();
Reports.close(); 



 return 0;
}


int test_main8()
{
CellAut myCA;
OneD_direct myCAL(0);
// MazoyerFSSP myCAS;
// TwoStates myCAS(110);

// MooreFSSP myCAS;
Yunes1FSSP myCAS;
myCA.CALocation=  &myCAL;
myCA.CState = &myCAS;



BDD_Image * myBWBDD = new BDD_Image(myCA);

myBWBDD->check_BDD_representation();

myBWBDD->show_transition_BDD();
myBWBDD->dump_transition_BDD();

cout << myBWBDD->image_size_r(myBWBDD->TransitionResults);
return 0;
}

/*
int test_main9()
{

	
Config * MyCfg = new Config; 

MyCfg->Init(argv[1]);

	string toptag("CellAutBW");
    ca_analysis  * MyAlgorithm = CreateNamedAlgorithm(MyCfg,toptag); 
    MyAlgorithm -> calc();

	return 0;
}
*/


int test_main10()
{
	// test the breadth-first-search universal topology builder
CellAut myCA;
TwoD_neumann myCAL(0,0);myCA.CALocation=  &myCAL;


string targets_s = "(0,20) / (1,20) / (2,20) / (3,20) / (4,20)";
vector<CellAutLocation* > targets;
targets=location_list(&myCA,targets_s);
TopoIndexer my_topoix(&myCAL,5,targets,true);


my_topoix.report_structure(cout,5);

exit(0);
}