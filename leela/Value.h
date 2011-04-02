/**
 * \file   leela/Value.h
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-03-27
 *
 * \brief  Header file for the Value class.
 */

#ifndef VALUE_H_235HDVSVGD34
#define VALUE_H_235HDVSVGD34

#include <string>
#include <ostream>

#include "Object.h"

using namespace std;

class Interpreter;

class Boolean;
class Number;
class String;

class Value : public Object {
public:
	Value();
	virtual ~Value() {}
	
	virtual void print(ostream& output) const;
	
	virtual Ref<Boolean> toBoolean() const;
	virtual Ref<Number>  toNumber() const;
	virtual Ref<String>  toString() const;
	
	virtual Ref<Value> add(Ref<Value> other);
	virtual Ref<Value> subtract(Ref<Value> other);
	virtual Ref<Value> multiply(Ref<Value> other);
	virtual Ref<Value> divide(Ref<Value> other);
	
	// virtual void call(Interpreter& interpreter);
};

class Boolean : public Value {
private:
	bool _value;

public:
	Boolean() : _value(false) { }
	Boolean(bool value) : _value(value) { }
};

class Number : public Value {
private:
	int _value;

public:
	Number();
	Number(int value);
	
	virtual void print(ostream& output) const;
	
	int getValue() const { return _value; }
	
	static Ref<Number> parse(string str);
};

class String : public Value {
private:
	string _value;

public:
	String();
	String(string value);
	
	virtual void print(ostream& output) const;
};

class Variable : public Value {
private:
	Ref<Value> _value;

public:
	Variable();
	Variable(Ref<Value> value);
};

std::ostream& operator<< (std::ostream& output, const Value& value);

#endif /* end of include guard: VALUE_H_235HDVSVGD34 */

