#include <iostream>
#include "lru_stack.h"

using namespace std;

int main(int argc, char ** argv){
	
	int temp;
	
	LRU_stack direct_mapped;
	LRU_stack fourway(4);
	
	cout << "Testing direct mapped LRU" << endl;
	direct_mapped.update_stack_on_hit(0);
	direct_mapped.update_stack_on_hit(1);
	direct_mapped.update_stack_on_miss();
	direct_mapped.print_stack();
	cin >> temp;
	
	cout << "Testing four-way set associative LRU hits" << endl;
	fourway.update_stack_on_hit(3);
	fourway.update_stack_on_hit(0);
	fourway.update_stack_on_hit(2);
	fourway.update_stack_on_hit(3);
	fourway.update_stack_on_hit(0);
	fourway.print_stack();
	cin >> temp;
	
	cout << "Testing four-way set associative LRU misses" << endl;
	cout << "Way number: " << fourway.update_stack_on_miss() << endl;
	cout << "Way number: " << fourway.update_stack_on_miss() << endl;
	cout << "Stack:" << endl;
	fourway.print_stack();
	cin >> temp;
	
	return 0;
}
