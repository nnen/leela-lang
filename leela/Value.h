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

	virtual void         repr(ostream& output);

	static Ref<None>     getInstance();

	virtual Ref<Boolean> toBoolean();
	virtual Ref<Number>  toNumber();
	virtual Ref<String>  toString();
	
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
	
	virtual void print(ostream& output) = 0;
	
	virtual bool equals(Ref<Value> other)
	{
		Ref<ScalarValue<T> > otherValue = other.as<ScalarValue<T> >();
		if (otherValue.isNull()) return false;

		return (getValue() == otherValue->getValue());
	}
	
	virtual bool lessThan(Ref<Value> other)
	{
		Ref<ScalarValue<T> > otherValue = other.as<ScalarValue<T> >();
		if (otherValue.isNull()) return false;
		
		return (getValue() < otherValue->getValue());
	}

	virtual T getValue() const { return _value; }
};

/* Boolean ********************************************************************/

class Boolean : public ScalarValue<bool> {
public:
	Boolean() : ScalarValue<bool>(false) {}
	Boolean(bool value) : ScalarValue<bool>(value) {}
	virtual ~Boolean() {}
	
	virtual void print(ostream& output);

	virtual Ref<Boolean> toBoolean() { return this; }
	virtual Ref<Number>  toNumber();
	virtual Ref<String>  toString();
};

/* Number *********************************************************************/

class Number : public ScalarValue<Integer> {
public:
	Number() : ScalarValue<Integer>(0) {}
	Number(Integer value) : ScalarValue<Integer>(value) {}
	virtual ~Number() {}
	
	virtual void print(ostream& output);
	virtual void repr(ostream& output);
	
	virtual Ref<Boolean> toBoolean();
	virtual Ref<Number>  toNumber() { return this; }
	virtual Ref<String>  toString();

	virtual int          getHash() const { return (int) getValue(); }

	static Ref<Number> parse(string str);
};

/* String *********************************************************************/

class String : public ScalarValue<string> {
public:
	String() : ScalarValue<string>() {}
	String(string value) : ScalarValue<string>(value) {}
	virtual ~String() {}
	
	virtual void        print(ostream& output);
	virtual void        repr(ostream& output);
	
	virtual Ref<Boolean> toBoolean();
	virtual Ref<Number>  toNumber();
	virtual Ref<String>  toString() { return this; }
	
	virtual int          getHash() const { return (int) getValue().size(); }
	
	virtual Ref<Value>   add(Ref<Value> other);
	
	static int    hexToInt(char c);
	static char   hexToChar(char digit1, char digit2);
	static char   intToHex(int i);
	static string charToHex(char c);
};

/* Table **********************************************************************/

class Table : public Value {
public:
	typedef vector<Ref<Value> >      Array;
	typedef multimap<int, ValuePair> Map;
	
private:
	Array _array;
	Map   _table;
	
	int getIntIndex(Ref<Value> key);
	Map::iterator findInMap(Ref<Value> key);
	bool removeFromMap(Ref<Value> key, Ref<Value>& value);

	void fillArray();
	
public:
	Table() {}
	virtual ~Table() {}
	
	virtual Ref<String> toString();
	
	bool       hasKey(Ref<Value> key);
	
	void       set(Ref<Value> key, Ref<Value> value);
	Ref<Value> get(Ref<Value> key, Ref<Value> deflt);
	Ref<Value> remove(Ref<Value> key);
	
	int        getSize() { return _array.size() + _table.size(); }
};

/* Variable *******************************************************************/

class Variable : public Value {
private:
	Ref<Value> _value;

public:
	Variable() : Value(), _value(None::getInstance()) {}
	Variable(Ref<Value> value) : Value(), _value(value) {}
	virtual ~Variable() {}

	virtual void         repr(ostream& output);
	
	Ref<Value>           getValue() const { return _value; }
	void                 setValue(Ref<Value> value) { _value = value; }

	virtual void         assign(Ref<Value> value);
};


#endif /* end of include guard: VALUE_H_235HDVSVGD34 */

