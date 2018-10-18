#include<iostream>
using namespace std;
template<int N>
class sumClass{
public:
 enum { ret = N + sumClass<N-1>::ret};
};

template<>
class sumClass<1>{
public:
 enum { ret = 1};
};

int main()
{
  cout<<  sumClass<10>::ret << endl;
  return 0;
}
