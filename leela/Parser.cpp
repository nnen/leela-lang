/**
 * \file   leela/Parser.cpp
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-03-29
 *
 * \brief  Implementation of the Parser class.
 */

#include "Parser.h"

SyntaxError::SyntaxError(string message) throw()
	: Exception(message)
{
}

Token Parser::accept(Token::Type type)
{
	if (peek().type != type)
		// TODO: Add syntax error handling here.
		return accept();
	return accept();
}

void Parser::startContext(vector<Ref<Object> >& match, Ref<Object>& result)
{
	_contexts.next();
}

void Parser::closeContext(vector<Ref<Object> >& match, Ref<Object>& result)
{
	_contexts.close();
}

void Parser::makeIdentList(vector<Ref<Object> >& match, Ref<Object>& result)
{
	result = new Box<list<string> >();
}

void Parser::appendIdentList(vector<Ref<Object> >& match, Ref<Object>& result)
{
	BRef<list<string> > list;
	BRef<string>        ident;
	
	if (match.size() > 2) {
		list = match[2];
		ident = match[1];
	} else {
		list = match[1];
		ident = match[0];
	}
	
	list->push_front(*ident);
	result = list;
}

void Parser::parse(Ref<Input> input, Ref<Output> output)
{
	// 1. pass
	
	_input = input;
	_output = output;
	_lexer = new Lexer(input);
	_contexts.reset();
	_writer.clear();
	
	parseProgram();
	
	// 2. pass
	
	_contexts.reset();
	_writer.clear();
	
	parseProgram();
	
	_lexer = NULL;
	_input = NULL;
	_output = NULL;
	_writer.output(output);
}

