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
	if (peek().type != type) {
		// TODO: Add syntax error handling here.
		stringstream s;
		s << "Unexpected token. Expected: " << Token::getTypeName(type);
		throw SyntaxError(s.str());
	}
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

void Parser::startFunction(vector<Ref<Object> >& match, Ref<Object>& result)
{
	startChunk(match, result);
	startContext(match, result);
	_writer.pushLabel("function");
}

void Parser::endFunction(vector<Ref<Object> >& match, Ref<Object>& result)
{
	Ref<Context> context = _contexts->current();
	
	endContext(match, result);
	endChunk(match, result);
	_writer.makeFunction();
	
	foreach (freeVar, context->getFreeVars()) {
		_writer.writeInstruction(
			AsmScanner::TOKEN_LOAD_CLOSURE,
			(*freeVar)->freeVar->index
		);
	}
}

void Parser::addLocal(vector<Ref<Object> >& match, Ref<Object>& result)
{
	_contexts->current()->addLocal(match.back().as<String>()->getValue());
}

void Parser::allocLocals(vector<Ref<Object> >& match, Ref<Object>& result)
{
	_writer.writeInstruction(
		AsmScanner::TOKEN_ALLOC,
		_contexts->current()->getLocalCount()
	);
}

void Parser::addArg(vector<Ref<Object> >& match, Ref<Object>& result)
{
	_contexts->current()->addParam(match.back().as<String>()->getValue());
}

void Parser::pushNumber(vector<Ref<Object> >& match, Ref<Object>& result)
{
	_writer.writeInstruction(
		AsmScanner::TOKEN_PUSH,
		match.back().as<Number>()->getValue()
	);
}

void Parser::getSymbolValue(vector<Ref<Object> >& match, Ref<Object>& result)
{
	Ref<Symbol> symbol =
		_contexts->current()->getSymbol(match.back().as<String>()->getValue());
	_writer.writeInstruction(AsmScanner::TOKEN_LOAD, symbol->index);
}

void Parser::syntaxError(vector<Ref<Object> >& match, Ref<Object>& result)
{
	throw SyntaxError("Syntax error!");
}

void Parser::unexpectedToken(vector<Ref<Object> >& match, Ref<Object>& result)
{
	stringstream s;
	s << "Unexpected token: " << Token::getTypeName(_lexer->peek().type);
	throw SyntaxError(s.str());
}

void Parser::parse(Ref<Input> input, Ref<Output> output)
{
	// 1. pass
	
	_input = new BufferedInput(*input);
	_output = output;
	_lexer = new Lexer(_input);
	_contexts = new ContextTable();
	_writer.clear();

	parseProgram();
	
	// 2. pass
	
	_input->rewind();
	_lexer = new Lexer(_input);
	_contexts->reset();
	_writer.clear();
	
	parseProgram();
	
	_lexer = NULL;
	_input = NULL;
	_output = NULL;
	_writer.output(output);
}

