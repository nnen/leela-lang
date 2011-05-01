/**
 * \file   leela/Token.h
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-03-24
 *
 * \brief  Header file for the Token class and derived classes.
 */

#ifndef TOKEN_H_434634DFBDV
#define TOKEN_H_434634DFBDV

#include <string>
#include <ostream>

#include "Char.h"
#include "Value.h"
#include "Set.h"

#define FIRST_STATE (__LINE__ * 100)

using namespace std;

struct CharLocation {
public:
	int line;
	int column;

	CharLocation() : line(-1), column(-1) {}
	CharLocation(int line, int column) : line(line), column(column) {}
	CharLocation(const CharLocation& other) : line(other.line), column(other.column) {}

	CharLocation& operator ++ (int dummy) { column++; return *this; }
	CharLocation& operator += (int width) { column += width; return *this; }
	
	CharLocation& newLine() { line++; column = 0; return *this; }

	bool isKnown() const { return ((line >= 0) && (column >= 0)); }
};

struct Token {
public:
	enum Type {
		#define T(name, repr) name,
		#define TK(name, str, repr) name,
		#define TD(name, ch, repr) name,
		#define T2(name, ch1, ch2, repr) name,
		
		#include "token_types.h"
		
		TYPE_COUNT
	};
	
	Type         type;
	CharLocation location;
	Ref<Value>   value;
	
	Token();
	Token(Type type);
	Token(Type type, string str);
	Token(Type type, string str, CharLocation location);
	~Token() { }

	void setString(Type type, string str);
	
	static Ref<Value> parseNumber(string str);
	
	static Type getDelimiter(Char c);
	static Type getDelimiter(Char c1, Char c2);
	
	static const char * getTypeName(Type type);
	static const char * getRepresentation(Type type);
};

ostream& operator << (ostream& output, Token& token);

bool operator < (Token::Type a, Token::Type b);
bool operator > (Token::Type a, Token::Type b);

#endif /* end of include guard: TOKEN_H_434634DFBDV */

