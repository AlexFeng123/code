#include<iostream>
#include<stdexcept> 
#include<typeinfo>
#include <cxxabi.h>
using namespace std;
struct Mocxxx{};
struct Motxxx{};
int status;
template<typename T,typename isFloat=std::true_type> struct TypeID
{
static int const ID = -1;
};
template<typename T> struct  TypeID<T,typename is_floating_point<T>::type> { static int const ID = 1;};
template<typename T> struct  TypeID<T,typename is_integral<T>::type> { static int const ID = 2;};
struct x{ using type = float;};
struct y{ using type2 =  float;};


struct Counter
{
    void increase() {}   
};

template<typename T>
void inc_counter(T& intType, decay_t<decltype(++intType)>* = nullptr)
{
}
template<typename T>
void inc_counter(T& counterObj,
                decay_t<decltype(counterObj.increase())>* = nullptr)
{
   counterObj.increase(); 
}


template <typename T> void foo(T& c,decay_t<decltype(c.increase())>* = nullptr){};

int main()
{
  Counter x;
  int y;
  inc_counter(x);
  inc_counter(y);
  foo(x);
  return 0;
}
