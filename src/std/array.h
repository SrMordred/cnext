#pragma once

#include <math.h>
#include <string.h>
#include <stdlib.h>

#include "std/util.h"

typedef struct Array
{
    void*  ptr;
    size_t len;
    size_t cap;
    size_t size;
} Array;

Array array( const size_t size )
{
    Array arr = { NULL, 0, 0, size };
    return arr;
}

void array_free( Array* this )
{
    free(this->ptr);
}

void array_reserve(Array* this, size_t new_cap )
{
    const size_t old_cap = this->cap;
    if( new_cap > old_cap)
    {
        const size_t size = this->size;
        const size_t old_len = this->len;
        const size_t new_cap_pow = ceilf( old_cap * 1.5f );
        if( new_cap_pow > new_cap )
            new_cap = new_cap_pow;

        void* tmp = malloc( new_cap * size );
        memcpy( tmp, this->ptr, old_len * size );
        this->cap = new_cap;

        free( this->ptr );
        this->ptr = tmp;
    }
}

void array_resize( Array* this, size_t new_len )                                                
{     
    if( new_len >  this->len )                                               
    {       
        array_reserve( this, new_len );
        //TODO : fill or calloc
        this->len = new_len;
    }   
}    

void array_push( Array* this, void* value )
{
    const size_t len  = this->len;
    const size_t size = this->size;
    array_reserve(this, len + 1 );
    memcpy(this->ptr + (len * size), value, size);
    ++(this->len);
}
                                            