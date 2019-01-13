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
	std::atomic<int> epoch;
	int64 reservations;
	thread_local int counter;
	thread_local std::vector<Node> retired;
	int epoch_freq; // freq. of increasing epoch
	int empty_freq; // freq. of reclaiming retired
private:
	void empty(){
		//max_safe_epoch = reservations.min();
		std::vector<int>::iterator max_safe_epoch = std::min_element(std::begin(retired), std::end(retired));
		for(auto node : retired){
				//all blocks retired IN or AFTER max_save_epoch will be protected	
			if (node.retire_epoch < max_safe_epoch){
			//deallocate block
				delete node;
			}
		}

	}

public:
	memory_manager(int thread_cnt){
		epoch.store(0);
		reservations[thread_cnt];
		counter = 0;
		epoch_freq = 1;
		empty_freq = 1;
	}

	//TODO:allocate method
	//TODO:unreserve method

	Node* read(Node** ptraddr){
		return *ptraddr
	}

	void write(Node** ptraddr, Node* ptr){
		*ptraddr = ptr;
	}

	bool CAS(Node** ptraddr, Node* old_value, Node* new_value){
		return compare_exchange_weak(ptraddr, old_value, new_value, std::memory_order_release, std::memory_order_relaxed)
	}

	void start_op (){
		//Start of an operation
		reservations[std::this_thread::get_id()] = epoch.load();
	}

	void end_op (){
		//End of an operation
		reservations[std::this_thread::get_id()] = MAX_INT64;
	}

	void retire (Node & ptr){
		//block will not be used by any future operation
		//?? paper says (block*ptr) maybe lecture said (block&ptr)
		retired.push_back(ptr);
		ptr.retire_epoch = epoch.load();
		counter++;
		if (counter % epoch_freq == 0)
			fetch_and_add(epoch,1);
		if (retired.cnt % empty_freq == 0)
			retired.clear();
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
	// set-ups
	memory_manager<Node> mm;
	// 3 nodes with values 0, 2 and 4
	Node n0, n1, n2;
	n0->data = 0;
	n1->data = 2;
	n2->data = 4;

	n0->next = &n1;
	n1->next = &n2;
	n2->next = NULL;
	Node* head = &n;

	std::thread tA([&]{
    	// Thread A tries to read n1
		mm.start_op();
		Node* target = &n1;
		Node* p1 = mm.read(&target);
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
			Node* new_n1 = mm.alloc(sizeof(node));
			new_n1.val = 3;
			new_n1.next = &n2;
			Node* target = &n0;
			Node* p0 = mm.read(&target);
			if (p0 != NULL){
				if (mm.CAS(&p0->next, &n1, new_n1)){
					mm.retire(&n1);
					mm.end_op();
					return 0;
				}
			}
		}
	});

	return 0;	
}