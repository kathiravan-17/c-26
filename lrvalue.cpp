#include <iostream>

int main() {
    int x = 42;

    // lvalue — has name, has address, lives in scope
    std::cout << &x << "\n";    // ✅ has address

    // rvalue — no name, no address, dies immediately
    // std::cout << &42 << "\n"; // ❌ compile error

    // ++x = lvalue — returns x itself
    int& ref1 = ++x;            // ✅ can take reference

    // x++ = rvalue — returns temporary copy
    // int& ref2 = x++;          // ❌ compile error

    std::cout << "x = " << x << "\n";
}
