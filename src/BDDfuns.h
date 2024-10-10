#include "enumeration_algorithms.h"


boolean_function * addition(Cudd * mgr, const vector<unsigned> invarsa, const vector<unsigned> invarsb);

boolean_function * compare(Cudd * mgr, const vector<unsigned> invarsa, const vector<unsigned> invarsb);

boolean_function * popcount(Cudd * mgr, const vector<unsigned> invars);

boolean_function *  life1cell(Cudd * mgr,const vector<unsigned> invars) ;
