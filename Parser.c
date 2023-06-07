/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Winter, 2022
* Author: Svillen Ranev - Paulo Sousa
* Professors: Paulo Sousa / George Kriger / Abdullah Kadri
************************************************************
*/

/*
************************************************************
* File name: mainBuffer.c
* Compiler: MS Visual Studio 2022
* Author: Paulo Sousa
* Course: CST 8152 – Compilers, Lab Section: [011, 012, 013, 014]
* Assignment: A12, A22, A32.
* Date: Jan 01 2022
* Professor: Paulo Sousa
* Purpose: This file contains all functionalities from Parser.
* Function list: (...).
************************************************************
*/

/* TODO: Adjust the function header */

#ifndef PARSER_H_
#include "Parser.h"
#endif

/*
************************************************************
 * Process Parser
 ***********************************************************
 */
 /* TODO: This is the function to start the parser - check your program definition */

india_nul startParser() {
	lookahead = tokenizer();
	if (lookahead.code != SEOF_T) {
		program();
	}
	matchToken(SEOF_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Source file parsed");
}


/*
 ************************************************************
 * Match Token
 ***********************************************************
 */
 /* TODO: This is the main code for match - check your definition */
india_nul matchToken(india_int tokenCode, india_int tokenAttribute) {
	india_int matchFlag = 1;

	switch (lookahead.code) {
	case KEY_T:
	case RELO_T:
	case LOGO_T:
	case ARTHO_T:
		if (lookahead.attribute.codeType != tokenAttribute)
			matchFlag = 0;
	default:
		if (lookahead.code != tokenCode)
			matchFlag = 0;
	}

	if (matchFlag && lookahead.code == SEOF_T)
		return;

	if (matchFlag) {

		lookahead = tokenizer();

		if (lookahead.code == ERR_T) {

			printError();
			lookahead = tokenizer();
			syntaxErrorNumber++;
		}
	}
	else
		syncErrorHandler(tokenCode);
}

/*
 ************************************************************
 * Syncronize Error Handler
 ***********************************************************
 */
 /* TODO: This is the function to handler error - adjust basically datatypes */
india_nul syncErrorHandler(india_int syncTokenCode) {

	printError();

	syntaxErrorNumber++;

	while (lookahead.code != syncTokenCode) {

		if (lookahead.code == SEOF_T)
			exit(syntaxErrorNumber);

		lookahead = tokenizer();
	}

	if (lookahead.code != SEOF_T)
		lookahead = tokenizer();
}

/*
 ************************************************************
 * Print Error
 ***********************************************************
 */
 /* TODO: This is the function to error printing - adjust basically datatypes */
india_nul printError() {

	Token t = lookahead;

	printf("%s%s%3d\n", STR_LANGNAME, ": Syntax error:  Line:", line);
	printf("*****  Token code:%3d Attribute: ", t.code);

	switch (t.code) {
	case ERR_T:
		printf("%s\n\n", t.attribute.errLexeme);
		break;
	case SEOF_T:
		printf("SEOF_T\t\t%d\t\n\n", t.attribute.seofType);
		break;
	case MVID_T:
		printf("%s\n\n", t.attribute.idLexeme);
		break;
	case IVID_T:
		printf("%s\n\n", t.attribute.idLexeme);
		break;
	case FVID_T:
		printf("%s\n\n", t.attribute.idLexeme);
		break;
	case SVID_T:
		printf("%s\n\n", t.attribute.idLexeme);
		break;
	case BVID_T:
		printf("%s\n\n", t.attribute.idLexeme);
		break;
	case SL_T:
		printf("%s\n\n", bGetContent(stringLiteralTable, t.attribute.contentString));
		break;
	case KEY_T:
		printf("%s\n\n", keywordTable[t.attribute.codeType]);
		break;
	case LPR_T:
	case LSB_T:
	case RSB_T:
	case RPR_T:
	case LBR_T:
	case RBR_T:
	case EOS_T:
	case COMMA_T:
	case SCO_T:
	case ASSO_T:
		printf("NA\n\n");
		break;
	case ARTHO_T:
		printf("%d\n\n", t.attribute.codeType);
		break;
	case RELO_T:
		printf("%d\n\n", t.attribute.codeType);
		break;
	case LOGO_T:
		printf("%d\n\n", t.attribute.codeType);
		break;
	case INTL_T:
		printf("%d\n\n", t.attribute.codeType);
		break;
	case FPVL_T:
		printf("%g\n\n", t.attribute.floatValue);
		break;
	case SLR_T:
		printf("%s\n\n", bGetContent(literalReaderTable, (short)t.attribute.codeType));
		break;
	case INTR_T:
		printf("%s\n\n", bGetContent(literalReaderTable, (short)t.attribute.codeType));
		break;
	case FDINTR_T:
		printf("%s\n\n", bGetContent(literalReaderTable, (short)t.attribute.codeType));
		break;
	case BOOLR_T:
		printf("%s\n\n", bGetContent(literalReaderTable, (short)t.attribute.codeType));
		break;
	default:
		printf("%s%s%d\n\n", STR_LANGNAME, ": Scanner error: invalid token code: ", t.code);
	}
}

/*
 ************************************************************
 * Program statement
 * BNF: <program> -> int main~ { opt_statements }
 * FIRST(<program>)= {KW_T (MAIN)}.
 ***********************************************************
 */
india_nul program() {

	switch (lookahead.code) {

	case KEY_T:// check first int

		if (lookahead.attribute.codeType == INT) {

			matchToken(KEY_T, INT);

			switch (lookahead.code) { //then next main~

			case MVID_T://main~
				if (strncmp(lookahead.attribute.idLexeme, "main~", 5) == 0) {

					matchToken(MVID_T, NO_ATTR);//main~

					matchToken(LBR_T, NO_ATTR);// {

					optionalStatements();// inside main~ (other statements)

					matchToken(RBR_T, NO_ATTR);// }

					break;
				}
				else {
					printError();
				}
			case SEOF_T:
				; // Empty
				break;
			default://error
				printError();
			}
		}
		break;
	default://error if first key is not int
		printError();

	}

	printf("%s%s\n", STR_LANGNAME, ": Program parsed");
}

/*
 ************************************************************
 * Optional Var List Declarations
 * BNF: <opt_varlist_declarations> -> <varlist_declarations> | e
 * FIRST(<opt_varlist_declarations>) = { e, KW_T (INT), KW_T (FINT), KW_T (BOOLEAN), KW_T (STRING)}.
 ***********************************************************
 */
india_nul optVarListDeclarations() {

	switch (lookahead.code) {

	case KEY_T:
		if (lookahead.attribute.codeType == INT
			|| lookahead.attribute.codeType == FINT
			|| lookahead.attribute.codeType == BOOLEAN
			|| lookahead.attribute.codeType == STRING) {
			varListDeclarations();
		}
		break;
	default://no error it could be empty or other statements without usage of variables
		; // Empty
	}
	printf("%s%s\n", STR_LANGNAME, ": Optional Variable List Declarations parsed");
}


/*GJK*/
/*--------------------------------------------------------------------------------
 ************************************************************
 * Var List Declarations
 * BNF: <varlist_declarations> -> <varlist_declaration> <varlist_declarationsPrime>
 * FIRST(<varlist_declarations>) = { KW_T (INT), KW_T (FLOAT), KW_T (STRING)}.
 ***********************************************************
 */
india_nul varListDeclarations() {

	varListDeclaration();
	varListDeclarationsPrime();
	printf("%s%s\n", STR_LANGNAME, ": Variable List Declarations parsed");
}

/*
 ************************************************************
 * Var List Declaration
 * BNF: <varlist_declaration> -> <integer_varlist_declaration>
					| <float_varlist_declaration>
					| <string_varlist_declaration>
 * FIRST(<varlist_declarations>) = { KW_T (INT), KW_T (FLOAT), KW_T (STRING)}.
 ***********************************************************
 */
 // int i, j, k;
india_nul varListDeclaration() {

	switch (lookahead.code) {
	case KEY_T:
		switch (lookahead.attribute.codeType) {
		case INT:
			matchToken(KEY_T, INT);
			intVarListDeclaration();
			break;
		case FINT:
			matchToken(KEY_T, FINT);
			fintVarListDeclaration();
			break;
		case BOOLEAN:
			matchToken(KEY_T, BOOLEAN);
			boolVarListDeclaration();
			break;
		case STRING:
			matchToken(KEY_T, STRING);
			stringVarListDeclaration();
			break;
		default:
			printError();
			break;
		}
		break;
	default:
		printError();
	}

	printf("%s%s\n", STR_LANGNAME, ": Variable List Declaration parsed");
}


/*
 ************************************************************
 * Var List Declaration Prime
 * BNF: <varlist_declarationsPrime> -> <varlist_declaration> <varlist_declarationsPrime> | e
 * FIRST(<varlist_declarationsPrime>) = { KW_T (INT), KW_T (FLOAT), KW_T (STRING), e }
 ***********************************************************
 */
india_nul varListDeclarationsPrime() {

	//no need to check for comma? (we checked comma in each var lists)

	switch (lookahead.code) {

	case KEY_T:
		switch (lookahead.attribute.codeType) {
		case INT:
			varListDeclaration();
			varListDeclarationsPrime();
			break;
		case FINT:
			varListDeclaration();
			varListDeclarationsPrime();
			break;
		case BOOLEAN:
			varListDeclaration();
			varListDeclarationsPrime();
			break;
		case STRING:
			varListDeclaration();
			varListDeclarationsPrime();
			break;
		default:
			printError();
		}
		break;
	default:
		; // Empty
	}

}
/*----------------------------------------------------------------------------------------*/
/*GJK*/

////////
india_nul intVarListDeclaration() {

	intVarList();

	if (lookahead.code == EOS_T) {

		matchToken(EOS_T, NO_ATTR);// at last check ;

	}
	else {

		printError();
	}

	printf("int Var List Declaration parsed.\n");
}

//<integer_variable_list> ->  <integer_variable> | <integer_variable_list>, <integer_variable>
india_nul intVarList() {

	switch (lookahead.code) {
	case IVID_T:
		matchToken(IVID_T, NO_ATTR);
		printf("int Variable identifier parsed.\n");
		intVarListPrime();
		break;
	default:
		printError();
	}

	printf("int Var List parsed.\n");
}

//if , is the next token only then check next token as IVID and call prime again else error
india_nul intVarListPrime() {

	if (lookahead.code == COMMA_T) {

		matchToken(COMMA_T, NO_ATTR);

		switch (lookahead.code) {

		case IVID_T:
			matchToken(IVID_T, NO_ATTR);
			printf("int Variable identifier parsed.\n");
			intVarListPrime();
			break;
		default://we checked , so if we have , and no ivid then error
			;
		}
	}
}
////////

india_nul fintVarListDeclaration() {

	fintVarList();

	if (lookahead.code == EOS_T) {

		matchToken(EOS_T, NO_ATTR);// at last check ;

	}
	else {

		printError();
	}

	printf("fint Var List Declaration parsed.\n");
}

//<integer_variable_list> ->  <integer_variable> | <integer_variable_list>, <integer_variable>
india_nul fintVarList() {

	switch (lookahead.code) {

	case FVID_T:
		matchToken(FVID_T, NO_ATTR);
		printf("fint Variable identifier parsed.\n");
		fintVarListPrime();
		break;
	default:
		printError();
	}

	printf("fint Var List parsed.\n");
}

//if , is the next token only then check next token as IVID and call prime again else error
india_nul fintVarListPrime() {

	if (lookahead.code == COMMA_T) {

		matchToken(COMMA_T, NO_ATTR);

		switch (lookahead.code) {

		case FVID_T:
			matchToken(FVID_T, NO_ATTR);
			printf("fint Variable identifier parsed.\n");
			fintVarListPrime();
			break;
		default://we checked , so if we have , and no ivid then error
			;
		}
	}
}
////////

india_nul stringVarListDeclaration() {

	stringVarList();

	if (lookahead.code == EOS_T) {

		matchToken(EOS_T, NO_ATTR);// at last check ;

	}
	else {

		printError();
	}

	printf("string Var List Declaration parsed.\n");
}

//<integer_variable_list> ->  <integer_variable> | <integer_variable_list>, <integer_variable>
india_nul stringVarList() {

	switch (lookahead.code) {

	case SVID_T:
		matchToken(SVID_T, NO_ATTR);
		printf("string Variable identifier parsed.\n");
		stringVarListPrime();
		break;
	default:
		printError();
	}

	printf("string Var List parsed.\n");
}

//if , is the next token only then check next token as IVID and call prime again else error
india_nul stringVarListPrime() {

	if (lookahead.code == COMMA_T) {

		matchToken(COMMA_T, NO_ATTR);

		switch (lookahead.code) {

		case SVID_T:
			matchToken(SVID_T, NO_ATTR);
			printf("string Variable identifier parsed.\n");
			stringVarListPrime();
			break;
		default://we checked , so if we have , and no ivid then error
			;
		}
	}
}
////////

india_nul boolVarListDeclaration() {

	boolVarList();

	if (lookahead.code == EOS_T) {

		matchToken(EOS_T, NO_ATTR);// at last check ;

	}
	else {

		printError();
	}

	printf("boolean Var List Declaration parsed.\n");
}

//<integer_variable_list> ->  <integer_variable> | <integer_variable_list>, <integer_variable>
india_nul boolVarList() {

	switch (lookahead.code) {

	case BVID_T:
		matchToken(BVID_T, NO_ATTR);
		printf("boolean Variable identifier parsed.\n");
		boolVarListPrime();
		break;
	default:
		printError();
	}

	printf("boolean Var List parsed.\n");
}

//if , is the next token only then check next token as IVID and call prime again else error
india_nul boolVarListPrime() {

	if (lookahead.code == COMMA_T) {

		matchToken(COMMA_T, NO_ATTR);

		switch (lookahead.code) {

		case BVID_T:
			matchToken(BVID_T, NO_ATTR);
			printf("boolean Variable identifier parsed.\n");
			boolVarListPrime();
			break;
		default://we checked , so if we have , and no ivid then error
			;
		}
	}
}

/*
 ************************************************************
 * Optional statement (inside main~ method)
 * BNF: <opt_statements> -> <statements> | ϵ
 * FIRST(<opt_statements>) = { ϵ , IVID_T, FVID_T, SVID_T, KW_T(IF),
 *				KW_T(WHILE), KW_T(READ), KW_T(WRITE), <opt_varlist_declarations> }
 ***********************************************************
 */
india_nul optionalStatements() {

	switch (lookahead.code) {

	case IVID_T:
	case FVID_T:
	case SVID_T:
	case BVID_T:
		statements();
		break;
	case KEY_T:
		if (lookahead.attribute.codeType == PRINTF
			|| lookahead.attribute.codeType == INT
			|| lookahead.attribute.codeType == FINT
			|| lookahead.attribute.codeType == STRING
			|| lookahead.attribute.codeType == BOOLEAN
			|| lookahead.attribute.codeType == SCANF) {
			statements();
			break;
		}
	default:
		; // Empty
	}

	printf("%s%s\n", STR_LANGNAME, ": Optional statements parsed");
}




/* TODO_205: Continue the development (all non-terminal functions) */



/*
 ************************************************************
 * Statements
 * BNF: <statements> -> <statement><statementsPrime>
 * FIRST(<statements>) = { IVID_T, FVID_T, SVID_T, KW_T(IF),
 *		KW_T(WHILE), KW_T(READ), KW_T(WRITE) }
 ***********************************************************
 */
india_nul statements() {
	statement();
	statementsPrime();
	printf("%s%s\n", STR_LANGNAME, ": Statements parsed");
}

/*
 ************************************************************
 * Statements Prime
 * BNF: <statementsPrime>  <statement><statementsPrime> | ϵ
 * FIRST(<statementsPrime>) = { ϵ , IVID_T, FVID_T, SVID_T,
 *		KW_T(IF), KW_T(WHILE), KW_T(READ), KW_T(WRITE) }
 ***********************************************************
 */
india_nul statementsPrime() {

	switch (lookahead.code) {

	case IVID_T:
	case FVID_T:
	case SVID_T:
	case BVID_T:
		statement();
		statementsPrime();
		break;
	case KEY_T:
		switch (lookahead.attribute.codeType) {
		case SCANF:
		case PRINTF:
		case INT:
		case FINT:
		case BOOLEAN:
		case STRING:
			statement();
			statementsPrime();
			break;
		}
		break;
	default:
		; //empty string
	}
}

/*
 ************************************************************
 * Single statement
 * BNF: <statement> ->  <assignment statement> | <selection statement> |
 *	<iteration statement> | <input statement> | <output statement>
 * FIRST(<statement>) = { IVID_T, FVID_T, SVID_T, KW_T(IF), KW_T(WHILE),
 *			KW_T(READ), KW_T(WRITE) }
 ***********************************************************
 */
india_nul statement() {

	switch (lookahead.code) {

	case IVID_T:
	case FVID_T:
	case BVID_T:
	case SVID_T:
		assignmentStatement();
		break;
	case KEY_T:

		switch (lookahead.attribute.codeType) {

		case INT:
		case FINT:
		case BOOLEAN:
		case STRING:
			optVarListDeclarations();
			break;
		case PRINTF:
			outputStatement();
			break;
		case SCANF:
			inputStatement();
			break;
		default:
			printError();
		}
		break;
	default:
		printError();
	}
	printf("%s%s\n", STR_LANGNAME, ": Statement parsed");
}

/*
 ************************************************************
 * Assignment Statement
 * BNF: <assignment statement> -> <assignment expression>
 * FIRST(<assignment statement>) = { IVID_T, FVID_T, SVID_T, KW_T(IF),
 *		KW_T(WHILE), KW_T(READ), KW_T(WRITE) }
 ***********************************************************
 */
india_nul assignmentStatement() {
	assignmentExpression();
	matchToken(EOS_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Assignment statement parsed");
}


/*
 ************************************************************
 * Assignment Expression
 * BNF: <assignment expression> -> <integer_variable> = <arithmetic expression>
 *						| <float_variable> = <arithmetic expression>
 *						| <string_variable>= <string expression>
 * FIRST(<assignment expression>) = { IVID_T, FVID_T, SVID_T }
 ***********************************************************
 */
india_nul assignmentExpression() {

	switch (lookahead.code) {

	case IVID_T:
		matchToken(IVID_T, NO_ATTR);
		printf("int Variable identifier parsed.\n");
		matchToken(ASSO_T, NO_ATTR);
		arithmeticExpression();
		break;
	case FVID_T:
		matchToken(FVID_T, NO_ATTR);
		printf("fint Variable identifier parsed.\n");
		matchToken(ASSO_T, NO_ATTR);
		arithmeticExpression();
		break;
	case BVID_T:
		matchToken(BVID_T, NO_ATTR);
		matchToken(ASSO_T, NO_ATTR);
		booleanDecExpression();
		break;
	case SVID_T:
		matchToken(SVID_T, NO_ATTR);
		matchToken(ASSO_T, NO_ATTR);
		stringExpression();
		break;
	default:
		printError();
	}
	printf("%s%s\n", STR_LANGNAME, ": Assignment expression parsed");
}

india_nul arithmeticExpression() {

	switch (lookahead.code) {

	case ARTHO_T:
		switch (lookahead.attribute.codeType) {
		case OP_ADD:
		case OP_SUB:
			unaryArithmeticExpression();
			break;
		default:
			printError();
		}
		printf("%s%s\n", STR_LANGNAME, ": Arithmetic expression parsed");
		break;
	case IVID_T:
	case FVID_T:
	case INTL_T:
	case FPVL_T:
	case LPR_T:
		additiveArithmeticExpression();
		printf("%s%s\n", STR_LANGNAME, ": Arithmetic expression parsed");
		break;
	default:
		printError();
		break;
	}

}

india_nul unaryArithmeticExpression() {

	switch (lookahead.code) {
	case ARTHO_T:
		switch (lookahead.attribute.codeType) {
		case OP_ADD:
			matchToken(ARTHO_T, OP_ADD);
			primaryArithmeticExpression();
			break;
		case OP_SUB:
			matchToken(ARTHO_T, OP_SUB);
			primaryArithmeticExpression();
			break;
		default:
			printError();
		}
	}

	printf("%s%s\n", STR_LANGNAME, ": Unary arithmetic expression parsed");

}

india_nul primaryArithmeticExpression() {

	switch (lookahead.code) {
	case IVID_T:
		matchToken(IVID_T, NO_ATTR);
		break;
	case FVID_T:
		matchToken(FVID_T, NO_ATTR);
		break;
	case INTL_T:
	case FPVL_T:
		matchToken(lookahead.code, NO_ATTR);//var
		break;
	case LPR_T:
		matchToken(LPR_T, NO_ATTR);
		arithmeticExpression();
		matchToken(RPR_T, NO_ATTR);
		break;
	}

	printf("%s%s\n", STR_LANGNAME, ": Primary arithmetic expression parsed");

}

india_nul additiveArithmeticExpression() {

	multiplicativeArithmeticExpression();
	additiveArithmeticExpressionPrime();

	printf("%s%s\n", STR_LANGNAME, ": Additive arithmetic expression parsed");
}

india_nul additiveArithmeticExpressionPrime() {

	switch (lookahead.code) {
	case ARTHO_T:
		switch (lookahead.attribute.codeType) {
		case OP_ADD:
		case OP_SUB:
			matchToken(ARTHO_T, lookahead.attribute.codeType);
			multiplicativeArithmeticExpression();
			additiveArithmeticExpressionPrime();
			break;
		}
	}
}

india_nul multiplicativeArithmeticExpression() {

	primaryArithmeticExpression();
	multiplicativeArithmeticExpressionPrime();

	printf("%s%s\n", STR_LANGNAME, ": Multiplicative arithmetic expression parsed");
}

india_nul multiplicativeArithmeticExpressionPrime() {

	switch (lookahead.code) {
	case ARTHO_T:
		switch (lookahead.attribute.codeType) {
		case OP_MUL:
		case OP_DIV:
		case OP_MOD:
			matchToken(ARTHO_T, lookahead.attribute.codeType);
			primaryArithmeticExpression();
			multiplicativeArithmeticExpressionPrime();
			break;
		}
		break;
	}

}

/*<boolean declaration expression>   YES | NO | <primary boolean expression>(=BVID_T)  */
india_nul booleanDecExpression() {

	switch (lookahead.attribute.codeType) {

	case YESS:
		matchToken(KEY_T, YESS);
		break;
	case NOO:
		matchToken(KEY_T, NOO);
		break;
	default:
		if (lookahead.code == BVID_T) {
			matchToken(BVID_T, NO_ATTR);
		}
		else {
			printError();
		}

	}
	printf("%s%s\n", STR_LANGNAME, ": Boolean declaration expression parsed");
}
/*
 ************************************************************
 * Output Statement
 * BNF: <output statement> -> WRITE (<output statementPrime>);
 * FIRST(<output statement>) = { KW_T(WRITE) }
 ***********************************************************
 */
india_nul outputStatement() {
	matchToken(KEY_T, PRINTF);//printf
	matchToken(LPR_T, NO_ATTR);//(
	outputVariableList();
	matchToken(RPR_T, NO_ATTR);
	matchToken(EOS_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Output statement parsed");
}

india_nul inputStatement() {

	matchToken(KEY_T, SCANF);//scanf
	matchToken(LPR_T, NO_ATTR);//(
	specifierList();		   //(“%d”, num@
	matchToken(RPR_T, NO_ATTR);//)
	matchToken(EOS_T, NO_ATTR);//;
	printf("%s%s\n", STR_LANGNAME, ": Input statement parsed");
}

/*
 ************************************************************
 * Output Variable List
 * BNF: <opt_variable list> -> <variable list>
 * FIRST(<opt_variable_list>) = { IVID_T, FVID_T, SVID_T, ϵ }
 ***********************************************************
 */
india_nul outputVariableList() {

	//"NOTE" should have been expressed as one or more productions
	// so only two cases 1. SL_T and 2. <specifier list, variable list >
	switch (lookahead.code) {

	case SL_T:
		matchToken(SL_T, NO_ATTR);
		printf("%s\n", "INDIA: Output list (string literal) parsed");
		break;
	default:
		specifierList();
		printf("%s\n", "INDIA: Output list (specifier and variable list) parsed");
	}

	printf("%s%s\n", STR_LANGNAME, ": Output variable list parsed");
}

india_nul specifierList() {

	specifier();
	specifierListPrime();

	printf("%s%s\n", STR_LANGNAME, ": Specifier and Variable list both parsed");
}

//specifer is linking varlist in default (as it should be come after that)
india_nul specifier() {

	switch (lookahead.code) {

	case SLR_T:
		matchToken(SLR_T, NO_ATTR);
		printf("string specifier parsed.\n");
		break;
	case INTR_T:
		matchToken(INTR_T, NO_ATTR);
		printf("int specifier parsed.\n");
		break;
	case FDINTR_T:
		matchToken(FDINTR_T, NO_ATTR);
		printf("fint specifier parsed.\n");
		break;
	case BOOLR_T:
		matchToken(BOOLR_T, NO_ATTR);
		printf("boolean specifier parsed.\n");
		break;
	default:
		printError();
	}
}

india_nul specifierListPrime() {

	switch (lookahead.code) {

	case COMMA_T:
		matchToken(COMMA_T, NO_ATTR);

		if (lookahead.code == IVID_T ||
			lookahead.code == FVID_T ||
			lookahead.code == SVID_T ||
			lookahead.code == BVID_T) {
			varList();
		}
		else {
			specifier();
			specifierListPrime();
		}
		break;
	default:
		printError();
	}
}

india_nul varList() {

	varIdentifier();
	varListPrime();

	printf("%s%s\n", STR_LANGNAME, ": Variable list parsed");
}

india_nul varIdentifier() {

	switch (lookahead.code) {

	case IVID_T:
		matchToken(IVID_T, NO_ATTR);
		printf("int Variable identifier parsed.\n");
		break;
	case FVID_T:
		matchToken(FVID_T, NO_ATTR);
		printf("fint Variable identifier parsed.\n");
		break;
	case SVID_T:
		matchToken(SVID_T, NO_ATTR);
		printf("string Variable identifier parsed.\n");
		break;
	case BVID_T:
		matchToken(BVID_T, NO_ATTR);
		printf("boolean Variable identifier parsed.\n");
		break;
	default:
		printError();
		break;
	}
}

india_nul varListPrime() {

	switch (lookahead.code) {
	case COMMA_T:
		matchToken(COMMA_T, NO_ATTR);
		varIdentifier();
		varListPrime();
		break;
	}
}

india_nul stringExpression() {

	primaryStringExpression();
	stringExpressionPrime();

	printf("%s%s\n", STR_LANGNAME, ": String expression parsed.");

}

india_nul stringExpressionPrime() {

	if (lookahead.code == SCO_T) {

		matchToken(SCO_T, NO_ATTR);
		primaryStringExpression();
		stringExpressionPrime();
	}
}

india_nul primaryStringExpression() {

	switch (lookahead.code) {

	case SVID_T:
	case SL_T:
		matchToken(lookahead.code, NO_ATTR);
		printf("%s%s\n", STR_LANGNAME, ": Primary string expression parsed.");
		break;
	default:
		printError();
		break;
	}
}
