/*****************
* Epoch-Based Reclamation
* Implementation from PPpPP 18 : Interval-based memory reclamation
*****************/
#include<iostream>
#include<vector>
#include <thread>

template<typename T>
struct Node {
	int retire_epoch;
	T data;
	Node* next;
}


template<typename T>
class memory_manager(){
	int64 epoch;
	int64 reservations;
	thread_local int counter;
	thread_local std::vector<int> retired;
	int epoch_freq; // freq. of increasing epoch
	int empty_freq; // freq. of reclaiming retired
private:
	void empty(){
		//max_safe_epoch = reservations.min();
		std::vector<int>::iterator max_safe_epoch = std::min_element(std::begin(retired), std::end(retired));
		for(auto block : retired){
				//all blocks retired IN or AFTER max_save_epoch will be protected	
			if (block.retire_epoch < max_safe_epoch){
			//deallocate block
				free(block)
			}
		}

	}

public:
	memory_manager(int thread_cnt){
		epoch=0;
		reservations[thread_cnt];
		counter = 0;
		epoch_freq = 1;
		empty_freq = 1;
	}

	void start_op(){
		//Start of an operation
		reservations[std::this_thread::get_id()] = epoch;
	}

	void enq_op(){
		//End of an operation
		reservations[std::this_thread::get_id()] = MAX_INT64;
	}

	void retire (block & ptr){
		//block will not be used by any future operation
		//?? paper says (block*ptr) maybe lecture said (block&ptr)
		retired.push_back(ptr);
		ptr.retire_epoch = epoch;
		counter++;
		if (counter % epoch_freq == 0)
			fetch_and_add(epoch,1);
		if (retired.cnt % empty_freq == 0)
			empty();
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

int main() {
	block blk;
	// set-ups
	memory_manager<Node> mm;
	// 3 nodes with values 0, 2 and 4
	node n0(0), n1(2), n2(4);
	n0.next = &n1;
	n1.next = &n2;
	n2.next = NULL;
	node* head = &n;

	std::thread tA([&]{
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
	});

	std::thread tB([&]{
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
	});

	return 0;	
}