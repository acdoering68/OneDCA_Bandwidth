#include "BDDfuns.h"

boolean_function * addition(Cudd * mgr, const vector<unsigned> invarsa, const vector<unsigned> invarsb)
{
boolean_function*  res = new boolean_function;
res->varindexes = invarsa;
res->varindexes.insert(res->varindexes.end(), invarsb.begin(),invarsb.end());

BDD carry=mgr->bddZero();
unsigned i;
for (i=0;i<max(invarsa.size(),invarsb.size());i++)
{
	BDD a = (i<invarsa.size())?mgr->bddVar(invarsa[i]):mgr->bddZero();
	BDD b = (i<invarsb.size())?mgr->bddVar(invarsb[i]):mgr->bddZero();
	res->yvars.push_back(carry ^a ^ b);   
    carry=carry*(a + b) + a * b;
}
res->yvars.push_back(carry);
return res;
}

boolean_function * compare(Cudd * mgr, const vector<unsigned> invarsa, const vector<unsigned> invarsb)
{
	boolean_function*  res = new boolean_function;
	res->varindexes = invarsa;
	res->varindexes.insert(res->varindexes.end(), invarsb.begin(),invarsb.end());

	BDD carry=mgr->bddZero();
	unsigned i;
	for (i=0;i<max(invarsa.size(),invarsb.size());i++)
	{
		BDD a = (i<invarsa.size())?mgr->bddVar(invarsa[i]):mgr->bddZero();
		BDD b = (i<invarsb.size())?mgr->bddVar(invarsb[i]):mgr->bddZero();

		carry = carry * !(a ^ b) + a * !b;
	}
	res->yvars.push_back(carry);
	return res;
}


unsigned int ilog2(unsigned int v)
{
	unsigned int vh=v;
	unsigned int res=0;
	while (vh>0)
	{res=res++;vh=vh>>1;
	}
	return res;
}


boolean_function * popcount(Cudd * mgr, const vector<unsigned> invars)
{

boolean_function*  res = new boolean_function;
res->varindexes = invars;
  
const int loglength = ilog2(invars.size()+1);
unsigned i;
int j;
  BDD n; BDD c;
for (j=0;j<loglength;j++) {res->yvars.push_back(mgr->bddZero());}
for (i=0;i<invars.size();i++)
{
	n=mgr->bddVar( res->varindexes[i]);
 c=res->yvars[0] * n;
 res->yvars[0] = res->yvars[0] ^ n ;
 for (j=1;j<loglength;j++) 
  {n=c;
   c=res->yvars[j] * c; // new carry
   res->yvars[j]=n ^ res->yvars[j];
   }// c should here be always bddZero
}
return res;
}
// invars[0] is the cell's own state, 1..8 are the neighbors
boolean_function *  life1cell(Cudd * mgr,const vector<unsigned> invars) 
{
	boolean_function*  res = new boolean_function;
res->varindexes = invars;
// need 4 bits to count the number of neighbors 0..8
vector<BDD> h1(4); // 4 sums
BDD c; // carry
BDD n;// the state of the neighbor cell for readability
int i;
int j;
for (j=0;j<4;j++) {h1[j]=mgr->bddZero();}
for (i=1;i<9;i++)
{
	n=mgr->bddVar(invars[i]);
	
 c=h1[0] * n;
 h1[0] = h1[0] ^ n ;
 for (j=1;j<4;j++)
  {n=c;
   c=h1[j] * c; // new carry
   h1[j]=n ^ h1[j];
   }// c should here be always bddZero
}
// the game of life rule:
n = mgr->bddVar(invars[0]);
res->yvars.push_back( n*h1[1]*!h1[2] + // cell is alive and stays so if two or three neighbors are alive
                     !n*h1[0]*h1[1]*!h1[2]); // cell was dead and is born if three neighbors are alive

 return res;
}