#include "stdio.h"

#include "main_request.h"
#include <stdlib.h>
#include <stdarg.h>  
#include <assert.h>

#define MALLOC_TIMES 50

/*realise

*/
void PRINT_LOG(const char * cmd, ...){
	//printf("%s %s ", __DATE__, __TIME__);  
    va_list args;       //定义一个va_list类型的变量，用来储存单个参数  
    va_start(args,cmd); //使args指向可变参数的第一个参数  
    vprintf(cmd,args);  //必须用vprintf等带V的  
    va_end(args);       //结束可变参数的获取
    //printf("\n"); 
	return;
}
void *PLATFORM_MALLOC(size_t size){
	return malloc(size);
}

void PLATFORM_FREE(void *ptr){
	free(ptr);
}

//test items
/*
#define CUST_MALLOC(size) cust_wrap_malloc(__LINE__, size);
#define CUST_FREE(ptr) cust_wrap_free(__LINE__, ptr);
void init_for_malloc(void);
void destroy_for_malloc(void);
*/
int test_init_destroy(){
	int ret = 0;
	init_for_malloc();
	ret = retrieve_malloc_chain();
	assert(ret == 0);
	destroy_for_malloc();
	ret = retrieve_malloc_chain();
	assert(ret == 0);
	return ret;
	
}



int test_1(){
	printf("%s entry\n", __func__);
	char* p[100];
	int i=0;
	int ret = -1;
	
	init_for_malloc();
	ret = retrieve_malloc_chain();
	assert(ret == 0);
	printf("init_for_malloc end\n");
	for(i=0;i<MALLOC_TIMES;i++){
		p[i] = (char*)CUST_MALLOC(100);
		printf("CUST_MALLOC end:%p\n",p[i]);
		//retrieve_malloc_chain();
	}
	ret = retrieve_malloc_chain();
	assert(ret == MALLOC_TIMES);
	for(i=0;i<MALLOC_TIMES;i++){
		CUST_FREE(p[i]);
		p[i] = NULL;
	}
	ret = retrieve_malloc_chain();
	assert(ret == 0);
out:

	destroy_for_malloc();
	assert(retrieve_malloc_chain() == 0);
	return 0;
	printf("%s exit\n", __func__);
}

int main(){
	int ret = -1;
	assert(ret == -1);//run well
	printf("%s entry\n", __func__);
    //ret = test_1();

	test_init_destroy();
	
printf("%s exit\n", __func__);
return 0;
}