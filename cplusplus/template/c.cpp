#include<iostream>
using namespace std;

template<typename T, int i = 1>
class someComputing{
public:
    using retType = volatile T*;
    enum { ret = i + someComputing<T, i-1>::ret};
    static void f() {cout << " someCompluting: i=" << i << endl;}
};

template<typename T>
class someComputing<T,0>{
    public:
        enum { ret = 0};
};

template<typename T>
class codeComputing{
    public:
        static void f(){ T::f();}
};

int main()
{
    someComputing<int>::retType a = 0;
    cout << sizeof(a)<<endl;
    cout << someComputing<int ,500>::ret << endl;
    codeComputing<someComputing<int,99>>::f(); 
    return 0;
}

