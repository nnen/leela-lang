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
	case NUMBER_LITERAL:
		this->value = Number::parse(str);
		break;
	case IDENTIFIER:
	case STRING_LITERAL:
		this->value = new String(str);
		break;
	default:
		break;
	}
}

Token::Type Token::getDelimiter(Char c)
{
	switch ((char) c) {
	#define TD(name, ch) case ch: return name;
	#include "token_types.h"
	}
	return UNKNOWN;
}

Token::Type Token::getDelimiter(Char c1, Char c2)
{
	#define T2(name, ch1, ch2) if (((char)c1 == ch1) && ((char)c2 == ch2)) return name;
	#include "token_types.h"
	
	return UNKNOWN;
}

const char * Token::getTypeName(Type type)
{
	switch (type) {
	#define T(name)            case name:      return #name;
	#define TK(name, str)      case KW_##name: return "KW_" #name;
	#define TD(name, ch)       case name:      return #name;
	#define T2(name, ch1, ch2) case name:      return #name;
	#include "token_types.h"
	default: return "[UNKNOWN TOKEN TYPE]";
	}
}

ostream& operator << (ostream& output, const Token& token)
{
	output << "Token(" << Token::getTypeName(token.type);

	switch (token.type) {
	case Token::IDENTIFIER:
	case Token::NUMBER_LITERAL:
	case Token::STRING_LITERAL:
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

