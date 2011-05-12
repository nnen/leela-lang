/**
 * \file   Parser.rules.tpl.cpp
 * \author Jan Milik <milikjan@fit.cvut.cz>
 * \date   2011-04-28
 *
 * \brief  
 */

#include "Parser.h"


Ref<Object> Parser::parseVarDecl(Ref<Object> inherited, vector<Ref<Object> > siblings)
{
	vector<Ref<Object> > match;
	Ref<Object>          result;
	
	_nonterminals.push_back("VarDecl");

	switch (peek().type) {
	default:
		unexpectedToken(inherited, siblings, match, result);
		break;
		
	case Token::VAR:
		match.push_back(accept(Token::VAR).value);
		match.push_back(accept(Token::IDENT).value);
		addLocal(inherited, siblings, match, result);
		match.push_back(parseVarDeclRest(result, match));
		match.push_back(accept(Token::SEMICOLON).value);
		break;
		
	}

	_nonterminals.pop_back();
	
	return result;
}

Ref<Object> Parser::parseSum(Ref<Object> inherited, vector<Ref<Object> > siblings)
{
	vector<Ref<Object> > match;
	Ref<Object>          result;
	
	_nonterminals.push_back("Sum");

	switch (peek().type) {
	case Token::MINUS:
		match.push_back(accept(Token::MINUS).value);
		match.push_back(parseTerm(result, match));
		match.push_back(parseSumRest(result, match));
		break;
		
	case Token::IDENT:
	case Token::NUMBER:
	case Token::STRING:
	case Token::LEFT_PAR:
		match.push_back(parseTerm(result, match));
		match.push_back(parseSumRest(result, match));
		break;
		
	default:
		unexpectedToken(inherited, siblings, match, result);
		break;
		
	}

	_nonterminals.pop_back();
	
	return result;
}

Ref<Object> Parser::parseConstExpr(Ref<Object> inherited, vector<Ref<Object> > siblings)
{
	vector<Ref<Object> > match;
	Ref<Object>          result;
	
	_nonterminals.push_back("ConstExpr");

	switch (peek().type) {
	case Token::STRING:
		match.push_back(accept(Token::STRING).value);
		pushString(inherited, siblings, match, result);
		break;
		
	case Token::NUMBER:
		match.push_back(accept(Token::NUMBER).value);
		pushNumber(inherited, siblings, match, result);
		break;
		
	default:
		unexpectedToken(inherited, siblings, match, result);
		break;
		
	}

	_nonterminals.pop_back();
	
	return result;
}

Ref<Object> Parser::parseProgram(Ref<Object> inherited, vector<Ref<Object> > siblings)
{
	vector<Ref<Object> > match;
	Ref<Object>          result;
	
	_nonterminals.push_back("Program");

	switch (peek().type) {
	default:
		unexpectedToken(inherited, siblings, match, result);
		break;
		
	case Token::VAR:
	case Token::BEGIN:
	case Token::CONST:
		startContext(inherited, siblings, match, result);
		match.push_back(parsePreamble(result, match));
		allocLocals(inherited, siblings, match, result);
		match.push_back(parseCompoundStmt(result, match));
		_writer.writeInstruction(AsmScanner::TOKEN_STOP);
		writeChunks(inherited, siblings, match, result);
		writeStrings(inherited, siblings, match, result);
		break;
		
	}

	_nonterminals.pop_back();
	
	return result;
}

Ref<Object> Parser::parseStatement(Ref<Object> inherited, vector<Ref<Object> > siblings)
{
	vector<Ref<Object> > match;
	Ref<Object>          result;
	
	_nonterminals.push_back("Statement");

	switch (peek().type) {
	case Token::IDENT:
		match.push_back(parseAssignment(result, match));
		break;
		
	default:
		break;
		
	case Token::WHILE:
		match.push_back(accept(Token::WHILE).value);
		startWhile(inherited, siblings, match, result);
		match.push_back(parseExpression(result, match));
		_writer.writeInstruction(AsmScanner::TOKEN_NOT);
		whileJump(inherited, siblings, match, result);
		match.push_back(accept(Token::DO).value);
		match.push_back(parseStatement(result, match));
		endWhile(inherited, siblings, match, result);
		break;
		
	case Token::BEGIN:
		match.push_back(parseCompoundStmt(result, match));
		break;
		
	case Token::IF:
		match.push_back(accept(Token::IF).value);
		match.push_back(parseExpression(result, match));
		_writer.writeInstruction(AsmScanner::TOKEN_NOT);
		ifJump(inherited, siblings, match, result);
		match.push_back(accept(Token::THEN).value);
		match.push_back(parseStatement(result, match));
		match.push_back(parseElseStmt(result, match));
		break;
		
	case Token::RETURN:
		match.push_back(accept(Token::RETURN).value);
		match.push_back(parseRValue(result, match));
		_writer.writeInstruction(AsmScanner::TOKEN_RETURN);
		break;
		
	case Token::PRINT:
		match.push_back(accept(Token::PRINT).value);
		match.push_back(parseRValue(result, match));
		_writer.writeInstruction(AsmScanner::TOKEN_PRINT);
		break;
		
	}

	_nonterminals.pop_back();
	
	return result;
}

Ref<Object> Parser::parseFactor(Ref<Object> inherited, vector<Ref<Object> > siblings)
{
	vector<Ref<Object> > match;
	Ref<Object>          result;
	
	_nonterminals.push_back("Factor");

	switch (peek().type) {
	default:
		unexpectedToken(inherited, siblings, match, result);
		break;
		
	case Token::IDENT:
	case Token::NUMBER:
	case Token::STRING:
	case Token::LEFT_PAR:
		match.push_back(parsePrimaryExpr(result, match));
		match.push_back(parsePostfixOp(result, match));
		break;
		
	}

	_nonterminals.pop_back();
	
	return result;
}

Ref<Object> Parser::parseConstDeclRest(Ref<Object> inherited, vector<Ref<Object> > siblings)
{
	vector<Ref<Object> > match;
	Ref<Object>          result;
	
	_nonterminals.push_back("ConstDeclRest");

	switch (peek().type) {
	default:
		break;
		
	case Token::COMMA:
		match.push_back(accept(Token::COMMA).value);
		match.push_back(accept(Token::IDENT).value);
		match.push_back(accept(Token::EQUALS).value);
		match.push_back(parseConstExpr(result, match));
		addConst(inherited, siblings, match, result);
		match.push_back(parseConstDeclRest(result, match));
		break;
		
	}

	_nonterminals.pop_back();
	
	return result;
}

Ref<Object> Parser::parseTermRest(Ref<Object> inherited, vector<Ref<Object> > siblings)
{
	vector<Ref<Object> > match;
	Ref<Object>          result;
	
	_nonterminals.push_back("TermRest");

	switch (peek().type) {
	case Token::SLASH:
		match.push_back(accept(Token::SLASH).value);
		match.push_back(parseFactor(result, match));
		_writer.writeInstruction(AsmScanner::TOKEN_DIV);
		match.push_back(parseTermRest(result, match));
		break;
		
	case Token::ASTERIX:
		match.push_back(accept(Token::ASTERIX).value);
		match.push_back(parseFactor(result, match));
		_writer.writeInstruction(AsmScanner::TOKEN_MUL);
		match.push_back(parseTermRest(result, match));
		break;
		
	default:
		break;
		
	}

	_nonterminals.pop_back();
	
	return result;
}

Ref<Object> Parser::parseAssignment(Ref<Object> inherited, vector<Ref<Object> > siblings)
{
	vector<Ref<Object> > match;
	Ref<Object>          result;
	
	_nonterminals.push_back("Assignment");

	switch (peek().type) {
	default:
		unexpectedToken(inherited, siblings, match, result);
		break;
		
	case Token::IDENT:
		match.push_back(accept(Token::IDENT).value);
		match.push_back(parseAssignVar(result, match));
		break;
		
	}

	_nonterminals.pop_back();
	
	return result;
}

Ref<Object> Parser::parseArgNameList(Ref<Object> inherited, vector<Ref<Object> > siblings)
{
	vector<Ref<Object> > match;
	Ref<Object>          result;
	
	_nonterminals.push_back("ArgNameList");

	switch (peek().type) {
	default:
		break;
		
	case Token::IDENT:
		match.push_back(accept(Token::IDENT).value);
		addArg(inherited, siblings, match, result);
		match.push_back(parseArgNameListRest(result, match));
		break;
		
	}

	_nonterminals.pop_back();
	
	return result;
}

Ref<Object> Parser::parseArgNameListRest(Ref<Object> inherited, vector<Ref<Object> > siblings)
{
	vector<Ref<Object> > match;
	Ref<Object>          result;
	
	_nonterminals.push_back("ArgNameListRest");

	switch (peek().type) {
	default:
		break;
		
	case Token::COMMA:
		match.push_back(accept(Token::COMMA).value);
		match.push_back(accept(Token::IDENT).value);
		addArg(inherited, siblings, match, result);
		match.push_back(parseArgNameListRest(result, match));
		break;
		
	}

	_nonterminals.pop_back();
	
	return result;
}

Ref<Object> Parser::parseStatementRest(Ref<Object> inherited, vector<Ref<Object> > siblings)
{
	vector<Ref<Object> > match;
	Ref<Object>          result;
	
	_nonterminals.push_back("StatementRest");

	switch (peek().type) {
	default:
		break;
		
	case Token::SEMICOLON:
		match.push_back(accept(Token::SEMICOLON).value);
		match.push_back(parseStatement(result, match));
		match.push_back(parseStatementRest(result, match));
		break;
		
	}

	_nonterminals.pop_back();
	
	return result;
}

Ref<Object> Parser::parsePreamble(Ref<Object> inherited, vector<Ref<Object> > siblings)
{
	vector<Ref<Object> > match;
	Ref<Object>          result;
	
	_nonterminals.push_back("Preamble");

	switch (peek().type) {
	case Token::VAR:
		match.push_back(parseVarDecl(result, match));
		match.push_back(parsePreamble(result, match));
		break;
		
	default:
		break;
		
	case Token::CONST:
		match.push_back(parseConstDecl(result, match));
		match.push_back(parsePreamble(result, match));
		break;
		
	}

	_nonterminals.pop_back();
	
	return result;
}

Ref<Object> Parser::parseVarDeclRest(Ref<Object> inherited, vector<Ref<Object> > siblings)
{
	vector<Ref<Object> > match;
	Ref<Object>          result;
	
	_nonterminals.push_back("VarDeclRest");

	switch (peek().type) {
	default:
		break;
		
	case Token::COMMA:
		match.push_back(accept(Token::COMMA).value);
		match.push_back(accept(Token::IDENT).value);
		addLocal(inherited, siblings, match, result);
		match.push_back(parseVarDeclRest(result, match));
		break;
		
	}

	_nonterminals.pop_back();
	
	return result;
}

Ref<Object> Parser::parseTerm(Ref<Object> inherited, vector<Ref<Object> > siblings)
{
	vector<Ref<Object> > match;
	Ref<Object>          result;
	
	_nonterminals.push_back("Term");

	switch (peek().type) {
	default:
		unexpectedToken(inherited, siblings, match, result);
		break;
		
	case Token::IDENT:
	case Token::NUMBER:
	case Token::LEFT_PAR:
	case Token::STRING:
		match.push_back(parseFactor(result, match));
		match.push_back(parseTermRest(result, match));
		break;
		
	}

	_nonterminals.pop_back();
	
	return result;
}

Ref<Object> Parser::parseAssignIndex(Ref<Object> inherited, vector<Ref<Object> > siblings)
{
	vector<Ref<Object> > match;
	Ref<Object>          result;
	
	_nonterminals.push_back("AssignIndex");

	switch (peek().type) {
	case Token::PERIOD:
		_writer.writeInstruction(AsmScanner::TOKEN_LOAD);
		match.push_back(accept(Token::PERIOD).value);
		match.push_back(accept(Token::IDENT).value);
		identToString(inherited, siblings, match, result);
		match.push_back(parseAssignIndex(result, match));
		break;
		
	default:
		unexpectedToken(inherited, siblings, match, result);
		break;
		
	case Token::ASSIGN:
		match.push_back(accept(Token::ASSIGN).value);
		match.push_back(parseRValue(result, match));
		_writer.writeInstruction(AsmScanner::TOKEN_STORE);
		break;
		
	case Token::LEFT_BRA:
		_writer.writeInstruction(AsmScanner::TOKEN_LOAD);
		match.push_back(accept(Token::LEFT_BRA).value);
		match.push_back(parseRValue(result, match));
		match.push_back(accept(Token::RIGHT_BRA).value);
		match.push_back(parseAssignIndex(result, match));
		break;
		
	}

	_nonterminals.pop_back();
	
	return result;
}

Ref<Object> Parser::parseRValue(Ref<Object> inherited, vector<Ref<Object> > siblings)
{
	vector<Ref<Object> > match;
	Ref<Object>          result;
	
	_nonterminals.push_back("RValue");

	switch (peek().type) {
	default:
		unexpectedToken(inherited, siblings, match, result);
		break;
		
	case Token::FUNCTION:
		match.push_back(accept(Token::FUNCTION).value);
		startFunction(inherited, siblings, match, result);
		match.push_back(accept(Token::LEFT_PAR).value);
		match.push_back(parseArgNameList(result, match));
		match.push_back(accept(Token::RIGHT_PAR).value);
		match.push_back(accept(Token::COLON).value);
		match.push_back(parseFunctionPreamble(result, match));
		allocLocals(inherited, siblings, match, result);
		match.push_back(parseStatement(result, match));
		_writer.writeInstruction(AsmScanner::TOKEN_PUSH);
		_writer.writeInstruction(AsmScanner::TOKEN_RETURN);
		endFunction(inherited, siblings, match, result);
		break;
		
	case Token::LAMBDA:
		match.push_back(accept(Token::LAMBDA).value);
		startFunction(inherited, siblings, match, result);
		match.push_back(accept(Token::LEFT_PAR).value);
		match.push_back(parseArgNameList(result, match));
		match.push_back(accept(Token::RIGHT_PAR).value);
		match.push_back(accept(Token::COLON).value);
		match.push_back(parseRValue(result, match));
		_writer.writeInstruction(AsmScanner::TOKEN_RETURN);
		endFunction(inherited, siblings, match, result);
		break;
		
	case Token::IDENT:
	case Token::NUMBER:
	case Token::MINUS:
	case Token::STRING:
	case Token::LEFT_PAR:
		match.push_back(parseExpression(result, match));
		break;
		
	}

	_nonterminals.pop_back();
	
	return result;
}

Ref<Object> Parser::parseParamListRest(Ref<Object> inherited, vector<Ref<Object> > siblings)
{
	vector<Ref<Object> > match;
	Ref<Object>          result;
	
	_nonterminals.push_back("ParamListRest");

	switch (peek().type) {
	default:
		returnZero(inherited, siblings, match, result);
		break;
		
	case Token::COMMA:
		match.push_back(accept(Token::COMMA).value);
		match.push_back(parseRValue(result, match));
		match.push_back(parseParamListRest(result, match));
		returnPlusOne(inherited, siblings, match, result);
		break;
		
	}

	_nonterminals.pop_back();
	
	return result;
}

Ref<Object> Parser::parseParamList(Ref<Object> inherited, vector<Ref<Object> > siblings)
{
	vector<Ref<Object> > match;
	Ref<Object>          result;
	
	_nonterminals.push_back("ParamList");

	switch (peek().type) {
	default:
		returnZero(inherited, siblings, match, result);
		break;
		
	case Token::FUNCTION:
	case Token::IDENT:
	case Token::STRING:
	case Token::NUMBER:
	case Token::LEFT_PAR:
	case Token::MINUS:
	case Token::LAMBDA:
		match.push_back(parseRValue(result, match));
		match.push_back(parseParamListRest(result, match));
		returnPlusOne(inherited, siblings, match, result);
		break;
		
	}

	_nonterminals.pop_back();
	
	return result;
}

Ref<Object> Parser::parseExpression(Ref<Object> inherited, vector<Ref<Object> > siblings)
{
	vector<Ref<Object> > match;
	Ref<Object>          result;
	
	_nonterminals.push_back("Expression");

	switch (peek().type) {
	default:
		unexpectedToken(inherited, siblings, match, result);
		break;
		
	case Token::LEFT_PAR:
	case Token::IDENT:
	case Token::NUMBER:
	case Token::STRING:
	case Token::MINUS:
		match.push_back(parseSum(result, match));
		match.push_back(parseExpressionRest(result, match));
		break;
		
	}

	_nonterminals.pop_back();
	
	return result;
}

Ref<Object> Parser::parseExpressionRest(Ref<Object> inherited, vector<Ref<Object> > siblings)
{
	vector<Ref<Object> > match;
	Ref<Object>          result;
	
	_nonterminals.push_back("ExpressionRest");

	switch (peek().type) {
	case Token::NOT_EQUALS:
		match.push_back(accept(Token::NOT_EQUALS).value);
		match.push_back(parseSum(result, match));
		_writer.writeInstruction(AsmScanner::TOKEN_EQUALS);
		_writer.writeInstruction(AsmScanner::TOKEN_NOT);
		match.push_back(parseExpressionRest(result, match));
		break;
		
	case Token::LESS_OR_EQ:
		match.push_back(accept(Token::LESS_OR_EQ).value);
		match.push_back(parseSum(result, match));
		_writer.writeInstruction(AsmScanner::TOKEN_LESS_OR_EQ);
		match.push_back(parseExpressionRest(result, match));
		break;
		
	case Token::RIGHT_ANGLE:
		match.push_back(accept(Token::RIGHT_ANGLE).value);
		match.push_back(parseSum(result, match));
		_writer.writeInstruction(AsmScanner::TOKEN_GREATER);
		match.push_back(parseExpressionRest(result, match));
		break;
		
	default:
		break;
		
	case Token::LEFT_ANGLE:
		match.push_back(accept(Token::LEFT_ANGLE).value);
		match.push_back(parseSum(result, match));
		_writer.writeInstruction(AsmScanner::TOKEN_LESS);
		match.push_back(parseExpressionRest(result, match));
		break;
		
	case Token::EQUALS:
		match.push_back(accept(Token::EQUALS).value);
		match.push_back(parseSum(result, match));
		_writer.writeInstruction(AsmScanner::TOKEN_EQUALS);
		match.push_back(parseExpressionRest(result, match));
		break;
		
	case Token::GREATER_OR_EQ:
		match.push_back(accept(Token::GREATER_OR_EQ).value);
		match.push_back(parseSum(result, match));
		_writer.writeInstruction(AsmScanner::TOKEN_GREATER_OR_EQ);
		match.push_back(parseExpressionRest(result, match));
		break;
		
	}

	_nonterminals.pop_back();
	
	return result;
}

Ref<Object> Parser::parsePostfixOp(Ref<Object> inherited, vector<Ref<Object> > siblings)
{
	vector<Ref<Object> > match;
	Ref<Object>          result;
	
	_nonterminals.push_back("PostfixOp");

	switch (peek().type) {
	case Token::LEFT_PAR:
		match.push_back(accept(Token::LEFT_PAR).value);
		match.push_back(parseParamList(result, match));
		writeCall(inherited, siblings, match, result);
		match.push_back(accept(Token::RIGHT_PAR).value);
		match.push_back(parsePostfixOp(result, match));
		break;
		
	case Token::LEFT_BRA:
		match.push_back(accept(Token::LEFT_BRA).value);
		match.push_back(parseExpression(result, match));
		_writer.writeInstruction(AsmScanner::TOKEN_LOAD);
		match.push_back(accept(Token::RIGHT_BRA).value);
		match.push_back(parsePostfixOp(result, match));
		break;
		
	default:
		break;
		
	}

	_nonterminals.pop_back();
	
	return result;
}

Ref<Object> Parser::parseAssignVar(Ref<Object> inherited, vector<Ref<Object> > siblings)
{
	vector<Ref<Object> > match;
	Ref<Object>          result;
	
	_nonterminals.push_back("AssignVar");

	switch (peek().type) {
	default:
		unexpectedToken(inherited, siblings, match, result);
		break;
		
	case Token::PERIOD:
		getValueForLookup(inherited, siblings, match, result);
		match.push_back(accept(Token::PERIOD).value);
		match.push_back(accept(Token::IDENT).value);
		identToString(inherited, siblings, match, result);
		match.push_back(parseAssignIndex(result, match));
		break;
		
	case Token::ASSIGN:
		match.push_back(accept(Token::ASSIGN).value);
		match.push_back(parseRValue(result, match));
		assignVar(inherited, siblings, match, result);
		break;
		
	case Token::LEFT_PAR:
		getValueForLookup(inherited, siblings, match, result);
		match.push_back(accept(Token::LEFT_PAR).value);
		match.push_back(parseParamList(result, match));
		writeCall(inherited, siblings, match, result);
		match.push_back(accept(Token::RIGHT_PAR).value);
		break;
		
	case Token::LEFT_BRA:
		getValueForLookup(inherited, siblings, match, result);
		match.push_back(accept(Token::LEFT_BRA).value);
		match.push_back(parseRValue(result, match));
		match.push_back(accept(Token::RIGHT_BRA).value);
		match.push_back(parseAssignIndex(result, match));
		break;
		
	}

	_nonterminals.pop_back();
	
	return result;
}

Ref<Object> Parser::parseSumRest(Ref<Object> inherited, vector<Ref<Object> > siblings)
{
	vector<Ref<Object> > match;
	Ref<Object>          result;
	
	_nonterminals.push_back("SumRest");

	switch (peek().type) {
	case Token::MINUS:
		match.push_back(accept(Token::MINUS).value);
		match.push_back(parseTerm(result, match));
		_writer.writeInstruction(AsmScanner::TOKEN_SUB);
		match.push_back(parseSumRest(result, match));
		break;
		
	case Token::PLUS:
		match.push_back(accept(Token::PLUS).value);
		match.push_back(parseTerm(result, match));
		_writer.writeInstruction(AsmScanner::TOKEN_ADD);
		match.push_back(parseSumRest(result, match));
		break;
		
	default:
		break;
		
	}

	_nonterminals.pop_back();
	
	return result;
}

Ref<Object> Parser::parseFunctionPreamble(Ref<Object> inherited, vector<Ref<Object> > siblings)
{
	vector<Ref<Object> > match;
	Ref<Object>          result;
	
	_nonterminals.push_back("FunctionPreamble");

	switch (peek().type) {
	default:
		break;
		
	case Token::VAR:
		match.push_back(parseVarDecl(result, match));
		match.push_back(parseFunctionPreamble(result, match));
		break;
		
	}

	_nonterminals.pop_back();
	
	return result;
}

Ref<Object> Parser::parseElseStmt(Ref<Object> inherited, vector<Ref<Object> > siblings)
{
	vector<Ref<Object> > match;
	Ref<Object>          result;
	
	_nonterminals.push_back("ElseStmt");

	switch (peek().type) {
	default:
		ifEnd(inherited, siblings, match, result);
		break;
		
	case Token::ELSE:
		match.push_back(accept(Token::ELSE).value);
		elseJump(inherited, siblings, match, result);
		match.push_back(parseStatement(result, match));
		ifEnd(inherited, siblings, match, result);
		break;
		
	}

	_nonterminals.pop_back();
	
	return result;
}

Ref<Object> Parser::parsePrimaryExpr(Ref<Object> inherited, vector<Ref<Object> > siblings)
{
	vector<Ref<Object> > match;
	Ref<Object>          result;
	
	_nonterminals.push_back("PrimaryExpr");

	switch (peek().type) {
	case Token::NUMBER:
	case Token::STRING:
		match.push_back(parseConstExpr(result, match));
		break;
		
	case Token::LEFT_PAR:
		match.push_back(accept(Token::LEFT_PAR).value);
		match.push_back(parseRValue(result, match));
		match.push_back(accept(Token::RIGHT_PAR).value);
		break;
		
	case Token::IDENT:
		match.push_back(accept(Token::IDENT).value);
		pushSymbolValue(inherited, siblings, match, result);
		break;
		
	default:
		unexpectedToken(inherited, siblings, match, result);
		break;
		
	}

	_nonterminals.pop_back();
	
	return result;
}

Ref<Object> Parser::parseConstDecl(Ref<Object> inherited, vector<Ref<Object> > siblings)
{
	vector<Ref<Object> > match;
	Ref<Object>          result;
	
	_nonterminals.push_back("ConstDecl");

	switch (peek().type) {
	default:
		unexpectedToken(inherited, siblings, match, result);
		break;
		
	case Token::CONST:
		match.push_back(accept(Token::CONST).value);
		match.push_back(accept(Token::IDENT).value);
		match.push_back(accept(Token::EQUALS).value);
		match.push_back(parseConstExpr(result, match));
		addConst(inherited, siblings, match, result);
		match.push_back(parseConstDeclRest(result, match));
		match.push_back(accept(Token::SEMICOLON).value);
		break;
		
	}

	_nonterminals.pop_back();
	
	return result;
}

Ref<Object> Parser::parseCompoundStmt(Ref<Object> inherited, vector<Ref<Object> > siblings)
{
	vector<Ref<Object> > match;
	Ref<Object>          result;
	
	_nonterminals.push_back("CompoundStmt");

	switch (peek().type) {
	default:
		unexpectedToken(inherited, siblings, match, result);
		break;
		
	case Token::BEGIN:
		match.push_back(accept(Token::BEGIN).value);
		match.push_back(parseStatement(result, match));
		match.push_back(parseStatementRest(result, match));
		match.push_back(accept(Token::END).value);
		break;
		
	}

	_nonterminals.pop_back();
	
	return result;
}

