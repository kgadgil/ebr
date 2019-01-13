/*******
Thread-local storage class specifier example
*******/
#include <iostream>
#include <thread>

void function(){
	thread_local int cnt=0;
	cnt++;
	std::cout << "cnt " << cnt << std::endl;
}

int main(){
	int thread_cnt = 6;
	std::thread threadArr[thread_cnt];
	for (int i = 0; i < thread_cnt; i++){
		threadArr[i] = std::thread(function);
	}
	for (int i = 0; i < thread_cnt; i++){
		threadArr[i].join();
	}
	return 0;
}