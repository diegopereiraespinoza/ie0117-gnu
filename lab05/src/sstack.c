#include "sstack.h"  // stack with a singly list

// script to work with singly lists
// all functions return the head
// except those to find the tail
// s_peek | s_top

/*
// function definitions
*/
// return: head
// add a node to the end of list
SNode* s_push(SNode *head, SNode *node) {
	return (s_insert_at_tail(head, node));
}
// return: tail
// get the last node on the list
SNode* s_peek(SNode *head) {
	return (s_find_tail(head));
}
// return: tail
// get the last node on the list
SNode* s_top(SNode *head) {
	return (s_find_tail(head));
}
// return: head
// delete the last node from the list
SNode* s_pop(SNode *head) {
	SNode* tail = (NULL);
	tail = s_find_tail((head));
	return (s_delete(head, tail));
}
// return true if the list is empty
bool s_isempty(SNode *head) {
	if (!(head)) {
		return (true);
	}
	return (false);
}
