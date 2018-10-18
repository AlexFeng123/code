#include<iostream>
using namespace std;
class X
{
    public:
    explicit X(int y){
    cout <<"x"<<endl;
    }
    explicit X(long a){cout <<"y"<<endl;}
    static  void f() {} 
    static const int x{0};
    const int &y = x;
};

int main()
{
   X a2(X::x);
   int yy =( X::x+1,X::x+3);
   while( yy --> 0)
   {
   cout << yy <<endl;
   } return 0;
}

