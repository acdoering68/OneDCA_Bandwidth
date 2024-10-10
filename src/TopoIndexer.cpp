

#include "TopoIndexer.h"

#include <iostream>
#include <vector>
// #include <pair>


// fill those three data structures
// vector<int *> nb_ix_h;
// int * cells_over_distance; 
// int max_ix; 

/*
bool compare_CellAutLocation(CellAutLocation * l, CellAutLocation * r)
{
return *l == *r;
}

*/




// do it properly later, for now default init for 1-D and sufficient distance

TopoIndexer::TopoIndexer(int border_depth) : expand(200) 
{

	// expand = 200;
	// baseline_r_h = new int[2*expand]; baseline_s_h = new int[expand];
	baseline_r_h.assign(2*expand,0);baseline_s_h.assign(expand,0);
	int * this_nb;
	int i;
	int thispos;
	// 0 is special:
	nb_ix_h.resize(3*expand+2);
	for (i =0;i<expand;i++)
	{
		this_nb = new int[3];assert(this_nb);
		this_nb[0]=2*i;       this_nb[1]=2*i-1;     this_nb[2]=2*i+1;
		nb_ix_h[2*i+expand]=this_nb; 
		this_nb = new int[3];assert(this_nb);
		this_nb[0]=2*i+1;     this_nb[1]=2*i;       this_nb[2]=2*i+2;
		nb_ix_h[2*i+1+expand]=this_nb; 
		this_nb = new int[3];   assert(this_nb);// this is the "send" side, but all are continously enumerated
		this_nb[0]= -i;       this_nb[1]= -i-1;     this_nb[2]= -i+1;
		nb_ix_h[expand-i]=this_nb; 

		baseline_r_h[2*i]=2*i;  baseline_r_h[2*i+1]=2*i+1;  baseline_s_h[i]= -i-1;

	}; 
	max_ix = 3*expand+1;
	index2location_p.assign(3*expand,OneD_direct(0));
	for (i=0;i<3*expand;i++)
	{
		index2location_p[i].position   = i-expand;
	} 
	// int * cells_over_distance; // used to address an enitre cone in one array: how many cells have a given distance to the border cells 

}

int TopoIndexer::baseline_r(int i) {return baseline_r_h[i];}
int TopoIndexer::baseline_s(int i) {return baseline_s_h[i];}


int TopoIndexer::generation_start(int depth,int gen)
{
	return 0;
}


int TopoIndexer::generation_end(int depth,int gen) 
{
	return cells_per_generation(depth,gen);
}

const int * TopoIndexer::nb_ix(int depth,int gen,int p)
{
	// return nb_ix_h[p+expand+depth-gen];
	return nb_ix_h[p];
};


// the "expand +" is needed to compensate the negative offsets 
int  TopoIndexer::offsets_per_generation(int conesize, int g)
{
	// return  2*(g*conesize - (g*(g-1))/2)+g*conesize;
	// return  conesize+  3*g*conesize - (g*(g-1));
	
	
	return rcells_over_distance_acc2[conesize]   +scells_over_distance_acc2[conesize]
	      -rcells_over_distance_acc2[conesize-g] -scells_over_distance_acc2[conesize-g];
};

int TopoIndexer::cells_per_generation(int conesize,int g)
{
	return rcells_over_distance_acc[conesize-g]+scells_over_distance_acc[conesize-g];
	// return 2*(conesize-g)+conesize;
};

const OneD_direct * TopoIndexer::index2location(int p) 
{
	return &(index2location_p[p+expand]);
}



// now the proper way with breadth-first search
// this is a nested search, for each new cell we add in the target set we have to do a new, wider search
TopoIndexer::TopoIndexer(CellAutLocation* topo,int expand,const  vector<CellAutLocation* > target_cells,bool incremental_target=true) : expand(expand)
{
	// CellAutLocation::LocationHash   * visited;
	// hash_map<CellAutLocation ,int> visited;
	// map<CellAutLocation,int,LocComp> visited;
	// map<cals,int,cals_less> visited;
	map<cals,int,cals_less>::iterator new_element_vis;
	unsigned int i;
	unsigned int dist;
	int cells_thisdistance;
	int running_index;
	//	* vector< *pair<CellAutLocation,short int> > cur_work_stack; 
	//    * vector< *pair<CellAutLocation,short int> > new_work_stack; 
	vector< CellAutLocation* > cur_work_stack; 	
	vector< CellAutLocation* > new_work_stack;
	CellAutLocation * work_element;
	CellAutLocation * new_element;
	// the map shall use the location, not the pointer!
	// hash_map<CellAutLocation ,int>::iterator new_element_vis;
	int * current_neighbors;
	rcells_over_distance.assign(expand+2,0);
	scells_over_distance.assign(expand+2,0); 
	rcells_over_distance[0]=target_cells.size();
	scells_over_distance[0]=0;
	scells_over_distance_acc.push_back(scells_over_distance[0]);
    rcells_over_distance_acc.push_back(rcells_over_distance[0]);
	for (i=1;i<expand+1;i++) {rcells_over_distance[i]=0;scells_over_distance[i]=0;}
	unsigned maxinit=target_cells.size();
	if (incremental_target)
	{maxinit=1;
	}
	for (i=0;i<maxinit;i++)
	{new_element = target_cells[i];
	cur_work_stack.push_back(new_element);
	cals ncals(new_element); 
	visited[ncals]=i;
    baseline_r_h.push_back(i);
	}
	// cells_over_distance[0] = target_cells.size();

	running_index = maxinit;
	for (dist=1;dist<expand+1;dist++)
	 {new_work_stack.clear(); cells_thisdistance=0;
	  while(cur_work_stack.size() >0) 
	   {work_element = cur_work_stack.back();
	    cur_work_stack.pop_back();
	    current_neighbors = new int[topo->neighbors()+1];
		new_element_vis =  visited.find(cals(work_element));
	    if (new_element_vis == visited.end()) // new location
		 {  current_neighbors[0] = nb_ix_h.size();}
		else
		 {  current_neighbors[0] = new_element_vis->second;} // for target cells, the index is already fixed
	    for (i=1;i<topo->neighbors()+1;i++)
	     {new_element = work_element->neighbor(i);
	      new_element_vis =  visited.find(cals(new_element));
	      if (new_element_vis == visited.end()) // new location
	        {cells_thisdistance++;
	         if (new_element->remote_cell())
	          {
		       scells_over_distance[dist]++;
		       baseline_s_h.push_back(running_index);
	          }
	         else
	          {
		       rcells_over_distance[dist]++; 
		       baseline_r_h.push_back(running_index);
	          }
	        visited[cals(new_element)]=running_index;
	        current_neighbors[i] = running_index;
	        cout << "new neighbor of " << work_element->as_string() << " " << running_index << " i=" << i <<" "<< new_element->as_string() << endl;
	        new_work_stack.push_back(new_element);
	        //nb_ix_h.push_back(new int[topo->neighbors()]);
	        running_index++;
	     }else
	     {current_neighbors[i] = new_element_vis->second;
	      cout << "exs neighbor of " << work_element->as_string() << " " << new_element_vis->second << " i=" << i << endl;
	     }
	  }
	// current_neighbors[0] = nb_ix_h.size();
	cout << " adding neighbor vector " << nb_ix_h.size();
	for (i=0;i<topo->neighbors()+1;i++)
		cout << " " << current_neighbors[i];
	nb_ix_h.push_back(current_neighbors);
	cout << endl;
	};
	cur_work_stack=new_work_stack;
	// cells_over_distance[i+1]=cells_thisdistance;

	  scells_over_distance_acc.push_back(scells_over_distance_acc[dist-1]+scells_over_distance[dist]);
      rcells_over_distance_acc.push_back(rcells_over_distance_acc[dist-1]+rcells_over_distance[dist]);


	};
	max_ix = running_index;
	scells_over_distance_acc2.push_back(scells_over_distance_acc[0]); // should be 0?!
	rcells_over_distance_acc2.push_back(rcells_over_distance_acc[0]);
	for (i=1;i<scells_over_distance_acc.size();i++)
		 scells_over_distance_acc2.push_back(scells_over_distance_acc2[i-1]+scells_over_distance_acc[i]);
	for (i=1;i<rcells_over_distance_acc.size();i++)
		 rcells_over_distance_acc2.push_back(rcells_over_distance_acc2[i-1]+rcells_over_distance_acc[i]);
        // more debug
        cout << "rcells over distance: ";
        for (i=0;i<rcells_over_distance.size();i++)
	  cout << rcells_over_distance[i] << " ";
        cout << endl;
        cout << "scells over distance: ";
        for (i=0;i<scells_over_distance.size();i++)
	  cout << scells_over_distance[i] << " ";
        cout << endl;


};
 

void TopoIndexer::report_structure(ostream &report_stream,int verbosity)
{
	unsigned i;
	report_stream << "<TopologyIndex   expand=" << expand << ">" << endl;
	report_stream << "<ExpansionOverDistanceRcv> ";
	for (i=0;i<expand;i++)  report_stream << rcells_over_distance[i] << " ";
	report_stream << "</ExpansionOverDistanceRcv> ";

	report_stream << "<ExpansionOverDistanceSnd> ";
	for (i=0;i<expand;i++)  report_stream << scells_over_distance[i] << " ";
	report_stream << "</ExpansionOverDistanceSnd> ";



	report_stream << "</TopologyIndex>";
}

