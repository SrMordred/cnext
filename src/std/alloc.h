#pragma once 

#include <stdlib.h>
#include "std/util.h"

void* alloc( const size_t size)
{
	return malloc( size );
}

void* alloc_zero( const size_t size)
{
	return calloc( 1, size );
}

Slice alloc_slice( const size_t len, const size_t size )
{
	Slice slice;
	slice.ptr = malloc( len * size );
	slice.len = 0;
	slice.cap = len;
	return slice;
}

