#pragma once

#include <math.h>
#include <string.h>
#include <stdlib.h>

#include "std/util.h"


#define Array( NAME, TYPE )  \
class( NAME, { \
    TYPE*  ptr; \
    size_t len; \
    size_t cap; \
}) \

#define array_reserve( this, new_cap_ ) \
do { \
    size_t new_cap = new_cap_; \
    const size_t old_cap = this.cap; \
    if( new_cap > old_cap) \
    { \
        const size_t old_len     = this.len; \
        const size_t new_cap_pow = ceilf( old_cap * 1.5f ); \
        if( new_cap_pow > new_cap ) \
            new_cap = new_cap_pow; \
 \
        void* new_ptr = alloc( new_cap * sizeof( *this.ptr ) ); \
        memcpy( new_ptr, this.ptr, old_len * sizeof( *this.ptr ) ); \
        this.cap = new_cap; \
 \
        release( this.ptr ); \
        this.ptr = new_ptr; \
    } \
} while(0)  \

// void array_resize( Array* this, size_t new_len )                                                
// {     
//     if( new_len >  this->len )                                               
//     {       
//         array_reserve( this, new_len );
//         //TODO : fill or calloc
//         this->len = new_len;
//     }   
// }    

#define array_push( this, value )  \
do {\
    array_reserve( this, this.len + 1 );\
    this.ptr[this.len++] = value; \
} while(0) \

#define array_free( this ) release( this.ptr )