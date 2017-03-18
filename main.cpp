#include <iostream>
#include "any_with_soo.h"
#include <cstdint>
#include <memory>
#include <vector>
#include <boost/any.hpp>

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
	mylib::swap(anyint, anystr);
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

using namespace mylib;
void test5() {
	any a = 5;
	{
		any b = 10;
		for (int i = 101; i > 1; i--) {
			swap(a, b);
		}
	}
	cout << any_cast<int>(a) << endl;
}

// void test6() {
// 	unique_ptr<int> a = make_unique<int>(5);
// 	boost::any aholder = a;
// 	{
// 		unique_ptr<int> b = make_unique<int>(6);
// 		boost::any bholder = b;
// 		boost::swap(aholder,bholder);
// 	}
// 	cout << boost::any_cast<unique_ptr<int>	>(aholder) << endl;
// }

void test7() {
	any a = 54321;
	a = a;
	cout << any_cast<int>(a) << endl;
	swap(a, a);
	cout << any_cast<int>(a) << endl;
}

void test8() {
	unique_ptr<any> pa = make_unique<any>(5);
	any b = move(*pa);
	pa.reset();
	cout << any_cast<int>(b) << endl;
}

int main() {
	test0();
	test1();
	test2();
	test3();
    test4();
    test5();
    test7();
    test8();
}