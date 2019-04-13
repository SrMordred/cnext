#ifndef ARRAY_H
#define ARRAY_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

typedef struct { void*   ptr; size_t  len; size_t  cap; } array ;   
typedef struct { int*    ptr; size_t  len; size_t  cap; } array_int ;   

// void array_int_reserve( array_int* this, size_t new_cap );                                               
                                               
void _ctor_array( array* this, const size_t size, size_t new_len )                                              
{                                               
    this->ptr       = malloc( size * new_len );                                             
    this->len       = 0;                                                
    this->cap       = new_len;                                              
}                            

void _dtor_array( array* this )                                                
{
    free(this->ptr);
}

void _reserve_array( array* this, const size_t size, size_t new_cap )                                               
{        
    if( new_cap > this->cap )
    {          
        const size_t new_cap_pow = ceilf( this->cap * 1.5f );
        if( new_cap_pow > new_cap )
            new_cap = new_cap_pow;
        void* tmp = malloc( size * new_cap );           
        memcpy(tmp, this->ptr, this->len * size );        
        free(this->ptr);
        this->ptr = tmp;
        this->cap = new_cap;                                                
    }                                               
}

void _resize_array( array* this, size_t size, size_t new_len )                                                
{       
    const size_t len = this->len;                                        
    if( new_len > len )                                               
    {                                               
        _reserve_array( this, size, new_len );  
        memset( this->ptr + len, 0, (new_len - len) * size );
        this->len = new_len;                                                
    }                                               
}    

void _clear_array( array* this )
{
    this->len = 0;
}

#define ctor_array( THIS, NEW_LEN ) _ctor_array( (array*) &THIS, sizeof(*THIS.ptr), NEW_LEN )

#define dtor_array( THIS ) _dtor_array( (array*) &THIS )

#define push_array( THIS, VALUE )                                       \
_reserve_array( (array*)&THIS, sizeof(*THIS.ptr) , THIS.len + 1 );      \
THIS.ptr[ THIS.len++ ] = VALUE                                          \

#define reserve_array( THIS, NEW_CAP ) _reserve_array( (array*) &THIS, sizeof(*THIS.ptr), NEW_CAP )
#define resize_array( THIS, NEW_LEN ) _resize_array( (array*) &THIS, sizeof(*THIS.ptr), NEW_LEN )

#define clear_array( THIS ) clear_array( (array*) &THIS )

#define print_array( THIS, FORMAT )                     \
printf("[ ");                                             \
const size_t len = THIS.len;                            \
if( len )                                               \
{                                                       \
    for(size_t i = 0; i < len-1 ; ++i )                 \
    {                                                   \
        printf(FORMAT, THIS.ptr[i]);                    \
        printf(", ");                                     \
    }                                                   \
    printf(FORMAT, THIS.ptr[len-1]);                    \
    printf(" ]\n");                                         \
}                                                       \
else                                                    \
{                                                       \
    printf(" ]\n");                                         \
}                                                       \

                                                
                                           
                                                
                                              
                                              
                                                
// void array_int_print( array_int* this )                                             
// {                                               
//     printf("[ ");                                               
//     const size_t len = this->len;                                               
//     if( len )                                               
//     {                                               
//         for(size_t i = 0; i < len-1 ; ++i )                                             
//         {                                               
//             printf("%d, ", this->ptr[i]);                                               
//         }                                               
//         printf("%d ]", this->ptr[len-1]);                                               
//     }                                               
//     else                                                
//     {                                               
//         printf(" ]");                                               
//     }                                               
// }                                               
                                                


#endif