
#include "conf_set.h"

void Conf_set_hash::clear()
{

	count = 0; 
	store.clear();
}

void Conf_set_hash::add(intcellstate * conf)
{

vector<intcellstate> conv_asvec(size,0);
unsigned int i;
for (i=0;i<size;i++){conv_asvec[i] = conf[i];}
pair<set<vector<intcellstate> >::iterator,bool> ins_res =   store.insert( conv_asvec );

if (ins_res.second) count++;
}
 // ca needed for converting the states into characters;
string Conf_set_hash::dump(CellAut* theca)
{
	bool first=true;
	string res;
	int i;
	set<vector<intcellstate> >::iterator it;
	for (it=store.begin();it!=store.end();it++)
	{
		if (!first) {res.push_back(',');}
		first=false;
      for (i=0;i<(*it).size();i++)
	  {
		  res.push_back(theca->CState->display_table[(*it)[i]]);
	  }
	}
	return res;
}


ostream& operator<<(ostream& os,const Conf_set& cos) { return os;}