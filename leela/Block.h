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
#include <vector>

#include "Instruction.h"

using namespace std;

struct Symbol {
public:
	enum Type {
		PARAM,
		LOCAL,
		FREE_VAR
	};
	
	Type   type;
	string name;
	int    address;
	
	Symbol(Type type, string name)
	{
		this->type = type;
		this->name = name;
		this->address = NULL;
	}
};

class SymbolTable {
private:
	set<string> _all;
	vector<string>   _vars;
	vector<string>   _freeVars;
	map<string, int> _map;
	// Maps symbols to their addresses in the outer context.
	map<string, int> _freeVariables;

public:
	void addParam(string name);
	void addLocal(string name);
	
	void compile();
	
	int getAddress(string symbol) const;
	string getSymbol(int address) const;
	
	int getSize() const;
};

class Block : public Object {
protected:
	Ref<Block>             _contex;
	
	int                    _address;
	vector<Intruction>     _instructions;
	SymbolTable            _symbols;

public:
	Block();
	virtual ~Block();
};

#endif /* end of include guard: BLOCK_H_86BSDC136DFV */

