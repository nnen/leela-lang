/**
 * \file   Exception.cpp
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-04-30
 *
 * \brief  
 */

#include "Exception.h"

Exception::Exception() throw()
	: exception()
{
}

Exception::Exception(string message) throw()
	: exception(), _message(message)
{
}

