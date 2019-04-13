#include <stdbool.h>
#include <ctype.h>

#include "stdlib/string.h"
#include "stdlib/util.h"
#include "stdlib/io.h"
#include "stdlib/array.h"
#include "stdlib/util.h"



#define ENUMS \
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
	__ENUM( TK_AND ) \
	__ENUM( TK_CLASS ) \
	__ENUM( TK_ELSE ) \
	__ENUM( TK_FALSE ) \
	__ENUM( TK_FUN ) \
	__ENUM( TK_FOR ) \
	__ENUM( TK_IF ) \
	__ENUM( TK_NIL ) \
	__ENUM( TK_OR ) \
	__ENUM( TK_PRINT ) \
	__ENUM( TK_RETURN ) \
	__ENUM( TK_SUPER ) \
	__ENUM( TK_THIS ) \
	__ENUM( TK_TRUE ) \
	__ENUM( TK_VAR ) \
	__ENUM( TK_WHILE ) \
	__ENUM( TK_EOF ) \
	
#define __ENUM(x) x,
typedef enum { ENUMS TOP } TK ;
#undef __ENUM

#define __ENUM(x) #x,    
const char * const TK_TO_STR[] = { ENUMS };
#undef __ENUM
                 
                                             


typedef struct 
{
	TK 		type;
	char* 	text;
	int     len;
	int     line;
	union {
		int _int;
	} value;
	

} Token;

Token* token_array;


//PARSER
char* start;
char* current;
int line    = 1;

void add_token( TK type )
{
	Token t = { type, start , current - start , line, {} };
	array_push(token_array, t );
}

void add_token_int( TK type, int value )
{
	Token t = { type, start , current - start , line, { ._int = value } };
	array_push(token_array, t );
}

void parser_error(char* msg)
{
	printf("%s\n", msg);
}


char next()
{ 
	return *current++;
}

bool is_eof()
{ 
	return *current == '\0'; 
}

bool match(char expected)
{
	if( is_eof() ) return false;
	if( *current != expected ) return false;
	++current;
	return true;
}

void string_scan()
{                                   
    while (*current != '"' && !is_eof()) 
	{                   
		if (*current == '\n') line++;                           
		next();                                            
    }
    if (is_eof())
	{                                        
		parser_error("Unterminated string.");
		return;                                               
    }                                                       
	next();
    add_token(TK_STRING);                                
}   

void number_scan() {                                     
    while (isdigit(*current)) next();

                                
    if (*current == '.' && isdigit( *(current + 1) ) ) 
	{               
     	next();                                              
      	while (isdigit(*current) ) next();                      
    }                                                         

    add_token_int(TK_NUMBER, strtol(start, &current , 10 ));
}

void id_scan() {                
    while ( isalnum(*current) || *current == '_' ) next();

	//TODO: keyword checking

    add_token(TK_IDENTIFIER);                    
} 

void print_tokens()
{
	foreach( tk, token_array, {
		printf("%s %.*s line: %d \n", TK_TO_STR[ tk->type ], tk->len + 1, tk->text, tk->line);
	});
}



void scan()
{
	char c = next();

	switch(c)
	{
		case '(': add_token( TK_OPEN_PAREN ) ; break;
		case ')': add_token( TK_CLOSE_PAREN ) ; break;
		case '[': add_token( TK_OPEN_BRACKET ) ; break;
		case ']': add_token( TK_OPEN_BRACKET ) ; break;
		case '{': add_token( TK_OPEN_BRACE ) ; break;
		case '}': add_token( TK_OPEN_BRACE ) ; break;

		case ',': add_token( TK_COMMA ) ; break;
		case '.': add_token( TK_DOT ) ; break;

		case '+': add_token( TK_PLUS ) ; break;
		case '-': add_token( TK_MINUS ) ; break;
		case '*': add_token( TK_STAR ) ; break;
		case '/':                                                       
        	if (match('/'))//comment line
			{                                             
          		while (*current != '\n' && !is_eof()) next();             
        	}
			else                                                      
          		add_token( TK_SLASH );                                            
        break;  

		case '=': add_token( match('=') ? TK_EQUAL_EQUAL : TK_EQUAL ) ; break;
		case '!': add_token( match('=') ? TK_NOT_EQUAL : TK_NOT ) ; break;
		case '>': add_token( match('=') ? TK_GREATER_EQUAL : TK_GREATER ) ; break;
		case '<': add_token( match('=') ? TK_LESS_EQUAL : TK_LESS ) ; break;

		case '"': string_scan(); break;


		//space ignoring
		case ' ':                                    
      	case '\r':                                   
      	case '\t':                                   
        break;

		//new line
		case '\n': line++; break; 

		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			number_scan();
		break;

		case 'a' : case 'b' : case 'c' : case 'd' : case 'e' : case 'f' : case 'g' : case 'h' : 
		case 'i' : case 'j' : case 'k' : case 'l' : case 'm' : case 'n' : case 'o' : case 'p' : 
		case 'q' : case 'r' : case 's' : case 't' : case 'u' : case 'v' : case 'w' : case 'x' : 
		case 'y' : case 'z' : case '_' : 
			id_scan();
		break;
		

		default:
			printf("Unexpected character : '%c'", c);
		break;
		
	}
}



int main(int argc, char *argv[]) {
	(void) argv;
	(void) argc;

	string file = file_to_string("script.cn");
	token_array = array_create();

	current = file;

	while( !is_eof() )
	{
		start = current;
		scan();
	}

	print_tokens();


	// foreach( c , file , printf("%c|", *c); );
	
	// string_print(file);
	string_free(file);

	
	
	return 0;

}