/**
 * \file   leela/Value.cpp
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-03-27
 *
 * \brief  Implementation of the Value class.
 */

#include "Value.h"
#include "Function.h"
#include "Exception.h"

/* Value **********************************************************************/

void Value::print(ostream& output)
{
	Ref<String> str = this->toString();
	if (str.isNull()) return;
	output << *str;
}

void Value::repr(ostream& output)
{
	print(output);
}

Ref<Boolean> Value::toBoolean()
{
	return Ref<Boolean>(new Boolean(true));
}

Ref<Number> Value::toNumber()
{
	throw ConversionError(Ref<Value>(this), "Number");
}

Ref<String> Value::toString()
{
	throw ConversionError(this, "String");
}

int Value::getHash() const
{
	return (int) this;
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

bool Value::equals(Ref<Value> other)
{
	return (this == other.getPtr());
}

bool Value::lessThan(Ref<Value> other)
{
	throw InvalidOperationError(this, other, "less than");
	return false;
}

Ref<Value> Value::index(Ref<Value> key)
{
	throw InvalidOperationError(this, key, "get indexed value");
}

void Value::assign(Ref<Value> value)
{
	throw InvalidOperationError(this, value, "assign value");
}

Ref<ActivationFrame> Value::call(vector<Ref<Value> > arguments)
{
	throw InvalidOperationError(this, "call");
}

bool Value::equals(Ref<Value> first, Ref<Value> second)
{
	if (first->equals(second)) return true;
	if (second->equals(first)) return true;
	return false;
}

std::ostream& operator<< (std::ostream& output, Value& value)
{
	value.print(output);
	return output;
}

/* None ***********************************************************************/

Ref<None> None::_instance;

Ref<None> None::getInstance()
{
	if (_instance.isNull())
		_instance = new None();
	return _instance;
}

Ref<Boolean> None::toBoolean() const
{
	return new Boolean(false);
}

Ref<Number> None::toNumber() const
{
	return new Number(0);
}

Ref<String> None::toString() const
{
	return new String("");
}

int None::getHash() const
{
	return 0;
}

bool None::lessThan(Ref<Value> other)
{
	if (Value::equals(this, other))
		return false;
	return true;
}

/* Boolean ********************************************************************/

void Boolean::print(ostream& output) const
{
	if (getValue())
		output << "true";
	else
		output << "false";
}

/* Number *********************************************************************/

void Number::print(ostream& output) const
{
	output << _value;
}

Ref<Number> Number::parse(string str)
{
	int val = 0;
	
	for (int i = 0; i < (int)str.length(); i++) {
		val *= 10;
		val += (int) (str[i] - '0');
	}
	
	return Ref<Number>(new Number(val));
}

/* String *********************************************************************/

void String::print(ostream& output) const
{
	output << _value;
}

/* Table **********************************************************************/

/*
void Table::set(Ref<Value> key, Ref<Value> value)
{
	int hash = key->getHash();
	vector<ValuePair> * list = NULL;

	if (_table.count(hash) < 1) {
		list = new vector<ValuePair>();
		list->push_back(make_pair(key, value));
		_table[hash] = list;
		return;
	}
	
	list = _table[hash];
	
	for (int i = 0; i < list->size(); ) {
		if (Value::equals((*list)[i].first, key))
			list->
	}
}
*/

Ref<Value> Table::get(Ref<Value> key, Ref<Value> dflt)
{
	int hash = key->getHash();
	if (_table.count(hash) < 1) return dflt;
	
	vector<ValuePair> * list = _table[hash];
	
	foreach (pair, *list)
		if (Value::equals(pair->first, key)) return pair->second;
	
	return dflt;
}

/* Variable *******************************************************************/

void Variable::assign(Ref<Value> value)
{
	_value = value;
}

