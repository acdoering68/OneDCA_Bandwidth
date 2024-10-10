#ifndef algorithm_h
#define algorithm_h

#include "ca.h"
#include <string>
#include <iostream>
#include "configurator.h"
#include "TopoIndexer.h"
#include "Cone.h"

#include "cuddObj.hh"


unsigned int ilog2(unsigned int a);

class ca_analysis
{
public: 
	ca_analysis(CellAut ca);
    /* const */ string name;
    int td; /* temporal depth */
	int sd; /* spacial depth */
	intcellstate max_state; /* make this public, cause sometimes we want to exclude some states from processing */ 
    CellAut theca;
	unsigned int state_bits; // frequently needed
    virtual void configure(Config * cfg,string toptag); 
	virtual void prepare();
    virtual void calc();
    virtual void report(string toptag);

	ofstream* report_stream; 
    int verbosity;

	    // these are too useful to make them protected or private
	// not only used for enumeration algorithms, but also for conversion to BDD
	 bool next_conf(intcellstate* conf,int start, int end);
	 void init_conf(intcellstate* conf,int start, int end);
	 TopoIndexer * myTI; 
};

// factory mechanism:

class ca_analysis_factory
{
public:
	virtual ca_analysis* create(CellAut ca)=0;
};

class ca_algorithm_directory
{
public:
static map<string,ca_analysis_factory*> * getDirectory();
};

template <class c_a>
class ca_analysis_factory_builder
{
public:
	ca_analysis_factory_builder<c_a>(string name){ 
		(*ca_algorithm_directory::getDirectory())[name]= new c_a();}
};
#define CA_ALGORITHM_FACTORY(classname,confname) class  classname ##_factory : public ca_analysis_factory {public:  classname  * create(CellAut ca) {return  new classname (ca);} }; \
	static  ca_analysis_factory_builder<classname ##_factory> factory_## classname ##_inst(confname);
//	classname::classname() : ca_analysis("#classname"){}


class just_run : public ca_analysis
{
public:
	just_run(CellAut ca);
	virtual void configure(Config * cfg,string toptag); 
	virtual void prepare();
    virtual void calc();
    virtual void report(string toptag);

	intcellstate* Aconf;
	intcellstate* Cconf;
    intcellstate* Bconf;
	TopoIndexer * myTI; 
	intcellstate cone_stor[12000];

	ofstream* report_stream; 
};

class CA_BDD;

class BDDcaSt  
{
public:
  BDDcaSt(CA_BDD* BDDI);

  
  vector<BDD> transition(vector< vector<BDD> > curr_state); 

private:
  CA_BDD* BDDI;
  // vector< vector<BDD> > generation_BDD; // will be indexed by intcellstate
  //                                      // two-dim, as we need several bits to represent the state
 
};

class CA_BDD : virtual public ca_analysis
{
public:
CA_BDD(CellAut ca);
// "factory" function for CA's

    // virtual void configure(Config * cfg,string toptag); 
	virtual void prepare();
   //  virtual void calc();
	// could add timing information for cone construction
   // virtual void report(string toptag);



Cudd mgr; // one joint CUDD Manager
BDDcaSt* theBDDca;

vector<BDD> TransitionResults;
vector<BDD> Avars;
  vector<BDD> Cvars;
  vector<BDD> Bexprs;
  vector<BDD> Bapplied;

  int Avar_index_start;
  int Cvar_index_start;

  vector<vector<BDD> > BDD_cone; // two dimensions as we need several bits for each state
  vector<BDD> TransitionInVars;

  void build_BDD_cone();
  void BDD_cone_clean_lowlevels();
};





ca_analysis* CreateNamedAlgorithm(Config * conf,string toptag);





#endif
