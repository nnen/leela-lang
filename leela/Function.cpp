/**
 * \file   Function.cpp
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-04-16
 *
 * \brief  
 */

#include "Function.h"

Function::Function(Address code)
	: _code(code)
{
}

void Function::pushClosure(Ref<Variable> variable)
{
	_closure.push_back(variable);
}

Ref<ActivationFrame> Function::activate()
{
	Ref<ActivationFrame> result = new ActivationFrame(this);
	foreach (var, _closure)
		result->pushVariable(*var);
	return result;
}

ActivationFrame::ActivationFrame(Ref<Function> function)
	: _function(function), instrCounter(function->getCode())
{
}

Ref<Variable> ActivationFrame::getVar(int index)
{
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

Ref<Value> ActivationFrame::pop()
{
	if (_stack.size() < 1) return Ref<Value>();
	Ref<Value> value = _stack.back();
	_stack.pop_back();
	return value;
}

