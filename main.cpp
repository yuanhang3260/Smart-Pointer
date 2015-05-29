#include <stdlib.h>
#include <stdio.h>
#include "iostream"
#include "shared_ptr.h"
#include "weak_ptr.h"
#include "unique_ptr.h"

using namespace std;

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
  sp2.reset(new int(2));
  printf("sp2 ref_num = %d\n", sp2.ref_num());
  printf("sp1 ref_num = %d\n", sp1.ref_num());
  sp1 = sp2;
  sp3 = sp1;
  printf("sp1 ref_num = %d\n", sp1.ref_num());
  printf("sp2 ref_num = %d\n", sp2.ref_num());
  printf("sp3 ref_num = %d\n", sp3.ref_num());

  printf("init sp5\n");
  shared_ptr<int> sp5(std::move(sp4));
  printf("sp5 ref_num = %d\n", sp5.ref_num());

  printf("[end testing shared_ptr]\n");
}

weak_ptr<int> gw;

void f() {
  if (auto spt = gw.lock()) { // Has to be copied into a shared_ptr before usage
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

  printf("[end testing weak_ptr]\n");
}

void test_unique_ptr() {
  printf("\n==== testing unique_ptr ====\n");
  unique_ptr<string> up1(new string("hello"));
  up1.release();
  up1.reset(new string("hello2"));

  unique_ptr<string> up2(new string("world"));
  unique_ptr<string> up3(std::move(up2));
  up1.reset(new string("snoopy"));
  up1 = std::move(up3);
  up1.release();

  printf("[end testing unique_ptr]\n");
}

int main(int argc, char** argv) {
  test_shared_ptr();
  test_weak_ptr();
  test_unique_ptr();

  printf("\n");
  return 0; 
}
