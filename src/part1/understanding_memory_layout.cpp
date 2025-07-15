#include <iostream>
#include <string>
#include <cstddef>

struct A {
  std::string name;
  int age;
};

int main() {
  A a;
  a.name = "Bruce";
  a.age = 31;
  
  std::cout << "Size of std::string: " << sizeof(std::string) << " bytes\n";
  std::cout << "Size of int: " << sizeof(int) << " bytes\n";
  std::cout << "Size of struct A: " << sizeof(A) << " bytes\n";
  std::cout << "Offset of name: " << offsetof(A, name) << " bytes\n";
  std::cout << "Offset of age: " << offsetof(A, age) << " bytes\n";
  
  char* p = reinterpret_cast<char*>(&a);
  std::cout << "Address of a: " << static_cast<void*>(&a) << "\n";
  std::cout << "Address of a.name: " << static_cast<void*>(&a.name) << "\n";
  std::cout << "Address of a.age: " << static_cast<void*>(&a.age) << "\n";
  
  std::cout << "p (start): " << static_cast<void*>(p) << "\n";
  std::cout << "p + 1: " << static_cast<void*>(p + 1) << "\n";
  std::cout << "p + sizeof(std::string): " << static_cast<void*>(p + sizeof(std::string)) << "\n";
  
  return 0;
}
