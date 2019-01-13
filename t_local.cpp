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
	int val = 0;
	std::thread t1(function);
	std::thread t2(function);
	t1.join();
	t2.join();
	return 0;
}