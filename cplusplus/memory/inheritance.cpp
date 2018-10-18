#include<iostream>
using namespace std;
class A {
    public:
  int a;
  virtual void x() {}
};
 
struct B : A
{
    void x(){} 
};

int main()
{
    A *a= new B;
    return 0;
}
