#pragma once

#include <stddef.h>
#include "std/types.h"

// don´t ask...
#define GET_MACRO(_1,_2,_3,_4,_5,NAME,...) NAME 
#define print(...) GET_MACRO(__VA_ARGS__, print5, print4,print3,print2,print1)(__VA_ARGS__); printf("\n")

#define print1(_1) printf(__print_generic(_1) , _1 )
#define print2(_1, _2) print1(_1); print1(_2)
#define print3(_1, _2, _3) print1(_1); print1(_2); print1(_3) 
#define print4(_1, _2, _3, _4) print1(_1); print1(_2); print1(_3); print1(_4)
#define print5(_1, _2, _3, _4, _5) print1(_1); print1(_2); print1(_3); print1(_4); print1(_5)

#define __print_generic(x) _Generic((x),        /* Get the name of a type */             \
                                                                                  \
        bool: "%d",                  unsigned char: "%c",          \
         char: "%c",                     signed char: "%c",            \
    short int: "%d",         unsigned short int: "%u",     \
          int: "%d",                     unsigned int: "%u",           \
     long int: "%ld",           unsigned long int: "%lu",      \
long long int: "%lld",          unsigned long long int: "%llu", \
        float: "%f",                         double: "%f",                 \
  long double: "%f",                   char *: "%s",        \
       void *: "%p",                int *: "%p",         \
default: "%p") \

/*
ex:
foreach(int , a, int_array, { print(a); })
*/
#define foreach(TYPE, VAR, RANGE, BODY ) \
{ \
	TYPE*         __ptr   = (TYPE*) RANGE.ptr; \
	const size_t  __limit = RANGE.len; \
	for(size_t __counter = 0 ; __counter < __limit ; ++__counter) {\
	TYPE VAR = __ptr[__counter];	\
	BODY \
	}	 \
}\

#define loop(ARRAY, BODY ) \
{ \
	const __typeof( ARRAY.ptr ) __ptr = ARRAY.ptr; \
	const size_t  __limit       = ARRAY.len; \
	for(size_t __counter = 0 ; __counter < __limit ; ++__counter) {\
	__typeof( *ARRAY.ptr ) value = __ptr[__counter];	\
	BODY \
	}	 \
} \


#define cast(TYPE, VAR) (( TYPE )( VAR ))

typedef struct Slice
{
	void* 		ptr;
	size_t   	len;
	size_t   	cap;
} Slice;

typedef struct View
{
	void* 		ptr;
	size_t   	len;
} View;

View view_from( void* ptr, const size_t len )
{
	return (View){ ptr, len };
}


void length_set( void* this, const int value )
{
    *cast(size_t*, this + sizeof(size_t)) = value;
}



// void copy( void* dest, void* source, const size_t size )
// {
// 	memcpy( dest, source, size );
// }

// void copy_slice( void* dest, void* source, const size_t size )
// {
// 	memcpy( dest, source, ptr_len( source ) * size );
// }

#define class( STRUCT_NAME, STRUCT_BODY ) typedef struct STRUCT_NAME STRUCT_BODY STRUCT_NAME
#define Enum( ENUM_NAME, ... ) typedef enum ENUM_NAME __VA_ARGS__ ENUM_NAME