
#ifndef __CUST_MALLOC_H__
#define __CUST_MALLOC_H__

//#include <stdlib.h>
//#include <stdio.h>
#include "signal_chain.h"
#include "main_request.h"
#include "chain_common.h"
//#include "signal_chain.h"

//#define CUST_MALLOC(size) cust_wrap_malloc(__LINE__, size);
//#define CUST_FREE(ptr) cust_wrap_free(__LINE__, ptr);



//for signal_chain diaoyong

void *cust_malloc(size_t size);
void cust_free(void *ptr);



//void init_for_malloc(void);
//void destroy_for_malloc(void);

int retrieve_malloc_chain(void);
#endif /*__CUST_MALLOC_H__*/