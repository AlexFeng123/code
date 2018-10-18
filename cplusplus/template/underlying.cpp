#include<iostream>
#include<type_traits>
using namespace std;
enum e1 {};
enum class e2: int{};


int main()
{
  shared_ptr<int> x(new int(3));
  shared_ptr<int> y(x);
  x = y;
  return 0;
}
