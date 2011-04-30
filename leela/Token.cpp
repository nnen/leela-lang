/**
 * \file   leela/Token.cpp
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-03-24
 *
 * \brief  Implementation of the Token class and derived classes.
 */

#include "Token.h"

Token::Token()
	: type(Token::UNKNOWN), location(), value()
{
}

Token::Token(Token::Type type)
	: type(type), location(), value()
{
}

Token::Token(Token::Type type, string str)
	: type(type), location(), value()
{
	setString(type, str);
}

Token::Token(Token::Type type, string str, CharLocation location)
	: type(type), location(location), value()
{
	setString(type, str);
}

void Token::setString(Token::Type type, string str)
{
	this->type = type;
	switch (type) {
	case NUMBER:
		this->value = Number::parse(str);
		break;
	case IDENT:
	case STRING:
		this->value = new String(str);
		break;
	default:
		break;
	}
}

Token::Type Token::getDelimiter(Char c)
{
	switch ((char) c) {
	#define TD(name, ch, repr) case ch: return name;
	#include "token_types.h"
	}
	return UNKNOWN;
}

Token::Type Token::getDelimiter(Char c1, Char c2)
{
	#define T2(name, ch1, ch2, repr) if (((char)c1 == ch1) && ((char)c2 == ch2)) return name;
	#include "token_types.h"
	
	return UNKNOWN;
}

const char * Token::getTypeName(Type type)
{
	switch (type) {
	#define T(name, repr)            case name: return #name;
	#define TK(name, str, repr)      case name: return #name;
	#define TD(name, ch, repr)       case name: return #name;
	#define T2(name, ch1, ch2, repr) case name: return #name;
	#include "token_types.h"
	default: return "[UNKNOWN TOKEN TYPE]";
	}
}

const char * Token::getRepresentation(Type type)
{
	switch (type) {
	#define T(name, repr)            case name: return repr;
	#define TK(name, str, repr)      case name: return repr;
	#define TD(name, ch, repr)       case name: return repr;
	#define T2(name, ch1, ch2, repr) case name: return repr;
	#include "token_types.h"
	default: return "[UNKNOWN TOKEN TYPE]";
	}
}

ostream& operator << (ostream& output, const Token& token)
{
	output << "Token(" << Token::getTypeName(token.type);

	switch (token.type) {
	case Token::IDENT:
	case Token::NUMBER:
	case Token::STRING:
		if (token.value.isNull())
			output << ", NULL";
		else
			output << ", " << *(token.value);
		break;
	default: break;
	}

	if (token.location.isKnown())
		output << ", " << token.location.line << ":" << token.location.column;
	output << ")";
	return output;
}

bool operator < (Token::Type a, Token::Type b)
{
	return (int)a < (int)b;
}

bool operator > (Token::Type a, Token::Type b)
{
	if (a < b) return false;
	if (a == b) return false;
	return true;
}

