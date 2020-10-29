
#ifndef __CUST_MALLOC_H__
#define __CUST_MALLOC_H__

#include <stdlib.h>
//#include <stdio.h>
#include "signal_chain.h"
#include "chain_common.h"
//#include "signal_chain.h"

//#define CUST_MALLOC(size) cust_wrap_malloc(__LINE__, size);
//#define CUST_FREE(ptr) cust_wrap_free(__LINE__, ptr);



//for signal_chain diaoyong
void *cust_malloc(size_t size);
void cust_free(void *ptr);



void init_for_malloc(void);
void destroy_for_malloc(void);

int retrieve_malloc_chain(void);
int handle_node_for_retrieve(void* m_node_info);
void PRINT_LOG(int level, const char * cmd, ...);
void* PLATFORM_MALLOC(size_t size);
void PLATFORM_FREE(void *ptr);
int handle_node_for_retrieve(void* node_info);


void *cust_wrap_malloc(int line_num, size_t size);
void cust_wrap_free(int line_num, void *ptr);
#endif /*__CUST_MALLOC_H__*/