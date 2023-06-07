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
* File name: scanner.h
* Compiler: MS Visual Studio 2022
* Author: Paulo Sousa
* Course: CST 8152 � Compilers, Lab Section: [011, 012, 013, 014]
* Updated by: Krushang Patel and VandanKumar Patel
* Assignment: A22, A32.
* Date: March 19,2022
* Professor: Paulo Sousa
* Purpose: This file contains all functionalities from Scanner.
* Function list: startScanner(), tokenizer(), nextState(), nextClass()
*				 funcID(), funcKey(), funcFPV(), funcSL(), funcIV()
*		 funcErr(), funcPrintToken()
* References
* 1. Professor Paulo Sousa's code examples
* 2. Git hub - github.com/PrinceAFelix/CST8152-Compilers/blob/main/Assignment%202/scanner.c
************************************************************
*/
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>   /* standard input / output */
#include <ctype.h>   /* conversion functions */
#include <stdlib.h>  /* standard library functions and constants */
#include <string.h>  /* string functions */
#include <limits.h>  /* integer types constants */
#include <float.h>   /* floating-point types constants */

/* #define NDEBUG        to suppress assert() call */
#include <assert.h>  /* assert() prototype */

/* project header files */

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef BUFFER_H_
#include "Buffer.h"
#endif

#ifndef SCANNER_H_
#include "Scanner.h"
#endif

/*
----------------------------------------------------------------
TO_DO: Global vars definitions
----------------------------------------------------------------
*/

/* Global objects - variables */
/* This buffer is used as a repository for string literals. */
extern BufferPointer stringLiteralTable;	/* String literal table */
extern BufferPointer literalReaderTable;	/* String literal table */
india_int line;								/* Current line number of the source code */
extern india_int errorNumber;				/* Defined in platy_st.c - run-time error number */

extern india_int stateType[];
extern india_chr* keywordTable[];
extern india_chr* ArithOperatorsList[];
extern india_chr* RelOperatorsList[];
extern india_chr* LogiOperatorsList[];
extern india_chr* BracketList[];
extern PTR_ACCFUN finalStateTable[];
extern india_int transitionTable[][TABLE_COLUMNS];

/* Local(file) global objects - variables */
static BufferPointer lexemeBuffer;			/* Pointer to temporary lexeme buffer */
static BufferPointer sourceBuffer;			/* Pointer to input source buffer */

/*
 ************************************************************
 * Intitializes scanner
 *		This function initializes the scanner using defensive programming.
 *
 * Written by: Krushang Patel and Vandankumar Patel
 ***********************************************************
 */
india_int startScanner(BufferPointer psc_buf) {

	/* in case the buffer has been read previously  */
	bRecover(psc_buf);
	bClear(stringLiteralTable);
	bClear(literalReaderTable);
	line = 1;
	sourceBuffer = psc_buf;
	return EXIT_SUCCESS; /*0*/
}

/*
 ************************************************************
 * Process Token
 *		Main function of buffer, responsible to classify a char (or sequence
 *		of chars). In the first part, a specific sequence is detected (reading
 *		from buffer). In the second part, a pattern (defined by Regular Expression)
 *		is recognized and the appropriate function is called (related to final states
 *		in the Transition Diagram).
 *
 * Written by: Krushang Patel and Vandankumar Patel
 ***********************************************************
 */
Token tokenizer(void) {

	/* TO_DO: Follow the standard and adjust datatypes */
	Token currentToken = { 0 }; /* token to return after pattern recognition. Set all structure members to 0 */
	india_chr c;				/* input symbol */
	india_int state = 0;		/* initial state of the FSM */
	india_int lexStart;			/* start offset of a lexeme in the input char buffer (array) */
	india_int lexEnd;			/* end offset of a lexeme in the input char buffer (array)*/

	india_int lexLength;		/* token length */
	india_int i;				/* counter */
	india_chr newc;				/* new char */

	while (1) { /* endless loop broken by token returns it will generate a warning */
		c = bGetChar(sourceBuffer);
		int pos = 0;
		/* ------------------------------------------------------------------------
			Part 1: Implementation of token driven scanner.
			Every token is possessed by its own dedicated code
			-----------------------------------------------------------------------
		*/

		/* TO_DO: All patterns that do not require accepting functions */
		switch (c) {

			/* Cases for spaces */
			case ' ':
			case '\t':
			case '\f':
				break;
			case '\n':
				line++;
				break;
			/* Cases for symbols */
			case ';':
				currentToken.code = EOS_T;
				return currentToken;
				break;
			case '(':
				currentToken.code = LPR_T;
				return currentToken;
				break;
			case ')':
				currentToken.code = RPR_T;
				return currentToken;
				break;
			case '{':
				currentToken.code = LBR_T;
				return currentToken;
				break;
			case '}':
				currentToken.code = RBR_T;
				return currentToken;
				break;
			case '[':
				currentToken.code = LSB_T;
				return currentToken;
				break;
			case ']':
				currentToken.code = RSB_T;
				return currentToken;
				break;
			case ',':
				currentToken.code = COMMA_T;
				return currentToken;
				break;
			/*Arithmethic operator (+ - * /) and String Concatenation (++)*/
			case '+':
				newc = bGetChar(sourceBuffer);
				if (newc == '+') { /* ++ */
					currentToken.code = SCO_T;
					return currentToken;
				}
				else {
					bRetract(sourceBuffer);
					currentToken.code = ARTHO_T;
					currentToken.attribute.arithmeticOperator = OP_ADD;
					return currentToken;
				}
			case '-':
				currentToken.code = ARTHO_T;
				currentToken.attribute.arithmeticOperator = OP_SUB;
				return currentToken;
				break;
			case '*':
				currentToken.code = ARTHO_T;
				currentToken.attribute.arithmeticOperator = OP_MUL;
				return currentToken;
				break;
			case '/':
				currentToken.code = ARTHO_T;
				currentToken.attribute.arithmeticOperator = OP_DIV;
				return currentToken;
				break;
			/*Relational Operator (== =<), Arithmethic operator (=%) and Assignment Operator (=)*/
			case '=':
				newc = bGetChar(sourceBuffer);
				if (newc == '=') { /* == */
					currentToken.code = RELO_T;
					currentToken.attribute.relationalOperator = OP_EQ;
					return currentToken;
				}
				else if (newc == '<') { /* =< */
					currentToken.code = RELO_T;
					currentToken.attribute.relationalOperator = OP_LTE;
					return currentToken;
				}
				else if (newc == '%') { /* =% */
					currentToken.code = ARTHO_T;
					currentToken.attribute.arithmeticOperator = OP_MOD;
					return currentToken;
				}
				else {
					bRetract(sourceBuffer);
					currentToken.code = ASSO_T;
					return currentToken;
				}
				break;
			case '<':
				currentToken.code = RELO_T;
				currentToken.attribute.relationalOperator = OP_LT;
				return currentToken;
				break;
			case '!': /* ! != */
				newc = bGetChar(sourceBuffer);
				if (newc == '=') { /* != */
					currentToken.code = RELO_T;
					currentToken.attribute.relationalOperator = OP_NE;
					return currentToken;
				}
				else {
					bRetract(sourceBuffer);
					currentToken.code = LOGO_T;
					currentToken.attribute.logicalOperator = OP_NOT;
					return currentToken;
				}
				break;
			case '>':
				newc = bGetChar(sourceBuffer);
				if (newc == '=') { /* >= */
					currentToken.code = RELO_T;
					currentToken.attribute.relationalOperator = OP_GTE;
					return currentToken;
				}
				else {
					bRetract(sourceBuffer);
					currentToken.code = RELO_T;
					currentToken.attribute.relationalOperator = OP_GT;
					return currentToken;
				}
				break;
			/* Logical operators ( && || ! )*/
			case '&':
				newc = bGetChar(sourceBuffer);
				if (newc == '&') { /* && */
					currentToken.code = LOGO_T;
					currentToken.attribute.logicalOperator = OP_AND;
					return currentToken;
				}
				else {
					bRetract(sourceBuffer);
					currentToken.code = ERR_T;
					currentToken.attribute.errLexeme[0] = '&';
					currentToken.attribute.errLexeme[1] = '\0';
					return currentToken;
				}
				break;
			case '|':
				newc = bGetChar(sourceBuffer);
				if (newc == '|') { /* || */
					currentToken.code = LOGO_T;
					currentToken.attribute.logicalOperator = OP_OR;
					return currentToken;
				}
				else {
					bRetract(sourceBuffer);
					currentToken.code = ERR_T;
					currentToken.attribute.errLexeme[0] = '|';
					currentToken.attribute.errLexeme[1] = '\0';
					return currentToken;
				}
				break;
			case '^':
				newc = bGetChar(sourceBuffer);
				do {
					c = bGetChar(sourceBuffer);
					if (c == CHARSEOF0 || c == CHARSEOF255) {
						bRetract(sourceBuffer);
					}
					else if (c == '\n') {
						line++;
					}
				} while (c != '^' && c != CHARSEOF0 && c != CHARSEOF255);
				break;
			/* Cases for END OF FILE */
			case CHARSEOF0:
				currentToken.code = SEOF_T;
				currentToken.attribute.seofType = SEOF_0;
				return currentToken;
				break;
			case CHARSEOF255:
				currentToken.code = SEOF_T;
				currentToken.attribute.seofType = SEOF_255;
				return currentToken;
				break;
			default: /* general case */
				state = nextState(state, c);
				lexStart = bGetReadPos(sourceBuffer) - 1;
				bSetMark(sourceBuffer, lexStart);

				while (stateType[state] == NOAS) {
					c = bGetChar(sourceBuffer);
					state = nextState(state, c);
					pos++;
				}
				if (stateType[state] == ASWR)
					bRetract(sourceBuffer);
				lexEnd = bGetReadPos(sourceBuffer);
				lexLength = lexEnd - lexStart;
				lexemeBuffer = bCreate((short)lexLength + 2, 0, MODE_FIXED);
				if (!lexemeBuffer) {
					fprintf(stderr, "Scanner error: Can not create buffer\n");
					exit(1);
				}
				bRestore(sourceBuffer);
				for (i = 0; i < lexLength; i++)
					bAddChar(lexemeBuffer, bGetChar(sourceBuffer));
				bAddChar(lexemeBuffer, BUFFER_EOF);
				currentToken = (*finalStateTable[state])(bGetContent(lexemeBuffer, 0));
				bDestroy(lexemeBuffer);
				return currentToken;
		} /* switch */
	} /* while */
} /* tokenizer */

/*
 ************************************************************
 * Get Next State
	The assert(int test) macro can be used to add run-time diagnostic to programs
	and to "defend" from producing unexpected results.
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	(*) assert() is a macro that expands to an if statement;
	if test evaluates to false (zero) , assert aborts the program
	(by calling abort()) and sends the following message on stderr:
	(*) Assertion failed: test, file filename, line linenum.
	The filename and linenum listed in the message are the source file name
	and line number where the assert macro appears.
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	If you place the #define NDEBUG directive ("no debugging")
	in the source code before the #include <assert.h> directive,
	the effect is to comment out the assert statement.
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	The other way to include diagnostics in a program is to use
	conditional preprocessing as shown bellow. It allows the programmer
	to send more details describing the run-time problem.
	Once the program is tested thoroughly #define DEBUG is commented out
	or #undef DEBUG is used - see the top of the file.

 * Written by: Krushang Patel and Vandankumar Patel
 ***********************************************************
 */
india_int nextState(india_int state, india_chr c) {
	india_int col;
	india_int next;
	col = nextClass(c);
	next = transitionTable[state][col];
	if (DEBUG)
		printf("Input symbol: %c Row: %d Column: %d Next: %d \n", c, state, col, next);
	assert(next != IS);
	if (DEBUG)
		if (next == IS) {
			printf("Scanner Error: Illegal state:\n");
			printf("Input symbol: %c Row: %d Column: %d\n", c, state, col);
			exit(1);
		}
	return next;
}

/*
 ************************************************************
 * Get Next Token Class
	* Create a function to return the column number in the transition table:
	* Considering an input char c, you can identify the "class".
	* For instance, a letter should return the column for letters, etc.
	* Written by: Krushang Patel and Vandankumar Patel
 ***********************************************************
 */
static india_int nextClass(india_chr c) {
	india_int val = -1;
	switch (c) {
	case CHRCOL2:
		val = 2;
		break;
	case CHRCOL3:
		val = 3;
		break;
	case CHRCOL4:
		val = 4;
		break;
	case CHRCOL5:
		val = 5;
		break;
	case CHRCOL6:
		val = 6;
		break;
	case CHRCOL7:
		val = 7;
		break;
	case CHRCOL8:
		val = 8;
		break;
	case CHRCOL9:
		val = 9;
		break;
	case CHARSEOF0:
	case CHARSEOF255:
		val = 10;
		break;
	default:
		if (isalpha(c))
			val = 0;
		else if (isdigit(c))
			val = 1;
		else
			val = 11;
	}
	return val;
}

/*
 ************************************************************
 * Acceptance State Function ID
 *		In this function, the pattern for IDs must be recognized.
 *		Since keywords obey the same pattern, is required to test if
 *		the current lexeme matches with KW from language.
 *	- Remember to respect the limit defined for lexemes (VID_LEN) and
 *	  set the lexeme to the corresponding attribute (vidLexeme).
 *    Remember to end each token with the \0.
 *    Written by: Krushang Patel and Vandankumar Patel
 ***********************************************************
 */
Token funcID(india_chr lexeme[]) {
	Token currentToken = { 0 };
	size_t length = strlen(lexeme);
	india_chr lastch = lexeme[length - 1];
	india_int isID = NO;
	switch (lastch) {
	case MVIDPREFIX:
		currentToken.code = MVID_T;
		isID = YES;
		break;
	case IVIDPREFIX:
		currentToken.code = IVID_T;
		isID = YES;
		break;
	case FVIDPREFIX:
		currentToken.code = FVID_T;
		isID = YES;
		break;
	case SVIDPREFIX:
		currentToken.code = SVID_T;
		isID = YES;
		break;
	case BVIDPREFIX:
		currentToken.code = BVID_T;
		isID = YES;
		break;
	default:
		/* Test Keyword */
		currentToken = funcKEY(lexeme);
		break;
	}
	if (isID == YES) {
		strncpy(currentToken.attribute.idLexeme, lexeme, VID_LEN);
		currentToken.attribute.idLexeme[VID_LEN] = CHARSEOF0;
	}
	return currentToken;
}

/*
************************************************************
 * This function checks if one specific lexeme is a keyword.
 * Written by: Krushang Patel and Vandankumar Patel
 ***********************************************************
 */
Token funcKEY(india_chr lexeme[]) {
	Token currentToken = { 0 };
	india_int kwindex = -1, j = 0;
	for (j = 0; j < KWT_SIZE; j++)
		if (!strcmp(lexeme, &keywordTable[j][0]))
			kwindex = j;
	if (kwindex != -1) {
		currentToken.code = KEY_T;
		currentToken.attribute.codeType = kwindex;
	}
	else {
		currentToken = funcErr(lexeme);
	}
	return currentToken;
}

/*
************************************************************
 * This function checks if one specific lexeme is an integer value.
 * Written by: Krushang Patel and Vandankumar Patel
 ***********************************************************
 */
Token funcIV(india_chr lexeme[]) {

	Token currentToken = { 0 };

	india_intL strInt;

	if (lexeme[0] != '\0' && strlen(lexeme) > NUM_LEN) {
		currentToken = (*finalStateTable[ES])(lexeme);
	}
	else {
		strInt = atol(lexeme);
		if (strInt >= 0 && strInt <= SHRT_MAX) {
			currentToken.code = INTL_T;
			currentToken.attribute.intValue = (india_int)strInt;
		}
		else {
			currentToken = (*finalStateTable[ES])(lexeme);
		}
	}
	return currentToken;
}

/*
****************************************************************************
 * This function checks if one specific lexeme is a floating point value.
 * Written by: Krushang Patel and Vandankumar Patel
 ***************************************************************************
 */
Token funcFPV(india_chr lexeme[]) {

	Token currentToken = { 0 };

	india_dint strdouble = atof(lexeme);
	if (strdouble == 0.0 || strdouble >= FLT_MIN && strdouble <= FLT_MAX) {
		currentToken.code = FPVL_T;
		currentToken.attribute.floatValue = (india_fint)strdouble;
	}
	else {
		currentToken = (*finalStateTable[ES])(lexeme);
	}
	return currentToken;
}

/*
************************************************************
 * Acceptance State Function SL
 *		Function responsible to identify SL (string literals).
 * - The lexeme must be stored in the String Literal Table
 *   (stringLiteralTable). You need to include the literals in
 *   this structure, using offsets. Remember to include \0 to
 *   separate the lexemes. Remember also to incremente the line.
 *   Written by: Krushang Patel and Vandankumar Patel
 ***********************************************************
 */
Token funcSL(india_chr lexeme[]) {
	Token currentToken = { 0 };
	india_int i = 0, len = (india_int)strlen(lexeme);

	/*%s, %d, %f, %g, %l, %c, SL*/
	if ((strcmp(&lexeme[1], "%s\"") == 0) && (strcmp(&lexeme[2], "s\"") == 0)) {

		currentToken.attribute.contentString = bGetWritePos(literalReaderTable);
		for (i = 1; i < len - 1; i++) {
			if (lexeme[i] == '\n')
				line++;
			if (!bAddChar(literalReaderTable, lexeme[i])) {
				currentToken.code = RTE_T;
				strcpy(currentToken.attribute.errLexeme, "Run Time Error:");
				errorNumber = RTE_CODE;
				return currentToken;
			}
		}
		if (!bAddChar(literalReaderTable, CHARSEOF0)) {
			currentToken.code = RTE_T;
			strcpy(currentToken.attribute.errLexeme, "Run Time Error:");
			errorNumber = RTE_CODE;
			return currentToken;
		}
		currentToken.code = SLR_T;
	}
	else if ((strcmp(&lexeme[1], "%d\"") == 0) && (strcmp(&lexeme[2], "d\"") == 0)) {

		currentToken.attribute.contentString = bGetWritePos(literalReaderTable);
		for (i = 1; i < len - 1; i++) {
			if (lexeme[i] == '\n')
				line++;
			if (!bAddChar(literalReaderTable, lexeme[i])) {
				currentToken.code = RTE_T;
				strcpy(currentToken.attribute.errLexeme, "Run Time Error:");
				errorNumber = RTE_CODE;
				return currentToken;
			}
		}
		if (!bAddChar(literalReaderTable, CHARSEOF0)) {
			currentToken.code = RTE_T;
			strcpy(currentToken.attribute.errLexeme, "Run Time Error:");
			errorNumber = RTE_CODE;
			return currentToken;
		}
		currentToken.code = INTR_T;
	}
	else if ((strcmp(&lexeme[1], "%f\"") == 0) && (strcmp(&lexeme[2], "f\"") == 0)) {

		currentToken.attribute.contentString = bGetWritePos(literalReaderTable);
		for (i = 1; i < len - 1; i++) {
			if (lexeme[i] == '\n')
				line++;
			if (!bAddChar(literalReaderTable, lexeme[i])) {
				currentToken.code = RTE_T;
				strcpy(currentToken.attribute.errLexeme, "Run Time Error:");
				errorNumber = RTE_CODE;
				return currentToken;
			}
		}
		if (!bAddChar(literalReaderTable, CHARSEOF0)) {
			currentToken.code = RTE_T;
			strcpy(currentToken.attribute.errLexeme, "Run Time Error:");
			errorNumber = RTE_CODE;
			return currentToken;
		}
		currentToken.code = FDINTR_T;
	}
	else if ((strcmp(&lexeme[1], "%b\"") == 0) && (strcmp(&lexeme[2], "b\"") == 0)) {

		currentToken.attribute.contentString = bGetWritePos(literalReaderTable);
		for (i = 1; i < len - 1; i++) {
			if (lexeme[i] == '\n')
				line++;
			if (!bAddChar(literalReaderTable, lexeme[i])) {
				currentToken.code = RTE_T;
				strcpy(currentToken.attribute.errLexeme, "Run Time Error:");
				errorNumber = RTE_CODE;
				return currentToken;
			}
		}
		if (!bAddChar(literalReaderTable, CHARSEOF0)) {
			currentToken.code = RTE_T;
			strcpy(currentToken.attribute.errLexeme, "Run Time Error:");
			errorNumber = RTE_CODE;
			return currentToken;
		}
		currentToken.code = BOOLR_T;
	}
	else {

		currentToken.attribute.contentString = bGetWritePos(stringLiteralTable);
		for (i = 1; i < len - 1; i++) {
			if (lexeme[i] == '\n')
				line++;
			if (!bAddChar(stringLiteralTable, lexeme[i])) {
				currentToken.code = RTE_T;
				strcpy(currentToken.attribute.errLexeme, "Run Time Error:");
				errorNumber = RTE_CODE;
				return currentToken;
			}
		}
		if (!bAddChar(stringLiteralTable, CHARSEOF0)) {
			currentToken.code = RTE_T;
			strcpy(currentToken.attribute.errLexeme, "Run Time Error:");
			errorNumber = RTE_CODE;
			return currentToken;
		}
		currentToken.code = SL_T;
	}
	return currentToken;
}

/*
************************************************************
 * Acceptance State Function Error
 *		Function responsible to deal with ERR token.
 * - This function uses the errLexeme, respecting the limit given
 *   by ERR_LEN. If necessary, use three dots (...) to use the
 *   limit defined. The error lexeme contains line terminators,
 *   so remember to increment line.
 *
 * Written by : Krushang Patel and Vandankumar Patel
 ***********************************************************
 */
Token funcErr(india_chr lexeme[]) {
	Token currentToken = { 0 };
	india_int i = 0, len = (india_int)strlen(lexeme);
	if (len > ERR_LEN) {
		strncpy(currentToken.attribute.errLexeme, lexeme, ERR_LEN - 3);
		currentToken.attribute.errLexeme[ERR_LEN - 3] = CHARSEOF0;
		strcat(currentToken.attribute.errLexeme, "...");
	}
	else {
		strcpy(currentToken.attribute.errLexeme, lexeme);
	}
	for (i = 0; i < len; i++)
		if (lexeme[i] == '\n')
			line++;
	currentToken.code = ERR_T;
	return currentToken;
}

/*
 ************************************************************
 * The function prints the token returned by the scanner
 * Written by: Krushang Patel and Vandankumar Patel
 ***********************************************************
 */
india_nul printToken(Token t) {
	extern india_chr* keywordTable[]; /* link to keyword table in */
	extern india_chr* ArithOperatorsList[];
	extern india_chr* RelOperatorsList[];
	extern india_chr* LogiOperatorsList[];
	extern india_chr* BracketList[];

	switch (t.code) {
	case RTE_T:
		printf("%-70s %-70s\n\n", "RTE_T", t.attribute.errLexeme);
		/* Call here run-time error handling component */
		if (errorNumber) {
			printf("%d", errorNumber);
			exit(errorNumber);
		}
		printf("\n\n");
		break;
	case ERR_T:
		printf("%-69s %-70s\n\n", "ERR_T", t.attribute.errLexeme);
		break;
	case LSB_T:
		printf("%-69s %-70s\n\n", "LSB_T", BracketList[4]);
		break;
	case RSB_T:
		printf("%-69s %-70s\n\n", "RSB_T", BracketList[5]);
		break;
	case LPR_T:
		printf("%-69s %-70s\n\n", "LPR_T", BracketList[0]);
		break;
	case RPR_T:
		printf("%-69s %-70s\n\n", "RPR_T", BracketList[1]);
		break;
	case LBR_T:
		printf("%-70s %-70s\n\n", "LBR_T", BracketList[2]);
		break;
	case RBR_T:
		printf("%-69s %-70s\n\n", "RBR_T", BracketList[3]);
		break;
	case EOS_T:
		printf("%-69s %-70s\n\n", "EOS_T", ";");
		break;
	case SEOF_T:
		printf("%-67s %-50d\n\n", "SEOF_T", t.attribute.seofType);
		break;
	case SCO_T:
		printf("%-69s %-70s\n\n", "SCO_T", "++");
		break;
	case ASSO_T:
		printf("%-68s %-70s\n\n", "ASSO_T", "=");
		break;
	case ARTHO_T:
		if (t.attribute.arithmeticOperator == OP_ADD) {
			printf("%-57s %-70s\n\n", "ARTHO_T (OP_ADD)", ArithOperatorsList[OP_ADD]);
		}
		else if (t.attribute.arithmeticOperator == OP_SUB) {
			printf("%-57s %-70s\n\n", "ARTHO_T (OP_SUB)", ArithOperatorsList[OP_SUB]);
		}
		else if (t.attribute.arithmeticOperator == OP_MUL) {
			printf("%-56s %-70s\n\n", "ARTHO_T (OP_MUL)", ArithOperatorsList[OP_MUL]);
		}
		else if (t.attribute.arithmeticOperator == OP_DIV) {
			printf("%-58s %-70s\n\n", "ARTHO_T (OP_DIV)", ArithOperatorsList[OP_DIV]);
		}
		else if (t.attribute.arithmeticOperator == OP_MOD) {
			printf("%-56s %-70s\n\n", "ARTHO_T (OP_MOD)", ArithOperatorsList[OP_MOD]);
		}
		break;
	case RELO_T:
		if (t.attribute.relationalOperator == OP_EQ) {
			printf("%-48s %-70s\n\n", "RELO_T (Relational operator) (OP_EQ)", RelOperatorsList[OP_EQ]);
		}
		else if (t.attribute.relationalOperator == OP_NE) {
			printf("%-48s %-70s\n\n", "RELO_T (Relational operator) (OP_NE)", RelOperatorsList[OP_NE]);
		}
		else if (t.attribute.relationalOperator == OP_GT) {
			printf("%-49s %-70s\n\n", "RELO_T (Relational operator) (OP_GT)", RelOperatorsList[OP_GT]);
		}
		else if (t.attribute.relationalOperator == OP_GTE) {
			printf("%-48s %-70s\n\n", "RELO_T (Relational operator) (OP_GTE)", RelOperatorsList[OP_GTE]);
		}
		else if (t.attribute.relationalOperator == OP_LT) {
			printf("%-50s %-70s\n\n", "RELO_T (Relational operator) (OP_LT)", RelOperatorsList[OP_LT]);
		}
		else if (t.attribute.relationalOperator == OP_LTE) {
			printf("%-49s %-70s\n\n", "RELO_T (Relational operator) (OP_LTE)", RelOperatorsList[OP_LTE]);
		}
		break;
	case LOGO_T:
		if (t.attribute.logicalOperator == OP_AND) {
			printf("%-47s %-70s\n\n", "LOGO_T (Logical operator) (OP_AND)", LogiOperatorsList[OP_AND]);
		}
		else if (t.attribute.logicalOperator == OP_OR) {
			printf("%-49s %-70s\n\n", "LOGO_T (Logical operator) (OP_OR)", LogiOperatorsList[OP_OR]);
		}
		else if (t.attribute.logicalOperator == OP_NOT) {
			printf("%-47s %-70s\n\n", "LOGO_T (Logical operator) (OP_NOT)", LogiOperatorsList[OP_NOT]);
		}
		break;
	case COMMA_T:
		printf("%-65s %-70s\n\n", "COMMA_T", ",");
		break;
	case INTL_T:
		printf("%-64s %-50d\n\n", "INTL_T (Integer literal)", t.attribute.codeType);
		break;
	case FPVL_T:
		printf("%-60s %-70g\n\n", "FPVL_T (Floating point literal)", t.attribute.floatValue);
		break;
	case SL_T:
		printf("%-66s %-7d %-60s\n\n", "SL_T (String literal)", (short)t.attribute.codeType, bGetContent(stringLiteralTable, (short)t.attribute.codeType));
		break;
	case SLR_T:
		printf("%-70s %-70s\n\n", "SLR_T", bGetContent(literalReaderTable, (short)t.attribute.codeType));
		break;
	case INTR_T:
		printf("%-69s %-70s\n\n", "INTR_T", bGetContent(literalReaderTable, (short)t.attribute.codeType));
		break;
	case FDINTR_T:
		printf("%-66s %-70s\n\n", "FDINTR_T", bGetContent(literalReaderTable, (short)t.attribute.codeType));
		break;
	case BOOLR_T:
		printf("%-66s %-70s\n\n", "BOOLR_T", bGetContent(literalReaderTable, (short)t.attribute.codeType));
		break;
	case IVID_T:
		printf("%-71s %-70s\n\n", "IVID_T", t.attribute.idLexeme);
		break;
	case FVID_T:
		printf("%-70s %-70s\n\n", "FVID_T", t.attribute.idLexeme);
		break;
	case SVID_T:
		printf("%-69s %-70s\n\n", "SVID_T", t.attribute.idLexeme);
		break;
	case BVID_T:
		printf("%-70s %-70s\n\n", "BVID_T", t.attribute.idLexeme);
		break;
	case MVID_T:
		printf("%-69s %-70s\n\n", "MVID_T", t.attribute.idLexeme);
		break;
	case KEY_T:
		printf("%-70s %-70s\n\n", "KEY_T", keywordTable[t.attribute.codeType]);
		break;
	default:
		printf("%-70s %-70d\n\n", "Scanner error: invalid token code:", t.code);
	}
}
