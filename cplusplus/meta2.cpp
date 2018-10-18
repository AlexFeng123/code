#include<iostream>
using namespace std;

template<bool cond, int T, int F>
struct IF;

template<int T, int F>
struct IF<true,T,F>
{
    constexpr static int value = T;
};


template<int T, int F>
struct IF<false,T,F>
{
    constexpr static int value = F;
};

template<int N , int M>
struct Sum
{
    constexpr static int value = N + M;
};

template<int N , int M>
struct Sub 
{
    constexpr static int value = N - M;
};

template<int N , int M>
struct Mul 
{
    constexpr static int value = N * M;
};

template<int N, int M, int L>
struct Totol
{
  constexpr static int value = IF<Mul<N,M>::value >=L,0+Totol<N-1,M,L>::value,Mul<N,M>::value+Totol<N-1,M,L>::value>::value;
};

template<int M, int L>
struct Totol<1,M,L>
{
  constexpr static int value = IF<Mul<1,M>::value >=L,0,M>::value;
};

int main()
{
    cout << Sub<Sum<Totol<340,3,1000>::value,Totol<200,5,1000>::value>::value,Totol<100,15,1000>::value>::value <<endl;
    return 0;
}



