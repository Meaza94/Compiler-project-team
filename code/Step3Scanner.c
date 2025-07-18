/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Summer, 2025
* Author: Mihretab Meaza & Mohamad Al Dakkak
* Professors: Paulo Sousa
************************************************************
* File name: Step3Scanner.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A3 - Scanner Implementation
* Date: July 02 2025
* Purpose: This file contains all functionalities for mplusplus Math DSL Scanner
* Function list: All scanner implementation functions for algebraic math language
************************************************************
*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>   /* standard input / output */
#include <ctype.h>   /* conversion functions */
#include <stdlib.h>  /* standard library functions and constants */
#include <string.h>  /* string functions */
#include <limits.h>  /* integer types constants */
#include <float.h>   /* floating-point types constants */

/* #define NDEBUG to suppress assert() call */
#include <assert.h>  /* assert() prototype */

/* project header files */
#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef BUFFER_H_
#include "Step2Reader.h"
#endif

#ifndef SCANNER_H_
#include "Step3Scanner.h"
#endif

/*
----------------------------------------------------------------
Global Variables for mplusplus Math DSL Scanner
----------------------------------------------------------------
*/

/* Global objects - variables */
extern BufferPointer stringLiteralTable;       /* String literal table */
mplusplus_intg line;                            /* Current line number of the source code */
extern mplusplus_intg errorNumber;              /* Run-time error number */

extern mplusplus_intg stateType[NUM_STATES];
extern mplusplus_strg keywordTable[KWT_SIZE];
extern PTR_ACCFUN finalStateTable[NUM_STATES];
extern mplusplus_intg transitionTable[NUM_STATES][CHAR_CLASSES];

/* Local(file) global objects - variables */
static BufferPointer lexemeBuffer;              /* Pointer to temporary lexeme buffer */
static BufferPointer sourceBuffer;              /* Pointer to input source buffer */

/* Global Scanner Data */
ScannerData scData = { 0 };

/*
 ************************************************************
 * Initialize Scanner for mplusplus Math DSL
 * This function initializes the scanner using defensive programming.
 * Parameters: BufferPointer psc_buf - pointer to source buffer
 * Return: mplusplus_intg - success/failure status
 ************************************************************
 */

mplusplus_intg startScanner(BufferPointer psc_buf) {
    /* Initialize histogram for token statistics */
    mplusplus_intg i = 0;
    for (i = 0; i < NUM_TOKENS; i++)
        scData.scanHistogram[i] = 0;

    /* Basic scanner initialization */
    /* in case the buffer has been read previously */
    readerRecover(psc_buf);
    readerClear(stringLiteralTable);
    line = 1;
    sourceBuffer = psc_buf;
    return EXIT_SUCCESS;
}

/*
 ************************************************************
 * Main Tokenizer Function for mplusplus Math DSL - NO WARNINGS VERSION
 * Main function of scanner, responsible to classify a char (or sequence
 * of chars). In the first part, a specific sequence is detected (reading
 * from buffer). In the second part, a pattern (defined by Regular Expression)
 * is recognized and the appropriate function is called (related to final states
 * in the Transition Diagram).
 * Parameters: mplusplus_void
 * Return: Token - the recognized token
 ************************************************************
 */

Token tokenizer(mplusplus_void) {
    Token currentToken = { 0 };
    mplusplus_char c;
    mplusplus_intg state = 0;
    mplusplus_intg lexStart;
    mplusplus_intg lexEnd;
    mplusplus_intg lexLength;
    mplusplus_intg i;
    mplusplus_intg col;
    mplusplus_intg next_st;
    mplusplus_strg lexeme;

    while (1) { /* endless loop broken by token returns */
        c = readerGetChar(sourceBuffer);

        /* Check for end of file first */
        if (c == READER_TERMINATOR) {
            currentToken.code = SEOF_T;
            currentToken.attribute.seofType = SEOF_0;
            scData.scanHistogram[currentToken.code]++;
            return currentToken;
        }
        // ... rest of your existing code

        if (c == EOS_CHR) {
            currentToken.code = SEOF_T;
            currentToken.attribute.seofType = SEOF_0;
            scData.scanHistogram[currentToken.code]++;
            return currentToken;
        }

        if (c == (mplusplus_char)EOF_CHR) {
            currentToken.code = SEOF_T;
            currentToken.attribute.seofType = SEOF_255;
            scData.scanHistogram[currentToken.code]++;
            return currentToken;
        }

        /* ------------------------------------------------------------------------
            Part 1: Implementation of token driven scanner for mplusplus Math DSL.
            Only handle tokens that CANNOT be confused with DFA patterns
            -----------------------------------------------------------------------
        */

        switch (c) {
            /* Cases for whitespace */
        case SPC_CHR:
        case TAB_CHR:
            break;
        case NWL_CHR:
            line++;
            break;

            /* Comments - handle directly */
        case HST_CHR:
            /* Skip comment until end of line */
            while ((c = readerGetChar(sourceBuffer)) != NWL_CHR &&
                c != READER_TERMINATOR) {
                /* consume comment characters */
            }
            if (c == NWL_CHR) {
                line++;
            }
            /* Return comment token */
            currentToken.code = CMT_T;
            scData.scanHistogram[currentToken.code]++;
            return currentToken;

            /* Logical operators - removed unreachable code */
        case AND_CHR:
            c = readerGetChar(sourceBuffer);
            if (c == '&') goto dfa_processing;
            {
                currentToken.code = LOGOP_T;
                currentToken.attribute.logicalOperator = OP_AND;
                scData.scanHistogram[currentToken.code]++;
                return currentToken;
            }
            readerRetract(sourceBuffer);
            goto dfa_processing;

            /* Mathematical operator symbols - directly recognized */
        case PLUS_CHR:
            currentToken.code = AROP_T;
            currentToken.attribute.arithmeticOperator = OP_ADD;
            scData.scanHistogram[currentToken.code]++;
            return currentToken;

        case MINUS_CHR:
            goto dfa_processing;  // ← Let DFA handle it


        case MUL_CHR:
            currentToken.code = AROP_T;
            currentToken.attribute.arithmeticOperator = OP_MUL;
            scData.scanHistogram[currentToken.code]++;
            return currentToken;
        case DIV_CHR:
            currentToken.code = AROP_T;
            currentToken.attribute.arithmeticOperator = OP_DIV;
            scData.scanHistogram[currentToken.code]++;
            return currentToken;

            /* Assignment operator */
        case EQ_CHR:
            /* Look ahead for == */
            c = readerGetChar(sourceBuffer);
            if (c == EQ_CHR) {
                currentToken.code = RELOP_T;
                currentToken.attribute.relationalOperator = OP_EQ;
            }
            else {
                readerRetract(sourceBuffer);
                currentToken.code = ASOP_T;
                currentToken.attribute.codeType = 0;
            }
            scData.scanHistogram[currentToken.code]++;
            return currentToken;

            /* Relational operators */
        case LT_CHR:
            c = readerGetChar(sourceBuffer);
            if (c == EQ_CHR) {
                currentToken.code = RELOP_T;
                currentToken.attribute.relationalOperator = OP_LE;
            }
            else {
                readerRetract(sourceBuffer);
                currentToken.code = RELOP_T;
                currentToken.attribute.relationalOperator = OP_LT;
            }
            scData.scanHistogram[currentToken.code]++;
            return currentToken;
        case GT_CHR:
            c = readerGetChar(sourceBuffer);
            if (c == EQ_CHR) {
                currentToken.code = RELOP_T;
                currentToken.attribute.relationalOperator = OP_GE;
            }
            else {
                readerRetract(sourceBuffer);
                currentToken.code = RELOP_T;
                currentToken.attribute.relationalOperator = OP_GT;
            }
            scData.scanHistogram[currentToken.code]++;
            return currentToken;
        case NOT_CHR:
            c = readerGetChar(sourceBuffer);
            if (c == EQ_CHR) {
                currentToken.code = RELOP_T;
                currentToken.attribute.relationalOperator = OP_NE;
            }
            else {
                readerRetract(sourceBuffer);
                currentToken.code = LOGOP_T;
                currentToken.attribute.logicalOperator = OP_NOT;
            }
            scData.scanHistogram[currentToken.code]++;
            return currentToken;

            /* Parentheses and braces for mathematical expressions */
        case LPR_CHR:
            currentToken.code = LPR_T;
            scData.scanHistogram[currentToken.code]++;
            return currentToken;

        case RPR_CHR:
            currentToken.code = RPR_T;
            scData.scanHistogram[currentToken.code]++;
            return currentToken;  // Return RPR_T directly, let next tokenizer handle next char like ';'

        case LBR_CHR:
            currentToken.code = LBR_T;
            scData.scanHistogram[currentToken.code]++;
            return currentToken;
        case RBR_CHR:
            currentToken.code = RBR_T;
            scData.scanHistogram[currentToken.code]++;
            return currentToken;

            /* End of statement */
        case SCL_CHR:
            currentToken.code = EOS_T;
            scData.scanHistogram[currentToken.code]++;
            return currentToken;

            /* ------------------------------------------------------------------------
                Part 2: Implementation of Finite State Machine (DFA) for mplusplus Math DSL
                ALL complex patterns (identifiers, numbers, floats) use DFA
                -----------------------------------------------------------------------
            */
        default: /* ALL letters, digits, dots, etc. go through DFA */
        dfa_processing:
            state = 0;
            lexStart = readerGetPosRead(sourceBuffer) - 1;
            readerSetMark(sourceBuffer, lexStart);

            while (1) {
                col = nextClass(c);
                next_st = transitionTable[state][col];

                if (next_st == ESNR || next_st == ESWR) {
                    state = next_st;
                    break;
                }

                state = next_st;

                if (stateType[state] != NOFS) {
                    if (stateType[state] == FSWR) {
                        readerRetract(sourceBuffer);
                    }
                    break;
                }

                c = readerGetChar(sourceBuffer);
                if (c == READER_TERMINATOR) break;
            }

            if (state == ESNR || state == ESWR) {
                if (state == ESWR) {
                    readerRetract(sourceBuffer);
                }
                currentToken.code = ERR_T;
                currentToken.attribute.errLexeme[0] = c;
                currentToken.attribute.errLexeme[1] = EOS_CHR;
                scData.scanHistogram[currentToken.code]++;
                return currentToken;
            }

            lexEnd = readerGetPosRead(sourceBuffer);
            lexLength = lexEnd - lexStart;

            /* Create lexeme buffer */
            lexemeBuffer = readerCreate((mplusplus_intg)lexLength + 2);
            if (!lexemeBuffer) {
                fprintf(stderr, "Scanner error: Can not create buffer\n");
                exit(1);
            }

            /* Copy lexeme to buffer */
            readerRestore(sourceBuffer);
            for (i = 0; i < lexLength; i++)
                readerAddChar(lexemeBuffer, readerGetChar(sourceBuffer));
            readerAddChar(lexemeBuffer, READER_TERMINATOR);
            lexeme = readerGetContent(lexemeBuffer, 0);

            /* Defensive programming */
            if (!lexeme) {
                readerFree(lexemeBuffer);
                currentToken.code = ERR_T;
                strcpy(currentToken.attribute.errLexeme, "Invalid lexeme");
                scData.scanHistogram[currentToken.code]++;
                return currentToken;
            }

            /* Call appropriate final state function */
            currentToken = (*finalStateTable[state])(lexeme);
            readerFree(lexemeBuffer);
            return currentToken;
        } /* switch */

    } /* while */
} /* tokenizer */
/*
 ************************************************************
 * Get Next State for mplusplus Math DSL DFA - FIXED Line 149
 * The assert(int test) macro can be used to add run-time diagnostic to programs
 * and to "defend" from producing unexpected results.
 * Parameters: mplusplus_intg state, mplusplus_char c
 * Return: mplusplus_intg - next state
 ************************************************************
 */

mplusplus_intg nextState(mplusplus_intg state, mplusplus_char c) {
    mplusplus_intg col;
    mplusplus_intg next;
    col = nextClass(c);
    next = transitionTable[state][col];

    assert(next != FS);
    return next;
}

/*
 ************************************************************
 * Get Next Token Class for mplusplus Math DSL
 * Create a function to return the column number in the transition table:
 * Considering an input char c, you can identify the "class".
 * Parameters: mplusplus_char c
 * Return: mplusplus_intg - column index
 ************************************************************
 */

mplusplus_intg nextClass(mplusplus_char c) {
    mplusplus_intg val = -1;

    switch (c) {
    case UND_CHR:
        val = COL_UNDERSCORE;
        break;
    case AMP_CHR:
        val = COL_AMPERSAND;
        break;
    case QUT_CHR:
        val = COL_QUOTE;
        break;
    case HST_CHR:
        val = COL_HASH;
        break;
    case DOT_CHR:
        val = COL_DOT;
        break;
    case READER_TERMINATOR:
    case (mplusplus_char)EOF_CHR:
        val = COL_EOF;
        break;
    default:
        if (isalpha(c))
            val = COL_LETTER;
        else if (isdigit(c))
            val = COL_DIGIT;
        else
            val = COL_OTHER;
    }
    return val;
}

/*
 ************************************************************
 * Acceptance State Function for Integer Literals (mplusplus Math DSL)
 * Function responsible to identify IL (integer literals).
 * - It is necessary to respect the limit (ex: 2-byte integer in C).
 * - In the case of larger lexemes, error should be returned.
 * Parameters: mplusplus_strg lexeme
 * Return: Token
 ************************************************************
 */

Token funcIL(mplusplus_strg lexeme) {
    Token currentToken = { 0 };
    mplusplus_long tlong;

    if (lexeme[0] != EOS_CHR && strlen(lexeme) > NUM_LEN) {
        currentToken = (*finalStateTable[ESNR])(lexeme);
    }
    else {
        if (lexeme[0] == MINUS_CHR && strlen(lexeme) > 1) {
            tlong = atol(lexeme + 1) * -1; // Handle unary minus
        }
        else {
            tlong = atol(lexeme);
        }
        if (tlong >= SHRT_MIN && tlong <= SHRT_MAX) {
            currentToken.code = INL_T;
            scData.scanHistogram[currentToken.code]++;
            currentToken.attribute.intValue = (mplusplus_intg)tlong;
        }
        else {
            currentToken = (*finalStateTable[ESNR])(lexeme);
        }
    }
    return currentToken;
}

/*
 ************************************************************
 * Acceptance State Function for Float Literals (mplusplus Math DSL)
 * Function responsible to identify FLT (float literals like 3.14, 2.718).
 * Parameters: mplusplus_strg lexeme
 * Return: Token
 ************************************************************
 */

Token funcFLT(mplusplus_strg lexeme) {
    Token currentToken = { 0 };
    mplusplus_real tfloat;

    // Reject lone "." as invalid float
    if (strcmp(lexeme, ".") == 0) {
        strcpy(currentToken.attribute.errLexeme, ".");
        currentToken.code = ERR_T;
        scData.scanHistogram[currentToken.code]++;
        return currentToken;
    }

    if (lexeme[0] != EOS_CHR && strlen(lexeme) > FLT_LEN) {
        currentToken = (*finalStateTable[ESNR])(lexeme);
    }
    else {
        if (lexeme[0] == MINUS_CHR && strlen(lexeme) > 1) {
            tfloat = (mplusplus_real)atof(lexeme + 1) * -1.0;
        }
        else {
            tfloat = (mplusplus_real)atof(lexeme);
        }
        if (tfloat >= -FLT_MAX && tfloat <= FLT_MAX) {
            currentToken.code = FLT_T;
            scData.scanHistogram[currentToken.code]++;
            currentToken.attribute.floatValue = tfloat;
        }
        else {
            currentToken = (*finalStateTable[ESNR])(lexeme);
        }
    }

    return currentToken;
}
/*
 ************************************************************
 * Acceptance State Function for Identifiers (mplusplus Math DSL)
 * In this function, the pattern for IDs must be recognized.
 * Since keywords obey the same pattern, is required to test if
 * the current lexeme matches with KW from language.
 * Parameters: mplusplus_strg lexeme
 * Return: Token
 ************************************************************
 */

Token funcID(mplusplus_strg lexeme) {
    Token currentToken = { 0 };
    size_t length = strlen(lexeme);
    mplusplus_char lastch = lexeme[length - 1];

    /* Check if it ends with & for method names */
    if (lastch == AMP_CHR) {
        /* Method name ending with & */
        currentToken.code = MNID_T;
        scData.scanHistogram[currentToken.code]++;
        strncpy(currentToken.attribute.idLexeme, lexeme, VID_LEN);
        currentToken.attribute.idLexeme[VID_LEN] = EOS_CHR;
        return currentToken;
    }
    /* Regular identifier - delegate to funcKEY to check for keywords */
    return funcKEY(lexeme);
}

/*
 ************************************************************
 * Acceptance State Function for String Literals (mplusplus Math DSL)
 * Function responsible to identify SL (string literals).
 * - The lexeme must be stored in the String Literal Table.
 * Parameters: mplusplus_strg lexeme
 * Return: Token
 ************************************************************
 */

Token funcSL(mplusplus_strg lexeme) {
    Token currentToken = { 0 };
    mplusplus_intg i = 0, len = (mplusplus_intg)strlen(lexeme) - 2; // Exclude quotes
    if (len > readerGetSize(stringLiteralTable) - readerGetPosWrte(stringLiteralTable)) {
        currentToken.code = ERR_T;
        strcpy(currentToken.attribute.errLexeme, "String literal too long");
        errorNumber = RTE_CODE;
        return currentToken;
    }
    currentToken.attribute.contentString = readerGetPosWrte(stringLiteralTable);
    for (i = 1; i < (mplusplus_intg)strlen(lexeme) - 1; i++) {
        if (lexeme[i] == NWL_CHR) line++;
        if (!readerAddChar(stringLiteralTable, lexeme[i])) {
            currentToken.code = ERR_T;
            strcpy(currentToken.attribute.errLexeme, "Run Time Error");
            errorNumber = RTE_CODE;
            return currentToken;
        }
    }
    if (!readerAddChar(stringLiteralTable, EOS_CHR)) {
        currentToken.code = ERR_T;
        strcpy(currentToken.attribute.errLexeme, "Run Time Error");
        errorNumber = RTE_CODE;
        return currentToken;
    }
    currentToken.code = STR_T;
    scData.scanHistogram[currentToken.code]++;
    return currentToken;
}

/*
 ************************************************************
 * Keyword Recognition Function for mplusplus Math DSL - FIXED Line 296
 * This function checks if lexeme is keyword, identifier, or integer.
 * Parameters: mplusplus_strg lexeme
 * Return: Token
 ************************************************************
 */

Token funcKEY(mplusplus_strg lexeme) {
    Token currentToken = { 0 };
    mplusplus_intg kwindex = -1, j = 0;
    mplusplus_char cleanLexeme[VID_LEN + 1];
    mplusplus_intg i, len;

    /* Trim trailing whitespace/punctuation */
    len = (mplusplus_intg)strlen(lexeme);
    for (i = 0; i < len && i < VID_LEN; i++) {
        if (isalnum(lexeme[i]) || lexeme[i] == '_') {
            cleanLexeme[i] = lexeme[i];
        }
        else {
            break; /* Stop at first non-identifier character */
        }
    }
    // Additional trim for any leading/trailing spaces (though leading shouldn't occur)
    while (i > 0 && isspace(cleanLexeme[i - 1])) {
        i--;
    }
    while (i > 0 && ispunct(cleanLexeme[i - 1]) && cleanLexeme[i - 1] != '_') {
        i--;
    }
    cleanLexeme[i] = EOS_CHR;

    /* Search for keyword in keyword table first */
    for (j = 0; j < KWT_SIZE; j++) {
        if (!strcmp(cleanLexeme, keywordTable[j])) {
            kwindex = j;
            break;
        }
    }

    if (kwindex != -1) {
        currentToken.code = KW_T;
        currentToken.attribute.codeType = kwindex;
    }
    else {
        currentToken.code = ID_T;
        strncpy(currentToken.attribute.idLexeme, cleanLexeme, VID_LEN);
        currentToken.attribute.idLexeme[VID_LEN] = EOS_CHR;
    }

    scData.scanHistogram[currentToken.code]++;
    return currentToken;
}

/*
 ************************************************************
 * Acceptance State Function for Comments (mplusplus Math DSL)
 * Function responsible to identify CMT (comments starting with #).
 * Parameters: mplusplus_strg lexeme
 * Return: Token
 ************************************************************
 */

Token funcCMT(mplusplus_strg lexeme) {
    Token currentToken = { 0 };
    mplusplus_intg i = 0, len = (mplusplus_intg)strlen(lexeme);

    /* Count newlines in comment for line tracking */
    for (i = 1; i < len - 1; i++) {
        if (lexeme[i] == NWL_CHR)
            line++;
    }

    currentToken.code = CMT_T;
    scData.scanHistogram[currentToken.code]++;
    return currentToken;
}

/*
 ************************************************************
 * Acceptance State Function for Errors (mplusplus Math DSL)
 * Function responsible to deal with ERR token.
 * Parameters: mplusplus_strg lexeme
 * Return: Token
 ************************************************************
 */

Token funcErr(mplusplus_strg lexeme) {
    Token currentToken = { 0 };
    mplusplus_intg i = 0, len = (mplusplus_intg)strlen(lexeme);

    if (len > ERR_LEN) {
        strncpy(currentToken.attribute.errLexeme, lexeme, ERR_LEN - 3);
        currentToken.attribute.errLexeme[ERR_LEN - 3] = EOS_CHR;
        strcat(currentToken.attribute.errLexeme, "...");
    }
    else {
        strcpy(currentToken.attribute.errLexeme, lexeme);
    }

    /* Count newlines for line tracking */
    for (i = 0; i < len; i++) {
        if (lexeme[i] == NWL_CHR)
            line++;
    }

    currentToken.code = ERR_T;
    scData.scanHistogram[currentToken.code]++;
    return currentToken;
}

/*
 ************************************************************
 * Token Printing Function for mplusplus Math DSL
 * The function prints the token returned by the scanner
 * Parameters: Token t
 * Return: mplusplus_void
 ************************************************************
 */

mplusplus_void printToken(Token t) {
    extern mplusplus_strg keywordTable[];

    switch (t.code) {
    case ERR_T:
        printf("ERR_T\t\t%s\n", t.attribute.errLexeme);
        break;
    case SEOF_T:
        printf("SEOF_T\t\t%d\t\n", t.attribute.seofType);
        break;
    case INL_T:
        printf("INL_T\t\t%d\n", t.attribute.intValue);
        break;
    case FLT_T:
        printf("FLT_T\t\t%.5f\n", t.attribute.floatValue);
        break;
    case ID_T:
        printf("ID_T\t\t%s\n", t.attribute.idLexeme);
        break;
    case MNID_T:
        printf("MNID_T\t\t%s\n", t.attribute.idLexeme);
        break;
    case STR_T:
        printf("STR_T\t\t%d\t ", (mplusplus_intg)t.attribute.contentString);
        printf("%s\n", readerGetContent(stringLiteralTable, (mplusplus_intg)t.attribute.contentString));
        break;
    case KW_T:
        printf("KW_T\t\t%s\n", keywordTable[t.attribute.codeType]);
        break;
    case AROP_T:
        switch (t.attribute.arithmeticOperator) {
        case OP_ADD: printf("AROP_T\t\t+\n"); break;
        case OP_SUB: printf("AROP_T\t\t-\n"); break;
        case OP_MUL: printf("AROP_T\t\t*\n"); break;
        case OP_DIV: printf("AROP_T\t\t/\n"); break;
        default: printf("AROP_T\t\t%d\n", t.attribute.arithmeticOperator); break;
        }
        break;
    case ASOP_T:
        printf("ASOP_T\t\t=\n");
        break;
    case RELOP_T:
        switch (t.attribute.relationalOperator) {
        case OP_EQ: printf("RELOP_T\t\t==\n"); break;
        case OP_NE: printf("RELOP_T\t\t!=\n"); break;
        case OP_GT: printf("RELOP_T\t\t>\n"); break;
        case OP_LT: printf("RELOP_T\t\t<\n"); break;
        case OP_GE: printf("RELOP_T\t\t>=\n"); break;
        case OP_LE: printf("RELOP_T\t\t<=\n"); break;
        default: printf("RELOP_T\t\t%d\n", t.attribute.relationalOperator); break;
        }
        break;
    case LOGOP_T:
        switch (t.attribute.logicalOperator) {
        case OP_AND: printf("LOGOP_T\t\t&&\n"); break;
        case OP_OR: printf("LOGOP_T\t\t||\n"); break;
        case OP_NOT: printf("LOGOP_T\t\t!\n"); break;
        default: printf("LOGOP_T\t\t%d\n", t.attribute.logicalOperator); break;
        }
        break;
    case LPR_T:
        printf("LPR_T\t\t(\n");
        break;
    case RPR_T:
        printf("RPR_T\t\t)\n");
        break;
    case LBR_T:
        printf("LBR_T\t\t{\n");
        break;
    case RBR_T:
        printf("RBR_T\t\t}\n");
        break;
    case EOS_T:
        printf("EOS_T\t\t;\n");
        break;
    case CMT_T:
        printf("CMT_T\t\t@\n");
        break;
    default:
        printf("Scanner error: invalid token code: %d\n", t.code);
    }
}

/*
 ************************************************************
 * Scanner Statistics Function for mplusplus Math DSL
 * The function prints statistics of tokens
 * Parameters: ScannerData scData
 * Return: mplusplus_void
 ************************************************************
 */

mplusplus_void printScannerData(ScannerData scData) {
    /* Print Scanner statistics */
    printf("Statistics:\n");
    printf("----------------------------------\n");
    int cont = 0;
    for (cont = 0; cont < NUM_TOKENS; cont++) {
        if (scData.scanHistogram[cont] > 0)
            printf("%s%s%s%d%s", "Token[", tokenStrTable[cont], "]=", scData.scanHistogram[cont], "\n");
    }
    printf("----------------------------------\n");
}