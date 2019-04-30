#pragma once

#include <stdio.h>
#include <string.h>
#include "std/util.h"
#include "std/alloc.h"

typedef Slice String;

String string( void* string )
{
	char* str = (char*) string;
	const size_t len = strlen( str );

	String this = alloc_slice( len, 1 );
	this.len    = len;
	copy(this.ptr, str, len);
	cast(char*, this.ptr)[len] = 0;
	return this;
}

String string_new_cap( const size_t cap )
{
	String this;

	this.ptr = malloc( cap + 1);
	this.len = 0;
	this.cap = cap;

	cast(char*, this.ptr)[0] = 0;
	return this;
}

void string_push( String* this, const void* value )
{
	char* str = (char*) value;
	const size_t old_cap = this->cap;
	const size_t old_len = this->len;
	const size_t val_len = strlen( str );
	const size_t new_cap = old_len + val_len;

	if( new_cap > old_cap )
	{
		char* tmp = malloc( new_cap + 1 );
		copy( tmp, this->ptr, old_len );
		copy( tmp + old_len, str , val_len );
		this->len        = new_cap;
		this->cap        = new_cap;
		cast(char*, this->ptr)[new_cap] = 0;

		free(this->ptr);
		this->ptr = tmp;
		return;
	}
	copy(this->ptr + old_len, str , val_len);
	this->len = new_cap;
	cast(char*, this->ptr)[new_cap] = 0;
}

void string_print( void* this )
{
	View* view = cast(View*, this);
	printf("%.*s\n", (int)view->len, (char*)view->ptr);
}

void string_free( String this )
{
	free( this.ptr );
}

class( StringView, {
	char* 		ptr;
	size_t   	len;
});

StringView string_view_from( void* ptr, size_t len )
{
	return (StringView){ (char*)ptr, len };
}

