/**
 * \file   Parser.rules.tpl.cpp
 * \author Jan Milik <milikjan@fit.cvut.cz>
 * \date   2011-04-28
 *
 * \brief  
 */

#include "Parser.h"


Ref<Object> Parser::parseIndex()
{
	vector<Ref<Object> > match;
	Ref<Object> result;

	switch (peek().type) {
	default:
		unexpectedToken(match, result);
		break;
		
	case Token::LEFT_BRA:
		match.push_back(accept(Token::LEFT_BRA).value);
		match.push_back(parseExpression());
		match.push_back(accept(Token::RIGHT_BRA).value);
		break;
		
	}
	
	return result;
}

Ref<Object> Parser::parseVarDeclRest()
{
	vector<Ref<Object> > match;
	Ref<Object> result;

	switch (peek().type) {
	default:
		break;
		
	case Token::COMMA:
		match.push_back(accept(Token::COMMA).value);
		match.push_back(accept(Token::IDENT).value);
		addLocal(match, result);
		match.push_back(parseVarDeclRest());
		break;
		
	}
	
	return result;
}

Ref<Object> Parser::parseProgram()
{
	vector<Ref<Object> > match;
	Ref<Object> result;

	switch (peek().type) {
	default:
		unexpectedToken(match, result);
		break;
		
	case Token::VAR:
	case Token::BEGIN:
	case Token::CONST:
		startContext(match, result);
		match.push_back(parsePreamble());
		allocLocals(match, result);
		match.push_back(parseCompoundStmt());
		_writer.writeInstruction(AsmScanner::TOKEN_STOP);
		break;
		
	}
	
	return result;
}

Ref<Object> Parser::parseStatement()
{
	vector<Ref<Object> > match;
	Ref<Object> result;

	switch (peek().type) {
	case Token::BEGIN:
		match.push_back(parseCompoundStmt());
		break;
		
	case Token::IDENT:
		match.push_back(parseLValue());
		match.push_back(accept(Token::ASSIGN).value);
		match.push_back(parseRValue());
		break;
		
	default:
		break;
		
	case Token::RETURN:
		match.push_back(accept(Token::RETURN).value);
		match.push_back(parseRValue());
		_writer.writeInstruction(AsmScanner::TOKEN_RETURN);
		break;
		
	case Token::WHILE:
		match.push_back(accept(Token::WHILE).value);
		match.push_back(parseExpression());
		match.push_back(accept(Token::DO).value);
		match.push_back(parseStatement());
		break;
		
	case Token::IF:
		match.push_back(accept(Token::IF).value);
		match.push_back(parseExpression());
		match.push_back(accept(Token::THEN).value);
		match.push_back(parseStatement());
		match.push_back(parseElseStmt());
		break;
		
	}
	
	return result;
}

Ref<Object> Parser::parseFactor()
{
	vector<Ref<Object> > match;
	Ref<Object> result;

	switch (peek().type) {
	case Token::NUMBER:
		match.push_back(accept(Token::NUMBER).value);
		pushNumber(match, result);
		break;
		
	case Token::IDENT:
		match.push_back(parsePrimaryExpr());
		break;
		
	default:
		unexpectedToken(match, result);
		break;
		
	case Token::LEFT_PAR:
		match.push_back(accept(Token::LEFT_PAR).value);
		match.push_back(parseExpression());
		match.push_back(accept(Token::RIGHT_PAR).value);
		break;
		
	}
	
	return result;
}

Ref<Object> Parser::parseConstDeclRest()
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
		addConst(match, result);
		match.push_back(parseConstDeclRest());
		break;
		
	}
	
	return result;
}

Ref<Object> Parser::parseTermRest()
{
	vector<Ref<Object> > match;
	Ref<Object> result;

	switch (peek().type) {
	case Token::SLASH:
		match.push_back(accept(Token::SLASH).value);
		match.push_back(parseFactor());
		_writer.writeInstruction(AsmScanner::TOKEN_DIV);
		match.push_back(parseTermRest());
		break;
		
	case Token::ASTERIX:
		match.push_back(accept(Token::ASTERIX).value);
		match.push_back(parseFactor());
		_writer.writeInstruction(AsmScanner::TOKEN_MUL);
		match.push_back(parseTermRest());
		break;
		
	default:
		break;
		
	}
	
	return result;
}

Ref<Object> Parser::parseLValue()
{
	vector<Ref<Object> > match;
	Ref<Object> result;

	switch (peek().type) {
	default:
		unexpectedToken(match, result);
		break;
		
	case Token::IDENT:
		match.push_back(accept(Token::IDENT).value);
		match.push_back(parseLValueRest());
		break;
		
	}
	
	return result;
}

Ref<Object> Parser::parseArgNameList()
{
	vector<Ref<Object> > match;
	Ref<Object> result;

	switch (peek().type) {
	default:
		break;
		
	case Token::IDENT:
		match.push_back(accept(Token::IDENT).value);
		addArg(match, result);
		match.push_back(parseArgNameListRest());
		break;
		
	}
	
	return result;
}

Ref<Object> Parser::parseArgNameListRest()
{
	vector<Ref<Object> > match;
	Ref<Object> result;

	switch (peek().type) {
	default:
		break;
		
	case Token::COMMA:
		match.push_back(accept(Token::COMMA).value);
		match.push_back(accept(Token::IDENT).value);
		addArg(match, result);
		match.push_back(parseArgNameListRest());
		break;
		
	}
	
	return result;
}

Ref<Object> Parser::parseStatementRest()
{
	vector<Ref<Object> > match;
	Ref<Object> result;

	switch (peek().type) {
	default:
		break;
		
	case Token::SEMICOLON:
		match.push_back(accept(Token::SEMICOLON).value);
		match.push_back(parseStatement());
		match.push_back(parseStatementRest());
		break;
		
	}
	
	return result;
}

Ref<Object> Parser::parsePreamble()
{
	vector<Ref<Object> > match;
	Ref<Object> result;

	switch (peek().type) {
	case Token::VAR:
		match.push_back(parseVarDecl());
		match.push_back(parsePreamble());
		break;
		
	case Token::CONST:
		match.push_back(parseConstDecl());
		match.push_back(parsePreamble());
		break;
		
	default:
		break;
		
	}
	
	return result;
}

Ref<Object> Parser::parseElseStmt()
{
	vector<Ref<Object> > match;
	Ref<Object> result;

	switch (peek().type) {
	default:
		break;
		
	case Token::ELSE:
		match.push_back(accept(Token::ELSE).value);
		match.push_back(parseStatement());
		break;
		
	}
	
	return result;
}

Ref<Object> Parser::parseTerm()
{
	vector<Ref<Object> > match;
	Ref<Object> result;

	switch (peek().type) {
	default:
		unexpectedToken(match, result);
		break;
		
	case Token::IDENT:
	case Token::NUMBER:
	case Token::LEFT_PAR:
		match.push_back(parseFactor());
		match.push_back(parseTermRest());
		break;
		
	}
	
	return result;
}

Ref<Object> Parser::parseRValue()
{
	vector<Ref<Object> > match;
	Ref<Object> result;

	switch (peek().type) {
	case Token::FUNCTION:
		match.push_back(accept(Token::FUNCTION).value);
		startFunction(match, result);
		match.push_back(accept(Token::LEFT_PAR).value);
		match.push_back(parseArgNameList());
		match.push_back(accept(Token::RIGHT_PAR).value);
		match.push_back(accept(Token::COLON).value);
		match.push_back(parseVarDecl());
		allocLocals(match, result);
		match.push_back(parseStatement());
		endFunction(match, result);
		break;
		
	case Token::IDENT:
	case Token::NUMBER:
	case Token::LEFT_PAR:
	case Token::MINUS:
		match.push_back(parseExpression());
		break;
		
	case Token::LAMBDA:
		match.push_back(accept(Token::LAMBDA).value);
		startFunction(match, result);
		match.push_back(accept(Token::LEFT_PAR).value);
		match.push_back(parseArgNameList());
		match.push_back(accept(Token::RIGHT_PAR).value);
		match.push_back(accept(Token::COLON).value);
		match.push_back(parseRValue());
		_writer.writeInstruction(AsmScanner::TOKEN_RETURN);
		endFunction(match, result);
		break;
		
	default:
		unexpectedToken(match, result);
		break;
		
	}
	
	return result;
}

Ref<Object> Parser::parseParamListRest()
{
	vector<Ref<Object> > match;
	Ref<Object> result;

	switch (peek().type) {
	default:
		break;
		
	case Token::COMMA:
		match.push_back(accept(Token::COMMA).value);
		match.push_back(parseRValue());
		match.push_back(parseParamListRest());
		break;
		
	}
	
	return result;
}

Ref<Object> Parser::parseParamList()
{
	vector<Ref<Object> > match;
	Ref<Object> result;

	switch (peek().type) {
	default:
		break;
		
	case Token::FUNCTION:
	case Token::IDENT:
	case Token::NUMBER:
	case Token::LEFT_PAR:
	case Token::MINUS:
	case Token::LAMBDA:
		match.push_back(parseRValue());
		match.push_back(parseParamListRest());
		break;
		
	}
	
	return result;
}

Ref<Object> Parser::parseExpression()
{
	vector<Ref<Object> > match;
	Ref<Object> result;

	switch (peek().type) {
	case Token::MINUS:
		match.push_back(accept(Token::MINUS).value);
		match.push_back(parseTerm());
		match.push_back(parseExpressionRest());
		break;
		
	default:
		unexpectedToken(match, result);
		break;
		
	case Token::IDENT:
	case Token::NUMBER:
	case Token::LEFT_PAR:
		match.push_back(parseTerm());
		match.push_back(parseExpressionRest());
		break;
		
	}
	
	return result;
}

Ref<Object> Parser::parseExpressionRest()
{
	vector<Ref<Object> > match;
	Ref<Object> result;

	switch (peek().type) {
	case Token::MINUS:
		match.push_back(accept(Token::MINUS).value);
		match.push_back(parseTerm());
		_writer.writeInstruction(AsmScanner::TOKEN_SUB);
		match.push_back(parseExpressionRest());
		break;
		
	default:
		break;
		
	case Token::PLUS:
		match.push_back(accept(Token::PLUS).value);
		match.push_back(parseTerm());
		_writer.writeInstruction(AsmScanner::TOKEN_ADD);
		match.push_back(parseExpressionRest());
		break;
		
	}
	
	return result;
}

Ref<Object> Parser::parsePostfixOp()
{
	vector<Ref<Object> > match;
	Ref<Object> result;

	switch (peek().type) {
	case Token::LEFT_PAR:
		match.push_back(accept(Token::LEFT_PAR).value);
		match.push_back(parseParamList());
		match.push_back(accept(Token::RIGHT_PAR).value);
		match.push_back(parsePostfixOp());
		break;
		
	case Token::LEFT_BRA:
		match.push_back(parseIndex());
		match.push_back(parsePostfixOp());
		break;
		
	default:
		break;
		
	}
	
	return result;
}

Ref<Object> Parser::parseVarDecl()
{
	vector<Ref<Object> > match;
	Ref<Object> result;

	switch (peek().type) {
	default:
		unexpectedToken(match, result);
		break;
		
	case Token::VAR:
		match.push_back(accept(Token::VAR).value);
		match.push_back(accept(Token::IDENT).value);
		addLocal(match, result);
		match.push_back(parseVarDeclRest());
		match.push_back(accept(Token::SEMICOLON).value);
		break;
		
	}
	
	return result;
}

Ref<Object> Parser::parseLValueRest()
{
	vector<Ref<Object> > match;
	Ref<Object> result;

	switch (peek().type) {
	default:
		break;
		
	case Token::LEFT_BRA:
		match.push_back(parseIndex());
		match.push_back(parseLValueRest());
		break;
		
	}
	
	return result;
}

Ref<Object> Parser::parsePrimaryExpr()
{
	vector<Ref<Object> > match;
	Ref<Object> result;

	switch (peek().type) {
	default:
		unexpectedToken(match, result);
		break;
		
	case Token::IDENT:
		match.push_back(accept(Token::IDENT).value);
		getSymbolValue(match, result);
		match.push_back(parsePostfixOp());
		break;
		
	}
	
	return result;
}

Ref<Object> Parser::parseConstDecl()
{
	vector<Ref<Object> > match;
	Ref<Object> result;

	switch (peek().type) {
	default:
		unexpectedToken(match, result);
		break;
		
	case Token::CONST:
		match.push_back(accept(Token::CONST).value);
		match.push_back(accept(Token::IDENT).value);
		match.push_back(accept(Token::EQUALS).value);
		match.push_back(accept(Token::NUMBER).value);
		addConst(match, result);
		match.push_back(parseConstDeclRest());
		match.push_back(accept(Token::SEMICOLON).value);
		break;
		
	}
	
	return result;
}

Ref<Object> Parser::parseCompoundStmt()
{
	vector<Ref<Object> > match;
	Ref<Object> result;

	switch (peek().type) {
	default:
		unexpectedToken(match, result);
		break;
		
	case Token::BEGIN:
		match.push_back(accept(Token::BEGIN).value);
		match.push_back(parseStatement());
		match.push_back(parseStatementRest());
		match.push_back(accept(Token::END).value);
		break;
		
	}
	
	return result;
}

