#include <string.h>
#include "cust_malloc.h"
#include "signal_chain.h"

static chain_node* p_chain;
static void handle_malloc(chain_node* p_chain, node_info_t* node_info);
void* __real_cust_malloc(size_t size);
void __real_cust_free(void *ptr);

//因为malloc只有一个参数size，所以无法区分不同的malloc。结尾处新建了cust_wrap_malloc用行号区分malloc，从而定位问题。
void* __wrap_cust_malloc(size_t size){
	PRINT_LOG(LOG_MSG_DEBUG, "%s entry\n", __func__);
	void* p;
	
	p = __real_cust_malloc(size);//调用真正的malloc
	PRINT_LOG(LOG_MSG_DEBUG, "__wrap_malloc called,cust_malloc:0x%x\n", p);
PRINT_LOG(LOG_MSG_DEBUG, "%s exit\n", __func__);
	return p;


}

void __wrap_cust_free(void *ptr){
	PRINT_LOG(LOG_MSG_DEBUG, "%s entry\n", __func__);
	PRINT_LOG(LOG_MSG_DEBUG, "__wrap_free called,%p has been freed\n", ptr);
	PRINT_LOG(LOG_MSG_DEBUG, "%s exit\n", __func__);
	__real_cust_free(ptr); //调用真正的malloc
	return;
	
    

}

void *cust_malloc(size_t size){
	void* p;
	PRINT_LOG(LOG_MSG_DEBUG, "%s entry\n", __func__);
	// p = malloc(size);
	p = PLATFORM_MALLOC(size);
	PRINT_LOG(LOG_MSG_DEBUG, "orenda_malloc:0x%x\n", p);
	PRINT_LOG(LOG_MSG_DEBUG, "%s exit\n", __func__);
	return p;
	
}

void cust_free(void *ptr){
	PRINT_LOG(LOG_MSG_DEBUG, "%s entry\n", __func__);
	PRINT_LOG(LOG_MSG_DEBUG, "orenda_free:0x%x\n", ptr);
	//free(ptr);
	PLATFORM_FREE(ptr);
	PRINT_LOG(LOG_MSG_DEBUG, "%s exit\n", __func__);
}


void init_for_malloc(){
	
	PRINT_LOG(LOG_MSG_DEBUG, "%s entry\n", __func__);
	node_info_t* node_info;
	node_info = (node_info_t*)cust_malloc(sizeof(node_info_t));
	if(node_info == NULL){
        PRINT_LOG(LOG_MSG_ERROR, "cust_malloc failed,%d\n",__LINE__);
		return ;
	}
	node_info->line_num = 0;
	PRINT_LOG(LOG_MSG_DEBUG, "mark,%d\n", __LINE__);
	p_chain = create_malloc_node(node_info);
	PRINT_LOG(LOG_MSG_DEBUG, "%s exit\n", __func__);
	//retrieve_chain(p_chain);
}
void destroy_for_malloc(){
	PRINT_LOG(LOG_MSG_DEBUG, "%s entry\n", __func__);
	destroy_malloc_chain(p_chain);
	p_chain = NULL;
	PRINT_LOG(LOG_MSG_DEBUG, "%s exit\n", __func__);
}
//return the count of node in which non-zero line_num
//I care the line_num which count is not 0.
//so,print the valuable info inside,and return the total count number.
int retrieve_malloc_chain(){
	int ret;
	PRINT_LOG(LOG_MSG_DEBUG, "%s entry\n", __func__);
	ret = retrieve_chain(p_chain);
	PRINT_LOG(LOG_MSG_DEBUG, "%s exit,%d\n", __func__, ret);
	return ret;
}

void *cust_wrap_malloc(int line_num, size_t size){
	PRINT_LOG(LOG_MSG_DEBUG, "%s entry\n", __func__);
	void* p;
	node_info_t node_info;
	p = cust_malloc(size);
	node_info.pointer = (unsigned long)p;
	node_info.line_num = line_num;
	PRINT_LOG(LOG_MSG_DEBUG, "mark,%d\n", __LINE__);
	handle_malloc(p_chain, &node_info);
	PRINT_LOG(LOG_MSG_DEBUG, "%s exit\n", __func__);
	retrieve_malloc_chain();
	return p;
}
//check whether pointer in the data_chain
// if exist,delete it.
static int pointer_in_chain_and_del(unsigned long pointer, chain_node* data_chain){
	PRINT_LOG(LOG_MSG_DEBUG, "%s entry\n", __func__);
	int ret = -1;
	chain_node* p = data_chain->next;
	chain_node* parent_ptr = data_chain;
	//void* node_info;
	while(p != NULL){
		if((unsigned long)(p->node_info) == pointer){
			ret = 0;
			//delete node
			parent_ptr->next = p->next;
			
			cust_free(p);
			p = NULL;
			break;
		}
		parent_ptr = p;
		p = (chain_node*)(p->next);
	}
	PRINT_LOG(LOG_MSG_DEBUG, "%s,ret:%d\n", __func__, ret);
	PRINT_LOG(LOG_MSG_DEBUG, "%s exit\n", __func__);
	return ret;
	
}

static void handle_malloc(chain_node* p_chain, node_info_t* node_info){
	PRINT_LOG(LOG_MSG_DEBUG, "%s entry\n", __func__);
	chain_node* p = p_chain->next;
	int node_count = 0;
	node_info_t* m_node_info;
	node_info_t* node_in_chain;
	PRINT_LOG(LOG_MSG_DEBUG, "mmmmmmmmmmmmmmmmmmmm:begin to handle_malloc,P:0x%x,line_num:%d\n", node_info->pointer,node_info->line_num);
	while(p != NULL){
		PRINT_LOG(LOG_MSG_DEBUG, "%s, node_count:=%d\n", __func__, node_count++);
		//handle_node_for_retrieve((void*)p->node_info);
		node_in_chain = (node_info_t*)p->node_info;
		//compare line_num
		if(node_in_chain->line_num == node_info->line_num){
			PRINT_LOG(LOG_MSG_DEBUG, "line_num compare success,count:%d\n", node_in_chain->count);
			// ����һ��data node��������data��
			node_in_chain->count ++;
			chain_node* data_chain = (chain_node*)cust_malloc(sizeof(chain_node));
			data_chain->node_info = (void*)node_info->pointer;
			add_node_to_chain((chain_node*)node_in_chain->data, data_chain);
			PRINT_LOG(LOG_MSG_DEBUG, "mmmmmmmmmmmmm:---end to handle_malloc,count:%d\n", node_in_chain->count);
			return;
		}
		
		
		p = p->next;
	}
	//if the line_num doesnot exist,create new node.
	PRINT_LOG(LOG_MSG_DEBUG, "line_num don't compare,create new node\n");
	m_node_info = (node_info_t*)cust_malloc(sizeof(node_info_t));
	if(m_node_info == NULL){
		PRINT_LOG(LOG_MSG_ERROR, "m_node_info cust_malloc failed\n");
		return;
	}
	memset(m_node_info, '\0', sizeof(node_info_t));
	//memcpy(m_node_info, node_info, sizeof(node_info_t));
	m_node_info->count =1;
	m_node_info->line_num = node_info->line_num;
	//node_info->line_num = node_info->line_num;
	//???Ӧ�ý�pointer����m_node_info�б�����
	m_node_info->data = (void*)create_data_chain(NULL);
	create_and_add_pointer_to_data((chain_node*)m_node_info->data, (void*)node_info->pointer);
	
	chain_node* chain_node_p =  create_malloc_node(m_node_info);//create data chain inside
	//((void*)chain_node_p->node_info);
	
	add_node_to_chain(p_chain, chain_node_p);
	//handle_node_for_retrieve((void*)chain_node_p->node_info);
	PRINT_LOG(LOG_MSG_DEBUG, "%s exit\n", __func__);
	PRINT_LOG(LOG_MSG_DEBUG, "mmmmmmmmmmmmm:---end to handle_malloc\n");
}
//p_chain is the head of malloc chain.
static void handle_free(chain_node* p_chain, node_info_t* node_info){
	PRINT_LOG(LOG_MSG_DEBUG, "%s entry\n", __func__);
	chain_node* p;

	chain_node* parent_ptr = p_chain;
	p = parent_ptr->next;

	int node_count = 0;
	node_info_t* node_in_chain;
	while(p != NULL){
		PRINT_LOG(LOG_MSG_DEBUG, "%s, node_count:=%d\n", __func__, node_count++);
		//handle_node_for_retrieve((void*)p->node_info);
		node_in_chain = (node_info_t*)p->node_info;
		//if pointer in the node_in_chain->data
		if(!pointer_in_chain_and_del(node_info->pointer, node_in_chain->data)){
			
			node_in_chain->count --;
			PRINT_LOG(LOG_MSG_DEBUG, "cust_free,so the count dec,line_num:%d,count:%d\n", node_in_chain->line_num, node_in_chain->count);
			// if the malloc_chain node has no count in node_info,delete the malloc_chain.
			if(node_in_chain->count == 0){
				//delete the node
				PRINT_LOG(LOG_MSG_DEBUG, "begin to free malloc node\n");
				parent_ptr->next = p->next;
				cust_free(p);
				p = NULL;
				
			}
			return;
		}
		parent_ptr = parent_ptr->next;
		p = parent_ptr->next;
	}
	
	
}

void cust_wrap_free(int line_num, void *ptr){
	PRINT_LOG(LOG_MSG_DEBUG, "%s entry\n", __func__);
	node_info_t node_info;
	node_info.pointer = (unsigned long)ptr;
	PRINT_LOG(LOG_MSG_DEBUG, "prepare to cust_free:0x%x\n", ptr);
	handle_free(p_chain, &node_info);
	cust_free(ptr);
	//ptr = NULL;
	retrieve_malloc_chain();
	PRINT_LOG(LOG_MSG_DEBUG, "%s exit\n", __func__);
	return;
}

int __WEAK handle_node_for_retrieve(void* m_node_info)
{
	PRINT_LOG(LOG_MSG_DEBUG, "%s entry\n", __func__);
	node_info_t* node_info = (node_info_t*)m_node_info;
	chain_node* chain_node_p;
	int line_num,count;
	int ret = 0;
	
	if(node_info == NULL){
		return 0;
	}
	
	PRINT_LOG(LOG_MSG_DEBUG, "begin to retrieve---------sub_chain:0x%x------------------\n", node_info->data);
	chain_node_p = (chain_node*)node_info->data;
	chain_node_p = chain_node_p->next;//if node_info is not NULL,node_info->data is not NULL either
	
	count = node_info->count;
	line_num = node_info->line_num;

	PRINT_LOG(LOG_MSG_INFO, "count:%d, [line_num:%d]==>", count, line_num);
	while(chain_node_p != NULL){
		PRINT_LOG(LOG_MSG_INFO, "0x%x,", chain_node_p->node_info);
		chain_node_p = chain_node_p->next;
	}
	PRINT_LOG(LOG_MSG_INFO, "\n");
    PRINT_LOG(LOG_MSG_DEBUG, "end to retrieve++++++++++++++++++++++++++++++\n");


	
	ret = line_num>0?count:0;
	PRINT_LOG(LOG_MSG_DEBUG, "%s exit,ret:%d\n", __func__, ret);
    return ret;
}

void __WEAK PRINT_LOG(int level, const char * cmd, ...){

	return;
}
void* __WEAK PLATFORM_MALLOC(size_t size){
	return NULL;
}

void __WEAK PLATFORM_FREE(void *ptr){
	return;
}

