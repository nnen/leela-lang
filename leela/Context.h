/**
 * \file   Context.h
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-04-19
 *
 * \brief  Header file for the {@link Context} class.
 */

#ifndef CONTEXT_H_ZXCZC098098123BC
#define CONTEXT_H_ZXCZC098098123BC

#include <string>
#include <map>
#include <vector>

#include "leela.h"
#include "Object.h"

using namespace std;

class Symbol : public Object {
public:
	enum Type {
		FREE_VAR,
		PARAM,
		LOCAL
	};
	
	string      name;
	Type        type;
	UInteger    index;
	bool        indexInvalid;
	
	Ref<Symbol> freeVar;
	
	Symbol(string name, Type type);
	virtual ~Symbol();
};

class Context : public Object {
private:
	Ref<Context>              _parent;
	
	map<string, Ref<Symbol> > _symbols;
	vector<Ref<Symbol> >      _params;
	vector<Ref<Symbol> >      _freeVars;
	
	int                       _freeVarCount;
	int                       _paramCount;
	
	int                       _nextFreeVar;
	int                       _nextParam;
	int                       _nextLocal;

public:
	Context();
	Context(Ref<Context> parent);
	virtual ~Context();
	
	/**
	 * Resets the context for another parser pass.
	 */
	void reset();
	
	bool addParam(string name);
	bool addLocal(string name);
	Ref<Symbol> addFreeVar(string name);
	Ref<Symbol> getSymbol(string name);
	
	Ref<Symbol> getSymbol();
};

class ContextTable : public Object {
private:
	vector<Ref<Context> > _contexts;
	int                   _next;
	
	stack<Ref<Context> >  _openContexts;

public:
	ContextTable();
	virtual ~ContextTable();
	
	/**
	 * Resets the context table for another parser pass.
	 */
	void reset();
	
	Ref<Context> current();
	void         next();
	void         close();
};

#endif /* end of include guard: CONTEXT_H_ZXCZC098098123BC */

