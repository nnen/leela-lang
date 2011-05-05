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

/* SEMANTIC ACTIONS **********************************************************************/
void Parser::startContext(Ref<Object>           inherited,
                          vector<Ref<Object> >  siblings,
                          vector<Ref<Object> >& match,
                          Ref<Object>&          result)
{
	_contexts->next();
}

void Parser::endContext(Ref<Object>           inherited,
                        vector<Ref<Object> >  siblings,
                        vector<Ref<Object> >& match,
                        Ref<Object>&          result)
{
	_contexts->close();
}

void Parser::startChunk(Ref<Object>           inherited,
                        vector<Ref<Object> >  siblings,
                        vector<Ref<Object> >& match,
                        Ref<Object>&          result)
{
	_writer.startChunk();
}

void Parser::endChunk(Ref<Object>           inherited,
                      vector<Ref<Object> >  siblings,
                      vector<Ref<Object> >& match,
                      Ref<Object>&          result)
{
	_writer.endChunk();
}

void Parser::writeChunks(Ref<Object>           inherited,
                         vector<Ref<Object> >  siblings,
                         vector<Ref<Object> >& match,
                         Ref<Object>&          result)
{
	_writer.endAllChunks();
}

void Parser::writeStrings(Ref<Object>           inherited,
                          vector<Ref<Object> >  siblings,
                          vector<Ref<Object> >& match,
                          Ref<Object>&          result)
{
	_writer.writeComment("===== Strings section =====");
	foreach (str, _strings->getStrings()) {
		_writer.writeLabel(_strings->getLabel(*str));
		_writer.write(*str);
	}
}

void Parser::startFunction(Ref<Object>           inherited,
                           vector<Ref<Object> >  siblings,
                           vector<Ref<Object> >& match,
                           Ref<Object>&          result)
{
	startChunk(match, result);
	startContext(match, result);
	_writer.pushLabel("function");
}

void Parser::endFunction(vector<Ref<Object> >& match, Ref<Object>& result)
(Ref<Object>           inherited,
                         vector<Ref<Object> >  siblings,
                         vector<Ref<Object> >& match,
                         Ref<Object>&          result)
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
(Ref<Object>           inherited,
                         vector<Ref<Object> >  siblings,
                         vector<Ref<Object> >& match,
                         Ref<Object>&          result)

{
	_contexts->current()->addLocal(match.back().as<String>()->getValue());
}

void Parser::aloocLocals(Ref<Object>           inherited,
                         vector<Ref<Object> >  siblings,
                         vector<Ref<Object> >& match,
                         Ref<Object>&          result)
{
	_writer.writeInstruction(
		AsmScanner::TOKEN_ALLOC,
		_contexts->current()->getLocalCount()
	);
}

void Parser::addArg(Ref<Object>           inherited,
                    vector<Ref<Object> >  siblings,
                    vector<Ref<Object> >& match,
                    Ref<Object>&          result)

{
	_contexts->current()->addParam(match.back().as<String>()->getValue());
}

void Parser::pushNumber(Ref<Object>           inherited,
                        vector<Ref<Object> >  siblings,
                        vector<Ref<Object> >& match,
                        Ref<Object>&          result)

{
	_writer.writeInstruction(
		AsmScanner::TOKEN_PUSH,
		match.back().as<Number>()->getValue()
	);
}

void Parser::pushString(Ref<Object>           inherited,
                        vector<Ref<Object> >  siblings,
                        vector<Ref<Object> >& match,
                        Ref<Object>&          result)
{
	_writer.writeInstruction(
		AsmScanner::TOKEN_PUSH,
		_strings->getLabel(match.back().as<String>()->getValue());
	);
}

void Parser::getSymbolValue(Ref<Object>           inherited,
                            vector<Ref<Object> >  siblings,
                            vector<Ref<Object> >& match,
                            Ref<Object>&          result)
{
	Ref<Symbol> symbol =
		_contexts->current()->getSymbol(match.back().as<String>()->getValue());
	_writer.writeInstruction(AsmScanner::TOKEN_LOAD, symbol->index);
}

void Parser::assignVar(Ref<Object>           inherited,
                       vector<Ref<Object> >  siblings,
                       vector<Ref<Object> >& match,
                       Ref<Object>&          result)
{
	ident = siblings[0]->as<String>();
	if (ident.isNull()) throw Exception("Parsing error.");
	
	_writer.writeInstruction(
		AsmScanner::TOKEN_STORE,
		_contexts->current->getSymbol(ident->getValue()).index
	);
}

void Parser::identToString(Ref<Object>           inherited,
                           vector<Ref<Object> >  siblings,
                           vector<Ref<Object> >& match,
                           Ref<Object>&          result)
{
	ident = match->back()->as<String>();
	if (ident.isNull()) throw Exception("Parsing error.");
	
	_writer.writeInstruction(
		AsmScanner::PUSH_STRING
	);
}

void Parser::syntaxError(Ref<Object>           inherited,
                         vector<Ref<Object> >  siblings,
                         vector<Ref<Object> >& match,
                         Ref<Object>&          result)
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

