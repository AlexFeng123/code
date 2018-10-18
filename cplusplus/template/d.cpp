#include<iostream>
using namespace std;
template<int i> struct D{D(void*);operator int();};

template<int p,int i> struct is_prime{
 enum{ prim = (p%i) && is_prime<(i>2?p:0), i-1>::prim};
};
template<> struct is_prime<0,0>{ enum { prim = 1};};
template<> struct is_prime<0,1>{ enum { prim = 1};};

template<int i> struct Prime_print {
 Prime_print<i-1> a;
 enum { prim = is_prime<i,i-1>::prim };
 void f() { D<i> d = prim?1:0; a.f();}
};

template<> struct Prime_print<2>{
    enum { prim = 1};
    void f() {D<2> d = prim?1:0;}
};

int main()
{
  Prime_print<10> a;
  a.f();  
  return 0;
}
