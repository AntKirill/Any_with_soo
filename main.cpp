#include <iostream>
#include "any_with_soo.h"
#include <cstdint>
#include <vector>

using namespace std;

void test1() {
	mylib::any a = 1;
	a = string("Privet");
	cout << mylib::any_cast<short>(mylib::any(short(2))) << endl;
	cout << mylib::any_cast<string>(a) << endl;

	const mylib::any a1 = 5;
	cout << mylib::any_cast<int>(a1) << endl;

	mylib::any b1 = 123;
	int *ptr_value = mylib::any_cast<int>(&b1);
	cout << *ptr_value << endl;

	const mylib::any b2 = 223;
	const int *ptr_value2 = mylib::any_cast<int>(&b2);
	cout << *ptr_value2 << endl;

	mylib::any anystr = 2;
	mylib::any anyint = string("Hello, World!");
	mylib::swap(anystr, anyint);
	cout << mylib::any_cast<string>(anystr) << endl;
	cout << mylib::any_cast<int>(anyint) << endl;

	anystr.swap(anyint);
	cout << mylib::any_cast<int>(anystr) << endl;
	cout << mylib::any_cast<string>(anyint) << endl;
}

void test2() {
	vector<int> v;
	for (int i = 0;i < 100; i++) v.push_back(i);
	mylib::any a = v;
	
	vector<int> v1 = mylib::any_cast<vector<int> >(a); 
	 for (size_t i = 0; i < v1.size(); i++) {
	 	cout << v[i] << " ";
	 }
	 cout << endl;
}

void test3() {
	int a = 5;
	int &b = a;
	mylib::any t = b;
	a++;
	b++;
	cout << mylib::any_cast<int>(t) << endl;
}

void test4() {
	mylib::any a = 5;
	mylib::any b = string("Privet");
	a = b;
	cout << mylib::any_cast<string>(a) << endl;
	cout << mylib::any_cast<string>(b) << endl;
}

struct local {
	void *mas[3];
};

void test0() {
   
	local aa;
    mylib::any a = aa;

//    cout << mylib::any_cast<int>(a) << endl;
}

int main() {
    test0();
	test1();
	test2();
	test3();
    test4();
}