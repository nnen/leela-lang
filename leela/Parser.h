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
#include "StringTable.h"
#include "Input.h"
#include "Output.h"
#include "Exception.h"
#include "AsmWriter.h"

using namespace std;

class Parser;

class SyntaxError : public Exception {
protected:
	Token  _token;

public:
	SyntaxError(const Parser& parser) throw();
	SyntaxError(string message) throw();
	SyntaxError(Token token, string message) throw();
	virtual ~SyntaxError() throw() {}

	virtual void print(ostream& output);
};

class Parser {
friend class SyntaxError;
public:
	typedef vector<Ref<Object> > Match;
	
	#include "Parser.rules.h"

private:
	Ref<BufferedInput> _input;
	Ref<Output>        _output;
	Ref<Lexer>         _lexer;
	
	int                _pass;
	Ref<ContextTable>  _contexts;
	Ref<StringTable>   _strings;
	AsmWriter          _writer;
	
	vector<string>     _nonterminals;
	
	Token peek() { return _lexer->peek(); }
	Token accept() { return _lexer->get(); }
	Token accept(Token::Type type);

	/*
	void startContext(vector<Ref<Object> >& match, Ref<Object>& result);
	void endContext(vector<Ref<Object> >& match, Ref<Object>& result);
	void startChunk(vector<Ref<Object> >& match, Ref<Object>& result);
	void endChunk(vector<Ref<Object> >& match, Ref<Object>& result);
	void startFunction(vector<Ref<Object> >& match, Ref<Object>& result);
	void endFunction(vector<Ref<Object> >& match, Ref<Object>& result);
	
	void addLocal(vector<Ref<Object> >& match, Ref<Object>& result);
	void allocLocals(vector<Ref<Object> >& match, Ref<Object>& result);
	void addArg(vector<Ref<Object> >& match, Ref<Object>& result);
	void addConst(vector<Ref<Object> >& match, Ref<Object>& result) {}
	void pushNumber(vector<Ref<Object> >& match, Ref<Object>& result);
	void pushString(Ref<Object> inherited, Match siblings, Match& match, Ref<Object>& result);
	void getSymbolValue(vector<Ref<Object> >& match, Ref<Object>& result);
	*/

	//void syntaxError(vector<Ref<Object> >& match, Ref<Object>& result);
	void unexpectedToken(Ref<Object> inherited,
	                     Match siblings,
	                     Match& match,
					 Ref<Object>& result);

	void onUndefinedSymbol(string name);

public:
	Parser() {}
	virtual ~Parser() {}
	
	bool parse(Ref<Input> input, Ref<Output> output);
};

#endif /* end of include guard: PARSER_H_23VERTBYU8N */

