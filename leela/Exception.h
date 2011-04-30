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

using namespace std;

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
};

#endif /* end of include guard: EXCEPTION_H_2GHF6029SDCVSD1234 */

