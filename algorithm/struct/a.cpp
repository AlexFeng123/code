#include<iostream>
#include<cmath>
#include<limits>
#include<cstdint>
using namespace std;

int main()
{
    uint64_t  x = 0;
    double c = 0;
    cin >> x;
    double x1 = ((-1)+sqrt(1+4*x))/2;
    x = x1;
    uint64_t y = (uint64_t) numeric_limits<uint64_t>::max();
    c = (double) y;
    cout << c <<endl;
    cout << numeric_limits<double>::max() <<endl;
    cout << x;
}
