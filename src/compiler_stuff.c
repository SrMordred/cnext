// // #include "compiler/lexer.h"
// // #include "compiler/parser.h"

// #include "test.h"

// int main(int argc, char *argv[]) {
// 	(void) argv;
// 	(void) argc;


// 	test();
// 	// lexer_init();
// 	// lexer_load_file("script.cn");
// 	// lexer_print_tokens();

// 	// parser_load_tokens( lexer_get_tokens() );

// 	// string_print( file_to_string("script.cn") );


// 	return 0;

// }



#include <stdio.h> 
#include <dirent.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

int file_is_dir(const char* const file) {
   struct stat stats;
   if (stat(file, &stats) != 0) return 0;
   return S_ISDIR(stats.st_mode);
}

long file_last_mod( const char* const file )
{
	struct stat stats;
   if (stat(file, &stats) != 0) return 0;
   return stats.st_mtime;
}


void listFilesRecursively(char *basePath)
{
    char path[1000];
    struct dirent *dp;
    DIR *dir = opendir(basePath);

    // Unable to open directory stream
    if (!dir)
        return;

    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
			strcpy(path, basePath);
            strcat(path, "/");
            strcat(path, dp->d_name);
			
            printf("%s\n", path);

			struct stat filestat;

			stat(path,&filestat);
			long t = file_last_mod(path);
			printf(" File modify time %s", ctime(&t) );
			
			if( file_is_dir(path) )
            	listFilesRecursively(path);
        }
    }

    closedir(dir);
}
  
int main(void) 
{ 
	listFilesRecursively("src");
    
    return 0; 
} 
