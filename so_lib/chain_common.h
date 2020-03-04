#ifndef __CHAIN_COMMON_H__
#define __CHAIN_COMMON_H__

#include <stdlib.h>

#define __WEAK __attribute__((weak))
typedef struct single_chain_node chain_node;

typedef struct node_info{
	int id;
	//int node_info_use_count;
	int count;
	void* data;//pointer
	int line_num;// for match
	unsigned int pointer; // for match
	int unused;
	int unused1;
}node_info_t;
typedef struct single_chain_node{
	//chain_node* pre;
	chain_node* next;
	void* node_info;
}single_chain_node_t;

#endif /*__CHAIN_COMMON_H__*/