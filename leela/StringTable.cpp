/**
 * \file   StringTable.cpp
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-05-03
 *
 * \brief  
 */

#include "StringTable.h"

string StringTable::nextLabel()
{
	stringstream s;
	s << "string_" << _list.size();
	return s.str();
}

StringTable::StringTable()
	: Object()
{
}

int StringTable::getIndex(Ref<String> str)
{
	if (_index.count(str->getValue()) > 0)
		return _index[str->getValue];

	int index = _list.size();
	_list.push_back(str);
	_labels[str->getValue()] = index;
	return index;
}

string StringTable::getLabel(Ref<String> str)
{
	stringstream s;
	int index = getIndex(str);
	
	s << "string_" << index;

	return s.str();
}

vector<Ref<String> >& StringTable::getStrings()
{
	return _list;
}

