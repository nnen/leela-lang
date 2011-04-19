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

#include "leela.h"
#include "Object.h"

using namespace std;

class Interpreter;

class Boolean;
class Number;
class String;

class Value : public Object {
public:
	Value() : Object() {}
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

template<class T>
class ScalarValue : public Value {
protected:
	T _value;

public:
	ScalarValue() : _value() {}
	ScalarValue(T value) : _value(value) {}
	virtual ~ScalarValue() {}

	virtual void print(ostream& output) const = 0;
	
	virtual T getValue() const { return _value; }
};

class Boolean : public ScalarValue<bool> {
public:
	Boolean() : ScalarValue<bool>(false) {}
	Boolean(bool value) : ScalarValue<bool>(value) {}
	virtual ~Boolean() {}

	virtual void print(ostream& output) const;
};

class Number : public ScalarValue<Integer> {
public:
	Number() : ScalarValue<Integer>(0) {}
	Number(Integer value) : ScalarValue<Integer>(value) {}
	virtual ~Number() {}

	static Ref<Number> parse(string str);

	virtual void print(ostream& output) const;
};

class String : public ScalarValue<string> {
public:
	String() : ScalarValue<string>() {}
	String(string value) : ScalarValue<string>(value) {}
	virtual ~String() {}

	virtual void print(ostream& output) const;
};

/*
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
*/

class Variable : public Value {
private:
	Ref<Value> _value;

public:
	Variable() : Value(), _value() {}
	Variable(Ref<Value> value) : Value(), _value(value) {}
	virtual ~Variable() {}
	
	Ref<Value> getValue() const { return _value; }
	void       setValue(Ref<Value> value) { _value = value; }
};

std::ostream& operator<< (std::ostream& output, const Value& value);

#endif /* end of include guard: VALUE_H_235HDVSVGD34 */

