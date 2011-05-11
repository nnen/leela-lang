/**
 * \file   leela/Parser.cpp
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-03-29
 *
 * \brief  Implementation of the Parser class.
 */

#include "Parser.h"
#include "Machine.h"

SyntaxError::SyntaxError(const Parser& parser) throw()
	: Exception()
{
	_token = parser._lexer->peek();
}

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

void SyntaxError::print(ostream& output)
{
	if (_token.location.isKnown())
		output << "Syntax error at line " << _token.location.line << ": ";
	else
		output << "Syntax error: ";
	output << _message.str();
}

Token Parser::accept(Token::Type type)
{
	if (peek().type != type) {
		SyntaxError e(*this);
		e.getStream() << "Unexpected token (" << Token::getTypeName(peek().type)
		              << "). Expected: " << Token::getTypeName(type);
		throw e;
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

// If-Else statement

SEMANTIC_ACTION(ifJump)
{
	_writer.writeInstruction(
		AsmScanner::TOKEN_JUMP_IF,
		_writer.pushLabel("if")
	);
}

SEMANTIC_ACTION(elseJump)
{
	string elseLabel = _writer.popLabel();

	_writer.writeInstruction(
		AsmScanner::TOKEN_JUMP,
		_writer.pushLabel("else")
	);
	
	_writer.writeLabel(elseLabel);
}

SEMANTIC_ACTION(ifEnd)
{
	_writer.writeLabel(_writer.popLabel());
}

// While statement

SEMANTIC_ACTION(startWhile)
{
	_writer.writeLabel(_writer.pushLabel("while"));
}

SEMANTIC_ACTION(whileJump)
{
	string endLabel = _writer.pushLabel("while_end");
	_writer.writeInstruction(
		AsmScanner::TOKEN_JUMP_IF,
		endLabel
	);
}

SEMANTIC_ACTION(endWhile)
{
	string endLabel = _writer.popLabel();
	string startLabel = _writer.popLabel();
	
	_writer.writeInstruction(
		AsmScanner::TOKEN_JUMP,
		startLabel
	);
	
	_writer.writeLabel(endLabel);
}

SEMANTIC_ACTION(startFunction)
{
	_writer.startChunk();
	_contexts->next();
	// _writer.pushLabel("function");
	_writer.writeLabel(_writer.pushLabel("function"));
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
	Ref<String> ident = match[match.size() - 3];
	Ref<Value>  value = match[match.size() - 1];
	
	UInteger index = Machine::BUILTIN_COUNT + _constants.size();
	_constants[ident->getValue()] = index;
	
	_writer.writeInstruction(
		AsmScanner::TOKEN_DEF_CONST);
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
	string ident = ((Ref<String>) match.back())->getValue();
	
	// Check whether the symbol is a builtin
	UInteger index = 0;
	if (Machine::getBuiltinIndex(ident, index)) {
		_writer.writeInstruction(AsmScanner::TOKEN_LOAD_CONST, index);
		return;
	}
	
	// Check whether the symbol is a constant
	if (_constants.count(ident) > 0) {
		_writer.writeInstruction(
			AsmScanner::TOKEN_LOAD_CONST,
			_constants[ident]
		);
		return;
	}
	
	// Find the symbol definition in the local context
	Ref<Symbol> symbol =
		_contexts->current()->getSymbol(ident);
	if (symbol.isNull())
		onUndefinedSymbol(ident);
	// Write LOAD instruction for the symbol found	
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
	
	Ref<Symbol> symbol = _contexts->current()->getSymbol(ident->getValue());
	if (symbol.isNull())
		onUndefinedSymbol(ident->getValue());
	
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
	Ref<String> ident = (Ref<String>) match.back();
	
	_writer.writeInstruction(
		AsmScanner::TOKEN_PUSH,
		_strings->getLabel(ident)
	);
}

void Parser::unexpectedToken(Ref<Object> inherited,
                             Match siblings,
					    Match& match,
					    Ref<Object>& result)
{
	stringstream s;
	s << "Unexpected token: " << Token::getTypeName(_lexer->peek().type);
	throw SyntaxError(_lexer->peek(), s.str());
}

/*
void Parser::syntaxError(int line, string message)
{
	cerr << "Syntax error at line " << line << ": " << message << endl;
}
*/

void Parser::onUndefinedSymbol(string name)
{
	SyntaxError e(*this);
	e.getStream() << "Undefined symbol: " << name << ".";
	throw e;
}

bool Parser::parse(Ref<Input> input, Ref<Output> output)
{
	// 1. pass
	
	_input = new BufferedInput(*input);
	_output = output;
	_lexer = new Lexer(_input);
	_contexts = new ContextTable();
	_constants.clear();
	_strings = new StringTable();
	_nonterminals.clear();
	_writer.clear();

	try {
		parseProgram(NULL, vector<Ref<Object> >());
	} catch (SyntaxError e) {
		e.print(cerr);
		cerr << endl;
		cerr << "Nonterminals: ";
		foreach (nonterminal, _nonterminals)
			cerr << *nonterminal << "  ";
		cerr << endl;
		return false;
	}
	
	// 2. pass
	
	_input->rewind();
	_lexer = new Lexer(_input);
	_contexts->reset();
	_constants.clear();
	_nonterminals.clear();
	_writer.clear();
	
	parseProgram(NULL, vector<Ref<Object> >());
	
	_lexer = NULL;
	_input = NULL;
	_output = NULL;
	_writer.output(output);

	return true;
}

