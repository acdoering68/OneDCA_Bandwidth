
/* This is the basic enumeration based algorithm. 
Though it is terribly slow it is the base of many of my other algorithms
*/


#include "Enum_BW.h"
// for debugging
#include <iostream>
#include <cmath>
// #include <ostream>

// : name("Enum_BW")
Enum_BW::Enum_BW(CellAut ca)  : ca_analysis(ca)  {name = "Enum_BW";};
Enum_BWInc::Enum_BWInc(CellAut ca)  : ca_analysis(ca), Enum_BW(ca)  {name = "Enum_BWInc";};

// factory macro: class name, string for config file
CA_ALGORITHM_FACTORY(Enum_BW,"FullEnumeration");

void Enum_BW::enum_start()
{
	init_conf(Aconf,0,2*td);
	init_conf(Cconf,0,td);
}



void Enum_BW::copy_aconf()
{
	int j;
	intcellstate * oldaconf;
	oldaconf = Aconf;
	Aconf = new intcellstate[2*td];
	for (j=0;j<2*current_depth;j++) {Aconf[j]=oldaconf[j];}

}
unsigned long int ipow(unsigned long int p,unsigned long int e)
{
	unsigned long int res=1; 
	unsigned long int ph=p;
	unsigned long int eh=e;
	while (e)
	{if (e&1) {res=res*ph;}
	ph=ph*ph; e = e>>1;}
	return res;
}

void Enum_BWInc::clear_history()
{
	int i;
	int j;
	if (keep_history)
	{
		all_time_history.push_back(new_incremental_history);
	}
		for (i=0;i<new_incremental_history.size();i++)
		{for (j=0;j<new_incremental_history[i].size();j++)
		{if (new_incremental_history[i][j]) {delete  new_incremental_history[i][j];}}
		new_incremental_history.clear();
		}
	new_incremental_history = vector<vector<intcellstate *> >(ipow(max_state,td));
}




void Enum_BW::prepare()
{

	/* build up index structures for the geometry 
	and 

	*/

	myTI = new TopoIndexer(td);
	bestAbsets =0;


	// for a topology-neutral solution get values from TI
	Aconf = new intcellstate[2*td];
	Cconf = new intcellstate[td];
	Bconf = new intcellstate[td];
	Abestconf = NULL; bestAbsets=0;
	encBsets = new Conf_set_hash();
	encBsets ->size = td;
	encBsets ->clear();
	current_depth = td;
	// can be overwritten before starting calculation
	max_state = theca.CState->state_num;

    

	// Bcountvals.clear();
	Bcountvals.assign(ipow(max_state,td),0);

	
	long long int h;
	max_compress_length = 8*sizeof(h)/state_bits;

    cone_ops_per_depth = new unsigned int[td];
	int i;
	for (i=0;i<td;i++){cone_ops_per_depth[i]=0;} 
	
}


void Enum_BW::configure(Config * cfg,string toptag)
{
  bool error;
	ca_analysis::configure(cfg,toptag);
  
	std::istringstream stm;
    vector<string> keys(4,"");
    keys[0] = toptag; keys[1] = "Method";keys[2] = "Step"; 
  
   keys[3]="DumpBsets";
   dump_bsets = ('y' == cfg->GetDataByFormat("EEEA",keys,error)[0]);
  
    Bsetdump = "";
	
}

CA_ALGORITHM_FACTORY(Enum_BWInc,"DepthIncrementalEnumeration");
void Enum_BWInc::prepare()
{   // inherited!
	Enum_BW::prepare();
	int hsize;
	int i;
#ifndef inthash_decendance
	vector<unsigned int> h(ipow(max_state,td),0);
	vector<vector<unsigned int> > hh(ipow(max_state,td),h);
#endif
    if (report_decent_accumulated)
	{
#ifndef inthash_decendance
			decendance.assign(td-1,hh);
#endif
 // I think the hash table does not need to be initialized
	}
    

	/*
    cone_ops_per_depth = new unsigned int[td];
	int i;
	for (i=0;i<td;i++){cone_ops_per_depth[i]=0;} 
	*/
}

void Enum_BWInc::configure(Config  * cfg,string toptag)
{
        bool error;
	Enum_BW::configure(cfg,toptag);
/*
bool report_decent;
	bool report_decent_accumulated;
	bool keep_history;
    bool report_sizes;
	*/

  std::istringstream stm;
  vector<string> keys(4,"");
  keys[0] = toptag; keys[1] = "Method";keys[2] = "Step"; 
  
   keys[3]="AccumulatedDescent";
   report_decent_accumulated = ('y' == cfg->GetDataByFormat("EEEA",keys,error)[0]);
   
   keys[3]="DisplayHistory";
   keep_history = ('y' == cfg->GetDataByFormat("EEEA",keys,error)[0]);
    report_sizes = report_decent_accumulated;
   keys[3]="CreateIllustration";
   create_illustration = ('y' == cfg->GetDataByFormat("EEEA",keys,error)[0]);
}


unsigned long long int Enum_BW::compress_statevec(const intcellstate* conf,const unsigned int length)
{
	long long int res=0;

	int i;

	assert(length <= max_compress_length);
	for (i=0;i<length;i++)
	{res = (res<<state_bits) | (conf[i] & state_bits-1);}
	return res;
}


void Enum_BW::calc()
{
	int i;
	thandle1=start_time();
		enum_start();
		keep_aconfs = false;
		while (!enum_some(1000)) {}// cout << ".";
		thandle2=start_time();
};

void Enum_BWInc::calc()
{
	int i;
	thandle1=start_time();
// first do standard enumeration with depth =1
		enum_start(); // need both, as we start with a normal enumeration
		incremental_start();
		keep_aconfs = true;
		current_depth=1;
		encBsets->size=1;
		new_incremental_history = vector<vector<intcellstate *> >(ipow(max_state,td));
		while (!enum_some(1000)) {}
		thandle2=start_time();
		bestA1bsets = bestAbsets; 
		current_depth=2;encBsets->size=2;
		prev_incremental_history = new_incremental_history;
		clear_history();
		current_history_Bcount=bestAbsets;
		current_history_conf=0;bestAbsets=0;
		for (i=0;i<current_depth*2-2;i++){Aconf[i]=prev_incremental_history[current_history_Bcount][current_history_conf][i];};
		init_conf(Aconf,current_depth*2-2,current_depth*2);
		init_conf(Cconf,0,                current_depth);
		while (!incremental_some(1000)){};
		thandle3=start_time();
		if (keep_history){all_time_history.push_back(new_incremental_history);}
}    


bool Enum_BW::enum_some(int steps)
{
	int i=0;
	int j;
	while(i<steps)
	{
		cone_ops_per_depth[current_depth]++;
		calc_cone(myTI, &theca,Aconf,Cconf,cone_stor, current_depth,sd,Bconf);
		encBsets->add(Bconf);
		i++;



		if (next_conf(Cconf,0, current_depth))
		{ // cout << ":"; cout << flush;


			// register Aconf, result
			if (keep_aconfs)
			{
				new_incremental_history[encBsets->count].push_back(Aconf);
				copy_aconf();
			}

			// Bcountvals.push_back(encBsets->count);
			Bcountvals[encBsets->count]++;
			// cout << encBsets->count << "\n";
			if (bestAbsets < encBsets->count)
			{    // cout << "<";
			  // I have a memory management problem ... 
			  // if (Abestconf) {delete Abestconf;}
				Abestconf = Aconf;
				Aconf = new intcellstate[2*td];
				for (j=0;j<2*current_depth;j++) {Aconf[j]=Abestconf[j];}
				bestAbsets = encBsets->count;
			}
			if (dump_bsets)
			{
				if (Bsetdump != "") {Bsetdump = Bsetdump + ";\n";}
				Bsetdump = Bsetdump + theca.CState->conf2string(Aconf,2*current_depth) + " :" + encBsets->dump(&theca);
			}

			encBsets->clear();

			init_conf(Cconf,0,current_depth);
			if (next_conf(Aconf,0,2*current_depth))
			{
				return true; // done
			}
		}
	}
	return false;
}



void Enum_BW::report(string toptag)
{

	int i;
	char ch;
(*report_stream) << "<RESULT name=\"" << toptag << "\"\n";
(*report_stream) << "<Explanation>Images for best Aconf</Explaination>" 
                 <<  "<BAC n=\"" << bestAbsets << "\" v=\"";

for (i=0; i<2* td;i++) 
{ch = theca.CState->display_table[Abestconf[i]];
(*report_stream) << ch;}

(*report_stream) << "\"/>\n";

ofstream Aconfs;
 Aconfs.open ("aconfs_3_mazoyer.txt");
 

 for (i=0;i< Bcountvals.size();i++)
 {Aconfs << Bcountvals[i] << "\n";}
 Aconfs.close();
// Reports.close(); 


if (dump_bsets)
			{
				(*report_stream) << "<BSETS>" << Bsetdump << "</BSETS>";
}

 (*report_stream) << "<TIME>" << result_time(thandle1,thandle2) << "</TIME>";
 (*report_stream) << "</RESULT>";
report_stream->close();
}

void Enum_BWInc::incremental_start()
{
	init_conf(Aconf,0,2*td);
	init_conf(Cconf,0,td);
	current_depth = 1;
	clear_history();
	low_registration_bound = 1; // only register if found more then this
	cut_off_bound=2;
}

bool Enum_BWInc::next_aconf()
{
	int j=0;
	int i;
	// unsigned long int Bres;
	unsigned long long int Acompressed;
	if (next_conf(Aconf,2*current_depth-2,2*current_depth))
	{

		// if we stay with the same bandwidth, we do not need to test for completion   
		if (prev_incremental_history[current_history_Bcount].size()<= ++current_history_conf)
		{
			current_history_Bcount--;current_history_conf=0;
			// we could have empty slots in the history
			while (prev_incremental_history[current_history_Bcount].size()==0 && current_history_Bcount>= cut_off_bound) 
			{current_history_Bcount--;}
			if (current_history_Bcount< cut_off_bound)
			{ // done with this depth
				if (report_sizes)
				{ *report_stream  << "<generation>";
				for (j=0;j<new_incremental_history.size();j++)
				{if (new_incremental_history[j].size() != 0)
				{(*report_stream) << j << ":" << new_incremental_history[j].size() << "\n";}
				}
				(*report_stream) << "</generation>";
				}
				if (current_depth<td)
				{encBsets->size=++current_depth;
				cout << "!" << current_depth << "\n";
				prev_incremental_history = new_incremental_history;
				clear_history();
				
				current_history_Bcount=bestAbsets;
				current_history_conf=0;
				for (i=0;i<current_depth*2-2;i++){Aconf[i]=prev_incremental_history[current_history_Bcount][current_history_conf][i];};
				init_conf(Aconf,current_depth*2-2,current_depth*2);
				init_conf(Cconf,0,                current_depth);
				// and so on, will be glad if I can go one step
				}else{return true;}
			}
		}
		for (i=0;i<current_depth*2-2;i++){Aconf[i]=prev_incremental_history[current_history_Bcount][current_history_conf][i];}
		init_conf(Aconf,current_depth*2-2,current_depth*2);
		return false; 
	}
	return false;
}

bool Enum_BWInc::incremental_some(int steps)
{
	int i=0;
	int j=0;
	unsigned long int Bres;
	unsigned long long int Acompressed;
	while(j<steps)
	{
		cone_ops_per_depth[current_depth]++;
		calc_cone(myTI, &theca,Aconf,Cconf,cone_stor, current_depth,sd,Bconf);
		j++;
		if (next_conf(Cconf,0, current_depth))
		{	
			Bres = encBsets->count;
			if (Bres > low_registration_bound)
			{new_incremental_history[Bres].push_back(Aconf);copy_aconf();
			if (false)
			// if (report_decent) 
			  { *report_stream  << "<decent>  (" << current_history_Bcount 
			     << "," << current_history_conf << ") -> (" 
		         << Bres << "," << new_incremental_history[Bres].size() 
			     << ") </decent>\n"; 
			}
			if (report_decent_accumulated)
			{
#ifdef inthash_decendance
				decendance[decendance_index(current_depth-2,current_history_Bcount,Bres)]++;
#else
				decendance[current_depth-2][current_history_Bcount][Bres]++;
#endif
			}
			}
			// Acompressed = 
			// register Aconf, result
			// Bcountvals.push_back(encBsets->count);
			// cout << encBsets->count << "\n";
			if (bestAbsets < encBsets->count)
			  {    
			  cout << "<" << current_depth << " " <<Bres << "\n";
			  if (Abestconf) {delete Abestconf;}
			  Abestconf = Aconf;copy_aconf();
			  bestAbsets = Bres; cut_off_bound = bestAbsets / bestA1bsets;
			  if (cut_off_bound < 2) {cut_off_bound = 2;}
			  }
			encBsets->clear();

			init_conf(Cconf,0,current_depth);
			if (next_aconf()) {return true;}
		}else{ // just counted the current Aconf up, adding it to the Bsets register
			encBsets->add(Bconf);
		}
	}
	return false;
}

string svg_header(int td,int max_img)
{
ostringstream oss;
return 
string("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n") + 
string("<!-- Created with Cellular Automata Bandwidth Determination by Andreas Doering -->\n") +
string("<svg \n") + 
   string("xmlns:dc=\"http://purl.org/dc/elements/1.1/\"\n") +
   string("xmlns:svg=\"http://www.w3.org/2000/svg\"\n") +
   string("xmlns=\"http://www.w3.org/2000/svg\"\n") +
     // later calc an appropriate size here! 
   string("width=\"297mm\"\n") +
   string("height=\"210mm\"\n") +
   string("id=\"svg1306\">");
}

const string svg_closer = string("</svg>");


const int svg_generation_space =100;


void Enum_BWInc::report(string toptag)
{

	int max_img; // for graphics generation
	// Enum_BW::report(toptag);
		int i;
		int j;
		int k;
		int count_nonzero;
	char ch;

	unsigned int generation; unsigned int pBres; unsigned int nbres;
(*report_stream) << "<RESULT name=\"" << toptag << "\"\n";
(*report_stream) << "<Explanation>Images for best Aconf</Explaination>" 
                 <<  "<BAC n=\"" << bestAbsets << "\" v=\"";

for (i=0; i<2* td;i++) 
{ch = theca.CState->display_table[Abestconf[i]];
(*report_stream) << ch;}

(*report_stream) << "\"/>\n";
/*
ofstream Aconfs;
 Aconfs.open ("aconfs_3_mazoyer.txt");
 

 for (i=0;i< Bcountvals.size();i++)
 {Aconfs << Bcountvals[i] << "\n";}
 Aconfs.close();
 */
// Reports.close(); 

 if (keep_history)
 {
	 (*report_stream) << "<HISTORY>";
	 for (i=1;i<td+1;i++) 
	 {
         (*report_stream) << "<GENERATION n=\"" << i << "\">";
		 for (j=0;j<all_time_history[i].size();j++)
		 {(*report_stream) << all_time_history[i][j].size() << " ";}
		 (*report_stream) << "</GENERATION>";
	 }

	 (*report_stream) << "</HISTORY>";
 }
 if (report_decent_accumulated)
 // if (false)
 {
	 (*report_stream) << "<DECENT>";
#ifdef inthash_decendance
	 Tdecendance::iterator p;
     for(p = decendance.begin();p != decendance.end();p++)
	 {
        
		generation = (p->first>>20); pBres = (p->first >> 10) & ((1<<10)-1); 
		nbres =  p->first & ((1<<10)-1); 
			(*report_stream) << generation  << " "  <<  pBres << " "  << nbres << " " << (p->second) << "\n";
	 }
#else


    for (i=0;i<td-1;i++)
	{
		for (j=0;j<decendance[i].size();j++)
		{
			for (k=0;k<decendance[i][j].size();k++)
			{
				if (decendance[i][j][k] != 0)
				{(*report_stream) << i << " " << j << " " << k << " " << decendance[i][j][k] << "\n";}
			}
		}
	}
#endif
(*report_stream) << "</DECENT>";

 }

if (dump_bsets)
			{
				(*report_stream) << "<BSETS>" << Bsetdump << "</BSETS>";
}

(*report_stream) << "<TIME job=\"FirstGenerationEnumeration\">" << result_time(thandle1,thandle2) << "</TIME>";
(*report_stream) << "<TIME job=\"RemainingIncrementalEnumeration\">" << result_time(thandle2,thandle3) << "</TIME>";
 (*report_stream) << "</RESULT>";
report_stream->close();

 
if (create_illustration && keep_history)
{
// reuse report stream
	string report_filename = "report" + toptag + "_illustration.svg";
	report_stream = new ofstream(report_filename.c_str(),ios_base::out); 
    (*report_stream) << svg_header(td,20); // for now
     // make the annotation of the generations
	(*report_stream) << "<g>"; 
    for (i=1;i<td+1;i++) 
	  {
       (*report_stream) << "<text font-family=\"serif\" font-size=\"12pt\" fill=\"black\" x=\"20\" y=\"" << i*svg_generation_space+10 
		                << "\"> " << i << " </text>\n";
	}
    (*report_stream) << "</g>\n"; 
	 // now the "dots" for each configuration, optionally add a string with one example configuration 
	 // again as group
	(*report_stream) << "<g>\n"; 
	for (i=0;i<td+1;i++)
	{ count_nonzero=0;
      for (j=0;j<all_time_history[i].size();j++)
	  {
		  if (all_time_history[i][j].size() != 0)
		  {
              (*report_stream) << "<circle cx=\"" << (count_nonzero*50+60) << "\" cy=\"" << i*svg_generation_space+10 
				               <<  "\" fill=\"black\" r=\"" << int(sqrt(log(double(all_time_history[i][j].size()))+1.0))*3 << "\"/>\n"; 
			  count_nonzero++;
		  }
	  }
	}
   (*report_stream) << "</g>"; 
   // now the decendance lines, if we have them. 
    if (report_decent_accumulated)
 {
   (*report_stream) << "<g>\n"; 
#ifdef inthash_decendance
	 Tdecendance::iterator p;
     for(p = decendance.begin();p != decendance.end();p++)
	 {
        (*report_stream) << " <line stroke-width=\"2\" stroke=\"blue\" x1=\"";
		  generation = (p->first>>20); pBres = (p->first >> 10) & ((1<<10)-1); 
		  nbres =  p->first & ((1<<10)-1); 
		    // find geometrical position of start: need to skip 0 entries until we found ours
		   count_nonzero=0;
		  for (j=0;j<all_time_history[generation+1].size() && j<pBres;j++)
		    {if (all_time_history[generation+1][j].size() != 0) {count_nonzero++;}}
        (*report_stream) << (count_nonzero*50+60) << "\" y1=\"" << (generation+1)*svg_generation_space+10 <<"\" x2=\"";
         count_nonzero=0; 
		 for (j=0;j<all_time_history[generation+2].size() && j<nbres;j++)
		    {if (all_time_history[generation+2][j].size() != 0) {count_nonzero++;}} 
		  (*report_stream) << (count_nonzero*50+60) << "\" y2=\"" << (generation+2)*svg_generation_space+10 <<"\"/>\n";
	 }
#endif
	 (*report_stream) << "</g>"; 
	}


/*	for (i=0;i<td+1;i++)
	{ count_nonzero=0;
      for (j=0;j<
  */ 
   (*report_stream) << svg_closer;	 
   report_stream->close();

}
};


BDD_Image::BDD_Image(CellAut ca) : ca_analysis(ca), Enum_BWInc(ca) , CA_BDD(ca)
{
	name = "BDD_Image";
     thandle1=start_time();
	 // init_conf(base_conf,0,ns);
	// convert the transition function into a BDD
	
};

void BDD_Image::show_transition_BDD()
{

	int i;
cout << "BDD of transition function\n";
for (i=0;i<TransitionResults.size();i++)
{
cout << "result bit " << i << " ";
TransitionResults[i].print(2);
TransitionResults[i].PrintCover();
}

};


void BDD_Image::dump_transition_BDD()
{
	int i;
	 

FILE * uDrawGraph_out;
uDrawGraph_out = fopen("Dump_Transition_BDD.udg","w");

FILE * FactoredForm_out;
FactoredForm_out = fopen("Dump_Transition_BDD.faf","w");

// BDDvector has been removed from cudd > 3.0
// BDDvector v = TransitionResults[0].CharToVect();
std::vector<BDD> v = TransitionResults[0].CharToVect();
mgr.DumpDaVinci(v,NULL,NULL,uDrawGraph_out);
mgr.DumpFactoredForm(v,NULL,NULL,FactoredForm_out);


fclose(uDrawGraph_out);
fclose(FactoredForm_out);
};

void BDD_Image::check_BDD_representation()
{

 vector<BDD> CheckTransitionResults;

unsigned int trans_in_bits = state_bits*(theca.CALocation->neighbors()+1);
	unsigned int i;
	unsigned int j;
	unsigned int ns;
	unsigned int h=0;
	ns=theca.CALocation->neighbors()+1;

    int * invals = new int[trans_in_bits];

    
	max_state = theca.CState->state_num;
	intcellstate * base_conf = new intcellstate[ns];
	init_conf(base_conf,0,ns);


	for (i=0;i<state_bits;i++)
	{CheckTransitionResults.push_back(BDD());}

	intcellstate res;
	intcellstate BDDres;
	int BDDbitres;
	vector<intcellstate> state_vec(ns,0);
	while (true) // will end by break
	{
		BDDres = 0;
		for (i=0;i<ns;i++) {state_vec[i]=base_conf[i];}
		theca.CState->transition(state_vec);
		res = theca.CState->as_int();

		for (i=0;i<trans_in_bits;i++)
		 {
		 	// I write it this way, to underline the relation of the definition of 
			// Cudd_Eval
			invals[i] = (base_conf[i/state_bits] & (1<<(i%state_bits)))?1:0;
		 }
		for (i=0;i<state_bits;i++)
		{
         CheckTransitionResults[i] = TransitionResults[i].Eval(invals);
		}
		for (i=0;i<state_bits;i++)
		{
         if (CheckTransitionResults[i] == mgr.bddOne())
		 {BDDbitres =  1;}else
		 if (CheckTransitionResults[i] == mgr.bddZero())
		 {BDDbitres =  0;}else
		 {cout << "Nonconstant evaluation result at bit " << i << " and configuration ";
		 for (j=0;j<ns;j++) {cout << base_conf[j] << ",";}
		 cout << "\n";
		 }
		 BDDres = BDDres | (BDDbitres << i);
		}
		if (res != BDDres) 
		{cout << "BDD result mismatch!" << res << "!=" << BDDres;
         for (j=0;j<ns;j++) {cout << base_conf[j] << ",";}
		}
	if (next_conf(base_conf,0,ns)) {break;}
	}
}



/*
unsigned long int BDD_Image::image_size_r(vector<BDD> multi_bit_function)
{
// simple imitation of a recursive version
	// vector<int> backtrack; // this will contain the indexes where we branched
	vector<BDD> mbf_accumulated; 
	BDD current_condition = mgr.bddOne();
	BDD h;
	int i=0;
	int n=multi_bit_function.size();
	unsigned long int res = 0;
	while (i<n)
	{
		h = multi_bit_function[i] 	* current_condition;
		if       ((i>=n) || h==mgr.bddZero())
		{// need to backtrack
			while (i>0)
			{if (backtrack.size()) 
			  {
				i--;  backtrack.pop_back();
			    current_condition = mbf_accumulated.back(); mbf_accumulated.pop_back();
			    current_condition = current_condition * !multi_bit_function[i];
			
			
			
			res++;
			
		}else{
		 
			if () 
		{i++;}
		else 
		{ // go into recursion
			mbf_accumulated.push_back(current_condition);
			backtrack.push_back(i);
			current_condition = h;
			i++;
			}
		}
	}
	return res;


}
*/


// recursive variant
unsigned long int BDD_Image::image_size_h(vector<BDD> multi_bit_function,int i,BDD current_condition)
{
	BDD h;
	int hr;
if (i>= multi_bit_function.size())
{return 1;
}
h = current_condition * multi_bit_function[i];
hr=0;
if (h!=mgr.bddZero()) { hr=image_size_h(multi_bit_function,i+1,h);}
h = current_condition * !multi_bit_function[i];
if (h!=mgr.bddZero()) { hr +=image_size_h(multi_bit_function,i+1,h);}
return hr;
}

 // with report
unsigned long int BDD_Image::image_size_hr(vector<BDD> multi_bit_function,int i,BDD current_condition,int respat)
{
	BDD h;
	int hr;
	int j;int s;int t;
if (i>= multi_bit_function.size())
{
	for (j=0;j<current_depth;j++)
	{
		t=0;
		for (s=0;s<state_bits;s++)
		{
			if (respat & (1<<(state_bits*current_depth -1  - ((j*state_bits)+s) )))
			{ t = t | (1<<s);}
		}
		Bsetdump.push_back(theca.CState->display_table[t]);
	}
	Bsetdump.push_back(' ');
	return 1;
}
h = current_condition * multi_bit_function[i];
hr=0;
if (h!=mgr.bddZero()) { hr=image_size_hr(multi_bit_function,i+1,h,(respat<<1)|1);}
h = current_condition * !multi_bit_function[i];
if (h!=mgr.bddZero()) { hr +=image_size_hr(multi_bit_function,i+1,h,(respat<<1)|0);}
return hr;
}



unsigned long int BDD_Image::image_size_r(vector<BDD> multi_bit_function)
{
unsigned long int res;
if (dump_bsets)
{
res = image_size_hr(multi_bit_function,0,mgr.bddOne(),0);
Bsetdump = Bsetdump + ";\n";
return res;
}else{
return image_size_h(multi_bit_function,0,mgr.bddOne());
}
}

// unsigned long int 

CA_ALGORITHM_FACTORY(BDD_Image,"BDD_Image_Incremental");

void BDD_Image::configure(Config  * cfg,string toptag)
{
Enum_BWInc::configure(cfg,toptag);
 bool error; 
std::istringstream stm;
  vector<string> keys(4,"");
  keys[0] = toptag; keys[1] = "Method";keys[2] = "Step"; 
  
   keys[3]="DumpTransitionBDD";
   if ('y' == cfg->GetDataByFormat("EEEA",keys,error)[0])
   {
	   dump_transition_BDD();
   }
   keys[3]="OnlyBuildCone";
   only_build_cone = ('y' == cfg->GetDataByFormat("EEEA",keys,error)[0]);

 // configure here more !
}

void BDD_Image::prepare()
{
	 // inherited, includes initialization of TopoIndexer 
	Enum_BWInc::prepare();
	// create BDD variables for cone size
	CA_BDD::prepare();
	Bapplied.assign(state_bits,mgr.bddZero());
	bestA1bsets = 100; // just something non-zero will be overwritten
}


void BDD_Image::report(string toptag){

int i;

if (only_build_cone)
{
 (*report_stream) << "<RESULT name=\"" << toptag << "\"\n"; 
 (*report_stream) << "<TIME>" << result_time(thandle1,thandle2) << "</TIME>";
 (*report_stream) << "</RESULT>";
report_stream->close();
}

(*report_stream) << "<REPRESENTATION> <BASE>";
for (i=0;i<state_bits;i++)
{
(*report_stream) << TransitionResults[i].nodeCount() << " ";
}
(*report_stream) << "</BASE> <TOP>";    
for (i=0;i<state_bits*td;i++)
{
(*report_stream) << Bexprs[i].nodeCount() << " ";
}

(*report_stream) << " <TOP> </REPRESENTATION>\n";
	Enum_BWInc::report(toptag);
}


bool BDD_Image::enum_some(int steps)
{
	int i=0;
	int j=0;
	int prev_depth;
	bool next_aconfres;
	unsigned long int Bres;
	unsigned long long int Acompressed;
	while(j<steps)
	{
		
		j++;
        
       ApplyAconf();
	   if (dump_bsets)
	   {
		   Bsetdump = Bsetdump + theca.CState->conf2string(Aconf,2*current_depth) + " :";
	   }
	   Bres = image_size_r(Bapplied);
			if (Bres > low_registration_bound)
			{new_incremental_history[Bres].push_back(Aconf);copy_aconf();
			}
			// Acompressed = 
			// register Aconf, result
			// Bcountvals.push_back(encBsets->count);
			// cout << encBsets->count << "\n";
			if (report_decent_accumulated && current_depth>1)
			{
#ifdef inthash_decendance
				decendance[decendance_index(current_depth-2,current_history_Bcount,Bres)]++;
#else
				decendance[current_depth-2][current_history_Bcount][Bres]++;
#endif
			}
			if (bestAbsets < Bres)
			{    cout << "<" << current_depth << " " <<Bres << "\n";

				if (Abestconf) {delete Abestconf;}
				Abestconf = Aconf;copy_aconf();
				bestAbsets = Bres; cut_off_bound = bestAbsets / bestA1bsets;
				if (cut_off_bound < 2) {cut_off_bound = 2;}
			}
			prev_depth = current_depth;
			if (current_depth==1)
			{
				if (next_conf(Aconf,0,2))
				{
				if (td==1) {return true;}
				bestA1bsets = bestAbsets; 
				current_depth=2;encBsets->size=2;
				prev_incremental_history = new_incremental_history;
				clear_history();
				current_history_Bcount=bestAbsets;
			    current_history_conf=0;bestAbsets=0;
		        for (i=0;i<current_depth*2-2;i++){Aconf[i]=prev_incremental_history[current_history_Bcount][current_history_conf][i];};
		        init_conf(Aconf,current_depth*2-2,current_depth*2);
				Bexprs = FindBconf(current_depth);Bapplied.assign(current_depth*state_bits,mgr.bddZero());
				}
			}else{
				if (next_aconf()) {return true;}
			  // depth increment happens in next_aconf, but I do not want to 
			  // redo it
			if (prev_depth != current_depth) {Bexprs = FindBconf(current_depth);Bapplied.assign(current_depth*state_bits,mgr.bddZero());}
			}
			
			 
	}
	return false;

}

vector<BDD> BDD_Image::FindBconf(int current_depth)
{
    vector<BDD> res; 
	int j;
	int s;
   int i=0; 
      // both should be something special from TopoIndexer, which we do not have yet.
	for (s=0;s<current_depth;s++)
	{for (j=0;j<state_bits;j++) 
	{res.push_back(BDD_cone[myTI->offsets_per_generation(td,current_depth)+s][j]);
	}
	}
	return res;

}

void BDD_Image::ApplyAconf()
{ // given the current depth, the current Bexprs and the current Aconf,
	// calculate vector<BDD> Bapplied by applying the configuration bits to the input vars.
	// this should be a simple tree traversal, but I did not find a partial Eval, 
	// and do not want to mess around with more Cudd-specific functions
	int i;
	int j;
	int s;
	int v;
	intcellstate ai;
	BDD t;
	BDD aib;
	for (i=0;i<Bexprs.size();i++)
	{
		t = Bexprs[i];
		for (j=0;j<current_depth*2;j++) // number of Avars to consider!
		{
			ai = Aconf[j];
			for (s=0;s<state_bits;s++)
			{
				aib = (ai&(1<<s))?mgr.bddOne():mgr.bddZero();
				t=t.Compose(aib,Avar_index_start+j*state_bits+s);
			}
		}
		Bapplied[i]=t;
	}
}


void BDD_Image::calc()
{
	int i;

	thandle1=start_time();
	
    build_BDD_cone();
    if (only_build_cone)
	{
      thandle2=start_time();
	}else
	{

	Bexprs =FindBconf(1);

// first do standard enumeration with depth =1
		enum_start(); // need both, as we start with a normal enumeration
		incremental_start();
		keep_aconfs = true;
		current_depth=1;
		prev_incremental_history =  vector<vector<intcellstate *> >(ipow(max_state,td));
		thandle2=start_time();
		current_depth=1;
		// clear_history();
		current_history_Bcount=0;
		current_history_conf=0;bestAbsets=0;
		init_conf(Aconf,0,current_depth*2);


		
		while (!enum_some(1000)){};
		thandle3=start_time();
		if (keep_history){all_time_history.push_back(new_incremental_history);}
	}
}    






// That worked for some time, but after adding the one_wayBW method, it would become quite messy
// need proper factories now
/*
ca_analysis* CreateNamedAlgorithm(Config * conf,string toptag)
{
ca_analysis* res;
std::istringstream stm;
 bool error;    
CellAut * theca;
vector<string> keys(4,"");

theca = CreateNamedCellAut(conf,toptag);

string MethodName;

keys[0] = toptag;

  keys[1] = "Method";keys[2] = "Step";keys[3]="type";
  MethodName = conf->GetDataByFormat("EEEA",keys,error);
  
if (MethodName == "FullEnumeration")
{res = new Enum_BW(*theca);}
else if (MethodName == "DepthIncrementalEnumeration")
{res = new Enum_BWInc(*theca);}
else if (MethodName == "BDD_Image_Incremental")
{res = new BDD_Image(*theca);}
else if (MethodName == "JustRun")
{res = new just_run(*theca);}


return res;
}
*/
