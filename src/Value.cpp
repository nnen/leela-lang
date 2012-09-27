/**
 * \file   leela/Value.cpp
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-03-27
 *
 * \brief  Implementation of the Value class.
 */

#include <cctype>

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
	output << "<Value object at " << this << ">";
	// print(output);
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

Hash Value::getHash() const
{
	return (Hash) this;
}

Ref<Value> Value::add(Ref<Value> other)
{
	Ref<Number> a = this->toNumber();
	Ref<Number> b = other->toNumber();
	
	if (a.isNull() || b.isNull())
		throw new InvalidOperationError(this, other, "adding");
	
	return new Number(a->getValue() + b->getValue());
}

Ref<Value> Value::subtract(Ref<Value> other)
{
	Ref<Number> a = this->toNumber();
	Ref<Number> b = other->toNumber();
	
	if (a.isNull() || b.isNull())
		throw new InvalidOperationError(this, other, "subtracting");
	
	return new Number(a->getValue() - b->getValue());
}

Ref<Value> Value::multiply(Ref<Value> other)
{
	Ref<Number> a = this->toNumber();
	Ref<Number> b = other->toNumber();
	
	if (a.isNull() || b.isNull())
		throw new InvalidOperationError(this, other, "multiplying");
	
	return new Number(a->getValue() * b->getValue());
}

Ref<Value> Value::divide(Ref<Value> other)
{	
	Ref<Number> a = this->toNumber();
	Ref<Number> b = other->toNumber();
	
	if (a.isNull() || b.isNull())
		throw new InvalidOperationError(this, other, "dividing");
	
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

void None::repr(ostream& output)
{
	output << "<None>";
}

Ref<None> None::getInstance()
{
	if (_instance.isNull())
		_instance = new None();
	return _instance;
}

Ref<Boolean> None::toBoolean()
{
	return new Boolean(false);
}

Ref<Number> None::toNumber()
{
	return new Number(0);
}

Ref<String> None::toString()
{
	return new String("None");
}

Hash None::getHash() const
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

void Boolean::print(ostream& output)
{
	if (getValue())
		output << "true";
	else
		output << "false";
}

void Boolean::repr(ostream& output)
{
	print(output);
}

Ref<Number> Boolean::toNumber()
{
	if (getValue())
		return new Number(1);
	return new Number(0);
}

Ref<String> Boolean::toString()
{
	if (getValue())
		return new String("true");
	else
		return new String("false");
}

/* Number *********************************************************************/

void Number::print(ostream& output)
{
	output << _value;
}

void Number::repr(ostream& output)
{
	output << _value;
}

Ref<Boolean> Number::toBoolean()
{
	return new Boolean(getValue() != 0);
}

Ref<String> Number::toString()
{
	stringstream s;
	s << getValue();
	return new String(s.str());
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

void String::print(ostream& output)
{
	output << _value;
}

void String::repr(ostream& output)
{
	output << '"';
	for (unsigned int i = 0; i < _value.size(); i++) {
		if (isprint(_value[i]) && !isspace(_value[i]) && (_value[i] != '"'))
			output << _value[i];
		else
			output << "\\x" << charToHex(_value[i]);
	}
	output << '"';
}

Ref<Boolean> String::toBoolean()
{
	return new Boolean(getValue().size() > 0);
}

Ref<Number> String::toNumber()
{
	string str = getValue();
	Integer value = 0;
	unsigned int i = 0;
	bool negative = false;

	while ((i < str.size()) && isspace(str[i]))
		i++;
	
	if ((i < str.size()) && (str[i] == '-')) {
		negative = true;
		i++;
	}
	
	while ((i < str.size()) && isdigit(str[i])) {
		value *= 10;
		value += str[i++] - '0';
	}
	
	if (negative) value *= -1;
	
	return new Number(value);
}

Ref<Value> String::add(Ref<Value> other)
{
	stringstream s;
	try {
		s << getValue() << other->toString()->getValue();
	} catch (Exception e) {
		throw InvalidOperationError(this, other, "adding");
	}
	return new String(s.str());
}

int String::hexToInt(char c)
{
	if (c >= '0' && c <= '9')
		return (c - '0');
	else if (c >= 'a' && c <= 'f')
		return (0x0a + (c - 'a'));
	else if (c >= 'A' && c <= 'F')
		return (0x0a + (c - 'A'));
	return 0;
}

char String::hexToChar(char digit1, char digit2)
{
	unsigned char result = 0;
	result += 0x10 * hexToInt(digit1);
	result +=        hexToInt(digit2);
	return (char)result;
}

char String::intToHex(int i)
{
	if (i >= 0 && i <= 9)
		return ('0' + i);
	else if (i >= 10 && i <= 15)
		return ('a' + (i - 10));
	return '0';
}

string String::charToHex(char c)
{
	string s = "00";
	unsigned char uc = (unsigned char) c;
	s[0] = intToHex(uc / 0x10);
	s[1] = intToHex(uc % 0x10);
	return s;
}

/* Table **********************************************************************/

int Table::getIntIndex(Ref<Value> key)
{
	Ref<Number> number = key.as<Number>();
	if (number.isNull()) return -1;
	return number->getValue();
}

Table::Map::iterator Table::findInMap(Ref<Value> key)
{
	Hash hash = key->getHash();
	pair<Map::iterator, Map::iterator> range = _table.equal_range(hash);
	
	for (Map::iterator i = range.first; i != range.second; i++)
		if (i->second.first->equals(key)) return i;
	
	return _table.end();
}

bool Table::removeFromMap(Ref<Value> key, Ref<Value>& value)
{
	Map::iterator item = findInMap(key);
	if (item == _table.end()) return false;
	
	value = item->second.second;
	_table.erase(item);
	
	return true;
}

void Table::fillArray()
{
	Ref<Value> value;
	while (removeFromMap(new Number(_array.size()), value))
		_array.push_back(value);
}

Ref<String> Table::toString()
{
	stringstream s;
	bool first = true;
	
	s << "[";
	foreach (item, _array) {
		if (first)
			first = false;
		else
			s << ", ";
		(*item)->repr(s);
	}
	foreach (pair, _table) {
		if (first)
			first = false;
		else
			s << ", ";
		pair->second.first->repr(s);
		s << ": ";
		pair->second.second->repr(s);
	}
	s << "]";
	return new String(s.str());
}

bool Table::hasKey(Ref<Value> key)
{
	int index = getIntIndex(key);
	if ((index >= 0) && ((unsigned int)index < _array.size())) return true;
	
	Hash hash = key->getHash();
	
	pair<Map::iterator, Map::iterator> range = _table.equal_range(hash);
	
	for (Map::iterator i = range.first; i != range.second; i++)
		if (i->second.first->equals(key)) return true;
	return false;
}

void Table::set(Ref<Value> key, Ref<Value> value)
{
	if (key.isNull()) {
		_array.push_back(value);
		fillArray();
		return;
	}
	
	int index = getIntIndex(key);
	if ((index >= 0) && ((unsigned int)index < _array.size())) {
		_array[index] = value;
		return;
	}
	
	if (index == (int)_array.size()) {
		_array.push_back(value);
		fillArray();
		return;
	}
	
	Hash hash = key->getHash();
	
	Map::iterator item = findInMap(key);
	
	if (item == _table.end()) {
		_table.insert(make_pair(hash, make_pair(key, value)));
	} else {
		item->second.second = value;
	}
}

Ref<Value> Table::get(Ref<Value> key, Ref<Value> dflt)
{
	int index = getIntIndex(key);
	if ((index >= 0) && ((unsigned int)index < _array.size()))
		return _array[index];
	
	Hash hash = key->getHash();
	if (_table.count(hash) < 1) return dflt;
	
	pair<Map::iterator, Map::iterator> range = _table.equal_range(hash);
	
	for (Map::iterator i = range.first; i != range.second; i++)
		if (i->second.first->equals(key)) return i->second.second;
	
	return dflt;
}

Ref<Value> Table::remove(Ref<Value> key)
{
	int index = getIntIndex(key);
	if ((index >= 0) && ((unsigned int)index < _array.size())) {
		Ref<Value> value = _array[index];
		
		for (unsigned int i = index + 1; i < _array.size(); i++) {
			Ref<Number> number = new Number(i);
			_table.insert(make_pair(number->getHash(),
				make_pair(number, _array[i])));
		}
		
		_array.resize(index);
		
		return value;
	}
	
	Map::iterator item = findInMap(key);
	if (item == _table.end()) return NULL;

	Ref<Value> value = item->second.second;
	_table.erase(item);
	return value;
}

/* Variable *******************************************************************/

void Variable::repr(ostream& output)
{
	output << "<Variable object at " << this << ">";
}

void Variable::assign(Ref<Value> value)
{
	_value = value;
}

