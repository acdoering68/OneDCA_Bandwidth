#ifndef conf_set_h
#define conf_set_h


#include "ca.h"
#include <set>
#include <vector>
#include <string>

using namespace std;


class Conf_set
{
public:

    virtual void clear()=0; 
    virtual void add(intcellstate * conf)=0;
	unsigned long int count;
    unsigned int size; 
	virtual string dump(CellAut* theca)=0;
};

/* default implementation with STL hash-set */


class Conf_set_hash : public Conf_set
{
public: 
	void clear();  
    void add(intcellstate * conf);
	virtual string dump(CellAut* theca);
	
private: 
	
    set<vector<intcellstate> > store;
};




#endif
