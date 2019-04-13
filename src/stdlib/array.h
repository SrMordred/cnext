#pragma once

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "stdlib/util.h"

typedef struct { size_t cap; size_t len; void* ptr[]; } arrayHead;   

typedef void* array;

void* array_create()
{
    HeadLenCap* head = head_len_cap_create( 0, 0 );
    return (array) head->ptr;
}

void array_free( array val )
{
    free( head_len_cap_cast(val) );
}

array _array_reserve(array arr, const size_t size, size_t new_cap )
{
    HeadLenCap* old_head = head_len_cap_cast(arr);

    const size_t old_cap = old_head->cap;

    if( new_cap > old_cap)
    {
        const size_t new_cap_pow = ceilf( old_cap * 1.5f );
        if( new_cap_pow > new_cap )
            new_cap = new_cap_pow;
        
        HeadLenCap* new_head = head_len_cap_create_from( old_head, new_cap, size );
        free( old_head );
        return (array)new_head->ptr;
    }
    return arr;
}
#define array_reserve( ARRAY, LEN ) (typeof(ARRAY)) _array_reserve( ARRAY, sizeof(ARRAY[0]), LEN )

array _array_resize( array arr, size_t size, size_t new_len )                                                
{     
    HeadLenCap* old_head = head_len_cap_cast( arr );
    if( new_len >  old_head->len )                                               
    {       
        arr = _array_reserve( arr, size, new_len );
        //TODO : fill or calloc
        head_len_cap_cast(arr)->len = new_len;
    }   
    return arr;
}    
#define array_resize( ARRAY, LEN ) (typeof(ARRAY)) _array_resize( ARRAY, sizeof(ARRAY[0]), LEN )

#define array_push( ARRAY, VALUE ) \
{\
    ARRAY = array_reserve(ARRAY, length(ARRAY) + 1);\
    const size_t len = length(ARRAY); \
    ARRAY[ len ] = VALUE;\
    length_set(ARRAY, len + 1 ); \
}\



                                            