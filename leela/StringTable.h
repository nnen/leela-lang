/**
 * \file   StringTable.h
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-05-03
 *
 * \brief  
 */

#ifndef STRING_TABLE_H_2356BDFDVF324SDc
#define STRING_TABLE_H_2356BDFDVF324SDc

#include <vector>
#include <map>
#include <string>
#include <sstream>

#include "Object.h"
#include "Value.h"

class StringTable : public Object {
private:
	vector<Ref<String> > _list;
	map<string, int>     _index;
	
	map<string, string>  _labels;
	
	string nextLabel();

public:
	StringTable();
	virtual ~StringTable() {}

	int getIndex(Ref<String> str);
	string getLabel(Ref<String> str);
	
	vector<Ref<String> >& getStrings();
};

#endif /* end of include guard: STRING_TABLE_H_2356BDFDVF324SDc */

