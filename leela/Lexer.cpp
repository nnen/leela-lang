/**
 * \file   leela/Lexer.cpp
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-03-25
 *
 * \brief  Implementation of the Lexer class.
 */

#include <cassert>

#include "leela.h"
#include "Lexer.h"

Char Lexer::current()
{
	if (!(_current.isChar() || _next.isChar()))
		advance();
	return _current;
}

Char Lexer::next()
{
	if (!_next.isChar())
		advance();
	return _next;;
}

Char Lexer::advance()
{
	assert(_input != NULL);
	
	Char result = _current;
	
	do {
		if (_current.isChar()) {
			_location++;
			if (_current.isNewLine()) _location.newLine();
		}

		_current = _next;
		_next = Char(_next, _input->stream().get());
	} while ((!_current.isChar()) && _next.isChar());
	
	return result;
}

Token Lexer::getToken()
{
	while (current().isChar()) {
		switch (_state) {
		case STATE_INIT:
			_tokenLocation = _location;
			
			if (current().isWhitespace()) {
				advance();
			} else if (current().isDoubleDelimiter()) {
				Token::Type type = Token::getDelimiter(current(), next());
				if (type != Token::UNKNOWN) {
					advance(); advance();
					return endToken(type);
				}
				if (current().isSingleDelimiter())
					return endToken(Token::getDelimiter(advance()));
			} else if (current().isSingleDelimiter()) {
				return endToken(Token::getDelimiter(advance()));
			} else if (current().isLetter()) {
				_state = STATE_IDENT;
				append();
			} else if (current().isDigit()) {
				_state = STATE_NUMBER;
				append();
			} else if (current() == '"') {
				_state = STATE_STRING;
				advance();
			} else {
				_state = STATE_UNKNOWN;
			}
			break;
		case STATE_IDENT:
			if (current().isLetter() || current().isDigit()) {
				append();
			} else {
				#define TK(name, str) if (_tokenString == str) return endToken(Token::KW_##name);
				#include "token_types.h"
				return endToken(Token::IDENTIFIER);
			}
			break;
		case STATE_NUMBER:
			if (current().isDigit()) {
				append();
			} else if (current().isDelimiter()) {
				return endToken(Token::NUMBER_LITERAL);
			} else {
				append();
				_state = STATE_UNKNOWN;
			}
			break;
		case STATE_STRING:
			if (current() == '"') {
				advance();
				return endToken(Token::STRING_LITERAL);
			} else if (current() == '\\') {
				advance();
				_state = STATE_ESCAPED;
			} else {
				append();
			}
			break;
		case STATE_ESCAPED:
			switch (current().value) {
			case 'n': advance(); append('\n'); break;
			case 't': advance(); append('\t'); break;
			default: append(); break;
			}
			_state = STATE_STRING;
			break;
		
		case STATE_UNKNOWN:
			if (current().isDelimiter())
				return endToken(Token::UNKNOWN);
			append();
			break;
		
		case STATE_COUNT:
		default:
			_state = STATE_INIT;
			break;
		}
	}
	return Token(Token::END);
}

const char * Lexer::getStateName(Lexer::LexerState state)
{
	switch (state) {
	#define S(name) case STATE_##name: return #name;
	LEXER_STATES
	#undef S
	default: return "[UNKNOWN LEXER STATE]";
	}
}

Token Lexer::peek()
{
	return _token;
}

Token Lexer::get()
{
	Token result = _token;
	_token = getToken();
	return result;
}

Lexer::Lexer(Ref<Input> input)
{
	if (input == NULL)
		throw exception();
	
	_input = input;
	_state = STATE_INIT;
	_location = CharLocation(0, 0);
	
	_token = getToken();
}

void Lexer::dumpTokens(ostream& output)
{
	Token token;
	int lastLine = -1;
	
	while ((token = getToken()).type != Token::END) {
		if (token.location.line != lastLine)
			output << std::endl << "Line " << token.location.line << ":" << std::endl;
		output << "   " << setiosflags(ios_base::left) << setw(16) << Token::getTypeName(token.type)
		       << "   " << token.location.line << ":" << setw(4) << token.location.column;
		if (!token.value.isNull())
				 output << " " << *(token.value);
		output << std::endl;
		// output << "\t" << token << std::endl;

		lastLine = token.location.line;
	}
}

void Lexer::dumpState(ostream& output)
{
	output << "LEXER:" << endl;
	output << "\tSTATE:   " << getStateName(_state) << endl;
	output << "\tCURRENT: " << _current             << endl;
	output << "\tNEXT:    " << _next                << endl;
	output << "\tSTRING:  " << _tokenString         << endl;
}

