


#include "ca.h"


bool CellAutLocation::isless(const CellAutLocation * cal) const {return false;};
  // should I raise an exception

unsigned int CellAutLocation::neighbors() {assert(false); return 0;}
CellAutLocation * CellAutLocation::neighbor(int i) {assert(false); return NULL;}
const bool CellAutLocation::remote_cell() {assert(false); return false;}
const bool TwoD_moore::remote_cell()
       {return (posx<0) || (posy<0);};

	   const bool TwoD_neumann::remote_cell()
       {return (posx<0) || (posy<0);};

	   TwoD_neumann::TwoD_neumann(int px,int py) {posx=px;posy=py;};  
	   TwoD_moore::TwoD_moore(int px,int py) {posx=px;posy=py;};  

bool operator== (const  TwoD_neumann l, const  TwoD_neumann r) 
                  {return (l.posx == r.posx) && (l.posy==r.posy) ;}
	   
bool TwoD_moore::isless(const CellAutLocation * cal)const {return (((TwoD_moore*) cal)->posx <  posx) | 
                                                        ((((TwoD_moore*) cal)->posx ==  posx) & 
														(((TwoD_moore*) cal)->posy <  posy));};	   

bool TwoD_neumann::isless(const CellAutLocation * cal)const {return (((TwoD_neumann*) cal)->posx <  posx) | 
                                                        ((((TwoD_neumann*) cal)->posx ==  posx) & 
														(((TwoD_neumann*) cal)->posy <  posy));};	   


// int CellAutLocation::neighbors=0;
CellAutLocation * OneD_direct::neighbor(int i) 
{
  
  if (i==1) 
    {return new OneD_direct(position+1);}
  else if (i==0)
    {return new OneD_direct(position);}
  else if (i==2)
    {return new OneD_direct(position-1);}
  else 
    {throw "unknown neighbor\n";}
}


CellAutLocation * TwoD_neumann::neighbor(int i) 
{
  
  if (i==1) 
    {return new TwoD_neumann(posx+1,posy);}
  else if (i==0)
    {return new TwoD_neumann(posx,posy);}
  else if (i==2)
    {return new TwoD_neumann(posx-1,posy);}
  else if (i==3)
    {return new TwoD_neumann(posx,posy-1);}
  else if (i==4)
    {return new TwoD_neumann(posx,posy+1);}
  else 
    {throw "unknown neighbor\n";}
}

CellAutLocation * TwoD_moore::neighbor(int i) 
{
  unsigned dx=0;
  unsigned dy=0;
  if (i>0 && i<4) dx=-1;
  if (i>4 && i<8) dx=1;
  if (i>2 && i<6) dy=-1;
  if (i>6 || i==1) dy=1;
  if (i>8) throw "unknown neighbor\n";
  
  return new TwoD_moore(posx+dx,posy+dy);
}


bool OneD_direct::isless(const CellAutLocation * cal)const {return ((OneD_direct*) cal)->position < position;};

// size_t OneD_direct::operator()(const OneD_direct& ODpos)
// { return ODpos.position;} 

const bool OneD_direct::remote_cell()
       {return (position<0);};

ostream& operator << (ostream& os, OneD_direct a) {
  os << a.position;
  return os;
}

CellAutLocation * CellAutLocation::factory(string s)
{
return new CellAutLocation();
}

CellAutLocation  * OneD_direct::factory(string s)
{
  std::istringstream stm;
  stm.str(s);
  OneD_direct * res = new OneD_direct(0);
  stm >> res->position;
  return res;
}

CellAutLocation  * TwoD_neumann::factory(string s)
{
  std::istringstream stm;
  char lp,comma,rp;
  stm.str(s);
  TwoD_neumann * res = new TwoD_neumann(0,0);
  stm >> skipws >> lp  >> res->posx >> comma >> res->posy >> rp;
  if (lp!='(' || comma != ',' || rp != ')') cout  << "error";
  return res;
}

CellAutLocation  * TwoD_moore::factory(string s)
{
  std::istringstream stm;
  char lp,comma,rp;
  stm.str(s);
  TwoD_moore * res = new TwoD_moore(0,0);
  stm >> skipws >> lp  >> res->posx >> comma >> res->posy >> rp;
  if (lp!='(' || comma != ',' || rp != ')') cout  << "error";
  return res;
}
  // const CellAutLocation * &a
// bool CellAutLocation::operator<(const CellAutLocation * &b) const
// {return a->isless(b);}

/*
bool LocComp::operator <(const CellAutLocation * &a, const CellAutLocation * &b)
{
	return this->isless(b);
}
*/


cals::cals(CellAutLocation * a) : ptr(a) {}
// bool operator<(const cals &a, const cals &b) {return a()->isless(b());}
bool cals_less::operator ()(const cals &a, const cals &b) {return a()->isless(b());}
const CellAutLocation* cals::operator()() const {return ptr;}

cals cals::operator=(cals &rhs)
{
cals res(ptr);
return res;

}

/*

TwoStates::TwoStates(int ix) : trans_func_index(ix)
{
	state_num = 2;
 display_table = "01";
}

*/

TwoStates::TwoStates() : trans_func_index(110) // overwritten by configure
{
	state_num = 2;
 display_table = "01";
}


void TwoStates::configure(Config * cfg,string toptag)
{
std::istringstream stm;
bool error;
    vector<string> keys(4,"");
    keys[0] = toptag; keys[1] = "CellAut";keys[2] = "Transition"; 
  
   keys[3]="Rule";
   stm.str(cfg->GetDataByFormat("EEEA",keys,error));
    stm>>trans_func_index;
}

void TwoStates::transition(vector<intcellstate > conf ) 
{
  int h; 
  h= 2*conf[0] + conf[1] + 4*conf[2];
  state =  (trans_func_index>>h  ) & 1;  // mod 2
}
// bool operator== (const OneD_direct l, const OneD_direct r) 
//                   {return l.position == r.position;};


intcellstate TwoStates::as_int() {return state;}

bool operator== (const  TwoD_moore l, const  TwoD_moore r) 
                  {return (l.posx == r.posx) && (l.posy==r.posy) ;}


// for now, I am not sure about the abstract class inheritance thing
void CellAutState::transition(std::vector<intcellstate> curr_state){}


r2a::r2a()  {state_num=3; display_table = "_AR";}
	
	
void r2a::transition(vector< intcellstate > curr_state)
{
state = curr_state[0]; // default: keep state
if (curr_state[0]==0 && curr_state[2]==1)
   state = 2; // grow
else if (curr_state[0]==1 && curr_state[1]==0)
   state = 2; // grow 2nd part
else if (curr_state[0]==2 && curr_state[2]==1)
   state = 1; // swap left side
else if (curr_state[0]==1 && curr_state[1]==2)
   state=2; // swap right side
}

	// string display_table ;  
void r2a::configure(Config * conf,string toptag)
{}


BinaryFSSP::BinaryFSSP() : state_num(5)
{
  display_table = string("_XNK");
 unsigned char trans_table_h[]	= {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 
    0,0,0,0,0,0,1,1,0,0,0,1,1,0,0,0,2,2,0,0,0,1,1,0,0, 
    0,0,0,0,0,0,2,2,0,0,0,2,2,0,0,0,3,3,0,0,0,2,2,0,0, 
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,3,3,4,0,0,0,0,0, 
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0};  
 transl_table = vector<unsigned char>(trans_table_h, trans_table_h + (sizeof trans_table_h / sizeof trans_table_h[0]));
}

void BinaryFSSP::transition(vector<intcellstate> conf)
{
  unsigned int ix;
  ix = 25*conf[0]+5*conf[1]+conf[2];
  state = transl_table[ix];
}

void BinaryFSSP::configure(Config * conf,string toptag){}

MooreFSSP::MooreFSSP()
{
  display_table = "XQCMNSFTDP";
  
 unsigned char trans_table_h[]	= 
/* Moore table */
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 

      0,0,0,0,0,0,0,0,0,0,0,1,4,1,4,1,1,1,1,0,0,3,0,0,0,0,8,0,8,0,
        0,3,0,0,0,0,8,0,3,0,0,1,0,8,0,0,0,0,1,0,0,1,0,0,0,0,0,0,1,0,
        0,2,8,0,8,0,2,0,0,0,0,1,0,0,0,0,0,0,1,0,0,1,8,1,4,1,8,1,0,0,
        0,0,0,0,0,0,0,0,0,0, 

      0,0,0,0,0,0,0,0,0,0,0,8,8,8,0,0,8,0,0,0,0,8,0,8,0,0,0,0,6,0,
        0,0,0,0,0,0,8,0,0,0,0,0,8,8,0,0,8,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,8,0,8,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,0,0,0,0,0,6,0,
        0,0,0,0,0,0,0,0,0,0, 

      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,2,0,1,0,8,0,0,0,0,0,0,8,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,0,0,0,0,0,0,9,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,0,0,0,1,0,2,1,1,
        0,9,0,0,0,2,0,0,9,0, 
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,0,0,5,9,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,2,
        0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,2,0,0,0,0,0,0,9,0,0,5,9,
        0,1,0,0,0,0,0,0,1,0, 
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,7,0,0,0,0,0,0,0,0,0,0,
        0,2,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,6,0,0,0,0,0,0,0,0,0,0,0,1,5,0,0,0,7,2,0,0,0,2,7,
        0,7,0,0,0,0,0,5,7,0, 
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0, 
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,1,0,0,0,7,0,0,0,0,0,0,0,0,0,0,
        0,2,0,0,8,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,8,8,8,1,0,
        0,0,0,0,0,8,0,0,0,7,0,0,0,0,0,8,0,0,0,7,0,0,0,0,2,1,7,0,0,7,
        0,7,0,0,0,0,7,7,7,7, 
      0,0,0,0,0,0,0,0,0,0,0,8,0,0,0,0,8,0,8,0,0,0,6,0,0,0,6,0,6,0,
        0,0,0,0,8,0,8,0,8,0,0,0,0,8,0,0,8,0,8,0,0,0,0,0,0,8,8,0,8,0,
        0,8,6,8,8,8,6,8,6,8,0,0,0,0,0,0,8,8,8,0,0,8,6,8,8,8,6,8,6,8,
        0,0,0,0,0,0,8,0,8,8, 
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,
        0,0,0,0,0,3,0,3,0,3,0,0,0,0,0,9,0,5,0,9,0,0,0,9,4,0,0,0,4,9,
        0,0,0,0,0,0,0,0,6,0,0,0,0,5,4,0,0,0,4,5,0,0,0,0,0,3,0,3,0,3,
		0,0,4,9,4,9,0,5,4,9};
 transl_table = vector<unsigned char>(trans_table_h, trans_table_h + (sizeof trans_table_h / sizeof trans_table_h[0]));
	
}

void MooreFSSP::transition(vector<intcellstate> conf)
{
  unsigned int ix;
  ix = 100*conf[0]+10*conf[1]+conf[2];
  state = transl_table[ix];
}

void MooreFSSP::configure(Config * conf,string toptag){}

// I think now, that the solution from F. Lunnon is not quite correct, I am not sure, what the firing state is, 
// red or black, if it is red, how does the leftmost cell know, to go to firing, the right neighbors state 
// was several time the same. 

// let's try from Thomas Worsch' script tables:


MazoyerFSSP::MazoyerFSSP() 
{  
	 state_num = 8;
	 // x is an illegal state, used to mark the use of a non-specified transition 
	display_table = string("$_abcGFx");
 unsigned char trans_table_h[]	 
	=   // the script does not give values for the successors of #, so keep it constant
	{0,0,0,0,0,0,0,7,  0,0,0,0,0,0,0,7,  0,0,0,0,0,0,0,7,  0,0,0,0,0,0,0,7,  0,0,0,0,0,0,0,7,  0,0,0,0,0,0,0,7,  0,0,0,0,0,0,0,7,  7,7,7,7,7,7,7,7, 
	 
	 7,1,7,7,7,7,7,7,  1,1,7,1,1,1,7,7,  4,5,1,1,1,4,7,7,  1,1,1,1,1,1,7,7,  5,2,1,1,1,5,7,7,  2,4,1,1,1,2,7,7,  7,7,7,7,7,7,7,7,  7,7,7,7,7,7,7,7,

	 7,7,6,7,5,7,7,7,  7,7,2,1,5,7,7,7,  6,2,2,3,4,3,7,7,  4,5,7,5,4,4,7,7,  7,2,2,7,7,7,7,7,  4,7,7,7,4,4,7,7,  7,7,7,7,7,7,7,7,  7,7,7,7,7,7,7,7,

	 7,7,7,7,7,7,7,7,  7,7,5,3,1,3,7,7,  7,5,3,3,1,7,7,7,  7,5,2,3,4,3,7,7,  1,1,2,7,7,1,7,7,  5,4,4,7,3,5,7,7,  7,7,7,7,7,7,7,7,  7,7,7,7,7,7,7,7,

	 7,7,7,7,7,7,7,7,  7,4,2,5,4,5,7,7,  3,3,7,3,7,3,7,7,  5,4,7,7,4,5,7,7,  7,4,2,3,4,3,7,7,  3,3,7,3,7,3,7,7,  7,7,7,7,7,7,7,7,  7,7,7,7,7,7,7,7,

	 7,2,7,5,5,6,7,7,  7,7,5,5,5,7,7,7,  7,3,7,5,5,7,7,7,  5,3,7,5,5,5,7,7,  2,2,7,5,5,2,7,7,  6,3,7,5,5,6,7,7,  7,7,7,7,7,7,7,7,  7,7,7,7,7,7,7,7,
	
	 7,7,7,7,7,7,7,7,  7,7,7,7,7,7,7,7,  7,7,7,7,7,7,7,7,  7,7,7,7,7,7,7,7,  7,7,7,7,7,7,7,7,  7,7,7,7,7,7,7,7,  7,7,7,7,7,7,7,7,  7,7,7,7,7,7,7,7,

	 7,7,7,7,7,7,7,7,  7,7,7,7,7,7,7,7,  7,7,7,7,7,7,7,7,  7,7,7,7,7,7,7,7,  7,7,7,7,7,7,7,7,  7,7,7,7,7,7,7,7,  7,7,7,7,7,7,7,7,  7,7,7,7,7,7,7,7
	};
      

/* 
MazoyerFSSP::MazoyerFSSP() : state_num(7)
{
  display_table = string("_LGABCX");
 unsigned char trans_table_h[]	 
	= 
    {0,0,0,0,0,0,0,  0,0,0,0,0,0,0,  0,0,0,0,0,0,0,  0,0,0,0,0,0,0,  0,0,0,0,0,0,0,  0,0,0,0,0,0,0,  0,0,0,0,0,0,0,
     
     0,0,0,0,0,0,0,  0,1,1,0,1,1,1,  0,5,3,1,1,1,3,  0,2,5,1,1,1,5,  0,1,1,1,1,1,1,  0,3,2,1,1,1,2,  0,2,0,0,0,0,2,
     
     0,0,0,0,0,0,0,  0,0,0,2,2,2,0,  0,4,6,0,2,2,6,  0,4,0,0,2,2,0,  0,4,2,0,2,2,2,  0,3,3,0,2,2,3,  0,3,6,0,2,2,6,
     
     0,0,0,0,0,0,0,  0,0,0,3,1,2,0,  0,0,5,0,0,5,5,  0,3,4,3,4,5,6,  0,2,5,0,2,5,5,  0,3,0,3,0,0,0,  0,0,0,6,0,2,0,
     
     0,0,0,0,0,0,0,  0,0,4,2,4,1,0,  0,5,2,5,0,4,2,  0,2,0,4,4,1,0,  0,2,4,3,4,5,0,  0,1,1,3,0,0,1,  0,0,0,0,0,0,0,
     
     0,0,0,0,0,0,0,  0,5,2,3,2,5,0,  0,4,4,0,4,0,4,  0,4,4,0,4,0,4,  0,5,2,0,0,5,2,  0,5,4,3,4,5,0,  0,0,0,0,0,0,0,

// I think I have to change the last line in comparison with CellDefn.java, 
//   because I would also update the border cells, and this would destroy the whole field!
//     0,0,0,0,0,0,0,  0,0,0,0,0,0,0,  0,0,0,0,0,0,0,  0,0,0,0,0,0,0,  0,0,0,0,0,0,0,  0,0,0,0,0,0,0,  0,0,0,0,0,0,1}; 
//
     6,6,6,6,6,6,6,  6,6,6,6,6,6,6,  6,6,6,6,6,6,6,  6,6,6,6,6,6,6,  6,6,6,6,6,6,6,  6,6,6,6,6,6,6,  6,6,6,6,6,6,6};
	 
*/	 
	  
 transl_table = vector<unsigned char>(trans_table_h, trans_table_h + (sizeof trans_table_h / sizeof trans_table_h[0]));
}

void MazoyerFSSP::transition(vector<intcellstate> conf)
{
  unsigned int ix;
  ix = 64*conf[0]+conf[1]+8*conf[2];
  state = transl_table[ix];
}

void MazoyerFSSP::configure(Config * conf,string toptag){}

Yunes1FSSP::Yunes1FSSP() 
{  
	 state_num = 9;
	 // the x state is used as illegal state here
	  // don't know why I did it this way, but this is row wise, compared with the paper
	display_table = string("_GZACd$Fx");
 unsigned char trans_table_h[]	 
	= 
	{0,2,2,0,3,0,0,7,8,  2,8,8,8,8,2,2,7,8,  2,8,2,8,8,2,2,7,8,  0,8,2,8,8,0,8,7,8,  3,8,8,8,8,8,8,7,8,  0,2,2,0,8,0,0,7,8,  0,8,2,0,8,0,8,7,8, 7,7,7,7,7,7,7,7,8,  8,8,8,8,8,8,8,8,8,
	 1,1,8,8,8,8,8,7,8,  1,8,0,1,4,5,8,7,8,  8,0,0,8,8,8,8,7,8,  8,1,8,1,8,8,8,7,8,  8,4,8,8,4,8,8,7,8,  8,5,8,8,8,5,8,7,8,  1,8,4,8,8,8,7,7,8, 7,7,7,7,7,7,7,7,8,  8,8,8,8,8,8,8,8,8, 
     5,5,5,8,8,5,5,7,8,  5,8,8,3,2,8,4,7,8,  5,8,8,8,8,1,8,7,8,  8,5,8,8,8,5,3,7,8,  8,2,8,8,8,2,4,7,8,  5,8,1,3,2,1,2,7,8,  5,8,8,3,4,2,8,7,8, 7,7,7,7,7,7,7,7,8,  8,8,8,8,8,8,8,8,8,
     8,8,8,5,0,5,8,7,8,  8,8,8,8,8,4,8,7,8,  8,8,8,8,8,4,8,7,8,  4,8,8,8,8,8,8,7,8,  8,8,4,8,8,3,4,7,8,  3,4,4,8,8,5,8,7,8,  8,8,8,5,4,8,8,7,8, 7,7,7,7,7,7,7,7,8,  8,8,8,8,8,8,8,8,8,
	 8,8,8,8,8,5,8,7,8,  8,4,8,4,4,8,8,7,8,  8,8,8,8,4,2,8,7,8,  8,4,8,4,8,2,8,7,8,  8,4,4,8,7,5,7,7,8,  5,8,2,8,5,5,8,7,8,  8,8,8,3,7,4,8,7,8, 7,7,7,7,7,7,7,7,8,  8,8,8,8,8,8,8,8,8,
	 8,4,0,3,0,4,8,7,8,  4,8,8,8,8,8,8,7,8,  0,8,4,0,3,4,0,7,8,  5,8,0,1,8,1,8,7,8,  0,8,3,8,0,0,8,7,8,  5,8,4,1,0,8,8,7,8,  8,8,0,8,0,8,8,7,8, 7,7,7,7,7,7,7,7,8,  8,8,8,8,8,8,8,8,8,
     6,6,6,6,6,6,6,6,8,  6,6,6,6,6,6,6,6,8,  6,6,6,6,6,6,6,6,8,  6,6,6,6,6,6,6,6,8,  6,6,6,6,6,6,6,6,8,  6,6,6,6,6,6,6,6,8,  6,6,6,6,6,6,6,6,8, 6,6,6,6,6,6,6,6,8,  8,8,8,8,8,8,8,8,8,
	 7,7,7,7,7,7,7,7,8,  7,7,7,7,7,7,7,7,8,  7,7,7,7,7,7,7,7,8,  7,7,7,7,7,7,7,7,8,  7,7,7,7,7,7,7,7,8,  7,7,7,7,7,7,7,7,8,  7,7,7,7,7,7,7,7,8, 7,7,7,7,7,7,7,7,8,  8,8,8,8,8,8,8,8,8,
	 8,8,8,8,8,8,8,8,8,  8,8,8,8,8,8,8,8,8,  8,8,8,8,8,8,8,8,8,  8,8,8,8,8,8,8,8,8,  8,8,8,8,8,8,8,8,8,  8,8,8,8,8,8,8,8,8,  8,8,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,8,8,  8,8,8,8,8,8,8,8,8
	};
	  
 transl_table = vector<unsigned char>(trans_table_h, trans_table_h + (sizeof trans_table_h / sizeof trans_table_h[0]));
}

void Yunes1FSSP::transition(vector<intcellstate> conf)
{
  unsigned int ix;
  ix = 81*conf[0]+conf[1]+9*conf[2];
  state = transl_table[ix];
}

void Yunes1FSSP::configure(Config * conf,string toptag){}

Yunes2FSSP::Yunes2FSSP() 
{  
	 state_num = 9;
	 // the x state is used as illegal state here
	display_table = string("_GZACd$Fx");
 unsigned char trans_table_h[]	 
	= 
		// this time I did it column wise, hence the other indexing scheme!
	{0,0,2,0,3,0,0,7,8,  1,1,8,8,8,8,8,7,8,  5,5,8,5,8,5,5,7,8,  0,4,2,5,8,5,8,7,8,  8,5,8,8,8,5,8,7,8,  8,8,0,3,0,4,8,7,8,  6,6,6,6,6,6,6,6,8, 7,7,7,7,7,7,7,7,8,  8,8,8,8,8,8,8,8,8,
	 0,8,2,0,8,0,8,7,8,  1,7,1,1,1,1,7,7,8,  5,8,1,2,1,2,8,7,8,  4,8,2,5,8,3,8,7,8,  5,8,0,8,8,8,8,7,8,  2,2,0,3,0,8,8,7,8,  6,6,6,6,6,6,6,6,8, 7,7,7,7,7,7,7,7,8,  8,8,8,8,8,8,8,8,8, 
     2,2,8,2,8,2,2,7,8,  8,1,1,8,8,5,8,7,8,  5,1,8,8,8,8,1,7,8,  2,2,8,8,8,1,8,7,8,  8,0,1,8,1,3,8,7,8,  0,0,4,0,3,4,0,7,8,  6,6,6,6,6,6,6,6,8, 7,7,7,7,7,7,7,7,8,  8,8,8,8,8,8,8,8,8,
	 0,0,2,8,8,0,8,7,8,  8,1,8,1,8,8,8,7,8,  5,2,5,8,8,2,2,7,8,  4,8,8,8,8,8,8,7,8,  8,8,8,8,8,8,8,7,8,  5,5,0,8,8,8,8,7,8,  6,6,6,6,6,6,6,6,8, 7,7,7,7,7,7,7,7,8,  8,8,8,8,8,8,8,8,8,
	 3,8,8,8,8,8,8,7,8,  8,1,8,8,1,8,8,7,8,  8,1,8,8,8,2,1,7,8,  8,8,8,8,8,8,8,7,8,  8,8,1,8,8,1,8,7,8,  0,0,3,8,8,8,8,7,8,  6,6,6,6,6,6,6,6,8, 7,7,7,7,7,7,7,7,8,  8,8,8,8,8,8,8,8,8,
	 0,0,8,0,8,8,0,7,8,  8,1,8,1,8,1,8,7,8,  5,2,8,2,2,8,2,7,8,  3,3,2,8,8,8,8,7,8,  5,8,3,8,1,1,8,7,8,  5,5,4,8,8,8,8,7,8,  6,6,6,6,6,6,6,6,8, 7,7,7,7,7,7,7,7,8,  8,8,8,8,8,8,8,8,8,
     8,8,8,8,8,8,8,6,8,  1,7,1,1,1,1,8,6,8,  5,8,8,8,8,8,7,6,8,  8,8,8,8,8,8,8,6,8,  8,8,1,8,8,1,8,6,8,  8,8,4,8,8,8,8,6,8,  6,6,6,6,6,6,6,6,8, 7,7,7,7,7,7,7,7,8,  8,8,8,8,8,8,8,8,8,
	 7,7,7,7,7,7,7,7,8,  7,7,7,7,7,7,7,7,8,  7,7,7,7,7,7,7,7,8,  7,7,7,7,7,7,7,7,8,  7,7,7,7,7,7,7,7,8,  7,7,7,7,7,7,7,7,8,  6,6,6,6,6,6,6,6,8, 7,7,7,7,7,7,7,7,8,  8,8,8,8,8,8,8,8,8,
	 8,8,8,8,8,8,8,8,8,  8,8,8,8,8,8,8,8,8,  8,8,8,8,8,8,8,8,8,  8,8,8,8,8,8,8,8,8,  8,8,8,8,8,8,8,8,8,  8,8,8,8,8,8,8,8,8,  8,8,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,8,8,  8,8,8,8,8,8,8,8,8
	};
	  
 transl_table = vector<unsigned char>(trans_table_h, trans_table_h + (sizeof trans_table_h / sizeof trans_table_h[0]));
}

void Yunes2FSSP::transition(vector<intcellstate> conf)
{
  unsigned int ix;
  ix = 81*conf[0]+conf[2]+9*conf[1];
  state = transl_table[ix];
}

void Yunes2FSSP::configure(Config * conf,string toptag){}

Gerken::Gerken() 
{  
	 state_num = 9;
	 // the x state is used as illegal state here
	display_table = string("_GZACd$Fx");
 unsigned char trans_table_h[]	 
	= 
		// this time I did it column wise, hence the other indexing scheme!
	{0,8,8,0,0,0,0,7,8,  0,3,8,3,5,4,8,7,8,  3,8,3,1,2,8,8,7,8,  8,8,8,8,8,8,8,8,8,  4,8,8,4,8,4,4,8,8,  5,8,5,8,5,8,5,8,8,  6,6,6,6,6,6,6,6,8, 7,7,7,7,7,7,7,7,8,  8,8,8,8,8,8,8,8,8,
	 2,8,8,2,8,2,4,7,8,  2,3,8,3,8,4,8,7,8,  8,8,8,8,8,8,8,7,8,  2,2,8,2,8,4,8,8,8,  2,7,8,2,8,2,7,8,8,  8,8,8,8,8,8,8,8,8,  6,6,6,6,6,6,6,6,8, 7,7,7,7,7,7,7,7,8,  8,8,8,8,8,8,8,8,8, 
     1,8,8,1,1,8,5,7,8,  1,8,3,3,5,8,8,7,8,  3,8,3,1,2,8,8,7,8,  8,8,8,8,8,8,8,8,8,  8,8,8,8,8,8,8,8,8,  1,8,1,8,1,8,1,8,8,  6,6,6,6,6,6,6,6,8, 7,7,7,7,7,7,7,7,8,  8,8,8,8,8,8,8,8,8,
	 3,3,8,3,5,4,8,7,8,  0,3,3,3,5,4,8,7,8,  3,8,3,1,2,8,8,7,8,  0,0,8,2,8,4,8,8,8,  3,4,8,0,8,0,4,8,8,  3,8,5,8,5,8,5,8,8,  6,6,6,6,6,6,6,6,8, 7,7,7,7,7,7,7,7,8,  8,8,8,8,8,8,8,8,8,
	 0,8,8,0,0,8,8,7,8,  0,8,8,8,8,8,8,7,8,  1,8,1,8,7,8,7,7,8,  8,8,8,8,8,8,8,8,8,  8,8,8,8,8,8,8,8,8,  3,8,5,8,5,8,5,8,8,  6,6,6,6,6,6,6,6,8, 7,7,7,7,7,7,7,7,8,  8,8,8,8,8,8,8,8,8,
	 0,8,8,8,8,0,8,7,8,  8,8,8,8,8,8,8,7,8,  8,8,8,8,8,8,8,7,8,  2,2,8,2,8,4,8,8,8,  0,4,8,0,8,0,4,8,8,  8,8,8,8,8,8,8,8,8,  6,6,6,6,6,6,6,6,8, 7,7,7,7,7,7,7,7,8,  8,8,8,8,8,8,8,8,8,
     8,8,8,8,8,8,8,6,8,  8,8,8,3,1,8,2,7,8,  1,8,1,1,7,8,8,7,8,  2,2,8,2,8,4,8,8,8,  8,7,8,8,8,8,8,8,8,  8,8,8,8,8,8,8,8,8,  6,6,6,6,6,6,6,6,8, 7,7,7,7,7,7,7,7,8,  8,8,8,8,8,8,8,8,8,
	 7,7,7,7,7,7,7,7,8,  7,7,7,7,7,7,7,7,8,  7,7,7,7,7,7,7,7,8,  8,8,8,8,8,8,8,8,8,  8,8,8,8,8,8,8,8,8,  8,8,8,8,8,8,8,8,8,  6,6,6,6,6,6,6,6,8, 7,7,7,7,7,7,7,7,8,  8,8,8,8,8,8,8,8,8,
	 8,8,8,8,8,8,8,8,8,  8,8,8,8,8,8,8,8,8,  8,8,8,8,8,8,8,8,8,  8,8,8,8,8,8,8,8,8,  8,8,8,8,8,8,8,8,8,  8,8,8,8,8,8,8,8,8,  8,8,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,8,8,  8,8,8,8,8,8,8,8,8
	};
	  
 transl_table = vector<unsigned char>(trans_table_h, trans_table_h + (sizeof trans_table_h / sizeof trans_table_h[0]));
}

void Gerken::transition(vector<intcellstate> conf)
{
  unsigned int ix;
  ix = 81*conf[0]+conf[2]+9*conf[1];
  state = transl_table[ix];
}


void Gerken::configure(Config * conf,string toptag){}

Balzer::Balzer() 
{  
	 state_num = 10;
	 // the x state is used as illegal state here
	display_table = string("ABC_GQR$Fx");
	unsigned char trans_table_h[]	 
	= 
{
0,9,9,0,9,0,9,9,8,9,
2,9,9,2,6,2,9,9,8,9,
2,9,9,2,5,2,9,9,8,9,
3,9,9,9,9,3,9,9,8,9,
1,9,9,1,9,1,9,9,8,9,
0,9,9,0,5,0,9,9,8,9,
3,9,9,9,9,3,9,9,8,9,
9,9,9,9,9,9,9,9,8,9,
8,8,8,8,8,8,8,8,8,9,
9,9,9,9,9,9,9,9,9,9,

9,9,9,9,9,9,9,9,8,9,
5,1,1,6,0,5,6,9,8,9,
5,1,1,6,0,5,6,9,8,9,
9,9,9,9,9,9,9,9,8,9,
3,9,2,9,9,2,9,9,8,9,
9,9,9,9,9,9,9,9,8,9,
5,1,1,9,0,5,9,9,8,9,
9,9,9,9,9,9,9,9,8,9,
8,8,8,8,8,8,8,8,8,9,
9,9,9,9,9,9,9,9,9,9,

9,9,9,9,9,9,9,9,8,9,
9,2,6,6,9,4,2,9,8,9,
4,2,6,6,9,4,2,9,8,9,
3,9,9,9,9,3,9,9,8,9,
3,9,2,2,4,4,2,9,8,9,
9,9,9,9,9,9,9,9,8,9,
9,2,1,1,9,4,2,9,8,9,
9,9,9,9,9,9,9,9,8,9,
8,8,8,8,8,8,8,8,8,9,
9,9,9,9,9,9,9,9,9,9,

9,9,9,0,9,5,9,9,8,9,
9,9,6,9,5,9,9,9,8,9,
2,9,6,2,5,2,9,4,8,9,
3,9,9,3,9,3,9,3,8,9,
2,9,2,2,4,2,9,4,8,9,
3,9,9,3,9,3,9,9,8,9,
9,9,1,0,0,5,9,9,8,9,
9,9,9,9,9,9,9,9,8,9,
8,8,8,8,8,8,8,8,8,9,
9,9,9,9,9,9,9,9,9,9,

9,4,4,9,4,9,9,4,8,9,
9,9,9,9,4,9,4,4,8,9,
9,9,9,9,4,4,4,4,8,9,
4,9,9,4,9,4,9,9,8,9,
4,4,4,4,8,4,9,8,8,9,
9,4,4,9,4,9,9,4,8,9,
9,9,9,4,4,9,4,4,8,9,
4,4,4,4,8,4,9,8,8,9,
8,8,8,8,8,8,8,8,8,9,
9,9,9,9,9,9,9,9,9,9,

5,9,9,5,9,5,9,9,8,9,
9,9,9,9,6,4,6,9,8,9,
4,9,9,4,4,4,6,9,8,9,
5,9,9,5,5,5,9,9,8,9,
9,9,9,9,4,9,9,9,8,9,
3,9,9,0,5,3,9,9,8,9,
3,9,9,0,5,3,9,9,8,9,
9,9,9,9,9,9,9,9,8,9,
8,8,8,8,8,8,8,8,8,9,
9,9,9,9,9,9,9,9,9,9,

9,9,9,9,9,9,9,9,8,9,
9,9,6,9,5,5,6,9,8,9,
9,2,6,2,5,5,6,9,8,9,
9,9,9,9,9,9,9,9,8,9,
2,9,9,1,4,2,9,9,8,9,
3,9,9,0,5,3,9,9,8,9,
9,1,6,1,5,5,6,9,8,9,
9,9,9,9,4,4,6,9,8,9,
8,8,8,8,8,8,8,8,8,9,
9,9,9,9,9,9,9,9,9,9,

7,7,7,7,7,7,7,7,7,7,
7,7,7,7,7,7,7,7,7,7,
7,7,7,7,7,7,7,7,7,7,
7,7,7,7,7,7,7,7,7,7,
7,7,7,7,7,7,7,7,7,7,
7,7,7,7,7,7,7,7,7,7,
7,7,7,7,7,7,7,7,7,7,
7,7,7,7,7,7,7,7,7,7,
7,7,7,7,7,7,7,7,7,7,
7,7,7,7,7,7,7,7,7,7,

8,8,8,8,8,8,8,8,8,9,
8,8,8,8,8,8,8,8,8,9,
8,8,8,8,8,8,8,8,8,9,
8,8,8,8,8,8,8,8,8,9,
8,8,8,8,8,8,8,8,8,9,
8,8,8,8,8,8,8,8,8,9,
8,8,8,8,8,8,8,8,8,9,
8,8,8,8,8,8,8,8,8,9,
8,8,8,8,8,8,8,8,8,9,
9,9,9,9,9,9,9,9,9,9,

9,9,9,9,9,9,9,9,9,9,
9,9,9,9,9,9,9,9,9,9,
9,9,9,9,9,9,9,9,9,9,
9,9,9,9,9,9,9,9,9,9,
9,9,9,9,9,9,9,9,9,9,
9,9,9,9,9,9,9,9,9,9,
9,9,9,9,9,9,9,9,9,9,
9,9,9,9,9,9,9,9,9,9,
9,9,9,9,9,9,9,9,9,9,
9,9,9,9,9,9,9,9,9,9

};



 /*
	= 
		// this time I did it column wise, hence the other indexing scheme!
	{0,0,2,0,3,0,0,7,8,  1,1,8,8,8,8,8,7,8,  5,5,8,5,8,5,5,7,8,  0,4,2,5,8,5,8,7,8,  8,5,8,8,8,5,8,7,8,  8,8,0,3,0,4,8,7,8,  6,6,6,6,6,6,6,6,8, 7,7,7,7,7,7,7,7,8,  8,8,8,8,8,8,8,8,8,
	 0,8,2,0,8,0,8,7,8,  1,7,1,1,1,1,7,7,8,  5,8,1,2,1,2,8,7,8,  4,8,2,5,8,3,8,7,8,  5,8,0,8,8,8,8,7,8,  2,2,0,3,0,8,8,7,8,  6,6,6,6,6,6,6,6,8, 7,7,7,7,7,7,7,7,8,  8,8,8,8,8,8,8,8,8, 
     2,2,8,2,8,2,2,7,8,  8,1,1,8,8,5,8,7,8,  5,1,8,8,8,8,1,7,8,  2,2,8,8,8,1,8,7,8,  8,0,1,8,1,3,8,7,8,  0,0,4,0,3,4,0,7,8,  6,6,6,6,6,6,6,6,8, 7,7,7,7,7,7,7,7,8,  8,8,8,8,8,8,8,8,8,
	 0,0,2,8,8,0,8,7,8,  8,1,8,1,8,8,8,7,8,  5,2,5,8,8,2,2,7,8,  4,8,8,8,8,8,8,7,8,  8,8,8,8,8,8,8,7,8,  5,5,0,8,8,8,8,7,8,  6,6,6,6,6,6,6,6,8, 7,7,7,7,7,7,7,7,8,  8,8,8,8,8,8,8,8,8,
	 3,8,8,8,8,8,8,7,8,  8,1,8,8,1,8,8,7,8,  8,1,8,8,8,2,1,7,8,  8,8,8,8,8,8,8,7,8,  8,8,1,8,8,1,8,7,8,  0,0,3,8,8,8,8,7,8,  6,6,6,6,6,6,6,6,8, 7,7,7,7,7,7,7,7,8,  8,8,8,8,8,8,8,8,8,
	 0,0,8,0,8,8,0,7,8,  8,1,8,1,8,1,8,7,8,  5,2,8,2,2,8,2,7,8,  3,3,2,8,8,8,8,7,8,  5,8,3,8,1,1,8,7,8,  5,5,4,8,8,8,8,7,8,  6,6,6,6,6,6,6,6,8, 7,7,7,7,7,7,7,7,8,  8,8,8,8,8,8,8,8,8,
     8,8,8,8,8,8,8,6,8,  1,7,1,1,1,1,8,6,8,  5,8,8,8,8,8,7,6,8,  8,8,8,8,8,8,8,6,8,  8,8,1,8,8,1,8,6,8,  8,8,4,8,8,8,8,6,8,  6,6,6,6,6,6,6,6,8, 7,7,7,7,7,7,7,7,8,  8,8,8,8,8,8,8,8,8,
	 7,7,7,7,7,7,7,7,8,  7,7,7,7,7,7,7,7,8,  7,7,7,7,7,7,7,7,8,  7,7,7,7,7,7,7,7,8,  7,7,7,7,7,7,7,7,8,  7,7,7,7,7,7,7,7,8,  6,6,6,6,6,6,6,6,8, 7,7,7,7,7,7,7,7,8,  8,8,8,8,8,8,8,8,8,
	 8,8,8,8,8,8,8,8,8,  8,8,8,8,8,8,8,8,8,  8,8,8,8,8,8,8,8,8,  8,8,8,8,8,8,8,8,8,  8,8,8,8,8,8,8,8,8,  8,8,8,8,8,8,8,8,8,  8,8,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,8,8,  8,8,8,8,8,8,8,8,8
	};
*/


 transl_table = vector<unsigned char>(trans_table_h, trans_table_h + (sizeof trans_table_h / sizeof trans_table_h[0]));
}

void Balzer::transition(vector<intcellstate> conf)
{
  unsigned int ix;
  ix = 100*conf[0]+10*conf[2]+conf[1];
  state = transl_table[ix];
}

void Balzer::configure(Config * conf,string toptag){}

Life::Life()  {state_num = 2; }

void Life::configure(Config * cfg,string toptag)
{
unsigned comb;
	//	bornl,bornh,diel,dieh
std::istringstream stm; bool error;
    vector<string> keys(4,"");
    keys[0] = toptag; keys[1] = "CellAut";keys[2] = "Transition"; 
  
   keys[3]="Rule";
   stm.str(cfg->GetDataByFormat("EEEA",keys,error));
    stm>>comb;
	bornl=comb%10;comb=comb/10;
    bornh=comb%10;comb=comb/10;
    diel=comb%10;comb=comb/10;
    dieh=comb%10;


}

void Life::transition(std::vector<intcellstate> curr_state)
{
unsigned count,i;
count = 0;
state = curr_state[0];
for (i=1;i<curr_state.size();i++) count=count+(1&curr_state[i]);
 if (curr_state[0] && ((count>dieh) || (count <diel))) state = 0;
 else
  if (!curr_state[0] && (count>=bornl) && (count <=bornh)) state = 1;    
}

BriansBrain::BriansBrain() : Life() {state_num = 3; }

void BriansBrain::configure(Config * cfg,string toptag)
{
  Life::configure(cfg,toptag);
}

void BriansBrain::transition(std::vector<intcellstate> curr_state)
{
unsigned count,i;
count = 0;
state = curr_state[0];
// states: 0 ready, 1 firing, 2 refractory
// 1->2 and 2->0 are fixed, 0->1 if ==2 neighb are firing, use config vars for an interval
 if (state) {state = (state+1) % 3;}
 else
   {
  for (i=1;i<curr_state.size();i++) count=count+(curr_state[i]==1);
  if ((count>=bornl) && (count <=bornh)) state = 1;    
   }
}



string CellAutState::conf2string(intcellstate* conf,int size)
{
	string res;
	int i;
	for (i=0;i<size;i++)
	{
		res.push_back(display_table[conf[i]]);
	}
	return res;
}


CellAut * CreateNamedCellAut(Config * conf,string toptag)
{
CellAut * res = new CellAut;

  std::istringstream stm;
    
  bool error;
vector<string> keys(4,"");
string TopoName;
string CAstateName;
int ca_dim;
int pos; string pos_string;
keys[0] = toptag;

  keys[1] = "CellAut";keys[2] = "Topology";keys[3]="type";
  TopoName = conf->GetDataByFormat("EEEA",keys,error);
  keys[3] = "dimension";
  stm.str(conf->GetDataByFormat("EEEA",keys,error)); stm >> ca_dim;

if (TopoName=="Moore" && ca_dim == 1)
  {keys[3]="pos"; pos_string = conf->GetDataByFormat("EEEA",keys,error);
stm.str(pos_string); stm >> pos;
	res->CALocation = new OneD_direct(pos);
}else if (TopoName=="Moore" && ca_dim == 2)
{
	res->CALocation = new TwoD_moore(0,0);
	// further topologies here,
};
keys[2] = "Transition";keys[3]="type";
 CAstateName = conf->GetDataByFormat("EEEA",keys,error);
if (CAstateName == "TwoStates" && ca_dim == 1)
{
int ix;
 keys[3]="ix"; stm.str( conf->GetDataByFormat("EEEA",keys,error));
 stm >> ix;
 res->CState = new TwoStates();


}else if (CAstateName == "r2a")
{
res->CState = new r2a();
}else if (CAstateName == "BinaryFSSP")
{
res->CState = new BinaryFSSP();
}else if (CAstateName == "Mazoyer")
{
res->CState = new MazoyerFSSP();
}else if (CAstateName == "Yunes1")
{
res->CState = new Yunes1FSSP();
}else if (CAstateName == "Yunes2")
{
res->CState = new Yunes2FSSP();
}else if (CAstateName == "Gerken")
{
res->CState = new Gerken();
}else if (CAstateName == "Balzer")
{
res->CState = new Balzer();
}else if (CAstateName == "Life")
{res->CState = new Life();
}else if (CAstateName == "BriansBrain")
{res->CState = new BriansBrain();
}




res->CState->configure(conf,toptag);
return res;
}

vector<CellAutLocation *> location_list_conf(CellAut* theca,Config * conf, string parent_tag)
{

  bool error;
vector<string> keys(4,"");

keys[0] = parent_tag; keys[1] = "Complexity";keys[2] = "TargetSet";

keys[3] = "locations";
  	
string confs=conf->GetDataByFormat("EEEA",keys,error);
return location_list(theca,confs);
}


#include <cstring>
vector<CellAutLocation *> location_list(CellAut* theca, string conf_string)
{
vector<CellAutLocation *> res;

string ws = conf_string;
string e;


 size_t pos,tpos;
tpos=0;
while (tpos<ws.size())
{
	tpos=ws.find_first_not_of(" \t\x0d\0x0a",tpos);
	pos=ws.find_first_of("/",tpos);
	e=ws.substr(tpos,pos-tpos);
	tpos=pos==string::npos?string::npos:pos+1;
	res.push_back(theca->CALocation->factory(e));
}
return res;
}




