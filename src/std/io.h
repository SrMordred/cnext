#pragma once

#include <stdio.h>
#include <stdlib.h>
#include "std/string.h"

String file_to_string( char* file_name )
{

	FILE* file = fopen( file_name , "rb");
	if(!file) 
	{
        perror("File opening failed");
		return (String){NULL,0,0};
    }

	fseek(file, 0, SEEK_END);
	long file_size = ftell(file);
	fseek(file, 0, SEEK_SET);  /* same as rewind(f); */

	String file_string = string_new_cap( file_size );
	fread(file_string.ptr, 1, file_size, file);
	fclose(file);

	return file_string;
}