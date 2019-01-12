/*****************
* Epoch-Based Reclamation
* Implementation from PPpPP 18 : Interval-based memory reclamation
*****************/
#include<iostream>
#include<vector>

template<typename T>
int64 epoch;
int64 reservations[thread_cnt];
therad_local int counter;
thread_local std::vector<int> retired;
int epoch_freq=1; // freq. of increasing epoch
int empty_freq=1; // freq. of reclaiming retired

class block {
	//block header : retire_epoch, retire_next
	// Node(eg: list node)
	double data;
	block* next;
	T retire_epoch;
}

//Start of an operation
void start_op(){
	reservations[thread_id()] = epoch;
}

//End of an operation
void enq_op(){
	reservations[thread_id()] = MAX_INT64;
}

//public interface
//block will not be used by any future operation
//?? paper says (block*ptr) maybe lecture said (block&ptr)
void retire (block & ptr){
	retired.append(ptr);
	ptr.retire_epoch = epoch;
	counter++;
	if (counter % epoch_freq == 0)
		fetch_and_add(epoch,1);
	if (retired.cnt % empty_freq == 0)
		empty();
}

//private function
void empty(){
	max_safe_epoch = reservations.min();
	for block belongs to retired
	//all blocks retired IN or AFTER max_save_epoch will be protected	
		if (block.retire_epoch < max_safe_epoch){
			//deallocate block
			free(block)
		}
}


/*linked list;s remove method
remove()
start_op();
//operations to the list
//when deleting in place of delete node
retire();
end_ops();

//search/contains also encap in start_op and end_op
search () {
	start_op
	...

	end_op()
}
therefore we need efficient implementation for start_op and end_op
*/

// set-ups
memory_manager<node> mm;
// 3 nodes with values 0, 2 and 4
//write node class 
node n0(0), n1(2), n2(4);
n0.next = &n1;
n1.next = &n2;
n2.next = NULL;
node* head = &n;

// 2 worker threads
int thread_A(){
	// Thread A tries to read n1
	mm.start_op();
	node* target = &n1;
	node* p1 = mm.read(&target);
	if (p1 != NULL){ // n1 still exists
		int ret = p1->val; mm.end_op();
		return ret;
	}
	else {
		mm.end_op();
		return NULL;
	}
}
void thread_B(){
	// Thread B tries to update n1
	while(true){
		mm.start_op();
		node* new_n1 = mm.alloc(sizeof(node));
		new_n1.val = 3;
		new_n1.next = &n2;
		node* target = &n0;
		node* p0 = mm.read(&target);
		if (p0 != NULL){
			if (mm.compare_exchange_weak(&p0->next, &n1,std::memory_order_release, std::memory_order_relaxed))
		}
	}
}
