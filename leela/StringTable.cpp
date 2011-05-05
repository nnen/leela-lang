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
	s << "string_" << _list.size();
	return s.str();
}

StringTable::StringTable()
	: Object()
{
}

string StringTable::getLabel(Ref<String> str)
{
	if (_labels.count(str->getValue()) > 0)
		return _labels[str->getValue()];
	
	_list.push_back(str);
	_addresses[str->getValue()] = 0;
	return 0;
}

vector<Ref<String> >& StringTable::getStrings()
{
	return _list;
}

