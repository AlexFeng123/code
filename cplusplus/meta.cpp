#include<iostream>
using namespace std;

template<int num,bool isDiv=(num%3==0)>
struct div_3: std::false_type{};

template<int num>
struct div_3<num,true>:std::true_type{};


template<int num,bool isDiv=(num%5==0)>
struct div_5: std::false_type{};

template<int num>
struct div_5<num,true>:std::true_type{};


template<int num,bool div_3_5=(div_3<num>::value||div_5<num>::value)>
struct sum
{
};

template<int num>
struct sum<num,true>
{
    enum{ value = num + sum<num-1>::value};
};

template<int num>
struct sum<num,false>
{
    enum{ value = 0 + sum<num-1>::value};
};
template<>
struct sum<3,true>
{
    enum{ value = 3};
};

int main()
{
   cout << sum<999>::value <<endl;
   return 0;
}
