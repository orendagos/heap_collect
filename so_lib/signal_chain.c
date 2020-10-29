
#include "signal_chain.h"
//#include <stdlib.h>
//#include <stdio.h>
//this file cannot fused with cust_malloc.c

//this ptr is the address of the pointer
static void cust_free_with_clear(void **ptr){
	PRINT_LOG(LOG_MSG_DEBUG, "%s entry\n", __func__);

chain_node* p_chain = (chain_node*)(*ptr);
PRINT_LOG(LOG_MSG_DEBUG, "%s,*ptr:%p\n", __func__, *ptr);
#if 0
if(p_chain->node_info->node_info_use_count>1){
	PRINT_LOG(LOG_MSG_DEBUG, "this node has been used,node_info_use_count:%d\n",p_chain->node_info->node_info_use_count--);
}else{
	cust_free(p_chain->node_info);
	p_chain->node_info = NULL;
}
#else
	if(p_chain->node_info != NULL){
		cust_free(p_chain->node_info);
		p_chain->node_info = NULL;
	}else{
		PRINT_LOG(LOG_MSG_DEBUG, "has been freed\n");
	}
	
#endif

	if(*ptr != NULL){
		cust_free(*ptr);
		}
	*ptr = NULL;
	PRINT_LOG(LOG_MSG_DEBUG, "%s exit\n", __func__);
} 

chain_node* create_data_chain(void* pointer){
	PRINT_LOG(LOG_MSG_DEBUG, "%s entry\n", __func__);
		chain_node* p_chain_node;
	p_chain_node = (chain_node*)cust_malloc(sizeof(chain_node));
	if(p_chain_node == NULL){
		PRINT_LOG(LOG_MSG_DEBUG, "cust_malloc fail \n");
		return NULL;
	}
	p_chain_node->next = NULL;
	p_chain_node->node_info = pointer;
	
	// whether 
	//p_chain_node->node_info->node_info_use_count ++;
	
	PRINT_LOG(LOG_MSG_DEBUG, "%s exit\n", __func__);
	return p_chain_node;
}

//create signal_chain
chain_node* create_malloc_node(void* node_info){
	PRINT_LOG(LOG_MSG_DEBUG, "%s entry\n", __func__);

	chain_node* p_chain_node;
	node_info_t* node_info_p = (node_info_t*)node_info;
	p_chain_node = (chain_node*)cust_malloc(sizeof(chain_node));
	if(p_chain_node == NULL){
		PRINT_LOG(LOG_MSG_ERROR, "cust_malloc fail \n");
		return NULL;
	}
	//node_info_p.count = 0;
	if(node_info_p->data == NULL){
		node_info_p->data = (void*)create_data_chain(NULL);
	}
	
	p_chain_node->next = NULL;
	p_chain_node->node_info = node_info_p;

	// whether 
	//p_chain_node->node_info->node_info_use_count ++;
	
	PRINT_LOG(LOG_MSG_DEBUG, "pointer:0x%x,line_num:%d\n", node_info_p->pointer, node_info_p->line_num);
	
	PRINT_LOG(LOG_MSG_DEBUG, "%s exit\n", __func__);
	return p_chain_node;
}

void create_and_add_pointer_to_data(chain_node* chain_head,void* pointer){
	PRINT_LOG(LOG_MSG_DEBUG, "%s entry\n", __func__);
	chain_node* p;
	
	if(chain_head == NULL){
		PRINT_LOG(LOG_MSG_ERROR, "data in node_info is NULL,should create it outside\n");
		return;
	}else{
		PRINT_LOG(LOG_MSG_DEBUG, "chain_head is not null, so create data_chain and add to tail\n");
	}
	
	chain_node* data_chain = (chain_node*)cust_malloc(sizeof(chain_node));
	if(data_chain== NULL){
		PRINT_LOG(LOG_MSG_ERROR, "cust_malloc failed\n");
		return;
	}
	data_chain->node_info = pointer;
	add_node_to_chain(chain_head, data_chain);
	
	p = chain_head;
	PRINT_LOG(LOG_MSG_DEBUG, "t0:0x%x\n", chain_head);
	while(p!= NULL){
		PRINT_LOG(LOG_MSG_DEBUG, "info:0x%x\n", p->node_info);
		p = p->next;
	}
	
	PRINT_LOG(LOG_MSG_DEBUG, "%s exit\n", __func__);
	return;
}

void add_node_to_chain(chain_node* p_chain, chain_node* p_chain_node){
	PRINT_LOG(LOG_MSG_DEBUG, "%s entry\n", __func__);
	chain_node* p;
	
	p=p_chain;
	if(p_chain_node == NULL){
		return;
	}
	while(p->next!=NULL){
		p=p->next;
	}
	
	p->next = p_chain_node;
	p_chain_node->next = NULL;
	PRINT_LOG(LOG_MSG_DEBUG, "%s exit\n", __func__);
	return ;
}

//retireve node //don't implement
//return the number of node:node_count
int retrieve_chain(chain_node* p_chain){
	PRINT_LOG(LOG_MSG_DEBUG, "%s entry\n", __func__);
	if(p_chain == NULL){
		PRINT_LOG(LOG_MSG_DEBUG, "p_chain is NULL\n");
		return 0;
	}
	chain_node* p = p_chain->next;
	//chain_node* p = p_chain;
	int node_count = 0;
	int non_zero_line_count = 0;//the count sum which line_num is not zero
	PRINT_LOG(LOG_MSG_INFO, "begin to retieve malloc info=================>\n");
	while(p != NULL){ //node_info 里面的data 头和chain_node是共存的。
		//PRINT_LOG(LOG_MSG_INFO, "****malloc_chain:count:%d[line_num:%d]", );//            %p,node_info:0x%x******%s, node_count:=%d\n",p, p->node_info,  __func__, node_count++);
		PRINT_LOG(LOG_MSG_DEBUG, "****malloc_chain: %p,node_info:0x%x******%s, node_count:=%d\n",p, p->node_info,  __func__, node_count++);
		non_zero_line_count += handle_node_for_retrieve((void*)p->node_info);
		p = p->next;
	}
	PRINT_LOG(LOG_MSG_INFO, "node_count:%d, non_zero_line_count:%d,\n end to retieve malloc info<=================\n", node_count, non_zero_line_count);
	PRINT_LOG(LOG_MSG_DEBUG, "%s exit\n", __func__);
	return non_zero_line_count;
}

//del chain
static void destroy_chain_data(chain_node* p_chain){//chain is a p_chain_node too.
PRINT_LOG(LOG_MSG_DEBUG, "%s entry\n", __func__);
	chain_node* p_chain_node;
	p_chain_node = p_chain;
	/*
	while(p != NULL){
		PRINT_LOG(LOG_MSG_DEBUG, "0x%x,", p);
		p = p->next;
	}
	*/
	
	if(p_chain_node->next != NULL){
		destroy_chain_data(p_chain_node->next);
		p_chain_node->next = NULL;

	}
	//cust_free(p_chain_node);
	cust_free(p_chain_node);
	p_chain_node = NULL;
	PRINT_LOG(LOG_MSG_DEBUG, "%s exit\n", __func__);
}

//one should notice that:call p_chain = NULL;after destroy_malloc_chain.Or errors will be occured when call retrieve
void destroy_malloc_chain(chain_node* p_chain){//chain is a p_chain_node too.
	PRINT_LOG(LOG_MSG_DEBUG, "%s entry\n", __func__);
	chain_node* p_chain_node;
	p_chain_node = p_chain;
	node_info_t* node_info = (node_info_t*)p_chain_node->node_info;
	if(p_chain_node->next != NULL){
		destroy_malloc_chain(p_chain_node->next);
		p_chain_node->next = NULL;

	}
	//cust_free(p_chain_node);
	destroy_chain_data((chain_node*)node_info->data);
	cust_free_with_clear((void**)(&p_chain_node));
	PRINT_LOG(LOG_MSG_DEBUG, "%s exit, p_chain=%p\n", __func__, p_chain);
}

