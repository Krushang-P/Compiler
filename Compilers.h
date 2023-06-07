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
* File name: compilers.h
* Compiler: MS Visual Studio 2022
* Author: Krushang Patel (#41021848) , Vandankumar Patel (#041000779)
* Course: CST 8152 � Compilers, Lab Section: [011]
* Assignment: A12
* Date: Feb 04 2022
* Professor: Paulo Sousa
* Purpose: This file defines the functions called by main function.
* Function list: mainBuffer(), mainScanner(), mainParser().
*************************************************************/

#ifndef COMPILERS_H_
#define COMPILERS_H_

#define DEBUG 0

/* Logical constants - adapt for your language */
#define YES	1
#define NO 0

/* You should add your own constant definitions here */
#define INDIA_MAX_SIZE				SHRT_MAX-1	/* maximum capacity */ 

/*
------------------------------------------------------------
Data types definitions
------------------------------------------------------------
*/
/* TO_DO: Define your typedefs */
typedef int 			india_int;
typedef short			india_intS;
typedef long			india_intL;
typedef float			india_fint;
typedef double			india_dint;

typedef char			india_chr;
typedef char*			india_string;
typedef int 			india_bol;

typedef void			india_nul;

typedef unsigned char	india_flg;

/*
------------------------------------------------------------
Programs:
1: Buffer - invokes MainBuffer code
2: Scanner - invokes MainScanner code
3: Parser - invokes MainParser code
------------------------------------------------------------
*/
enum PROGRAMS {
	PGM_BUFFER	= '1',
	PGM_SCANNER = '2',
	PGM_PARSER	= '3'
};

/*
------------------------------------------------------------
Main functions signatures
(Codes will be updated during next assignments)
------------------------------------------------------------
*/
india_int mainBuffer	(india_int argc, india_chr** argv);
india_int mainScanner(india_int argc, india_chr** argv);
india_int mainParser(india_int argc, india_chr** argv);

#endif
