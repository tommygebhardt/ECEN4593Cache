#include <cstring>
#include <iostream>
#include "lru_stack.h"

using namespace std;

#define LRU_DEBUG 0

/* LRU stack default constructor
 * 
 * Constructs a 1-way LRU stack
 */
LRU_stack::LRU_stack()
{
	ways = 1;
	// Create the single node
	head = new llnode;
	head->way_number = 0;
	head->prev = NULL;
	head->next = NULL;
	tail = head;
}

/* LRU stack constructor
 * 
 * Constructs an LRU stack that is as deep as the number of ways in the
 * set.  The LRU stack is a doubly-linked list.
 */
LRU_stack::LRU_stack(unsigned int assoc)
{
	llnode * lead_p;
	llnode * lag_p;

	ways = assoc;
	head = new llnode; // Create the head node
	head->way_number = 0;
	head->prev = NULL;
	lead_p = head;
	lag_p = head;

	// Create as many nodes as there are ways
	for(unsigned int i = 1; i < ways; ++i)
	{
		lead_p->next = new llnode;
		lead_p = lead_p->next;
		lead_p->prev = lag_p;
		lead_p->way_number = i;
		lag_p = lead_p;
	}

	// Terminate the linked list
	lead_p->next = NULL;
	tail = lead_p;
}

/* LRU stack destructor
 * 
 * Iterates through the doubly-linked list and frees all memory
 */
LRU_stack::~LRU_stack()
{
	llnode * node_p = head;
	// Free all memory
	for(unsigned int i = 0; i < ways; ++i)
	{
		head = head->next;
		delete node_p;
		node_p = head;
	}

	// Make sure the LRU stack is not used again
	head = NULL;
	tail = NULL;
	ways = 0;
}

/* LRU stack update_stack_on_hit
 * 
 * Performs a linear search for the newly referenced block (in terms of
 * the way number).  Since it was a hit, we simply need to update the
 * stack.
 */
void LRU_stack::update_stack_on_hit(unsigned long long ref_way_num)
{
#if LRU_DEBUG
	cout << "Updating stack on hit" << endl;
#endif
	llnode * node_p = head;

	if(ref_way_num >= ways)
	{
#if LRU_DEBUG
		cout << "ref_way_num >= ways" << endl;
#endif
		return;
	}
	if(head->way_number == ref_way_num)
	{
#if LRU_DEBUG
		cout << "head->way_number == ref_way_num" << endl;
#endif
		return;
	}

	while(node_p != tail)
	{
		node_p = node_p->next;
		if(node_p->way_number == ref_way_num)
		{
#if LRU_DEBUG
			cout << "Found ref_way_num" << endl;
#endif
			node_p->prev->next = node_p->next;
			if(node_p == tail)
			{
#if LRU_DEBUG
				cout << "ref_way_num was at the tail" << endl;
#endif
				tail = node_p->prev;
			}
			else
			{
#if LRU_DEBUG
				cout << "ref_way_num was NOT at the tail" << endl;
#endif
				node_p->next->prev = node_p->prev;
			}
			node_p->next = head;
			node_p->prev = NULL;
			head->prev = node_p;
			head = node_p;
#if LRU_DEBUG
			cout << "Finished updating LRU stack. Exiting..." << endl;
#endif
			return;
		}
	}

#if LRU_DEBUG
	cout << "Impossible! ref_way_num not in LRU stack" << endl;
#endif
	return;
}

/* LRU stack update_stack_on_miss
 * 
 * Moves the least recently used node from the tail of the linked list
 * to the head of the list.  Since it was a miss, we need to both update
 * the stack and return the way number of the least recently used block.
 */
unsigned long long LRU_stack::update_stack_on_miss()
{
	llnode * node_p = tail;

	if(head == tail)
	{
		return head->way_number;
	}

	node_p->prev->next = NULL;
	tail = node_p->prev;
	head->prev = node_p;
	node_p->next = head;
	node_p->prev = NULL;
	head = node_p;

	return head->way_number;
}

/* LRU stack print_stack
 * 
 * Iterates through the stack from the most recently used to the least
 * recently used, and prints the way numbers
 */
void LRU_stack::print_stack()
{
	llnode * node_p = head;

	for(unsigned int i = 0; i < ways; ++i)
	{
		cout << node_p->way_number << endl;
		node_p = node_p->next;
	}

	return;
}


