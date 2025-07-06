
/*

	  TEAM MEMBERS

Student Name: Mihretab Meaza
student Number: 041106564
Professor: Paulo Sousa
Compilers Assignment 2
language name: mplusplus


Student Name: Mohamad Al Dakkak
student Number: 041120078
Professor: Paulo Sousa
Compilers Assignment 2
language name: mplusplus

*/



/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Summer, 2025
* Author: TO_DO
* Professors: Paulo Sousa
************************************************************
#
# ECHO "=---------------------------------------="
# ECHO "|  COMPILERS - ALGONQUIN COLLEGE (S25)  |"
# ECHO "=---------------------------------------="
# ECHO "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    "
# ECHO "    @@                             @@    "
# ECHO "    @@           %&@@@@@@@@@@@     @@    "
# ECHO "    @@       @%% (@@@@@@@@@  @     @@    "
# ECHO "    @@      @& @   @ @       @     @@    "
# ECHO "    @@     @ @ %  / /   @@@@@@     @@    "
# ECHO "    @@      & @ @  @@              @@    "
# ECHO "    @@       @/ @*@ @ @   @        @@    "
# ECHO "    @@           @@@@  @@ @ @      @@    "
# ECHO "    @@            /@@    @@@ @     @@    "
# ECHO "    @@     @      / /     @@ @     @@    "
# ECHO "    @@     @ @@   /@/   @@@ @      @@    "
# ECHO "    @@     @@@@@@@@@@@@@@@         @@    "
# ECHO "    @@                             @@    "
# ECHO "    @@         S O F I A           @@    "
# ECHO "    @@                             @@    "
# ECHO "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    "
# ECHO "                                         "
# ECHO "[READER SCRIPT .........................]"
# ECHO "                                         "
*/

/*
************************************************************
* File name: Step2Reader.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A12.
* Date: Jan 01 2025
* Professor: Paulo Sousa
* Purpose: This file is the main header for Reader (.h)
************************************************************
*/

/*
 *.............................................................................
 * MAIN ADVICE:
 * Please check the "TODO" labels to develop your activity.
 *.............................................................................
 */

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef READER_H_
#define READER_H_

 /* TIP: Do not change pragmas, unless necessary .......................................*/
 /*#pragma warning(1:4001) *//*to enforce C89 type comments  - to make //comments an warning */

 /* standard header files */
#include <stdio.h>  /* standard input/output */
#include <malloc.h> /* for dynamic memory allocation*/
#include <limits.h> /* implementation-defined data type ranges and limits */

/* CONSTANTS DEFINITION: GENERAL (NOT LANGUAGE DEPENDENT) .................................. */

/* Constants about controls (not need to change) */
#define READER_ERROR				(-1)	/* General error message */
#define READER_TERMINATOR			'\0'	/* General EOF */

/* CONSTANTS DEFINITION: PREFIXED BY LANGUAGE NAME .................................. */

/* TO_DO: You should add your own constant definitions here */
#define READER_MAX_SIZE				INT_MAX	/* maximum capacity */ 

#define READER_DEFAULT_SIZE			250		/* default initial buffer reader capacity */

#define NCHAR						128		/* Chars from 0 to 127 */

#define CHARSEOF					(-1)	/* EOF Code for Reader */

/* STRUCTURES DEFINITION: SUFIXED BY LANGUAGE NAME .................................. */

/* TODO: Adjust datatypes */

/* Offset declaration */
typedef struct position {
	mplusplus_intg read;			/* the offset to the get a char position (in chars) */
	mplusplus_intg wrte;			/* the offset to the add chars (in chars) */
	mplusplus_intg mark;			/* the offset to the mark position (in chars) */
} Position;


typedef struct flag {
	mplusplus_boln isEmpty; /* checks if there is no content -  checks if the file is empty*/
	mplusplus_boln isFull; /* the content is using all size - this will be in the end of the file size - 1*/
	mplusplus_boln isRead; /* all content was read */
	mplusplus_boln isMoved; /* the content was moved in reallocation */
} Flag;

/* Buffer structure */
typedef struct bufferReader {
	mplusplus_strg		content;			/* pointer to the beginning of character array (character buffer) */
	mplusplus_intg		size;				/* current dynamic memory size (in bytes) allocated to character buffer */
	Flag			    flags;				/* contains character array reallocation flag and end-of-buffer flag */
	Position		    position;			/* Offset / position field */
	mplusplus_intg		histogram[NCHAR];	/* Statistics of chars */
	mplusplus_intg		numReaderErrors;	/* Number of errors from Reader */
	mplusplus_byte		checkSum;			/* Sum of bytes(chars) */
} Buffer, * BufferPointer;

/* FUNCTIONS DECLARATION:  .................................. */

/* General Operations */
BufferPointer	readerCreate(mplusplus_intg);
BufferPointer	readerAddChar(BufferPointer const, mplusplus_char);
mplusplus_boln	readerClear(BufferPointer const);
mplusplus_boln	readerFree(BufferPointer const);
mplusplus_boln	readerIsFull(BufferPointer const);
mplusplus_boln	readerIsEmpty(BufferPointer const);
mplusplus_boln	readerSetMark(BufferPointer const, mplusplus_intg);
mplusplus_intg	readerPrint(BufferPointer const);
mplusplus_intg	readerLoad(BufferPointer const, mplusplus_strg);
mplusplus_boln	readerRecover(BufferPointer const);
mplusplus_boln	readerRetract(BufferPointer const);
mplusplus_boln	readerRestore(BufferPointer const);
mplusplus_intg	readerChecksum(BufferPointer const);
/* Getters */
mplusplus_char	readerGetChar(BufferPointer const);
mplusplus_strg	readerGetContent(BufferPointer const, mplusplus_intg);
mplusplus_intg	readerGetPosRead(BufferPointer const);
mplusplus_intg	readerGetPosWrte(BufferPointer const);
mplusplus_intg	readerGetPosMark(BufferPointer const);
mplusplus_intg	readerGetSize(BufferPointer const);
mplusplus_byte	readerGetFlags(BufferPointer const);
mplusplus_void	readerPrintStat(BufferPointer const);
mplusplus_intg	readerNumErrors(BufferPointer const);

#endif