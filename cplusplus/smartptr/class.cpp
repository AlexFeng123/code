#include<iostream>
#include<memory>
#include<typeinfo>
using namespace std;
class base
{
    public:
    virtual void a(){ cout << "a "<<endl;}
    virtual void b(){ cout << "b" << endl;}

    long long  m1;
};
template<typename T2, typename T1>
T2 union_cast(T1 x)
{
    union {
     T1 s;
     T2 d;
    }u;
    u.s = x;
    return u.d;
}

int main()
{
  using f = void(*)(void);
  base b1;
  cout <<"base address:"<< &b1 << " size:"<<typeid(decltype(&b1)).name()<<endl;
  auto vbptr =reinterpret_cast<intptr_t**>(&b1);
  cout <<"vptr address:" << hex<< reinterpret_cast<intptr_t>(vbptr) <<endl;
  cout <<"m1  address:" << hex <<  reinterpret_cast<intptr_t>(&b1.m1) <<endl;
  
  void *first = union_cast<void*>(&base::a);
  void *second = union_cast<void*>(&base::b);

  cout <<"first func address:" << hex << reinterpret_cast<intptr_t>(first) <<endl;
  cout <<"second func address:" << hex << reinterpret_cast<intptr_t>(second) <<endl;
  cout <<"first func address:" << hex << reinterpret_cast<intptr_t>(vbptr[0][0]) <<endl;
  f func = (f)vbptr[0][0];
  func();  
  func = reinterpret_cast<f>(vbptr[0][1]);
  func(); 
   
  func = (f)first;
  func();
  return 0;
}
