#pragma once

#include <ctype.h>
#include <setjmp.h>

#include "std/types.h"
#include "std/error.h"
#include "std/string.h"
#include "std/util.h"
#include "std/io.h"
#include "std/array.h"
#include "std/util.h"
#include "compiler/token.h"


struct 
{
	char* start;
	char* current;
	int   line;

	StringView file;
	char*      file_name;
	Array      token_array;

} Lexer;

void lexer_add_token( TK type )
{
	Token t = { type, string_view_from( Lexer.start , Lexer.current - Lexer.start ) , Lexer.line, {} };
	array_push(&Lexer.token_array, &t );
}

void lexer_add_token_string( TK type )
{
	Token t = { type, string_view_from(Lexer.start + 1, Lexer.current - Lexer.start - 2 ) , Lexer.line, {} };
	array_push(&Lexer.token_array, &t );
}

void lexer_add_token_int( TK type, int value )
{
	Token t = { type, string_view_from( Lexer.start, Lexer.current - Lexer.start -1 ) , Lexer.line, { ._int = value } };
	array_push(&Lexer.token_array, &t );
}

void lexer_error(char* msg)
{
	printf("%s:%d -> Error: %s\n", 
		Lexer.file_name,
		Lexer.line,
		msg
	);
	printf("\n");

	char* start   = Lexer.current-1;
	char* end     = start;
	while(*start != 0 && *start != '\n' && *start != '\r')--start;
	++start;
	while(*end != 0 && *end != '\n'  && *end != '\r' )++end;

	int len = (int)(end - start);

	int digits = 0;
	int line = Lexer.line;
	while(line != 0)
    {
        line /= 10;
        ++digits;
    }

	printf("%d | %.*s\n", Lexer.line, len, start );
	printf("%*s\n", len+digits+3, "^" );

	error_throw( 0 );

}

char lexer_next()
{ 
	return *Lexer.current++;
}

bool lexer_is_eof()
{ 
	return *Lexer.current == '\0'; 
}

bool lexer_match(char expected)
{
	if( lexer_is_eof() ) return false;
	if( *Lexer.current != expected ) return false;
	++Lexer.current;
	return true;
}

void lexer_string_scan()
{                                   
    while (*Lexer.current != '"' && !lexer_is_eof()) 
	{                   
		if (*Lexer.current == '\n') Lexer.line++;                           
		lexer_next();                                            
    }
    if (lexer_is_eof())
	{                                        
		lexer_error("Unterminated string.");
		return;                                               
    }                                                       
	lexer_next();
    lexer_add_token_string(TK_STRING);                                
}   

void lexer_number_scan() {                                     
    while (isdigit(*Lexer.current)) lexer_next();

                                
    if (*Lexer.current == '.' && isdigit( *(Lexer.current + 1) ) ) 
	{               
     	lexer_next();                                              
      	while (isdigit(*Lexer.current) ) lexer_next();                      
    }                                                         

    lexer_add_token_int(TK_NUMBER, strtol(Lexer.start, &Lexer.current , 10 ));
}

void lexer_identifier_scan() {                
    while ( isalnum(*Lexer.current) || *Lexer.current == '_' ) lexer_next();

    char* id = Lexer.start;
    int len  = Lexer.current - Lexer.start;

    unsigned long index = dbj2( (unsigned char*)id, len );
    const char* hash = keywords[index];
    if( strncmp( id, hash, len ) == 0 ){
    	lexer_add_token( tk_keywords[ index ] );
    }
    else{
    	lexer_add_token(TK_IDENTIFIER);                    
    }
} 

void lexer_scan()
{
	char c = lexer_next();

	switch(c)
	{
		case '(': lexer_add_token( TK_OPEN_PAREN ) ; break;
		case ')': lexer_add_token( TK_CLOSE_PAREN ) ; break;
		case '[': lexer_add_token( TK_OPEN_BRACKET ) ; break;
		case ']': lexer_add_token( TK_OPEN_BRACKET ) ; break;
		case '{': lexer_add_token( TK_OPEN_BRACE ) ; break;
		case '}': lexer_add_token( TK_OPEN_BRACE ) ; break;

		case ',': lexer_add_token( TK_COMMA ) ; break;
		case '.': lexer_add_token( TK_DOT ) ; break;

		case '+': lexer_add_token( TK_PLUS ) ; break;
		case '-': 
			if( lexer_match('>') )
			{
				lexer_add_token( TK_ARROW );
			}
			else
				lexer_add_token( TK_MINUS ) ; 
		break;
		case '*': lexer_add_token( TK_STAR ) ; break;


		case '/':   
			//SINGLE LINE COMMENT
        	if (lexer_match('/'))//comment line
			{        
          		while (*Lexer.current != '\n' && !lexer_is_eof())
				{ 
					lexer_next();             
				} 
        	}
        	//BLOCK COMMENT
        	else if( lexer_match('*') )
        	{
          		while ( (*Lexer.current != '*' && *(Lexer.current + 1) != '/') && !lexer_is_eof())
          		{
          			lexer_next();
          		}

          		if(lexer_is_eof())
          		{
          			lexer_error("Unexpected end of comment");
          		}
          		lexer_next();
          		lexer_next();
        	}
			else                                                      
          		lexer_add_token( TK_SLASH );                                            
        break;  

		case '=': lexer_add_token( lexer_match('=') ? TK_EQUAL_EQUAL : TK_EQUAL ) ; break;
		case '!': lexer_add_token( lexer_match('=') ? TK_NOT_EQUAL : TK_NOT ) ; break;
		case '>': lexer_add_token( lexer_match('=') ? TK_GREATER_EQUAL : TK_GREATER ) ; break;
		case '<': lexer_add_token( lexer_match('=') ? TK_LESS_EQUAL : TK_LESS ) ; break;

		case '"': lexer_string_scan(); break;

		case ';': lexer_add_token( TK_SEMICOLON ); break;


		//space ignoring
		case ' ':                                    
      	case '\r':                                   
      	case '\t':                                   
        break;

		//new line
		case '\n': Lexer.line++; break; 

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
			lexer_number_scan();
		break;

		case 'a' : case 'b' : case 'c' : case 'd' : case 'e' : case 'f' : case 'g' : case 'h' : 
		case 'i' : case 'j' : case 'k' : case 'l' : case 'm' : case 'n' : case 'o' : case 'p' : 
		case 'q' : case 'r' : case 's' : case 't' : case 'u' : case 'v' : case 'w' : case 'x' : 
		case 'y' : case 'z' : case '_' : 
			lexer_identifier_scan();
		break;

		default:
			lexer_error("Unexpected character");
			// printf("Unexpected character : '%c'", c);
		break;
		
	}
}


StringView lexer_file_to_string(char* file_name)
{
	FILE* file = fopen( file_name , "rb");
	if(!file) 
	{
        perror("File opening failed");
		return (StringView){NULL,0};
    }

	fseek(file, 0, SEEK_END);
	long file_size = ftell(file);
	fseek(file, 0, SEEK_SET);
	
	String file_string = string_new_cap( file_size + 2 );
	fread(file_string.ptr + 1 , 1, file_size, file);

	StringView view = string_view_from( file_string.ptr, file_size );
	//file starting and ending with \n, 
	// useful on parser later, to limit start and endof lines
	char* ptr          = view.ptr;
	ptr[0]             = '\n';
	ptr[file_size + 1] = '\n';
	ptr[file_size + 2] = 0;

	fclose(file);
	return view;
}


void lexer_load_file(char* file_name)
{

	Lexer.token_array 	= array( sizeof( Token ) );
    Lexer.file 			= lexer_file_to_string( file_name );


    Lexer.file_name     = file_name;
	Lexer.current 		= Lexer.file.ptr + 1;
	Lexer.line    		= 1;

	Token t = { TK_START, { Lexer.current, 0 } , Lexer.line, {} };
	array_push(&Lexer.token_array, &t );
	// lexer_add_token( TK_START );

	error_catch( 0, END );

    while( !lexer_is_eof() )
	{
		Lexer.start = Lexer.current;
		lexer_scan();
	}

	END:

	t = (Token){ TK_EOF, {0} , Lexer.line, {} };
	array_push(&Lexer.token_array, &t );
}

void lexer_print_token(Token* tk)
{
	printf("%s %.*s line: %d \n", TK_STR[ tk->type ], (int)tk->text.len, tk->text.ptr, tk->line);
}

void lexer_print_tokens()
{
    foreach( Token, tk, Lexer.token_array, {
		printf("%s %.*s line: %d \n", TK_STR[ tk.type ], (int)tk.text.len, tk.text.ptr, tk.line);
	});
}

Array lexer_get_tokens()
{
	return Lexer.token_array;
}