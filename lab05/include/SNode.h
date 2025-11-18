#ifndef S_NODE_H
#define S_NODE_H

// data type for a singly node
// typedef struct SNode SNode;
typedef struct SNode {
	int index;
	//char locale[5];
	struct SNode* next;
} SNode;

#endif // S_NODE_H
