

//int printf ( const char * format, ... );
void PRINT_LOG(const char * format, ...);
void *PLATFORM_MALLOC(size_t size);
void PLATFORM_FREE(void *ptr);



#define CUST_MALLOC(size) cust_wrap_malloc(__LINE__, size);
#define CUST_FREE(ptr) cust_wrap_free(__LINE__, ptr);
void init_for_malloc(void);
void destroy_for_malloc(void);
int retrieve_malloc_chain(void);


