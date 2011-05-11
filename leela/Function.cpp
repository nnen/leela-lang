/**
 * \file   Function.cpp
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-04-16
 *
 * \brief  
 */

#include <stack>
#include <sstream>
#include <typeinfo>

#include "Function.h"
#include "Machine.h"
#include "Exception.h"

/* Function *******************************************************************/

Function::Function(Address code, int paramCount)
	: _code(code), _paramCount(paramCount)
{
}

void Function::repr(ostream& output)
{
	output << "<Function @" << _code << " (" << _paramCount << ")>";
}

Ref<String> Function::toString()
{
	stringstream s;
	s << "<Function @" << getCode() << ">";
	return new String(s.str());
}

void Function::call(Machine& machine, vector<Ref<Value> > arguments)
{
	if (_paramCount != arguments.size())
		throw RuntimeError("Invalid number function of arguments.");
	
	Ref<ActivationFrame> frame = new ActivationFrame(this);

	// Push the free variables to the
	// activation frame.
	foreach (var, _closure)
		frame->pushClosure(*var);

	// Pop arguments from the stack and push
	// them to the activation frame.
	foreach (arg, arguments)
		frame->pushVariable(*arg);
	
	machine.pushFrame(frame);
}

void Function::pushClosure(Ref<Variable> variable)
{
	_closure.push_back(variable);
}

/* BuiltinFunction ************************************************************/

void BuiltinFunction::call(Machine& machine, vector<Ref<Value> > arguments)
{
	machine.push(_function(machine, arguments));
}

Ref<String> BuiltinFunction::toString()
{
	stringstream s;
	s << "<BuiltinFunction at @" << this << ">";
	return new String(s.str());
}

Ref<Value> makeTable(Machine& machine, vector<Ref<Value> > arguments)
{
	return new Table();
}

Ref<Value> getSize(Machine& machine, vector<Ref<Value> > arguments)
{
	if (arguments.size() < 1)
		throw RuntimeError("Not enough arguments.");
	
	Ref<String> str = arguments.front().as<String>();
	if (!str.isNull())
		return new Number(str->getValue().size());
	
	Ref<Table> tbl = arguments.front().as<Table>();
	if (!tbl.isNull())
		return new Number(tbl->getSize());
	
	throw RuntimeError("Invalid argument(s).");
}

/* ActivationFrame ************************************************************/

ActivationFrame::ActivationFrame(Ref<Function> function)
	: _function(function), instrCounter(function->getCode())
{
}

Ref<String> ActivationFrame::toString()
{
	stringstream s;
	s << *_function << " (stack size: " << _stack.size() << ", alloc: " << _variables.size() << ")" << endl;
	foreach (it, _stack) {
		// s << "\t[" << typeid(**it).name() << "]: " << **it << endl;
		s << "\t";
		(*it)->repr(s);
		s << endl;
	}
	return new String(s.str());
}

Ref<Variable> ActivationFrame::getVar(int index)
{
	if (index < 0 || index >= (int) _variables.size())
		throw RuntimeError("Variable index out of range.");
	return _variables[index];
}

void ActivationFrame::pushClosure(Ref<Variable> variable)
{
	_variables.push_back(variable);
}

void ActivationFrame::pushVariable(Ref<Value> value)
{
	_variables.push_back(new Variable(value));
}

void ActivationFrame::pushVariable()
{
	_variables.push_back(new Variable());
}

void ActivationFrame::push(Ref<Value> value)
{
	_stack.push_back(value);
}

Ref<Value> ActivationFrame::peek()
{
	if (_stack.size() < 1) return Ref<Value>();
	return _stack.back();
}

Ref<Value> ActivationFrame::pop()
{
	if (_stack.size() < 1) return Ref<Value>();
	Ref<Value> value = _stack.back();
	_stack.pop_back();
	return value;
}

