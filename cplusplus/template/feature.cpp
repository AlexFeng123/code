#include<iostream>
using namespace std;
struct tag0{};
struct tag1{ };
struct tag2{};
template<class T>
struct x
{
    using tag = tag0;
};
template<>
struct x<tag1>
{
    using tag = tag1;
};
template<>
struct x<tag2>
{
  using tag = tag2;
};

template<typename T>
void function(T &a)
{ 
    using tag = typename T::tag; 
    functionImpl(a,tag());
}

template<typename T>
void functionImpl(T &a, tag1)
{
    cout<<"x"<<endl;
}

template<typename T>
void functionImpl(T &a, tag2)
{
    cout<<"Y"<<endl;
}

struct X
{
   void print(){ cout<<"x "<<endl;}    
};

template<typename T>
struct y: T
{
   void print10()
   {
       for(int i= 0; i < 10; ++i) this->print();
   }


};

template<class Derived>
class base
{
public:
   void print()
   {
       static_cast<Derived*>(this)->print();
   }
};

class Derived: public base<Derived>
{
public:
    void print(){cout << "Derived"<<endl;}
};

class Derived1: public base<Derived1>
{
public:
    void print(){cout << "Derived1"<<endl;}
};

struct Alex{};
template<typename T>
struct xClass
{
   using type = T;
};

template<typename T,class U>
void function(T &x, U){cout << "T"<<endl;}

template<typename T>
void function(T &x, Alex){cout << "Alex" << endl;}

template<typename T>
void f(T &x)
{
    function(x, typename xClass<T>::type());
}

int main()
{
     int intObj;
     Alex AlexObj;
     f(intObj);
     f(AlexObj);     
    return 0;
}
