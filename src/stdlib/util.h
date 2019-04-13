#pragma once

#include <stddef.h>
#include "stdlib/types.h"

#define foreach( VAR, RANGE, BODY ) \
{ \
	typeof(RANGE[0])* VAR = RANGE; \
	const size_t __limit = length( RANGE ); \
	for(size_t __counter = 0 ; __counter < __limit ; ++__counter, ++VAR  ) \
	BODY\
}\

typedef struct { size_t len; void* ptr[]; } HeadLen;
typedef struct { size_t cap; size_t len; void* ptr[]; } HeadLenCap;

HeadLen* head_len_cast(void* val)
{
    return (( HeadLen* )(val - sizeof( HeadLen )));
}

HeadLenCap* head_len_cap_cast(void* val)
{
    return (( HeadLenCap* )(val - sizeof( HeadLenCap )));
}

HeadLen* head_len_create( const size_t len, const size_t size )
{
    HeadLen* val = (HeadLen*) malloc( sizeof( HeadLen ) + (len * size) );
    val->len = len;
    return val;
}

HeadLenCap* head_len_cap_create( const size_t len, const size_t size )
{
    HeadLenCap* val = (HeadLenCap*) malloc( sizeof( HeadLenCap ) + (len * size) );
    val->len = 0;
    val->cap = len;
    return val;
}

HeadLen* head_len_create_from( HeadLen* from, size_t len, size_t size )
{
    HeadLen* new_head = head_len_create( len , size );
    const size_t old_len = from->len;
    const size_t new_len = len < old_len ? len : old_len;
    memcpy( new_head->ptr, from->ptr, new_len * size );
    return new_head;
}

HeadLenCap* head_len_cap_create_from( HeadLenCap* from, size_t len, size_t size )
{
    HeadLenCap* new_head = head_len_cap_create( len , size );
    const size_t old_len = from->len;
    const size_t new_len = len < old_len ? len : old_len;
    new_head->len        = new_len;
    memcpy( new_head->ptr, from->ptr, new_len * size );
    return new_head;
}

size_t* length_ptr(void* val)
{
    return &(( HeadLen* )(val - sizeof( HeadLen )))->len;
}

size_t length( void* val )
{
    return (( HeadLen* )(val - sizeof( HeadLen )))->len;
}

void length_set( void* val, size_t len )
{
    (( HeadLen* )(val - sizeof( HeadLen )))->len = len;
}

size_t capacity( void* val )
{
    return (( HeadLenCap* )(val - sizeof( HeadLenCap )))->cap;
}
