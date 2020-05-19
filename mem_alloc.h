
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *mem_alloc_memory(size_t s);
void mem_free_memory(void *p);
char *mem_delete_refer(void *p);
char *mem_add_refer(void *p);
void mem_update(void);