/**
 * \file   Exception.h
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-04-30
 *
 * \brief  
 */

#ifndef EXCEPTION_H_2GHF6029SDCVSD1234
#define EXCEPTION_H_2GHF6029SDCVSD1234

#include <exception>
#include <string>
#include <ostream>

#include "Object.h"
#include "Value.h"

using namespace std;

/* Exception ******************************************************************/

class Exception : public exception {
private:
	string _message;

protected:
	void setMessage(string message) { _message = message; }

public:
	Exception() throw();
	Exception(string message) throw();
	virtual ~Exception() throw() {}
		
	virtual const char* what() const throw() { return _message.c_str(); }
	
	virtual string getMessage() { return _message; }

	virtual void print(ostream& output) { output << getMessage(); }
};

/* RuntimeError ***************************************************************/

class RuntimeError : public Exception {
public:
	RuntimeError() throw();
	RuntimeError(string message) throw();
	virtual ~RuntimeError() throw() {}
};

/* RuntimeError ***************************************************************/

class ConversionError : public RuntimeError {
private:
	Ref<Value> _value;
	string     _targetType;

public:
	ConversionError(Ref<Value> value, string targetType) throw();
	virtual ~ConversionError() throw() {}
};

/* InvalidOperationError ******************************************************/

class InvalidOperationError : public RuntimeError {
private:
	Ref<Value> _first;
	Ref<Value> _second;
	string     _operation;

public:
	InvalidOperationError(Ref<Value> value, string operation) throw();
	InvalidOperationError(Ref<Value> first, Ref<Value> second, string operation) throw();
	virtual ~InvalidOperationError() throw() {}
};

#endif /* end of include guard: EXCEPTION_H_2GHF6029SDCVSD1234 */

