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
class Machine;

class Callable : public Value {
public:
	Callable() : Value() {}
	virtual ~Callable() {}
	
	virtual void call(Machine& machine, vector<Ref<Value> > arguments) = 0;
};

class Function : public Callable {
private:
	Address                _code;
	Address                _staticLink;
	int                    _paramCount;
	vector<Ref<Variable> > _closure;

public:
	Function(Address code, Address staticLink, int paramCount);
	virtual ~Function() {}
	
	virtual void        repr(ostream& output);
	
	virtual Ref<String> toString();
	virtual void        call(Machine& machine, vector<Ref<Value> > arguments);
	
	Address getCode() const { return _code; }
	Address getStaticLink() const { return _staticLink; }
	
	int getParamCount() const { return _paramCount; }
	
	void pushClosure(Ref<Variable> variable);
};

class BuiltinFunction : public Callable {
public:
	typedef Ref<Value> (*Function)(Machine& machine, vector<Ref<Value> > arguments);

private:
	Function _function;
	int      _argCount;
	
public:
	BuiltinFunction(Function function, int argCount) : Callable(), _function(function), _argCount(argCount) {}
	virtual ~BuiltinFunction() {}
	
	virtual void call(Machine& machine, vector<Ref<Value> > arguments);

	virtual Ref<String> toString();
};

Ref<Value> makeTable(Machine& machine, vector<Ref<Value> > arguments);
Ref<Value> getSize(Machine& machine, vector<Ref<Value> > arguments);
Ref<Value> removeItem(Machine& machine, vector<Ref<Value> > arguments);

class ActivationFrame : public Value {
private:
	Ref<Function>          _function;
	vector<Ref<Variable> > _variables;
	vector<Ref<Value> >    _stack;

public:
	Address                instrCounter;
	
	ActivationFrame(Ref<Function> function);
	virtual ~ActivationFrame() {}
	
	virtual Ref<String>  toString();
	
	Ref<Function>        getFunction() { return _function; }
	Ref<Variable>        getVar(int index);
	vector<Ref<Value> >& getStack() { return _stack; }
	
	void pushClosure(Ref<Variable> variable);
	void pushVariable(Ref<Value> value);
	void pushVariable();
	
	int        getStackSize() { return _stack.size(); }
	void       push(Ref<Value> value);
	Ref<Value> peek();
	Ref<Value> pop();
};

#endif /* end of include guard: FUNCTION_H_098SDC123DGB098 */

