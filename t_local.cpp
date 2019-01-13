/*******
Thread-local storage class specifier example
*******/
#include <iostream>
#include <thread>

int main(){
	int thread_cnt = 6;
	int x = 0;
	std::thread threadArr[thread_cnt];
	for (int i = 0; i < thread_cnt; i++){
		threadArr[i] = std::thread([&]{
			x++;
			std::cout << "thread id "<< std::this_thread::get_id() << std::endl;
			std::cout << "inc " << x << std::endl;
		});
	}
	for (int i = 0; i < thread_cnt; i++){
		threadArr[i].join();
	}
	std::cout << "outside " << x << std::endl;
	return 0;
}