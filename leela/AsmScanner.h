/**
 * \file   AsmScanner.h
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-04-16
 *
 * \brief  
 */

#ifndef ASM_SCANNER_H_23098SRFTGB567
#define ASM_SCANNER_H_23098SRFTGB567

#include <iostream>
#include <map>

#include "leela.h"
#include "Object.h"

using namespace std;

class AsmScanner : public Object {
public:
	enum Tokens {
		TOKEN_NONE, 
		
		TOKEN_LABEL,
		TOKEN_REGISTER,
		TOKEN_INTEGER,
		TOKEN_REFERENCE,
		TOKEN_ADDRESS,
		
		#define M(name, nothing, integer, reference, address, register) TOKEN_##name,
		#include "mnemonics.h"
		
		TOKEN_END,
		TOKEN_UNKNOWN
	};

private:
	
	static map<string, Tokens> _mnemonics;
	
	istream * _input;
	int       _current;
	Tokens    _currentToken;
	
	static void   initMnemonics();
	static Tokens getMnemonic(string str);
	
	int    peek();
	int    get();
	void   skipWhitespace();
	Tokens readToken();

public:
	string   label;
	string   reference;
	Address  address;
	UInteger reg;
	Integer  integer;
	
	AsmScanner();
	virtual ~AsmScanner();
	
	void reset(istream *input);
	
	Tokens peekToken();
	Tokens getToken();
	
	static const char * getTokenName(Tokens token);
	static const char * getMnemonic(Tokens mnemonic);
	static int          getLongestMnemonic();
};

#endif /* end of include guard: ASM_SCANNER_H_23098SRFTGB567 */

