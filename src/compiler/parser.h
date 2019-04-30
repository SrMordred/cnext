#pragma once

#include "compiler/lexer.h"
#include "compiler/node.h"


#define KB( SIZE ) (   (SIZE) * 1024)
#define MB( SIZE ) ((KB(SIZE)) * 1024)
#define GB( SIZE ) ((MB(SIZE)) * 1024)

/*

expression      -> equality ;
equality        -> comparison ( ( "!=" | "==" ) comparison )* ;
comparison      -> addition ( ( ">" | ">=" | "<" | "<=" ) addition )* ;
addition        -> multiplication ( ( "-" | "+" ) multiplication )* ;
multiplication  -> unary ( ( "/" | "*" ) unary )* ;
unary           -> ( "!" | "-" ) unary | primary ;
primary         -> NUMBER | STRING | "false" | "true" | "(" expression ")" | identifier ;

program         -> statement* EOF ;
statement       -> var_declaration ;
var_declaration -> type comma_statement ";";
comma_statement -> identifier ( "=" expression )? ( "," comma_statement )?

*/

struct 
{
    Token*          current;
    Array           ast;
    char*           file_name;
} Parser;


Token* parser_current()
{
    return Parser.current;
}

bool parser_is_eof()
{
    return parser_current()->type == TK_EOF;
}

Token* parser_next()
{
    //TODO: maybe there is a way to eliminate this if, there is a possiblity in lexer.h also
    if (!parser_is_eof()) Parser.current++;
    return Parser.current-1;    
}

Token* parser_previous()
{
    return parser_current()-1;    
}


bool parser_check(TK type)
{
    
    return parser_current()->type == type;          
} 

int parser_match( TK tk )
{
    if (parser_is_eof()) return false;         
    if( parser_current()->type == tk ){
        parser_next();
        return true;
    }
    return false;
}

int parser_match_n( int len, const TK * const array )
{
    if (parser_is_eof()) return false;         

    while(len--)
    {
        if( parser_current()->type == array[len] ){
            parser_next();
            return true;
        }
    }
    return false;
} 

void parser_resync()
{
    parser_next();

    while (!parser_is_eof())
    {                       
        if ( parser_previous()->type == TK_SEMICOLON) return;

        switch ( parser_current()->type)
        {              
            case TK_CHAR:
            case TK_DOUBLE:
            case TK_FLOAT:
            case TK_INT:
            case TK_LONG:
            case TK_SHORT:
            case TK_VOID:
            case TK_FOR:                              
            case TK_IF:                               
            case TK_WHILE:                            
            case TK_RETURN:                           
            return;                                 
            default: {}
        }                                        
        parser_next();
    }         
}

void parser_error( char* msg )
{
    Token* current = parser_current();
    printf("%s:%d -> Error: %s\n", 
        Parser.file_name,
        current->line,
        msg
    );
    printf("\n");

    char* start   = (char*)current->text.ptr;
    char* end     = start;
    while(*start != 0 && *start != '\n' && *start != '\r')--start;
    ++start;
    while(*end != 0 && *end != '\n'  && *end != '\r' )++end;

    int line_len  = (int)(end - start);
    int start_len = (int)((char*)current->text.ptr - start);

    int digits = 0;
    int line = Lexer.line;
    while(line != 0)
    {
        line /= 10;
        ++digits;
    }

    printf("%d | %.*s\n", current->line, line_len, start );
    printf("%*s\n", start_len + 1 + digits + 3, "^" );

    parser_resync();
}

Token* parser_consume(TK type, char* msg) 
{
    if (parser_check(type)) return parser_next();

    parser_error( msg );  
    exit(EXIT_FAILURE);
}  

Expression* parser_expression();

Expression* parser_primary() 
{        

    if (parser_match(TK_FALSE))
    {
        return NodeBool(false);
    } 
    if (parser_match(TK_TRUE))
    {
        return NodeBool(true);
    }
    if (parser_match(TK_NULL))
    {
        //TODO: NULL
        return NodeBool(false);
    }

    if (parser_match(TK_STRING))
    {                           
        const Token* previous = parser_previous();
        return NodeString( previous->text );
    }

    if (parser_match(TK_NUMBER))
    {
        return NodeInt( parser_previous()->_int ) ;
    }                                                      

    if (parser_match(TK_OPEN_PAREN)) 
    {                               
        Expression* expression = parser_expression();                            
        parser_consume(TK_CLOSE_PAREN, "Expect ')' after expression.");
        return NodeGrouping( expression );
    }     

    if( parser_match(TK_IDENTIFIER) ) 
    {
        // const Token* previous = parser_previous();
        return NodeIdentifier( parser_previous()->text );
    }
    parser_error( "Expected expression.");
    exit(EXIT_FAILURE);
    return NULL;
}

Expression* parser_unary() 
{                     
    if (parser_match_n(2,(TK[]){TK_NOT, TK_MINUS}))
    {                
        TK          operator = parser_previous()->type;
        Expression* right    = parser_unary();
        return      NodeUnary(right, operator);
    }

    return parser_primary();                        
} 

Expression* parser_multiplication() 
{                   
    Expression* left = parser_unary();                            

    while (parser_match_n(2, (TK[]){TK_SLASH, TK_STAR}))
    {                    
        TK operator       = parser_previous()->type;
        Expression* right = parser_unary();
        left              = NodeBinary( left, right, operator );
    }                                               

    return left;                                    
  } 

Expression* parser_addition() 
{                         
    Expression* left = parser_multiplication();

    while (parser_match_n(2, (TK[]){ TK_MINUS, TK_PLUS }))
    {                    
        TK operator       = parser_previous()->type;
        Expression* right = parser_multiplication();
        left              = NodeBinary( left, right, operator );
    }                                               

    return left;                                    
  }

Expression* parser_comparison() 
{                                
    Expression* left = parser_addition();

    while (parser_match_n(4, (TK[]){ TK_GREATER, TK_GREATER_EQUAL, TK_LESS, TK_LESS_EQUAL }))
    {
        TK operator       = parser_previous()->type;
        Expression* right = parser_addition();
        left              = NodeBinary( left, right, operator );
    }                                                        

    return left;                                             
}  

Expression* parser_equality()
{                         
    Expression* left = parser_comparison();

    while (parser_match_n(2,  (TK[]){TK_NOT_EQUAL, TK_EQUAL_EQUAL}))
    {        
        TK operator       = parser_previous()->type;
        Expression* right = parser_comparison();
        left              = NodeBinary( left, right, operator );
    }                                               

    return left;                                    
}

Expression* parser_expression() 
{
    return parser_equality();       
}

// statement       -> var_declaration ;
// var_declaration -> type comma_statement ";";
// comma_statement -> identifier ( "=" expression )? ( "," comma_statement )?

// Statement* parser_comma_statement( StringView type )
// {       
//     StringView identifier  = parser_consume( TK_IDENTIFIER, "Expect variable name." )->text;

//     if( parser_match( TK_EQUAL ) )
//     {
//         Expression* expression = parser_expression();
//     }

//     if( parser_match( TK_COMMA ) )
//     {
//         parser_comma_statement( type );
//     }

//     type    = parser_current()->text;
//     // return parser_
//     parser_consume( TK_SEMICOLON, "Expect ';' after variable declaration." );
//     return parser_var_declaration( type );
// }

// Statement* parser_var_declaration()
// {       
//     StringView type       = parser_previous()->text;
//     Statement* stmt = parser_comma_statement( type );
//     parser_consume( TK_SEMICOLON, "Expect ';' after variable declaration." );
//     return stmt;
// }

Statement* parser_statement() 
{         
    //TODO: this must be return parser_var_declaration, but don´t know how yet 
    //TODO: in the future TK_IDENTIFIER with valid type must be included
    // if (parser_match_n(7,
    //      (TK[]){ TK_CHAR, TK_DOUBLE,TK_FLOAT,
    //     TK_INT,TK_LONG,TK_SHORT,TK_VOID })
    // ) return parser_type_var_declaration();     
    parser_error( "Not Implemented!");
    return NULL;
} 

void parser_load_tokens(char* file_name, Array tokens )
{
    node_memory_init( MB(4) );

    Parser.current   = (Token*)(tokens.ptr) + 1;//pass TK_START
    Parser.ast       = array( sizeof( Statement* ) );
    Parser.file_name = file_name;

    while( !parser_is_eof() )
    {
        Statement* stmt = parser_statement();
        if(stmt == NULL) continue; //TODO: mabybe is possible to remove this if
        //this is related to the parser_resync
        array_push( &Parser.ast, &stmt );
    }
    
}

void parser_free()
{
    node_memory_free();
}

char tree_str[1024];

int ast_level = -2;

void parser_stmt_visit( Statement* stmt );
void parser_expr_visit( Expression* expr );

void parser_print_ast()
{
    memset(tree_str, ' ', 1024);
    foreach(Statement*, stmt, Parser.ast,{
        // printf("%.*s %.*s \n", (int)stmt->var_declaration.type.len, (char*)&stmt->var_declaration.type, (int)stmt->var_declaration.name.len, (char*)&stmt->var_declaration.name );
        parser_stmt_visit(stmt);
    });

    // Expression* expr = Parser.ast;
    // parser_ast_visit((Expr_t*)expr);
}

void parser_stmt_visit( Statement* stmt )
{
    ast_level += 2;
    // if(ast_level > 0)
    // {
    //     tree_str[ast_level-2] = '|';
    //     tree_str[ast_level-1] = '-';
    // }
    // if(ast_level >= 4)
    // {
    //     tree_str[ast_level-3] = ' ';   
    // }
    printf("%.*s", ast_level, &tree_str[0]);

    switch( stmt->type )
    {
        case STMT_VAR_DECLARATION:
            printf("VarDecl: %.*s %.*s\n",
                (int)stmt->var_declaration.type.len,       stmt->var_declaration.type.ptr,
                (int)stmt->var_declaration.identifier.len, stmt->var_declaration.identifier.ptr);
            if( stmt->var_declaration.init )
            {
                parser_expr_visit(stmt->var_declaration.init);
            }
        break;
        default:
            printf("OTHER STMT\n");
        break;
    }   
    ast_level -= 2;

}


void parser_expr_visit( Expression* expression )
{
    ast_level += 2;
    printf("%.*s", ast_level, &tree_str[0]);

    switch(expression->type)
    {
        case EXPR_GROUPING:
            printf("Parens:\n");
            parser_expr_visit(expression->grouping.expression);
        break;

        case EXPR_IDENTIFIER:
                printf("Identifier: %.*s\n", (int)expression->identifier.len, expression->identifier.ptr);
        break;

        case EXPR_VALUE:
            switch( expression->value.type )
            {
                case VALUE_STRING:
                    printf("String: %.*s\n", (int)expression->value._string.len, expression->value._string.ptr);
                break;
                case VALUE_INT:
                    printf("Int: %d\n", expression->value._int);
                break;
                case VALUE_BOOL:
                    printf("Bool: %s\n", expression->value._bool ? "false": "true");
                break;
            }
        break;

        case EXPR_UNARY:
            printf("Unary: \n");

        break;

        case EXPR_BINARY:
            switch( expression->binary.operator )
            {
                case TK_PLUS:
                    printf("Add:\n");
                    parser_expr_visit(expression->binary.left);
                    parser_expr_visit(expression->binary.right);
                break;
                case TK_MINUS:
                    printf("Sub:\n");
                    parser_expr_visit(expression->binary.left);
                    parser_expr_visit(expression->binary.right);
                break;
                case TK_SLASH:
                    printf("Div:\n");
                    parser_expr_visit(expression->binary.left);
                    parser_expr_visit(expression->binary.right);
                break;
                case TK_STAR:
                    printf("Mul:\n");
                    parser_expr_visit(expression->binary.left);
                    parser_expr_visit(expression->binary.right);
                break;

                default:
                    printf("Unknow Binary expression:");
                break;

            }

        break;

        case EXPR_EXPR:
            printf("expression()\n");

        break;
    }

    ast_level -= 2;

}
