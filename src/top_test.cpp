

#include "ca.h"
#include <xercesc/util/PlatformUtils.hpp>
#include "configurator.h"

// #include <ostream>
#include <iostream>

using namespace std;

int test_main(int argc, char* argv[])
{

Config * MyCfg = new Config; 

MyCfg->Init(argv[1]);




string ca_topo;
string ca_dim;
string ca_trans;

string temp_complexity;
string monitor;
string method_step;

// E=Entity,A=Attribute-Name, V-Attribute Value which must match,
  // R-Result is Attribute Value (optional, if Format ends with A,
  // T-Text section is result
  bool error;
  vector<string> keys(4,"");
  keys[0] = "CellAutBW"; keys[1] = "CellAut";keys[2] = "Topology";keys[3]="type";
  ca_topo = MyCfg->GetDataByFormat("EEEA",keys,error);
  keys[3] = "dimension";
  ca_dim = MyCfg->GetDataByFormat("EEEA",keys,error);

  cout << ca_topo;

  return 0;
}
