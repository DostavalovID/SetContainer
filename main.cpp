#include <iostream>
#include <string>
#include "Set.h"
#include <ctime>
using namespace std;

int test_passed(string test_name) {
	cout << test_name << " successfully passed :)" << endl;
	return 1;
}
int test_failed(string test_name) {
	cout << test_name << " failed :(" << endl;
	return 0;
}

void first_test_insert() {
	Mem mm(1000);
	Set s(mm);
	int elem6 = 8, elem3 = 7, elem5 = 3, elem9 = 33, elem0 = 4;
	size_t size;

	s.insert(&elem6, sizeof(elem6));
	s.insert(&elem3, sizeof(elem3));
	s.insert(&elem5, sizeof(elem5));
	s.insert(&elem9, sizeof(elem9));
	s.insert(&elem0, sizeof(elem0));
	s.print(s.root());
	Set::SetIterator*it = s.end();
	int* b = static_cast<int*>(it->getElement(size));
	cout << "end = " << *b << " size = " << size << endl;
	
	try {
		it->goToNext();
		b = static_cast<int*>(it->getElement(size));
		cout << "end = " << *b << " size = " << size << endl;
	}
	catch (Container::Error exception) {
		cerr << exception.msg << endl;
	}
	it = s.begin();
	int* a = static_cast<int*>(it->getElement(size));
	cout << "begin = " << *a <<" size = " << size << endl;
	it->goToNext();
	int* c = static_cast<int*>(it->getElement(size));
	cout << "next = " << *c << " size = " << size << endl;

	cout << "Using iterator" << endl;
	for (it = s.begin(); it->hasNext(); it->goToNext()) {
		int* d = static_cast<int*>(it->getElement(size));
		cout << "elem = " << *d << " size = " << size << endl;
	}
	int* d = static_cast<int*>(it->getElement(size));
	cout << "elem = " << *d << " size = " << size << endl;
	cout << endl << endl;
	it = s.find(&elem3, sizeof(elem3));
	d = static_cast<int*>(it->getElement(size));
	cout << "found elem = " << *d << " size = " << size << endl;
	cout << "set size = " << s.size() << endl;
	cout << "before clearing" << endl;
	s.print(s.root());
	cout << "size = " << s.size() << endl;
	//s.clear();
	cout << "after erasing" << endl;
	try {
		it = s.find(&elem3, sizeof(elem3));
		s.remove(it);
		s.print(s.root());
		cout << "I have removed" << endl;
		cout << "size = " << s.size() << endl;
	}
	catch (Container::Error exception) {
		cerr << exception.msg << endl;
	}
	try {
		it = s.find(&elem3, sizeof(elem3));
		d = static_cast<int*>(it->getElement(size));
		cout << "found elem = " << *d << " size = " << size << endl;
	}
	catch (Container::Error exception) {
		it = nullptr;
		cerr << exception.msg << endl;
	}
	try {
		s.remove(it);
	}
	catch (Container::Error exception) {
		cerr << exception.msg << endl;
	}
	s.clear();
	cout << "after clearing" << endl;
	s.print(s.root());
}

void random_insert() {
	//srand(time(0));
	Mem mm(5000);
	Set s(mm);
	int test_massive[1000], mas2[6] = { 6,3,5,3,9,0 };
	for (int i = 0; i < 1000; i++) {
		test_massive[i] = rand();
	}
	for (int i = 0; i < 1000; i++) {
		cout << i + 1 << " = " << &test_massive[i] << endl;
	}
	cout << "after adding to set: " << endl << endl;
	for (int i = 0; i < 1000; i++) {
		s.insert(&test_massive[i], sizeof(int));
	}
	s.print(s.root());
}


int main() {
	first_test_insert();
	//cout << "test 2:" << endl << endl;
	//random_insert();
	return 0;
}