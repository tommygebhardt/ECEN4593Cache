#ifndef LRU_STACK_H
#define LRU_STACK_H

typedef struct _llnode
{
	// The way number in the set (indexed from 0)
	unsigned long long way_number;
	struct _llnode * prev;
	struct _llnode * next;
} llnode;

class LRU_stack
{
private:
	llnode * head; // Most recently used
	llnode * tail; // Least recently used
	unsigned int ways;

public:
	LRU_stack();
	LRU_stack(unsigned int assoc);	
	~LRU_stack();
	void update_stack_on_hit(unsigned long long ref_way_num);
	unsigned long long update_stack_on_miss();
	void print_stack();
};

#endif // LRU_STACK_H
