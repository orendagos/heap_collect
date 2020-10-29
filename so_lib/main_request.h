#ifndef __MAIN_REQUEST_H__
#define __MAIN_REQUEST_H__
#include <stdlib.h>
//int printf ( const char * format, ... );
void PRINT_LOG(int level, const char * format, ...);
void *PLATFORM_MALLOC(size_t size);
void PLATFORM_FREE(void *ptr);

#define CUST_MALLOC(size) cust_wrap_malloc(__LINE__, size);
#define CUST_FREE(ptr) cust_wrap_free(__LINE__, ptr);
void init_for_malloc(void);
void destroy_for_malloc(void);
int retrieve_malloc_chain(void);
#endif // __MAIN_REQUEST_H__

