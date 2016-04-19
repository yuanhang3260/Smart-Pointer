#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "shared_ptr.h"
#include "weak_ptr.h"
#include "unique_ptr.h"

using namespace std;

struct MyDeleter {
  template<typename T>
  void operator()(T* p) {
    printf("MyDeleter\n");
    delete p;
  }
};

class MyClass {
 public:
  MyClass(int v) : value_(v) {}
  void foo() { printf("foo\n"); }
  int value() const { return value_; }
 
 private:
  int value_ = 0;
};

std::ostream& operator<<(std::ostream &out, const MyClass &obj) {
  out << "MyClass (value = " << obj.value() << ")";
  return out;
}

void test_shared_ptr() {
  printf("\n==== testing shared_ptr ====\n");
  printf("init sp1\n");
  shared_ptr<int> sp1(new int(5));
  printf("sp1 ref_num = %d\n", sp1.ref_num());
  printf("init sp2\n");
  shared_ptr<int> sp2(sp1);
  printf("sp1 ref_num = %d\n", sp1.ref_num());
  printf("init sp3\n");
  shared_ptr<int> sp3 = sp2;
  printf("sp1 ref_num = %d\n", sp1.ref_num());
  printf("init sp4\n");
  shared_ptr<int> sp4 = sp2;
  printf("sp1 ref_num = %d\n", sp1.ref_num());

  printf("\nreset sp2\n");
  sp2.reset(new int(2), MyDeleter());
  printf("sp2 ref_num = %d\n", sp2.ref_num());
  printf("sp1 ref_num = %d\n", sp1.ref_num());
  sp1 = sp2;
  printf("sp2 ref_num = %d\n", sp2.ref_num());
  sp3 = sp1;
  printf("sp1 ref_num = %d\n", sp1.ref_num());
  printf("sp2 ref_num = %d\n", sp2.ref_num());
  printf("sp3 ref_num = %d\n", sp3.ref_num());

  printf("sp4 ref_num = %d\n", sp4.ref_num());
  printf("init sp5\n");
  shared_ptr<int> sp5(std::move(sp4));
  printf("sp5 ref_num = %d\n", sp5.ref_num());

  sp5.reset();
  shared_ptr<int> sp6(sp5);

  shared_ptr<MyClass> sp7(new MyClass(5));
  printf("sp7->value() = %d\n", sp7->value());
  printf("(*sp7).foo(): ");
  (*sp7).foo();

  printf("[end testing shared_ptr]\n");
}

weak_ptr<int> gw;

void f() {
  // Has to be copied into a shared_ptr before usage.
  if (auto spt = gw.lock()) {
    cout << *spt << "\n";
  }
  else {
    cout << "gw is expired\n";
  }
}

void test_weak_ptr() {
  printf("\n==== testing weak_ptr ====\n");
  {
    shared_ptr<int> sp(new int(42));
    gw = sp;
    f();
  }
  f();

  shared_ptr<char> sp(new char('x'));
  weak_ptr<char> wp(sp);
  //auto sp2 = wp.lock();
  sp.reset(new char('y'));
  auto sp3 = wp.lock();
  if (!sp3) {
    printf("wp expired\n");
  }
  else {
    printf("*sp3 = %c\n", *sp3);
  }

  printf("[end testing weak_ptr]\n");
}

void test_unique_ptr() {
  printf("\n==== testing unique_ptr ====\n");
  unique_ptr<string> up1(new string("hello"));
  auto p1 = up1.release();
  printf("*p1 = %s\n", p1->c_str());
  delete p1;
  up1.reset(new string("hello2"));

  unique_ptr<string> up2(new string("world"));
  unique_ptr<string> up3(std::move(up2));
  up1 = std::move(up3);
  p1 = up1.release();
  printf("*p1 = %s\n", p1->c_str());
  delete p1;

  up1.reset(new string("snoopy"), MyDeleter());

  printf("[end testing unique_ptr]\n");
}

int main(int argc, char** argv) {
  test_unique_ptr();
  test_shared_ptr();
  test_weak_ptr();

  printf("\n");
  return 0; 
}
