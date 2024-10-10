#ifndef Cone_h
#define Cone_h

#include "ca.h"
#include "TopoIndexer.h"


// think about making this a template function, with parameter intcellstate!
void calc_cone(TopoIndexer * TI, CellAut * ca,const intcellstate* Aconf,const intcellstate* Cconf,intcellstate* cone_state, int depth,int width,intcellstate * ResLine = NULL);















#endif