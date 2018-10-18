#include<iostream>
#include <unistd.h> 
using namespace std;
struct X 
{
    X(){cout << "xxX" <<endl;}
    int a;
};

void* operator new(size_t x)
{
    cout << "XX"<<endl;
}
void operator delete(void* x)
{
    cout << "yy"<<endl;
}
void* operator new[](size_t x)
{
    cout << "XX[]"<<endl;
}
void operator delete[](void* x)
{
    cout << "yy[]"<<endl;
}
int main()
{
    void *heap;
     X *x = new X[10]();   
    cout << hex <<"x: " << x <<endl;
    heap = sbrk(0);
    //cout << hex << "a:" << (long) &a <<endl;
    cout << hex << "heap: " << (long) heap <<endl;
    cout << hex << "heap: " << (long)heap - (long)x <<endl;
    int a = 0;
    delete []x;
    while(1);
    return 0;
}
