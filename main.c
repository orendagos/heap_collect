#include "stdio.h"

#include "main_request.h"
#include <stdlib.h>
#include <stdarg.h>  
#include <assert.h>

#define MALLOC_TIMES 2

/*realise

*/
void print_log_level(int level){
	switch (level)
	{
	case LOG_MSG_ERROR:
		/* code */
		printf("[LOG_MSG_ERROR] ");
		break;
	case LOG_MSG_INFO:
		/* code */
		// printf("[LOG_MSG_INFO] "); // ignore the LOG_MSG_INFO,for special console output
		break;
	case LOG_MSG_DEBUG:
		/* code */
		printf("[LOG_MSG_DEBUG] ");
		break;
	
	default:
		break;
	}
}
void PRINT_LOG(int level, const char * cmd, ...){
	if(level > LOG_MSG_INFO){
		return;
	}
	print_log_level(level);
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
	//assert(retrieve_malloc_chain() == 0);
	printf("init_for_malloc end\n");
	for(i=0;i<MALLOC_TIMES;i++){
		p[i] = (char*)CUST_MALLOC(100);
		//printf("CUST_MALLOC end:%p\n",p[i]);
	}
	//ret = retrieve_malloc_chain();
	//assert(ret == MALLOC_TIMES);
	for(i=0;i<MALLOC_TIMES;i++){
		CUST_FREE(p[i]);
		p[i] = NULL;
	}
	//ret = retrieve_malloc_chain();
	//assert(ret == 0);

	p[1] = (char*)CUST_MALLOC(100);
	p[2] = (char*)CUST_MALLOC(100);
	p[3] = (char*)CUST_MALLOC(100);
	p[4] = (char*)CUST_MALLOC(100);
	CUST_FREE(p[4]);
	CUST_FREE(p[3]);
	CUST_FREE(p[2]);
	CUST_FREE(p[1]);
out:

	destroy_for_malloc();
	//assert(retrieve_malloc_chain() == 0);
	return 0;
	printf("%s exit\n", __func__);
}

int main(){
	int ret = -1;
	assert(ret == -1);//run well
	printf("%s entry\n", __func__);
    ret = test_1();

	//test_init_destroy();
	
printf("%s exit\n", __func__);
return 0;
}