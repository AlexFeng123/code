#include<iostream>
#include<type_traits>
#include<memory>
using namespace std;
enum e1 {};
enum class e2: int{};
auto Del=[](int *x)
{
    cout << " Del"<<endl;
    delete x;
};
int main()
{
    int *x = new int(1);
    shared_ptr<int> p(x,Del);
    shared_ptr<int> p1(x,Del);
    p = p1;
    cout << *x<<endl;
    cin>>*x;
    return 0;
}
