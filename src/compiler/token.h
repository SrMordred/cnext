#pragma once

#include "std/util.h"

#define ENUMS \
	__ENUM( TK_START ) \
	__ENUM( TK_OPEN_PAREN ) \
	__ENUM( TK_CLOSE_PAREN ) \
	__ENUM( TK_OPEN_BRACE ) \
	__ENUM( TK_CLOSE_BRACE ) \
	__ENUM( TK_OPEN_BRACKET ) \
	__ENUM( TK_CLOSE_BRACKET ) \
	__ENUM( TK_COMMA ) \
	__ENUM( TK_DOT ) \
	__ENUM( TK_MINUS ) \
	__ENUM( TK_PLUS ) \
	__ENUM( TK_SEMICOLON ) \
	__ENUM( TK_SLASH ) \
	__ENUM( TK_STAR ) \
	__ENUM( TK_NOT ) \
	__ENUM( TK_NOT_EQUAL ) \
	__ENUM( TK_EQUAL ) \
	__ENUM( TK_EQUAL_EQUAL ) \
	__ENUM( TK_GREATER ) \
	__ENUM( TK_GREATER_EQUAL ) \
	__ENUM( TK_LESS ) \
	__ENUM( TK_LESS_EQUAL ) \
	__ENUM( TK_IDENTIFIER ) \
	__ENUM( TK_STRING ) \
	__ENUM( TK_NUMBER ) \
	__ENUM( TK_FALSE ) \
	__ENUM( TK_TRUE ) \
	__ENUM( TK_NULL ) \
	__ENUM( TK_ARROW ) \
	/* C KEYWORDS */ \
	__ENUM( TK_AUTO ) \
	__ENUM( TK_BREAK ) \
	__ENUM( TK_CASE ) \
	__ENUM( TK_CONST ) \
	__ENUM( TK_CONTINUE ) \
	__ENUM( TK_DEFAULT ) \
	__ENUM( TK_DO ) \
	__ENUM( TK_ELSE ) \
	__ENUM( TK_ENUM ) \
	__ENUM( TK_EXTERN ) \
	__ENUM( TK_FOR ) \
	__ENUM( TK_GOTO ) \
	__ENUM( TK_IF ) \
	__ENUM( TK_REGISTER ) \
	__ENUM( TK_RETURN ) \
	__ENUM( TK_SIGNED ) \
	__ENUM( TK_SIZEOF ) \
	__ENUM( TK_STATIC ) \
	__ENUM( TK_STRUCT ) \
	__ENUM( TK_SWITCH ) \
	__ENUM( TK_TYPEDEF ) \
	__ENUM( TK_UNION ) \
	__ENUM( TK_UNSIGNED ) \
	__ENUM( TK_VOLATILE ) \
	__ENUM( TK_WHILE ) \
	/* C DATA TYPES */ \
	__ENUM( TK_CHAR ) \
	__ENUM( TK_DOUBLE ) \
	__ENUM( TK_FLOAT ) \
	__ENUM( TK_INT ) \
	__ENUM( TK_LONG ) \
	__ENUM( TK_SHORT ) \
	__ENUM( TK_VOID ) \
	\
	__ENUM( TK_EOF ) \

#define __ENUM(x) x,
typedef enum { ENUMS TOP } TK ;
#undef __ENUM

#define __ENUM(x) #x,    
const char * const TK_STR[] = { ENUMS };
#undef __ENUM

unsigned long dbj2(unsigned char *str ,int len)
{
    unsigned long hash = 192;
    int c;

    while(len-- && (c = *str++))
        hash = ((hash << 20) + hash) + c;

    return hash % 66;
}

const char* keywords[] = {
        [0] = "",
        [1] = "",
        [2] = "do",
        [3] = "",
        [4] = "default",
        [5] = "static",
        [6] = "",
        [7] = "",
        [8] = "",
        [9] = "",
        [10] = "while",
        [11] = "double",
        [12] = "",
        [13] = "",
        [14] = "typedef",
        [15] = "for",
        [16] = "float",
        [17] = "auto",
        [18] = "",
        [19] = "",
        [20] = "",
        [21] = "union",
        [22] = "",
        [23] = "",
        [24] = "",
        [25] = "",
        [26] = "sizeof",
        [27] = "register",
        [28] = "void",
        [29] = "",
        [30] = "",
        [31] = "unsigned",
        [32] = "struct",
        [33] = "int",
        [34] = "",
        [35] = "",
        [36] = "break",
        [37] = "",
        [38] = "enum",
        [39] = "",
        [40] = "",
        [41] = "",
        [42] = "goto",
        [43] = "case",
        [44] = "short",
        [45] = "continue",
        [46] = "switch",
        [47] = "",
        [48] = "extern",
        [49] = "",
        [50] = "char",
        [51] = "signed",
        [52] = "",
        [53] = "",
        [54] = "",
        [55] = "volatile",
        [56] = "",
        [57] = "return",
        [58] = "",
        [59] = "else",
        [60] = "",
        [61] = "if",
        [62] = "",
        [63] = "const",
        [64] = "",
        [65] = "long",
        [66] = "",
};

const TK tk_keywords[] = {
        [2] = TK_DO,
        [4] = TK_DEFAULT,
        [5] = TK_STATIC,
        [10] = TK_WHILE,
        [11] = TK_DOUBLE,
        [14] = TK_TYPEDEF,
        [15] = TK_FOR,
        [16] = TK_FLOAT,
        [17] = TK_AUTO,
        [21] = TK_UNION,
        [26] = TK_SIZEOF,
        [27] = TK_REGISTER,
        [28] = TK_VOID,
        [31] = TK_UNSIGNED,
        [32] = TK_STRUCT,
        [33] = TK_INT,
        [36] = TK_BREAK,
        [38] = TK_ENUM,
        [42] = TK_GOTO,
        [43] = TK_CASE,
        [44] = TK_SHORT,
        [45] = TK_CONTINUE,
        [46] = TK_SWITCH,
        [48] = TK_EXTERN,
        [50] = TK_CHAR,
        [51] = TK_SIGNED,
        [55] = TK_VOLATILE,
        [57] = TK_RETURN,
        [59] = TK_ELSE,
        [61] = TK_IF,
        [63] = TK_CONST,
        [65] = TK_LONG,
};
                                                             
typedef struct 
{
	TK         type;
	StringView text;
	int        line;
	union
    {
        bool    _bool;
        int     _int;
        char*   _string;
    };

} Token;