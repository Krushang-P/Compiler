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
* File name: Scanner.h
* Compiler: MS Visual Studio 2022
* Author: Krushang Patel and Vandankumar Patel
* Course: CST 8152 � Compilers, Lab Section: [011]
* Assignment: A22.
* Date: March 19 2022
* Purpose: This file is the main header for Scanner (.h)
* Function list: (...).
*************************************************************/

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef SCANNER_H_
#define SCANNER_H_

#ifndef NULL
#include <_null.h> /* NULL pointer constant is defined there */
#endif

/*#pragma warning(1:4001) */	/*to enforce C89 type comments  - to make //comments an warning */

/*#pragma warning(error:4001)*/	/* to enforce C89 comments - to make // comments an error */

/* Constants */
#define VID_LEN 20   /* variable identifier length */
#define ERR_LEN 40   /* error message length */
#define NUM_LEN 5    /* maximum number of digits for IL */

#define RTE_CODE 1   /* Value for run-time error */

/* TO_DO: Define Token codes - Create your token classes */
enum TOKENS {
	ERR_T,		/*  0: Error token */
	IVID_T,		/*	1: IVID Delimitator */
	FVID_T,		/*	2: FVID Delimitator */
	SVID_T,		/*	3: SVID Delimitator */
	BVID_T,		/*	4: BVID Delimitator */
	MVID_T,		/*	5: MVID Delimitator */
	INTL_T,		/*  6: Integer literal token */
	FPVL_T,		/*  7: Floating point literal token */
	SL_T,		/*  8: String literal token */
	LSB_T,		/*	9: Left squre bracket token [ */
	RSB_T,		/*	10: Right squre bracket token ] */
	LPR_T,		/*  11: Left parenthesis token ( */
	RPR_T,		/*  12: Right parenthesis token ) */
	LBR_T,		/*  13: Left brace token { */
	RBR_T,		/*  14: Right brace token } */
	EOS_T,		/*  15: End of statement (semicolon) ;*/
	RTE_T,		/*  16: Run-time error token */
	SEOF_T,		/*  17: Source end-of-file token */
	SCO_T,		/*  18: String concatenation operator token (++)*/
	ASSO_T,		/*  19: Assignment operator token (=)*/
	ARTHO_T,	/*  20: Arithmetic operator token (+ - * / =%) */
	RELO_T,		/*	21: Relational operator token (== != > >= < =<)*/
	LOGO_T,		/*  22: Logical operator token ( && || ! )*/
	KEY_T,		/*	23:	Keywords Token*/
	COMMA_T,	/*	24: Comma(,) Token */
	SLR_T,		/*	25: String specifier (%s) */
	INTR_T,		/*  26: Integer specifier (%d) */
	FDINTR_T,	/*  27: Float specifier (%f) */
	BOOLR_T,	/*  28: Boolean specifier (%b)*/
};

/* TO_DO: Operators token attributes */
typedef enum ArithmeticOperators { OP_ADD, OP_SUB, OP_MUL, OP_DIV, OP_MOD } AriOperator;
typedef enum RelationalOperators { OP_EQ, OP_NE, OP_GT, OP_GTE, OP_LT, OP_LTE } RelOperator;
typedef enum LogicalOperators { OP_AND, OP_OR, OP_NOT } LogOperator;
typedef enum SourceEndOfFile { SEOF_0, SEOF_255 } EofOperator;

/*For printing arithmatic operators*/
static india_chr* ArithOperatorsList[5] = {
	"+","-","*","/","=%"
};

/*For printing relational operators*/
static india_chr* RelOperatorsList[6] = {
	"==", "!=", ">", ">=", "<" , "=<"
};

/*For printing logicle operators*/
static india_chr* LogiOperatorsList[3] = {
	"&&", "||", "!"
};

/*For printing brackets*/
static india_chr* BracketList[6] = {
	"(", ")", "{", "}", "[" , "]"
};

/* TO_DO: Data structures for declaring the token and its attributes */
typedef union TokenAttribute {
	india_int codeType;						/* integer attributes accessor */
	AriOperator arithmeticOperator;			/* arithmetic operator attribute code */
	RelOperator relationalOperator;			/* relational operator attribute code */
	LogOperator logicalOperator;			/* logical operator attribute code */
	EofOperator seofType;					/* source-end-of-file attribute code */
	india_int intValue;						/* integer literal attribute (value) */
	india_int keywordIndex;					/* keyword index in the keyword table */
	india_int contentString;				/* string literal offset from the beginning of the string literal buffer (stringLiteralTable->content) */
	india_fint floatValue;					/* floating-point literal attribute (value) */
	india_chr idLexeme[VID_LEN + 1];		/* variable identifier token attribute */
	india_chr errLexeme[ERR_LEN + 1];		/* error token attribite */
} TokenAttribute;

/* TO_DO: Should be used if no symbol table is implemented */
typedef struct idAttibutes {
	india_flg flags;					/* Flags information */
	union {
		india_int intValue;				/* Integer value */
		india_fint floatValue;			/* Float value */
		india_nul* stringContent;		/* String value */
	} values;
} IdAttibutes;

/* Token declaration */
typedef struct Token {
	india_int code;				/* token code */
	TokenAttribute attribute;	/* token attribute */
	IdAttibutes   idAttribute;	/* not used in this scanner implementation - for further use */
} Token;

/* EOF definitions */
#define CHARSEOF0 '\0'
#define CHARSEOF255 0xFF

/*  Special case tokens processed separately one by one in the token-driven part of the scanner
 *  '=' , ' ' , '(' , ')' , '{' , '}' , == , <> , '>' , '<' , ';',
 *  white space, @comment@ , ',' , ';' , '-' , '+' , '*' , '/', ## ,
 *  .&., .|. , .!. , SEOF.
 */

 /* TO_DO: Error states and illegal state */
#define ES  6		/* Error state with no retract */
#define ER  7		/* Error state with retract */
#define IS -1		/* Illegal state */

 /* TO_DO: State transition table definition */
#define TABLE_COLUMNS 12

/* TO_DO: Define lexeme FIXED classes */
/* These constants will be used on nextClass */
#define CHRCOL2 '_'		/*U(_)*/
#define CHRCOL3 '.'		/*P(.)*/
#define CHRCOL4 '@'		/*A(@)*/
#define CHRCOL5 '$'		/*F($)*/
#define CHRCOL6 '&'		/*S(&)*/
#define CHRCOL7 '?'		/*B(?)*/
#define CHRCOL8 '~'		/*T(~)*/
#define CHRCOL9 '\"'	/*Q(")*/

/* These constants will be used on VID / MID function */
#define IVIDPREFIX '@'		/*	1: IVID Delimitator */
#define FVIDPREFIX '$'		/*	2: FVID Delimitator */
#define SVIDPREFIX '&'		/*	3: SVID Delimitator */
#define BVIDPREFIX '?'		/*	4: BVID Delimitator */
#define MVIDPREFIX '~'		/*	5: MVID Delimitator */

/* TO_DO: Transition table - type of states defined in separate table */
static india_int transitionTable[][TABLE_COLUMNS] = {
	/*		[A-z], [0-9], _ ,   .   , @    , $    , &  ,  ?  ,    ~   , "    , EOFS , Other
			L(0),  D(1), U(2), P(3), A(4), F(5),  S(6),  B(7),  T(8),  Q(9),  E1(10) O(11)  */
		{     1,    8,    ES,   ES,   ES,   ES,    ES,    ES,    ES,    13,    ER,    ES}, /* S0:	NOAS			*/
		{     1,    1,     1,    7,    2,    3,     4,     5,     6,     7,    ER,     7}, /* S1:	NOAS 			*/
		{    IS,   IS,    IS,   IS,   IS,   IS,    IS,    IS,    IS,    IS,    IS,    IS}, /* S2:	ASNR (IVID)		*/
		{    IS,   IS,    IS,   IS,   IS,   IS,    IS,    IS,    IS,    IS,    IS,    IS}, /* S3:	ASNR (FVID)		*/
		{    IS,   IS,    IS,   IS,   IS,   IS,    IS,    IS,    IS,    IS,    IS,    IS}, /* S4:	ASNR (SVID)		*/
		{    IS,   IS,    IS,   IS,   IS,   IS,    IS,    IS,    IS,    IS,    IS,    IS}, /* S5:	ASNR (BVID)		*/
		{    IS,   IS,    IS,   IS,   IS,   IS,    IS,    IS,    IS,    IS,    IS,    IS}, /* S6:	ASNR (MVID)		*/
		{    IS,   IS,    IS,   IS,   IS,   IS,    IS,    IS,    IS,    IS,    IS,    IS}, /* S7:	ASWR (KEY)		*/
		{     9,    8,     9,   10,    9,    9,     9,     9,     9,    9,     ER,     9}, /* S8:	NOAS			*/
		{    IS,   IS,    IS,   IS,   IS,   IS,    IS,    IS,    IS,    IS,    IS,    IS}, /* S9:	ASWR (IV)		*/
		{    ES,   11,    ES,   ES,   ES,   ES,    ES,    ES,    ES,    ES,    ER,    ES}, /* S10:	NOAS			*/
		{    12,   11,    12,   12,   12,   12,    12,    12,    12,    12,    ER,    12}, /* S11:	NOAS			*/
		{    IS,   IS,    IS,   IS,   IS,   IS,    IS,    IS,    IS,    IS,    IS,    IS}, /* S12:	ASWR (FPV)		*/
		{    13,   13,    13,   13,   13,   13,    13,    13,    13,    14,    ER,    13}, /* S13:	NOAS			*/
		{    IS,   IS,    IS,   IS,   IS,   IS,    IS,    IS,    IS,    IS,    IS,    IS}, /* S14:	ASNR (SL)		*/
		{    IS,   IS,    IS,   IS,   IS,   IS,    IS,    IS,    IS,    IS,    IS,    IS}, /* S15:	ASNR (ERR)		*/
		{    IS,   IS,    IS,   IS,   IS,   IS,    IS,    IS,    IS,    IS,    IS,    IS}  /* S16:	ASWR (ERR)		*/
};

/* Define accepting states types */
#define NOAS	0		/* not accepting state */
#define ASNR	1		/* accepting state with no retract */
#define ASWR	2		/* accepting state with retract */

/* TO_DO: Define list of acceptable states */
static india_int stateType[] = {
	NOAS, /* 00			*/
	NOAS, /* 01			*/
	ASNR, /* 02 (IVID)	*/
	ASNR, /* 03 (FVID)	*/
	ASNR, /* 04 (SVID)	*/
	ASNR, /* 05 (BVID)	*/
	ASNR, /* 06 (MVID)	*/
	ASWR, /* 07 (KEY)	*/
	NOAS, /* 08			*/
	ASWR, /* 09 (IV)	*/
	NOAS, /* 10			*/
	NOAS, /* 11			*/
	ASWR, /* 12 (FPV)	*/
	NOAS, /* 13			*/
	ASNR, /* 14 (SL)	*/
	ASNR, /* 15 (ERR)	*/
	ASWR  /* 16 (ERR)	*/
};

/*
-------------------------------------------------
TO_DO: Adjust your functions'definitions
-------------------------------------------------
*/

/* Static (local) function  prototypes */
india_int startScanner(BufferPointer psc_buf);
static india_int nextClass(india_chr c);			/* character class function */
static india_int nextState(india_int, india_chr);		/* state machine function */

/*
-------------------------------------------------
Automata definitions
-------------------------------------------------
*/

/* TO_DO: Pointer to function (of one char * argument) returning Token */
typedef Token(*PTR_ACCFUN)(india_chr* lexeme);

/* Declare accepting states functions */
Token funcID(india_chr lexeme[]);
Token funcIV(india_chr lexeme[]);
Token funcFPV(india_chr lexeme[]);
Token funcSL(india_chr lexeme[]);
Token funcKEY(india_chr lexeme[]);
Token funcErr(india_chr lexeme[]);

/*
 * Accepting function (action) callback table (array) definition
 * If you do not want to use the typedef, the equvalent declaration is:
 */

 /* TO_DO: Define final state table */
static PTR_ACCFUN finalStateTable[] = {
	NULL,		/* -		[00] */
	NULL,		/* -		[01] */
	funcID,		/* IVID		[02] */
	funcID,		/* FVID		[03] */
	funcID,		/* SVID		[04] */
	funcID,		/* BVID		[05] */
	funcID,		/* MVID		[06] */
	funcKEY,	/* KEY		[07] */
	NULL,		/* _		[08] */
	funcIV,		/* IV		[09] */
	NULL,		/* _		[10] */
	NULL,	    /* -		[11] */
	funcFPV,	/* FPV		[12] */
	NULL,		/* -		[13] */
	funcSL,		/* SL		[14] */
	funcErr,	/* ERR1(ES)	[15] */
	funcErr		/* ERR2(ER)	[16] */
};

/*
-------------------------------------------------
Language keywords
-------------------------------------------------
*/

/* TO_DO: Define the number of Keywords from the language */
#define KWT_SIZE 13

/* TO_DO: Define the list of keywords */
static india_chr* keywordTable[KWT_SIZE] = {
	"while",
	"if",
	"elif",
	"do",
	"printf",
	"else",
	"scanf",
	"int",
	"fint",
	"boolean",
	"string",
	"YES",
	"NO"
};

#endif
