#ifndef TopoIndexer_h
#define TopoIndexer_h


/* the idea is to create index array structures which allow 
storing configurations in arrays and using the index structures to find 
the rigth index
*/

#include "ca.h"
#include <map>
// #include <hash_map>
#include <ostream>

class TopoIndexer
{
public:
 // create an indexing structure for tdepth many steps and radius cells around the border cells
  TopoIndexer(CellAutLocation* topo,int expand,const  vector<CellAutLocation* > target_cells, bool incremental_target);
      // stupid default-setup for 1-D
	TopoIndexer(int border_depth);

// cells of the same generation are kept together
// so you can either allocate one array for all generations, or 
// use partial 

// int *  ixr; // indixes with increasing distance from border cell(s), receive side
// int *  ixs; // send side 

// get the index of a neighbor

const int* nb_ix(int depth,int gen,int p);	
	

//int * cells_over_distance; // used to address an enitre cone in one array: how many cells have a given distance to the border cells 

 vector<int>  rcells_over_distance,rcells_over_distance_acc,rcells_over_distance_acc2;
 vector<int> scells_over_distance,scells_over_distance_acc,scells_over_distance_acc2;
 

int  offsets_per_generation(int cone_size, int g);
    
int cells_per_generation(int conesize,int g);

int baseline_r(int);
int baseline_s(int);

int generation_start(int depth,int gen)
// {return gen-depth;}
;
int generation_end(int depth,int gen) 
// {return 2*depth-gen;}
;

int max_ix; // for the given problem the size you need for the cone to allocate

int * rbark; // this is a list of indexes on the receive side on which no other cell depends, 
             // but are not in the last temporal step
             // these are important to some algorithms
             // only valid if entire cone state kept in one array

int * sbark; // same for the send side

// optional: if we want to give configuations/pattern etc. in the original 
// coordinates, have to calculate back
 // for now, as we do not have the CellAutLocation parameter in the creator
// vector<CellAutLocation> index2location; 
 const OneD_direct *  index2location(int p);
	// temporarily used, might consider making it local
	
//	CellAutLocation::LocationHash   * visited;
    map<cals,int,cals_less> visited; // for the advanced sequences I need to convert coordinates to indexes later

void report_structure(ostream &report_stream,int verbosity);

private:
	const int expand;
    vector<OneD_direct> index2location_p;
	vector<int *> nb_ix_h;
	vector<int> baseline_r_h,baseline_s_h;
    // int * baseline_r_h;
    // int * baseline_s_h;

};
















#endif
