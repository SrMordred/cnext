#pragma once
#include <stdlib.h>


typedef struct
{
    void*   memory_block;
    size_t  current_position;

} MemoryArena ;

MemoryArena memory_arena( const size_t len )
{
    MemoryArena arena;
    arena.memory_block     = malloc( len );
    if(!arena.memory_block){
        perror("Malloc Error:");
    }
    arena.current_position = 0;

    return arena;
}

void* memory_arena_alloc(MemoryArena* arena, const size_t len )
{
    void* block = arena->memory_block + arena->current_position;
    arena->current_position += len;
    return block;
}

void memory_arena_reset(MemoryArena* arena)
{
    arena->current_position = 0;
}

void memory_arena_free( MemoryArena* arena )
{
    free( arena->memory_block );
}