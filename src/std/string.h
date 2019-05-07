#pragma once

#include <stdio.h>
#include <string.h>
#include "std/util.h"
#include "std/alloc.h"

class( String, {
	char* ptr;
	size_t len;
	size_t cap;
});

String string_from( void* string )
{
	char* str = (char*) string;
	const size_t len = strlen( str );
	String this;
	this.ptr = alloc( len + 1 );
	this.len = len;
	memcpy(this.ptr, str, len);
	this.ptr[len] = 0;
	return this;
}

String string( const size_t cap )
{
	String this;

	this.ptr = alloc( cap + 1);
	this.len = 0;
	this.cap = cap;

	this.ptr[0] = 0;
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
		char* tmp = alloc( new_cap + 1 );
		memcpy( tmp, this->ptr, old_len );
		memcpy( tmp + old_len, str , val_len + 1 );
		this->len        = new_cap;
		this->cap        = new_cap;

		free(this->ptr);
		this->ptr = tmp;
		return;
	}
	memcpy(this->ptr + old_len, str , val_len + 1);
	this->len = new_cap;
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

