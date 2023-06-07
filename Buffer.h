/*
*********************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Winter, 2022
* Author: Svillen Ranev - Paulo Sousa
* Professors: Paulo Sousa / George Kriger / Abdullah Kadri
*********************************************************************
*/

/*
***********************************************************************
* File name: Buffer.h
* Compiler: MS Visual Studio 2022
* Author: Krushang Patel (#41021848) , Vandankumar Patel (#041000779)
* Course: CST 8152 � Compilers, Lab Section: [011]
* Assignment: A12.
* Date: Feb 04, 2022
* Professor: Paulo Sousa
* Purpose: This file is the main header for Parser (.h)
**********************************************************************
*/

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef BUFFER_H_
#define BUFFER_H_

/*#pragma warning(1:4001) *//*to enforce C89 type comments  - to make //comments an warning */

/*#pragma warning(error:4001)*//* to enforce C89 comments - to make // comments an error */

/* standard header files */
#include <stdio.h>  /* standard input/output */
#include <malloc.h> /* for dynamic memory allocation*/
#include <limits.h> /* implementation-defined data type ranges and limits */

/* CONSTANTS DEFINITION: GENERAL (NOT LANGUAGE DEPENDENT) .................................. */

/* Modes (used to create buffer) */
enum BUFFERMODES {
	MODE_FIXED = 'f',
	MODE_ADDIT = 'a',
	MODE_MULTI = 'm'
};

/* TO_DO: Adjust all constants to your Language */

#define BUFFER_ERROR (-1)						/* General error message */
#define BUFFER_EOF '\0'							/* General EOF */

/* CONSTANTS DEFINITION: PREFIXED BY LANGUAGE NAME (INDIA) .................................. */

#define BUFFER_DEFAULT_SIZE			200			/* default initial buffer capacity */
#define BUFFER_DEFAULT_INCREMENT	100				/* default increment factor */

/* Add your bit-masks constant definitions here - Defined for INDIA */
/* BITS                                (7654.3210) */
#define INDIA_DEFAULT_FLAG 0x0F 	/* (0000.1111)_2 = (015)_10 */

/* TO_DO: BIT 7: REL = Relocation */
#define SET_RLB_FLAG_REL 0x80 	/* (1000.0000)_2 = (128)_10 (Apply: OR)*/
#define RESET_RLB_FLAG_REL 0x7F /* (0111.1111)_2 = (127)_10 (Apply: AND)*/
#define CHK_RLB_FLAG_REL 0x80 	/* (1000.0000)_2 = (128)_10 (Apply: AND)*/ 

/* TO_DO: BIT 6: EOB = EndOfBuffer */
#define SET_EOB_FLAG 0x40  	/* (0100.0000)_2 = (64)_10 */
#define RESET_EOB_FLAG 0xBF 	/* (1011.1111)_2 = (63)_10 */
#define CHK_EOB_FLAG 0x40  	/* (0100.1111)_2 = (64)_10 */

/* TO_DO: BIT 5: FUL = Full */
#define SET_FUL_FLAG 0x20 	/* (0010.0000)_2 = (47)_10 */
#define RESET_FUL_FLAG 0xDF 	/* (1101.1111)_2 = (223)_10*/
#define CHK_FUL_FLAG 0x20 	/* (0010.1111)_2 = (47)_10 */

/* TO_DO: BIT 4: EMP = Empty */
#define SET_EMP_FLAG 0x10 	/* (0001.0000)_2 = (16)_10 */
#define RESET_EMP_FLAG 0xEF 	/* (1110.1111)_2 = (239)_10*/
#define CHK_EMP_FLAG 0x10 	/* (0001.0000)_2 = (16)_10 */

/* STRUCTURES DEFINITION: SUFIXED BY LANGUAGE NAME (INDIA) .................................. */

/* TO_DO: Adjust datatypes */

/* Offset declaration */
typedef struct position {
	india_int markPos;			/* the offset (in chars) to the mark location */
	india_int readPos;			/* the offset (in chars) to the get-character location */
	india_int writePos;			/* the offset (in chars) to the add-character location */
} Position;

/* Buffer structure */
typedef struct buffer {
	india_chr*	string;			/* pointer to the beginning of character array (character buffer) */
	india_int	size;		    	/* current dynamic memory size (in bytes) allocated to character buffer */
	india_int	increment;		/* character array increment factor */
	india_int	mode;			/* operational mode indicator */
	india_flg	flags;			/* contains character array reallocation flag and end-of-buffer flag */
	Position	position;		/* Offset / position field */
} Buffer, * BufferPointer;

/* FUNCTIONS DECLARATION:  .................................. */
/* TO_DO: Define your function declarations using your language specification */

/* FUNCTIONS DECLARATION:  .................................. */
/* General Operations */
BufferPointer	bCreate(india_int, india_int, india_int);
BufferPointer	bAddChar(BufferPointer const, india_chr);
india_bol		bRetract(BufferPointer const);
india_bol		bRestore(BufferPointer const);
india_bol		bRecover(BufferPointer const);
india_bol		bClear(BufferPointer const);
india_bol		bDestroy(BufferPointer const);
india_bol		bSetMark(BufferPointer const, india_int);
india_bol		bIsEmpty(BufferPointer const);
india_bol		bIsFull(BufferPointer const);
india_bol		bIsRealocated(BufferPointer const);
india_int		bPrint(BufferPointer const);
india_int		bLoad(BufferPointer const, FILE* const);
/* Getters */
india_int		bGetSize(BufferPointer const);
india_int		bGetWritePos(BufferPointer const);
india_int		bGetMarkPos(BufferPointer const);
india_int		bGetReadPos(BufferPointer const);
india_int		bGetIncrement(BufferPointer const);
india_int		bGetMode(BufferPointer const);
india_chr		bGetChar(BufferPointer const);
india_chr*		bGetContent(BufferPointer const, india_int);
india_flg		bGetFlags(BufferPointer const);

#endif
