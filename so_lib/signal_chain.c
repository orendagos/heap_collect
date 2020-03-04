
#include "signal_chain.h"
//#include <stdlib.h>
//#include <stdio.h>
//this file cannot fused with cust_malloc.c

chain_node* create_data_chain(void* pointer){
	PRINT_LOG("%s entry\n", __func__);
		chain_node* p_chain_node;
	p_chain_node = (chain_node*)cust_malloc(sizeof(chain_node));
	if(p_chain_node == NULL){
		PRINT_LOG("cust_malloc fail \n");
		return NULL;
	}
	p_chain_node->next = NULL;
	p_chain_node->node_info = pointer;
	
	// whether 
	//p_chain_node->node_info->node_info_use_count ++;
	
	PRINT_LOG("%s exit\n", __func__);
	return p_chain_node;
}

//create signal_chain
chain_node* create_signal_chain(void* node_info){
	PRINT_LOG("%s entry\n", __func__);

	chain_node* p_chain_node;
	node_info_t* node_info_p = (node_info_t*)node_info;
	p_chain_node = (chain_node*)cust_malloc(sizeof(chain_node));
	if(p_chain_node == NULL){
		PRINT_LOG("cust_malloc fail \n");
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
	
	PRINT_LOG("pointer:0x%x,line_num:%d\n", node_info_p->pointer, node_info_p->line_num);
	
	PRINT_LOG("%s exit\n", __func__);
	return p_chain_node;
}

//add node to chain to the end
void add_node_to_chain(chain_node* p_chain, chain_node* p_chain_node){
	PRINT_LOG("%s entry\n", __func__);
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
	PRINT_LOG("%s exit\n", __func__);
	return ;
}
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)


void create_and_add_pointer_to_data(chain_node* chain_head,void* pointer){
	PRINT_LOG("%s entry\n", __func__);
	chain_node* p;
	
	if(chain_head == NULL){
		PRINT_LOG("data in node_info is NULL,should create it outside\n");
		return;
	}else{
		PRINT_LOG("chain_head is NOT NULL\n");
	}
	
	chain_node* data_chain = (chain_node*)cust_malloc(sizeof(chain_node));
	if(data_chain== NULL){
		PRINT_LOG("cust_malloc failed\n");
		return;
	}
	data_chain->node_info = pointer;
	add_node_to_chain(chain_head, data_chain);
	
	p = chain_head;
	PRINT_LOG("t0:0x%x\n", chain_head);
	while(p!= NULL){
		PRINT_LOG("info:0x%x\n", p->node_info);
		p = p->next;
	}
	
	PRINT_LOG("%s exit\n", __func__);
	return;
}

void handle_malloc(chain_node* p_chain, node_info_t* node_info){
	PRINT_LOG("%s entry\n", __func__);
	chain_node* p = p_chain->next;
	int i=0;
	int node_count = 0;
	node_info_t* m_node_info;
	node_info_t* node_in_chain;
	PRINT_LOG("mmmmmmmmmmmmmmmmmmmm:begin to handle_malloc,P:0x%x,line_num:%d\n", node_info->pointer,node_info->line_num);
	while(p != NULL){
		PRINT_LOG("%s, node_count:=%d\n", __func__, node_count++);
		//handle_node_for_retrieve((void*)p->node_info);
		node_in_chain = (chain_node*)p->node_info;
		//compare line_num
		if(node_in_chain->line_num == node_info->line_num){
			PRINT_LOG("line_num compare success,count:%d\n", node_in_chain->count);
			// 创建一个data node，并加载data后。
			node_in_chain->count ++;
			chain_node* data_chain = (chain_node*)cust_malloc(sizeof(chain_node));
			data_chain->node_info = node_info->pointer;
			add_node_to_chain((chain_node*)node_in_chain->data, data_chain);
			PRINT_LOG("mmmmmmmmmmmmm:---end to handle_malloc,count:%d\n", node_in_chain->count);
			return;
		}
		
		
		p = p->next;
	}
	//if the line_num doesnot exist,create new node.
	PRINT_LOG("line_num don't compare,create new node\n");
	m_node_info = (node_info_t*)cust_malloc(sizeof(node_info_t));
	if(m_node_info == NULL){
		PRINT_LOG("m_node_info cust_malloc failed\n");
		return;
	}
	memset(m_node_info, '\0', sizeof(node_info_t));
	//memcpy(m_node_info, node_info, sizeof(node_info_t));
	m_node_info->count =1;
	m_node_info->line_num = node_info->line_num;
	//node_info->line_num = node_info->line_num;
	//???应该将pointer放在m_node_info列表里面
	m_node_info->data = (void*)create_data_chain(NULL);
	create_and_add_pointer_to_data((chain_node*)m_node_info->data, node_info->pointer);
	
	chain_node* chain_node_p =  create_signal_chain(m_node_info);//create data chain inside
	handle_node_for_retrieve((void*)chain_node_p->node_info);
	
	add_node_to_chain(p_chain, chain_node_p);
	handle_node_for_retrieve((void*)chain_node_p->node_info);
	PRINT_LOG("%s exit\n", __func__);
	PRINT_LOG("mmmmmmmmmmmmm:---end to handle_malloc\n");
}

int check_node_match(void *t, void *expect_t, size_t offset0, size_t offset1){
	PRINT_LOG("%s entry\n", __func__);
	int ret;
	char* p_data = (char*)(t)+offset0;
	char* p_expect_data =(char*)(expect_t)+offset0;
	
	ret = memcmp(p_data, p_expect_data,offset1-offset0);
	PRINT_LOG("%s,ret:%d\n",__func__, ret);
	PRINT_LOG("%s exit\n", __func__);
	return ret;
	
}

//del node in chain
//orenda:should make sure the implement of node_match.
void del_node_in_chain(chain_node* p_chain, void* expect_node_info,  size_t offset0, size_t offset1){
	PRINT_LOG("%s entry\n", __func__);
	chain_node* p = p_chain;
	chain_node* p_bak;
	int ret = -1;
	while(p->next != NULL){
		//judge whether del the node 
		// src is p->next, dst is expect_p
		//if(p->next->node_info->data == data){
			//del this node
		ret = check_node_match((void*)p->next->node_info, (void*)expect_node_info, offset0,offset1);
		//node_match_array(p->next->node_info, expect_node_info, data, line_num);//error, data shoud be one of node_info->data???
		
		if(!ret){
			p_bak = p->next->next;
			//cust_free(p->next);
			
			cust_free_with_clear(&(p->next));
			
			p->next = p_bak;
			break;
		}
		p = p->next;
	}
	PRINT_LOG("%s exit\n", __func__);
}




int __WEAK handle_node_for_retrieve(void* node_info)
{
    PRINT_LOG("%s weak implement\n", __func__);
    return;
}

//retireve node //don't implement
//return the number of node:node_count
int retrieve_chain(chain_node* p_chain){
	PRINT_LOG("%s entry\n", __func__);
	//chain_node* p = p_chain->next;
	chain_node* p = p_chain;
	int i=0;
	int node_count = 0;
	int non_zero_line_count = 0;//the count sum which line_num is not zero
	while(p != NULL){
		PRINT_LOG("****main_chain:%p,node_info:0x%x******%s, node_count:=%d\n",p, p->node_info,  __func__, node_count++);
		non_zero_line_count += handle_node_for_retrieve((void*)p->node_info);
		p = p->next;
	}
	PRINT_LOG("%s exit\n", __func__);
	return non_zero_line_count;
}

//copy node_info_t
void copy_node_item(void *t, void *expect_t, size_t offset0, size_t offset1){
	PRINT_LOG("%s entry\n", __func__);
	//	int ret;
	char* p_data = (char*)(t)+offset0;
	char* p_expect_data =(char*)(expect_t)+offset0;
	
	memcpy(p_data, p_expect_data,offset1-offset0);
	//PRINT_LOG("%s,ret:%d, %d,%d\n",__func__, ret, t->unused, expect_t->unused);
	PRINT_LOG("%s exit\n", __func__);
	return;
}

//expect_node_info is 
void sync_new_node_info(chain_node* p_chain, void* expect_node_info, void* new_node_info,  size_t offset0, size_t offset1){
	PRINT_LOG("%s entry\n", __func__);
		chain_node* p = p_chain;
	chain_node* p_bak;
	int ret = -1;
	while(p->next != NULL){
		//judge whether del the node 
		// src is p->next, dst is expect_p
		//if(p->next->node_info->data == data){
			//del this node
		ret = check_node_match(p->next->node_info, expect_node_info, offset0,offset1);
		//node_match_array(p->next->node_info, expect_node_info, data, line_num);//error, data shoud be one of node_info->data???
		
		if(!ret){// compare success
			// target is p->next
			//memcpy(p->next->node_info, expect_node_info, sizeof(node_info_t));
			copy_node_item(p->next->node_info, new_node_info, offset0, offset1);
			break;
		}
		p = p->next;
	}
	PRINT_LOG("%s exit\n", __func__);
}

//del chain
void destroy_chain_data(chain_node* p_chain){//chain is a p_chain_node too.
PRINT_LOG("%s entry\n", __func__);
	chain_node* p_chain_node;
	chain_node* p;
	p_chain_node = p_chain;
	p = p_chain;
	/*
	while(p != NULL){
		PRINT_LOG("0x%x,", p);
		p = p->next;
	}
	*/
	
	if(p_chain_node->next != NULL){
		destroy_chain_data(p_chain_node->next);
		p_chain_node->next = NULL;

	}
	//cust_free(p_chain_node);
	PRINT_LOG("0220\n");
	cust_free(p_chain_node);
	p_chain_node = NULL;
	PRINT_LOG("%s exit\n", __func__);
}

//one should notice that:call p_chain = NULL;after destroy_chain.Or errors will be occured when call retrieve
void destroy_chain(chain_node* p_chain){//chain is a p_chain_node too.
	PRINT_LOG("%s entry\n", __func__);
	chain_node* p_chain_node;
	p_chain_node = p_chain;
	node_info_t* node_info = (node_info_t*)p_chain_node->node_info;
	if(p_chain_node->next != NULL){
		destroy_chain(p_chain_node->next);
		p_chain_node->next = NULL;

	}
	//cust_free(p_chain_node);
	destroy_chain_data((chain_node*)node_info->data);
	cust_free_with_clear(&p_chain_node);
	PRINT_LOG("%s exit, p_chain=%p\n", __func__, p_chain);
}



// for cust_malloc
//this ptr is the address of the pointer
void cust_free_with_clear(void **ptr){
	PRINT_LOG("%s entry\n", __func__);

chain_node* p_chain = (chain_node*)(*ptr);
PRINT_LOG("%s,*ptr:%p\n", __func__, *ptr);
#if 0
if(p_chain->node_info->node_info_use_count>1){
	PRINT_LOG("this node has been used,node_info_use_count:%d\n",p_chain->node_info->node_info_use_count--);
}else{
	cust_free(p_chain->node_info);
	p_chain->node_info = NULL;
}
#else
	if(p_chain->node_info != NULL){
		cust_free(p_chain->node_info);
		p_chain->node_info = NULL;
	}else{
		PRINT_LOG("has been freed\n");
	}
	
#endif

	if(*ptr != NULL){
		cust_free(*ptr);
		}
	*ptr = NULL;
	PRINT_LOG("%s exit\n", __func__);
} 

