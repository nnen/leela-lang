/**
 * \file   Parser.rules.tpl.h
 * \author Jan Milik <milikjan@fit.cvut.cz>
 * \date   2011-04-28
 *
 * \brief  
 */

enum Nonterminals {
	VAR_DECL,
	VAR_DECL_REST,
	CONST_EXPR,
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
	MULT,
	TERM,
	ASSIGN_INDEX,
	R_VALUE,
	PARAM_LIST_REST,
	PARAM_LIST,
	EXPRESSION,
	EXPRESSION_REST,
	POSTFIX_OP,
	ASSIGN_VAR,
	CONST_DECL,
	FUNCTION_PREAMBLE,
	L_VALUE_REST,
	ASSIGNMENT,
	PRIMARY_EXPR,
	MULT_REST,
	COMPOUND_STMT,
	NONTEMRINAL_COUNT
};

private:

/* Nonterminals ****************************************/

Ref<Object> parseVarDecl(Ref<Object> inherited, vector<Ref<Object> > siblings);
Ref<Object> parseVarDeclRest(Ref<Object> inherited, vector<Ref<Object> > siblings);
Ref<Object> parseConstExpr(Ref<Object> inherited, vector<Ref<Object> > siblings);
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
Ref<Object> parseMult(Ref<Object> inherited, vector<Ref<Object> > siblings);
Ref<Object> parseTerm(Ref<Object> inherited, vector<Ref<Object> > siblings);
Ref<Object> parseAssignIndex(Ref<Object> inherited, vector<Ref<Object> > siblings);
Ref<Object> parseRValue(Ref<Object> inherited, vector<Ref<Object> > siblings);
Ref<Object> parseParamListRest(Ref<Object> inherited, vector<Ref<Object> > siblings);
Ref<Object> parseParamList(Ref<Object> inherited, vector<Ref<Object> > siblings);
Ref<Object> parseExpression(Ref<Object> inherited, vector<Ref<Object> > siblings);
Ref<Object> parseExpressionRest(Ref<Object> inherited, vector<Ref<Object> > siblings);
Ref<Object> parsePostfixOp(Ref<Object> inherited, vector<Ref<Object> > siblings);
Ref<Object> parseAssignVar(Ref<Object> inherited, vector<Ref<Object> > siblings);
Ref<Object> parseConstDecl(Ref<Object> inherited, vector<Ref<Object> > siblings);
Ref<Object> parseFunctionPreamble(Ref<Object> inherited, vector<Ref<Object> > siblings);
Ref<Object> parseLValueRest(Ref<Object> inherited, vector<Ref<Object> > siblings);
Ref<Object> parseAssignment(Ref<Object> inherited, vector<Ref<Object> > siblings);
Ref<Object> parsePrimaryExpr(Ref<Object> inherited, vector<Ref<Object> > siblings);
Ref<Object> parseMultRest(Ref<Object> inherited, vector<Ref<Object> > siblings);
Ref<Object> parseCompoundStmt(Ref<Object> inherited, vector<Ref<Object> > siblings);


/* Semantic actions ************************************/

void allocLocals(
	Ref<Object> inherited,
	Match siblings,
	Match& matched,
	Ref<Object>& result);
void addConst(
	Ref<Object> inherited,
	Match siblings,
	Match& matched,
	Ref<Object>& result);
void pushNumber(
	Ref<Object> inherited,
	Match siblings,
	Match& matched,
	Ref<Object>& result);
void startFunction(
	Ref<Object> inherited,
	Match siblings,
	Match& matched,
	Ref<Object>& result);
void ifJump(
	Ref<Object> inherited,
	Match siblings,
	Match& matched,
	Ref<Object>& result);
void getSymbolValue(
	Ref<Object> inherited,
	Match siblings,
	Match& matched,
	Ref<Object>& result);
void ifEnd(
	Ref<Object> inherited,
	Match siblings,
	Match& matched,
	Ref<Object>& result);
void elseJump(
	Ref<Object> inherited,
	Match siblings,
	Match& matched,
	Ref<Object>& result);
void addArg(
	Ref<Object> inherited,
	Match siblings,
	Match& matched,
	Ref<Object>& result);
void getValueForLookup(
	Ref<Object> inherited,
	Match siblings,
	Match& matched,
	Ref<Object>& result);
void whileJump(
	Ref<Object> inherited,
	Match siblings,
	Match& matched,
	Ref<Object>& result);
void startContext(
	Ref<Object> inherited,
	Match siblings,
	Match& matched,
	Ref<Object>& result);
void identToString(
	Ref<Object> inherited,
	Match siblings,
	Match& matched,
	Ref<Object>& result);
void writeCall(
	Ref<Object> inherited,
	Match siblings,
	Match& matched,
	Ref<Object>& result);
void startWhile(
	Ref<Object> inherited,
	Match siblings,
	Match& matched,
	Ref<Object>& result);
void endWhile(
	Ref<Object> inherited,
	Match siblings,
	Match& matched,
	Ref<Object>& result);
void returnZero(
	Ref<Object> inherited,
	Match siblings,
	Match& matched,
	Ref<Object>& result);
void assignVar(
	Ref<Object> inherited,
	Match siblings,
	Match& matched,
	Ref<Object>& result);
void writeStrings(
	Ref<Object> inherited,
	Match siblings,
	Match& matched,
	Ref<Object>& result);
void addLocal(
	Ref<Object> inherited,
	Match siblings,
	Match& matched,
	Ref<Object>& result);
void endFunction(
	Ref<Object> inherited,
	Match siblings,
	Match& matched,
	Ref<Object>& result);
void writeChunks(
	Ref<Object> inherited,
	Match siblings,
	Match& matched,
	Ref<Object>& result);
void returnPlusOne(
	Ref<Object> inherited,
	Match siblings,
	Match& matched,
	Ref<Object>& result);
void pushString(
	Ref<Object> inherited,
	Match siblings,
	Match& matched,
	Ref<Object>& result);

