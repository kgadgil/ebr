/*******
Thread-local storage class specifier example
*******/
#include <iostream>
#include <thread>

void function(int cnt){
	//thread_local int cnt=0;
	cnt++;
	std::cout << "cnt " << cnt << std::endl;
}

int main(){
	int thread_cnt = 6;
	int start=0;
	std::thread threadArr[thread_cnt];
	for (int i = 0; i < thread_cnt; i++){
		threadArr[i] = std::thread(function, start);
	}
	for (int i = 0; i < thread_cnt; i++){
		threadArr[i].join();
	}
	std::cout << "value " << start << std::endl;
	return 0;
}