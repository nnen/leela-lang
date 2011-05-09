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
	Ref<Object> result;

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
	
	return result;
}

Ref<Object> Parser::parseVarDeclRest(Ref<Object> inherited, vector<Ref<Object> > siblings)
{
	vector<Ref<Object> > match;
	Ref<Object> result;

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
	
	return result;
}

Ref<Object> Parser::parseProgram(Ref<Object> inherited, vector<Ref<Object> > siblings)
{
	vector<Ref<Object> > match;
	Ref<Object> result;

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
	
	return result;
}

Ref<Object> Parser::parseStatement(Ref<Object> inherited, vector<Ref<Object> > siblings)
{
	vector<Ref<Object> > match;
	Ref<Object> result;

	switch (peek().type) {
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
		
	default:
		break;
		
	case Token::IF:
		match.push_back(accept(Token::IF).value);
		match.push_back(parseExpression(result, match));
		match.push_back(accept(Token::THEN).value);
		match.push_back(parseStatement(result, match));
		match.push_back(parseElseStmt(result, match));
		break;
		
	case Token::PRINT:
		match.push_back(accept(Token::PRINT).value);
		match.push_back(parseRValue(result, match));
		_writer.writeInstruction(AsmScanner::TOKEN_PRINT);
		break;
		
	case Token::RETURN:
		match.push_back(accept(Token::RETURN).value);
		match.push_back(parseRValue(result, match));
		_writer.writeInstruction(AsmScanner::TOKEN_RETURN);
		break;
		
	case Token::IDENT:
		match.push_back(parseAssignment(result, match));
		break;
		
	}
	
	return result;
}

Ref<Object> Parser::parseFactor(Ref<Object> inherited, vector<Ref<Object> > siblings)
{
	vector<Ref<Object> > match;
	Ref<Object> result;

	switch (peek().type) {
	default:
		unexpectedToken(inherited, siblings, match, result);
		break;
		
	case Token::NUMBER:
		match.push_back(accept(Token::NUMBER).value);
		pushNumber(inherited, siblings, match, result);
		break;
		
	case Token::IDENT:
		match.push_back(parsePrimaryExpr(result, match));
		break;
		
	case Token::LEFT_PAR:
		match.push_back(accept(Token::LEFT_PAR).value);
		match.push_back(parseExpression(result, match));
		match.push_back(accept(Token::RIGHT_PAR).value);
		break;
		
	case Token::STRING:
		match.push_back(accept(Token::STRING).value);
		pushString(inherited, siblings, match, result);
		break;
		
	}
	
	return result;
}

Ref<Object> Parser::parseConstDeclRest(Ref<Object> inherited, vector<Ref<Object> > siblings)
{
	vector<Ref<Object> > match;
	Ref<Object> result;

	switch (peek().type) {
	default:
		break;
		
	case Token::COMMA:
		match.push_back(accept(Token::COMMA).value);
		match.push_back(accept(Token::IDENT).value);
		match.push_back(accept(Token::EQUALS).value);
		match.push_back(accept(Token::NUMBER).value);
		addConst(inherited, siblings, match, result);
		match.push_back(parseConstDeclRest(result, match));
		break;
		
	}
	
	return result;
}

Ref<Object> Parser::parseTermRest(Ref<Object> inherited, vector<Ref<Object> > siblings)
{
	vector<Ref<Object> > match;
	Ref<Object> result;

	switch (peek().type) {
	case Token::SLASH:
		match.push_back(accept(Token::SLASH).value);
		match.push_back(parseFactor(result, match));
		_writer.writeInstruction(AsmScanner::TOKEN_DIV);
		match.push_back(parseTermRest(result, match));
		break;
		
	default:
		break;
		
	case Token::ASTERIX:
		match.push_back(accept(Token::ASTERIX).value);
		match.push_back(parseFactor(result, match));
		_writer.writeInstruction(AsmScanner::TOKEN_MUL);
		match.push_back(parseTermRest(result, match));
		break;
		
	}
	
	return result;
}

Ref<Object> Parser::parseLValue(Ref<Object> inherited, vector<Ref<Object> > siblings)
{
	vector<Ref<Object> > match;
	Ref<Object> result;

	switch (peek().type) {
	default:
		unexpectedToken(inherited, siblings, match, result);
		break;
		
	case Token::IDENT:
		match.push_back(accept(Token::IDENT).value);
		match.push_back(parseLValueRest(result, match));
		break;
		
	}
	
	return result;
}

Ref<Object> Parser::parseArgNameList(Ref<Object> inherited, vector<Ref<Object> > siblings)
{
	vector<Ref<Object> > match;
	Ref<Object> result;

	switch (peek().type) {
	default:
		break;
		
	case Token::IDENT:
		match.push_back(accept(Token::IDENT).value);
		addArg(inherited, siblings, match, result);
		match.push_back(parseArgNameListRest(result, match));
		break;
		
	}
	
	return result;
}

Ref<Object> Parser::parseArgNameListRest(Ref<Object> inherited, vector<Ref<Object> > siblings)
{
	vector<Ref<Object> > match;
	Ref<Object> result;

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
	
	return result;
}

Ref<Object> Parser::parseStatementRest(Ref<Object> inherited, vector<Ref<Object> > siblings)
{
	vector<Ref<Object> > match;
	Ref<Object> result;

	switch (peek().type) {
	default:
		break;
		
	case Token::SEMICOLON:
		match.push_back(accept(Token::SEMICOLON).value);
		match.push_back(parseStatement(result, match));
		match.push_back(parseStatementRest(result, match));
		break;
		
	}
	
	return result;
}

Ref<Object> Parser::parsePreamble(Ref<Object> inherited, vector<Ref<Object> > siblings)
{
	vector<Ref<Object> > match;
	Ref<Object> result;

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
	
	return result;
}

Ref<Object> Parser::parseElseStmt(Ref<Object> inherited, vector<Ref<Object> > siblings)
{
	vector<Ref<Object> > match;
	Ref<Object> result;

	switch (peek().type) {
	default:
		break;
		
	case Token::ELSE:
		match.push_back(accept(Token::ELSE).value);
		match.push_back(parseStatement(result, match));
		break;
		
	}
	
	return result;
}

Ref<Object> Parser::parseTerm(Ref<Object> inherited, vector<Ref<Object> > siblings)
{
	vector<Ref<Object> > match;
	Ref<Object> result;

	switch (peek().type) {
	default:
		unexpectedToken(inherited, siblings, match, result);
		break;
		
	case Token::IDENT:
	case Token::NUMBER:
	case Token::STRING:
	case Token::LEFT_PAR:
		match.push_back(parseFactor(result, match));
		match.push_back(parseTermRest(result, match));
		break;
		
	}
	
	return result;
}

Ref<Object> Parser::parseAssignIndex(Ref<Object> inherited, vector<Ref<Object> > siblings)
{
	vector<Ref<Object> > match;
	Ref<Object> result;

	switch (peek().type) {
	case Token::LEFT_BRA:
		_writer.writeInstruction(AsmScanner::TOKEN_LOAD);
		match.push_back(accept(Token::LEFT_BRA).value);
		match.push_back(parseRValue(result, match));
		match.push_back(accept(Token::RIGHT_BRA).value);
		match.push_back(parseAssignIndex(result, match));
		break;
		
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
		
	}
	
	return result;
}

Ref<Object> Parser::parseRValue(Ref<Object> inherited, vector<Ref<Object> > siblings)
{
	vector<Ref<Object> > match;
	Ref<Object> result;

	switch (peek().type) {
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
	case Token::MINUS:
	case Token::NUMBER:
	case Token::LEFT_PAR:
	case Token::STRING:
		match.push_back(parseExpression(result, match));
		break;
		
	default:
		unexpectedToken(inherited, siblings, match, result);
		break;
		
	}
	
	return result;
}

Ref<Object> Parser::parseParamListRest(Ref<Object> inherited, vector<Ref<Object> > siblings)
{
	vector<Ref<Object> > match;
	Ref<Object> result;

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
	
	return result;
}

Ref<Object> Parser::parseParamList(Ref<Object> inherited, vector<Ref<Object> > siblings)
{
	vector<Ref<Object> > match;
	Ref<Object> result;

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
	
	return result;
}

Ref<Object> Parser::parseExpression(Ref<Object> inherited, vector<Ref<Object> > siblings)
{
	vector<Ref<Object> > match;
	Ref<Object> result;

	switch (peek().type) {
	case Token::MINUS:
		match.push_back(accept(Token::MINUS).value);
		match.push_back(parseTerm(result, match));
		match.push_back(parseExpressionRest(result, match));
		break;
		
	default:
		unexpectedToken(inherited, siblings, match, result);
		break;
		
	case Token::IDENT:
	case Token::NUMBER:
	case Token::LEFT_PAR:
	case Token::STRING:
		match.push_back(parseTerm(result, match));
		match.push_back(parseExpressionRest(result, match));
		break;
		
	}
	
	return result;
}

Ref<Object> Parser::parseExpressionRest(Ref<Object> inherited, vector<Ref<Object> > siblings)
{
	vector<Ref<Object> > match;
	Ref<Object> result;

	switch (peek().type) {
	case Token::MINUS:
		match.push_back(accept(Token::MINUS).value);
		match.push_back(parseTerm(result, match));
		_writer.writeInstruction(AsmScanner::TOKEN_SUB);
		match.push_back(parseExpressionRest(result, match));
		break;
		
	case Token::PLUS:
		match.push_back(accept(Token::PLUS).value);
		match.push_back(parseTerm(result, match));
		_writer.writeInstruction(AsmScanner::TOKEN_ADD);
		match.push_back(parseExpressionRest(result, match));
		break;
		
	default:
		break;
		
	}
	
	return result;
}

Ref<Object> Parser::parsePostfixOp(Ref<Object> inherited, vector<Ref<Object> > siblings)
{
	vector<Ref<Object> > match;
	Ref<Object> result;

	switch (peek().type) {
	default:
		break;
		
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
		
	}
	
	return result;
}

Ref<Object> Parser::parseAssignVar(Ref<Object> inherited, vector<Ref<Object> > siblings)
{
	vector<Ref<Object> > match;
	Ref<Object> result;

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
		
	case Token::LEFT_BRA:
		getValueForLookup(inherited, siblings, match, result);
		match.push_back(accept(Token::LEFT_BRA).value);
		match.push_back(parseRValue(result, match));
		match.push_back(accept(Token::RIGHT_BRA).value);
		match.push_back(parseAssignIndex(result, match));
		break;
		
	}
	
	return result;
}

Ref<Object> Parser::parseFunctionPreamble(Ref<Object> inherited, vector<Ref<Object> > siblings)
{
	vector<Ref<Object> > match;
	Ref<Object> result;

	switch (peek().type) {
	default:
		break;
		
	case Token::VAR:
		match.push_back(parseVarDecl(result, match));
		match.push_back(parseFunctionPreamble(result, match));
		break;
		
	}
	
	return result;
}

Ref<Object> Parser::parseLValueRest(Ref<Object> inherited, vector<Ref<Object> > siblings)
{
	vector<Ref<Object> > match;
	Ref<Object> result;

	switch (peek().type) {
	default:
		break;
		
	}
	
	return result;
}

Ref<Object> Parser::parseAssignment(Ref<Object> inherited, vector<Ref<Object> > siblings)
{
	vector<Ref<Object> > match;
	Ref<Object> result;

	switch (peek().type) {
	default:
		unexpectedToken(inherited, siblings, match, result);
		break;
		
	case Token::IDENT:
		match.push_back(accept(Token::IDENT).value);
		match.push_back(parseAssignVar(result, match));
		break;
		
	}
	
	return result;
}

Ref<Object> Parser::parsePrimaryExpr(Ref<Object> inherited, vector<Ref<Object> > siblings)
{
	vector<Ref<Object> > match;
	Ref<Object> result;

	switch (peek().type) {
	default:
		unexpectedToken(inherited, siblings, match, result);
		break;
		
	case Token::IDENT:
		match.push_back(accept(Token::IDENT).value);
		getSymbolValue(inherited, siblings, match, result);
		match.push_back(parsePostfixOp(result, match));
		break;
		
	}
	
	return result;
}

Ref<Object> Parser::parseConstDecl(Ref<Object> inherited, vector<Ref<Object> > siblings)
{
	vector<Ref<Object> > match;
	Ref<Object> result;

	switch (peek().type) {
	default:
		unexpectedToken(inherited, siblings, match, result);
		break;
		
	case Token::CONST:
		match.push_back(accept(Token::CONST).value);
		match.push_back(accept(Token::IDENT).value);
		match.push_back(accept(Token::EQUALS).value);
		match.push_back(accept(Token::NUMBER).value);
		addConst(inherited, siblings, match, result);
		match.push_back(parseConstDeclRest(result, match));
		match.push_back(accept(Token::SEMICOLON).value);
		break;
		
	}
	
	return result;
}

Ref<Object> Parser::parseCompoundStmt(Ref<Object> inherited, vector<Ref<Object> > siblings)
{
	vector<Ref<Object> > match;
	Ref<Object> result;

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
	
	return result;
}

