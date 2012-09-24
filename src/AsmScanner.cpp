/**
 * \file   AsmScanner.cpp
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-04-16
 *
 * \brief  
 */

#include <cstring>

#include "AsmScanner.h"
#include "Value.h" // Used for string utility functions defined in String class.

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
	if (c == '\n') line++;
	_current = -1;
	return c;
}

void AsmScanner::skipWhitespace()
{
	while (isspace(peek())) get();
	while (peek() == ';') {
		while ((peek() != '\n') && (peek() >= 0)) get();
		while (isspace(peek())) get();
	}
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
		reg = get() - '0';
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

	// Read string
	if (peek() == '"') {
		str = "";
		get();
		while (peek() != '"') {
			if (peek() == '\\') {
				get();
				switch (peek()) {
				case 'n': str += '\n'; break;
				case 't': str += '\t'; break;
				case 'x': {
					get();
					char d1 = get();
					char d2 = peek();
					str += String::hexToChar(d1, d2);
					} break;
				default: str += peek(); break;
				}
				get();
			} else {
				str += get();
			}
		}
		get();
		return TOKEN_STRING;
	}
	
	cerr << line << ": Unknown token starting with character \"" << (char) peek() << "\"." << endl;
	
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
	
	line          = 0;
	
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
	case TOKEN_STRING:    return "STRING";
	
	#define M(name, nothing, integer, reference, address, register) case TOKEN_##name: return #name;
	#include "mnemonics.h"
	
	case TOKEN_END:       return "END";
	case TOKEN_UNKNOWN:   return "UNKNOWN";
	default:              return "[NOT A TOKEN]";
	}
}

const char * AsmScanner::getMnemonic(AsmScanner::Tokens mnemonic)
{
	switch (mnemonic) {
	#define M(name, nothing, integer, reference, address, register) \
		case TOKEN_##name: return #name;
	#include "mnemonics.h"
	default: return NULL;
	}
}

int AsmScanner::getLongestMnemonic()
{
	int length = 0;
	#define M(name, nothing, integer, reference, address, register) \
		if ((int)strlen(#name) > length) length = strlen(#name);
	#include "mnemonics.h"
	return length;
}

