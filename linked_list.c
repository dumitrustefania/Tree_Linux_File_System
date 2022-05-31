// Copyright 2022 Dumitru Bianca Stefania & Nioata Alexandra 312CA
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"
#include "linked_list.h"

List *ll_create()
{
	List *ll;

	//  allocate memory for a linked list
	ll = malloc(sizeof(*ll));

	//  initialize list parameters
	ll->head = NULL;
	return ll;
}

void ll_add_node(List *list, TreeNode *node)
{
	//  check if parameter list is allocated
	if (!list)
		return;

	ListNode *new_node = malloc(sizeof(ListNode));
	new_node->info = node;
	new_node->next = NULL;

	//  insert the new node in the list in the first position
	if (list->head)
		new_node->next = list->head;
	list->head = new_node;
}

int find_in_list(List *list, char *name, TreeNode **node)
{
	//  check if parameter list is allocated
	if (!list)
		return -1;

	ListNode *curr = list->head;
	int pos = 0;
	while (curr)
	{
		if (!strcmp(curr->info->name, name))
		{
			*node = curr->info;
			return pos;
		}
		curr = curr->next;
		pos++;
	}
	return -1;
}

void ll_remove_nth_node(List *list, unsigned int n)
{
	//  check if parameter list is allocated and if it is not an empty list
	if (!list || !list->head)
		return;

	//  traverse the list until finding the right place to insert the node
	ListNode *prev, *curr;
	curr = list->head;
	prev = NULL;
	while (n > 0)
	{
		prev = curr;
		curr = curr->next;
		--n;
	}

	//  remove the node in the list at desired position
	if (prev == NULL)  //  if desired position = 0
		list->head = curr->next;
	else
		prev->next = curr->next;
}
void ll_free(List *list)
{
	ListNode*curr = list->head;
	while(curr)
	{
		ListNode *tmp = curr->next;
		free(curr);
		curr = tmp;
	}
}
