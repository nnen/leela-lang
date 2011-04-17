/**
 * \file   Function.h
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-04-16
 *
 * \brief  
 */

#ifndef FUNCTION_H_098SDC123DGB098
#define FUNCTION_H_098SDC123DGB098

#include <vector>

#include "leela.h"
#include "Object.h"
#include "Value.h"

class ActivationFrame;

class Function : public Value {
private:
	Address                _code;
	vector<Ref<Variable> > _closure;

public:
	Function(Address code);
	virtual ~Function() {}
	
	Address getCode() const { return _code; }
	
	void pushClosure(Ref<Variable> variable);
	
	Ref<ActivationFrame> activate();
};

class ActivationFrame : public Value {
private:
	Ref<Function>          _function;
	vector<Ref<Variable> > _variables;
	vector<Ref<Value> >    _stack;

public:
	Address                instrCounter;
	
	ActivationFrame(Ref<Function> function);
	virtual ~ActivationFrame() {}
	
	Ref<Variable>        getVar(int index);
	vector<Ref<Value> >& getStack() { return _stack; }
	
	void pushClosure(Ref<Variable> variable);
	void pushVariable(Ref<Value> value);
	void pushVariable();
	
	void       push(Ref<Value> value);
	Ref<Value> pop();
};

#endif /* end of include guard: FUNCTION_H_098SDC123DGB098 */

