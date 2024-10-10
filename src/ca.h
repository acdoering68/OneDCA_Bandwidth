/* these are the basic classes for 
dealing with arbitrary cellular automata
*/


#ifndef ca_h 
 #define ca_h
// #include <STL>
 #include <vector>
// #include <hash_map>

#include <map>
#include <string>
#include <sstream>

#include "configurator.h"

// use this all over
#include <assert.h>
using namespace std;
// using namespace stdext; // VC++ has hash_map in stdext
// first the geometry
// this is the naive constructive approach, 
// we define the geometry as a type with equality and 
// a function to construct the neighbors from a given type.
// we can than use a hash table and create and integer indexed vector
// therefore, we also have to provide a hash function
class CellAutLocation
{
public:
  // every derived class should have a construction from string:
  virtual CellAutLocation *  factory(string l);
  virtual bool isless(const CellAutLocation *) const; // needed for Hash in each inherited class
  // do not know why it does not work this way, but inherited classes do not see their value!
  virtual unsigned int neighbors();
  // const static int neighbors=0; // good to know, how many neighbors has a cell
  virtual CellAutLocation * neighbor(int);// =0; // give the ith neighbor
  virtual const bool remote_cell(); // we assume an implicit border, such as x>0  
  virtual string as_string() {return "???";};
   // CellAutLocation
  // bool operator<(const CellAutLocation * &b);
  // virtual 
  // friend ostream& operator<<(ostream& out, const CellAutLocation& object){out << object.as_string(); return out;};
};

typedef unsigned short intcellstate; 

class CellAutState
{
public:
	short int state_num;
	CellAutState() : state_num(0) {};
	// for performance reason we use short int, but put here the sugar
  virtual  intcellstate as_int()=0;
  virtual void configure(Config * conf,string toptag){};
  // virtual CellAutState(intcellstate v); 
  // enumerate the states of a cell
  // this is more or lss needed for space economy, 
  // we could otherwise use a constructor
  
  // true, if a new state was created
  
  //transition  
  virtual void transition(vector< intcellstate > curr_state); 
  string display_table ;  
  string conf2string(intcellstate* conf,int size);

};

struct CellAut
{
	CellAutState     * CState;
    CellAutLocation  * CALocation;
};


// now some concrete subclasses

class OneD_direct : public CellAutLocation
{
public:
  CellAutLocation * factory(string l);
  bool isless(const CellAutLocation*) const ; // need to keep the parameter abstract here
  unsigned int neighbors() {return 2;};
  // const static int neighbors=2; 
  const bool rside() {return (position <=0);}
  virtual const bool remote_cell();
   
  
    // because of overloading have to return the base class
   CellAutLocation * neighbor(int);
    
   OneD_direct(int pos) {position=pos;}
   virtual string as_string() 
	{ostringstream out; out << "(" << position << ")"; return out.str();};
	
     
 // private: 
  int position;
};

bool operator== (const OneD_direct l, const OneD_direct r) ;

class TwoD_neumann : public CellAutLocation
{
public:
	CellAutLocation * factory(string l);
	bool isless(const CellAutLocation *) const; 
	TwoD_neumann(int px,int py);  
	unsigned int neighbors() {return 4;};
	CellAutLocation * neighbor(int i);
	// const static  int neighbors=4;
	virtual string as_string() 
	{ostringstream out; out << "(" << posx << "," << posy << ")"; return out.str();};
	
     
	 const bool remote_cell();
  
 
  int posx,posy;
};
 
bool operator== (const  TwoD_neumann l, const  TwoD_neumann r); 


class TwoD_moore : public CellAutLocation
{
public:
  bool isless(const CellAutLocation *) const; 
  unsigned int neighbors() {return 8;};
  // static const int neighbors=8;
   const bool remote_cell();
       // {return (posx==0) || (posy==0)};

virtual string as_string() 
	{ostringstream out; out << "(" << posx << "," << posy << ")"; return out.str();};
	
   
   TwoD_moore(int px,int py);
   CellAutLocation * neighbor(int i);
   CellAutLocation * factory(string l);
  int posx,posy;
};
 
bool operator== (const  TwoD_moore l, const  TwoD_moore r); 

/*
class LocComp // : public Compare
{
	bool operator<(const CellAutLocation * &b) const;
}
*/


class cals
{
public:
	cals(CellAutLocation * a);
	const CellAutLocation* operator()()const;
	cals operator=(cals &rhs);
private:
	CellAutLocation* ptr;
};





class cals_less : public less<cals>
{
public:
	 bool operator () (const cals & lhs,const cals & rhs);
   
};

	 // bool cals_less::operator<(const cals &a, const cals &b);


class TwoStates : virtual public CellAutState
{
 public:
  //  static const short int state_num=2;
  //  TwoStates(int ix);
   TwoStates();
   virtual void configure(Config * conf,string toptag);
   void transition(vector< intcellstate > curr_state); 
   int state;
   intcellstate as_int();
   // static 
   // string display_table ;  
   int trans_func_index;
   
};

// my own example automaton from my diploma thesis

class r2a : public CellAutState
{
public:
	
	r2a();
	
	unsigned char state;
	intcellstate as_int(){return state;};
	void transition(vector< intcellstate > curr_state); 
	// string display_table ;  
	virtual void configure(Config * conf,string toptag);

};



// the following FSSP definitions are inherited from 
// W.F. Lunnon
// http://www.cs.nuim.ie/~fred/CellAuto.html

class BinaryFSSP :  public CellAutState
{
public:
	const short int state_num;
	BinaryFSSP();
	unsigned char state;
	intcellstate as_int() {return (intcellstate)state;}
	
	void transition(vector< intcellstate > curr_state); 
	// string display_table ;  
	virtual void configure(Config * conf,string toptag);
private:
	vector< unsigned char> transl_table; 
	
	// const unsigned char * transl_table ;  
  
};

class MooreFSSP : virtual public CellAutState
{
public:
	static const short int state_num=10;
	MooreFSSP();
	unsigned char state;
	intcellstate as_int() {return (intcellstate)state;}
	
	void transition(vector< intcellstate > curr_state); 
	// string display_table ;  
	virtual void configure(Config * conf,string toptag);
private:
	vector< unsigned char> transl_table ;  
  
};

class MazoyerFSSP :  public CellAutState
{
public:
	// const short int state_num;
	MazoyerFSSP();
	unsigned char state;
	intcellstate as_int() {return (intcellstate)state;}
	
	void transition(vector< intcellstate > curr_state); 
	// string display_table ;  
	virtual void configure(Config * conf,string toptag);
private:
	vector< unsigned char> transl_table; 
	
	// const unsigned char * transl_table ;  
  
};

// as in "Smaller solutions for the firing squad" Theoretical Computer Science 2002, 83-109
class Yunes1FSSP :  public CellAutState
{
public:
	// const short int state_num;
	Yunes1FSSP();
	unsigned char state;
	intcellstate as_int() {return (intcellstate)state;}
	
	void transition(vector< intcellstate > curr_state); 
	// string display_table ;  
	virtual void configure(Config * conf,string toptag);
private:
	vector< unsigned char> transl_table; 
	
	// const unsigned char * transl_table ;  
  
};

class Yunes2FSSP :  public CellAutState
{
public:
	// const short int state_num;
	Yunes2FSSP();
	unsigned char state;
	intcellstate as_int() {return (intcellstate)state;}
	
	void transition(vector< intcellstate > curr_state); 
	// string display_table ;  
	virtual void configure(Config * conf,string toptag);
private:
	vector< unsigned char> transl_table; 
	
	// const unsigned char * transl_table ;  
  
};

class Gerken :  public CellAutState
{
public:
	// const short int state_num;
	Gerken();
	unsigned char state;
	intcellstate as_int() {return (intcellstate)state;}
	
	void transition(vector< intcellstate > curr_state); 
	// string display_table ;  
	virtual void configure(Config * conf,string toptag);
private:
	vector< unsigned char> transl_table; 
	
	// const unsigned char * transl_table ;  
  
};

class Balzer :  public CellAutState
{
public:
	// const short int state_num;
	Balzer();
	unsigned char state;
	intcellstate as_int() {return (intcellstate)state;}
	
	void transition(vector< intcellstate > curr_state); 
	// string display_table ;  
	virtual void configure(Config * conf,string toptag);
private:
	vector< unsigned char> transl_table; 
	
	// const unsigned char * transl_table ;  
  
};

class Life : public CellAutState
{
public:
	Life();
	unsigned char state;
	virtual intcellstate  as_int() {return (intcellstate)state;}
    virtual void transition(vector< intcellstate > curr_state); 
	virtual void configure(Config * conf,string toptag);
protected:
	unsigned bornl,bornh,diel,dieh;
	// vector< unsigned char> transl_table; 
};

class BriansBrain : public Life
{
public:
	BriansBrain();
	virtual intcellstate as_int() {return (intcellstate)state;}
        virtual void transition(vector< intcellstate > curr_state); 
	virtual void configure(Config * conf,string toptag);
protected:
        // inherited from Life
	// unsigned bornl,bornh,diel,dieh;
	// vector< unsigned char> transl_table; 
};



#include <xercesc/util/PlatformUtils.hpp>
#include "configurator.h"


CellAut * CreateNamedCellAut(Config * conf,string toptag);

// factory for initialized locations:
// make it also a member of CellAut?!
vector<CellAutLocation *> location_list_conf(CellAut* theca,Config * conf, string parent_tag);
vector<CellAutLocation *> location_list(CellAut* theca, string conf_string);

#endif
