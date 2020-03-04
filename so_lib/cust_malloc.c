#include "cust_malloc.h"

static chain_node* p_chain;

//因为malloc只有一个参数size，所以无法区分不同的malloc。结尾处新建了cust_wrap_malloc用行号区分malloc，从而定位问题。
void* __wrap_cust_malloc(size_t size){
	PRINT_LOG("%s entry\n", __func__);
	void* p;
	
	p = __real_cust_malloc(size);//调用真正的malloc
	PRINT_LOG("__wrap_malloc called,cust_malloc:0x%x\n", p);
PRINT_LOG("%s exit\n", __func__);
	return p;


}

void __wrap_cust_free(void *ptr){
	PRINT_LOG("%s entry\n", __func__);
	PRINT_LOG("__wrap_free called,%p has been freed\n", ptr);
	PRINT_LOG("%s exit\n", __func__);
	return __real_cust_free(ptr); //调用真正的malloc
	
    

}

void *cust_malloc(size_t size){
	void* p;
	PRINT_LOG("%s entry\n", __func__);
	// p = malloc(size);
	p = PLATFORM_MALLOC(size);
	PRINT_LOG("orenda_malloc:0x%x\n", p);
	PRINT_LOG("%s exit\n", __func__);
	return p;
	
}

void cust_free(void *ptr){
	PRINT_LOG("%s entry\n", __func__);
	PRINT_LOG("orenda_free:0x%x\n", ptr);
	//free(ptr);
	PLATFORM_FREE(ptr);
	PRINT_LOG("%s exit\n", __func__);
}


void init_for_malloc(){
	
	PRINT_LOG("%s entry\n", __func__);
	node_info_t* node_info;
	node_info = (node_info_t*)cust_malloc(sizeof(node_info_t));
	if(node_info == NULL){
        PRINT_LOG("cust_malloc failed,%d\n",__LINE__);
		return ;
	}
	node_info->line_num = 0;
	PRINT_LOG("mark,%d\n", __LINE__);
	p_chain = create_signal_chain(node_info);
	PRINT_LOG("%s exit\n", __func__);
	//retrieve_chain(p_chain);
}
void destroy_for_malloc(){
	PRINT_LOG("%s entry\n", __func__);
	destroy_chain(p_chain);
	p_chain = NULL;
	PRINT_LOG("%s exit\n", __func__);
}
//return the count of node in which non-zero line_num
//I care the line_num which count is not 0.
//so,print the valuable info inside,and return the total count number.
int retrieve_malloc_chain(){
	int ret;
	PRINT_LOG("%s entry\n", __func__);
	ret = retrieve_chain(p_chain);
	PRINT_LOG("%s exit,%d\n", __func__, ret);
	return ret;
}

void *cust_wrap_malloc(int line_num, size_t size){
	PRINT_LOG("%s entry\n", __func__);
	void* p;
	node_info_t node_info;
	p = cust_malloc(size);
	node_info.pointer = p;
	node_info.line_num = line_num;
	//add_node_to_chain(p_chain, &node_info, 1);//add to
	PRINT_LOG("mark,%d\n", __LINE__);
	handle_malloc(p_chain, &node_info);
	PRINT_LOG("%s exit\n", __func__);
	return p;
}
//check whether pointer in the data_chain
int pointer_in_chain(void* pointer, chain_node* data_chain){
	PRINT_LOG("%s entry\n", __func__);
	int ret = -1;
	chain_node* p = data_chain->next;
	void* node_info;
	while(p != NULL){
		if(p->node_info == pointer){
			ret = 0;
			break;
		}
		p = (chain_node*)(p->next);
	}
	PRINT_LOG("%s,ret:%d\n", __func__, ret);
	PRINT_LOG("%s exit\n", __func__);
	return ret;
	
}

void handle_free(chain_node* p_chain, node_info_t* node_info){
	PRINT_LOG("%s entry\n", __func__);
		chain_node* p = p_chain->next;
	int i=0;
	int node_count = 0;
	node_info_t* node_in_chain;
	while(p != NULL){
		PRINT_LOG("%s, node_count:=%d\n", __func__, node_count++);
		//handle_node_for_retrieve((void*)p->node_info);
		node_in_chain = (chain_node*)p->node_info;
		//if pointer in the node_in_chain->data
		if(!pointer_in_chain(node_info->pointer, node_in_chain->data)){
			PRINT_LOG("cust_free,so the count dec,line_num:%d,count:%d\n", node_in_chain->line_num, node_in_chain->count);
			node_in_chain->count --;
			// del the node in the chain,but maybe useless
			return;
		}
		p = p->next;
	}
	
	
}

void *cust_wrap_free(int line_num, void *ptr){
	PRINT_LOG("%s entry\n", __func__);
	node_info_t node_info;
	node_info.pointer = ptr;
	PRINT_LOG("prepare to cust_free:0x%x\n", ptr);
	handle_free(p_chain, &node_info);
	cust_free(ptr);
	//ptr = NULL;
	PRINT_LOG("%s exit\n", __func__);
}



int handle_node_for_retrieve(void* m_node_info)
{
	PRINT_LOG("%s entry\n", __func__);
	node_info_t* node_info = (node_info_t*)m_node_info;
	chain_node* chain_node_p;
	int line_num,count;
	int ret = 0;
	
	if(node_info == NULL){
		return 0;
	}
	
	PRINT_LOG("begin to retrieve---------sub_chain:0x%x------------------\n", node_info->data);
	chain_node_p = (chain_node*)node_info->data;
	
	count = node_info->count;
	line_num = node_info->line_num;

	PRINT_LOG("line_num:%d,count:%d;;", line_num, count);
	while(chain_node_p != NULL){
		PRINT_LOG("0x%x,", chain_node_p->node_info);
		chain_node_p = chain_node_p->next;
	}
	PRINT_LOG("\n");
    PRINT_LOG("end to retrieve++++++++++++++++++++++++++++++\n");


	
	ret = line_num>0?count:0;
	PRINT_LOG("%s exit,ret:%d\n", __func__, ret);
    return ret;
}


void __WEAK PRINT_LOG(const char * cmd, ...){

	return;
}
void* __WEAK PLATFORM_MALLOC(size_t size){
	return ;
}

void __WEAK PLATFORM_FREE(void *ptr){
	return;
}