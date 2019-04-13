#ifndef ARRAY_H
#define ARRAY_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct                                              
{                                               
    int*    ptr;                                               
    size_t  len;                                                
    size_t  cap;                                                
                                                
} array_int ;                                                

void array_int_reserve( array_int* this, size_t new_cap );                                               
                                               
array_int* array_int_new( size_t new_len )                                              
{                                               
    array_int* this = malloc( sizeof(array_int) );                                                
    this->ptr       = malloc( sizeof(TYPE) * new_len );                                             
    this->len       = 0;                                                
    this->cap       = new_len;                                              
    return this;                                                
}                                               
                                                
void array_int_free( array_int* this )                                              
{                                               
    free( this->ptr );                                              
    free( this );                                               
}                                               
                                                
                                                
void array_int_push( array_int* this, int value )                                               
{                                               
    array_int_reserve( this, this->len + 1 );                                                
    this->ptr[ this->len++ ] = value;                                               
}                                               
                                                
void array_int_resize( array_int* this, size_t new_len )                                                
{                                               
    if( new_len > this->len )                                               
    {                                               
        array_int_reserve( this, new_len );                                                
                                                        
        for(size_t i = this->len ; i < new_len; ++i )                                               
        {                                               
            this->ptr[ i ] = (int) 0;                                               
        }                                               
        this->len = new_len;                                                
    }                                               
}                                               
                                                
void array_int_reserve( array_int* this, size_t new_cap )                                               
{                                               
    if( new_cap > this->cap )                                               
    {                                               
        const size_t new_cap = this->cap * 1.5;                                             
        int* tmp = malloc( sizeof(int) * new_cap );                                             
        memcpy(tmp, this->ptr, this->len * sizeof(int) );                                               
        this->cap = new_cap;                                                
    }                                               
}                                               
                                                
void array_int_clear( array_int* this)                                              
{                                               
    this->len = 0;                                              
}                                               
                                                
void array_int_print( array_int* this )                                             
{                                               
    printf("[ ");                                               
    const size_t len = this->len;                                               
    if( len )                                               
    {                                               
        for(size_t i = 0; i < len-1 ; ++i )                                             
        {                                               
            printf("%d, ", this->ptr[i]);                                               
        }                                               
        printf("%d ]", this->ptr[len-1]);                                               
    }                                               
    else                                                
    {                                               
        printf(" ]");                                               
    }                                               
}                                               
                                                


#endif