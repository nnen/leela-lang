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
	_contexts->next();
}

void Parser::endContext(vector<Ref<Object> >& match, Ref<Object>& result)
{
	_contexts->close();
}

void Parser::startChunk(vector<Ref<Object> >& match, Ref<Object>& result)
{
	_writer.startChunk();
}

void Parser::endChunk(vector<Ref<Object> >& match, Ref<Object>& result)
{
	_writer.endChunk();
}

void Parser::parse(Ref<Input> input, Ref<Output> output)
{
	// 1. pass
	
	_input = input;
	_output = output;
	_lexer = new Lexer(input);
	_contexts = new ContextTable();
	_writer.clear();
	
	parseProgram();
	
	// 2. pass
	
	_lexer = new Lexer(input);
	_contexts->reset();
	_writer.clear();
	
	parseProgram();
	
	_lexer = NULL;
	_input = NULL;
	_output = NULL;
	_writer.output(output);
}

