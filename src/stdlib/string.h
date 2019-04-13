#pragma once

#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "stdlib/util.h"




typedef struct { size_t len; char* ptr[]; } stringHead;
typedef char* string;

string string_create( char* value )
{
	const size_t len = strlen( value );
	HeadLen* head = head_len_create( len, 1 );
	memcpy(head->ptr, value, len );
	return (string)head->ptr;
}

string string_push( string const str, const char* val )
{
	HeadLen* old_head = head_len_cast( str );

	const size_t old_len = old_head->len;
	const size_t val_len = strlen( val );
	const size_t new_len = old_len + val_len;

	HeadLen* new_head = head_len_create_from(old_head, new_len, 1);
	memcpy( new_head->ptr + old_len , val , val_len );
	free(old_head);

	return (string)new_head->ptr;
}

void string_print( string str )
{
	HeadLen* head = head_len_cast( str );
	printf("%.*s\n", (int)head->len, (string)head->ptr);
}

void string_free( string str )
{
	free( head_len_cast( str ) );
}