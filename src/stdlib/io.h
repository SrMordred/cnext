#pragma once

#include <stdio.h>
#include "stdlib/string.h"

string file_to_string( char* file_name )
{
	string file_str = string_create("");

	char buffer[4096];

	FILE* fp = fopen( file_name , "r");
    if(!fp) 
	{
        perror("File opening failed");
		return file_str;
    }

	while (fgets(buffer, sizeof(buffer), fp) != NULL)
		file_str = string_push( file_str, buffer );

	fclose(fp);

	return file_str;
}