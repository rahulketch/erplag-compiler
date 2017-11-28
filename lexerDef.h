/*
Group 8
Rahul Sharma	 2013B5A7713P
Srajan Jain 	 2013B5A7810P
*/

#define bufferSize 100
#define err_invalidID -97
#define err_bufferOverflow -98
//#define err_EOF -99
#define err_Comment -100
#define err_invalidInput -101
#define err_wrongSymbol -102
#define start_symbol nt_program
enum enum_tnt{t_DOLLAR, t_EPSILON, t_AND, t_ARRAY, t_ASSIGNOP, t_BC, t_BO, t_BOOLEAN, t_BREAK, t_CASE, t_COLON, t_COMMA, t_DECLARE, t_DEF, t_DEFAULT, t_DIV, t_DRIVER,t_DRIVERDEF,t_DRIVERENDDEF, t_END, t_ENDDEF, t_EQ, t_FALSE, t_FOR, t_GE, t_GET_VALUE, t_GT, t_ID, t_IN, t_INPUT, t_INTEGER, t_LE, t_LT, t_MINUS, t_MODULE, t_MUL, t_NE, t_NUM, t_OF, t_OR, t_PARAMETERS, t_PLUS, t_PRINT, t_PROGRAM, t_RANGEOP, t_REAL, t_RETURNS, t_RNUM, t_SEMICOL, t_SQBC, t_SQBO, t_START, t_SWITCH, t_TAKES, t_TRUE, t_USE, t_WHILE, t_WITH, nt_AnyTerm, nt_arithmeticExpr, nt_arithmeticOrBooleanExpr, nt_assignmentStmt, nt_caseStmts, nt_condionalStmt, nt_dataType, nt_declareStmt, nt_default, nt_driverModule, nt_expression, nt_factor, nt_idList, nt_index, nt_input_plist, nt_ioStmt, nt_iterativeStmt, nt_logicalOp, nt_lvalueIDStmt, nt_lvalueARRStmt, nt_module, nt_moduleDeclaration, nt_moduleDeclarations, nt_moduleDef, nt_moduleReuseStmt, nt_N1, nt_N2, nt_N3, nt_N4, nt_N5, nt_N7, nt_N8, nt_N9, nt_op1, nt_op2, nt_optional, nt_otherModules, nt_output_plist, nt_program, nt_range, nt_relationalOp, nt_ret, nt_simpleStmt, nt_statement, nt_statements, nt_term, nt_type, nt_value, nt_var, nt_whichId, nt_whichStmt, t_IDF, nt_minusExpr, undec };//after t_IDF, for convenience exceptions
typedef enum enum_tnt tnt;

typedef struct{
	char lexeme[bufferSize];
	unsigned long int lineNo;
	char tokenName[bufferSize];
	int tokenId;
} tokenInfo;
