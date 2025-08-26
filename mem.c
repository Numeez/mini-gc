#include "mem.h"
#include <stdio.h>

#undef realloc
#undef malloc
#undef free
#undef calloc
#include <stdlib.h> 

static int allocation_count = 0;


void* mem_calloc(size_t count, size_t size) {
    void* ptr = calloc(count, size);
    if (ptr != NULL) allocation_count++;
    return ptr;
}
void* mem_realloc(void* ptr, size_t size){
    if (ptr == NULL) {
        allocation_count++;
        return realloc(NULL, size);
    }
    void* new_ptr = realloc(ptr, size);
    if (new_ptr == NULL) {
        return NULL;
    }
    // No need to increment allocation_count: it's still the same allocation
    return new_ptr;
}

void* mem_malloc(size_t size){
    void* ptr = malloc(size);
    if (ptr!=NULL){
        // fprintf(stdout, "[mem] allocation_count=%d\n", allocation_count);
        allocation_count++;
    }  
    return ptr; 
}

void mem_free(void* ptr){
    if (ptr!=NULL){
    //  fprintf(stdout, "[mem] allocation_count=%d\n", allocation_count);
    allocation_count--;
    free(ptr);
    }
}

bool mem_all_free(){
    // fprintf(stdout, "[mem] allocation_count=%d\n", allocation_count);
    return allocation_count==0;
}

void mem_reset_counter(){
    allocation_count=0;
}