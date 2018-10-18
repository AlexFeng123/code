#include<iostream>
using namespace std;
template<typename T>
class aTMP{
    public: using reType = const T;
};

void f() { cout << "global f() \n";}

template<typename T>
class Base{
    public:
    template< int N = 99>
    void f(){ cout << "member f(): " << N << endl;}
};

template<typename T>
class Derived : public Base<T>{
    public:
   typename   T::reType m;
   Derived(typename T::reType a):m(a){}
   void df1() { f();}
   void df2() { this->template f();}
   void df3() { Base<T>::template f<22>();}
   void df4() { ::f();}
};

int main()
{
    Derived<aTMP<int>> a(10);
    a.df1();
    a.df2();
    a.df3();
    a.df4();
    return 0;
}

