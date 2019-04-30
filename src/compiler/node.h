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
    EXPR_UNARY,
    EXPR_BINARY,
    EXPR_EXPR,
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

class( Unary, {
	Expression* right;
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

class(Program, {
	Array statements;
});

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

Expression* NodeBool( bool value ) 
{
	Expression* expression  = node_alloc( sizeof *expression );
	expression->type        = EXPR_VALUE;
	expression->value.type  = VALUE_BOOL;
	expression->value._bool = value;
	return expression;
}

Expression* NodeInt( int value ) 
{
	Expression* expression  = node_alloc( sizeof *expression );
	expression->type        = EXPR_VALUE;
	expression->value.type  = VALUE_INT;
	expression->value._int  = value;
	return expression;
}

Expression* NodeString( StringView value ) 
{
	Expression* expression    = node_alloc( sizeof *expression );
	expression->type          = EXPR_VALUE;
	expression->value.type    = VALUE_STRING;
	expression->value._string = value;
	return expression;
}

Expression* NodeIdentifier( StringView identifier ) 
{
	Expression* expression = node_alloc( sizeof *expression );
	expression->type       = EXPR_IDENTIFIER;
	expression->identifier = identifier;
	return expression;
}

Expression* NodeGrouping( Expression* expression_group ) 
{
	Expression* expression          = node_alloc( sizeof *expression );
	expression->type                = EXPR_GROUPING;
	expression->grouping.expression = expression_group;
	return expression;
}

Expression* NodeUnary( Expression* right, TK operator  ) 
{
	Expression* expression     = node_alloc( sizeof *expression );
	expression->type           = EXPR_UNARY;
	expression->unary.operator = operator;
	expression->unary.right    = right;
	return expression;
}

Expression* NodeBinary( Expression* left, Expression* right, TK operator ) 
{
	Expression* expression      = node_alloc( sizeof *expression );
	expression->type            = EXPR_UNARY;
	expression->binary.left     = left;
	expression->binary.right    = right;
	expression->binary.operator = operator;
	return expression;
}

Statement* NodeVarDeclaration( StringView type, StringView identifier, Expression* init) 
{
	Statement* statement                  = node_alloc( sizeof *statement );
	statement->type                       = STMT_VAR_DECLARATION;
	statement->var_declaration.type       = type;
	statement->var_declaration.identifier = identifier;
	statement->var_declaration.init       = init;
	return statement;
}



