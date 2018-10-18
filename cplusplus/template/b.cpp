#include<iostream>
using namespace std;
template<typename T>
void print(T const &m){cout <<"b.cpp:"<<m <<endl;}
void fb()
{
    print('2');
    print(0.1);
}

