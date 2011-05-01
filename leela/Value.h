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
#include <vector>
#include <map>
#include <utility>

#include "leela.h"
#include "Object.h"

using namespace std;

class Boolean;
class Number;
class String;
class ActivationFrame;

/* Value **********************************************************************/

class Value : public Object {
public:
	Value() : Object() {}
	virtual ~Value() {}
	
	virtual void print(ostream& output);
	virtual void repr(ostream& output);
	
	virtual Ref<Boolean> toBoolean();
	virtual Ref<Number>  toNumber();
	virtual Ref<String>  toString();
	
	virtual int          getHash() const;
	
	virtual Ref<Value>   add(Ref<Value> other);
	virtual Ref<Value>   subtract(Ref<Value> other);
	virtual Ref<Value>   multiply(Ref<Value> other);
	virtual Ref<Value>   divide(Ref<Value> other);
	
	virtual bool         equals(Ref<Value> other);
	virtual bool         lessThan(Ref<Value> other);
	
	virtual Ref<Value>   index(Ref<Value> key);
	virtual void         assign(Ref<Value> value);
	virtual Ref<ActivationFrame> call(vector<Ref<Value> > arguments);
	
	static  bool         equals(Ref<Value> first, Ref<Value> second);
};

typedef pair<Ref<Value>, Ref<Value> > ValuePair;

std::ostream& operator<< (std::ostream& output, Value& value);

/* None ***********************************************************************/

class None : public Value {
private:
	static Ref<None> _instance;

	None() : Value() {}

public:
	virtual ~None() {}

	static Ref<None>     getInstance();

	virtual Ref<Boolean> toBoolean() const;
	virtual Ref<Number>  toNumber() const;
	virtual Ref<String>  toString() const;
	
	virtual int          getHash() const;
	
	virtual bool         lessThan(Ref<Value> other);
};

/* ScalarValue ****************************************************************/

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

/* Boolean ********************************************************************/

class Boolean : public ScalarValue<bool> {
public:
	Boolean() : ScalarValue<bool>(false) {}
	Boolean(bool value) : ScalarValue<bool>(value) {}
	virtual ~Boolean() {}
	
	virtual void print(ostream& output) const;
};

/* Number *********************************************************************/

class Number : public ScalarValue<Integer> {
public:
	Number() : ScalarValue<Integer>(0) {}
	Number(Integer value) : ScalarValue<Integer>(value) {}
	virtual ~Number() {}

	static Ref<Number> parse(string str);

	virtual void print(ostream& output) const;
};

/* String *********************************************************************/

class String : public ScalarValue<string> {
public:
	String() : ScalarValue<string>() {}
	String(string value) : ScalarValue<string>(value) {}
	virtual ~String() {}

	virtual void print(ostream& output) const;
};

/* Table **********************************************************************/

class Table : public Value {
private:
	map<int, vector<ValuePair>* > _table;

public:
	Table() {}
	virtual ~Table() {}
	
	void       set(Ref<Value> key, Ref<Value> value);
	Ref<Value> get(Ref<Value> key, Ref<Value> deflt);
};

/* Variable *******************************************************************/

class Variable : public Value {
private:
	Ref<Value> _value;

public:
	Variable() : Value(), _value() {}
	Variable(Ref<Value> value) : Value(), _value(value) {}
	virtual ~Variable() {}
	
	Ref<Value>           getValue() const { return _value; }
	void                 setValue(Ref<Value> value) { _value = value; }

	virtual void         assign(Ref<Value> value);
};


#endif /* end of include guard: VALUE_H_235HDVSVGD34 */

