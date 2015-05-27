#include <stdlib.h>
#include <stdio.h>
#include "iostream"
#include "smart_ptr.h"
#include "weak_ptr.h"

using namespace std;

int main(int argc, char** argv) {
  printf("init sp1\n");
  smart_ptr<int> sp1(new int(5));
  printf("sp1 ref_num = %d\n", sp1.ref_num());
  printf("init sp2\n");
  smart_ptr<int> sp2(sp1);
  printf("sp1 ref_num = %d\n", sp1.ref_num());
  printf("init sp3\n");
  smart_ptr<int> sp3 = sp2;
  printf("sp1 ref_num = %d\n", sp1.ref_num());
  printf("init sp4\n");
  smart_ptr<int> sp4 = sp2;
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
  smart_ptr<int> sp5(std::move(sp4));
  printf("sp5 ref_num = %d\n", sp5.ref_num());

  printf("\n");
  return 0; 
}
