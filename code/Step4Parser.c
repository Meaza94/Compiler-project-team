/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Summer, 2025
* Authors: Mihretab Meaza & Mohamad Al Dakkak
* Professors: Paulo Sousa
************************************************************
*/

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef SCANNER_H_
#include "Step3Scanner.h"
#endif

#ifndef PARSER_H_
#include "Step4Parser.h"
#endif

/* Parser data */
extern ParserData psData; /* BNF statistics */

/* Start the parser */
mplusplus_void startParser() {
    mplusplus_intg i = 0;
    for (i = 0; i < NUM_BNF_RULES; i++) {
        psData.parsHistogram[i] = 0;
    }
    lookahead = tokenizer();
    if (lookahead.code != SEOF_T) {
        program();
    }
    matchToken(SEOF_T, NO_ATTR);
    printf("%s%s\n", STR_LANGNAME, ": Source file parsed");
    printf("Number of Parser errors: %d\n", syntaxErrorNumber);
    printBNFData(psData);
}

/* Match token */
mplusplus_void matchToken(mplusplus_intg tokenCode, mplusplus_intg tokenAttribute) {
    mplusplus_intg matchFlag = 1;
    if (lookahead.code != tokenCode)
        matchFlag = 0;
    else if (lookahead.code == KW_T && lookahead.attribute.codeType != tokenAttribute)
        matchFlag = 0;

    if (matchFlag && lookahead.code == SEOF_T)
        return;

    if (matchFlag) {
        lookahead = tokenizer();
        if (lookahead.code == ERR_T) {
            printError();
            lookahead = tokenizer();
            syntaxErrorNumber++;
        }
    }
    if (lookahead.code == ERR_T) {
        printError();
        lookahead = tokenizer();
    }
    else {
        syncErrorHandler(tokenCode);
    }
}

/* Error handler */
mplusplus_void syncErrorHandler(mplusplus_intg syncTokenCode) {
    printError();
    syntaxErrorNumber++;
    while (lookahead.code != syncTokenCode) {
        if (lookahead.code == SEOF_T)
            exit(syntaxErrorNumber);
        lookahead = tokenizer();
    }
    if (lookahead.code != SEOF_T)
        lookahead = tokenizer();
}

/* Print error */
mplusplus_void printError() {
    extern mplusplus_intg numParserErrors;
    Token t = lookahead;
    printf("%s%s%3d\n", STR_LANGNAME, ": Syntax error:  Line:", line);
    printf("*****  Token code:%3d Attribute: ", t.code);

    switch (t.code) {
    case ERR_T:
        printf("*ERROR*: %s\n", t.attribute.errLexeme);
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
    case MNID_T:
        printf("ID_T\t\t%s\n", t.attribute.idLexeme);
        break;
    case STR_T:
        printf("STR_T: %s\n", readerGetContent(stringLiteralTable, t.attribute.contentString));
        break;
    case KW_T:
        printf("KW_T: %s\n", keywordTable[t.attribute.codeType]);
        break;
    default:
        printf("UNKNOWN TOKEN\n");
        break;
    }
}

/* Grammar rules */
mplusplus_void program() {
    psData.parsHistogram[BNF_program]++;
    while (lookahead.code == CMT_T) matchToken(CMT_T, NO_ATTR);
    if (lookahead.code == LBR_T) mathBlock();
    else if (lookahead.code != SEOF_T) statements();
    printf("%s%s\n", STR_LANGNAME, ": Program parsed");
}

mplusplus_void mathBlock() {
    psData.parsHistogram[BNF_mathBlock]++;
    matchToken(LBR_T, NO_ATTR);
    while (lookahead.code == CMT_T) matchToken(CMT_T, NO_ATTR);
    if (lookahead.code != RBR_T && lookahead.code != SEOF_T) statements();
    matchToken(RBR_T, NO_ATTR);
    printf("%s%s\n", STR_LANGNAME, ": Math block parsed");
}

mplusplus_void statements() {
    psData.parsHistogram[BNF_statements]++;
    statement();
    statementsPrime();
    printf("%s%s\n", STR_LANGNAME, ": Statements parsed");
}

mplusplus_void statementsPrime() {
    psData.parsHistogram[BNF_statementsPrime]++;
    if (lookahead.code == KW_T || lookahead.code == ID_T || lookahead.code == MNID_T || lookahead.code == CMT_T || lookahead.code == LBR_T) {
        while (lookahead.code == CMT_T) matchToken(CMT_T, NO_ATTR);
        if (lookahead.code != RBR_T && lookahead.code != SEOF_T) {
            if (lookahead.code == LBR_T) mathBlock();
            else statements();
        }
    }
    printf("%s%s\n", STR_LANGNAME, ": Statements prime parsed");
}

mplusplus_void statement() {
    psData.parsHistogram[BNF_statement]++;
    mplusplus_intg code = lookahead.code;
    switch (code) {
    case KW_T:
        switch (lookahead.attribute.codeType) {
        case KW_let:
            variableDeclaration();
            break;
        case KW_if:
            matchToken(KW_T, KW_if);
            matchToken(LPR_T, NO_ATTR);
            expression();
            matchToken(RPR_T, NO_ATTR);
            mathBlock();
            if (lookahead.attribute.codeType == KW_else) {
                matchToken(KW_T, KW_else);
                mathBlock();
            }
            break;
        case KW_while:
            matchToken(KW_T, KW_while);
            matchToken(LPR_T, NO_ATTR);
            expression();
            matchToken(RPR_T, NO_ATTR);
            mathBlock();
            break;
        case KW_return:
            returnStatement();
            break;
        case KW_solve:
        case KW_derive:
            matchToken(KW_T, lookahead.attribute.codeType);
            matchToken(LPR_T, NO_ATTR);
            expression();
            matchToken(RPR_T, NO_ATTR);
            matchToken(EOS_T, NO_ATTR);
            break;
        default:
            printError();
            break;
        }
        break;
    case MNID_T:
        outputStatement();
        break;
    case ID_T:
        if (strcmp(lookahead.attribute.idLexeme, "print&") == 0 ||
            strcmp(lookahead.attribute.idLexeme, "solve&") == 0 ||
            strcmp(lookahead.attribute.idLexeme, "input&") == 0) {
            outputStatement();
        }
        else {
            assignment();
        }
        break;
    default:
        printError();
        break;
    }
    printf("%s%s\n", STR_LANGNAME, ": Statement parsed");
}

mplusplus_void variableDeclaration() {
    psData.parsHistogram[BNF_variableDeclaration]++;
    matchToken(KW_T, KW_let);
    matchToken(ID_T, NO_ATTR);
    matchToken(ASOP_T, NO_ATTR);
    expression();
    matchToken(EOS_T, NO_ATTR);
    printf("%s%s\n", STR_LANGNAME, ": Variable declaration parsed");
}

mplusplus_void assignment() {
    psData.parsHistogram[BNF_assignment]++;
    matchToken(ID_T, NO_ATTR);
    matchToken(ASOP_T, NO_ATTR);
    expression();
    matchToken(EOS_T, NO_ATTR);
    printf("%s%s\n", STR_LANGNAME, ": Assignment parsed");
}

mplusplus_void returnStatement() {
    psData.parsHistogram[BNF_returnStatement]++;
    matchToken(KW_T, KW_return);
    expression();
    matchToken(EOS_T, NO_ATTR);
    printf("%s%s\n", STR_LANGNAME, ": Return statement parsed");
}

mplusplus_void outputStatement() {
    psData.parsHistogram[BNF_outputStatement]++;
    if (lookahead.code == MNID_T ||
        (lookahead.code == ID_T && strcmp(lookahead.attribute.idLexeme, "print&") == 0)) {
        lookahead = tokenizer(); // consume print&
    }
    else {
        printError();
        return;
    }
    matchToken(LPR_T, NO_ATTR);
    expression(); // Allow expression instead of just STR_T
    matchToken(RPR_T, NO_ATTR);
    matchToken(EOS_T, NO_ATTR);
    printf("%s%s\n", STR_LANGNAME, ": Output statement parsed");
}

mplusplus_void expression() {
    psData.parsHistogram[BNF_expression]++;
    arithmeticExpression();
    printf("%s%s\n", STR_LANGNAME, ": Expression parsed");
}

mplusplus_void arithmeticExpression() {
    psData.parsHistogram[BNF_arithmeticExpression]++;
    term();
    arithmeticExpressionPrime();
    printf("%s%s\n", STR_LANGNAME, ": Arithmetic expression parsed");
}

mplusplus_void arithmeticExpressionPrime() {
    psData.parsHistogram[BNF_arithmeticExpressionPrime]++;
    if (lookahead.code == AROP_T) {
        matchToken(AROP_T, lookahead.attribute.arithmeticOperator);
        term();
        arithmeticExpressionPrime();
    }
    printf("%s%s\n", STR_LANGNAME, ": Arithmetic expression prime parsed");
}

mplusplus_void term() {
    psData.parsHistogram[BNF_term]++;
    factor();
    printf("%s%s\n", STR_LANGNAME, ": Term parsed");
}

mplusplus_void factor() {
    psData.parsHistogram[BNF_factor]++;
    switch (lookahead.code) {
    case ID_T:
        matchToken(ID_T, NO_ATTR);
        break;
    case INL_T:
        matchToken(INL_T, NO_ATTR);
        break;
    case FLT_T:
        matchToken(FLT_T, NO_ATTR);
        break;
    case KW_T:
        if (lookahead.attribute.codeType == KW_pi || lookahead.attribute.codeType == KW_e)
            matchToken(KW_T, lookahead.attribute.codeType);
        else
            printError();
        break;
    case LPR_T:
        matchToken(LPR_T, NO_ATTR);
        expression();
        matchToken(RPR_T, NO_ATTR);
        break;
    default:
        printError();
        break;
    }
    printf("%s%s\n", STR_LANGNAME, ": Factor parsed");
}

