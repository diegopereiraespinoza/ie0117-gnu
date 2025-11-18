#ifndef S_STACK_H
#define S_STACK_H

#include "slist.h"   // singly list utils

// script to work with singly lists
// all functions return the head
// except those to find the tail
// s_peek | s_top

/*
// function declarations
*/
// return: head
// add a node to the end of list
SNode* s_push(SNode *head, SNode *node);
// return: tail
// get the last node on the list
SNode* s_peek(SNode *head);
// return: tail
// get the last node on the list
SNode* s_top(SNode *head);
// return: head
// delete the last node from the list
SNode* s_pop(SNode *head);
// return true if the list is empty
bool s_isempty(SNode *head);

#endif // S_STACK_H
