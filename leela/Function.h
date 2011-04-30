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
	int                    _paramCount;
	vector<Ref<Variable> > _closure;

public:
	Function(Address code, int paramCount);
	virtual ~Function() {}
	
	virtual Ref<String> toString() const;
	
	Address getCode() const { return _code; }
	
	int getParamCount() const { return _paramCount; }
	
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
	
	virtual Ref<String>  toString() const;
	
	Ref<Variable>        getVar(int index);
	vector<Ref<Value> >& getStack() { return _stack; }
	
	void pushClosure(Ref<Variable> variable);
	void pushVariable(Ref<Value> value);
	void pushVariable();
	
	void       push(Ref<Value> value);
	Ref<Value> pop();
};

#endif /* end of include guard: FUNCTION_H_098SDC123DGB098 */

