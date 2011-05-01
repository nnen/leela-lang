/**
 * \file   Parser.rules.tpl.h
 * \author Jan Milik <milikjan@fit.cvut.cz>
 * \date   2011-04-28
 *
 * \brief  
 */

enum Nonterminals {
	INDEX,
	VAR_DECL,
	VAR_DECL_REST,
	PROGRAM,
	STATEMENT,
	FACTOR,
	CONST_DECL_REST,
	TERM_REST,
	L_VALUE,
	ARG_NAME_LIST,
	ARG_NAME_LIST_REST,
	STATEMENT_REST,
	PREAMBLE,
	ELSE_STMT,
	TERM,
	R_VALUE,
	PARAM_LIST_REST,
	PARAM_LIST,
	EXPRESSION,
	EXPRESSION_REST,
	POSTFIX_OP,
	FUNCTION_PREAMBLE,
	L_VALUE_REST,
	PRIMARY_EXPR,
	CONST_DECL,
	COMPOUND_STMT,
	NONTEMRINAL_COUNT
};

private:

Ref<Object> parseIndex();
Ref<Object> parseVarDecl();
Ref<Object> parseVarDeclRest();
Ref<Object> parseProgram();
Ref<Object> parseStatement();
Ref<Object> parseFactor();
Ref<Object> parseConstDeclRest();
Ref<Object> parseTermRest();
Ref<Object> parseLValue();
Ref<Object> parseArgNameList();
Ref<Object> parseArgNameListRest();
Ref<Object> parseStatementRest();
Ref<Object> parsePreamble();
Ref<Object> parseElseStmt();
Ref<Object> parseTerm();
Ref<Object> parseRValue();
Ref<Object> parseParamListRest();
Ref<Object> parseParamList();
Ref<Object> parseExpression();
Ref<Object> parseExpressionRest();
Ref<Object> parsePostfixOp();
Ref<Object> parseFunctionPreamble();
Ref<Object> parseLValueRest();
Ref<Object> parsePrimaryExpr();
Ref<Object> parseConstDecl();
Ref<Object> parseCompoundStmt();

