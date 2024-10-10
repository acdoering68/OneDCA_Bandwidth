
#include "algorithm.h"


unsigned int ilog2(unsigned int a)
{
	unsigned int ah = a-1;
	unsigned int res = 0;
	while(ah){res++; ah=ah >>1;}
	return res;
}

ca_analysis::ca_analysis(CellAut ca) : theca(ca) {
state_bits = ilog2(ca.CState->state_num);
};




void ca_analysis::configure(Config * cfg,string toptag)
{
  bool error;
  std::istringstream stm;
  vector<string> keys(4,"");
  keys[0] = toptag; keys[1] = "Complexity";keys[2] = "temporal";
  stm.str(cfg->GetDataByFormat(string("EEA"),keys,error));stm >> td;
  
  string report_filename = "report" + toptag + "report.txt";
	report_stream = new ofstream(report_filename.c_str(),ios_base::out); // optionally | ios_base::app
    
   string location_list_s;
   keys[1] = "Shape";keys[2] = "targetcells";
   location_list_s = cfg->GetDataByFormat(string("EEA"),keys,error);
   vector<CellAutLocation* > target_cells = location_list(&theca,location_list_s);
   myTI = new TopoIndexer(theca.CALocation,td,target_cells,false); 
   // parameters to build the topoindexer:
	// expand,const  vector<CellAutLocation* > target_cells, bool incremental_target
  keys[0] = toptag; keys[1] = "Method";keys[2] = "Step";keys[3] = "verbosity";
  verbosity=0;
  stm.clear(); stm.str(cfg->GetDataByFormat(string("EEA"),keys,error));stm >> verbosity;

}

void ca_analysis::calc(){};
void ca_analysis::prepare(){};
void ca_analysis::report(string toptag){};


//return true, if already on last configuration
bool ca_analysis::next_conf(intcellstate* conf,int start, int end)
{
	int i=start;
	bool done;
	done = false;
	while (!done && i<end)
	{      // change here, if you introduce illegal states, which are not to be enumerated
		if (conf[i]<max_state-1)
		{conf[i]++;done = true;}
		else 
		{conf[i++]=0;}
	}
	return !done;
}

void ca_analysis::init_conf(intcellstate* conf,int start, int end)
{
	int i;
	for (i=start;i<end;i++) {conf[i]=0;}
}


map<string,ca_analysis_factory*> *
 ca_algorithm_directory::getDirectory()
{
static map<string,ca_analysis_factory*> algorithm_directory_inst;
return &algorithm_directory_inst;
}

ca_analysis* CreateNamedAlgorithm(Config * conf,string toptag)
{
CellAut * theca;
bool error;
vector<string> keys(4,"");

theca = CreateNamedCellAut(conf,toptag);

string MethodName;

keys[0] = toptag;

  keys[1] = "Method";keys[2] = "Step";keys[3]="type";
  MethodName = conf->GetDataByFormat("EEEA",keys,error);
  
 return  (*ca_algorithm_directory::getDirectory())[MethodName]->create(*theca);
}



just_run::just_run(CellAut ca) : ca_analysis(ca)   {name = "just_run";};

CA_ALGORITHM_FACTORY(just_run,"JustRun");

void just_run::configure(Config * cfg,string toptag)
{
  bool error;
	ca_analysis::configure(cfg,toptag);

	int squad_length;
	int i;
	int j;
	intcellstate ics;
	string starting_conf;
	std::istringstream stm;
	vector<string> keys(4,"");
	keys[0] = toptag; keys[1] = "Method";keys[2] = "Step"; keys[3]="Start";

	starting_conf = cfg->GetDataByFormat("EEEA",keys,error);


	myTI = new TopoIndexer(td);
	// for a topology-neutral solution get values from TI
	Aconf = new intcellstate[2*td];
	Cconf = new intcellstate[td];
	Bconf = new intcellstate[td];

	ics=0;
	for (j=0;j<theca.CState->state_num;j++)
	{
		if (theca.CState->display_table[j]=='_') {ics=j;}
	}
	for (i=0;i<td;i++)
	{Aconf[i]=ics;Aconf[td+i]=ics;Cconf[i]=ics;}

	if (starting_conf.size() > 0)
	{if (starting_conf[0] == '@')
	{ // just give length of synch field
		stm.str(starting_conf.substr(1));stm >>squad_length;
		if (squad_length > td-1) {cerr << "Squad length too large\n";exit(5);}
		for (j=0;j<theca.CState->state_num;j++)
		{if (theca.CState->display_table[j]=='$') {ics=j;}
		}
		Aconf[td]=ics;
		Aconf[td+squad_length]=ics;
		for (j=0;j<theca.CState->state_num;j++)
		{if (theca.CState->display_table[j]=='G') {ics=j;}}
		Aconf[td+squad_length-1]=ics;
	}else{
		for (i=0;i<starting_conf.size();i++)
		{
			for (j=0;j<theca.CState->state_num;j++)
			{
				if (theca.CState->display_table[j]==starting_conf[i])
				{Aconf[i+td]=j;}
			}
		}
	}
	}
	string report_filename = "report" + toptag + "report.txt";
	report_stream = new ofstream(report_filename.c_str(),ios_base::out); // optionally | ios_base::app
    // I do not call prepare here, as with inheritance I would call it more than once.
};

void just_run::prepare(){
	/* got to put this into configure, because I have to fill 
	   Aconf and Cconf

	myTI = new TopoIndexer(td);
	// for a topology-neutral solution get values from TI
	Aconf = new intcellstate[2*td];
	Cconf = new intcellstate[td];
	Bconf = new intcellstate[td];
*/

};
void just_run::calc()
{
calc_cone(myTI, &theca,Aconf,Cconf,cone_stor, td,sd,Bconf);

};
void just_run::report(string toptag){

int i;
int j;
string aline;
int cpg;
int maxll;
string spaces;
spaces.assign(3*td,' ');

for (i=0;i<td;i++)
{aline ="";cpg=myTI->cells_per_generation(td,i);
for (j=myTI->generation_start(td,i);j<myTI->generation_end(td,i);j++)
{
	aline.push_back(theca.CState->display_table[cone_stor[myTI->offsets_per_generation(td,i)+j]]);

}
if (i==0) {maxll = aline.size();}
(*report_stream) << spaces.substr(0,(maxll - aline.size())/2) << aline << "\n";
}

};



// for the abstract class just make empty output
// ostream& operator<<(ostream& os,const ca_analysis& ca) { return os;}

BDDcaSt::BDDcaSt(CA_BDD *BDDI) : BDDI(BDDI)
{

}

vector<BDD>  BDDcaSt::transition(vector< vector<BDD> > curr_state)
{
int i;
int j;
int state_bits = BDDI->state_bits;
vector<BDD> res;
for (i=0;i<state_bits;i++)
{
	res.push_back(BDDI->TransitionResults[i]);
	for (j=0;j<state_bits*(BDDI->theca.CALocation->neighbors()+1);j++)
	{res[i]= res[i].Compose(curr_state[j/state_bits][j%state_bits],j);
	}
}
return res;

}

CA_BDD::CA_BDD(CellAut ca) : ca_analysis(ca),  mgr(0,0) {
state_bits = ilog2(theca.CState->state_num);
 mgr.SetMaxCacheHard(16777216);
	unsigned int trans_in_bits = state_bits*(theca.CALocation->neighbors()+1);
	unsigned int i;
	unsigned int j;
	unsigned int ns;
	unsigned int h=0;
	ns=ca.CALocation->neighbors()+1;
	// !! attention: These must be the first variables created, 
	// as we use it's index later, 
	// if that is a problem, we have to get mgr->size and store it somewhere

	// have to call manager for each new variable
	for (i=0;i<trans_in_bits;i++)
	{TransitionInVars.push_back(mgr.bddVar());}
	for (i=0;i<state_bits;i++)
	{TransitionResults.push_back(BDD(mgr.bddZero()));}

	BDD temp_condition;

	// create BDD from abstract transition function
	// though it is exaggeration, use Enum_BW to get 
	// the next_conf method:

	
	max_state = theca.CState->state_num;
	intcellstate * base_conf = new intcellstate[ns];
	
     init_conf(base_conf,0,ns);

	intcellstate res;
	vector<intcellstate> state_vec(ns,0);
	while (true) // will end by break
	{
		// cout << "\n" << h++ << ":";
		for (i=0;i<ns;i++) {state_vec[i]=base_conf[i];}
		theca.CState->transition(state_vec);
		res = ca.CState->as_int();
		temp_condition  = mgr.bddOne();
		if (res) // only if any result bit non-zero, do something
		{
			for (i=0;i<trans_in_bits;i++)
			{
				if (base_conf[i/state_bits] & (1<<(i%state_bits)))
				{
				temp_condition = temp_condition * TransitionInVars[i];
				}else
				{
				temp_condition = temp_condition * (!TransitionInVars[i]);
				}
			}
			// cout << res << "->";
			for (i=0;i<state_bits;i++)
			{if (res & (1<<i))
			{ 
				// cout << i << "S";
				TransitionResults[i] = TransitionResults[i] + temp_condition;
				// cout << TransitionResults[i].nodeCount() << " ";
			}
			}
		}
		if (next_conf(base_conf,0,ns)) {break;}
	}
	theBDDca = new BDDcaSt(this);
	// thandle2=start_time();
	/*
        if (verbosity>4) // not set yet
	  {
	    cout << " Transition function as BDD" << endl; 
 	      for (i=0;i<state_bits;i++)
	      {TransitionResults[i].print(state_bits*ns,4);
	      }
	  }
	*/


}

void CA_BDD::prepare()
{
int num_Avars = state_bits *  myTI->rcells_over_distance_acc[td];
int num_Cvars = state_bits *  myTI->scells_over_distance_acc[td];

	int i;
	 // I should experiment whether it is better if A goes first or C, 
	 // I am not usre, what variable ordering means and whether variables with 
	 // low index go on top or bottom.

	Avar_index_start = mgr.ReadSize(); // index our first a-var will get
	 // this is not zero as we needed already variables for 
	 // the conversion of the transition function into BDDs
	for (i=0;i<num_Avars;i++)
	{Avars.push_back(mgr.bddVar());}
	Cvar_index_start = mgr.ReadSize();
    for (i=0;i<num_Cvars;i++)
	{Cvars.push_back(mgr.bddVar());}

	vector<BDD> h(state_bits,mgr.bddOne());
	BDD_cone.assign(myTI->offsets_per_generation(td,td)+myTI->generation_end(td,td),h);
}

void CA_BDD::build_BDD_cone()
// void BDD_Image::BDD_cone(TopoIndexer * TI, CellAut * ca,vector<vector<BDD> > BDD_cone,const vector<BDD> Avars,const vector<BDD> Cvars, vector<BDD> Bexprs, int depth, int width,)
{
	int i;
	int j;
	int ii;
	int n;
	int s;
	int ns;
	ns = theca.CALocation->neighbors()+1;
	vector<vector<BDD> > state_vec(ns);
	int this_row_startix;
	int next_row_startix;
	ii=0;for (i=0;i<myTI-> scells_over_distance_acc[td];i++)  {for (j=0;j<state_bits;j++) {BDD_cone[myTI->baseline_s(i)+(myTI->offsets_per_generation(td,0))][j]=Cvars[ii++];}}  
	ii=0;for (i=0;i<myTI-> rcells_over_distance_acc[td];i++){for (j=0;j<state_bits;j++) {BDD_cone[myTI->baseline_r(i)+(myTI->offsets_per_generation(td,0))][j]=Avars[ii++];}}  
	for (i=0;i<=td-1;i++)
	{   this_row_startix = myTI->offsets_per_generation(td,i);
	next_row_startix = myTI->offsets_per_generation(td,i+1);
	for (s=myTI->generation_start(td,i+1);s< myTI->generation_end(td,i+1);s++)
	  {    cout << this_row_startix <<  "+[";
        const int * nb_ixs = myTI->nb_ix(td,i,s);
		for (n=0;n<ns;n++) {
			cout << nb_ixs[n] << " ";
			state_vec[n]=BDD_cone[this_row_startix+nb_ixs[n]];
			// cout << theca.CState->display_table[state_vec[n]];
		}
		cout << "]->" << next_row_startix+s << endl;
		BDD_cone[next_row_startix+s] = theBDDca->transition(state_vec);
		// cout << "->" <<  theca.CState->display_table[BDD_cone[next_row_startix+s]] << next_row_startix+s << "]";
		// cout << "->" ;
	}
	// cout << "\n";
	// get result if required
// now separate function
/*
	if (Bconf)
	{    int i=0; 
	for (s=myTI->generation_start(td,td);s< myTI->generation_end(td,td);s++)
	{for (j=0;j<state_bits;j++) 
	{Bexprs[i++]=BDD_cone[myTI->offsets_per_generation(td,td)+myTI->baseline_r(s)][j];
	}
	}
	}
*/

	}

}

// for many algorithms, we only need the BDD for the top level, 
// setting the variables in the lower levels to zero might reduce the size of the BDD hash tables (in the manager)
void CA_BDD::BDD_cone_clean_lowlevels()
{
	int i;
        int j;
	int s;

	int next_row_startix;
	for (i=0;i<=td-2;i++)
	{ 
	  next_row_startix = myTI->offsets_per_generation(td,i+1);
	  for (s=myTI->generation_start(td,i+1);s< myTI->generation_end(td,i+1);s++)
	  { 
            for (j=0;j<state_bits;j++) 
	       BDD_cone[next_row_startix+s][j] = mgr.bddZero();
	  }
	}

}
