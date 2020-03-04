
#include "chain_common.h"


chain_node* create_signal_chain(void* node_info);
void destroy_chain(chain_node* p_chain);

void add_node_to_chain(chain_node* p_chain, chain_node* p_chain_node);
void del_node_in_chain(chain_node* p_chain, void* expect_node_info, size_t offset0, size_t offset1);

int retrieve_chain(chain_node* p_chain);
int handle_node_for_retrieve(void* node_info);

// for cust_malloc
void cust_free_with_clear(void **ptr);