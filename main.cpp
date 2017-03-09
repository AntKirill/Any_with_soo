#include <iostream>
#include <boost/any.hpp>
#include "any_with_soo.h"

using namespace std;

int main() {
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
    swap(anystr, anyint);
    cout << mylib::any_cast<string>(anystr) << endl;
    cout << mylib::any_cast<int>(anyint) << endl;

    anystr.swap(anyint);
    cout << mylib::any_cast<int>(anystr) << endl;
    cout << mylib::any_cast<string>(anyint) << endl;

}