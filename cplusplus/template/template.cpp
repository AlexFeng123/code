#include <iostream>
using namespace std;

template<typename Derived>
struct base{
  void interface()
  {
    static_cast<Derived*>(this)->implement();   
  }
  void implement()
  {
      cout << "base"<<endl;
  }
};

struct a: public base<a>{
  void implement()
  {
      cout << " a "<<endl;
  }
};

struct b: public base<b>{
  void implement()
  {
      cout << " b "<<endl;
  }
};
struct c: public base<c> {};

template<typename T>
void execute(T & base)
{
    base.implement();
}
int main()
{
    a a1;
    b b1;
    c c1;
    execute(a1);
    execute(b1);
    execute(c1);
    return 0;
}
