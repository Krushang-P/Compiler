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
* File name: compilers.c
* Compiler: MS Visual Studio 2022
* Author: Krushang Patel (#41021848) , Vandankumar Patel (#041000779)
* Course: CST 8152 � Compilers, Lab Section: [011]
* Assignment: A12
* Date: Feb 04, 2022
* Professor: Paulo Sousa
* Purpose: This file is the main program of Compilers Project
* Function list: main().
************************************************************
*/

#include <stdio.h>
#include <stdlib.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

/*
***********************************************************
* Function name: main
* Purpose: Main function
* Author: Krushang Patel and Vandankumar Patel
* History/Versions: Ver 1.0
* Called functions: mainBuffer(), mainScanner(), mainParser()
* Parameters: Command line arguments - argc, argv
* Return value: Status
* Algorithm: - Upon the call of debug, runs the entire code with
	       repsect to the choice in .exe, for Buffer(here), 
	       Parcer and Scanner Input (further assignments)
*************************************************************
*/
india_int main(int argc, char** argv) {
	india_int i;
	if (DEBUG) {
		for (i = 0; i < argc; ++i)
			printf("argv[%d] = %s\n", i, argv[i]);
	}
	if (argc < 2) {
		printf("%s%s%c%s%c%s%c%s", argv[0], ": OPTIONS [",
			PGM_BUFFER, "] - Buffer, [",
			PGM_SCANNER, "] - Scanner, [",
			PGM_PARSER, "] - Parser\n");
		return EXIT_FAILURE;
	}
	india_chr option = argv[1][0];
	switch (option) {
	case PGM_BUFFER:
		mainBuffer(argc, argv);
		break;
	case PGM_SCANNER:
		mainScanner(argc, argv);
		break;
	case PGM_PARSER:
		mainParser(argc, argv);
		break;
	default:
		printf("%s%s%c%s%c%s%c%s", argv[0], ": OPTIONS [",
			PGM_BUFFER, "] - Buffer, [",
			PGM_SCANNER, "] - Scanner, [",
			PGM_PARSER, "] - Parser\n");
		break;
	}
	return EXIT_SUCCESS;
}
