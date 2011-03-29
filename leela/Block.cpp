/**
 * \file   leela/Block.cpp
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-03-28
 *
 * \brief  Contains implementation of the Block class.
 */

#include "Block.h"

void SymbolTable::add(string symbol)
{
	if (_map.count(symbol) > 0) return;
	
	_list.push_back(symbol);
	_map[symbol] = 0;
}

void SymbolTable::compile()
{
	int address = 0;
	
	foreach(sym, _list)
		_map[sym] = address++;
}

