/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Winter, 2022
* Author: Svillen Ranev - Paulo Sousa
* Professors: Paulo Sousa / George Kriger / Abdullah Kadri
************************************************************
*/

/*
***********************************************************
* File name: buffer.c
* Compiler: MS Visual Studio 2022
* Author: Paulo Sousa
* Updated by: Krushang Patel and Vandankumar Patel
* Course: CST 8152 � Compilers, Lab Section: [011]
* Assignment: A12.
* Date: March 19 2022
* Professor: Paulo Sousa
* Purpose: This file is the main code for Buffer (A12)
************************************************************
*/

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef BUFFER_H_
#include "Buffer.h"
#endif

/*
***********************************************************
* Function name: bCreate
* Purpose: Creates the buffer according to capacity, increment
	factor and operational mode ('f', 'a', 'm')
* Author: Svillen Ranev / Paulo Sousa
* Updated by: Krushang Patel and Vandankumar Patel
* History/Versions: Ver 2.1
* Called functions: calloc(), malloc()
* Parameters:
*   size = initial capacity
*   increment = increment factor
*   mode = operational mode
* Return value: bPointer (pointer to Buffer)
* Algorithm: Allocation of memory according to inicial (default) values.
* TO_DO:
*	- Adjust datatypes for your LANGUAGE.
*   - Use defensive programming
*	- Check boundary conditions
*	- Check flags.
*************************************************************
*/

BufferPointer bCreate(india_int size, india_int increment, india_int mode) {
	BufferPointer b;
	/* TO_DO: Defensive programming */
	if (size<0 || size>INDIA_MAX_SIZE)
		return NULL;
	if (!size) {
		size = BUFFER_DEFAULT_SIZE;
		increment = BUFFER_DEFAULT_INCREMENT;
	}
	if (!increment)
		mode = MODE_FIXED;
	b = (BufferPointer)calloc(1, sizeof(Buffer));
	/* TO_DO: Defensive programming */
	if (b == NULL)
		return NULL;
	b->string = (india_chr*)malloc(size);
	/* TO_DO: Defensive programming */
	if (!b->string) {
		free(b);
		return NULL;
	}
	b->mode = mode;
	b->size = size;
	b->increment = increment;
	/* TO_DO: Initialize flags */
	b->flags = INDIA_DEFAULT_FLAG;
	/* TO_DO: The created flag must be signalized as EMP */
	b->flags = b->flags | SET_EMP_FLAG;
	return b;
}


/*
***********************************************************
* Function name: bAddChar
* Updated by: Krushang Patel and Vandankumar Patel
* Purpose: Adds a india_chr to buffer
* Parameters:
*   pBuffer = pointer to Buffer Entity
*   ch = india_chr to be added
* Return value:
*	bPointer (pointer to Buffer)
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
BufferPointer bAddChar(BufferPointer const pBuffer, india_chr ch) {
	india_chr* tempbuf;
	india_int newSize;
	/* TO_DO: Defensive programming */
	if (!pBuffer)
		return NULL;
	/* TO_DO: Reset Realocation */
	pBuffer->flags = pBuffer->flags & RESET_RLB_FLAG_REL;
	/* TO_DO: Test the inclusion of india_chrs */
	if (pBuffer->position.writePos * (india_int)sizeof(india_chr) < pBuffer->size) {
		/* TO_DO: This buffer is NOT full */
		pBuffer->flags = pBuffer->flags & RESET_FUL_FLAG;
	}
	else {
		/* TO_DO: Reset Full flag */
		pBuffer->flags = pBuffer->flags | SET_FUL_FLAG;
		switch (pBuffer->mode) {
		case MODE_FIXED:
			return NULL;
		case MODE_ADDIT:
			newSize = pBuffer->size + pBuffer->increment;
			/* TO_DO: Defensive programming */
			if (newSize < 0 || newSize <= pBuffer->size) {
				return NULL;
			}
			if (DEBUG)
				printf("%s%d%s", "\n................\n* New size: ",
					newSize, "\n................\n");
			break;
		case MODE_MULTI:
			newSize = pBuffer->size * pBuffer->increment;
			/* TO_DO: Defensive programming */
			if (newSize < 0 || newSize <= pBuffer->size) {
				return NULL;
			}
			if (DEBUG)
				printf("%s%d%s", "\n................\n* New size: ",
					newSize, "\n................\n");
			break;
		default:
			return NULL;
		}
		tempbuf = (india_chr*)realloc(pBuffer->string, newSize);
		/* TO_DO: Defensive programming */
		if (!tempbuf)
			return NULL;
		if (tempbuf != pBuffer->string) {
			/* TO_DO: Set Relocation */
			pBuffer->flags = pBuffer->flags | SET_RLB_FLAG_REL;
			pBuffer->string = tempbuf;
		}
		pBuffer->size = newSize;
	}
	/* TO_DO: Add the india_chr */
	pBuffer->flags = pBuffer->flags & RESET_EMP_FLAG;
	pBuffer->string[pBuffer->position.writePos++] = ch;
	return pBuffer;
}

/*
***********************************************************
* Function name: bClear
* Updated by: Krushang Patel and Vandankumar Patel
* Purpose: Clears the buffer
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
india_bol bClear(BufferPointer const pBuffer) {
	/* TO_DO: Defensive programming */
	if (!pBuffer)
		return NO;
	pBuffer->position.writePos = pBuffer->position.markPos = pBuffer->position.readPos = 0;
	/* TO_DO: Adjust flags original */
	pBuffer->flags = INDIA_DEFAULT_FLAG;
	return YES;
}

/*
***********************************************************
* Function name: bDestroy
* Updated by: Krushang Patel and Vandankumar Patel
* Purpose: Releases the buffer address
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
india_bol bDestroy(BufferPointer const pBuffer) {
	/* TO_DO: Defensive programming */
	if (!pBuffer)
		return NO;
	free(pBuffer->string);
	free(pBuffer);
	return YES;
}

/*
***********************************************************
* Function name: bIsFull
* Updated by: Krushang Patel and Vandankumar Patel
* Purpose: Checks if buffer is full
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
india_bol bIsFull(BufferPointer const pBuffer) {
	/* TO_DO: Defensive programming */
	if (!pBuffer)
		return NO;
	/* TO_DO: Check flag if buffer is FUL */
	if (pBuffer->flags & CHK_FUL_FLAG)
		return YES;
	return NO;
}

/*
***********************************************************
* Function name: bGetWritePos
* Updated by: Krushang Patel and Vandankumar Patel
* Purpose: Returns the position of india_chr to be added
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	addcPosition value
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
india_int bGetWritePos(BufferPointer const pBuffer) {
	/* TO_DO: Defensive programming */
	if (!pBuffer)
		return BUFFER_ERROR;
	return pBuffer->position.writePos;
}

/*
***********************************************************
* Function name: bGetSize
* Purpose: Returns the current buffer capacity
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	Size of buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
india_int bGetSize(BufferPointer const pBuffer) {
	/* TO_DO: Defensive programming */
	if (!pBuffer)
		return BUFFER_ERROR;
	return pBuffer->size;
}

/*
***********************************************************
* Function name: bGetMode
* Updated by: Krushang Patel and Vandankumar Patel
* Purpose: Returns the operational mode
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	operational mode.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
india_int bGetMode(BufferPointer const pBuffer) {
	/* TO_DO: Defensive programming */
	if (!pBuffer)
		return (india_chr)BUFFER_ERROR;
	return pBuffer->mode;
}


/*
***********************************************************
* Function name: bGetMarkPos
* Updated by: Krushang Patel and Vandankumar Patel
* Purpose: Returns the position of mark in the buffer
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	mark offset.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
india_int bGetMarkPos(BufferPointer const pBuffer) {
	/* TO_DO: Defensive programming */
	if (!pBuffer)
		return BUFFER_ERROR;
	return pBuffer->position.markPos;
}


/*
***********************************************************
* Function name: bSetMark
* Updated by: Krushang Patel and Vandankumar Patel
* Purpose: Adjust the position of mark in the buffer
* Parameters:
*   pBuffer = pointer to Buffer Entity
*   mark = mark position for india_chr
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
india_bol bSetMark(BufferPointer const pBuffer, india_int mark) {
	/* TO_DO: Defensive programming */
	if (!pBuffer || mark<0 || mark > pBuffer->position.writePos)
		return NO;
	pBuffer->position.markPos = mark;
	return YES;
}

/*
***********************************************************
* Function name: bPrint
* Updated by: Krushang Patel and Vandankumar Patel
* Purpose: Prints the string in the buffer.
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	Number of india_chrs printed.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
india_int bPrint(BufferPointer const pBuffer) {
	india_int cont = 0;
	india_chr c;
	/* TO_DO: Defensive programming */
	if (!pBuffer || !(pBuffer->string))
		return BUFFER_ERROR;
	c = bGetChar(pBuffer);
	/* TO_DO: Check flag if buffer EOB has achieved */
	while (!(pBuffer->flags & CHK_EOB_FLAG)) {
		cont++;
		printf("%c", c);
		c = bGetChar(pBuffer);
	}
	return cont;
}

/*
***********************************************************
* Function name: bLoad
* Updated by: Krushang Patel and Vandankumar Patel
* Purpose: Loads the string in the buffer with the content of
	an specific file.
* Parameters:
*   pBuffer = pointer to Buffer Entity
*   fi = pointer to file descriptor
* Return value:
*	Number of india_chrs read and put in buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
india_int bLoad(BufferPointer const pBuffer, FILE* const fi) {
	india_int size = 0;
	india_chr c;
	/* TO_DO: Defensive programming */
	if (!fi || !pBuffer)
		return BUFFER_ERROR;
	c = (india_chr)fgetc(fi);
	while (!feof(fi)) {
		if (!bAddChar(pBuffer, c)) {
			ungetc(c, fi);
			return BUFFER_ERROR;
		}
		c = (india_chr)fgetc(fi);
		size++;
	}
	if (ferror(fi))
		return BUFFER_ERROR;
	return size;
}

/*
***********************************************************
* Function name: bIsEmpty
* Updated by: Krushang Patel and Vandankumar Patel
* Purpose: Checks if buffer is empty.
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
india_bol bIsEmpty(BufferPointer const pBuffer) {
	/* TO_DO: Defensive programming */
	if (!pBuffer)
		return NO;
	/* TO_DO: Check flag if buffer is EMP */
	if (pBuffer->flags & CHK_EMP_FLAG)
		return YES;
	return NO;
}

/*
***********************************************************
* Function name: bGetChar
* Updated by: Krushang Patel and Vandankumar Patel
* Purpose: Returns the india_chr in the getC position.
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	Char in the getC position.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
india_chr bGetChar(BufferPointer const pBuffer) {
	/* TO_DO: Defensive programming */
	if (!pBuffer)
		return BUFFER_ERROR;
	if (pBuffer->position.readPos == pBuffer->position.writePos) {
		/* TO_DO: Set EOB flag */
		pBuffer->flags = pBuffer->flags | SET_EOB_FLAG;
		return BUFFER_EOF;
	}
	/* TO_DO: Reset EOB flag */
	pBuffer->flags = pBuffer->flags & RESET_EOB_FLAG;
	return pBuffer->string[pBuffer->position.readPos++];
}


/*
***********************************************************
* Function name: bRecover
* Updated by: Krushang Patel and Vandankumar Patel
* Purpose: Rewinds the buffer.
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
india_bol bRecover(BufferPointer const pBuffer) {
	/* TO_DO: Defensive programming */
	if (!pBuffer)
		return NO;
	pBuffer->position.readPos = 0;
	pBuffer->position.markPos = 0;
	return YES;
}


/*
***********************************************************
* Function name: bRetract
* Updated by: Krushang Patel and Vandankumar Patel
* Purpose: Retracts the buffer to put back the india_chr in buffer.
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
india_bol bRetract(BufferPointer const pBuffer) {
	/* TO_DO: Defensive programming */
	if (!pBuffer || pBuffer->position.readPos == 0)
		return NO;
	pBuffer->position.readPos--;
	return YES;
}


/*
***********************************************************
* Function name: bRestore
* Updated by: Krushang Patel and Vandankumar Patel
* Purpose: Resets the buffer.
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
india_bol bRestore(BufferPointer const pBuffer) {
	/* TO_DO: Defensive programming */
	if (!pBuffer)
		return NO;
	pBuffer->position.readPos = pBuffer->position.markPos;
	return YES;
}


/*
***********************************************************
* Function name: bGetReadPos
* Updated by: Krushang Patel and Vandankumar Patel
* Purpose: Returns the value of getCPosition.
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	The readPos offset.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
india_int bGetReadPos(BufferPointer const pBuffer) {
	/* TO_DO: Defensive programming */
	if (!pBuffer)
		return BUFFER_ERROR;
	return pBuffer->position.readPos;
}

/*
***********************************************************
* Function name: bGetIncrement
* Updated by: Krushang Patel and Vandankumar Patel
* Purpose: Returns the buffer increment.
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	The Buffer increment.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
india_int bGetIncrement(BufferPointer const pBuffer) {
	/* TO_DO: Defensive programming */
	if (!pBuffer)
		return BUFFER_ERROR;
	return pBuffer->increment;
}

/*
***********************************************************
* Function name: bGetContent
* Updated by: Krushang Patel and Vandankumar Patel
* Purpose: Returns the pointer to String.
* Parameters:
*   pBuffer = pointer to Buffer Entity
*   pos = position to get the pointer
* Return value:
*	Position of string india_chr.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
india_chr* bGetContent(BufferPointer const pBuffer, india_int pos) {
	/* TO_DO: Defensive programming */
	if (!pBuffer || pos < 0 || pos > pBuffer->position.writePos)
		return NULL;
	return pBuffer->string + pos;
}

/*
***********************************************************
* Function name: bGetFlags
* Updated by: Krushang Patel and Vandankumar Patel
* Purpose: Returns the entire flags of Buffer.
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	Flags from Buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
#define FLAGS_
#undef FLAGS_
#ifndef FLAGS_
india_flg bGetFlags(BufferPointer const pBuffer) {
	/* TO_DO: Defensive programming */
	if (!pBuffer)
		return (india_flg)BUFFER_ERROR;
	return pBuffer->flags;
}
#else
#define bGetFlags(pBuffer) ((pBuffer)?(pBuffer->flags):(RT_FAIL_1))
#endif
