#include "compiler/lexer.h"
#include "compiler/parser.h"


#include "std/string.h"
#include "std/array.h"


#include "std/util.h"
#include "std/types.h"
#include "std/alloc.h"

// typedef struct DictView
// {
// 	void* keys;
// 	void* values;

// 	size_t len;
// 	size_t cap;


// } DictView;


// DictView dict_view_new()
// {
// 	const size_t dict_start_size = 64;

// 	DictView dict;

// 	dict.keys   = alloc_zero( sizeof(ulong) * dict_start_size );
// 	dict.values = alloc_zero( sizeof(void*) * dict_start_size);

// 	dict.cap = dict_start_size;
// 	dict.len = 0;

// 	return dict;
// }

// unsigned long dict_dbj2(unsigned char *str ,int len)
// {
//     unsigned long hash = 5381;
//     int c;

//     while(len-- && (c = *str++))
//         hash = ((hash << 5) + hash) + c;

//     return hash;
// }

// void dict_view_push( DictView* dict, void* key, StringView value )
// {
// 	ulong index = dict_dbj2(value.ptr, value.len) % dict.cap;
// 	while( keys )

// }	


typedef void (*f_ptr)();


int main(int argc, char *argv[]) 
{
	(void) argv;
	(void) argc;



	// cast(void(*)() , a.ptr )();

	// print( ptr_cap(a) );

	// b->ptr = "";

	// char* a = str_from("teste");

	// print(a);
	






	lexer_load_file("script.cn");
	lexer_print_tokens();

	parser_load_tokens("script.cn", lexer_get_tokens() );

	// // String file = file_to_string("script.cn");
	// // string_print( &file );

	parser_print_ast();





	return 0;

}


