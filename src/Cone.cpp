


#include "Cone.h"
#include <iostream>
// #include <ostream>

using namespace std;




void calc_cone(TopoIndexer * TI, CellAut * ca,const intcellstate* Aconf,const intcellstate* Cconf,intcellstate* cone_state, int depth, int width,intcellstate * Bconf)
{

	int i;
	int n;
	int s;
	int ns;
	ns = ca->CALocation->neighbors()+1;
	int this_row_startix;
	int next_row_startix;
	vector<intcellstate> state_vec(ns,0);
	for (i=0;i<depth;i++)     {cone_state[TI->baseline_s(i)+(TI->offsets_per_generation(depth,0))]=Cconf[i];}  
    for (i=0;i<2*depth;i++)   {cone_state[TI->baseline_r(i)+(TI->offsets_per_generation(depth,0))]=Aconf[i];}  
	for (i=0;i<=depth-1;i++)
	{   this_row_startix = TI->offsets_per_generation(depth,i);
        next_row_startix = TI->offsets_per_generation(depth,i+1);
		for (s=TI->generation_start(depth,i+1);s< TI->generation_end(depth,i+1);s++)
		{    // cout << "[";
			for (n=0;n<ns;n++) {
				// cout << this_row_startix+TI->nb_ix(depth,i,s)[n] << " ";
				state_vec[n]=cone_state[this_row_startix+TI->nb_ix(depth,i,s)[n]];
				// cout << ca->CState->display_table[state_vec[n]];
			   }
			ca->CState->transition(state_vec);  
			cone_state[next_row_startix+s] = ca->CState->as_int();
			// cout << "->" <<  ca->CState->display_table[cone_state[next_row_startix+s]] << next_row_startix+s << "]";
			// cout << "->" ;
		}
		// cout << "\n";
	}
	// get result if required
  if (Bconf)
  {    int i=0; 
	  for (s=TI->generation_start(depth,depth);s< TI->generation_end(depth,depth);s++)
	  {Bconf[i++]=cone_state[TI->offsets_per_generation(depth,depth)+TI->baseline_r(s)];
	  }
  }

}
