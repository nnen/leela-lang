/**
 * \file   leela/Block.h
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-03-28
 *
 * \brief  Header file for the Block class.
 */

#ifndef BLOCK_H_86BSDC136DFV
#define BLOCK_H_86BSDC136DFV

#include <string>
#include <map>
#include <vector>

#include "Object.h"
#include "Instruction.h"

using namespace std;

class SymbolTable {
public:
	class Item : public Object {
	public:
		enum Type {
			GLOBAL,
			PARAM,
			LOCAL,
			FREE_VAR
		};
		
		Type   type;
		string name;
		int    address;
		int    closureAddress;
		
		Item(Type type, string name);
		Item(Type type, string name, int address);
		
		virtual ~Item() {}
	};

private:
	map<string, Ref<Item> > _all;
	vector<Ref<Item> >      _params;
	vector<Ref<Item> >      _locals;
	vector<Ref<Item> >      _freeVars;

public:
	bool      addParam(string name);
	bool      addLocal(string name);
	void      addParams(vector<string> names);
	void      addLocals(vector<string> names);
	
	Ref<Item> getSymbol(string name, Block& block);
	
	int       getSize() const { return _all.size(); }

	void      dump(ostream& output) const;
};

class Block : public Object {
protected:
	Ref<Block>          _parent;
	int                 _index;
	int                 _address;
	vector<Instruction> _instructions;
	SymbolTable         _symbols;

public:
	Block(int index);
	Block(int index, Ref<Block> parent);
	virtual ~Block() {}
	
	Ref<Block>   getParent() const { return _parent; }
	int          getIndex() const { return _index; }
	int          getAddress() const { return _address; }
	void         setAddress(int value) { _address = value; }
	SymbolTable& getSymbols() { return _symbols; }
	int          getLength() const { return _instructions.size(); }
};

class BlockTable : public Object {
private:
	int                 _current;
	vector<Ref<Block> > _blocks;

public:
	BlockTable();
	
	void       rewind() { _current = 0; }
	
	int        getSize() const { return _blocks.size(); }

	Ref<Block> getNewBlock();
	Ref<Block> getBlock(int index);
	Ref<Block> getNextBlock() { return getBlock(_current++); }

	void       compile();
};

#endif /* end of include guard: BLOCK_H_86BSDC136DFV */

