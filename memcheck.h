#ifndef MEMCHECK_H
#define MEMCHECK_H

#include <stdlib.h>
#include <stdio.h>

#define malloc(size)        memcheck_malloc(size, __FILE__, __LINE__)
#define free(ptr)           memcheck_free(ptr, __FILE__, __LINE__)
#define main()              memcheck_main()
#define calloc(nmemb, size) memcheck_calloc(nmemb, size, __FILE__, __LINE__)
#define realloc(ptr, size)  memcheck_realloc(ptr, size, __FILE__, __LINE__)

int memcheck_main();
void memcheck_free(void *ptr, char *file, int line);
void *memcheck_malloc(size_t size, char *file, int line);
void *memcheck_calloc(size_t nmemb, size_t size, char *file, int line);
void *memcheck_realloc(void *ptr, size_t size, char *file, int line);

#endif
