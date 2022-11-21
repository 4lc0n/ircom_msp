#include "ringbuffer.hpp"
// #include <iostream>

// using namespace std;

char buff[20];

int main(){
	// cout << "first line" << endl;
	
	Ringbuffer<char> ringbuffer = Ringbuffer<char>(buff, 20);

	// cout << "empty: " << ringbuffer.is_empty() << endl;

	ringbuffer.put('c');
	ringbuffer.put('d');

	// cout << "in storage: " << ringbuffer.in_storage() << ", free: " << ringbuffer.storage_available() << endl;
	// cout << "empty: " << ringbuffer.is_empty() << ", full: " << ringbuffer.is_full() << endl;



	while(!ringbuffer.is_empty()){
		ringbuffer.pop();
		// cout << ringbuffer.pop() << endl;
	}
	return 0;
}
