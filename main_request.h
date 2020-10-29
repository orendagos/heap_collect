#include <stdlib.h>
#define LOG_MSG_ERROR 2
#define LOG_MSG_INFO 3
#define LOG_MSG_DEBUG 4

//int printf ( const char * format, ... );
void PRINT_LOG(int level, const char * format, ...);
void *PLATFORM_MALLOC(size_t size);
void PLATFORM_FREE(void *ptr);



#define CUST_MALLOC(size) cust_wrap_malloc(__LINE__, size);
#define CUST_FREE(ptr) cust_wrap_free(__LINE__, ptr);
void init_for_malloc(void);
void destroy_for_malloc(void);
int retrieve_malloc_chain(void);
/*
typedef struct node_info{
	int id;
	//int node_info_use_count;
	int count;
	void* data;//pointer
	int line_num;// for match
	unsigned long pointer; // for match
	int unused;
	int unused1;
}node_info_t;
*/
//int __WEAK handle_node_for_retrieve(void* m_node_info);

