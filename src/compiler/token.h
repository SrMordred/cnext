#pragma once

#include "std/util.h"



typedef enum {
    TK_START , 
    TK_OPEN_PAREN , 
    TK_CLOSE_PAREN , 
    TK_OPEN_BRACE , 
    TK_CLOSE_BRACE , 
    TK_OPEN_BRACKET , 
    TK_CLOSE_BRACKET , 
    TK_COMMA , 
    TK_DOT , 
    
    TK_SEMICOLON , 
    TK_COLON , 

    TK_LESS , 
    TK_GREATER , 
    TK_LESS_EQUAL , 
    TK_GREATER_EQUAL , 
   
    TK_NOT , 
    TK_BITNOT ,  //~
    TK_NOT_EQUAL , 
    TK_EQUAL_EQUAL , 
    TK_IDENTIFIER , 
    TK_STRING , 
    TK_NUMBER , 
    TK_FALSE , 
    TK_TRUE , 
    TK_NULL , 
    TK_ARROW , 
    TK_AUTO , 
    TK_BREAK , 
    TK_CASE , 
    TK_CONST , 
    TK_CONTINUE , 
    TK_DEFAULT , 
    TK_DO , 
    TK_ELSE , 
    TK_ENUM , 
    TK_EXTERN , 
    TK_FOR , 
    TK_GOTO , 
    TK_IF , 
    TK_REGISTER , 
    TK_RETURN , 
    TK_SIGNED , 
    TK_SIZEOF , 
    TK_STATIC , 
    TK_STRUCT , 
    TK_SWITCH , 
    TK_TYPEDEF , 
    TK_UNION , 
    TK_UNSIGNED , 
    TK_VOLATILE , 
    TK_WHILE , 
    TK_CHAR , 
    TK_DOUBLE , 
    TK_FLOAT , 
    TK_INT , 
    TK_LONG , 
    TK_SHORT , 
    TK_VOID , 

    TK_QUESTION , 

    TK_RSHIFT,
    TK_LSHIFT,

    TK_PLUS_PLUS,
    TK_MINUS_MINUS,
    /*
    BINARY OPERATORS
    '-' | '+' | '/' | '*' | '%' | '&' | '|' | '^' | '||' | '&&'
    */
    TK_MINUS , 
    TK_PLUS , 
    TK_SLASH , 
    TK_STAR , 
    TK_MOD,
    TK_BITAND,
    TK_BITOR,
    TK_XOR,
    TK_OR,
    TK_AND,

    /*
    ASSIGN OPERATORS
    '=' | '*=' | '/=' | '%=' | '+=' | '-=' | '<<=' | '>>=' | '&=' | '^=' | '|='\
    */
    TK_EQUAL , 
    TK_STAR_EQUAL , 
    TK_SLASH_EQUAL , 
    TK_MOD_EQUAL , 
    TK_PLUS_EQUAL , 
    TK_MINUS_EQUAL , 
    TK_LSHIFT_EQUAL , 
    TK_RSHIFT_EQUAL , 
    TK_AND_EQUAL , 
    TK_XOR_EQUAL , 
    TK_OR_EQUAL , 

    TK_EOF 
} TK ;

const char * const TK_STR[] = {
    "TK_START" , 
    "TK_OPEN_PAREN" , 
    "TK_CLOSE_PAREN" , 
    "TK_OPEN_BRACE" , 
    "TK_CLOSE_BRACE" , 
    "TK_OPEN_BRACKET" , 
    "TK_CLOSE_BRACKET" , 
    "TK_COMMA" , 
    "TK_DOT" , 
    
    "TK_SEMICOLON" , 
    "TK_COLON" , 
   
    "TK_LESS" , 
    "TK_GREATER" , 
    "TK_LESS_EQUAL" , 
    "TK_GREATER_EQUAL" , 

    "TK_NOT" , 
    "TK_BITNOT" ,
    "TK_NOT_EQUAL" , 
    "TK_EQUAL_EQUAL" , 
    "TK_IDENTIFIER" , 
    "TK_STRING" , 
    "TK_NUMBER" , 
    "TK_FALSE" , 
    "TK_TRUE" , 
    "TK_NULL" , 
    "TK_ARROW" , 
    "TK_AUTO" , 
    "TK_BREAK" , 
    "TK_CASE" , 
    "TK_CONST" , 
    "TK_CONTINUE" , 
    "TK_DEFAULT" , 
    "TK_DO" , 
    "TK_ELSE" , 
    "TK_ENUM" , 
    "TK_EXTERN" , 
    "TK_FOR" , 
    "TK_GOTO" , 
    "TK_IF" , 
    "TK_REGISTER" , 
    "TK_RETURN" , 
    "TK_SIGNED" , 
    "TK_SIZEOF" , 
    "TK_STATIC" , 
    "TK_STRUCT" , 
    "TK_SWITCH" , 
    "TK_TYPEDEF" , 
    "TK_UNION" , 
    "TK_UNSIGNED" , 
    "TK_VOLATILE" , 
    "TK_WHILE" , 
    "TK_CHAR" , 
    "TK_DOUBLE" , 
    "TK_FLOAT" , 
    "TK_INT" , 
    "TK_LONG" , 
    "TK_SHORT" , 
    "TK_VOID" , 

    "TK_QUESTION" , 

    "TK_RSHIFT",
    "TK_LSHIFT",

    "TK_PLUS_PLUS",
    "TK_MINUS_MINUS",
    /*
    BINARY OPERATORS
    '-' | '+' | '/' | '*' | '%' | '&' | '|' | '^' | '||' | '&&'
    */
    "TK_MINUS" , 
    "TK_PLUS" , 
    "TK_SLASH" , 
    "TK_STAR" , 
    "TK_MOD",
    "TK_BITAND",
    "TK_BITOR",
    "TK_XOR",
    "TK_OR",
    "TK_AND",

    /*
    ASSIGN OPERATORS
    '=' | '*=' | '/=' | '%=' | '+=' | '-=' | '<<=' | '>>=' | '&=' | '^=' | '|='\
    */
    "TK_EQUAL" , 
    "TK_STAR_EQUAL" , 
    "TK_SLASH_EQUAL" , 
    "TK_MOD_EQUAL" , 
    "TK_PLUS_EQUAL" , 
    "TK_MINUS_EQUAL" , 
    "TK_LSHIFT_EQUAL" , 
    "TK_RSHIFT_EQUAL" , 
    "TK_AND_EQUAL" , 
    "TK_XOR_EQUAL" , 
    "TK_OR_EQUAL" , 

    "TK_EOF" 
};

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