#include<memory>
#include<iostream>
using namespace std;
int main()
{
    int x=2;
    shared_ptr<int> sp(&x);
    return 0;
}
