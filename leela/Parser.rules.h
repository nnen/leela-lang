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
	ASSIGN_INDEX,
	R_VALUE,
	PARAM_LIST_REST,
	PARAM_LIST,
	EXPRESSION,
	EXPRESSION_REST,
	POSTFIX_OP,
	ASSIGN_VAR,
	FUNCTION_PREAMBLE,
	L_VALUE_REST,
	ASSIGNMENT,
	PRIMARY_EXPR,
	CONST_DECL,
	COMPOUND_STMT,
	NONTEMRINAL_COUNT
};

private:

/* Nonterminals ****************************************/

Ref<Object> parseIndex(Ref<Object> inherited, vector<Ref<Object> > siblings);
Ref<Object> parseVarDecl(Ref<Object> inherited, vector<Ref<Object> > siblings);
Ref<Object> parseVarDeclRest(Ref<Object> inherited, vector<Ref<Object> > siblings);
Ref<Object> parseProgram(Ref<Object> inherited, vector<Ref<Object> > siblings);
Ref<Object> parseStatement(Ref<Object> inherited, vector<Ref<Object> > siblings);
Ref<Object> parseFactor(Ref<Object> inherited, vector<Ref<Object> > siblings);
Ref<Object> parseConstDeclRest(Ref<Object> inherited, vector<Ref<Object> > siblings);
Ref<Object> parseTermRest(Ref<Object> inherited, vector<Ref<Object> > siblings);
Ref<Object> parseLValue(Ref<Object> inherited, vector<Ref<Object> > siblings);
Ref<Object> parseArgNameList(Ref<Object> inherited, vector<Ref<Object> > siblings);
Ref<Object> parseArgNameListRest(Ref<Object> inherited, vector<Ref<Object> > siblings);
Ref<Object> parseStatementRest(Ref<Object> inherited, vector<Ref<Object> > siblings);
Ref<Object> parsePreamble(Ref<Object> inherited, vector<Ref<Object> > siblings);
Ref<Object> parseElseStmt(Ref<Object> inherited, vector<Ref<Object> > siblings);
Ref<Object> parseTerm(Ref<Object> inherited, vector<Ref<Object> > siblings);
Ref<Object> parseAssignIndex(Ref<Object> inherited, vector<Ref<Object> > siblings);
Ref<Object> parseRValue(Ref<Object> inherited, vector<Ref<Object> > siblings);
Ref<Object> parseParamListRest(Ref<Object> inherited, vector<Ref<Object> > siblings);
Ref<Object> parseParamList(Ref<Object> inherited, vector<Ref<Object> > siblings);
Ref<Object> parseExpression(Ref<Object> inherited, vector<Ref<Object> > siblings);
Ref<Object> parseExpressionRest(Ref<Object> inherited, vector<Ref<Object> > siblings);
Ref<Object> parsePostfixOp(Ref<Object> inherited, vector<Ref<Object> > siblings);
Ref<Object> parseAssignVar(Ref<Object> inherited, vector<Ref<Object> > siblings);
Ref<Object> parseFunctionPreamble(Ref<Object> inherited, vector<Ref<Object> > siblings);
Ref<Object> parseLValueRest(Ref<Object> inherited, vector<Ref<Object> > siblings);
Ref<Object> parseAssignment(Ref<Object> inherited, vector<Ref<Object> > siblings);
Ref<Object> parsePrimaryExpr(Ref<Object> inherited, vector<Ref<Object> > siblings);
Ref<Object> parseConstDecl(Ref<Object> inherited, vector<Ref<Object> > siblings);
Ref<Object> parseCompoundStmt(Ref<Object> inherited, vector<Ref<Object> > siblings);


/* Semantic actions ************************************/

Ref<Object> allocLocals(
	Ref<Object> inherited,
	Match siblings,
	Match& matched,
	Ref<Object>& result);
Ref<Object> addConst(
	Ref<Object> inherited,
	Match siblings,
	Match& matched,
	Ref<Object>& result);
Ref<Object> assignIndex(
	Ref<Object> inherited,
	Match siblings,
	Match& matched,
	Ref<Object>& result);
Ref<Object> assignVar(
	Ref<Object> inherited,
	Match siblings,
	Match& matched,
	Ref<Object>& result);
Ref<Object> endFunction(
	Ref<Object> inherited,
	Match siblings,
	Match& matched,
	Ref<Object>& result);
Ref<Object> addArg(
	Ref<Object> inherited,
	Match siblings,
	Match& matched,
	Ref<Object>& result);
Ref<Object> pushNumber(
	Ref<Object> inherited,
	Match siblings,
	Match& matched,
	Ref<Object>& result);
Ref<Object> pushString(
	Ref<Object> inherited,
	Match siblings,
	Match& matched,
	Ref<Object>& result);
Ref<Object> startFunction(
	Ref<Object> inherited,
	Match siblings,
	Match& matched,
	Ref<Object>& result);
Ref<Object> writeStrings(
	Ref<Object> inherited,
	Match siblings,
	Match& matched,
	Ref<Object>& result);
Ref<Object> startContext(
	Ref<Object> inherited,
	Match siblings,
	Match& matched,
	Ref<Object>& result);
Ref<Object> identToString(
	Ref<Object> inherited,
	Match siblings,
	Match& matched,
	Ref<Object>& result);
Ref<Object> writeChunks(
	Ref<Object> inherited,
	Match siblings,
	Match& matched,
	Ref<Object>& result);
Ref<Object> getSymbolValue(
	Ref<Object> inherited,
	Match siblings,
	Match& matched,
	Ref<Object>& result);
Ref<Object> addLocal(
	Ref<Object> inherited,
	Match siblings,
	Match& matched,
	Ref<Object>& result);

