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

Array( ArrayStatement, Statement* );

struct 
{
    Token*          current;
    // ArrayExpression ast;
    Expression*     ast;
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

TK parser_current_tk()
{
    return Parser.current->type;
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
    while(*start != 0 && *start != '\r' && *start != '\r')--start;
    ++start;
    while(*end != 0 && *end != '\r'  && *end != '\r' )++end;

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
Expression* parser_cast_expression();

/*
//LOTS OF TODOS HERE
primaryExpression
    :   Identifier
    |   Constant
    |   StringLiteral+
    |   '(' expression ')'
    |   genericSelection
    |   '__extension__'? '(' compoundStatement ')' // Blocks (GCC extension)
    |   '__builtin_va_arg' '(' unaryExpression ',' typeName ')'
    |   '__builtin_offsetof' '(' typeName ',' unaryExpression ')'
    ;
*/
Expression* parser_primary_expression()  
{
    TK op = parser_current_tk();
    if( op == TK_IDENTIFIER )
    {
        parser_next();
        return node_identifier( parser_previous()->text );
    }
    if( op == TK_STRING )
    {
        parser_next();
        return node_string( parser_previous()->text );
    }
    if( op == TK_NUMBER )
    {
        parser_next();
        return node_int( parser_previous()->_int );
    }
    if( op == TK_OPEN_PAREN )
    {
        parser_next();
        Expression* expr = parser_expression();
        parser_consume(TK_CLOSE_PAREN, "Close Parens Expected!");
        return expr;
    }
    parser_error("Primary Expression Expected!");
    return node_error();
}
/*
postfixExpression
    :   primaryExpression
    |   postfixExpression '[' expression ']' //TODO:
    |   postfixExpression '(' argumentExpressionList? ')' //TODO:
    |   postfixExpression '.' Identifier //TODO:
    |   postfixExpression '->' Identifier //TODO:
    |   postfixExpression '++'
    |   postfixExpression '--'
    |   '(' typeName ')' '{' initializerList '}' //TODO:
    |   '(' typeName ')' '{' initializerList ',' '}' //TODO:
    |   '__extension__' '(' typeName ')' '{' initializerList '}' //TODO:
    |   '__extension__' '(' typeName ')' '{' initializerList ',' '}' //TODO:
    ;

*/
Expression* parser_postfix_expression()  
{
    Expression* expr = parser_primary_expression();
    TK op            = parser_current_tk();
    if( op == TK_PLUS_PLUS || op == TK_MINUS_MINUS)
    {
        parser_next();
        return node_unary( expr, op, EXPR_UNARY_POST );
    }
    return expr;
}

/*
unaryExpression
    :   postfixExpression
    |   '++' unaryExpression
    |   '--' unaryExpression
    |   unaryOperator castExpression
    |   'sizeof' unaryExpression //TODO:
    |   'sizeof' '(' typeName ')' //TODO:
    |   '_Alignof' '(' typeName ')' //TODO:
    |   '&&' Identifier // GCC extension address of label //TODO:
    ;
*/

Expression* parser_unary_expression()  
{
    TK op = parser_current_tk();

    if( op == TK_PLUS_PLUS || op == TK_MINUS_MINUS )
    {
        parser_next();
        Expression* expr = parser_unary_expression();
        return node_unary( expr, op, EXPR_UNARY_PRE );
    }
    else if( 
        op == TK_AND ||
        op == TK_STAR ||
        op == TK_PLUS ||
        op == TK_MINUS ||
        op == TK_BITNOT ||
        op == TK_NOT 
    )
    {
        parser_next();
        Expression* right = parser_cast_expression();
        return node_unary( right, op, EXPR_UNARY_PRE );
    }

    return parser_postfix_expression();
}

/*
castExpression
    :   '(' typeName ')' castExpression //TODO:
    |   '__extension__' '(' typeName ')' castExpression //TODO:
    |   unaryExpression
    |   DigitSequence // for //TODO:
    ;

*/
Expression* parser_cast_expression()  
{
    // TK op = parser_current_tk();

    // if( op == TK_OPEN_PAREN )
    // {
    //     parser_next();
    //     Token* type = parser_consume(TK_IDENTIFIER, "Type Identifier Expected!");
    //     parser_consume(TK_CLOSE_PAREN, "Closing Parens expected!");
    // }

    return parser_unary_expression();
}

/*
multiplicativeExpression
    :   castExpression
    |   multiplicativeExpression '*' castExpression
    |   multiplicativeExpression '/' castExpression
    |   multiplicativeExpression '%' castExpression
    ;
*/

Expression* parser_multiplicative_expression()  
{
    Expression* left = parser_cast_expression();
    TK op = parser_current_tk();
    if( op == TK_STAR || op == TK_SLASH || op == TK_MOD )
    {
        parser_next();
        return node_binary( left, op, parser_multiplicative_expression() );
    }
    return left;
}

/*
additiveExpression
    :   multiplicativeExpression
    |   additiveExpression '+' multiplicativeExpression
    |   additiveExpression '-' multiplicativeExpression
    ;
*/
Expression* parser_additive_expression()  
{
    Expression* left = parser_multiplicative_expression();
    TK op = parser_current_tk();
    if( op == TK_PLUS || op == TK_MINUS )
    {
        parser_next();
        return node_binary( left, op, parser_additive_expression() );
    }
    return left;
}

/*
shiftExpression
    :   additiveExpression
    |   shiftExpression '<<' additiveExpression
    |   shiftExpression '>>' additiveExpression
    ;
*/
Expression* parser_shift_expression()  
{
    Expression* left = parser_additive_expression();
    TK op = parser_current_tk();
    if( op == TK_LSHIFT || op == TK_RSHIFT )
    {
        parser_next();
        return node_binary( left, op, parser_shift_expression() );
    }
    return left;
}


/*
relationalExpression
    :   shiftExpression
    |   relationalExpression '<' shiftExpression
    |   relationalExpression '>' shiftExpression
    |   relationalExpression '<=' shiftExpression
    |   relationalExpression '>=' shiftExpression
    ;
*/
Expression* parser_relational_expression()  
{
    Expression* left = parser_shift_expression();
    TK op = parser_current_tk();
    if( op >= TK_LESS && op <= TK_GREATER_EQUAL )
    {
        parser_next();
        return node_binary( left, op, parser_relational_expression() );
    }
    return left;
}

/*
equalityExpression
    :   relationalExpression
    |   equalityExpression '==' relationalExpression
    |   equalityExpression '!=' relationalExpression
    ;
*/

Expression* parser_equality_expression()  
{
    Expression* left = parser_relational_expression();
    TK op = parser_current_tk();
    if( op == TK_EQUAL_EQUAL || op == TK_NOT_EQUAL )
    {
        parser_next();
        return node_binary( left, op, parser_equality_expression() );
    }
    return left;
}

/*
andExpression
    :   equalityExpression
    |   andExpression '&' equalityExpression
    ;
*/

Expression* parser_and_expression()  
{
    Expression* left = parser_equality_expression();
    TK op = parser_current_tk();
    if( op == TK_BITAND )
    {
        parser_next();
        return node_binary( left, op, parser_and_expression() );
    }
    return left;
}

/*
exclusiveOrExpression
    :   andExpression
    |   exclusiveOrExpression '^' andExpression
    ;
*/

Expression* parser_exclusive_or_expression()  
{
    Expression* left = parser_and_expression();
    TK op = parser_current_tk();
    if( op == TK_XOR )
    {
        parser_next();
        return node_binary( left, op, parser_exclusive_or_expression() );
    }
    return left;
}

/*
inclusiveOrExpression
    :   exclusiveOrExpression
    |   inclusiveOrExpression '|' exclusiveOrExpression
    ;
*/

Expression* parser_inclusive_or_expression()  
{
    Expression* left = parser_exclusive_or_expression();
    TK op = parser_current_tk();
    if( op == TK_BITOR )
    {
        parser_next();
        return node_binary( left, op, parser_inclusive_or_expression() );
    }
    return left;
}

/*
logicalAndExpression
    :   inclusiveOrExpression
    |   logicalAndExpression '&&' inclusiveOrExpression
    ;
*/
Expression* parser_logical_and_expression()  
{
    Expression* left = parser_inclusive_or_expression();
    TK op = parser_current_tk();
    if( op == TK_AND )
    {
        parser_next();
        return node_binary( left, op, parser_logical_and_expression() );
    }
    return left;
}

/*
logicalOrExpression
    :   logicalAndExpression
    |   logicalOrExpression '||' logicalAndExpression
    ;
*/

Expression* parser_logical_or_expression()  
{
    Expression* left = parser_logical_and_expression();
    TK op = parser_current_tk();
    if( op == TK_OR )
    {
        parser_next();
        return node_binary( left, op, parser_logical_or_expression() );
    }
    return left;
}

/*
conditionalExpression
    :   logicalOrExpression ('?' expression ':' conditionalExpression)?
    ;
*/

Expression* parser_conditional_expression()  
{
    Expression* left = parser_logical_or_expression();
    //TODO: TERNARY
    //ALSO IS A EXPR NOT STATEMENT!
    // if( parser_current_tk() == TK_QUESTION )
    // {

    // }

    return left;
}

/*
assignmentExpression
    :   conditionalExpression
    |   unaryExpression assignmentOperator assignmentExpression
    |   DigitSequence // for //TODO: dont know if necessary
    ;
*/
Expression* parser_assignment_expression()  
{
    Expression* left = parser_conditional_expression();
    TK op            = parser_current_tk();

    if( op >= TK_EQUAL && op <= TK_OR_EQUAL )
    {
        parser_next();
        return node_binary( left, op , parser_assignment_expression() );
    }

    return left;
}
/*
expression
    :   assignmentExpression
    |   expression ',' assignmentExpression
    ;
*/
Expression* parser_expression()  
{
    Expression* left = parser_assignment_expression();
    TK op            = parser_current_tk();

    if( op == TK_COMMA )
    {
        parser_next();
        return node_binary( left, op , parser_expression() );
    }
    return left;
}

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

void parser_load_tokens(char* file_name, ArrayToken tokens )
{
    node_memory_init( MB(4) );

    Parser.current   = (Token*)( tokens.ptr) + 1;//pass TK_START
    // Parser.ast       = ( ArrayExpression ){};
    Parser.file_name = file_name;

    Parser.ast = parser_expression();

    // while( !parser_is_eof() )
    // {
    //     Statement* stmt = parser_statement();
    //     if(stmt == NULL) continue; //TODO: mabybe is possible to remove this if
    //     //this is related to the parser_resync
    //     array_push( Parser.ast, stmt );
    // }
    
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
    // foreach(Statement*, stmt, Parser.ast,{
    //     // printf("%.*s %.*s \n", (int)stmt->var_declaration.type.len, (char*)&stmt->var_declaration.type, (int)stmt->var_declaration.name.len, (char*)&stmt->var_declaration.name );
    //     parser_stmt_visit(stmt);
    // });

    Expression* expr = Parser.ast;
    parser_expr_visit(expr);
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

        case EXPR_UNARY_PRE:
            print("Unary Prefix: ", (char*)TK_STR[expression->unary.operator]);
            parser_expr_visit(expression->unary.expr);
        break;

        case EXPR_UNARY_POST:
            print("Unary Postfix: ", (char*)TK_STR[expression->unary.operator] );
            parser_expr_visit(expression->unary.expr);
        break;

        case EXPR_ERROR:
            print("Error Node: \n");
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

                case TK_COMMA:
                    printf("Comma :\n");
                    parser_expr_visit(expression->binary.left);
                    parser_expr_visit(expression->binary.right);
                break;

                case TK_MOD:
                    printf("'%':\n");
                    parser_expr_visit(expression->binary.left);
                    parser_expr_visit(expression->binary.right);
                break;

                case TK_EQUAL_EQUAL:
                    printf("'==':\n");
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
