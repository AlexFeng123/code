#include<iostream>
using namespace std;
#define STR(x)  XXX(#x)

#define XXX(x) \
   decltype(STR(x)) STR(x)

int main()
{
  int x;
  XXX(x);
  return 0;
}
