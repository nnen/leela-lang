/**
 * \file   Context.cpp
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-04-20
 *
 * \brief  Implementation of the {@link Context} class.
 */

#include <iostream>

#include "Context.h"
#include "Machine.h"

Ref<Symbol> Context::getBoundVar(string name)
{
	Ref<Context> context = this;
	
	// Iterate through all contexts between this
	// one and the root context and return the first
	// non-free-var symbol wih the given name.
	while (!context.isNull()) {
		if (context->_symbols.count(name) > 0) {
			Ref<Symbol> var = context->_symbols[name];
			if (var->type != Symbol::FREE_VAR)
				return var;
		}
		context = context->_parent;
	}

	// If we're in the root context, check whether the
	// symbol is a builtin.
	UInteger builtinIndex;
	if (Machine::getBuiltinIndex(name, builtinIndex))
		return new Symbol(name, Symbol::CONSTANT, builtinIndex);
	
	return NULL;
}

Symbol::Symbol(string name, Type type, Ref<Symbol> freeVar)
	: Object(),
	  name(name),
	  type(type),
	  index(0),
	  indexInvalid(true),
	  freeVar(freeVar)
{
}

Symbol::Symbol(string name, Type type)
	: Object(),
	  name(name),
	  type(type),
	  index(0),
	  indexInvalid(true)
{
}

Symbol::Symbol(string name, Type type, int index)
	: Object(),
	  name(name),
	  type(type),
	  index(index),
	  indexInvalid(false)
{
}

Context::Context()
	: Object(),
	  _constantCount(Machine::BUILTIN_COUNT),
	  _freeVarCount(0),
	  _paramCount(0),
	  _nextFreeVar(0),
	  _nextParam(0),
	  _nextLocal(0)
{
	reset();
}

Context::Context(Ref<Context> parent)
	: Object(),
	  _parent(parent),
	  _constantCount(Machine::BUILTIN_COUNT),
	  _freeVarCount(0),
	  _paramCount(0),
	  _nextFreeVar(0),
	  _nextParam(0),
	  _nextLocal(0)
{
}

void Context::reset()
{
	_nextFreeVar = 0;
	_nextParam   = 0;
	_nextLocal   = 0;
	
	foreach(pair, _symbols)
		if (pair->second->type != Symbol::CONSTANT)
			pair->second->indexInvalid = true;

	_constants.clear();
}

bool Context::addConstant(string name)
{
	if (!_parent.isNull())
		return _parent->addConstant(name);
	
	// if (_symbols.count(name) > 0) return false;
	if (_symbols.count(name) > 0) return true;
	
	UInteger index = 0;
	if (Machine::getBuiltinIndex(name, index))
		return false;
	
	Ref<Symbol> constant = new Symbol(name, Symbol::CONSTANT, _constantCount++);
	_constants.push_back(constant);
	_symbols[name] = constant;
	
	return true;
}

bool Context::addParam(string name)
{
	Ref<Symbol> param;
	
	if (_symbols.count(name) > 0) {
		param = _symbols[name];
		if (param->type != Symbol::PARAM) return false;
	} else {
		param = new Symbol(name, Symbol::PARAM);
		_symbols[name] = param;
		_params.push_back(param);
	}
	
	param->index = _freeVarCount + _nextParam++;
	if (_nextParam > _paramCount) _paramCount = _nextParam;
	
	return true;
}

bool Context::addLocal(string name)
{
	Ref<Symbol> local;

	if (_symbols.count(name) > 0) {
		local = _symbols[name];
		if (local->type != Symbol::LOCAL) return false;
	} else {
		local = new Symbol(name, Symbol::LOCAL);
		_symbols[name] = local;
	}
	
	local->index = _freeVarCount + _paramCount + _nextLocal++;
	
	return true;
}

Ref<Symbol> Context::addFreeVar(string name)
{
	Ref<Symbol> freeVar;
	
	if (_symbols.count(name) > 0) {
		freeVar = _symbols[name];
		if (freeVar->type != Symbol::FREE_VAR) return NULL;
	} else {
		Ref<Symbol> var = getBoundVar(name);
		if (var.isNull()) return NULL;
		if (var->type == Symbol::CONSTANT) return var;
		
		freeVar          = new Symbol(name, Symbol::FREE_VAR, var);
		_symbols[name]   = freeVar;
		_freeVars.push_back(freeVar);
	}
	
	if (!freeVar.isNull() && freeVar->indexInvalid) {
		freeVar->index        = _nextFreeVar++;
		freeVar->indexInvalid = false;
		if (_nextFreeVar > _freeVarCount) _freeVarCount = _nextFreeVar;
	}
	
	return freeVar;
}

Ref<Symbol> Context::getSymbol(string name)
{
	Ref<Symbol> symbol;
	
	if (_symbols.count(name) > 0)
		symbol = _symbols[name];
	else
		symbol = addFreeVar(name);

	if (!symbol.isNull() && (symbol->type == Symbol::FREE_VAR) && symbol->indexInvalid) {
		symbol->index        = _nextFreeVar++;
		symbol->indexInvalid = false;
		if (_nextFreeVar > _freeVarCount) _freeVarCount = _nextFreeVar;
	} 
	
	return symbol;
}

void ContextTable::reset()
{
	_next = 0;
	while (!_openContexts.empty())
		_openContexts.pop();
}

Ref<Context> ContextTable::current()
{
	if (_openContexts.size() < 1) next();
	return _openContexts.top();
}

void ContextTable::next()
{
	if (_next < (int) _contexts.size()) {
		_openContexts.push(_contexts[_next++]);
		_openContexts.top()->reset();
		return;
	}
	
	Ref<Context> context;
	if (_openContexts.size() > 0)
		context = new Context(_openContexts.top());
	else
		context = new Context();
	
	_contexts.push_back(context);
	_openContexts.push(context);
	_next++;
}

void ContextTable::close()
{
	if (_openContexts.size() > 0)
		_openContexts.pop();
}

