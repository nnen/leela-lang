/**
 * \file   Parser.rules.tpl.h
 * \author Jan Milik <milikjan@fit.cvut.cz>
 * \date   2011-04-28
 *
 * \brief  
 */

enum Nonterminals {
	VAR_DECL,
	SUM,
	CONST_EXPR,
	PROGRAM,
	STATEMENT,
	FACTOR,
	COMPARSION,
	ARRAY_ITEM_LIST,
	CONST_DECL_REST,
	TERM_REST,
	ASSIGNMENT,
	ARG_NAME_LIST,
	ARG_NAME_LIST_REST,
	STATEMENT_REST,
	ARRAY_ITEM_LIST_REST,
	PREAMBLE,
	VAR_DECL_REST,
	TERM,
	R_VALUE,
	PARAM_LIST_REST,
	ASSIGN_ITEM,
	PARAM_LIST,
	ARRAY_ITEM_REST,
	EXPRESSION,
	EXPRESSION_REST,
	POSTFIX_OP,
	COMPARSION_REST,
	SUM_REST,
	ASSIGN_VAR,
	FUNCTION_PREAMBLE,
	ELSE_STMT,
	ARRAY_ITEM,
	PRIMARY_EXPR,
	CONST_DECL,
	COMPOUND_STMT,
	NONTEMRINAL_COUNT
};

private:

/* Nonterminals ****************************************/

Ref<Object> parseVarDecl(Ref<Object> inherited, vector<Ref<Object> > siblings);
Ref<Object> parseSum(Ref<Object> inherited, vector<Ref<Object> > siblings);
Ref<Object> parseConstExpr(Ref<Object> inherited, vector<Ref<Object> > siblings);
Ref<Object> parseProgram(Ref<Object> inherited, vector<Ref<Object> > siblings);
Ref<Object> parseStatement(Ref<Object> inherited, vector<Ref<Object> > siblings);
Ref<Object> parseFactor(Ref<Object> inherited, vector<Ref<Object> > siblings);
Ref<Object> parseComparsion(Ref<Object> inherited, vector<Ref<Object> > siblings);
Ref<Object> parseArrayItemList(Ref<Object> inherited, vector<Ref<Object> > siblings);
Ref<Object> parseConstDeclRest(Ref<Object> inherited, vector<Ref<Object> > siblings);
Ref<Object> parseTermRest(Ref<Object> inherited, vector<Ref<Object> > siblings);
Ref<Object> parseAssignment(Ref<Object> inherited, vector<Ref<Object> > siblings);
Ref<Object> parseArgNameList(Ref<Object> inherited, vector<Ref<Object> > siblings);
Ref<Object> parseArgNameListRest(Ref<Object> inherited, vector<Ref<Object> > siblings);
Ref<Object> parseStatementRest(Ref<Object> inherited, vector<Ref<Object> > siblings);
Ref<Object> parseArrayItemListRest(Ref<Object> inherited, vector<Ref<Object> > siblings);
Ref<Object> parsePreamble(Ref<Object> inherited, vector<Ref<Object> > siblings);
Ref<Object> parseVarDeclRest(Ref<Object> inherited, vector<Ref<Object> > siblings);
Ref<Object> parseTerm(Ref<Object> inherited, vector<Ref<Object> > siblings);
Ref<Object> parseRValue(Ref<Object> inherited, vector<Ref<Object> > siblings);
Ref<Object> parseParamListRest(Ref<Object> inherited, vector<Ref<Object> > siblings);
Ref<Object> parseAssignItem(Ref<Object> inherited, vector<Ref<Object> > siblings);
Ref<Object> parseParamList(Ref<Object> inherited, vector<Ref<Object> > siblings);
Ref<Object> parseArrayItemRest(Ref<Object> inherited, vector<Ref<Object> > siblings);
Ref<Object> parseExpression(Ref<Object> inherited, vector<Ref<Object> > siblings);
Ref<Object> parseExpressionRest(Ref<Object> inherited, vector<Ref<Object> > siblings);
Ref<Object> parsePostfixOp(Ref<Object> inherited, vector<Ref<Object> > siblings);
Ref<Object> parseComparsionRest(Ref<Object> inherited, vector<Ref<Object> > siblings);
Ref<Object> parseSumRest(Ref<Object> inherited, vector<Ref<Object> > siblings);
Ref<Object> parseAssignVar(Ref<Object> inherited, vector<Ref<Object> > siblings);
Ref<Object> parseFunctionPreamble(Ref<Object> inherited, vector<Ref<Object> > siblings);
Ref<Object> parseElseStmt(Ref<Object> inherited, vector<Ref<Object> > siblings);
Ref<Object> parseArrayItem(Ref<Object> inherited, vector<Ref<Object> > siblings);
Ref<Object> parsePrimaryExpr(Ref<Object> inherited, vector<Ref<Object> > siblings);
Ref<Object> parseConstDecl(Ref<Object> inherited, vector<Ref<Object> > siblings);
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
void negate(
	Ref<Object> inherited,
	Match siblings,
	Match& matched,
	Ref<Object>& result);
void pushSibling(
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
void dumpSymbols(
	Ref<Object> inherited,
	Match siblings,
	Match& matched,
	Ref<Object>& result);
void addLocal(
	Ref<Object> inherited,
	Match siblings,
	Match& matched,
	Ref<Object>& result);
void pushSymbolValue(
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
void writeLine(
	Ref<Object> inherited,
	Match siblings,
	Match& matched,
	Ref<Object>& result);

