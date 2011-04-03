/**
 * \file   leela/Value.cpp
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-03-27
 *
 * \brief  Implementation of the Value class.
 */

#include "Value.h"

void Value::print(ostream& output) const
{
	Ref<String> str = this->toString();
	if (str.isNull()) return;
	output << *str;
}

Ref<Boolean> Value::toBoolean() const
{
	return Ref<Boolean>(new Boolean(true));
}

Ref<Number> Value::toNumber() const
{
	return Ref<Number>();
}

Ref<String> Value::toString() const
{
	return Ref<String>();
}

Ref<Value> Value::add(Ref<Value> other)
{
	Ref<Number> a = this->toNumber();
	Ref<Number> b = other->toNumber();
	
	if (a.isNull() || b.isNull()) {
		// TODO: Do some error handling here.
	}
	
	return new Number(a->getValue() + b->getValue());
}

Ref<Value> Value::subtract(Ref<Value> other)
{
	Ref<Number> a = this->toNumber();
	Ref<Number> b = other->toNumber();
	
	if (a.isNull() || b.isNull()) {
		// TODO: Do some error handling here.
	}
	
	return new Number(a->getValue() - b->getValue());
}

Ref<Value> Value::multiply(Ref<Value> other)
{
	Ref<Number> a = this->toNumber();
	Ref<Number> b = other->toNumber();
	
	if (a.isNull() || b.isNull()) {
		// TODO: Do some error handling here.
	}
	
	return new Number(a->getValue() * b->getValue());
}

Ref<Value> Value::divide(Ref<Value> other)
{	
	Ref<Number> a = this->toNumber();
	Ref<Number> b = other->toNumber();
	
	if (a.isNull() || b.isNull()) {
		// TODO: Do some error handling here.
	}
	
	return new Number(a->getValue() / b->getValue());
}

void Boolean::print(ostream& output) const
{
	if (getValue())
		output << "true";
	else
		output << "false";
}

void Number::print(ostream& output) const
{
	output << _value;
}

Ref<Number> Number::parse(string str)
{
	int val = 0;
	
	for (int i = 0; i < (int)str.length(); i++) {
		val *= 10;
		val += (int) ('9' - str[i]);
	}
	
	return Ref<Number>(new Number(val));
}

void String::print(ostream& output) const
{
	output << _value;
}

std::ostream& operator<< (std::ostream& output, const Value& value)
{
	value.print(output);
	return output;
}

