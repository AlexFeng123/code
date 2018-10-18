#include<iostream>
#include<exception>
using namespace std;
struct Y
{
    Y()try :a(new int[100000000000]()){cout << " yyy" <<endl;}catch(exception &e){ cout << "exception:"<< e.what()<<endl;}
    ~Y(){cout << "y" <<endl;}
    int *a;
};

struct X
{
   Y *m_ptr_y;
   ~X() {cout <<" X " <<endl;}
};

int main() noexcept  
{
    try{
    Y y1;
    X x1;
    }
    catch(exception &e)
    {
       cout << "xxxx"<< e.what()<<endl;
    }
    return 0;
}
