/**
 * \file   AsmScanner.cpp
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-04-16
 *
 * \brief  
 */

#include "AsmScanner.h"

map<string, AsmScanner::Tokens> AsmScanner::_mnemonics;

void AsmScanner::initMnemonics()
{
	if (_mnemonics.size() > 0) return;
	
	#define M(name, nothing, integer, reference, address, register) _mnemonics[#name] = TOKEN_##name;
	#include "mnemonics.h"
}

AsmScanner::Tokens AsmScanner::getMnemonic(string str)
{
	initMnemonics();
	if (_mnemonics.count(str) < 1)
		return TOKEN_UNKNOWN;
	return _mnemonics[str];
}

int AsmScanner::peek()
{
	if (_current >= 0) return _current;
	if (_input)
		_current = _input->get();
	else
		_current = -1;
	return _current;
}

int AsmScanner::get()
{
	int c = peek();
	_current = -1;
	return c;
}

void AsmScanner::skipWhitespace()
{
	while (isspace(peek())) get();
	if (peek() == ';')
		while ((peek() != '\n') && (peek() >= 0)) get();
	while (isspace(peek())) get();
}

AsmScanner::Tokens AsmScanner::readToken()
{
	if (_currentToken != TOKEN_NONE) return _currentToken;
	
	skipWhitespace();
	
	if (peek() < 0) return TOKEN_END;
	
	// Read reference or label
	if (islower(peek())) {
		string value;
		value += get();
		
		while (!isspace(peek())) {
			if (peek() == ':') {
				get();
				label = value;
				return TOKEN_LABEL;
			}
			
			value += get();
		}
		
		reference = value;
		return TOKEN_REFERENCE;
	}
	
	// Read register number
	if (peek() == '#') {
		get();
		reg = 0;
		while (isdigit(peek())) {
			reg *= 10;
			reg += get() - '0';
		}
		return TOKEN_REGISTER;
	}
	
	// Read mnemonic
	if (isupper(peek())) {
		string mnemonic = "";
		while (isupper(peek()) || peek() == '_')
			mnemonic += get();
		return getMnemonic(mnemonic);
	}
	
	// Read integer
	if (isdigit(peek()) || (peek() == '-')) {
		bool negative = false;
		if (peek() == '-') {
			get();
			negative = true;
		}
		integer = 0;
		while (isdigit(peek())) {
			integer *= 10;
			integer += get() - '0';
		}
		if (negative) integer *= -1;
		return TOKEN_INTEGER;
	}
	
	// Read address
	if (peek() == '@') {
		get();
		address = 0;
		while (isdigit(peek())) {
			address *= 10;
			address += get() - '0';
		}
		return TOKEN_ADDRESS;
	}
	
	return TOKEN_UNKNOWN;
}

AsmScanner::AsmScanner()
{
	reset(NULL);
}

AsmScanner::~AsmScanner()
{
	_input = NULL;
}

void AsmScanner::reset(istream *input)
{
	_input        = input;
	_current      = -1;
	_currentToken = TOKEN_NONE;
	
	label         = "";
	reference     = "";
	address       = 0;
	reg           = 0;
	integer       = 0;
}

AsmScanner::Tokens AsmScanner::peekToken()
{
	if (_currentToken == TOKEN_NONE) {
		_currentToken = readToken();
		// cerr << "TOKEN: " << getTokenName(_currentToken) << endl;
	}
	return _currentToken;
}

AsmScanner::Tokens AsmScanner::getToken()
{
	Tokens value = peekToken();
	_currentToken = TOKEN_NONE;
	return value;
}

const char * AsmScanner::getTokenName(AsmScanner::Tokens token)
{
	switch (token) {
	case TOKEN_NONE:      return "NONE";
	
	case TOKEN_LABEL:     return "LABEL";
	case TOKEN_REGISTER:  return "REGISTER";
	case TOKEN_INTEGER:   return "INTEGER";
	case TOKEN_REFERENCE: return "REFERENCE";
	case TOKEN_ADDRESS:   return "ADDRESS";
	
	#define M(name, nothing, integer, reference, address, register) case TOKEN_##name: return #name;
	#include "mnemonics.h"
	
	case TOKEN_END:       return "END";
	case TOKEN_UNKNOWN:   return "UNKNOWN";
	default:              return "[NOT A TOKEN]";
	}
}

