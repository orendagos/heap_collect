#ifndef __SIGNAL_CHAIN_H__
#define __SIGNAL_CHAIN_H__
#include "chain_common.h"
#include "cust_malloc.h"


chain_node* create_malloc_node(void* node_info);
void destroy_malloc_chain(chain_node* p_chain);

void add_node_to_chain(chain_node* p_chain, chain_node* p_chain_node);
//void del_node_in_chain(chain_node* p_chain, void* expect_node_info, size_t offset0, size_t offset1);
void create_and_add_pointer_to_data(chain_node* chain_head,void* pointer);
chain_node* create_data_chain(void* pointer);

int retrieve_chain(chain_node* p_chain);
int handle_node_for_retrieve(void* node_info);

#endif // __SIGNAL_CHAIN_H__
