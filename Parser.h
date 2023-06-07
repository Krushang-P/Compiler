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
* File name: parser.h
* Compiler: MS Visual Studio 2022
* Author: Paulo Sousa
* Course: CST 8152 – Compilers, Lab Section: [011, 012, 013, 014]
* Assignment: A12, A22, A32.
* Date: Jan 01 2022
* Professor: Paulo Sousa
* Purpose: This file is the main header for Parser (.h)
*************************************************************/

#ifndef PARSER_H_
#define PARSER_H_

/* Inclusion section */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif
#ifndef BUFFER_H_
#include "Buffer.h"
#endif
#ifndef SCANNER_H_
#include "Scanner.h"
#endif

/* Global vars */
static Token lookahead;
india_int syntaxErrorNumber = 0;
extern Buffer* stringLiteralTable;
extern Buffer* literalReaderTable;
extern india_int line;
extern Token tokenizer();
extern india_chr* keywordTable[];

#define STR_LANGNAME "India"

/* TO_DO: Create ALL constants for keywords (sequence given in table.h) */

/* Constants */
/*YES and NO*/
enum KEYWORDS {
	NO_ATTR = -1,
	WHILE,
	IF,
	ELIF,
	DO,
	PRINTF,
	ELSE,
	SCANF,
	INT,
	FINT,
	BOOLEAN,
	STRING,
	YESS,
	NOO
};

/* Function definitions */
india_nul startParser();
india_nul matchToken(india_int, india_int);
india_nul syncErrorHandler(india_int);
india_nul printError();

india_nul program();
india_nul optVarListDeclarations();
india_nul varListDeclarations();
india_nul varListDeclaration();
india_nul varListDeclarationsPrime();

india_nul intVarListDeclaration();
india_nul fintVarListDeclaration();
india_nul stringVarListDeclaration();
india_nul boolVarListDeclaration();

india_nul intVarList();
india_nul intVarListPrime();

india_nul fintVarList();
india_nul fintVarListPrime();

india_nul stringVarList();
india_nul stringVarListPrime();

india_nul boolVarList();
india_nul boolVarListPrime();

india_nul optionalStatements();

/* TODO: Place ALL non-terminal function declarations */
india_nul assignmentExpression();
india_nul assignmentStatement();

india_nul outputStatement();

india_nul outputVariableList();

india_nul specifierListPrime();
india_nul specifierList();
india_nul specifier();

india_nul statement();

india_nul inputStatement();

india_nul arithmeticExpression();
india_nul unaryArithmeticExpression();
india_nul primaryArithmeticExpression();

india_nul multiplicativeArithmeticExpression();
india_nul multiplicativeArithmeticExpressionPrime();

india_nul additiveArithmeticExpression();
india_nul additiveArithmeticExpressionPrime();

india_nul statements();
india_nul statementsPrime();

india_nul booleanDecExpression();

india_nul varList();
india_nul varIdentifier();
india_nul varListPrime();

india_nul stringExpression();
india_nul stringExpressionPrime();
india_nul primaryStringExpression();
#endif
