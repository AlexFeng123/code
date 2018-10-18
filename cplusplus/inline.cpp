#include<iostream>
#include<stdio.h>
using namespace std;

static inline  int64_t factorial(int64_t n)
{
   return n == 0 ? 1 : n * factorial(n-1);  
}

int main()
{
    cout << factorial(10) << endl;
    printf("0x%p",factorial); 
    return 0;
}
