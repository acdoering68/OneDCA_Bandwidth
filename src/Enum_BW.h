#ifndef Enum_BW_h
#define Enum_BW_h


#include "algorithm.h"
#include "conf_set.h"
// add __STDC__ ??
// #include "cuddObj.hh"
#include "TopoIndexer.h"
#include "Cone.h"
#include "monitor.h"

#include <iostream>
#include <map>
// this is needed for dumping BDDs
 // if it only existed .. 
// #include <stdiostr>

#include <stdio.h>

 // for faster and more compact storage of the decendance 
 // I use 
#define inthash_decendance 1


// basic enumeration and 
// ((((.. 1+1)+1)+1...) based incremental enumeration

class Enum_BW : virtual public ca_analysis
{
public:
	// string name; //const 
    // Enum_BW();
	void calc();
    Enum_BW(CellAut ca);
	  // inherited from ca_analysis
	// int td; /* temporal depth */
	// int sd; /* spacial depth */ 
    
    // vector<bool> sort_policy; // sort after n-th depth 
    bool keep_aconfs; // if false, sorting is not used
	// static const int discard_maxth=10000; 
	// unsigned int discard_threshold; // 
    unsigned long int bestAbsets;
	unsigned long int bestA1bsets; // for depth = 1, needed for cutoff
	void prepare();
	void report(string toptag);
	intcellstate* Abestconf;
    vector<unsigned int> Bcountvals;

	// time monitoring
	int thandle1; int thandle2;

	bool dump_bsets;
	string Bsetdump;

    


    void configure(Config * cfg,string toptag);
// private:
protected :
	 intcellstate* Aconf;
	 intcellstate* Cconf;
     intcellstate* Bconf;
	 
	 unsigned int max_compress_length;
	 unsigned long long compress_statevec(const intcellstate* conf,const unsigned int length);
	 //     unsigned long long Enum_BW::compress_statevec(const intcellstate* conf,const unsigned int length);
	 // intcellstate* maxAconf;
     
	 intcellstate cone_stor[1200]; // be a bit generous

	 int current_depth;

	 void copy_aconf();
	
     
	 bool enum_some(int steps); /* to allow interactive stopping, etc. */
	 void enum_start(); /* set Aconf/Cconf to start */
	  // after setting td, sd, before running enumeration

	 
      

	 Conf_set * encBsets; // which one will be configured  	
     
     // intcellstate state_num;
	unsigned int * cone_ops_per_depth;
	  // even though this is not needed for basic enumeration, 
	  // we need it here, in order to use basic enumeration for preparation
	  // of the incremental one
	 vector<vector<intcellstate* > > new_incremental_history;
};

// incremental enumeration is so close to basic enumeration, 
// that I rather inherit it

class Enum_BWInc : public  Enum_BW
{
public :
	Enum_BWInc(CellAut ca);
	void calc();
    void prepare();
	void configure(Config * cfg,string toptag);
	void report(string toptag);
	
	void incremental_start();
    bool incremental_some(int steps);


	static const int discard_maxth=10000; 
	unsigned int discard_threshold; // 
    unsigned int low_registration_bound;
	unsigned int cut_off_bound;
	
	
	// when using compression
	 // could make a little class to distinguish the variants
	// vector<vector<unsigned long long int> > incremental_history; 
   
    void clear_history();
	vector<vector<intcellstate* > > prev_incremental_history;
	int current_history_Bcount;
	int current_history_conf;

     
	// time monitoring
	int thandle3;
	
     // some monitoring:
/*  inherited from Enum_BW
	bool keep_aconfs; // if false, sorting is not used
	unsigned long int bestAbsets;
	unsigned long int bestA1bsets; // for depth = 1, needed for cutoff
*/	
	
	bool report_decent;
	bool report_decent_accumulated;
	bool keep_history;
    bool report_sizes;
	bool create_illustration;
    bool next_aconf(); // based on history etc.
	                   // also needed for BDD-based method
 

    vector<vector<vector<intcellstate* > > > all_time_history;
#ifdef inthash_decendance
	typedef map<unsigned int,unsigned int> Tdecendance;
	Tdecendance decendance;
#else 
	vector<vector<vector<unsigned int> > > decendance;
#endif
	vector<unsigned int> confs_per_generation; 
	 



};

inline unsigned int decendance_index(int generation,int prev_bres,int bres) {return (generation << 20) | (prev_bres << 10) | bres;}





class BDD_Image : public Enum_BWInc, public CA_BDD
{
public:
// cudd specific:


  // string name;
  BDD_Image(CellAut ca);
  void calc();
  void prepare();
  void report(string toptag);
  void configure(Config * cfg,string toptag);


  void show_transition_BDD();
  void dump_transition_BDD();
  void check_BDD_representation();
 // could be private, but for testing want to have it visible,
//  needs to be class member for the manager
 // since I have now a separate library for those, 
 // I should use the lib and not replicate it here ...
  unsigned long int image_size_r(vector<BDD> multi_bit_function);
  unsigned long int image_size_h(vector<BDD> multi_bit_function,int i,BDD current_condition);
  unsigned long int image_size_hr(vector<BDD> multi_bit_function,int i,BDD current_condition,int p);
  // new method 
  // uns

/*
  vector<BDD> TransitionResults;
  // but needed by BDDcaSt:
  friend class BDDcaSt;
*/

  bool only_build_cone;

private:
  // different from Enum_BW!
bool enum_some(int steps);

unsigned int state_bits;
      // the stuff from basic enumeration which I also need when using BDD-Image
     // is inherited from Enum_BW
    /*
    TopoIndexer * myTI; 
    intcellstate* Aconf;
    ostream* report_stream; 
  */
/*
  vector<BDD> Avars;
  vector<BDD> Cvars;
  vector<BDD> Bexprs;
  vector<BDD> Bapplied;

  int Avar_index_start;
  int Cvar_index_start;

  vector<vector<BDD> > BDD_cone; // two dimensions as we need several bits for each state
  void build_BDD_cone();
*/
  vector<BDD> FindBconf(int current_depth);

  void ApplyAconf();

 

  
// first: need the transition function as BDD, 
// this requires the input nodes and the the result nodes

// vector<BDD> TransitionResults;

};






#endif
