/**
 * \file   leela/Block.cpp
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-03-28
 *
 * \brief  Contains implementation of the Block class.
 */

#include "Block.h"

/* SYMBOL TABLE ITEM **********************************************************/

SymbolTable::Item::Item(SymbolTable::Item::Type type, string name)
	: type(type), name(name), address(0)
{
}

SymbolTable::Item::Item(SymbolTable::Item::Type type, string name, int address)
	: type(type), name(name), address(address)
{
}

void SymbolTable::dump(ostream& output) const
{
	output << "SYMBOL TABLE:" << endl;
	
	output << "   PARAMETERS:"
	foreach (param, _params) {
		output << "      " << (*param)->name << " " << (*param)->address << endl;
	}
	
	output << "   LOCALS:"
	foreach (local, _locals) {
		output << "      " << (*local)->name << " " << (*local)->address << endl;
	}
	
	output << "   FREE VARIABLES:"
	foreach (freeVar, _freeVars) {
		output << "      " << (*freeVar)->name << " " << (*freeVar)->address << " " << (*freeVar)->closureAddress << endl;
	}
	
	output << "END OF SYMBOL TABLE" << endl;
}

/* SYMBOL TABLE ***************************************************************/

bool SymbolTable::addParam(string name)
{
	if (_all.count(name) > 0)
		return false;

	Ref<Item> param = new Item(Item::PARAM, name, _params.size());
	_params.push_back(param);
	_all[name] = param;
	
	return true;
}

bool SymbolTable::addLocal(string name)
{
	if (_all.count(name) > 0)
		return false;

	Ref<Item> local = new Item(Item::LOCAL, name, _locals.size());
	_locals.push_back(local);
	_all[name] = local;

	return true;
}

void SymbolTable::addParams(vector<string> names)
{
	foreach(name, names)
		addParam(*name);
}

void SymbolTable::addLocals(vector<string> names)
{
	foreach(name, names)
		addLocal(*name);
}

Ref<SymbolTable::Item> SymbolTable::getSymbol(string name)
{
	if (_all.count(name) < 1) {
		Ref<Item> freeVar = new Item(Item::FREE_VAR, name, _freeVars.size());
		_freeVars.push_back(freeVar);
		_all[name] = freeVar;
		return freeVar;
	}

	return _all[name];
}

/* BLOCK **********************************************************************/

Block::Block(int index)
	: _parent(), _index(index), _address(0), _instructions(), _symbols()
{
}

Block::Block(int index, Ref<Block> parent)
	: _parent(parent), _index(index), _address(0), _instructions(), _symbols()
{
}

/* BLOCK TABLE ****************************************************************/

BlockTable::BlockTable()
	: _current(0), _blocks()
{
	rewind();
}

Ref<Block> BlockTable::getNewBlock()
{
	int        index = getSize();
	Ref<Block> block = new Block(index);

	_blocks.push_back(block);

	return block;
}

Ref<Block> BlockTable::getBlock(int index)
{
	while (getSize() >= index)
		getNewBlock();
	
	return _blocks[index];
}

void BlockTable::compile()
{
	int address = 0;
	foreach(block, _blocks) {
		(*block)->setAddress(address);
		address += (*block)->getLength();
	}
}

