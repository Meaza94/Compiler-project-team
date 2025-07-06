/*

      TEAM MEMBERS

Student Name: Mihretab Meaza
student Number: 041106564
Professor: Paulo Sousa
Compilers Assignment 1
language name: mplusplus


Student Name: Mohamad Al Dakkak
student Number: 041120078
Professor: Paulo Sousa
Compilers Assignment 1
language name: mplusplus

*/

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef READER_H_
#include "Step2Reader.h"
#endif

#ifndef SCANNER_H_
#define SCANNER_H_

#ifndef NULL
#include <_null.h> /* NULL pointer constant is defined there */
#endif

/* Constants */
#define VID_LEN 20  /* variable identifier length */
#define ERR_LEN 40  /* error message length */
#define NUM_LEN 5   /* maximum number of digits for IL */
#define FLT_LEN 10  /* maximum length for float literals */

#define RTE_CODE 1  /* Value for run-time error */

/* mplusplus Math DSL Token Count */
#define NUM_TOKENS 18

/* mplusplus Math DSL Token Codes */
enum TOKENS {
    ERR_T,      /*  0: Error token */
    SEOF_T,     /*  1: Source end-of-file token */
    INL_T,      /*  2: Integer literal token */
    FLT_T,      /*  3: Float literal token */
    ID_T,       /*  4: Identifier token (variables) */
    MNID_T,     /*  5: Method name identifier token (ends with &) */
    STR_T,      /*  6: String literal token */
    KW_T,       /*  7: Keyword token */
    AROP_T,     /*  8: Arithmetic operators (+, -, *, /) */
    ASOP_T,     /*  9: Assignment operator (=) */
    RELOP_T,    /* 10: Relational operators (>, <, >=, <=, ==, !=) */
    LOGOP_T,    /* 11: Logical operators (&&, ||, !) */
    LPR_T,      /* 12: Left parenthesis token */
    RPR_T,      /* 13: Right parenthesis token */
    LBR_T,      /* 14: Left brace token */
    RBR_T,      /* 15: Right brace token */
    EOS_T,      /* 16: End of statement (semicolon) */
    CMT_T       /* 17: Comment token */
};

/* Token String Table for mplusplus Math DSL */
static mplusplus_strg tokenStrTable[NUM_TOKENS] = {
    "ERR_T",
    "SEOF_T",
    "INL_T",
    "FLT_T",
    "ID_T",
    "MNID_T",
    "STR_T",
    "KW_T",
    "AROP_T",
    "ASOP_T",
    "RELOP_T",
    "LOGOP_T",
    "LPR_T",
    "RPR_T",
    "LBR_T",
    "RBR_T",
    "EOS_T",
    "CMT_T"
};

/* mplusplus Math DSL Operator Attributes */
typedef enum ArithmeticOperators {
    OP_ADD,     /* + */
    OP_SUB,     /* - */
    OP_MUL,     /* * */
    OP_DIV      /* / */
} AriOperator;

typedef enum RelationalOperators {
    OP_EQ,      /* == */
    OP_NE,      /* != */
    OP_GT,      /* > */
    OP_LT,      /* < */
    OP_GE,      /* >= */
    OP_LE       /* <= */
} RelOperator;

typedef enum LogicalOperators {
    OP_AND,     /* && */
    OP_OR,      /* || */
    OP_NOT      /* ! */
} LogOperator;

typedef enum SourceEndOfFile {
    SEOF_0,
    SEOF_255
} EofOperator;

/* Token Attribute Union for mplusplus Math DSL */
typedef union TokenAttribute {
    mplusplus_intg codeType;                    /* integer attributes accessor */
    AriOperator arithmeticOperator;             /* arithmetic operator attribute code */
    RelOperator relationalOperator;             /* relational operator attribute code */
    LogOperator logicalOperator;                /* logical operator attribute code */
    EofOperator seofType;                       /* source-end-of-file attribute code */
    mplusplus_intg intValue;                    /* integer literal attribute (value) */
    mplusplus_real floatValue;                  /* float literal attribute (value) */
    mplusplus_intg keywordIndex;                /* keyword index in the keyword table */
    mplusplus_intg contentString;               /* string literal offset from the beginning of the string literal buffer */
    mplusplus_char idLexeme[VID_LEN + 1];       /* variable identifier token attribute */
    mplusplus_char errLexeme[ERR_LEN + 1];      /* error token attribute */
} TokenAttribute;

/* Identifier Attributes for mplusplus Math DSL */
typedef struct idAttibutes {
    mplusplus_byte flags;                       /* Flags information */
    union {
        mplusplus_intg intValue;                /* Integer value */
        mplusplus_real floatValue;              /* Float value */
        mplusplus_strg stringContent;           /* String value */
    } values;
} IdAttibutes;

/* Token Structure for mplusplus Math DSL */
typedef struct Token {
    mplusplus_intg code;                        /* token code */
    TokenAttribute attribute;                   /* token attribute */
    IdAttibutes idAttribute;                    /* for future use */
} Token;

/* Scanner Data Structure */
typedef struct scannerData {
    mplusplus_intg scanHistogram[NUM_TOKENS];   /* Statistics of tokens */
} ScannerData, * pScanData;

///////////////////////////////////////////////////////////////////////////////////////////////////////

/* mplusplus Math DSL Character Constants */
#define EOS_CHR     '\0'    /* End of string */
#define EOF_CHR     0xFF    /* End of file */
#define UND_CHR     '_'     /* Underscore */
#define AMP_CHR     '&'     /* Ampersand for method names */
#define QUT_CHR     '\"'    /* Quote */
#define HST_CHR     '#'     /* Hash for comments */
#define TAB_CHR     '\t'    /* Tab */
#define SPC_CHR     ' '     /* Space */
#define NWL_CHR     '\n'    /* Newline */
#define SCL_CHR     ';'     /* Semicolon */
#define LPR_CHR     '('     /* Left parenthesis */
#define RPR_CHR     ')'     /* Right parenthesis */
#define LBR_CHR     '{'     /* Left brace */
#define RBR_CHR     '}'     /* Right brace */
#define COM_CHR     ','     /* Comma */
#define DOT_CHR     '.'     /* Dot for floats */

/* Mathematical Operator Characters */
#define PLUS_CHR    '+'     /* Addition */
#define MINUS_CHR   '-'     /* Subtraction */
#define MUL_CHR     '*'     /* Multiplication */
#define DIV_CHR     '/'     /* Division */
#define MOD_CHR     '%'     /* Modulo */
#define PWR_CHR     '^'     /* Power */
#define EQ_CHR      '='     /* Assignment/Equality */
#define LT_CHR      '<'     /* Less than */
#define GT_CHR      '>'     /* Greater than */
#define NOT_CHR     '!'     /* Logical NOT */
#define AND_CHR     '&'     /* Used in && */
#define OR_CHR      '|'     /* Used in || */

/* DFA States for mplusplus Math DSL */
#define ESNR        8       /* Error state with no retract */
#define ESWR        9       /* Error state with retract */
#define FS          -1      /* Final state marker */

/* DFA Configuration */
#define NUM_STATES  10      /* Total number of states */
#define CHAR_CLASSES 9      /* Number of character classes INCLUDING DOT */

/* Character Class Indices */
#define COL_LETTER      0   /* [a-zA-Z] */
#define COL_DIGIT       1   /* [0-9] */
#define COL_UNDERSCORE  2   /* _ */
#define COL_AMPERSAND   3   /* & */
#define COL_QUOTE       4   /* " */
#define COL_DOT         5   /* . */
#define COL_EOF         6   /* EOF */
#define COL_HASH        7   /* # */
#define COL_OTHER       8   /* everything else */

/* COMPLETE: DFA Transition Table with PROPER FLOAT support */
static mplusplus_intg transitionTable[NUM_STATES][CHAR_CLASSES] = {
    /*       L   D   _   &   "   .   E   #   O */
    /* S0 */{1,  2,  1,  1,  5,  ESNR, ESWR, 6, ESNR},
    /* S1 */{1,  1,  1,  3,  4,  4,  4,  4,  4},
    /* S2 */{4,  2,  4,  4,  4,  7,  4,  4,  4},    
    /* S3 */{4,  4,  4,  4,  4,  4,  4,  4,  4},
    /* S4 */{ESNR, ESNR, ESNR, ESNR, ESNR, ESNR, ESNR, ESNR, ESNR},
    /* S5 */{5,  5,  5,  5,  4,  5,  ESWR, 5,  5},
    /* S6 */{6,  6,  6,  6,  6,  6,  ESWR, 4,  6},
    /* S7 */{4,  7,  4,  4,  4,  4,  4,  4,  4},    
    /* S8 */{ESNR, ESNR, ESNR, ESNR, ESNR, ESNR, ESNR, ESNR, ESNR},
    /* S9 */{ESNR, ESNR, ESNR, ESNR, ESNR, ESNR, ESNR, ESNR, ESNR}
};

/* State Types for mplusplus Math DSL */
#define NOFS    0       /* not accepting state */
#define FSNR    1       /* accepting state with no retract */
#define FSWR    2       /* accepting state with retract */

static mplusplus_intg stateType[NUM_STATES] = {
    NOFS,   /* 00 - Initial state */
    NOFS,   /* 01 - Building identifier */
    FSWR,   /* 02 - Integer literal */
    FSNR,   /* 03 - Method name (ends with &) */
    FSWR,   /* 04 - Identifier/Keyword/String end */
    NOFS,   /* 05 - Building string */
    NOFS,   /* 06 - Building comment */
    FSWR,   /* 07 - Float literal - CRITICAL: FSWR for proper recognition */
    FSNR,   /* 08 - Error state no retract */
    FSWR    /* 09 - Error state with retract */
};

/* Function Prototypes for mplusplus Math DSL */
mplusplus_intg startScanner(BufferPointer psc_buf);
static mplusplus_intg nextClass(mplusplus_char c);
static mplusplus_intg nextState(mplusplus_intg state, mplusplus_char c);
mplusplus_void printScannerData(ScannerData scData);
Token tokenizer(mplusplus_void);

/* Function Pointer Type for Accepting Functions */
typedef Token(*PTR_ACCFUN)(mplusplus_strg lexeme);

/* Accepting State Function Declarations */
Token funcID(mplusplus_strg lexeme);    /* Identifiers and keywords */
Token funcIL(mplusplus_strg lexeme);    /* Integer literals */
Token funcFLT(mplusplus_strg lexeme);   /* Float literals */
Token funcSL(mplusplus_strg lexeme);    /* String literals */
Token funcCMT(mplusplus_strg lexeme);   /* Comments */
Token funcKEY(mplusplus_strg lexeme);   /* Keywords */
Token funcErr(mplusplus_strg lexeme);   /* Error handling */

/* COMPLETE: Final State Table with proper float handling */
static PTR_ACCFUN finalStateTable[NUM_STATES] = {
    NULL,       /* 00 - Initial state */
    NULL,       /* 01 - Building identifier */
    funcIL,     /* 02 - Integer literal */
    funcID,     /* 03 - Method name */
    funcKEY,    /* 04 - Identifier/Keyword/String end */
    NULL,       /* 05 - Building string */
    NULL,       /* 06 - Building comment */
    funcFLT,    /* 07 - Float literal - CRITICAL: calls funcFLT */
    funcErr,    /* 08 - Error state no retract */
    funcErr     /* 09 - Error state with retract */
};

/* FIXED: Keywords for mplusplus Math DSL - Complete list with "let" */
#define KWT_SIZE 12

static mplusplus_strg keywordTable[KWT_SIZE] = {
    "let",      /* Variable declaration - CRITICAL: Added let as first entry */
    "if",       /* Conditional */
    "else",     /* Alternative */
    "while",    /* Loop */
    "return",   /* Return statement */
    "solve",    /* Solve equation */
    "derive",   /* Derivative */
    "pi",       /* Mathematical constant π */
    "e",        /* Mathematical constant e */
    "sin",      /* Sine function */
    "cos",      /* Cosine function */
    "log"       /* Logarithm function */
};

/* Mathematical Method Names for mplusplus DSL */
static mplusplus_strg mathMethodTable[] = {
    "sqrt&",     /* Square root */
    "pow&",      /* Power function */
    "abs&",      /* Absolute value */
    "sin&",      /* Sine */
    "cos&",      /* Cosine */
    "tan&",      /* Tangent */
    "log&",      /* Logarithm */
    "ln&",       /* Natural logarithm */
    "exp&",      /* Exponential */
    "print&",    /* Output function */
    "solve&",    /* Equation solver */
    "derive&",   /* Derivative calculator */
    "integrate&" /* Integration calculator */
};

/* Language Attributes for mplusplus Math DSL */
#define INDENT TAB_CHR

typedef struct languageAttributes {
    mplusplus_char indentationCharType;
    mplusplus_intg indentationCurrentPos;
    mplusplus_intg mathematicalDepth;       /* Nesting level for mathematical expressions */
    mplusplus_intg equationCount;           /* Number of equations processed */
} LanguageAttributes;

/* Global Scanner Variables */
extern mplusplus_intg numScannerErrors;
extern ScannerData scData;

#endif