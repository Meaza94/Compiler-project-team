/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Fall, 2024
* Author: Mihretab Meaza & Mohamad Al Dakkak
* Professors: Paulo Sousa
************************************************************
*/

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
#ifndef READER_H_
#include "Step2Reader.h"
#endif
#ifndef SCANNER_H_
#include "Step3Scanner.h"
#endif

/* Global vars */
static Token            lookahead;
extern BufferPointer    stringLiteralTable;
extern mplusplus_intg   line;
extern Token            tokenizer(mplusplus_void);
extern mplusplus_strg   keywordTable[KWT_SIZE];
static mplusplus_intg   syntaxErrorNumber = 0;

#define STR_LANGNAME    "mplusplus"
#define LANG_WRTE       "print&"
#define LANG_READ       "input&"
#define LANG_MAIN       "main&"

/* Constants - Updated for mplusplus Math DSL */
enum KEYWORDS {
    NO_ATTR = -1,
    KW_let,      /* let - variable declaration */
    KW_if,       /* if - conditional */
    KW_else,     /* else - alternative */
    KW_while,    /* while - loop */
    KW_return,   /* return - return statement */
    KW_solve,    /* solve - equation solver */
    KW_derive,   /* derive - derivative */
    KW_pi,       /* pi - mathematical constant */
    KW_e,        /* e - mathematical constant */
    KW_sin,      /* sin - sine function */
    KW_cos,      /* cos - cosine function */
    KW_log       /* log - logarithm function */
};

/* Number of BNF rules */
#define NUM_BNF_RULES 15

/* Parser structure */
typedef struct parserData {
    mplusplus_intg parsHistogram[NUM_BNF_RULES];    /* Number of BNF Statements */
} ParserData, * pParsData;

/* Number of errors */
mplusplus_intg numParserErrors;

/* Scanner data */
ParserData psData;

/* Function definitions */
mplusplus_void startParser();
mplusplus_void matchToken(mplusplus_intg, mplusplus_intg);
mplusplus_void syncErrorHandler(mplusplus_intg);
mplusplus_void printError();
mplusplus_void printBNFData(ParserData psData);

/* List of BNF statements */
enum BNF_RULES {
    BNF_error,                                        /*  0: Error token */
    BNF_program,                                      /*  1: Program */
    BNF_mathBlock,                                    /*  2: Math block */
    BNF_statement,                                    /*  3: Statement */
    BNF_statements,                                   /*  4: Statements */
    BNF_statementsPrime,                              /*  5: Statements Prime */
    BNF_variableDeclaration,                          /*  6: Variable declaration */
    BNF_assignment,                                   /*  7: Assignment */
    BNF_expression,                                   /*  8: Expression */
    BNF_arithmeticExpression,                         /*  9: Arithmetic expression */
    BNF_arithmeticExpressionPrime,                    /* 10: Arithmetic expression prime */
    BNF_term,                                         /* 11: Term */
    BNF_factor,                                       /* 12: Factor */
    BNF_returnStatement,                              /* 13: Return statement */
    BNF_outputStatement                               /* 14: Output statement */
};

/* List of BNF rule names */
static mplusplus_strg BNFStrTable[NUM_BNF_RULES] = {
    "BNF_error",
    "BNF_program",
    "BNF_mathBlock",
    "BNF_statement",
    "BNF_statements",
    "BNF_statementsPrime",
    "BNF_variableDeclaration",
    "BNF_assignment",
    "BNF_expression",
    "BNF_arithmeticExpression",
    "BNF_arithmeticExpressionPrime",
    "BNF_term",
    "BNF_factor",
    "BNF_returnStatement",
    "BNF_outputStatement"
};

/* Non-terminal function declarations */
mplusplus_void program();
mplusplus_void mathBlock();
mplusplus_void statement();
mplusplus_void statements();
mplusplus_void statementsPrime();
mplusplus_void variableDeclaration();
mplusplus_void assignment();
mplusplus_void expression();
mplusplus_void arithmeticExpression();
mplusplus_void arithmeticExpressionPrime();
mplusplus_void term();
mplusplus_void factor();
mplusplus_void returnStatement();
mplusplus_void outputStatement();

#endif