#include <stddef.h>
#include <stdbool.h>

void* mem_malloc(size_t size);
void* mem_realloc(void* ptr,size_t size);
void* mem_calloc(size_t count,size_t size);
void mem_free(void* ptr);
bool mem_all_free();
void mem_reset_counter();


#define malloc(size) mem_malloc(size)
#define free(ptr) mem_free(ptr)
#define realloc(ptr,size) mem_realloc(ptr,size)
#define calloc(count,size) mem_calloc(count,size)
