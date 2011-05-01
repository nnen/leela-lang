/**
 * \file   Exception.cpp
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-04-30
 *
 * \brief  
 */

#include <sstream>

#include "Exception.h"

/* Exception ******************************************************************/

Exception::Exception() throw()
	: exception()
{
}

Exception::Exception(string message) throw()
	: exception(), _message(message)
{
}

/* RuntimeError ***************************************************************/

RuntimeError::RuntimeError() throw()
	: Exception()
{
}

RuntimeError::RuntimeError(string message) throw()
	: Exception(message)
{
}

/* ConversionError ************************************************************/

ConversionError::ConversionError(Ref<Value> value, string targetType) throw()
	: RuntimeError(), _value(value), _targetType(targetType)
{
	stringstream s;
	s << "ConversionError: Value ";
	value->repr(s);
	s << " cannot be converted to type " << targetType << ".";
	setMessage(s.str());
}

/* ConversionError ************************************************************/

InvalidOperationError::InvalidOperationError(Ref<Value> value, string operation) throw()
	: RuntimeError(), _first(value), _operation(operation)
{
	stringstream s;
	s << "InvalidOperationError: Cannot perform operation \""
	  << operation << "\" on value ";
	value->repr(s);
	s << ".";
	setMessage(s.str());
}

InvalidOperationError::InvalidOperationError(Ref<Value> first,
                                             Ref<Value> second,
											 string     operation) throw()
	: RuntimeError(), _first(first), _second(second), _operation(operation)
{
	stringstream s;
	s << "InvalidOperationException: Cannot perform operation \""
	  << operation << "\" on values ";
	_first->repr(s);
	s << " and ";
	_second->repr(s);
	s << ".";
	setMessage(s.str());
}

