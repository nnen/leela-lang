/**
 * \file   leela/Parser.h
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-03-29
 *
 * \brief  Header file for the Parser class.
 */

#ifndef PARSER_H_23VERTBYU8N
#define PARSER_H_23VERTBYU8N

#include <vector>
#include <list>
#include <stack>
#include <map>
#include <set>

#include "leela.h"
#include "Object.h"
#include "Lexer.h"
#include "Token.h"
#include "Context.h"
#include "Input.h"
#include "Output.h"
#include "Exception.h"
#include "AsmWriter.h"

using namespace std;

class SyntaxError : public Exception {
public:
	SyntaxError(string message) throw();
	virtual ~SyntaxError() throw() {}
};

class Parser {
public:
	#include "Parser.rules.h"

private:
	Ref<BufferedInput> _input;
	Ref<Output>        _output;
	Ref<Lexer>         _lexer;
	
	Ref<ContextTable> _contexts;
	AsmWriter         _writer;
	
	Token peek() { return _lexer->peek(); }
	Token accept() { return _lexer->get(); }
	Token accept(Token::Type type);
	
	void startContext(vector<Ref<Object> >& match, Ref<Object>& result);
	void endContext(vector<Ref<Object> >& match, Ref<Object>& result);
	void startChunk(vector<Ref<Object> >& match, Ref<Object>& result);
	void endChunk(vector<Ref<Object> >& match, Ref<Object>& result);
	
	void addLocal(vector<Ref<Object> >& match, Ref<Object>& result) {}
	void addArg(vector<Ref<Object> >& match, Ref<Object>& result) {}
	void addConst(vector<Ref<Object> >& match, Ref<Object>& result) {}
	void pushNumber(vector<Ref<Object> >& match, Ref<Object>& result) {}
	
	void syntaxError(vector<Ref<Object> >& match, Ref<Object>& result);
	void unexpectedToken(vector<Ref<Object> >& match, Ref<Object>& result);

public:
	Parser() {}
	virtual ~Parser() {}
	
	void parse(Ref<Input> input, Ref<Output> output);
};

#endif /* end of include guard: PARSER_H_23VERTBYU8N */

