// Copyright 2022 Dumitru Bianca Stefania & Nioata Alexandra 312CA
#ifndef LINKED_LIST_H_
#define LINKED_LIST_H_

//  allocate memory for a new linked list
List * ll_create();

//  add a new node at given position in a list
void ll_add_node(List *list, TreeNode *node);

//  remove a node from given position from a list
void ll_remove_nth_node(List *list, unsigned int n);

//  find position of an element in a list and save in "node"
//  parameter a pointer to it
int find_in_list(List *list, char *name, TreeNode **node);

//  free memory for a list
void ll_free(List *list);

#endif  // LINKED_LIST_H_



