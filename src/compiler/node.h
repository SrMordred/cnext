#include "compiler/token.h"

typedef struct Node Node;
typedef struct Expression Expression;

Enum(VALUE, 
{
	VALUE_BOOL,
	VALUE_INT,
	VALUE_STRING,
});

Enum(EXPR,
{
	EXPR_GROUPING,
    EXPR_VALUE,
    EXPR_IDENTIFIER,
    EXPR_UNARY_PRE,
    EXPR_UNARY_POST,
    EXPR_BINARY,
    EXPR_EXPR,
    EXPR_ERROR
});

Enum( STMT ,
{
	STMT_VAR_DECLARATION
});

Enum( NODE ,
{
	NODE_EXPR,
	NODE_STMT,
});

class( Grouping ,{
	Expression* expression;
});

// replaced with StringView
// class( Identifier, {
// 	StringView identifier;
// });

class( Value, {
	VALUE type;
	union
	{
		bool       _bool;
		int        _int;
		StringView _string;
	};
});

class(Error,{});

class( Unary, {
	Expression* expr;
	TK operator;
});

class( Binary, {
	Expression* left;
	Expression* right;
	TK      	operator;
});

class(Expression, {
	EXPR type;

	union
	{
		Grouping   grouping;
		StringView identifier;
		Value      value;
		Unary      unary;
		Binary     binary;
		Error      error;
	};

});

class(VarDeclaration,{

	StringView  type;
	StringView  identifier;
	Expression* init;

});

class(Statement, {
	STMT type;
	union
	{
		VarDeclaration var_declaration;
	};
});

class(Node, {
	NODE type;
	union
	{
		Expression expression;
		Statement  statement;
	};
});

// class(Program, {
// 	ArrayDecl(statements;
// });

//MEMORY AND ALLOCATION

#include "std/memory_arena.h"

MemoryArena node_memory;

void node_memory_init( const size_t starting_memory )
{
	node_memory = memory_arena( starting_memory );
}

void node_memory_free()
{
	memory_arena_free( &node_memory );
}

void* node_alloc( const size_t size )
{
	return memory_arena_alloc( &node_memory, size );
}

Expression* node_error() 
{
	Expression* expression  = node_alloc( sizeof *expression );
	expression->type        = EXPR_ERROR;
	return expression;

}


Expression* node_bool( bool value ) 
{
	Expression* expression  = node_alloc( sizeof *expression );
	expression->type        = EXPR_VALUE;
	expression->value.type  = VALUE_BOOL;
	expression->value._bool = value;
	return expression;
}

Expression* node_int( int value ) 
{
	Expression* expression  = node_alloc( sizeof *expression );
	expression->type        = EXPR_VALUE;
	expression->value.type  = VALUE_INT;
	expression->value._int  = value;
	return expression;
}

Expression* node_string( StringView value ) 
{
	Expression* expression    = node_alloc( sizeof *expression );
	expression->type          = EXPR_VALUE;
	expression->value.type    = VALUE_STRING;
	expression->value._string = value;
	return expression;
}

Expression* node_identifier( StringView identifier ) 
{
	Expression* expression = node_alloc( sizeof *expression );
	expression->type       = EXPR_IDENTIFIER;
	expression->identifier = identifier;
	return expression;
}

Expression* node_grouping( Expression* expression_group ) 
{
	Expression* expression          = node_alloc( sizeof *expression );
	expression->type                = EXPR_GROUPING;
	expression->grouping.expression = expression_group;
	return expression;
}

Expression* node_unary( Expression* expr, TK operator, EXPR pre_or_post ) 
{
	Expression* expression     = node_alloc( sizeof *expression );
	expression->type           = pre_or_post;
	expression->unary.operator = operator;
	expression->unary.expr     = expr;
	return expression;
}

Expression* node_binary( Expression* left, TK operator , Expression* right ) 
{
	Expression* expression      = node_alloc( sizeof *expression );
	expression->type            = EXPR_BINARY;
	expression->binary.left     = left;
	expression->binary.right    = right;
	expression->binary.operator = operator;
	return expression;
}

Statement* node_varDeclaration( StringView type, StringView identifier, Expression* init) 
{
	Statement* statement                  = node_alloc( sizeof *statement );
	statement->type                       = STMT_VAR_DECLARATION;
	statement->var_declaration.type       = type;
	statement->var_declaration.identifier = identifier;
	statement->var_declaration.init       = init;
	return statement;
}



