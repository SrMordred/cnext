#pragma once

#include "std/types.h"

#define MAX_ERROR_INDEX 32

jmp_buf __error[MAX_ERROR_INDEX];

void error_throw( const size_t index )
{
	longjmp(__error[index], 1 );	
}

#define error_catch( INDEX, GOTO ) do{ if( setjmp(__error[(INDEX)]) != 0 ) goto GOTO; } while(0)




