#include <iostream>
#include <cstring>
#include <cstdlib>

using namespace std;

void unsafe_string_copy() {
    char src[20] = "ThisIsATestString";
    char dest[10];
    strcpy(dest, src);
    cout << "Copied string: " << dest << endl;
}

void use_after_free_example() {
    int* p = new int(42);
    delete p;
    cout << *p << endl;
}

void null_deref() {
    int* p = nullptr;
    cout << *p << endl;
}

void sum_and_overflow() {
    int a = INT_MAX;
    int b = 100;
    int c = a + b;
    cout << "Overflow result: " << c << endl;
}

void divide_checked() {
    int x = 5;
    int y = 0;
    cout << "Divide: " << x / y << endl;
}

void dangling_pointer() {
    int x = 42;
    int* p = &x;
    cout << "Dangling pointer: " << *(p + 1000) << endl;
}

void unsafe_array() {
    char buf[5];
    for (int i = 0; i < 10; ++i) {
        buf[i] = 'A' + i;
    }
    cout << "Buffer content: " << buf << endl;
}

void safe_code() {
    int a = 5;
    int b = 10;
    int c = a * b;
    cout << "Safe multiplication: " << c << endl;

    string s = "Hello";
    cout << s + " world!" << endl;
}

int main() {
    cout << "=== Testfile for CodeQL ===" << endl;

    unsafe_string_copy();
    use_after_free_example();
    null_deref();
    sum_and_overflow();
    divide_checked();
    dangling_pointer();
    unsafe_array();
    safe_code();

    return 0;
}
