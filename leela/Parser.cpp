/**
 * \file   leela/Parser.cpp
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-03-29
 *
 * \brief  Implementation of the Parser class.
 */

#include "Parser.h"

SyntaxError::SyntaxError(Token token, string message) throw()
	: Exception(), _token(token)
{
	stringstream s;
	s << "Syntax error at line " << token.location.line << ": " << message;
	setMessage(s.str());
}

SyntaxError::SyntaxError(string message) throw()
	: Exception()
{
	stringstream s;
	s << "Syntax error: " << message;
	setMessage(s.str());
}

Token Parser::accept(Token::Type type)
{
	if (peek().type != type) {
		// TODO: Add syntax error handling here.
		stringstream s;
		s << "Unexpected token. Expected: " << Token::getTypeName(type);
		throw SyntaxError(peek(), s.str());
	}
	return accept();
}

/* SEMANTIC ACTIONS **********************************************************************/

#define SEMANTIC_ACTION(name) void Parser::name(Ref<Object> inherited, \
	Match siblings, \
	Match& match, \
	Ref<Object>& result)

SEMANTIC_ACTION(startContext)
{
	_contexts->next();
}

/*
SEMANTIC_ACTION(endContext)
{
	_contexts->close();
}

SEMANTIC_ACTION(startChunk)
{
	_writer.startChunk();
}
*/

/*
SEMANTIC_ACTION(endChunk)
{
	_writer.endChunk();
}
*/

SEMANTIC_ACTION(writeChunks)
{
	_writer.endAll();
}

SEMANTIC_ACTION(writeStrings)
{
	_writer.writeComment("===== Strings section =====");

	vector<Ref<String> > strings = _strings->getStrings();
	
	foreach (str, strings) {
		Ref<String> s = *str;
		if (s.isNull()) throw Exception();
		_writer.writeLabel(_strings->getLabel(s));
		_writer.write(s);

		//if (str->isNull()) throw Exception();
		//_writer.writeLabel(_strings->getLabel(*str));
		//if (str->isNull()) throw Exception();
		//_writer.write(*str);
	}
}

SEMANTIC_ACTION(startFunction)
{
	_writer.startChunk();
	_contexts->next();
	_writer.pushLabel("function");
}

SEMANTIC_ACTION(endFunction)
{
	Ref<Context> context = _contexts->current();
	
	_contexts->close();
	_writer.endChunk();
	
	_writer.writeInstruction(
		AsmScanner::TOKEN_PUSH,
		context->getParamCount()
	);
	_writer.writeInstruction(
		AsmScanner::TOKEN_MAKE,
		_writer.popLabel()
	);
	
	foreach (freeVar, context->getFreeVars()) {
		_writer.writeInstruction(
			AsmScanner::TOKEN_LOAD_CLOSURE,
			(*freeVar)->freeVar->index
		);
	}
}

SEMANTIC_ACTION(addConst)
{
	// TODO: Implement this semantic action.
}

SEMANTIC_ACTION(addLocal)
{
	_contexts->current()->addLocal(match.back().as<String>()->getValue());
}

SEMANTIC_ACTION(allocLocals)
{
	_writer.writeInstruction(
		AsmScanner::TOKEN_ALLOC,
		_contexts->current()->getLocalCount()
	);
}

SEMANTIC_ACTION(addArg)
{
	_contexts->current()->addParam(match.back().as<String>()->getValue());
}

SEMANTIC_ACTION(pushNumber)
{
	_writer.writeInstruction(
		AsmScanner::TOKEN_PUSH,
		match.back().as<Number>()->getValue()
	);
}

SEMANTIC_ACTION(pushString)
{
	_writer.writeInstruction(
		AsmScanner::TOKEN_PUSH,
		_strings->getLabel(match.back().as<String>())
	);
}

SEMANTIC_ACTION(getSymbolValue)
{
	Ref<Symbol> symbol =
		_contexts->current()->getSymbol(match.back().as<String>()->getValue());
	_writer.writeInstruction(AsmScanner::TOKEN_LOAD, symbol->index);
}

SEMANTIC_ACTION(writeCall)
{
	Ref<Number> argCount = (Ref<Number>) match.back();
	_writer.writeInstruction(
		AsmScanner::TOKEN_CALL,
		(UInteger) argCount->getValue()
	);
}

SEMANTIC_ACTION(returnZero)
{
	result = new Number(0);
}

SEMANTIC_ACTION(returnPlusOne)
{
	Ref<Number> number = (Ref<Number>) match.back();
	result = new Number(number->getValue() + 1);
}

SEMANTIC_ACTION(assignVar)
{
	Ref<String> ident = (Ref<String>) siblings[0];
	//Ref<String> ident = siblings[0].as<String>();
	//if (ident.isNull()) throw Exception("Parsing error.");
	
	_writer.writeInstruction(
		AsmScanner::TOKEN_STORE,
		_contexts->current()->getSymbol(ident->getValue())->index
	);
}

SEMANTIC_ACTION(getValueForLookup)
{
	Ref<String> ident = (Ref<String>) siblings[0];
	
	_writer.writeInstruction(
		AsmScanner::TOKEN_LOAD,
		_contexts->current()->getSymbol(ident->getValue())->index
	);
}

/*
SEMANTIC_ACTION(assignIndex)
{
	// TODO: Implement the "assignIndex" semantic actions.
}
*/

SEMANTIC_ACTION(identToString)
{
	//Ref<String> ident = match.back().as<String>();
	//if (ident.isNull()) throw Exception("Parsing error.");
	Ref<String> ident = (Ref<String>) match.back();
	
	_writer.writeInstruction(
		AsmScanner::TOKEN_PUSH,
		_strings->getLabel(ident)
	);
}

//SEMANTIC_ACTION(syntaxError)
//{
//	throw SyntaxError("Syntax error!");
//}

void Parser::unexpectedToken(Ref<Object> inherited,
                             Match siblings,
					    Match& match,
					    Ref<Object>& result)
{
	stringstream s;
	s << "Unexpected token: " << Token::getTypeName(_lexer->peek().type);
	throw SyntaxError(s.str());
}

/*
void Parser::syntaxError(int line, string message)
{
	cerr << "Syntax error at line " << line << ": " << message << endl;
}
*/

bool Parser::parse(Ref<Input> input, Ref<Output> output)
{
	// 1. pass
	
	_input = new BufferedInput(*input);
	_output = output;
	_lexer = new Lexer(_input);
	_contexts = new ContextTable();
	_strings = new StringTable();
	_writer.clear();

	try {
		parseProgram(NULL, vector<Ref<Object> >());
	} catch (SyntaxError e) {
		e.print(cerr);
		cerr << endl;
		return false;
	}
	
	// 2. pass
	
	_input->rewind();
	_lexer = new Lexer(_input);
	_contexts->reset();
	_writer.clear();
	
	parseProgram(NULL, vector<Ref<Object> >());
	
	_lexer = NULL;
	_input = NULL;
	_output = NULL;
	_writer.output(output);

	return true;
}

